##  ----- SNAKE -----
## Starring Boris the Snake as the protagonist as he takes leaps of faith jumping from board to board,
## speedily dodging himself and eating food to replenish his soul reserves. All hail BORIS THE SNAKE
## Embedded Systems Assignment for ENCE260, Semester 2 2017, University of Canterbury.
## Code written by Jozef Crosland and Thomas Morrison
## Designed specifically for the UC Fun Kit 4

-----------------------------------------------------------------------------------------
Game play instructions:
-----------------------------------------------------------------------------------------
Snake has 3 modes to it, which cycle through during game play. These are the title screen, game mode and end screen. The game initially starts on the title screen, awaiting for the navswitches on both boards to be pushed, after which the game begins.

The goal is to eat as many food bits as possible and to avoid running into yourself for as long as possible.
Food bits spawn as a single dot on one screen and will respawn on the alternate screen once it has been eaten. In this way, players need to look at the other to coordinate their movements to move towards the food bits while avoiding collisions. This is especially a challenge when Boris becomes long!

After a collision, the game has ended and the score will display, showing the total number of food bits eaten by both players. The navswitch will await the push on both boards to return to the title screen, where the game begins again. This game cycle continues indefinitely for as long as the players desire. All the score information is reset at the beginning of a new game.

-----------------------------------------------------------------------------------------
Compilation Instructions and Directory Information:
-----------------------------------------------------------------------------------------
All required modules are contained within the 'lib' directory. 'game.c' is the main program the runs the game and is dependent on the said modules in the 'lib' directory. The Makefile is also required to be present in the 'team114' directory. The avr-gcc compiler, which is specific to the AVR Atmel microcontrollers, is required to compile these files into an executable and to load onto the ATmega32U2 microcontroller.

The UC Electrical and Computer Engineering Wikipedia page contains instructions for isntalling the avr-gcc compiler and other required ancillary software here on the 'AVR compilation tools' page:
http://ecewiki.elec.canterbury.ac.nz/mediawiki/index.php/AVR_compilation_tools

You will need to be a staff member of UC or a student to access these, or have permission from them to access this wikipedia page.

In the bash terminal, type 'make' in the 'team114' directory to compile the source code to generate the relocatable object files. Type 'make program' to then compile onto the UC Fun Kit. This will link the object files and generate the hex executable file.

Ensure that the reset button has been pressed on the UCFK so that the device is seen as present in the USB ports on your computer. 

If the Makefile is empty, run the program 'automake.py' with python3 and output the results into the Makefile. DO NOT attempt to modify the modules and / or dependencies, as the game will fail to compile. No Boris D:



