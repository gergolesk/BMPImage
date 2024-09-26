#include <iostream>
#include "BMPImage.h"


int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: drawBmp.exe <path_to_file>" << endl;
        return 1;
    }

    string fileName = argv[1];
    BMPImage bmpImage;

    if (!bmpImage.openBMP(fileName)) {
        return 1;
    }

    bmpImage.displayBMP();
    bmpImage.closeBMP();

    return 0;
}
