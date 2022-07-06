#include <iostream>
#include <filesystem>
#include <unistd.h>
#include "argh.h"
#include <string>
#include <fstream>

namespace fs = std::filesystem;

std::string help =
    "tf is a simple command tool to create files based on templates given in a template folder.\n"
    "Syntax:\n"
    "1. tf EXTENSION TEMPLATE_NAME FILE_NAME\n"
    "2. tf [-S, --set] PATH\n"
    "3. tf [-F, --folder]\n"
    "4. tf [-H, --help]\n"
    "\n"
    "1. Creates a file with name FILE_NAME and extension EXTENSION based on a template file with name TEMPLATE_NAME.\n"
    "2. Changes path to a template folder to PATH.\n"
    "3. Prints absolute path to the template folder.\n"
    "4. Prints this.\n"
    "\n"
    "Templates are nothing but files in a form of TEMPLATE_NAME.EXTENSION.\n";

void write_to_file(const std::string path, const std::string content);

std::string read_from_file(const std::string path);

void path_cleanup(std::string &path);

int main(int argc, char *argv[])
{
    argh::parser parser;
    parser.add_params({"-S", "--set"});
    parser.parse(argc, argv);

    std::string config_file_name = ".tf.conf";
    std::string user = getlogin();
    std::string homepath = "/home/" + user + "/";
    std::string template_folder_path{};

    if (!fs::exists(homepath + config_file_name))
    {
        write_to_file(homepath + config_file_name, homepath + "template_folder/");
        if (!fs::exists(homepath + "template_folder/"))
        {
            fs::create_directory(homepath + "template_folder/");
        }
    }

    template_folder_path = read_from_file(homepath + config_file_name);

    path_cleanup(template_folder_path);

    std::string extension{}, template_name{}, file_name{};
    int number_of_args = parser.size();

    if (number_of_args == 4)
    {
        extension = parser[1];
        template_name = parser[2];
        file_name = parser[3];

        std::string template_file = template_name + "." + extension;

        if (!fs::exists(template_folder_path + template_file))
        {
            std::cerr << "Template " + template_file + " doesn't exist." << std::endl;
            return 3; // endpoint
        }
        else
            fs::copy(template_folder_path + template_file, "./" + file_name + "." + extension);
        return 0; // endpoint
    }
    else if (number_of_args == 3)
    {
        if (parser({"-S", "--set"}))
        {
            std::string path_to_new_folder = parser({"-S", "--set"}).str();
            std::string folder_absolute_path = fs::absolute(path_to_new_folder);
            if (fs::exists(folder_absolute_path))
            {
                write_to_file(homepath + config_file_name, folder_absolute_path);
                return 0; // endpoint
            }
            else
            {
                std::cerr << "No such folder " + template_folder_path + " exists. Create it first." << std::endl;
                return 1; // endpoint
            }
        }
    }
    else if (number_of_args == 2)
    {
        if (parser[{"F", "folder"}])
        {
            std::cout << template_folder_path << std::endl;
            return 0; // endpoint
        }
        else if (parser[{"H", "help"}])
        {
            std::cout << help << std::endl;
            return 0; // endpoint
        }
        else
        {
            std::cerr << "Invalid arguments" << std::endl;
            return 2;
        }
    }
    else
    {
        std::cerr << "Invalid number of arguments." << std::endl;
        return 2; // endpoint
    }
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