#include <iostream>
#include <filesystem>
#include <unistd.h>
#include <argh.h>
#include <string>
#include <fstream>
#include <sstream>

namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
    std::string configfile_name = "tf.conf";
    std::string template_folder{};

    //// reading path to a template folder, wrap in a function later
    std::fstream config(configfile_name);
    if (config.is_open())
    {
        std::string line = "";
        std::istringstream stream{};
        getline(config, line);
        template_folder = line;
    }
    ////

    //// changing ~ to homepath, wrap it in a func later
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
    ////

    argh::parser parser(argv);
    parser.params({"-F", "--folder"});

    if (parser({"-F", "--folder"}))
    {
        std::string new_template_folder = parser({"-F", "--folder"}).str();
        }
    return 0;
}