#pragma once


#define IRC_MESSAGE_MAX 512
#define MAX_EVENTS 1000

#define VIOLET "\033[35m"
#define BLEU "\033[34m"
#define CYAN "\033[36m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define RESET "\033[0m"


#include <sys/socket.h>     // socket(), bind(), accept(), setsockopt()
#include <netinet/in.h>     // sockaddr_in, htons(), htonl(), ntohs(), ntohl()
#include <arpa/inet.h>      // inet_addr(), inet_ntoa()
#include <netdb.h>          // gethostbyname(), getaddrinfo()


#include <unistd.h>         // read(), write(), close()
#include <fcntl.h>          // fcntl() — pour O_NONBLOCK
#include <sys/types.h>      // types généraux (socklen_t, etc.)
#include <sys/stat.h>       // fstat()
#include <signal.h>         // signal(), sigaction()
#include <sys/epoll.h>      // epoll_*


#include <iostream>         // std::cout, std::cerr
#include <string>           // std::string
#include <vector>           // std::vector
#include <map>              // std::map
#include <cstring>          // std::memset, std::strerror, std::strcmp
#include <cstdlib>          // std::exit, std::atoi
#include <csignal>          // std::signal
#include <ctime>            // std::time (pour gestion d'inactivité)


#include <cerrno>           // errno
#include <cstdio>
#include <sstream>
#include <algorithm>