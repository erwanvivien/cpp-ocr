#pragma once

#include <iostream>
#include <vector>

#include "matrix.hh"

class Image
{
public:
    Image(const std::string &s, char expected);
    ~Image();

    size_t get_w() const;
    size_t get_h() const;

    Image &resize();
    Image &gray();

    Matrix &get_mat();

    std::vector<float> &operator[](size_t index);
    const std::vector<float> &operator[](size_t index) const;

private:
    size_t h_;
    size_t w_;

    Matrix pixels_;

    char expected_;
};

std::ostream &operator<<(std::ostream &os, const Image &img);