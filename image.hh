#pragma once

#include <vector>
#include <iostream>

#include "matrix.hh"

class Image
{
public:
    Image(const std::string& s, char expected);
    ~Image();

    Image &resize();
    Image &gray();

    Matrix &get_mat();

private:
    size_t h_;
    size_t w_;

    Matrix pixels_;

    char expected_;
};
