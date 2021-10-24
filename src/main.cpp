#include <cstdio>
#include "tensorflow/lite/interpreter.h"
#include "tensorflow/lite/kernels/register.h"
#include "tensorflow/lite/model.h"
#include "tensorflow/lite/optional_debug_tools.h"
#include <cimg.h>
#include <fstream>
#include <vector>
#include <utility> // std::pair
#include <stdexcept> // std::runtime_error
#include <sstream> // std::stringstream
#include <string>
#include <iostream>
#include <map>
#include <cstdarg>
// This is an example that is minimal to read a model
// from disk and perform inference. There is no data being loaded
// that is up to you to add as a user.
//
// NOTE: Do not add any dependencies to this that cannot be built with
// the minimal makefile. This example must remain trivial to build with
// the minimal build tool.
//
// Usage: minimal <tflite model>
using namespace cimg_library; 
typedef std::vector<CImg<unsigned char>> csv_file;

#define TFLITE_MINIMAL_CHECK(x)                              \
  if (!(x)) {                                                \
    fprintf(stderr, "Error at %s:%d\n", __FILE__, __LINE__); \
    exit(1);                                                 \
  }

enum DATA_TYPE {
    TF_FLOAT,
    TF_UINT8
};
std::vector<CImg<float>> run_inference(std::vector<CImg<unsigned char>> inputs);
void set_output_size(int num_outputs, int x, int y, int z, DATA_TYPE type);
void set_input_size(int num_inputs, int x, int y, int z, DATA_TYPE type, bool normalise);

csv_file read_csv(std::string filename, int xSize, int ySize, int zSize, int numReads);
std::unique_ptr<tflite::Interpreter> interpreter;
int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "minimal <tflite model>\n");
    return 1;
  }
  const char* filename = argv[1];

  // Load model
  std::unique_ptr<tflite::FlatBufferModel> model =
      tflite::FlatBufferModel::BuildFromFile(filename);
  TFLITE_MINIMAL_CHECK(model != nullptr);

  // Build the interpreter with the InterpreterBuilder.
  // Note: all Interpreters should be built with the InterpreterBuilder,
  // which allocates memory for the Interpreter and does various set up
  // tasks so that the Interpreter can read the provided model.
  tflite::ops::builtin::BuiltinOpResolver resolver;
  tflite::InterpreterBuilder builder(*model, resolver);
  builder(&interpreter);
  TFLITE_MINIMAL_CHECK(interpreter != nullptr);

  // Allocate tensor buffers.
  TFLITE_MINIMAL_CHECK(interpreter->AllocateTensors() == kTfLiteOk);

  csv_file testData = read_csv("../model_data/sign_mnist_test/sign_mnist_test.csv", 28, 28, 1, 100);
//  std::cout << "Drawing image 0" << std::endl;

//   for(auto& image : testData) {
//       CImgDisplay dsp(image.resize(128,128,1,1,2), "test", 0);
//       while(!dsp.is_closed()) {
//       }
//   }
    set_input_size(1,28,28,1,TF_FLOAT,true);
    set_output_size(1,26,1,1,TF_FLOAT);
    CImgDisplay dsp(testData[0].resize(128,128,1,1,2), "test", 0);
    while(!dsp.is_closed()) {
    }
    run_inference({testData[0]});

  return 0;
}



int g_inputSize, g_xSize, g_ySize, g_zSize, g_dataType;
bool g_normalise;
int g_outputSize, g_xOutSize, g_yOutSize, g_zOutSize, g_outdataType;

void set_input_size(int num_inputs, int x, int y, int z, DATA_TYPE type, bool normalise) {
    g_inputSize = num_inputs;
    g_xSize = x;
    g_ySize = y;
    g_zSize = z;
    g_dataType = type;
    g_normalise = normalise;
}

void set_output_size(int num_outputs, int x, int y, int z, DATA_TYPE type) {
    g_outputSize = num_outputs;
    g_xOutSize = x;
    g_yOutSize = y;
    g_zOutSize = z;
    g_outdataType = type;
}

std::vector<CImg<float>> run_inference(std::vector<CImg<unsigned char>> inputs) {
    for (int inputIdx = 0; inputIdx < g_inputSize; inputIdx++) {
        int input = interpreter->inputs()[inputIdx];
        float* input_data_ptr = interpreter->typed_tensor<float>(input);
        for (int z = 0; z < g_zSize; z++) {
            for(int y = 0; y < g_ySize; y++) {
                for(int x = 0; x < g_xSize; x++) {
                    input_data_ptr[z*g_ySize*g_xSize + y*g_xSize + x] = (float)(inputs[inputIdx](x,y,0,z))/ (g_normalise ? 255.0 : 1.0);
                }
            }
        }
    }
    TFLITE_MINIMAL_CHECK(interpreter->Invoke() == kTfLiteOk);
    // Read output buffers
    std::vector<CImg<float>> results;
    for (int outputIdx = 0; outputIdx < g_outputSize; outputIdx++) {
        int output = interpreter->outputs()[outputIdx];
        float* output_ptr = interpreter->typed_tensor<float>(output);
        CImg<float> resultVector(g_xOutSize, g_yOutSize, 1, g_zOutSize);
        for(int z = 0; z < g_zOutSize; z++) {
            for(int y = 0; y < g_yOutSize; y++) {
                for(int x = 0; x < g_xOutSize; x++) {
                    float outputVal = *(output_ptr + z*g_yOutSize*g_xOutSize+y*g_xOutSize+x);
                    std::cout <<"got output val " << outputVal << std::endl;
                    resultVector.draw_point(x,y,z,&outputVal);
                }
            }
        }
        results.push_back(resultVector);
    }
    
    // for(int i = 0; i < g_xOutSize; i++)
    //     std::cout << "inference result is: " << results[0](i,0,0,0) << std::endl;
    return results;
}

template <typename Out>
void split(const std::string &s, char delim, Out result) {
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, delim)) {
        *result++ = item;
    }
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

csv_file read_csv(std::string filename, int xSize, int ySize, int zSize, int numReads){
    // Reads a CSV file into a vector of <string, vector<int>> pairs where
    // each pair represents <column name, column values>

    // Create a vector of <string, int vector> pairs to store the result
    
    std::vector<CImg<unsigned char>> result;

    // Create an input filestream
    std::ifstream myFile(filename);

    // Make sure the file is open
    if(!myFile.is_open()) throw std::runtime_error("Could not open file");

    // Helper vars
    std::string line;
    unsigned char val;

    std::getline(myFile, line); //skip label line
    // Read data, line by line
    int idx = 0;
    while(std::getline(myFile, line))
    {
        std::vector<std::string> pixelVals = split(line, ',');
        result.push_back(CImg(xSize, ySize, 1, zSize, 0));
        for(int k = 0; k < zSize; k++) {
            for(int j = 0; j < ySize; j++) {
                for(int i = 0; i < xSize; i++) {
                    // Add the current integer to the 'colIdx' column's values vector
                    uint8_t pix_val = std::stoul(pixelVals[k*ySize*xSize + j*xSize + i + 1]);
                    result.at(idx).draw_point(i,j,k,&pix_val);
                }
            }
        }
        idx++;
        if (idx == numReads) break;
    }

    // Close file
    myFile.close();

    return result;
}
