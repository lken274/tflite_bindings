#pragma once
void read_float_direct(const char* inputString, int xSize, int ySize, int zSize);
void read_float_csv(const char* filename, int xSize, int ySize, int zSize, int numReads);
void read_uint8_direct(const char* inputString, int xSize, int ySize, int zSize);
void read_uint8_csv(const char* filename, int xSize, int ySize, int zSize, int numReads);