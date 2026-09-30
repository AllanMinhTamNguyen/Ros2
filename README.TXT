ROS2 A* planner

CUstom A* planning algorithm integrated into ROS 2 using C++.
This package ingests nav_msg/OccupancyGrid data and broadcasts the resulting path via nav_msg/Path for RVizz2 visualization

Designed with production-grade engineering in mind, with this repo featuring contained dev environment and an automated CI/DI pipeline

STARTING:
Option 1: Docker/VsCode container
1. Clone the repository
git clone https://github.com/username/astar.git
cd astar
2. open in VsCode
3. click open in container when prompted

USAGE:
1. Start node:
ros2 run astar astar_node

2. Publish mao and Open RVizz2
Open RVizz2

3.Configure RVizz2
set global fixed frame to map
add a Path display and set the topic to /plan

4.Set start and goal
Click 2D pose estimate, click and drag on a grid tile
Click 2d goal pose, clic and drag on a grid tile
Yipee
