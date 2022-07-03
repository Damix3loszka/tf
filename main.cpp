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
    // std::string config_file = "/etc/tf.conf";
    std::string config_file = "tf.conf";
    std::string template_folder{};

    argh::parser parser;
    parser.add_params({"-F", "--folder", "-C", "--create"});
    parser.parse(argc, argv);

    //// changing default template folder
    if (parser({"-F", "--folder"}))
    {
        std::string new_template_folder = parser({"-F", "--folder"}).str();
        std::fstream config(config_file, std::ios::out);
        config << new_template_folder;
        config.close();
        return 0; ////endpoint
    }
    ////

    //// reading path to a template folder, wrap in a function later
    std::fstream config(config_file, std::ios::in);
    std::string line = "";
    std::istringstream stream{};
    getline(config, line);
    template_folder = line;
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
    if (parser.size() < 3 || parser.size() > 4)
    {
        std::cerr << "Invalid number of arguments." << std::endl;
        return 2; // endpoint
    }
    ////

    std::string extension, template_name, file_name;

    if (parser.size() == 4)
    {
        extension = parser[1];
        template_name = parser[2];
        file_name = parser[3];
    }
    else
    {
        extension = parser[1];
        template_name = "default";
        file_name = parser[2];
    }
    //// reading template name, extension and file name

    std::string template_file = template_name + "." + extension;
    ////

    //// copying template file to current directory
    if (!fs::exists(template_folder + template_file))
    {
        if (template_name != "default")
        {
            std::cerr << "No template with given name." << std::endl;
            return 3; // endpoint
        }

        std::cout << "Create a default template for ." + extension + "?(y or n): ";
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
        fs::copy(template_folder + template_file, "./" + file_name + "." + extension);

    ////
    return 0; // endpoint
}