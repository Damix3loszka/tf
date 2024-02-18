#include "template_system.h"

template_system::template_system(const fs::path template_folder)
{
    this->template_folder = template_folder;
    if (this->template_folder.empty())
    {
        throw std::runtime_error("No template folder selected. Use tf -s PATH to set template folder.");
    }
    else if (!fs::exists(this->template_folder))
    {
        throw std::runtime_error("Selected template folder doesn't exist. Use tf -s PATH to set template folder.");
    }
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

void template_system::create_from_template(const fs::path template_file, const fs::path filename)
{
    fs::path file_to_create = filename;
    file_to_create += template_file.extension();
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
    fs::copy(template_folder / template_file, file_to_create.string());
}

void template_system::list_templates(const std::string extension)
{
    std::regex re("\\." + extension + "$");

    for (auto const entry : fs::recursive_directory_iterator(template_folder))
    {
        fs::path entry_path = entry.path();
        if (fs::is_directory(entry_path))
            continue;
        fs::path parent_folder =
            *std::mismatch(template_folder.begin(), template_folder.end(), entry_path.begin()).second;
        parent_folder = (parent_folder == entry_path.filename()) ? "" : parent_folder;
        if (std::regex_search(entry_path.filename().string(), re))
            std::cout << (parent_folder / entry_path.filename()).string() << std::endl;
    }
}

void template_system::edit_template(const fs::path template_file)
{
    std::string command = "open " + std::string(template_folder / template_file);
    system(command.c_str());
}
