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

csv_file read_csv(std::string filename, int xSize, int ySize, int zSize, int numReads);

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
  std::unique_ptr<tflite::Interpreter> interpreter;
  builder(&interpreter);
  TFLITE_MINIMAL_CHECK(interpreter != nullptr);

  // Allocate tensor buffers.
  TFLITE_MINIMAL_CHECK(interpreter->AllocateTensors() == kTfLiteOk);
 // printf("=== Pre-invoke Interpreter State ===\n");
  //tflite::PrintInterpreterState(interpreter.get());

  // Fill input buffers
  // TODO(user): Insert code to fill input tensors.
  // Note: The buffer of the input tensor with index `i` of type T can
  // be accessed with `T* input = interpreter->typed_input_tensor<T>(i);`
  
  csv_file testData = read_csv("../model_data/sign_mnist_test/sign_mnist_test.csv", 28, 28, 1, 100);
//  std::cout << "Drawing image 0" << std::endl;

//   for(auto& image : testData) {
//       CImgDisplay dsp(image.resize(128,128,1,1,2), "test", 0);
//       while(!dsp.is_closed()) {
//       }
//   }

  // Run inference
  for(int idx = 0; idx < 100; idx++) {
    int input_tensor_idx = 0;
    int input = interpreter->inputs()[input_tensor_idx];
    float* input_data_ptr = interpreter->typed_tensor<float>(input);
    for(int k = 0; k < 1; k++) {
            for(int j = 0; j < 28; j++) {
                for(int i = 0; i < 28; i++) {
                    //input
                    input_data_ptr[k*28*28 + j*28 + i] = (float)(testData[idx](i,j,0,k) / 255.0);
                }
            }
    }
    TFLITE_MINIMAL_CHECK(interpreter->Invoke() == kTfLiteOk);

    // Read output buffers
    float max_confidence = 0;
    int index = -1;
    int output_tensor_idx = 0;
    int output = interpreter->outputs()[output_tensor_idx];
    float* output_ptr = interpreter->typed_tensor<float>(output);
    for(int i = 0; i < 25; i++) {
        float outputVal = *(output_ptr+i);
        //std::cout << i << ":" << outputVal << std::endl;
        if (outputVal > max_confidence) {
            max_confidence = outputVal;
            index = i;
        }
    }
    std::cout << "inference result is: " << index << std::endl;
    }

  return 0;
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
