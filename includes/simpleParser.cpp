# include <fstream> 
# include <iostream>

/* @brief 
    A simple parser for the brackets separation in the file
*/

int checkBrackets(std::string line) {
    std::string::iterator it = line.begin();
    for (; it != line.end(); it++) {
        if (*it == '{' or *it == '}')
            return (1);
    }
    return (0);
}

void getConfig() {
    std::ifstream config_file;
    std::string line;
    size_t brackets_parse = 0;
    config_file.open("../conf_files/configuration.conf", std::ios::in);
    if (!config_file.is_open())
        return ;
    while (std::getline(config_file, line)) {
        std::cout << line << std::endl;
        brackets_parse += checkBrackets(line);
    }
    config_file.close();
    if (brackets_parse % 2 == 0) {
        std::cout << "Status: All brackets closed" << std::endl;
        return ;
    }
    std::cout << "Status: Some bracket(s) open" << std::endl;
}

int main() {
    getConfig();
    return 0;
}