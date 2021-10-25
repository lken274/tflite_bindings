#pragma once
#include "../main_includes.hpp"
#include "../internal/display_image.hpp"
#include "../internal/run_inference.hpp"
#include "../internal/read_csv.hpp"
#define func extern "C" __declspec(dllexport)

func void init_model(const char* modelpath);
func const char* run_inference_on_next();
func void set_output_size(const char* num_outputs, const char* x, const char* y, const char* z/*, DATA_TYPE type */);
func void set_input_size(const char* num_inputs, const char* x, const char* y, const char* z /*, const char* type, const char* normalise*/);
func void load_csv_inputs(const char* filename);
func void printResultsData();
func const char* getFloatOutput(int output, int x, int y, int z);
func const char* getIntOutput(int output, int x, int y, int z);
func void set_data_type(const char* dataType, const char* doNormalise);