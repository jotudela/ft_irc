# 🚀 INTRODUCTION

The `ft_irc` project challenges students to developing an IRC (Internet Relay Chat) server in C++98.
The goal is to understand how a network protocol works, manage multiple clients simultaneously, and apply concepts of system programming and sockets.

![](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

## 🛠️ Program Specificities and Considerations

In this project a lot of things :
- develop an IRC server in C++98.
- don't need to develop a client.
- don't need to manage server-to-server communication.
- binary should be called like this `./ircserv <port> <password>`
- Communication between the client and the server will be done in TCP/IP (v4 or v6).
- However, only the following features are required:
[Tabulation] - You must be able to authenticate, set a nickname and username, join a channel, and send and receive private messages with your reference client.
[Tabulation] - All messages sent by a customer in a channel must be forwarded to all customers who have joined that channel.
[Tabulation] - You must have basic operators and users.
[Tabulation] - You must therefore implement the channel operator-specific commands.
[Tabulation] - `KICK` - Kick a client from the channel
[Tabulation] - `INVITE` - Invite a customer to the channel
[Tabulation] - `TOPIC` - Change or view the channel theme
[Tabulation] - `MODE` - Change the channel mode:
[Tabulation] [Tabulation] - `i`: Set/delete invite-only channel
[Tabulation] [Tabulation] - `t`: Set/remove TOPIC command restrictions for channel operators
[Tabulation] [Tabulation] - `k`: Set/delete channel key (password)
[Tabulation] [Tabulation] - `o`: Grant/withdraw channel operator privilege
[Tabulation] [Tabulation] - `l`: Set/remove user limit for channel

## ⚙️ Usage

For test my program, the simple way is to use `netcat`, so 
