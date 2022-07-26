#ifndef __POPULATE_UTILS_HPP__
# define __POPULATE_UTILS_HPP__

# include "utils.hpp"


void populate_error_map(std::map<int, std::string>&);
void populate_mime_map(std::map<std::string, std::string>&);
void populate_icons_map(std::map<std::string, std::string>&);
void define_html_content(std::string&, const std::string&);
void generate_redir_status_code(std::map<int, std::string>&);

#endif // _POPULATE_UTILS_HPP__