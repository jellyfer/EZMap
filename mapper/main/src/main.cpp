#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <unittest++/UnitTest++.h>
#include "/cad2/ece297s/public/m1/tests/unit_test_util.h"
#include "m1.h"
#include "m2_M.h"
#include "m3.h"
#include "m3_additional.h"
#include <string>

using namespace std;

int main(int argc, char** argv) {

    std::string map_path;
    if(argc == 1) {
        //Use a default map
        string str;
        cout <<"Please enter map name, eg. toronto_canada"<<endl;
        getline(cin, str);
        map_path = "/cad2/ece297s/public/maps/"+str+".streets.bin";
        cout << endl;
   /* } else if (argc == 2) {
        //Get the map from the command line
        string str = argv[1];
        map_path = "/cad2/ece297s/public/maps/"+str+".streets.bin";*/
    } else {
        //Invalid arguments
        std::cerr << "Usage: " << argv[0] << " [map_file_path]\n";
        std::cerr << "Default is to enter name of map";
        return 1;
    }

    //Load the map and related data structures
    bool load_success = load_map(map_path);
    if(!load_success) {
        std::cerr << "Failed to load map '" << map_path << "'\n";
        return 2;
    }

    
    std::cout << "Successfully loaded map '" << map_path << "'\n";
    //You can now do something with the map
        
    
    
    
    //Clean-up the map related data structures
    std::cout << "Closing map\n";
    close_map(); 
    
    return 0;
}
