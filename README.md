## tf
**tf is a simple command tool to create files based on templates given in a template folder.**

Install(requires CMake):
1. Clone this repository. 
2. While being in the project directory type in terminal:
    ```
    chmod +x configure_and_install.sh
    ./configure_and_install.sh
    ```

To view help:
```
tf -H  or  tf --help 
```
Upon first use creates a folder template_folder in current user's home directory.
This folder is used to store template files. It can be changed later.

This project uses [Argh!](https://github.com/adishavit/argh).