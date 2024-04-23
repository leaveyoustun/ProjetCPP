#include "Generic_mapper.hpp"

#include <sstream>
#include <fstream>
#include <vector>

namespace travel {

class Station_Parser : public Generic_mapper {
public:
    Station_Parser() {
    }

    virtual ~Station_Parser() {
    }

public:
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

        while (std::getline(file, line)){
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

                std::getline(data, word);
                station.address = word;

                stations_hashmap[id] = station;

                //std::cout << stations_hashmap[id] << "\n";
            }
            catch(const std::exception& e)
            {
                std::cout << "error for word" <<word << '\n';
            }      
            
        }
    }

    virtual void read_connections(const std::string& _filename) override
    {
        std::ifstream file(_filename);

        //std::cout << file << std::endl;
        if (file.fail()) 
        {
            std::cout << "Failed to open file" << std::endl;
            return;
        }
        std::string line;
        // We skip the header
        std::getline(file, line);

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

                connections_hashmap[from_stop_id][to_stop_id] = min_transfer_time;
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
            
        }


    }
    virtual std::vector<std::pair<uint64_t,uint64_t> > compute_travel(uint64_t _start, uint64_t _end)
    {
        
    }
    virtual std::vector<std::pair<uint64_t,uint64_t> > compute_and_display_travel(uint64_t _start, uint64_t _end)
    {

    }


};
}


int main(){

    travel::Station_Parser dsp;


    dsp.read_connections("data/connections.csv");

    std::unordered_map<uint64_t,std::unordered_map<uint64_t,uint64_t> > a = dsp.get_connections_hashmap();
    std::cout<<"a"<< std::endl;
    std::cout << a[9][10] << std::endl;
    return 0;
}

/*
In order to execute, we need to do:

for compiling:
g++ -std=c++11 -Wall -Wextra -Werror -pedantic -pedantic-errors -O3 -o StationParser StationParser.cpp

to create an executable that can be accessed:
chmod a+x StationParser

and finally to execute:
./StationParser

*/


// Question 5 road map
/*
    Le code compile pas parce que la methode read_connections, qui est une methode virtuelle pure,
    n'a pas encore été définie dans la classe StationParser.
*/


// Question 7  road map
/*
    Pareil, le code compile pas parce que la classe mère Generic_mapper a des méthodes
    virtuelles pures qui n'ont pas encore été rédefinies dans la classe fille.
*/