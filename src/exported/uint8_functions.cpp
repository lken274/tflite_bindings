#include "exported_functions.hpp"

func uint8_t getIntOutput(int output, int x, int y, int z) {
    return (uint8_t) g_CurrentIntResults[output](x,y,0,z);
}