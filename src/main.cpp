#include <tensorflow/lite/c/c_api.h>
#include <iostream>
int main(void) {
    std::cout << "TensorFlow Version: " << TfLiteVersion() << std::endl;
    return 0;
}

