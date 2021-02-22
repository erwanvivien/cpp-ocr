#include <cmath>
#include <exception>
#include <filesystem>
#include <iostream>
#include <vector>

#include "image.hh"
#include "matrix.hh"

inline bool ends_with(std::string const &value, std::string const &ending)
{
    if (ending.size() > value.size())
        return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

void iter_folder(std::vector<Image> &image_set, const std::string &path,
                 int lvl = 0, char expected = -1)
{
    const std::filesystem::path pathToShow{ path };

    for (const auto &entry : std::filesystem::directory_iterator(pathToShow))
    {
        const auto filenameStr = entry.path().filename().string();
        std::cout << std::string(4 * lvl, ' ');
        if (entry.is_directory())
        {
            std::cout << "dir:  " << filenameStr << '\n';
            iter_folder(image_set, path + "/" + filenameStr, lvl + 1,
                        filenameStr[0]);
        }
        else if (entry.is_regular_file())
        {
            if (!ends_with(filenameStr, ".bmp"))
                continue;
            if (expected == -1)
                throw "Expected shouldn't be -1";

            std::cout << "file: " << filenameStr << '\n';
            Image img(path + "/" + filenameStr, expected);
            image_set.push_back(img);
        }
        else
            std::cout << "??    " << filenameStr << '\n';
    }
}

int main()
{
    std::vector<Image> image_set;
    iter_folder(image_set, "/mnt/c/Users/Erwan/Desktop/mnist_png/training/0", 1,
                '0');

    Matrix w_i_h(20, 784);
    Matrix w_h_o(10, 20);

    w_i_h.randomize(-0.5f, 0.5f);
    w_h_o.randomize(-0.5f, 0.5f);

    Matrix b_i_h(20, 1);
    Matrix b_h_o(10, 1);

    std::cout << w_i_h << '\n'
              << w_h_o << '\n'
              << b_i_h << '\n'
              << b_h_o << '\n';

    size_t epoch = 5;

    float learn_rate = 0.01;

    for (size_t i = 0; i < epoch; i++)
    {
        size_t nr_correct = 0;
        for (auto img : image_set)
        {
            auto h = b_i_h + w_i_h * img.get_mat();
            h.activate();

            auto o = b_h_o + w_h_o * h;
            o.activate();

            /// Error cost function
            Matrix label(10, 1);
            label[img.expected_][0] = 1;

            // Useful for other act. function
            // auto mat_sum = label - b_h_o;
            // mat_sum.power(2);
            // auto sum = mat_sum.sum();
            // auto error = sum / b_h_o.get_h();

            nr_correct += (o.argmax() == label.argmax());

            // Backward progation
            auto delta_o = o - label;
            w_h_o += (delta_o * h.transpose()) * -learn_rate;
            b_h_o += delta_o * -learn_rate;

            auto one_mat = Matrix(h.get_h(), h.get_w());
            one_mat.fill(1);
            one_mat = one_mat - h;

            for (size_t i = 0; i < h.get_h(); i++)
                one_mat[i][0] *= h[i][0];

            auto delta_h = w_h_o.transpose() * delta_o;
            for (size_t i = 0; i < h.get_h(); i++)
                delta_h[i][0] *= one_mat[i][0];

            w_i_h += (delta_h * img.get_mat().transpose()) * -learn_rate;
            b_i_h += delta_h * -learn_rate;
        }

        std::cout << learn_rate << nr_correct << '\n';
    }

    std::string path = "/mnt/c/Users/Erwan/Desktop/mnist_png/training/";
    while (1)
    {
        int nb;
        std::cin >> nb;
        size_t rdm;
        std::cin >> rdm;

        try
        {
            Image img(path + std::to_string(nb) + "/" + std::to_string(rdm)
                          + ".bmp",
                      nb + '0');
            auto h = b_i_h + w_i_h * img.get_mat();
            h.activate();

            auto o = b_h_o + w_h_o * h;
            o.activate();

            /// Error cost function
            std::cout << "Img expected: " << nb + '0' << "got"
                      << '0' + o.argmax();
        }
        catch (std::exception &e)
        {
            continue;
        }
    }

    return 0;
}