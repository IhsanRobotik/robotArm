# robotArm

### Motion Planning with MoveIt (Click to watch)
[![Watch the video](https://img.youtube.com/vi/X_238GgZyBY/0.jpg)](https://youtu.be/X_238GgZyBY)

### Serial Communication Test  
[![Watch the video](https://img.youtube.com/vi/gJDfuzPGHt4/0.jpg)](https://youtu.be/gJDfuzPGHt4)

This repository contains resources for the `robotArm` project, including CAD files, controller code, and ROS nodes.

## Structure

### cobot11_controller/
ROS package handling motion control, hardware interface, and launch configurations.

### listener/
ROS node for subscribing to feedback data or sensor output.

### ros_serial.ino
Arduino sketch for serial communication between ROS and the robot via `rosserial`.
