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
This tool copies files from template folder to current directory and renames them.

Syntax:
tf TEMPLATE_FILE <FILE_NAME>
    Creates a file with name FILE_NAME and extension based on a TEMPLATE_FILE.

tf -a FILE
    Creates a template file from FILE.

tf -d TEMPLATE_FILE
    Delete TEMPLATE_FILE.

td -e TEMPLATE_FILE
    Edit TEMPLATE_FILE.

tf -l <EXTENSION>
    List template files. Specify EXTENSION to show only files with given extension.

tf -s PATH
    Change template folder path.

tf -p
    Show template folder path.

tf -h
    Show help.)""";

void write_configuration(const fs::path path, const std::string content);

fs::path read_configuration(const fs::path path);

int main(int argc, char *argv[])
{
    try
    {
        argh::parser parser;
        parser.add_param("-s"); // Set template folder path
        parser.add_param("-a"); // Add file as a template
        parser.add_param("-l"); // List template files
        parser.add_param("-d"); // Remove template file
        parser.add_param("-e"); // Edit template file
        parser.parse(argc, argv);

        int pos_args_num = parser.pos_args().size();

        fs::path config_file = ".tf.conf";
        std::string user = getlogin();
        fs::path homepath = "/home/" + user;
        fs::path template_folder{};

        if (!fs::exists(homepath / config_file))
        {
            std::ofstream conf(homepath / config_file);
            conf.close();
        }
        if (parser("s"))
        {
            fs::path new_folder = parser("s").str();
            write_configuration(homepath / config_file, new_folder);
            std::cout << "Path changed." << std::endl;
        }

        template_folder = read_configuration(homepath / config_file);

        template_system t_sys{template_folder};

        if (argc == 1 || argc > 3)
        {
            throw std::runtime_error("Invalid number of arguments. Use tf -h for help.");
        }

        if (pos_args_num == 3)
        {
            fs::path template_file = parser[1];
            fs::path filename = parser[2];
            t_sys.create_from_template(template_file, filename);
            return 0;
        }
        if (parser("e"))
        {
            t_sys.edit_template(parser("e").str());
            return 0;
        }
        if (parser("d"))
        {
            t_sys.remove_template(parser("d").str());
            return 0;
        }
        if (parser["d"])
        {
            throw std::runtime_error("Invalid argument.");
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
        if (parser["p"])
        {
            std::cout << "Template folder: " << template_folder.string();
        }
        if (parser["h"])
        {
            std::cout << help << std::endl;
            return 0;
        }
        return 0;
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
