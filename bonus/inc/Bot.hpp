#pragma once

#include "libs.hpp"

class Bot
{
    private:

        Bot( const Bot & );
        Bot& operator=( const Bot & );

    public:

        Bot();
        ~Bot();

        void parrot( std::string& msg, int fd, std::string hour );
};