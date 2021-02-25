# Champion
Champion is a comminication manager used to evaluate the messages between the client and the game the user is playing developed using Unix own mechanisms like Pipes, Named Pipes and Threads. It was developed due to a course project, in the Operating Systems class at Instituto Superior de Engenharia de Coimbra

## About the work
The system is composed by a Client application, written in C, which is going to communicate directly with the user, receiveing his information from keyboard input and sending it to the manager as well as receiving information from the manager and displaying it to the user. Beyond the Client, the system has multiple games that are just simple applications, written in C as well, that only receive a input, does his own calculation, evaluates the answer given and updates the score. And finally the system is composed by a Manager that handles all the communication between the many users in the game and the game itsef.
The main purpose of this system is to handle communication between the Client/User and the Server/Game using Unix mechanisms.

## Unix mechanisms addressed 
To communicate from the game to the manager and back, the system uses a system composed by 2 (unanammed) pipes and to communicate from the manager to the client and back the system is composed by 2 pipes, in each end, to read the information in it.
Each client registered on the system has this own manager thread that handles all the work in the manager app.

## Evaluation
Grade: 100%

## Run the project
Everythins could be done in the terminal app
1. Define the Environment Variables: ` . ./env_vars `
2. Compile the project: ` make `
3. Run the manager: ` ./arbitro -d DURATION_OF_CHAMPIONSHIP -w WAIT_TIME_FOR_PLAYERS `
    Both DURATION_OF_CHAMPIONSHIP and WAIT_TIME_FOR_PLAYERS are expressed in minutes
4. Open a new terminal, for each Client you want to run is necessary open a new terminal window/tab
   Just need to launch each one in his terminal ` ./cliente `
