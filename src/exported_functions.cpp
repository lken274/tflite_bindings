#include "exported_functions.hpp"

func void init_model(const char* modelpath) {
    g_model = tflite::FlatBufferModel::BuildFromFile(modelpath);
    TFLITE_MINIMAL_CHECK(g_model != nullptr);
    tflite::ops::builtin::BuiltinOpResolver resolver;
    tflite::InterpreterBuilder builder(*g_model, resolver);
    builder(&g_interpreter);
    TFLITE_MINIMAL_CHECK(g_interpreter != nullptr);

        // Allocate tensor buffers.
    TFLITE_MINIMAL_CHECK(g_interpreter->AllocateTensors() == kTfLiteOk);
}

func double check_dll_connected() {
    return 1.0;
}

func void set_data_type(const char* inputDataType, const char* doNormalise, const char* outputDataType) {
    std::string strDataType = std::string(inputDataType);
    if (strDataType == "float") {
        g_dataType = TF_FLOAT;
    }
    else if (strDataType == "uint8") {
        g_dataType = TF_UINT8;
    }
    else if (strDataType == "string"){
        g_dataType = TF_STRING;
    }
    else g_dataType = TF_FLOAT;

    std::string strOutputDataType = std::string(outputDataType);
    if (strOutputDataType == "float") {
        g_outdataType = TF_FLOAT;
    }
    else if (strOutputDataType == "uint8") {
        g_outdataType = TF_UINT8;
    }
    else if (strOutputDataType == "string"){
        g_outdataType = TF_STRING;
    }
    else g_outdataType = TF_FLOAT;

    std::string strDoNormalise = std::string(doNormalise);
    if (strDoNormalise == "true") g_normalise = true;
    else g_normalise = false;
}
func void set_input_size(const char* num_inputs, const char* x, const char* y, const char* z/*, const char* type, const char* normalise*/)
{
    g_inputSize = std::stoi(num_inputs);
    g_xSize = std::stoi(x);
    g_ySize = std::stoi(y);
    g_zSize = std::stoi(z);
}

func void set_output_size(const char* num_outputs, const char* x, const char* y, const char* z/*, DATA_TYPE type */)
{
    g_outputSize = std::stoi(num_outputs);
    g_xOutSize = std::stoi(x);
    g_yOutSize = std::stoi(y);
    g_zOutSize = std::stoi(z);
}

func const char* get_input_size_string() {
    std::string output = "num:" + std::to_string(g_inputSize) + ",x:" + std::to_string(g_xSize) + ",y:" + std::to_string(g_ySize) + ",z:" + std::to_string(g_zSize);
    std::cout << output << std::endl;
    return output.c_str();
}

func const char* get_output_size_string() {
    std::string output = "num:" + std::to_string(g_outputSize) + ",x:" + std::to_string(g_xOutSize) + ",y:" + std::to_string(g_yOutSize) + ",z:" + std::to_string(g_zOutSize);
    std::cout << output << std::endl;
    return output.c_str();
}

func const char* run_inference_on_next() {
    std::cout << "Running single inference" << std::endl;
    std::vector<CImg<unsigned char>> inputs;
    for(int i = 0; i < g_inputSize; i++) {
        if (i >= g_loaded_images.size()) return "0";
        inputs.push_back(g_loaded_images[g_currentImageIdx++]);
    }
    g_CurrentFloatResults = run_inference(inputs);
    return "1";
}

func void load_csv_inputs(const char* filename)
{
    g_loaded_images = read_csv(filename, g_xSize, g_ySize, g_zSize, -1);
    std::cout << "Loaded " << g_loaded_images.size() << " images" << std::endl;
}

func float getFloatOutput(int output, int x, int y, int z) {
    return (float) g_CurrentFloatResults[output](x,y,0,z);
}

func const char* get_results_string() {
    std::stringstream ss;
    for(size_t i = 0; i < g_CurrentFloatResults.size(); i++) {
        ss << "Output " << i << ":";
        for(int j = 0; j < g_xOutSize; j++) {
            ss << std::fixed << std::setprecision( 2 ) << getFloatOutput(i,j,0,0) << " ";
        }
    }
    std::string output = ss.str().substr(0, ss.str().size() - 1);
    return output.c_str();
}


func void printResultsData() {
    std::cout << "Current results:" << std::endl;
    std::cout << get_results_string() << std::endl;
}

