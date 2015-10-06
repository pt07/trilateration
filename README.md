# trilateration
Simulation of a GPS system using trilateration to localize the target.
To solve trilateration Ceres-Solver is used to minimize the sum of the squares of the errors of the measurements.

USAGE:
./trilaterate
-r or --receiver double double double : the position of the receiver
-s or --satellite double double double : the position of a satellite
-d or --dev double : the standard deviation of the gaussian white noise added to measurements
-b or --bias : target's time bias

e.g.
./main -r 3 -3 2 -s 15 26 40 -s 1 -32 50 -s -60 -8 70 -s 24 73 56 -s -99 -66 708
