#include <iostream>
#include <filesystem>
#include <unistd.h>
#include "include/argh.h"
#include <string>
#include <fstream>

namespace fs = std::filesystem;

void write_to_file(const std::string path, const std::string content);

std::string read_from_file(const std::string path);

void path_cleanup(std::string &path);

int main(int argc, char *argv[])
{

    ////init
    argh::parser parser;
    parser.add_params({"-F", "--folder"});
    parser.parse(argc, argv);

    std::string config_file_name = ".tf.conf";
    std::string user = getlogin();
    std::string path = "/home/" + user + "/";
    std::string template_folder_path{};

    if (!fs::exists(path + config_file_name))
    {
        write_to_file(path + config_file_name, path + "template_folder/");
        if (!fs::exists(path + "template_folder/"))
        {
            fs::create_directory(path + "template_folder/");
        }
    }
    ////

    //// reading path to a template folder
    template_folder_path = read_from_file(path + config_file_name);
    ////

    //// changing ~ to homepath, wrap it in a func later
    path_cleanup(template_folder_path);
    ////

    if (!fs::exists(template_folder_path))
    {
        //// changing default template folder
        if (parser({"-F", "--folder"}))
        {
            std::string new_template_folder_path = parser({"-F", "--folder"}).str();
            path_cleanup(new_template_folder_path);
            write_to_file(path + config_file_name, new_template_folder_path);
            return 0; ////endpoint
        }
        else
        {
            std::cerr << "No such folder " + template_folder_path + " exists. Use -F or --folder to set a default template folder." << std::endl;
            return 1; // endpoint
        }
        ////
    }
    ////

    //// checking for correct number of arguments
    if (parser.size() < 3 || parser.size() > 4)
    {
        std::cerr << "Invalid number of arguments." << std::endl;
        return 2; // endpoint
    }
    ////

    std::string extension, template_name, file_name;

    //// reading template name, extension and file name
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

    std::string template_file = template_name + "." + extension;
    ////

    //// copying template file to current directory
    if (!fs::exists(template_folder_path + template_file))
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
            std::ofstream file(template_folder_path + template_file);
            file << " ";
            file.close();
            write_to_file(template_folder_path + template_file, " ");
            std::cout << "Template created." << std::endl;
        }
        return 0; // endpoint
    }
    else
        fs::copy(template_folder_path + template_file, "./" + file_name + "." + extension);

    ////
    return 0; // endpoint
}

void write_to_file(const std::string path, const std::string content)
{
    std::ofstream config(path);
    config << content;
    config.close();
}

std::string read_from_file(const std::string path)
{
    std::string contents{};
    std::ifstream file(path);
    file >> contents;
    file.close();
    return contents;
}

// changing ~ to homepath and adding / at the and if necessary
void path_cleanup(std::string &path)
{
    if (path[0] == '~')
    {
        std::string user = getlogin();
        path = path.replace(0, 1, "/home/" + user);
    }
    if (path[path.size() - 1] != '/')
        path + "/";
}