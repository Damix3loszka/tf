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

    argh::parser parser;
    parser.add_params({"-F", "--folder"});
    parser.parse(argc, argv);

    //// changing default template folder
    if (parser({"-F", "--folder"}))
    {
        std::string new_template_folder = parser({"-F", "--folder"}).str();
        std::fstream config(configfile_name, std::ios::out);
        if (config.is_open())
            config << new_template_folder;
        config.close();
        return 0; ////endpoint
    }
    ////

    //// reading path to a template folder, wrap in a function later
    std::fstream config(configfile_name, std::ios::in);
    if (config.is_open())
    {
        std::string line = "";
        std::istringstream stream{};
        getline(config, line);
        template_folder = line;
    }
    config.close();
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
    ////
    if (!fs::exists(template_folder))
    {
        std::cout << "No such folder " + template_folder + " exists. Use -F or --folder to set a default template folder." << std::endl;
        return 1;
    }

    //// checking for correct number of arguments
    if (parser.size() < 2)
    {
        std::cout << "No arguments given." << std::endl;
        return 2; // endpoint
    }
    if (parser.size() > 3)
    {
        std::cout << "Too many arguments given." << std::endl;
        return 2; // endpoint
    }
    ////

    //// reading template name and extension

    return 0; // endpoint
}