# 2DTrilateration
Simulation of a GPS system using trilateration to localize the target.
To solve trilateration Ceres-Solver is used to minimize the sum of the squares of the errors of the measurements.

USAGE:
./trilaterate
-t or --target <double> <double> : the position of the target
-b or --beacon <double> <double> : the position of a beacon
-d or --dev <double> : the standard deviation of the gaussian white noise added to measurements

e.g.
./trilaterate -t 5.2 5.3 -b 1.2 2.2 -b 9.9 10 -b 0 8 -b 2 3 -b 6 7 -b 10 1 -b 0 15 -d 1
