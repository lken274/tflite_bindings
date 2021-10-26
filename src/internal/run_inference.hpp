#pragma once
#include "../main_includes.hpp"
#include "display_image.hpp"

template <class U>
std::vector<CImg<U>> run_inference(std::vector<CImg<U>> inputs)
{
    std::cout << "Loading inputs..." << std::endl;
    for (int inputIdx = 0; inputIdx < g_inputSize; inputIdx++)
    {
        int input = g_interpreter->inputs()[inputIdx];
        U *input_data_ptr = g_interpreter->typed_tensor<U>(input);
        for (int z = 0; z < g_zSize; z++)
        {
            for (int y = 0; y < g_ySize; y++)
            {
                for (int x = 0; x < g_xSize; x++)
                {
                    input_data_ptr[z * g_ySize * g_xSize + y * g_xSize + x] = (U)(inputs[inputIdx](x, y, 0, z)) / (g_normalise ? 255.0 : 1.0);
                }
            }
        }
    }
    TFLITE_MINIMAL_CHECK(g_interpreter->Invoke() == kTfLiteOk);
    // Read output buffers
    std::vector<CImg<U>> results;
    std::cout << "Loading outputs..." << std::endl;
    for (int outputIdx = 0; outputIdx < g_outputSize; outputIdx++)
    {
        int output = g_interpreter->outputs()[outputIdx];
        U *output_ptr = g_interpreter->typed_tensor<U>(output);
        CImg<U> resultVector(g_xOutSize, g_yOutSize, 1, g_zOutSize);
        for (int z = 0; z < g_zOutSize; z++)
        {
            for (int y = 0; y < g_yOutSize; y++)
            {
                for (int x = 0; x < g_xOutSize; x++)
                {
                    U outputVal = *(output_ptr + z * g_yOutSize * g_xOutSize + y * g_xOutSize + x);
                    resultVector.draw_point(x, y, z, &outputVal);
                }
            }
        }
        results.push_back(resultVector);
    }
    //display_image(inputs[0], "image test");
    return results;
}