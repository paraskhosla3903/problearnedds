/**
 * @file main.cpp
 * @author Paras Khosla (paraskhosla3903@gmail.com)
 * @brief Defines the flow of the program
 * @version 0.1
 * @date 2025-02-24
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "commons.h"

#include <iostream>

using namespace pds;

int main(int argc, char** argv)
{
    if(argc != 2){
        std::cerr << "Usage: " << argv[0] << " <configFilePath>" << "\n";
        return 1;
    }
    
}