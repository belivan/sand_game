# Eng_Comp_Individual_Project

## Overview
This is an interactive, physics-based simulation game where players manipulate sand particles in various environments. 
The game combines music, physics, and graphics to create an engaging and dynamic experience. 

## Features
Dynamic Sand Simulation: Utilize the basicSandGenerator and sandGenerator functions to create and manipulate sand particles in real-time.
Music Integration: Choose from a selection of songs (song0 to song4) to play in the background, enhancing the gaming experience.
Game Modes: Play in different modes (MODE) including a demo mode (DEMO) with customizable time limits (DEMO_TIME).
User Interaction: Handle mouse and keyboard inputs to interact with the sand particles and control various aspects of the game.
Statistics Display: Monitor gameplay statistics such as the percentage of sand on the screen and other key metrics.
Customizable Difficulty: Adjust the difficulty level and intensity of sand particle generation to challenge yourself.
Background Image Management: Load and display different background images to vary the visual experience.
Automated Gameplay: Utilize the automatedUser feature for an automatic gameplay experience.

## System Requirements
Compatible with systems running libraries such as yssimplesound, fssimplewindow, ysglfontdata, yspng, and yspngenc.
Requires C++ standard libraries like <stdio.h>, <time.h>, <stdlib.h>, <math.h>, <iostream>, <limits>, <random>, <string>, and <iomanip>.

## Installation
Clone or download the repository containing the game source files.
Ensure all dependent libraries (Grid.h, MySound.h, Physics.h, etc.) are properly installed on your system.
Compile the game using a C++ compiler supporting the required libraries.

## How to Play
Start the game and select a mode.
Use mouse and keyboard inputs to interact with the sand particles.
Choose a background song to play during the game.
Experiment with different sand particle intensities and observe their behavior.
Utilize the display functions to monitor game statistics and performance.

## Customization
Players can adjust the NUMBER_OF_SAND_PARTICLES, LEVEL, AUTO_SPAWN_RATE, and other private variables to customize their gaming experience.
Choose from different songs for varied background music.

## Developer Notes
Ensure all external libraries are properly linked for optimal game performance.

## License
This game is released under MIT license, permitting personal use and modification under the terms of the license.
