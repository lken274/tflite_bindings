#include "internal_functions.hpp"

template <class T>
void display_image(CImg<T> &img, std::string imagename)
{
    CImgDisplay dsp(img.resize(256, 256, 1, 1, 2), imagename.c_str(), 0);
    while (!dsp.is_closed() && !dsp.is_keyENTER() && !dsp.is_keySPACE())
    {
    }
}

std::vector<CImg<float>> run_inference(std::vector<CImg<unsigned char>> inputs)
{
    std::cout << "Loading inputs..." << std::endl;
    for (int inputIdx = 0; inputIdx < g_inputSize; inputIdx++)
    {
        int input = g_interpreter->inputs()[inputIdx];
        float *input_data_ptr = g_interpreter->typed_tensor<float>(input);
        for (int z = 0; z < g_zSize; z++)
        {
            for (int y = 0; y < g_ySize; y++)
            {
                for (int x = 0; x < g_xSize; x++)
                {
                    input_data_ptr[z * g_ySize * g_xSize + y * g_xSize + x] = (float)(inputs[inputIdx](x, y, 0, z)) / (g_normalise ? 255.0 : 1.0);
                }
            }
        }
    }
    TFLITE_MINIMAL_CHECK(g_interpreter->Invoke() == kTfLiteOk);
    // Read output buffers
    std::vector<CImg<float>> results;
    std::cout << "Loading outputs..." << std::endl;
    for (int outputIdx = 0; outputIdx < g_outputSize; outputIdx++)
    {
        int output = g_interpreter->outputs()[outputIdx];
        float *output_ptr = g_interpreter->typed_tensor<float>(output);
        CImg<float> resultVector(g_xOutSize, g_yOutSize, 1, g_zOutSize);
        for (int z = 0; z < g_zOutSize; z++)
        {
            for (int y = 0; y < g_yOutSize; y++)
            {
                for (int x = 0; x < g_xOutSize; x++)
                {
                    float outputVal = *(output_ptr + z * g_yOutSize * g_xOutSize + y * g_xOutSize + x);
                    resultVector.draw_point(x, y, z, &outputVal);
                }
            }
        }
        results.push_back(resultVector);
    }
    return results;
}

template <typename Out>
void split(const std::string &s, char delim, Out result)
{
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, delim))
    {
        *result++ = item;
    }
}

std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

image_vector read_csv(const char* filename, int xSize, int ySize, int zSize, int numReads)
{
    // Create a vector of <string, int vector> pairs to store the result
    std::cout << "Attempting to read file " << filename << std::endl;
    std::vector<CImg<unsigned char>> result;

    // Create an input filestream
    std::ifstream myFile(filename);

    // Make sure the file is open
    if (!myFile.is_open())
        throw std::runtime_error("Could not open file");
    std::cout << "File opened successfully!" << std::endl;
    // Helper vars
    std::string line;

    std::getline(myFile, line); //skip label line
    // Read data, line by line
    int idx = 0;
    while (std::getline(myFile, line))
    {
        std::vector<std::string> pixelVals = split(line, ',');
        result.push_back(CImg(xSize, ySize, 1, zSize, 0));
        for (int k = 0; k < zSize; k++)
        {
            for (int j = 0; j < ySize; j++)
            {
                for (int i = 0; i < xSize; i++)
                {
                    uint8_t pix_val = std::stoul(pixelVals[k * ySize * xSize + j * xSize + i + 1]);
                    result.at(idx).draw_point(i, j, k, &pix_val);
                }
            }
        }
        idx++;
        if (idx == numReads)
            break;
    }
    myFile.close();
    return result;
}
