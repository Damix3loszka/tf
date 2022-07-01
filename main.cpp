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
    //// init if necessary
    ////

    std::string configfile_name = "tf.conf";
    std::string template_folder{};

    argh::parser parser;
    parser.add_params({"-F", "--folder", "-C", "--create"});
    parser.parse(argc, argv);

    //// changing default template folder
    if (parser({"-F", "--folder"}))
    {
        std::string new_template_folder = parser({"-F", "--folder"}).str();
        std::fstream config(configfile_name, std::ios::out);
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
        std::cerr << "No such folder " + template_folder + " exists. Use -F or --folder to set a default template folder." << std::endl;
        return 1; // endpoint
    }

    //// checking for correct number of arguments
    if (parser.size() < 2 || parser.size() > 3)
    {
        std::cerr << "Invalid number of arguments." << std::endl;
        return 2; // endpoint
    }
    ////

    //// reading template name and extension
    std::string extension = parser[1];
    std::string name = parser[2];

    if (name.empty())
        name = "default";

    std::string template_file = name + "." + extension;
    ////

    //// copying template file to current directory
    if (!fs::exists(template_folder + template_file))
    {
        if (name != "default")
        {
            std::cerr << "No template with given name." << std::endl;
            return 3; // endpoint
        }

        std::cout << "Create default template for ." + extension + "?(y or n): ";
        std::string answer;
        std::cin >> answer;
        if (answer == "y")
        {
            std::fstream file(template_folder + template_file, std::ios::out);
            file << " ";
            file.close();
            std::cout << "Template created." << std::endl;
        }
        return 0; // endpoint
    }
    else
        fs::copy(template_folder + template_file, ".");
    ////
    return 0; // endpoint
}