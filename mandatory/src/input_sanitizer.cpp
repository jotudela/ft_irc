#include "../inc/Server.hpp"
#include "../inc/Admin.hpp"

using std::string;
using std::cout;
using std::endl;
using std::cin;
using std::remove;
using std::istringstream;

// TODO ADMIN & USER should use these function
// Sanitize input by removing \n and \r returns
std::string Server::sanitize_input(const std::string& raw_input)
{
    string input(raw_input.c_str(), raw_input.length());
    input.erase(remove(input.begin(), input.end(), '\n'), input.end());
    input.erase(remove(input.begin(), input.end(), '\r'), input.end());
    return input;
}
