# arduino-speedometer

<p align="center">
  <br>
  <img src="https://github.com/astrosander/arduino-speedometer/assets/69795340/5e7e7e49-91c6-4f1c-a0e9-743648dda41e" width="160"/><br><br>
 <b>Bike computer based on arduino and Hall Sensor</b><br><br><br>
</p>

## Overview

<p align="left">
  This repository contains the Arduino code and project documentation for a bike computer based on Arduino and a Hall Sensor. The system accurately measures wheel movement to provide valuable bike metrics such as speed, distance covered, time, maximum speed, acceleration, and velocity-time graphs.
</p>

## Demonstration

<p align="center">
  <img src="Media/Animation.gif" width="600"/><br>
  A 3D animation illustrates the bicycle equipped with the hardware.
</p>
<br>


## How it works?

<p align="center">
  <img src="Media/sheme.png" width="600"/><br>
  Wiring diagram depicting the project setup
</p>

<details align="center">
  <summary><h3>Analog Hall Sensor KY-035 Value when the Magnet Passes By</h3></summary>
<img src="https://github.com/astrosander/arduino-speedometer/assets/69795340/3144cc90-7e9d-4b5f-9e38-0c8ade024679" width="600"/><br>
</details>
<br>
<p align="left">
  The system calculates bike metrics by analyzing the time difference between sensor triggers when the magnet passes by it. The setup involves placing a magnet on the front wheel's spokes, triggering a Hall Sensor on the fork. This data is then processed by an Arduino Uno powered by a Xiaomi PowerBank.
</p>

## Compacting..
<p align="center">
  <img src="https://github.com/astrosander/arduino-speedometer/assets/69795340/500bfbef-90af-41b8-a85f-a1c2999036f9" width="300" />
  <img src="https://github.com/astrosander/arduino-speedometer/assets/69795340/6ba9f45c-94db-4a27-9061-403455fd117a" width="300" />
  <img src="https://github.com/astrosander/arduino-speedometer/assets/69795340/b3500a88-1dff-447e-a42e-4ed6ca29e7fc" width="300" />
</p>


<p align="center">
  Close-up view of compacting the bicycle with the device
</p>
<br>
<p align="left">
  The Arduino and power bank are located on the bike frame, the magnet on the bike spoke, and the Hall sensor on the bike fork, secured with hot glue. The LCD display 16x02 (20x04 will work as well) is mounted on the handlebars, right in front of the rider's eyes."
</p>
