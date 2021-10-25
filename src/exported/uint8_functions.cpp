#include "exported_functions.hpp"

func const char* getIntOutput(int output, int x, int y, int z) {
    static std::string output_str = "";
    output_str = std::to_string(g_CurrentIntResults[output](x,y,0,z));
    return output_str.c_str();
}