
#include "main_includes.hpp"

//globals
int g_inputSize, g_xSize, g_ySize, g_zSize;
DATA_TYPE g_dataType;
bool g_normalise;
int g_outputSize, g_xOutSize, g_yOutSize, g_zOutSize;
std::unique_ptr<tflite::Interpreter> g_interpreter;
std::unique_ptr<tflite::FlatBufferModel> g_model;
std::string g_model_filename;
std::vector<CImg<float>> g_loaded_images;
int g_currentImageIdx = 0;

std::vector<CImg<float>> g_CurrentFloatResults;
std::vector<CImg<uint8_t>> g_CurrentIntResults;
std::vector<CImg<const char*>> g_CurrentStringResults;

