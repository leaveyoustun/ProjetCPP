#include "Generic_mapper.hpp"

#include <sstream>
#include <fstream>
#include <vector>
#include <limits>
#include <queue>
#include <string>
#include <algorithm> 

// Exception classes 

// Mother class
class Exceptions 
{
    public:
    Exceptions (std::string error) throw(): _error(error) {}; 
    virtual ~Exceptions() throw() {};
    virtual const char * what() const throw() 
    {
        return this->_error.c_str();
    };
    protected:
    std::string _error; 
};


// Class for compilation errors
class BadCompile: public Exceptions 
{
    public:
    BadCompile (std::string error) throw(): Exceptions(error) {}; 
    virtual ~BadCompile() throw() {};
    virtual const char * what() const throw() 
    {
        std::cerr << "Compilation error!"<< std::endl;
        return this->_error.c_str();
    }
};


// Class for station errors
class BadStations: public Exceptions{
    public:
    BadStations(std::string error) throw(): Exceptions(error){};
    virtual ~BadStations() throw() {};
    virtual const char * what() const throw() 
    {
        std::cerr << "Stations error!"<< std::endl;
        return this->_error.c_str();
    }
};

// Structure used for the comparison of the members of the priority queue
struct Compare {
    bool operator()(const std::pair<uint64_t, uint64_t>& a, const std::pair<uint64_t, uint64_t>& b) {
        return a.second > b.second;  
    }
};

// Some functions we will use for the code

// Function that will freeze the screen until ENTER is pressed
void WaitEnter()
{
    std::string input;
    std::getline(std::cin, input);
}

// Function used to convert a string in lower case
std::string to_lower(const std::string& str) {
    std::string lower_str = str;
    std::transform(lower_str.begin(), lower_str.end(), lower_str.begin(),
                [](unsigned char c){ return std::tolower(c); });
    return lower_str;
}

// Function that will convert the strings 'y' and 'n' into true and false
bool to_bool(std::string const &input) 
{ 
        std::string lower_input = to_lower(input);
        if (lower_input == "y") return true;
        if (lower_input == "n") return false;
        throw BadCompile("Boolean error - please enter 'y' or 'n'.");
}   


namespace travel {


// Implementation of the Itinerary_Calculator class
class Itinerary_Calculator : public Generic_mapper {
public:
    // Constructor
    Itinerary_Calculator() {
    }

    // Destructor
    virtual ~Itinerary_Calculator() {
    }

    // Method used to call the read_stations and read_connections methods
    void load_data(const std::string& stationsFile, const std::string& connectionsFile) {
        this->read_stations(stationsFile);
        this->read_connections(connectionsFile);
    }

    // Method that will calculate the shortest itinerary from one station to another, using Djikstra's algorithm
    virtual std::vector<std::pair<uint64_t,uint64_t> > compute_travel(uint64_t _start, uint64_t _end) override
    {
        std::priority_queue<std::pair<uint64_t, uint64_t>, std::vector<std::pair<uint64_t, uint64_t>>, Compare> pq;
        std::unordered_map<uint64_t, uint64_t> distances;
        std::unordered_map<uint64_t, uint64_t> previous_vertex;
        std::vector<std::pair<uint64_t, uint64_t>> path;

        // We initialize distances to infinity and the distance to the start node as 0
        for (const auto& conn : this->connections_hashmap) {
            distances[conn.first] = std::numeric_limits<uint64_t>::max();
        }
        distances[_start] = 0;

        // We push the start node into the priority queue
        pq.push({0, _start});

        
        while(!pq.empty()){

            // We take the vertex at the top of the priority queue, so the closest one
            uint64_t dist = pq.top().first;
            uint64_t node = pq.top().second;
            pq.pop();

            // We calculate the distance to each of it's neighbors
            for (const auto& adj : this->connections_hashmap[node]) {
                uint64_t adjNode = adj.first;
                uint64_t weight = adj.second;

                // We check if a shorter path is found
                if (dist + weight < distances[adjNode]) {
                    distances[adjNode] = dist + weight;
                    previous_vertex[adjNode] = node;
                    pq.push({distances[adjNode], adjNode});
                }
            }
        }

        // We reconstruct the path from end to start
        for (uint64_t at = _end; at != _start; at = previous_vertex[at]) {
            if (distances[at] == distances[previous_vertex[at]])
                continue;
            path.push_back({at, distances[at]});
            }
        
        path.push_back({_start, distances[_start]});
        std::reverse(path.begin(), path.end());

        return path;
    }

