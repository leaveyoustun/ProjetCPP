#include <iostream>
#include "StationParser.cpp"

int main() {
    travel::Station_Parser dsp;


    dsp.read_stations("data/stations.csv");

    return 0;
}