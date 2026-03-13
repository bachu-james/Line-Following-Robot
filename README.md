# Line Following Robot Using PIC16F877A

This project implements an autonomous line-following robot using a `PIC16F877A`, two IR reflective sensors, an `L298N` motor driver, and a `7V Li-ion battery pack`. The firmware is written in Embedded C for `MPLAB XC8`.
## project video link 
video link : https://www.youtube.com/watch?v=wIEhFeruSD0

## Project Summary

The robot follows a black line on a light surface. Two IR sensors observe the path:

- Left sensor on black, right sensor on black: robot moves forward
- Left sensor on black, right sensor on white: robot steers left
- Left sensor on white, right sensor on black: robot steers right
- Both sensors on white: robot briefly searches in the last known turn direction

The code assumes common digital IR sensor modules that output:

- `LOW` when black is detected
- `HIGH` when white is detected

If your sensor board behaves the other way around, change `SENSOR_ON_LINE` and `SENSOR_OFF_LINE` in [src/main.c](/C:/Users/bichu/Desktop/File_V3/PIC_LINEFOLLOWER_ROBOT/src/main.c).

## Hardware Used

- `PIC16F877A` microcontroller
- `L298N` motor driver module
- 2 DC geared motors
- 2 IR line sensors
- 7V Li-ion battery
- Robot chassis and wheels
- 20 MHz crystal with capacitors

## Pin Mapping

### IR sensors

- Left IR sensor output -> `RB0`
- Right IR sensor output -> `RB1`

### L298N control pins

- `ENA` -> `RC2`
- `ENB` -> `RC1`
- `IN1` -> `RD0`
- `IN2` -> `RD1`
- `IN3` -> `RD2`
- `IN4` -> `RD3`

### Power notes

- Power the `PIC16F877A` with regulated `5V`
- Power the motor side of `L298N` from the battery pack
- Connect all grounds together
- Do not drive the PIC directly from the 7V battery without regulation

## Control Logic

| Left Sensor | Right Sensor | Robot Action |
|-------------|--------------|--------------|
| Black       | Black        | Forward      |
| Black       | White        | Turn left    |
| White       | Black        | Turn right   |
| White       | White        | Recover line |

## Firmware File

- Main source: [src/main.c](/C:/Users/bichu/Desktop/File_V3/PIC_LINEFOLLOWER_ROBOT/src/main.c)

## Build Steps In MPLAB X

1. Create a new standalone XC8 project for `PIC16F877A`
2. Add [src/main.c](/C:/Users/bichu/Desktop/File_V3/PIC_LINEFOLLOWER_ROBOT/src/main.c) to the project
3. Select the `XC8` compiler
4. Build the project to generate the `.hex` file
5. Program the PIC using PICkit or simulate it in Proteus

## Embedded Design Notes

- The firmware uses simple digital decisions for fast response and low code size
- `RC1` and `RC2` are used as motor enable outputs and are driven high continuously
- Recovery behavior helps the robot reacquire the line after overshoot
- A short loop delay improves stability and reduces sensor chatter

## Possible Improvements

- Add PWM speed control using `CCP1/CCP2`
- Add three or five sensors for smoother tracking
- Implement PID control for higher speed and accuracy
- Add a start button and low-battery indicator
