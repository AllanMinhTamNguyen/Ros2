#include <gtest/gtest.h>
#include "/home/allan/projects/astar/include/astar.hpp"

// Test 1: Can it find a path across an empty room?

TEST(AStarTest, EmptyGrid) {
    astar::Planner planner(5,5);
    std::vector<int> grid (25,0);

    auto path = planner.find_path(0,0,4,4, grid);

    ASSERT_FALSE(path.empty());
    EXPECT_EQ(path.front().first, 0);
    EXPECT_EQ(path.front().second, 0);
    EXPECT_EQ(path.back().first, 4);
    EXPECT_EQ(path.back().second,4);
}


// Test 2: Can it navigate around an obstacle?
TEST(AStarTest, NavigatesAroundWall) {
    astar::Planner planner(5, 5);
    std::vector<int> grid(25, 0);

    // Build a solid wall down the middle column (x = 2), but leave a gap at the bottom (y = 4)
    grid[0 * 5 + 2] = 1;
    grid[1 * 5 + 2] = 1;
    grid[2 * 5 + 2] = 1;
    grid[3 * 5 + 2] = 1;

    // Try to go from top-left to top-right
    auto path = planner.find_path(0, 0, 4, 0, grid);

    ASSERT_FALSE(path.empty());
    
    // Ensure the algorithm actually walked through the gap and didn't cheat through the wall
    for (const auto& node : path) {
        if (node.first == 2) {
            EXPECT_EQ(node.second, 4); // The only time X=2 is allowed is at Y=4
        }
    }
}

// Test 3: Does it safely give up if the goal is trapped?
TEST(AStarTest, ImpossiblePathReturnsEmpty) {
    astar::Planner planner(3, 3);
    std::vector<int> grid(9, 0);

    // Box the start node in completely
    grid[0 * 3 + 1] = 1; // Wall to the East
    grid[1 * 3 + 0] = 1; // Wall to the South
    
    auto path = planner.find_path(0, 0, 2, 2, grid);

    EXPECT_TRUE(path.empty()); // The path must be empty because the goal is unreachable
}