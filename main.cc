#include <filesystem>
#include <iostream>
#include <vector>

#include "image.hh"
#include "matrix.hh"

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
            std::cout << "file: " << filenameStr << '\n';
            // Image img("")
        }
        else
            std::cout << "??    " << filenameStr << '\n';
    }
}

int main()
{
    // Matrix w_i_h(20, 784);
    // Matrix w_h_o(10, 20);

    // w_i_h.randomize(-0.5f, 0.5f);
    // w_h_o.randomize(-0.5f, 0.5f);

    // Matrix b_i_h(20, 1);
    // Matrix b_h_o(10, 1);

    // size_t epoch = 5;
    // size_t nr_correct = 0;

    // float learn_rate = 0.01;

    iter_folder("/mnt/c/Users/Erwan/Desktop/mnist_png/training");

    // for (size_t i = 0; i < epoch; i++)
    // {
    // }

    return 0;
}