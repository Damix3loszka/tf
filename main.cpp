#include <iostream>
#include <filesystem>
#include <unistd.h>
#include <argh.h>
#include <string>
#include <fstream>

namespace fs = std::filesystem;

int main(int argc, char *argv[])
{

    ////init
    argh::parser parser;
    parser.add_params({"-F", "--folder", "-C", "--create"});
    parser.parse(argc, argv);

    std::string config_file_name = ".tf.conf";
    std::string user = getlogin();
    std::string path = "/home/" + user + "/";
    std::string template_folder{};

    if (!fs::exists(path + config_file_name))
    {
        std::ofstream config_file(path + config_file_name);
        config_file << path + "template_folder/";
        if (!fs::exists(path + "template_folder/"))
        {
            fs::create_directory(path + "template_folder/");
        }
    }

    //// reading path to a template folder, wrap in a function later
    std::ifstream config(path + config_file_name);
    config >> template_folder;
    config.close();
    ////

    //// changing ~ to homepath, wrap it in a func later
    if (template_folder[0] == '~')
        template_folder = template_folder.replace(0, 1, "/home/" + user);
    if (template_folder[template_folder.size() - 1] != '/')
        template_folder + "/";
    ////

    if (!fs::exists(template_folder))
    {
        //// changing default template folder
        if (parser({"-F", "--folder"}))
        {
            std::string new_template_folder = parser({"-F", "--folder"}).str();
            if (new_template_folder[0] == '~')
                template_folder = template_folder.replace(0, 1, "/home/" + user);
            if (new_template_folder[new_template_folder.size() - 1] != '/')
                template_folder + "/";
            std::ofstream config(path + config_file_name);
            config << new_template_folder;
            config.close();
            return 0; ////endpoint
        }
        else
        {
            std::cerr << "No such folder " + template_folder + " exists. Use -F or --folder to set a default template folder." << std::endl;
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
            std::ofstream file(template_folder + template_file);
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