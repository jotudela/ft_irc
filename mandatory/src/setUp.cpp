#include "../inc/Server.hpp"

static Server* g_server_instance = NULL;

extern Server* g_server_instance;

void Server::sigint_handler( int signal )
{
	(void)signal;
	if (g_server_instance)
        g_server_instance->freeData();
	g_server_instance->printMsgAdmin(0, "Shutting down server.\r\n");
	g_server_instance->~Server();
    std::exit(0);
}

void Server::makeNonBlocking( int fd )
{
	fcntl(fd, F_SETFL, O_NONBLOCK);
}

void Server::setUpServer()
{
	// Création du socket d'écoute
	_serverFd = socket(AF_INET, SOCK_STREAM, 0);
	this->makeNonBlocking(_serverFd);

	_opt = 1;
	setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &_opt, sizeof(_opt));

	_serverAddr.sin_family = AF_INET;
	_serverAddr.sin_addr.s_addr = INADDR_ANY;
	_serverAddr.sin_port = htons(_port);

	bind(_serverFd, (sockaddr*)&_serverAddr, sizeof(_serverAddr));
	listen(_serverFd, SOMAXCONN);

	// Création de l'instance epoll
	_epollFD = epoll_create1(0);

	// Ajout du socket d'écoute à epoll
	_events[0].events = EPOLLIN;
	_events[0].data.fd = _serverFd;
	epoll_ctl(_epollFD, EPOLL_CTL_ADD, _serverFd, &_events[0]);

	// Ajout de l'entrée standard (fd 0)
	this->makeNonBlocking(STDIN_FILENO);
	epoll_event stdin_ev;
	memset(&stdin_ev, 0, sizeof(stdin_ev));
	stdin_ev.events = EPOLLIN;
	stdin_ev.data.fd = STDIN_FILENO;
	epoll_ctl(_epollFD, EPOLL_CTL_ADD, STDIN_FILENO, &stdin_ev);
	signal(SIGPIPE, SIG_IGN);
	g_server_instance = this;
	signal(SIGINT, Server::sigint_handler);
}

int Server::createNewUser()
{
	int client_fd = accept(_serverFd, NULL, NULL);
	if (client_fd == -1)
	{
		perror("accept");
		return -1;
	}

	makeNonBlocking(client_fd);

	epoll_event client_event;
	memset(&client_event, 0, sizeof(client_event));
	client_event.events = EPOLLIN | EPOLLET;
	client_event.data.fd = client_fd;

	if (epoll_ctl(_epollFD, EPOLL_CTL_ADD, client_fd, &client_event) == -1)
	{
		perror("epoll_ctl: client_fd");
		close(client_fd);
		return -1;
	}

	_users[client_fd] = new User();
	_userStates[client_fd] = WAIT_NICK;
	_tempNick[client_fd] = "";
	_tempUser[client_fd] = "";
	return 0;
}

int Server::userToStaff( int fd )
{
	// Vérifie que l'utilisateur existe
	if (_users.find(fd) == _users.end())
		return -1;

	User* user = _users[fd];

	// Crée un nouvel admin avec les infos de l'user
	Admin* staff = new Admin();
	staff->setStaffNames(user->getNickName(), user->getUserName());
	staff->setStaffStatus();
	staff->setStatus(true);
	staff->setTStatus(true);
	_staffStates[fd] = JOINED;

	// Ajoute dans le map des staffs
	_staffs[fd] = staff;

	// Supprime de la map des users
	delete _users[fd];
	_users.erase(fd);
	_tempNick.erase(fd);
	_tempUser.erase(fd);

	return 0;
}

int Server::staffToUser( int fd )
{
	if (_staffs.find(fd) == _staffs.end())
		return -1;

	Admin* staff = _staffs[fd];

	User* user = new User();
	user->setNickName(staff->getNickName());
	user->setUserName(staff->getUserName());
	user->setStatus(true);
	_users[fd] = user;

	delete _staffs[fd];
	_staffs.erase(fd);
	_staffStates.erase(fd);

	return 0;
}

void Server::freeData()
{
	// Libération des ressources
	for (std::map<int, User*>::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		close(it->first);               // Ferme la socket
		delete it->second;              // Supprime l'objet User
	}
	_users.clear();
	_userStates.clear();
	_tempNick.clear();
	_tempUser.clear();

	for (std::map<int, Admin*>::iterator it = _staffs.begin(); it != _staffs.end(); ++it)
	{
		close(it->first);              // Ferme la socket
		delete it->second;             // Supprime l'objet Admin
	}
	_staffs.clear();
	_staffStates.clear();
}