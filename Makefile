NAME    = minishell
SRCS = main.c add_token.c env.c herdooc.c cd_builtins.c env_builtins.c pipe.c \
       export_builtins.c free.c check_quotes.c expand.c helper.c helper1.c \
       Redirection.c syntax.c token.c pwd_builtins.c unset_builtins.c  \
       execute_builtin.c  utils.c ft_splitc.c signal.c echo_builtins.c  \
	   utild_echo.c exit_builtins.c execute_external.c utils_pipe.c

OBJS    = $(SRCS:.c=.o)

CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -g
all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -lreadline

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re