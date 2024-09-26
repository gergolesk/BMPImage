#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>  // Для BITMAPFILEHEADER и BITMAPINFOHEADER

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
        // Перебор пикселей, начиная с нижней строки (BMP хранит строки снизу вверх)
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
                    // Черный цвет - объект
                    line.push_back('#');
                } else  {
                    //Белый цвет - фон
                   line.push_back(' ');
                } 
            }
            data.push_back(line);
        }
    }

public:
    // Открытие BMP файла
    bool openBMP(const string &fileName) {
        bmpFile.open(fileName, ios::binary);
        if (!bmpFile) {
            cerr << "Error: Failed to open file " << fileName << endl;
            return false;
        }

        // Чтение заголовка файла
        bmpFile.read(reinterpret_cast<char*>(&fileHeader), sizeof(BITMAPFILEHEADER));
        if (fileHeader.bfType != 0x4D42) {  // Проверка на "BM"
            cerr << "Error: Invalid BMP file format." << endl;
            return false;
        }

        // Чтение заголовка информации
        bmpFile.read(reinterpret_cast<char*>(&infoHeader), sizeof(BITMAPINFOHEADER));

        // Проверка на поддержку только 24 или 32 битных BMP
        if (infoHeader.biBitCount != 24 && infoHeader.biBitCount != 32) {
            cerr << "Error: Only 24 and 32-bit BMP images are supported." << endl;
            return false;
        }

        // Чтение данных пикселей
        int rowSize = ((infoHeader.biWidth * infoHeader.biBitCount + 31) / 32) * 4;
        int dataSize = rowSize * abs(infoHeader.biHeight);
        pixelData.resize(dataSize);

        // Переход к началу данных пикселей
        bmpFile.seekg(fileHeader.bfOffBits, ios::beg);
        bmpFile.read(reinterpret_cast<char*>(pixelData.data()), dataSize);

        writeToVector();

        return true;
    }

    

    // Отображение BMP файла в консоли
    void displayBMP() {
        if (colored) {
            cout<<"The image contains colored pixels. They will be ignored when output."<<endl;
        }
       for (auto line : data) {
            for (auto ch : line) {
                cout<<ch;
            }
            cout<<endl;
       } 
        
    }

    // Закрытие BMP файла
    void closeBMP() {
        if (bmpFile.is_open()) {
            bmpFile.close();
        }
    }

    ~BMPImage(){
        closeBMP();
        pixelData.clear();
    }

};