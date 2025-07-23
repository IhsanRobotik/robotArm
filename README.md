# robotArm

### Description
Hobby project I made during the weekend, not fully completed but I proved the concept. The robot currently moves with 3 joints controlled by motors and closed-loop PID using AS5600 I2C magnetic encoders. Mechanical parts are fully 3D printed. Definitely still needs a lot of improvement. I’m planning to redo the project in the future when I have the time, aiming to make a full 6-joint version and switch to stepper motors instead of DC motors and encoders.

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
