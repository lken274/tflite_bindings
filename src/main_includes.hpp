#pragma once
#include <iomanip>
#include "tensorflow/lite/interpreter.h"
#include "tensorflow/lite/kernels/register.h"
#include "tensorflow/lite/model.h"
#include "tensorflow/lite/optional_debug_tools.h"
#include <cimg.h>
#include <fstream>
#include <vector>
#include <stdexcept> // std::runtime_error
#include <sstream>   // std::stringstream
#include <string>
#include <iostream>
#include <cstdarg>
#include <any>
#include <ndarray.hpp>

using namespace cimg_library;

#define TFLITE_MINIMAL_CHECK(x)                                  \
    if (!(x))                                                    \
    {                                                            \
        fprintf(stderr, "Error at %s:%d\n", __FILE__, __LINE__); \
        exit(1);                                                 \
    }

// custom data types
enum DATA_TYPE
{
    TF_FLOAT,
    TF_UINT8,
    TF_STRING
};
enum INPUT_FORMAT
{
    CSV,
    IMAGE,
    DIRECT
};

extern int g_inputSize, g_xSize, g_ySize, g_zSize;
extern DATA_TYPE g_dataType;
extern bool g_normalise;
extern int g_outputSize, g_xOutSize, g_yOutSize, g_zOutSize;
extern std::unique_ptr<tflite::Interpreter> g_interpreter;
extern std::unique_ptr<tflite::FlatBufferModel> g_model;
extern std::string g_model_filename;
extern std::vector<CImg<float>>  g_loaded_float_inputs;
extern std::vector<CImg<std::string>> g_loaded_string_inputs;
extern int g_currentImageIdx;

extern std::vector<CImg<float>> g_CurrentFloatResults;
extern std::vector<CImg<uint8_t>> g_CurrentIntResults;
extern std::vector<CImg<std::string>> g_CurrentStringResults;