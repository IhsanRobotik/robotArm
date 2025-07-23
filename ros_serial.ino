#include <ros.h>
#include <std_msgs/Float64MultiArray.h>
#include <sensor_msgs/JointState.h>
#include <stdlib.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <PID_v1.h>
#include "AS5600.h"

#define joint1Pwm 19
#define joint2Pwm 17
#define joint3Pwm 0
#define joint4Pwm 27
#define joint5Pwm 34
#define joint6Pwm 34

// Define TCA9548A I2C address
#define TCA9548A_ADDRESS 0x70

// directional pin
#define IN1 18
#define IN2 5
#define IN3 16
#define IN4 4
#define IN1_2 2
#define IN2_2 15

AS5600 sensor1; // Instance for the first AS5600 sensor
AS5600 sensor2; // Instance for the second AS5600 sensor
AS5600 sensor3; // Instance for the third AS5600 sensor

int32_t realPos[6] = {0,0,0,0,0,0};

int encoder1;
int mappedEn1;

double Setpoint[6] = {0, 0, 0, 0, 0, 0}; 

double Input[3] = {0, 0, 0};  
double Output[3] = {0, 0, 0};  
double Kp[3] = {0.5, 1, 0.5};
double Ki = 0, Kd = 0;

PID myPID0(&Input[0], &Output[0], &Setpoint[0], Kp[0], Ki, Kd, DIRECT);
PID myPID1(&Input[1], &Output[1], &Setpoint[1], Kp[1], Ki, Kd, DIRECT);
PID myPID2(&Input[2], &Output[2], &Setpoint[2], Kp[2], Ki, Kd, DIRECT);

// Define OLED display dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C

// Create SSD1306 display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ROS NodeHandle
ros::NodeHandle nh;

// Array to store joint angles
float angle[6] = {0, 0, 0, 0, 0, 0};

// Callback function to handle joint state updates
void cmd_cb(const sensor_msgs::JointState& cmd_arm) {
  // Check if the message contains enough positions
  angle[0] = cmd_arm.position[0];
  angle[1] = cmd_arm.position[1];
  angle[2] = cmd_arm.position[2];
  angle[3] = cmd_arm.position[3];
  angle[4] = cmd_arm.position[4];
  angle[5] = cmd_arm.position[5];
}

// Replace `mydata` initialization with:
std_msgs::Float64MultiArray mydata;

// ROS Publisher and Subscriber
ros::Subscriber<sensor_msgs::JointState> sub("/move_group/fake_controller_joint_states", cmd_cb);
ros::Publisher chatter("chatter", &mydata); // Updated type of `mydata`

void setup() {
  // Initialize ROS communication
  nh.getHardware()->setBaud(115200);
  nh.initNode();
  nh.subscribe(sub);
  nh.advertise(chatter);

  // Initialize the multi-array
  mydata.data_length = 6; // Set the length of the array to 6 (number of joints)
  mydata.data = (float*)malloc(mydata.data_length * sizeof(float)); // Dynamically allocate memory

  //select ch3 for oled
  Wire.begin();
  selectTCA9548AChannel(3);
  // Initialize the OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    for (;;);
  }

  // Display a welcome message
  display.clearDisplay();
  display.setTextSize(1);              // Small text size
  display.setTextColor(SSD1306_WHITE); // White text color
  display.setCursor(0, 0);             // Top-left corner
  display.println(F("Initializing..."));
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the display for real-time updates
  display.clearDisplay();

  // Initialize AS5600
  Wire.begin();

  // Initialize sensor1 on channel 0
  selectTCA9548AChannel(0);
  sensor1.begin(4); // Set direction pin
  sensor1.setDirection(AS5600_CLOCK_WISE);

  // Initialize sensor2 on channel 1
  selectTCA9548AChannel(1);
  sensor2.begin(4); // Set direction pin
  sensor2.setDirection(AS5600_CLOCK_WISE);

  // Initialize sensor3 on channel 2
  selectTCA9548AChannel(2);
  sensor3.begin(4); // Set direction pin
  sensor3.setDirection(AS5600_CLOCK_WISE);

  pinMode(joint1Pwm, OUTPUT);
  pinMode(joint2Pwm, OUTPUT);
  pinMode(joint3Pwm, OUTPUT);
  pinMode(joint4Pwm, OUTPUT);
  pinMode(joint5Pwm, OUTPUT);
  pinMode(joint6Pwm, OUTPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(IN1_2, OUTPUT);
  pinMode(IN2_2, OUTPUT);

  myPID0.SetOutputLimits(-255, 255);
  myPID1.SetOutputLimits(-255, 255);
  myPID2.SetOutputLimits(-255, 255);

  myPID0.SetMode(AUTOMATIC);
  myPID1.SetMode(AUTOMATIC);
  myPID2.SetMode(AUTOMATIC);

}

void loop() {
  // Populate the array with joint angles
  for (int i = 0; i < 6; i++) {
    mydata.data[i] = angle[i];
    chatter.publish(&mydata);
  }

  // Read first sensor on channel 0
  selectTCA9548AChannel(0);
  int32_t position1 = sensor1.getCumulativePosition();
  
  // PID
  Input[0] = position1;
  Setpoint[0] = (angle[0] * 652); //convert to degree from rad

  myPID0.Compute();
  analogWrite(joint1Pwm, abs(Output[0]));

  if (Output[0] > 0) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  }

  // Read second sensor on channel 1
  selectTCA9548AChannel(1);
  int32_t position2 = sensor2.getCumulativePosition();
  
  // PID
  Input[1] = position2;
  Setpoint[1] = (angle[1] * 652); //convert to degree from rad

  myPID1.Compute();
  analogWrite(joint2Pwm, abs(Output[1]));

  if (Output[1] > 0) {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  } else {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }

  // Read third sensor on channel 2
  selectTCA9548AChannel(2);
  int32_t position3 = sensor3.getCumulativePosition();
  
  // PID
  Input[2] = position3;
  Setpoint[2] = (angle[2] * 652); //convert to degree from rad

  myPID2.Compute();
  analogWrite(joint3Pwm, abs(Output[2]));

  if (Output[2] > 0) {
    digitalWrite(IN1_2, HIGH);
    digitalWrite(IN2_2, LOW);
  } else {
    digitalWrite(IN1_2, LOW);
    digitalWrite(IN2_2, HIGH);
  }

  realPos[0] = position1;
  realPos[1] = position2;
  realPos[2] = position3;

  // Update OLED display
  updateDisplay();


  // Handle ROS callbacks
  nh.spinOnce();
}

// Function to select TCA9548A channel
void selectTCA9548AChannel(uint8_t channel) {
  if (channel > 7) return; // Only 8 channels (0-7)
  Wire.beginTransmission(TCA9548A_ADDRESS);
  Wire.write(1 << channel); // Select channel
  Wire.endTransmission();
}

void updateDisplay() {
  //select ch3
  selectTCA9548AChannel(3);
  
  // Clear the display buffer
  display.clearDisplay();

  // Display the joint angles
  display.setTextSize(1); // Small text for better fit
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0); // Start at the top-left corner

  // Print joint angles
  for (int i = 0; i < 6; i++) {
    display.print("J");
    display.print(i + 1);
    display.print(": ");
    display.print(Setpoint[i], 2); // Print with 2 decimal places
    display.print(": ");
    display.println(realPos[i]);
  }

  display.println(mappedEn1);

  // Render the buffer to the display
  display.display();
}



