#include "exported_functions.hpp"

func const char* getStringOutput(int output, int x, int y, int z) {
    static std::string output_str;
    output_str = g_CurrentStringResults[output](x,y,0,z);
    return output_str.c_str() ;
}