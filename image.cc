#include "image.hh"

#include <filesystem>
#include <iostream>

Image::Image(const std::string& s, char expected)
   :    pixels_(0, 0),
   expected_(expected)
{
    FILE* f = fopen(s.c_str(), "rb");
    unsigned char info[54];

    // read the 54-byte header
    fread(info, sizeof(unsigned char), 54, f); 

    // extract image height and width from header
    int width = *(int*)&info[18];
    int height = *(int*)&info[22];

    w_ = width;
    h_ = height;

    // allocate 3 bytes per pixel
    int size = 3 * width * height;
    
    int row_padded = (width * 3 + 3) & (~3);
    unsigned char* data = new unsigned char[size];

    // read the rest of the data at once
    fread(data, sizeof(unsigned char), size, f); 
    fclose(f);

    pixels_.resize(h_, w_);

    for(int i = 0; i < height; i++)
    {
        fread(data, sizeof(unsigned char), row_padded, f);
        for(int j = 0; j < width*3; j += 3)
        {
            // Convert (B, G, R) to (R, G, B)
            int tmp = data[j];
            data[j] = data[j+2];
            data[j+2] = tmp;

            pixels_[i][j / 3] = 0.2126f * data[j] + 0.7152f * data[j + 1] + 0.0722 * data[j + 2];
        }
    }

    delete[] data;
}

Image::~Image()
{
    
}

Image &Image::resize()
{
    return *this;
}

Image &Image::gray()
{
    return *this;
}

Matrix &Image::get_mat()
{
    return pixels_;
}