    // Method that will calculate the shortest itinerary from one station to another and will display it in the console
    virtual std::vector<std::pair<uint64_t,uint64_t> > compute_and_display_travel(uint64_t _start, uint64_t _end) override
    {
        // We use the last method to calculate the itinerary
        auto path = this->compute_travel(_start, _end);

        // We show it for the user
        std::cout << "Best way from " << this->stations_hashmap[_start].name << " (line " << this->stations_hashmap[_start].line_name << ") to " << this->stations_hashmap[_end].name << " (line " << this->stations_hashmap[_end].line_name << ") is :" << std::endl << std::endl;
        
        for (size_t i = 1; i < path.size(); i++) 
        {
            // Current station
            uint64_t station_id = path[i].first;

            // Previous station
            uint64_t previous_station_id = path[i-1].first;
            uint64_t previous_station_time = path[i-1].second;

            // We calculate the time
            uint64_t time = path[i].second - previous_station_time;

            // We check if the current station has the same name as the previous station, so that we can tell the user to walk to the next line
            if (this->stations_hashmap[station_id].name == this->stations_hashmap[previous_station_id].name) 
            {
                std::cout << "Walk to " << this->stations_hashmap[station_id].name
                            << ", line " << this->stations_hashmap[station_id].line_name
                            << " (" << time << " seconds)" << std::endl;
            } else 
            {
                // If it's a different station, but the same line, we do not need to show each step of the algorithm
                // We will only show the station we get on the subway and the station we descend at
                while (this->stations_hashmap[station_id].line_name == this->stations_hashmap[previous_station_id].line_name &&
                i<path.size()-1 && this->stations_hashmap[path[i+1].first].line_name == this->stations_hashmap[station_id].line_name)
                {
                    i++;
                    station_id = path[i].first;
                }
                // We recalculate the time
                time = path[i].second - previous_station_time;

                // We show the result
                std::cout << "Take line " << this->stations_hashmap[previous_station_id].line_name << " "
                            << this->stations_hashmap[previous_station_id].line_desc << std::endl
                            << "From " << this->stations_hashmap[previous_station_id].name
                            << " to " << this->stations_hashmap[station_id].name
                            << " (" << time << " seconds)" << std::endl;
            }
        }
        // We show the total time
        std::cout << std::endl << "After " << path.back().second << " seconds, you have reached your destination!" << std::endl;
        return path;
    }


    // Function to calculate the Levenshtein distance between two strings
    int levenshtein_distance(const std::string& s1, const std::string& s2) 
    {
        const size_t len1 = s1.size(), len2 = s2.size();
        std::vector<std::vector<unsigned int>> d(len1 + 1, std::vector<unsigned int>(len2 + 1));

        d[0][0] = 0;
        for(unsigned int i = 1; i <= len1; ++i) d[i][0] = i;
        for(unsigned int i = 1; i <= len2; ++i) d[0][i] = i;

        for(unsigned int i = 1; i <= len1; ++i)
            for(unsigned int j = 1; j <= len2; ++j)
                d[i][j] = std::min(
                {
                    // Deletion cost
                    d[i - 1][j] + 1,

                    // Insertion cost  
                    d[i][j - 1] + 1,  

                    // Substitution cost
                    d[i - 1][j - 1] + (s1[i - 1] == s2[j - 1] ? 0 : 1)  
                });

        return d[len1][len2];
    }

    

    // Function that will find the closest station name based on Levenshtein distance
    uint64_t find_closest_station(const std::string& input) 
    {
        uint64_t closest_id = 0;
        int min_distance = std::numeric_limits<int>::max();

        // We look for the closest station 
        for (const auto& station_pair : this->stations_hashmap) 
        {
            int distance = levenshtein_distance(to_lower(input), to_lower(station_pair.second.name));
            if (distance < min_distance) 
            {
                min_distance = distance;
                closest_id = station_pair.first;
            }
        }

        // If the station the user typed (case ignored) is not an actual station in the dataset, we ask
        // the user if the closest station we found is actually the one they meant to type
        if(to_lower(input) != to_lower(this->stations_hashmap.at(closest_id).name))
        {
            std::cout<< input <<" station was not found, did you mean " << this->stations_hashmap.at(closest_id) << "? (y/n)";
            std::string character;
            std::cin >> character;
            bool boolean(to_bool(character));
            if(boolean)
            {
                system("clear");
                return closest_id;
            }
            else
            {
                system("clear");
                throw BadStations("Station not found");
            }
        }
        else
        {
            return closest_id;
        }
    }

