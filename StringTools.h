#ifndef __CPPTOOLS_STRING_TOOL_H__
#define __CPPTOOLS_STRING_TOOL_H__

#include <vector>
#include <string>

namespace cppTools {

std::vector<std::string> split(const std::string& str, const std::string& sep);
std::string& trim(std::string& str);
std::string implode(const std::string &sep, const std::vector<std::string>& str);
std::string& replace_all(const std::string& old_str, const std::string& new_str, std::string& source); 
std::string base64_encode(const std::string& str);
std::string base64_decode(const std::string& str);

}

#endif

