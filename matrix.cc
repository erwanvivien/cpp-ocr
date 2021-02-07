#include "matrix.hh"

#include <time.h>
// #include <random>

float RandomFloat(float a, float b)
{
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;

    return a + r;
}

Matrix::Matrix (size_t height, size_t width)
    : h_(height)
    , w_(width)
{
    srand(time(NULL));

    mat_ = std::vector<std::vector<float>>();
    for (size_t i = 0; i < height; i++)
        mat_.push_back(std::vector<float>(width, 0));
}

Matrix::~Matrix()
{

}


size_t Matrix::get_w() const
{
    return w_;
}

size_t Matrix::get_h() const
{
    return h_;
}

void Matrix::fill(float value)
{
    for (size_t i = 0; i < h_; i++)
        for (size_t j = 0; j < w_; j++)
            mat_[i][j] = value;
}

void Matrix::randomize(float from, float to)
{
    for (size_t i = 0; i < h_; i++)
        for (size_t j = 0; j < w_; j++)
            mat_[i][j] = RandomFloat(from, to);
}

std::vector<float>& Matrix::operator[](size_t index)
{
    if (index >= h_)
        throw "Index is too far";

    return mat_[index];
}

const std::vector<float>& Matrix::operator[](size_t index) const
{
    if (index >= h_)
        throw "Index is too far";

    return mat_[index];
}

Matrix Matrix::operator+(const Matrix& m) const
{
    if (w_ != m.w_ || h_ != m.h_)
        throw "Not same height / width";

    Matrix newone(h_, w_);
    for (size_t i = 0; i < h_; i++)
    {
        for (size_t j = 0; j < w_; j++)
        {
            newone[i][j] = mat_[i][j] + m[i][j];
        }
    }

    return newone;
}

Matrix& Matrix::operator+=(const Matrix& m)
{
    if (w_ != m.w_ || h_ != m.h_)
        throw "Not same height / width";

    for (size_t i = 0; i < h_; i++)
    {
        for (size_t j = 0; j < w_; j++)
        {
            mat_[i][j] += m[i][j];
        }
    }

    return *this;
}

Matrix Matrix::operator*(const Matrix& m) const
{
    if (w_ != m.h_)
        throw "Not good sizes for mult";

    Matrix tmp(h_, m.w_);
    for (size_t i = 0; i < h_; i++)
    {
        for (size_t j = 0; j < m.w_; j++)
        {
            for (size_t k = 0; k < w_; k++)
            {
                tmp[i][j] += mat_[i][k] * m[k][j];
            }
        }
    }

    return tmp;
}

Matrix Matrix::operator*(float elt) const
{
    Matrix tmp(h_, w_);

    for (size_t i = 0; i < h_; i++)
    {
        for (size_t j = 0; j < w_; j++)
        {
            tmp[i][j] = mat_[i][j] * elt;
        }
    }

    return tmp;
}

Matrix& Matrix::operator*=(float elt)
{
    for (size_t i = 0; i < h_; i++)
    {
        for (size_t j = 0; j < w_; j++)
        {
            mat_[i][j] *= elt;
        }
    }

    return *this;
}

void Matrix::resize(size_t height, size_t width)
{
    h_ = height;
    w_ = width;

    mat_ = std::vector<std::vector<float>>();
    for (size_t i = 0; i < height; i++)
        mat_.push_back(std::vector<float>(width, 0));
}

Matrix operator*(int elt, const Matrix& m)
{
    return m * elt;
}

std::ostream &operator<< (std::ostream&os, const Matrix& m)
{
    for (size_t i = 0; i < m.get_h(); i++)
    {
        for (size_t j = 0; j < m.get_w() - 1; j++)
        {
            os << m[i][j] << ' ';
        }
        os << m[i][m.get_w() - 1] << '\n';
    }

    return os;
}