#include "template_system.h"

template_system::template_system(const fs::path template_folder)
{
    this->template_folder = template_folder;
}

void template_system::add_template(const fs::path file)
{
    if (!fs::exists(template_folder / file))
    {
        std::string message = "Template " + file.filename().string() + " already exist.";
        throw std::runtime_error(message);
    }
    fs::copy(file, template_folder / file);
    std::cout << "Template added." << std::endl;
}

void template_system::remove_template(const fs::path template_file)
{
    if (!fs::exists(template_folder / template_file))
    {
        std::string message = "Template " + template_file.string() + " doesn't exist.";
        throw std::runtime_error(message);
    }
    fs::remove(template_folder / template_file);
    std::cout << "Template deleted." << std::endl;
}

void template_system::create_from_template(const fs::path template_file, const fs::path file_to_create)
{
    if (!fs::exists(template_folder / template_file))
    {
        std::string message = "Template " + template_file.string() + " doesn't exist.";
        throw std::runtime_error(message);
    }
    if (fs::exists(file_to_create))
    {
        std::string message = "File" + file_to_create.string() + " already exists.";
        throw std::runtime_error(message);
    }
    fs::copy(template_folder / template_file, file_to_create);
}

void template_system::list_templates(std::string extension)
{
    std::regex re("\\." + extension + "$");

    for (auto const file_entry : fs::directory_iterator(template_folder))
    {
        fs::path file = file_entry.path().filename();
        if (std::regex_search(file.string(), re))
            std::cout << file.string() << std::endl;
    }
}
