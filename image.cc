#include "image.hh"

#include <filesystem>
#include <iostream>
#include <string>

char grayscale_to_ascii(float value)
{
    std::string ramp = "$@B%8&WM#*oO0Q/|()1{}[]?;:,\"^`\\'.";
    value *= (ramp.length() - 1);
    value /= 255;

    int pos = ramp.length() - 1 - value;
    return ramp[pos];
}

Image::Image(const std::string &s, char expected)
    : expected_(expected - '0')
    , filename(s)
    , pixels_(0, 0)
    , onerow_(0, 0)
{
    FILE *f = fopen(s.c_str(), "rb");
    if (f == nullptr)
        throw std::runtime_error("File was not found");
    unsigned char info[54];

    // read the 54-byte header
    fread(info, sizeof(unsigned char), 54, f);

    // extract image height and width from header
    int fileoffset = *(int *)(info + 10);
    int width = *(int *)(info + 18);
    int height = *(int *)(info + 22);

    /// Extracts only bits per pixels
    int bit_per_pixels = (*(int *)(info + 26)) & 0x0000ffff;
    int planes = ((*(int *)(info + 26)) & 0xffff0000) >> 16;

    std::cout << planes << ':' << bit_per_pixels << '\n';
    bit_per_pixels = planes / 8;

    fseek(f, fileoffset, SEEK_SET);

    w_ = width;
    h_ = height;

    int size = bit_per_pixels * width * height;
    unsigned char *data = new unsigned char[size];

    // read the rest of the data at once
    fread(data, sizeof(unsigned char), size, f);
    fclose(f);

    pixels_.resize(h_, w_);
    onerow_.resize(h_ * w_, 1);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width * bit_per_pixels; j += bit_per_pixels)
        {
            int b = data[i * width * bit_per_pixels + j + 0];
            int g = data[i * width * bit_per_pixels + j + 1];
            int r = data[i * width * bit_per_pixels + j + 2];

            pixels_[height - 1 - i][j / bit_per_pixels] = (r + g + b) / 3;
            onerow_[height - 1 - i + j / bit_per_pixels][0] = (r + g + b) / 3;
        }
    }

    delete[] data;
}

Image::~Image()
{}

size_t Image::get_w() const
{
    return w_;
}

size_t Image::get_h() const
{
    return h_;
}

Image &Image::resize(size_t height, size_t width)
{
    pixels_.resize(height, width);
    return *this;
}

Matrix &Image::get_mat()
{
    return onerow_;
}

const Matrix &Image::get_mat() const
{
    return onerow_;
}

/// Access operators
std::vector<float> &Image::operator[](size_t index)
{
    return pixels_[index];
}

const std::vector<float> &Image::operator[](size_t index) const
{
    return pixels_[index];
}

std::ostream &operator<<(std::ostream &os, const Image &img)
{
    size_t h = img.get_h();
    size_t w = img.get_w();
    for (size_t i = 0; i < h; i++)
    {
        for (size_t j = 0; j < w; j++)
        {
            std::cout << grayscale_to_ascii(img[i][j]);
        }
        std::cout << '\n';
    }

    return os;
}