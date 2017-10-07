# SGPiApp - Data acquisition and storage of SICK scanner
SGPiApp - Team Suomi Genius (December 2016) - runs on Raspberry Pi 3 to acquire data from SICK Tim561 2D laser scanner and an Arduino that controls and communicates with motor drivers, encoders and IMU (Inertia Measurement Unit). It then stores the data received in a designated binary file as a record of the scanning session.

BUILD To build the program, simply cd to the main directory (containing make file) and type "make" to build the program.

RUN Then run SGPiApp program with appropriate parameters, for example: SGPiApp -v 100 -f road_session0.dat

The arguments for SGPiApp are: -v Desired velocity setpoint for the car -f Output filename of the record

The output file can then be read by SGApp application (link: https://github.com/hamk-automation/SGApp)
