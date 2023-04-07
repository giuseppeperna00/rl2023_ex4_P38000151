# rl2023_ex4_P38000151

This repository contains and urdf file with a basic description of the KUKA IIWA. The launch file robot_display_gui.launch launches rviz to display the robot model with a GUI to edit the joint positions and it also launches a node that implements a service server which returns the transformation between two given frames.

```
$ roslaunch rl2023_ex4 robot_display_gui.launch
```

In the tf_broadcast.cpp file is implemented a service client that constantly sends requests to the server to get the transformation between two specified frames and publishes it.

```
$ rosrun rl2023_ex4 tf_broadcast base_link end_effector
```
