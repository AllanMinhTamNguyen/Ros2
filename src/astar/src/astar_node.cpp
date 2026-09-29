#include <rclcpp/rclcpp.hpp>
#include <nav_msgs/msg/occupancy_grid.hpp>
#include <nav_msgs/msg/path.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <geometry_msgs/msg/pose_with_covariance_stamped.hpp>
#include "astar/astar.hpp"

using std::placeholders::_1;

class AStarNode : public rclcpp::Node {
public:
    AStarNode() : Node("astar_node"), map_ready_(false), start_ready_(false) {
        map_sub_ = this->create_subscription<nav_msgs::msg::OccupancyGrid>(
            "/map", 10, std::bind(&AStarNode::map_callback, this, _1));
            
        start_sub_ = this->create_subscription<geometry_msgs::msg::PoseWithCovarianceStamped>(
            "/initialpose", 10, std::bind(&AStarNode::start_callback, this, _1));
            
        goal_sub_ = this->create_subscription<geometry_msgs::msg::PoseStamped>(
            "/goal_pose", 10, std::bind(&AStarNode::goal_callback, this, _1));

        path_pub_ = this->create_publisher<nav_msgs::msg::Path>("/plan", 10);
        RCLCPP_INFO(this->get_logger(), "A* Node Started. Waiting for /map...");
    }

private:
    void map_callback(const nav_msgs::msg::OccupancyGrid::SharedPtr msg) {
        map_msg_ = *msg;
        grid_.clear();
        for (int8_t val : map_msg_.data) {
            grid_.push_back((val > 50 || val == -1) ? 1 : 0);
        }
        planner_ = std::make_shared<astar::Planner>(map_msg_.info.width, map_msg_.info.height);
        map_ready_ = true;
        RCLCPP_INFO(this->get_logger(), "Map received.");
    }

    void start_callback(const geometry_msgs::msg::PoseWithCovarianceStamped::SharedPtr msg) {
        start_pose_ = msg->pose.pose;
        start_ready_ = true;
        RCLCPP_INFO(this->get_logger(), "Start locked.");
    }

    void goal_callback(const geometry_msgs::msg::PoseStamped::SharedPtr msg) {
        if (!map_ready_ || !start_ready_) return;
        int start_x = (start_pose_.position.x - map_msg_.info.origin.position.x) / map_msg_.info.resolution;
        int start_y = (start_pose_.position.y - map_msg_.info.origin.position.y) / map_msg_.info.resolution;
        int goal_x = (msg->pose.position.x - map_msg_.info.origin.position.x) / map_msg_.info.resolution;
        int goal_y = (msg->pose.position.y - map_msg_.info.origin.position.y) / map_msg_.info.resolution;
        auto path = planner_->find_path(start_x, start_y, goal_x, goal_y, grid_);
        nav_msgs::msg::Path path_msg;
        path_msg.header.stamp = this->now();
        path_msg.header.frame_id = "map";

        for (const auto& point : path) {
            geometry_msgs::msg::PoseStamped pose;
            pose.pose.position.x = (point.first * map_msg_.info.resolution) + map_msg_.info.origin.position.x;
            pose.pose.position.y = (point.second * map_msg_.info.resolution) + map_msg_.info.origin.position.y;
            path_msg.poses.push_back(pose);
        }
        path_pub_->publish(path_msg);
        RCLCPP_INFO(this->get_logger(), "Path published to /plan");
    }

    rclcpp::Subscription<nav_msgs::msg::OccupancyGrid>::SharedPtr map_sub_;
    rclcpp::Subscription<geometry_msgs::msg::PoseWithCovarianceStamped>::SharedPtr start_sub_;
    rclcpp::Subscription<geometry_msgs::msg::PoseStamped>::SharedPtr goal_sub_;
    rclcpp::Publisher<nav_msgs::msg::Path>::SharedPtr path_pub_;

    nav_msgs::msg::OccupancyGrid map_msg_;
    geometry_msgs::msg::Pose start_pose_;
    bool map_ready_, start_ready_;
    std::vector<int> grid_;
    std::shared_ptr<astar::Planner> planner_;
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<AStarNode>());
    rclcpp::shutdown();
    return 0;
}