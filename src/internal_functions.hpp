#pragma once
#include "main_includes.hpp"
#include "exported_functions.hpp"
//prototypes

std::vector<CImg<float>> run_inference(std::vector<CImg<unsigned char>> inputs);
image_vector read_csv(const char* filename, int xSize, int ySize, int zSize, int numReads);
template <class T>
void display_image(CImg<T> &img, std::string imagename);