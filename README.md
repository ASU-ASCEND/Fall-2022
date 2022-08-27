# Fall-2022
All relevant files and documents relating to the ASU ASCEND Fall 2022 mission

## What is ASCEND @ ASU?
ASCEND is a NASA Funded High altitude balloon Launch where students create a payload, collect data while airborne, and analyzed said data to present at the Arizona NASA Space Grant Symposium


## Electrical Subsystem
The electrical subsystem is responsible for connecting the sensors, microprocessors, and batteries properly powering and communicating with one another. These components will fit within the payload’s designated position simultaneously meeting weight requirements.


## Radio Subsystem
The radio subsystem is responsible for keeping a real time link between the payload and the ground station. The radio section on the payload consists of a LoRa module on the main board and a feedline coming from it to and external antenna(also assembled in the lab) located outside the payload. The ground station simply consists of an arduino connected to a computer to output the serial data and a feedline that leads to a directional yagi antenna which we can assemble in the lab. 


## Meteorology Subsystem
The Meteorology Subsystem timeline for the Fall will be as follows: 
Calibrate new temperature and humidity sensors for our expected temperature and relative humidity range (40 C to -75 C, 0% to 100%). Write a MATLAB code to calculate pressure levels with respect to geopotential height using the hypsometric equation. Write a MATLAB code to convert GPS data into wind speed and direction with respect to pressure levels. Run MATLAB codes with obtained data, and complete a sounding analysis. Time permitting, we plan to write a MATLAB code to automatically complete a sounding analysis for us. This will allow us to automate soundings in real time utilizing the Radio subsystem's hard work. We will also export our data from MATLAB to R to create plots. This will serve as a way to teach new members R, and is an important step in presenting our data to non meteorologists. 

The Spring semester will serve as an opportunity to complete research in the atmosphere, as all of our sensors from the Fall semesters will be easily reused. Currently plans are to study the vertical spatial distribution of precursors involved in the formation of surface level ozone and cloud condensation nuclei. Research will be conducted as if we were writing a paper to be published. Input from new members will be strongly considered when designing the timeline for the Spring semester.

The Meteorology Subsystem will hold various workshops throughout the year, open to all ASCEND members. Workshops will be directly relevant to the subsystem's missions, with an emphasis on providing non meteorologists a background sufficient to understand our missions.


## Programming Subsystem
The six sensors will be recording data and need to be programmed to work as intended. The programming subsystem will take charge of the software development side of getting the various sensors (pressure, humidity, temperature) to cooperate with three subsystems Radio, Electrical, and Meteorology.


## Mechanical Subsystem
The mechanical subsystem is responsible for four (4) aspects: a comprehensive CAD model of the payload, the payload design, the construction of the payload, and  the organization of the parts and components within the payload.


## Resource

#### GanttLab
How to use:
1. Go to [GanttLab](https://www.ganttlab.com/)
2. Open GanttLab
3. Create a personal access token
  - Go to [GitHub](https://github.com/)
  - Click on your profile -> Go to settings -> Scroll down to "<> Developer settings"
  - Generate new token -> Name the new token -> Set expiration to desired amount -> Select repo scope -> Generate token
  - Copy token -> Go back to GanttLab -> paste token
4. All set!