    // Method that will calculate the itinerary between two stations passed in the parameters, but this time they are strings, and not the station IDs
    virtual std::vector<std::pair<uint64_t,uint64_t> > compute_travel(const std::string& start_name, const std::string& end_name) override
    {
        // We convert station names to IDs, handling potential errors in name entry
        uint64_t start_id = this->find_closest_station(start_name);
        uint64_t end_id = this->find_closest_station(end_name);

        // We test if the same station was entered
        if(start_id == end_id)
        {
            system("clear");
            throw BadStations("You entered the same station for departure and arrival!");
        }

        if (start_id == 0 || end_id == 0) 
        {
            std::cout << "Error: One or more station names could not be matched." << std::endl;
            return {};
        }

        return this->compute_travel(start_id, end_id);
    }

    // Method that will calculate and display the itinerary between two stations passed in the parameters, but this time they are strings, and not the station IDs
    virtual std::vector<std::pair<uint64_t,uint64_t> > compute_and_display_travel(const std::string& start_name, const std::string& end_name) override
    {
        // We convert station names to IDs, handling potential errors in name entry
        uint64_t start_id = this->find_closest_station(start_name);
        uint64_t end_id = this->find_closest_station(end_name);

        // We test if the same station was entered
        if(start_id == end_id)
        {
            system("clear");
            throw BadStations("You entered the same station for departure and arrival!");
        }

        if (start_id == 0 || end_id == 0) 
        {
            std::cout << "Error: One or more station names could not be matched." << std::endl;
            return {};
        }

        return this->compute_and_display_travel(start_id, end_id);
    }

// Protected methods read_stations and read_connections
protected:

    // Method that will read the data in the stations dataset and put it in the this->stations_hashmap
    void read_stations(const std::string& _filename) override {
        std::ifstream file(_filename);

        if (!file.is_open()) 
        {
            std::cerr << "Failed to open file" << std::endl;
            return;
        }
        std::string line;

        // We skip the header
        std::getline(file, line);

        // We store the inputs
        while (std::getline(file, line))
        {
            std::istringstream data(line);
            std::string word;
            uint64_t id;

            Station station;

            try
            {
                std::getline(data, word, ',');
                station.name = word;

                std::getline(data, word, ',');
                station.line_id = word;
                id = std::stoi(word);

                std::getline(data, word, ',');
                station.line_name = word;

                std::getline(data, word, ',');
                station.address = word;

                std::getline(data, word);
                station.line_desc = word;

                this->stations_hashmap[id] = station;

            }
            catch(const std::exception& e)
            {
                std::cout << "error for word" <<word << '\n';
            }      
        }
    }

