#include "DelphesAnalysis/Utilities.h"

double read_weight(std::string fileName){
    // string to store the lines
    std::string line;
    // reading the file
    std::ifstream fileToRead(fileName);
    std::stringstream string_to_double;
    double cross_section;

    // loop over the lines
    while (getline(fileToRead, line))
    {
        // find the line that contains the cross section
        if(line.find("#  Integrated weight (pb)  :") != std::string::npos)
        {
            string_to_double << line.substr(line.find(":") + 1, std::string::npos);
            string_to_double >> cross_section;
        }
    }

    fileToRead.close();

    return cross_section;
}