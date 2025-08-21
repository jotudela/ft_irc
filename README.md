# 🚀 INTRODUCTION

The `ft_irc` project challenges students to developing an IRC (Internet Relay Chat) server in C++98.
The goal is to understand how a network protocol works, manage multiple clients simultaneously, and apply concepts of system programming and sockets.

![](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

## 🛠️ Program Specificities and Considerations

In this project, we need to develop a lot of things :  
- Develop an IRC server in C++98.  
- Don't need to develop a client.  
- Don't need to manage server-to-server communication.  
- Binary should be called like this: `./ircserv <port> <password>`  
- Communication between the client and the server will be done in TCP/IP (v4 or v6).  
- However, only the following features are required:  
  - You must be able to authenticate, set a nickname and username, join a channel, and send and receive private messages with your reference client.  
  - All messages sent by a customer in a channel must be forwarded to all customers who have joined that channel.  
  - You must have basic operators and users.  
  - You must therefore implement the channel operator-specific commands:  
    - `KICK` – Kick a client from the channel  
    - `INVITE` – Invite a customer to the channel  
    - `TOPIC` – Change or view the channel theme  
    - `MODE` – Change the channel mode:  
      - `i`: Set/delete invite-only channel  
      - `t`: Set/remove TOPIC command restrictions for channel operators  
      - `k`: Set/delete channel key (password)  
      - `o`: Grant/withdraw channel operator privilege  
      - `l`: Set/remove user limit for channel  

and we need to manage `Ctrl D`, for example :
```bash
KI^DCK us^Der6
```

this command must works.

In bonus part we just include a parrot bot, wich repeat what you send in a canal.

And operaors can activate or desactivate the bot with :
| Command | Brief |
|---------|-------|
| `BOT ON` | Activate bot if he is DESACTIVATED |
| `BOT OFF` | Desactivate bot if he is ACTIVATED |

## ⚙️ Usage

Start with cloning my repo :
```bash
git clone git@github.com:jotudela/ft_irc.git cd ft_irc
```

For test my program, the simple way is to use `netcat`, so install with this :
```bash
sudo apt update
sudo apt install netcat
```

or with this :
```bash
sudo apt update
sudo apt install netcat-openbsd
```

Now compile the project :
```bash
cd bonus && make
```

and execute like this :
```bash
./ircserv 8080 password
```

at this moment you are now the first operator of the canal, so open a new terminal and type this :
```bash
nc localhost 8080
```

actually you need to do this (in your new terminal):
- 1 `NICK (your nick name)`
- 2 `USER (your user name)`
- 3 `JOIN (the channel)`
- 4 `type the password`

Now you can chat with the first operator !

Here are representation of what you should have:

My final grade :

![](imgs/110_percent.png)

![](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

## 🤝 Contribution
Contributions are open, make a pull request or open an issue 🚀
