#pragma once
#include <fstream>
#include <iostream>
#include <istream>
namespace Supergoon {
// Loads a file that is needed for the game to run, if the file doesn't exist throws an error and exits.
std::ifstream SafeLoadFile(std::string& name);

}  // namespace Supergoon
