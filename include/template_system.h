#ifndef TEMPLATE_SYSTEM
#define TEMPLATE_SYSTEM
#include <exception>
#include <filesystem>
#include <iostream>
#include <regex>
#include <string>
namespace fs = std::filesystem;
class template_system
{
  public:
    template_system(const fs::path template_folder); // default constructor
    ~template_system(){};                            // default destructor

    void add_template(const fs::path file);
    void remove_template(const fs::path template_file);
    void create_from_template(const fs::path template_file, const fs::path file_to_create);
    void list_templates(std::string extension);

  private:
    fs::path template_folder;
};
#endif