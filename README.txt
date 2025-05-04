C++ PROJECT 

Author: STAN Liviu-Mihai
Student Number: 21211659
Formation: M1 ISI

The code was fully written in VS Code

In order to execute the code, we will need to do the following:

for compiling:
g++ -std=c++11 -Wall -Wextra -Werror -pedantic -pedantic-errors -O3 -o Itinerary_Calculator Itinerary_Calculator.cpp

to create an executable that can be accessed:
chmod a+x Itinerary_Calculator

and finally to execute:
./Itinerary_Calculator data/name_of_stations_file.csv data/name_of_connections_file.csv


If you choose to test the smaller datasets (stations.csv and connections.csv) you will also be prompted to say if you want or not to see the read_stations and read_connections methods demonstration. This will print in the console all the stations passed in the stations_hashmap and all the connections passed in the connections_hashmap. I did not include this demonstration for the larger datasets since they would be extremely long to read.

For each of the datasets, the code will ask you for a departure station and an arrival station. To see the itinerary, you can directly write the name of the respective station, no need to write the ID.

Small remark: when you pass the datasets in the parameters to run the code, be mindful of where you have the files in your computer. For example, in my project folder, there is another folder (« data ») that stores the 4 datasets. If for you it’s the same, you will also need to include « data/ » before the name of the file, like I wrote in the execution example.


Answers for the questions 2,5 and 8 from the road map:

Question 2 road map

The code does not compile because the read_stations method, which is a pure virtual method, was not yet redefined in the Itinerary_Calculator class.

Question 5 road map

The code does not compile because the read_connections method, which is a pure virtual method, was not yet redefined in the Itinerary_Calculator class.


Question 7 road map

Same as before, the code doesn't compile because the mother class Generic_mapper has pure virtual methods that were not yet redefined in the daughter class.
