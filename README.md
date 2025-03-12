# SolarSystem_Simulator

## Author
Mohamed Bouchtout

## Overview
This project implements an Solar System simulation that models the motion of celestial bodies under the influence of gravitational forces, using Newtonian physics. The simulation uses the leapfrog integration method to update positions and velocities over time.

## Features
- Simulates celestial motion based on Newton's laws of motion and gravitation.
- Uses smart pointers to manage memory allocation and avoid leaks.
- Supports command-line arguments for simulation duration and time step.
- Reads the universe configuration from a file.
- Animates the celestial bodies in a graphical window.
- Outputs the final state of the universe in the original format.

## Command Usage
Run the program using:
- ./NBody T Δt < planets.txt

where:
- `T` (double) is the total simulation time.
- `Δt` (double) is the time step for each update.
- `planets.txt` is an input file containing initial conditions.

## Command Example
Command examples to run the simulator
- ./NBody 157788000.0 25000.0 < planets.txt
- ./NBody 120000000.0 20000.0 < customUniverse.txt

## Physics Implementation
1. Compute pairwise gravitational forces.
2. Sum forces to get net force for each body.
3. Compute acceleration using Newton's Second Law.
4. Update velocity using computed acceleration.
5. Update position using the new velocity.
6. Repeat for the specified time steps.

## File Structure
- `main.cpp`: Entry point of the simulation.
- `Universe.cpp`, `Universe.hpp`: Handles simulation logic and time-stepping.
- `CelestialBody.cpp`, `CelestialBody.hpp`: Represents a celestial body with mass, position, velocity, and force calculations.
- `Makefile`: Contains build instructions.
- `planets.txt`: Sample input file with celestial body data.
- `README-ps3.md`: Documentation file (this file).

## Extra Credit Implemented
- Displayed elapsed time on the main screen.
- Created a custom universe file and documented it.

## Compilation and Execution
To compile the project, run:
- make

To clean build artifacts:
- make clean

To run with linting:
- make lint

## Example Output
After running the simulation, the final universe state will be printed in the following format:

[number of bodies]
[universe radius]
[x_position] [y_position] [x_velocity] [y_velocity] [mass] [image_file]

## Notes
- Ensure `planets.txt` follows the required format.
- Small `Δt` values improve accuracy but increase computation time.
- The simulation stops when time `t >= T`.
- Unit tests are included to validate the correctness of the step method.
- You must have SFML and Boost Test libraries downloaded on your machine to run the program.


