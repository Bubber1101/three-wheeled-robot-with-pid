# three-wheeled-robot-with-pid
SImple code for a robot with an m-board and two motors for locomotion

The robot communicates via Serial and takes commands from the monitor in the form of

### [COMMAND]
or
### [COMMAND][VALUE]
where applicabe.

The interface 

## ***Move*** - M [VALUE]
Value - number of (approximately) centimeters the robot should move (can be negative)

## ***Set speed*** - V [VALUE]
Value - <0 - 255> the PWM value to be put on both motors

## ***Stop*** - S
Short for V 0

## ***Rotate*** - R [VALUE]
Value - Number of degrees the robot should turn, positive - right, negative - left

## ***Read sharp*** - I
Measures distance with Sharp infrared distance meter

## ***Read sonar*** - B
Triggers the sonar and then reads the echo. Calculates the distance based on time and prints the output

## ***Run fuzzy*** - F
Starts the fuzzy control algorithm, the robot will go fast when it's far from any obstacle (measured by Sharp) and gradually slows down and stops when it's near

## ***Run PID*** - P [VALUE]
Sets the desired speed (encoder increments per second) to value and starts the PID algorithm, the Kp,Ki,Kd values are different for each robot
