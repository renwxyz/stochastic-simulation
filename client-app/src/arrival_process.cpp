#include "../include/arrival_process.hpp"

#include <cstdlib> // rand(), RAND_MAX
#include <cmath>  // log()

double generate_interarrival_time(double lambda){
    double U = (rand()+1.0)/(RAND_MAX + 1.0);
    return -log(U)/lambda;
}