    // Method that will read the data in the connections dataset and put it in the this->connections_hashmap
    virtual void read_connections(const std::string& _filename) override
    {
        std::ifstream file(_filename);

        if (file.fail()) 
        {
            std::cout << "Failed to open file" << std::endl;
            return;
        }
        std::string line;

        // We skip the header
        std::getline(file, line);

        // We store the inputs
        while (std::getline(file, line))
        {
            std::istringstream data(line);
            std::string word;
            
            uint64_t from_stop_id;
            uint64_t to_stop_id;
            uint64_t min_transfer_time;
            std::unordered_map<uint64_t, uint64_t> transfer_time;
            try
            {
                std::getline(data,word,',');
                from_stop_id = std::stoi(word);
                
                std::getline(data,word,',');
                to_stop_id = std::stoi(word);

                std::getline(data,word);
                min_transfer_time = std::stoi(word);

                this->connections_hashmap[from_stop_id][to_stop_id] = min_transfer_time;
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
        }
    }
};
}


int main(int argc, char* argv[]) 
{
    
    try{
        // We test for compilation errors

        // If not enough parameters were written
        if (argc < 3) 
        {
            system("clear");
            std::cerr << "Usage: " << argv[0] << " <stations file> <connections file>" << std::endl;
            throw BadCompile("Not enough arguments");
        }

        // If too many parameters were written
        if (argc > 3)
        {
            system("clear");
            std::cerr << "Usage: " << argv[0] << " <stations file> <connections file>" << std::endl;
            throw BadCompile("Too many arguments");
        }

        // We convert the parameters to strings
        std::string arg1(argv[1]);
        std::string arg2(argv[2]);

        // If the small datasets were used
        if(arg1 =="data/stations.csv" && arg2 == "data/connections.csv")
        {
            // We instanciate the class
            travel::Itinerary_Calculator traveler;

            // We load the data
            traveler.load_data(arg1, arg2);

            // We show the first screen, and ask if the user would like to see the methods demonstrations
            system("clear");
            std::cout<< "Hello! \n\nWould you like to see the demonstration of read_stations and read_connections? (y/n)";
            std::string character;
            std::getline(std::cin, character);
            bool boolean(to_bool(character));
            

            // If the user said yes, we show the demonstration
            system("clear");
            if (boolean == true)
            {
                std::cout << "Stations:\n\n";
                const std::unordered_map<uint64_t, travel::Station> stations = traveler.get_stations_hashmap();

                for (const auto& pair : stations) 
                {
                    std::cout << pair.second << std::endl;
                }

                std::cout << "\nConnections:\n\n";
                std::unordered_map<uint64_t,std::unordered_map<uint64_t,uint64_t> > connections = traveler.get_connections_hashmap();
                
                for (const auto& from_pair : connections) 
                {
                    uint64_t from_id = from_pair.first;
                    const auto& destinations = from_pair.second;
                    for (const auto& to_pair : destinations) 
                    {
                        uint64_t to_id = to_pair.first;
                        uint64_t time = to_pair.second;
                        std::cout << "From " << stations.at(from_id)<< " to " << stations.at(to_id)<< " in " << time << " seconds\n";
                    }
                }

                // We wait for the user to go to the second screen
                std::cout<< "\n\nPress ENTER to go to the itinerary calculation!";
                WaitEnter();
            }

            

            // We show the second screen
            system("clear");
            std::cout<< "Hello! \n\nWhat itinerary would you like to know?\n\nStation of departure: ";

            std::string from_station;
            std::cin >> from_station;

            std::cout<<"Station of arrival: ";
            std::string to_station;
            std::cin >>to_station;

            // We check for typing errors
            if (from_station == "" || to_station == "")
            {
                system("clear");
                throw BadStations("At least one station was not entered!");
            }

            // We show the itinerary
            system("clear");
            std::vector<std::pair<uint64_t,uint64_t> > path;
            path = traveler.compute_and_display_travel(from_station, to_station);
        }

        // If the big datasets were used
        else if(arg1 =="data/s.csv" && arg2 == "data/c.csv")
        {
            // We instanciate the class
            travel::Itinerary_Calculator traveler;

            // We load the data
            traveler.load_data(arg1, arg2);

            // We show the first screen
            system("clear");
            std::cout<< "Hello! \n\nWhat itinerary would you like to know?\n\nStation of departure: ";

            std::string from_station;
            std::getline(std::cin, from_station, '\n');

            std::cout<<"Station of arrival: ";
            std::string to_station;
            std::getline(std::cin, to_station, '\n');

            // We check for typing errors
            if (from_station == "" || to_station == "")
            {
                system("clear");
                throw BadStations("At least one station was not entered!");
            }
            
            // We show the itinerary
            system("clear");
            std::vector<std::pair<uint64_t,uint64_t> > path;
            path = traveler.compute_and_display_travel(from_station, to_station);

        }

        // We check for compilation errors
        else
        {
            system("clear");
            throw BadCompile("ERROR: The names of the data sets are not correct!");
        }
    }
    

    // Dealing with exceptions
    catch(const BadCompile &a)
    {
        std::cerr << a.what() << std::endl;

    }
    catch(const BadStations &a)
    {
        std::cerr << a.what() << std::endl;

    }
    
    
    return 0;
}

/*
In order to execute, we need to do:

for compiling:
g++ -std=c++11 -Wall -Wextra -Werror -pedantic -pedantic-errors -O3 -o Itinerary_Calculator Itinerary_Calculator.cpp

to create an executable that can be accessed:
chmod a+x Itinerary_Calculator

and finally to execute:
./Itinerary_Calculator data/name_of_stations_file.csv data/name_of_connections_file.csv
*/

// Question 2 road map
/*
    The code does not compile because the read_stations method, which is a pure virtual method,
    was not yet redefined in the Itinerary_Calculator class.
*/

// Question 5 road map
/*
    The code does not compile because the read_connections method, which is a pure virtual method,
    was not yet redefined in the Itinerary_Calculator class.
*/

// Question 7 road map
/*
    Same as before, the code doesn't compile because the mother class Generic_mapper has pure
    virtual methods that were not yet redefined in the daughter class.
*/