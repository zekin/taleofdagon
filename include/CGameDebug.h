#ifndef CGAMEDEBUG_H
#define CGAMEDEBUG_H

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <regex>
#include "logging.h"

#include <fstream>
#include <iostream>
#include <iterator>
#include <regex>

#include <string>
#include <sstream>


class CGameDebug {
private:
    static int process(std::string line) {
       std::regex formatStartPlayer("^StartPlayer\\ ([0-9]+),([0-9]+)$", std::regex_constants::extended | std::regex_constants::icase);
       std::regex formatLoadMap("^LoadMap\\ ([A-z0-9\\-\\_]+\\.map)$", std::regex_constants::extended | std::regex_constants::icase);
       std::regex formatStateChange("^StateChange\\ (game|mainmenu)$", std::regex_constants::extended | std::regex_constants::icase);
       std::regex formatSpawnUnit("^SpawnUnit\\ ([A-z]+)\\ ([0-9]+),([0-9]+)$", std::regex_constants::extended | std::regex_constants::icase);
       std::regex formatComment("^\\/\\/.*$", std::regex_constants::extended | std::regex_constants::icase);
       std::regex formatWhitespace("^\\s*$", std::regex_constants::extended | std::regex_constants::icase);
       std::stringstream params;

       std::match_results<std::string::const_iterator> matches;

       if (std::regex_search(line, matches, formatStartPlayer)) {
           int x=0,y=0;

           params << matches[1] << " " << matches[2]; 
           params >> x >> y;
           
           DEBUG(LOG) << "StartPlayer (" << x << ", " << y << ")";

           return 0;
       } else if (std::regex_search(line, matches, formatLoadMap))  {
           std::string mapName(matches[1]);

           DEBUG(LOG) << "LoadMap (" << mapName << ")";
           
           return 0;
       } else if (std::regex_search(line, matches, formatStateChange)) {
           std::string stateName(matches[1]);
           
           DEBUG(LOG) << "StateChange (" << stateName << ")";
           return 0;
       } else if (std::regex_search(line, matches, formatSpawnUnit)) {
           std::string unitType(matches[1]);
           int x=0, y=0;
           params << matches[2] << " " << matches[3];
           params >> x >> y;

           DEBUG(LOG) << "SpawnUnit (" << unitType << ": " << x << ", " << y << ")" << '\n';
           return 0;
       } else if (std::regex_search(line, matches, formatComment)) {
           DEBUG(LOG) << "This is a comment";
           return 0;
       } else if (std::regex_search(line, matches, formatWhitespace)) {
           DEBUG(LOG) << "This is whitespace"; 
           return 0;
       }
       
       return 1;
    } 

    static int runfile(std::string filename) {
        std::fstream file("./debug/"+filename, std::ios_base::in);
        std::string line;

        if (!file.is_open()) {
            ERROR(LOG) << "Failed to open file.";
            return -1;
        }
        while (std::getline(file, line)) {
            if (process(line) == 1) {
                ERROR(LOG) << "Failed to process file " << filename << ", line " << line << " not accepted.";
            }
        }
            
        file.close(); 
        return 0;
    }
public:
    CGameDebug(std::string filename) {
        runfile(filename);
    }

    static int LoadMap(std::string map_name) {
        return 0;
    }
    static int PlayerStart(int x, int y) {
        return 0; 
    }
    static int StateChange(std::string state) {
        return 0;
    }
    static int SpawnUnit(std::string type, int x, int y) {
        return 0;        
    }
};

#endif