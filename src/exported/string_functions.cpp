#include "exported_functions.hpp"

func const char* getStringOutput(int output, int x, int y, int z) {
    return (const char*) g_CurrentStringResults[output](x,y,0,z);
}