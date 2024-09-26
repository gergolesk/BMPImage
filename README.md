
# BMP Image Viewer Project

## Overview

This project provides a simple console application that reads and displays BMP image files in ASCII art format. It handles 24-bit and 32-bit BMP images, rendering black pixels as objects (`#`) and white pixels as background (` `), while ignoring any colored pixels.

The project consists of two main components:
1. `main.cpp` - Contains the main application logic to handle command-line arguments and control the flow.
2. `BMPImage.cpp` - Defines the `BMPImage` class which handles opening, reading, and displaying BMP files.

## File Descriptions

### `drawBmp.cpp`

This file contains the entry point of the program. It accepts the path to the BMP file as a command-line argument and uses the `BMPImage` class to open, display, and close the image.

```cpp
#include <iostream>
#include "BMPImage.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: drawBmp.exe <path_to_file>" << std::endl;
        return 1;
    }

    std::string fileName = argv[1];
    BMPImage bmpImage;

    if (!bmpImage.openBMP(fileName)) {
        return 1;
    }

    bmpImage.displayBMP();
    bmpImage.closeBMP();

    return 0;
}
```

- **Arguments**: The program expects one argument, the file path to the BMP image.
- **Error Handling**: If the file cannot be opened or is in an invalid format, it prints an error message and exits.
- **BMP Handling**: Calls methods to open, display, and close the BMP image.

### `drawBmp.cpp`

This file defines the `BMPImage` class that handles reading and displaying BMP images. It includes the following components:

1. **Private Members**:
   - `fileHeader` and `infoHeader`: Stores BMP headers.
   - `pixelData`: Vector to store the pixel data read from the BMP file.
   - `data`: 2D vector storing the ASCII representation of the image.
   - `bmpFile`: Input stream to read the BMP file.
   - `colored`: Flag indicating if the image contains colored pixels.

2. **Methods**:
   - `writeToVector()`: Converts pixel data into an ASCII art representation.
   - `openBMP()`: Opens the BMP file and reads pixel data.
   - `displayBMP()`: Prints the ASCII representation of the image to the console.
   - `closeBMP()`: Closes the BMP file stream.
   - Destructor to ensure the file is closed and memory is cleaned up.

```cpp
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>  // For BITMAPFILEHEADER and BITMAPINFOHEADER

using namespace std;

class BMPImage {
private:
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    vector<unsigned char> pixelData;
    vector<vector<char>> data;
    ifstream bmpFile;
    bool colored = false;

    void writeToVector() {
        int width = infoHeader.biWidth;
        int height = abs(infoHeader.biHeight);
        int bytesPerPixel = infoHeader.biBitCount / 8;
        int rowSize = ((width * infoHeader.biBitCount + 31) / 32) * 4;
        for (int y = height - 1; y >= 0; --y) {
            vector<char> line;
            for (int x = 0; x < width; ++x) {
                int pixelIndex = y * rowSize + x * bytesPerPixel;
                unsigned char blue = pixelData[pixelIndex];
                unsigned char green = pixelData[pixelIndex + 1];
                unsigned char red = pixelData[pixelIndex + 2];

                if (!(red == 0 && green == 0 && blue == 0) && !((red == 255 && green == 255 && blue == 255))) {
                    colored = true;
                }

                if (red == 0 && green == 0 && blue == 0) {
                    line.push_back('#');  // Black pixels -> object
                } else {
                    line.push_back(' ');  // White pixels -> background
                }
            }
            data.push_back(line);
        }
    }

public:
    bool openBMP(const string &fileName) {
        bmpFile.open(fileName, ios::binary);
        if (!bmpFile) {
            cerr << "Error: Failed to open file " << fileName << endl;
            return false;
        }

        bmpFile.read(reinterpret_cast<char*>(&fileHeader), sizeof(BITMAPFILEHEADER));
        if (fileHeader.bfType != 0x4D42) {  // Check if the file is a BMP
            cerr << "Error: Invalid BMP file format." << endl;
            return false;
        }

        bmpFile.read(reinterpret_cast<char*>(&infoHeader), sizeof(BITMAPINFOHEADER));

        if (infoHeader.biBitCount != 24 && infoHeader.biBitCount != 32) {
            cerr << "Error: Only 24 and 32-bit BMP images are supported." << endl;
            return false;
        }

        int rowSize = ((infoHeader.biWidth * infoHeader.biBitCount + 31) / 32) * 4;
        int dataSize = rowSize * abs(infoHeader.biHeight);
        pixelData.resize(dataSize);

        bmpFile.seekg(fileHeader.bfOffBits, ios::beg);
        bmpFile.read(reinterpret_cast<char*>(pixelData.data()), dataSize);

        writeToVector();

        return true;
    }

    void displayBMP() {
        if (colored) {
            cout << "The image contains colored pixels. They will be ignored when output." << endl;
        }
        for (auto line : data) {
            for (auto ch : line) {
                cout << ch;
            }
            cout << endl;
        }
    }

    void closeBMP() {
        if (bmpFile.is_open()) {
            bmpFile.close();
        }
    }

    ~BMPImage() {
        closeBMP();
        pixelData.clear();
    }
};
```

### Class Methods Overview:

- **`openBMP()`**: Reads the BMP headers and pixel data. Supports 24-bit and 32-bit BMP images.
- **`writeToVector()`**: Converts pixel data into ASCII art, where black pixels are rendered as `#`, and white pixels as spaces.
- **`displayBMP()`**: Outputs the ASCII art to the console.
- **`closeBMP()`**: Ensures the file stream is closed.

## How to Run

1. Compile the project using a C++ compiler with support for the Windows API (for `BITMAPFILEHEADER` and `BITMAPINFOHEADER`).
2. Run the program from the command line, passing the path to a BMP file as an argument:

```bash
drawBmp.exe <path_to_bmp_file>
```

The program will display the image in the terminal as ASCII art.

## Requirements

- A C++ compiler with support for Windows API.
- BMP files with 24-bit or 32-bit color depth.

## Example Output

For a BMP image with simple black and white patterns, the output will resemble:

```
####      
    ####  
        ##
```

The ASCII art output represents black pixels as `#` and white pixels as spaces.
