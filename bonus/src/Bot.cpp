#include "Bot.hpp"

using std::string;
using std::cout;
using std::endl;
using std::cerr;

Bot::Bot(){}

Bot::~Bot(){}

void Bot::parrot( string& msg, int fd, string hour )
{
    write(fd, GREEN, strlen(GREEN));
    write(fd, "[BOT PARROT🦜] ", 18);
    write(fd, RESET, strlen(RESET));
    write(fd, hour.c_str(), hour.length());
    write(fd, "\n", 1);
    write(fd, "You just said : ", 17);
    write(fd, msg.c_str(), msg.length());
    write(fd, "\n", 1);
}