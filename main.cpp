#include <iostream>
#include <filesystem>
#include <unistd.h>
#include <argh.h>
#include <string>

namespace fs = std::filesystem;
int main(int argc, char *argv[])
{
    std::string template_folder = "~/projekty/tf/";

    if (!fs::exists(template_folder))
    {
        std::cout << "No such folder " + template_folder + " exists." << std::endl;
        return 1;
    }

    return 0;
}