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
tf -h 
```
Upon first use creates a configuration file in the current user's home directory and asks to select template folder.

This project uses [Argh!](https://github.com/adishavit/argh).