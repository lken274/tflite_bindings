#pragma once
#include "../main_includes.hpp"

template <class T>
void display_image(CImg<T> &img, std::string imagename)
{
    CImgDisplay dsp(img.resize(256, 256, 1, 1, 2), imagename.c_str(), 0);
    while (!dsp.is_closed() && !dsp.is_keyENTER() && !dsp.is_keySPACE())
    {
    }
}