#include "exported_functions.hpp"

func float getFloatOutput(int output, int x, int y, int z) {
    return (float) g_CurrentFloatResults[output](x,y,0,z);
}