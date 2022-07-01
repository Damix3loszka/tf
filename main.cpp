#include <iostream>
#include <filesystem>
#include <unistd.h>
#include <argh.h>
#include <string>

namespace fs = std::filesystem;
int main(int argc, char *argv[])
{
    std::string template_folder = "~/";

    if (template_folder[0] == '~')
    {
        std::string user = getlogin();

        if (user == "root")
        {
            template_folder = template_folder.replace(0, 1, "/root");
        }
        else
        {
            template_folder = template_folder.replace(0, 1, "/home/" + user);
        }
    }

    if (!fs::exists(template_folder))
    {
        std::cout << "No such folder " + template_folder + " exists." << std::endl;
        return 1;
    }

    return 0;
}