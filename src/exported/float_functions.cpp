#include "exported_functions.hpp"

func const char* getFloatOutput(int output, int x, int y, int z) {
    std::stringstream output_ss;
    static std::string output_str;
    output_ss << std::fixed << std::setprecision(2) << g_CurrentFloatResults[output](x,y,0,z);
    output_str = output_ss.str();
    return output_str.c_str();
}