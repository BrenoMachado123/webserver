#include <iostream>

int main() {
    std::cout << "Content-type:text/html\r\n\r\n";
    std::cout << "<html>\n";
    std::cout << "<head>\n";
    std::cout << "<title>Drop Down Box Data to CGI</title>\n";
    std::cout << "</head>\n";
    std::cout << "<body>\n";
    std::cout << "HELLO from CPP_COUNTRY_CGI.cpp";
    std::cout << "<br/>\n";
    std::cout << "</body>\n";
    std::cout << "</html>\n";
    return (0);
}