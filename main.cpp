#include "argh.h"
#include "template_system.h"
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <unistd.h>
namespace fs = std::filesystem;

std::string help = R"""(
tf is a simple command tool to create files based on templates given in a template folder.
Syntax:
1. tf EXTENSION TEMPLATE_NAME <FILE_NAME>
2. tf {-s, --set} PATH
3. tf {-p, --path}
4. tf {-h, --help}

1. Creates a file with name FILE_NAME and extension EXTENSION based on a template file with name TEMPLATE_NAME.
2. Changes path to a template folder to PATH.
3. Prints absolute path to the template folder.
4. Prints this.

This tool copies files from template folder to current directory and renames them. That is basically what it does."
Templates are nothing but files in a form of TEMPLATE_NAME.EXTENSION.
)""";

void write_configuration(const fs::path path, const std::string content);

fs::path read_configuration(const fs::path path);

void path_cleanup(fs::path &path);

void list_files(const fs::path folder_path, const std::string extension);

int main(int argc, char *argv[])

{

    argh::parser parser;
    parser.add_param("-s"); // Set template folder path
    parser.add_param("-a"); // Add file as a template
    parser.add_param("-l"); // List template files
    parser.add_param("-d"); // Remove template file
    parser.parse(argc, argv);

    fs::path config_file = ".tf.conf";
    std::string user = getlogin();
    fs::path homepath = "/home/" + user + "/";
    fs::path template_folder{};

    if (!fs::exists(homepath / config_file))
    {
        write_configuration(homepath / config_file, homepath / "template_folder");
        if (!fs::exists(homepath / "template_folder"))
        {
            fs::create_directory(homepath / "template_folder/");
        }
    }

    template_folder = read_configuration(homepath / config_file);

    template_system t_sys{template_folder};
    int number_of_pos_args = parser.size();
    try
    {
        if (argc == 1)
        {
            throw std::runtime_error("Invalid number of arguments.");
        }
        if (argc > 4)
        {
            throw std::runtime_error("Invalid number of arguments.");
        }

        if (number_of_pos_args >= 3)
        {
            fs::path template_file = parser[2] + "." + parser[1];
            fs::path file_to_create = (number_of_pos_args == 4) ? fs::path(parser[3]) : template_file;
            t_sys.create_from_template(template_file, file_to_create);
            return 0;
        }

        if (parser("d"))
        {
            t_sys.remove_template(parser("d").str());
            return 0;
        }
        else
        {
            throw std::runtime_error("Invalid argument.");
        }

        if (parser("s")) // Change template folder path
        {
            fs::path new_folder = parser("s").str();
            if (fs::exists(new_folder))
            {
                // path_cleanup(new_folder);
                write_configuration(homepath / config_file, new_folder.string());
                std::cout << "Path changed" << std::endl;
                return 0; // endpoint
            }
            else
            {
                throw std::runtime_error("Folder " + new_folder.string() + " doesn't exist.");
            }
        }
        if (parser["h"])
        {
            std::cout << help;
            return 0;
        }
        if (parser("l"))
        {
            t_sys.list_templates(parser("l").str());
            return 0;
        }
        else if (parser["l"])
        {
            t_sys.list_templates("*");
            return 0;
        }

        ////
        return 0; // endpoint
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}

void write_configuration(const fs::path path, const std::string content)
{
    std::ofstream config(path);
    config << content;
    config.close();
}

fs::path read_configuration(const fs::path path)
{
    fs::path contents{};
    std::ifstream file(path);
    file >> contents;
    file.close();
    return contents;
}

void list_files(const fs::path folder_path, const std::string extension)
{
}