#include "astar/astar.hpp"
#include <queue>
#include <cmath>
#include <algorithm>
#include <unordered_map>

namespace astar {

    Planner::Planner(int width, int height) : width_(width), height_(height) {}
    
    int Planner::to_index(int x, int y) const {
        return y * width_ + x;
    }
    
    float Planner::heuristic(int x1, int y1, int x2, int y2) const {
        return std::abs(x1 - x2) + std::abs(y1 - y2);
    }
    
    std::vector<std::pair<int,int>> Planner::find_path(
        int start_x, int start_y, int goal_x, int goal_y, const std::vector<int>& grid) {
        
        std::priority_queue<Node, std::vector<Node>, std::greater<Node>> open_set;
        std::unordered_map<int, Node> all_nodes;
        std::vector<std::pair<int,int>> path;
        std::unordered_map<int, std::pair<int,int>> came_from; 
        std::vector<bool> closed_set(width_ * height_, false);
        
        Node start_node{start_x, start_y, 0.0f, heuristic(start_x, start_y, goal_x, goal_y), -1, -1}; 
        open_set.push(start_node); 
        all_nodes[to_index(start_x, start_y)] = start_node;
        std::vector<float> g_score(width_*height_, std::numeric_limits<float>::infinity());
        g_score[to_index(start_x,start_y)] = 0;

        while (!open_set.empty()) {
            Node current = open_set.top();
            open_set.pop();

            if (current.x == goal_x && current.y == goal_y) {
                int currx = current.x;
                int curry = current.y;
                
                while (currx != -1 && curry != -1) {
                    path.push_back({currx, curry});
                    int index = to_index(currx, curry);

                    if (came_from.find(index) == came_from.end()) {
                        break;
                    }
                    auto parent = came_from[index];
                    currx = parent.first;
                    curry = parent.second;
                }
                std::reverse(path.begin(), path.end());
                return path;
            }

            int current_index = to_index(current.x, current.y);
            if (closed_set[current_index]) {
                continue;
            }
            closed_set[current_index] = true;
            
            const int dx[4] = {-1, 1, 0, 0};
            const int dy[4] = {0, 0, 1, -1};

            for (int i = 0; i < 4; i++) {       
                int neighbor_x = current.x + dx[i];
                int neighbor_y = current.y + dy[i];

                if (neighbor_x >= 0 && neighbor_x < width_ && neighbor_y >= 0 && neighbor_y < height_) {
                    int next_index = to_index(neighbor_x, neighbor_y);
                    if (grid[next_index] == 0 && !closed_set[next_index]) {
                        float g_cost = current.g_cost + 1.0f;
                        if(g_cost < g_score[next_index]){
                            float h_cost = heuristic(neighbor_x, neighbor_y, goal_x, goal_y);
                            g_score[next_index]= g_cost;
                            came_from[next_index] = {current.x, current.y};
                            open_set.push(Node{neighbor_x, neighbor_y, g_cost, h_cost, current.x, current.y});
                        }
            
                    }
                }
            }
        }
        return {};
    }
}