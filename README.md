# **ECE590 Final Project**

This repository serves for the purpose of the final project for ECE 590 Embeded System Class.

The goal of this project is to demonstrate the depth of our learning result from the class.

### **Project description**:
  Extend the robot finite state machine example to include a simulation of the robot in a 2D environment, a simulation of an intruder,and a simulation of the recharge station. 

All states must have the correct transition to other states with emitting the correspond events. 
At first the project should show transitions, states, and events by printing out to the screen, then, at the end of the project, it should be able to show everything on a 2-D environment with simulated robots, host, charging station, and intruders. With real-time states, events, and transitions printed out. 
This project will gave me a depth view of the Finite State Mahine with understanding and mastering how the staets change and how events trigger transitions and generated at the process of transition. 
Ideally, achieving 2-D view of the result would count as a success for this project. But this is subjective to the difficulty to the technical part and time devoted on this project. Very likely, I might change the final goal for this project.

Resources will be using:
  ECE590 elma file, 2-D drawing software(TBD)

**Five Milestone**
  - Having something that is executable (the robot skeleton.) Should be finished at the first week.
  - Add additional feature to the robot: differnt events, states, and transistions. Should be finished at the first week.
  - Export data so user could visualize it. Should be finished at the second week.
  - Find a user friendly 2-D drawing software. Should be finished at the second week.
  - (Extra) Exporting the code onto a real physical robot with different sensors trigger different events. This extra step is entirely         depends on how much time I have left. 

-------
# **Installing the Package**

We need to git clone everything into local directory. Then compile them in two steps. First, under ElmaEssentials we will compile elma package for required lib files. Second, go into robot_package directory to compile code for the robot. 
```
git clone https://github.com/kenanlv/ECE590_FP.git

cd ECE590_FP /ElmaEssentials

make

cd robot_package

make

./bin/test
```
bin/test will run all tests for robot package. 

-------------------------------
# Overview 
The whole package is contained three main parts: Robot, Map, and Intruder.
#### Robot
The robot class is essentially a finite state machine. With differnt events, different states' transistions will be triggered. It has the feature of wandering around the map, detecting intruders, making noise while finding the intruder if it sees one, eliminate the intruder and going back to wander state. It also has a battery feature. If the robot has gone for a certain range, it needs to go back to the charging station for recharge the battery. After its full charged, it will enter the wander state again. 

#### Map
Map class serves for the 2-D visulization of the entire project. It will print out the map with fixed position of charging station as initilization of the map. Then it will generate and print out robot and intruders in random position when it sees a cooresponding event. Map class will also watch for events emit by the robot, so that it can emit other events to deliver event propagation. Map class is also in charge of the movement of robot. And print the updated map if anything changed. 

#### Intruder
Intruder class will generate an intruder in a given time span. Map will randomly generate an intruder on the map, then robot will approach the intruder when it sees an intruder. After robot is near the intruder, it will eliminate the intruder from the map. 

----------------------------
# **Exapmles**

**Initilization of Map:** 
0 represents for charging station, and it will stays at the same place all the time.

<img src=Images/map_init.PNG width="300" height="300">

















**Wander And Generation of Robot:** 
Map will generate robot ("^") in random position, then robot goes into wander state. Meaning that robot will randomly go up, down, left, or right. ">" sign will print out some indicator for users to understand what the robot is trying to do next. 

<img src=Images/map_gen_robot.PNG width="300" height="300">

<img src=Images/map_gen_robot_2.PNG width="300" height="300">


























**Intruder Detection and Elimination:** 
After simulator being runing for certain time. Intruder class will emit an evnet that will trigger map to generate an intruder ("X") in a random position. Robot will immediately move towards intruder when it sees an intruder while making noise.

<img src=Images/rob_noise.PNG width="300" height="300">
<img src=Images/rob_noise_2.PNG width="300" height="300">




























**Elimination and Regarge:**
After robot eliminate the intruder, it goes back to the wander state. When it rans out of battery, it will navigate back to the charging station for recharge. 

<img src=Images/rob_rechar.PNG width="300" height="300">
<img src=Images/rob_rechar_2.PNG width="300" height="300">
<img src=Images/rob_rechar_3.PNG width="300" height="300">














































-----------------------------
