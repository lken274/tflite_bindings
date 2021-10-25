#include "read_csv.hpp"
#include "../main_includes.hpp"

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

void read_float_csv(const char* filename, int xSize, int ySize, int zSize, int numReads)
{
    // Create a vector of <string, int vector> pairs to store the result
    std::cout << "Attempting to read file " << filename << std::endl;
    std::vector<CImg<float>> result;

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
        result.push_back(CImg<float>(xSize, ySize, 1, zSize, 0));
        for (int k = 0; k < zSize; k++)
        {
            for (int j = 0; j < ySize; j++)
            {
                for (int i = 0; i < xSize; i++)
                {
                    float pix_val = std::stof(pixelVals[k * ySize * xSize + j * xSize + i + 1]);
                    result.at(idx).draw_point(i, j, k, &pix_val);
                }
            }
        }
        idx++;
        if (idx == numReads)
            break;
    }
    myFile.close();
    g_loaded_float_inputs = result;
}

void read_uint8_csv(const char* filename, int xSize, int ySize, int zSize, int numReads)
{
    // Create a vector of <string, int vector> pairs to store the result
    std::cout << "Attempting to read file " << filename << std::endl;
    std::vector<CImg<uint8_t>> result;

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
        result.push_back(CImg<uint8_t>(xSize, ySize, 1, zSize, 0));
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
    g_loaded_uint8_inputs = result;
}



