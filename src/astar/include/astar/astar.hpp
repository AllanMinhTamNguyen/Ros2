#pragma once
#include <vector>
#include <utility>

namespace astar {
    struct Node
    {
        int x;
        int y;
        float g_cost;
        float h_cost;
        float cost;

        float f_cost() const {
            return g_cost+h_cost;
        }

        bool operator>(const Node& other) const {
        return f_cost() > other.f_cost();
        }


        int parent_x;
        int parent_y;
    };
    

    class Planner
    {
    private:
        int width_;
        int height_;
        int to_index(int x, int y) const;
        std::vector<Node> get_neighbors (const Node& node, const std::vector<int>& grid);
        float heuristic (int x1, int x2, int y1, int y2) const;

    public:
        Planner(int width, int height);

        std::vector<std::pair<int,int>> find_path(
            int x, 
            int y, 
            int goal_x, 
            int goal_y, 
            const std::vector<int>& grid);
    };
}
