PROYECT_NAME        := ircserv
COMPILE      	    := c++
FLAGS_PARAMS       := -Wall -Wextra -Werror -std=c++98


SRCS        :=  src/main.cpp \
		src/Server.cpp \
		src/Admin.cpp \
		src/Bot.cpp \
		src/User.cpp \
		src/setUp.cpp \
		src/handler_irc.cpp \
		src/Server_getters.cpp \
		src/Server_tools_print.cpp \
		src/input_sanitizer.cpp \
		src/handle_admin/handler_admin.cpp \
		src/handle_admin/run_modes.cpp \
		src/handle_admin/run_features.cpp \
		src/handle_admin/admin_bot_comands.cpp \
		src/handle_users/handler_users.cpp \
		src/handle_users/user_run_features.cpp \
		src/handle_users/user_run_modes.cpp \
		src/handle_users/user_handle_states.cpp \
		src/handle_users/user_handle_client_disconnection.cpp

OBJS        := $(SRCS:src/%.cpp=objs/%.o)

CLR_RMV		:= \033[0m
RED			:= \033[1;31m
GREEN		:= \033[1;32m
YELLOW		:= \033[1;33m
BLUE		:= \033[1;34m
CYAN 		:= \033[1;36m
RM			:= rm -f
MKDIR		:= mkdir -p

$(PROYECT_NAME):	$(OBJS)
			@ echo "$(BOLD)$(RED)🛠️      Compiling ircserv    🛠️$(CLR_RMV)"
			@ ${COMPILE} ${FLAGS_PARAMS} -o ${PROYECT_NAME} $(OBJS)
			@ echo "$(BOLD)$(GREEN)🚀      ircserv fully compiled, ready to use       🚀$(CLR_RMV)✔️"
			@ echo "\n$(BOLD)Usage: ./ircserv <port> <password>$(CLR_RMV)"

all:		$(PROYECT_NAME)

clean:
			@ ${RM} -r objs
			@ echo "$(RED)Deleting $(CYAN)$(PROYECT_NAME) $(CLR_RMV)objs ✔️"

fclean:		clean
			@ ${RM} ${PROYECT_NAME}
			@ rm -rf objs
			@ echo "$(RED)Deleting $(CYAN)$(PROYECT_NAME) $(CLR_RMV)binary ✔️"

re:			fclean all

objs/%.o:	src/%.cpp
		@ ${MKDIR} $(dir $@)
		@ ${COMPILE} ${FLAGS_PARAMS} -I inc -c $< -o $@

.PHONY:		all clean fclean re