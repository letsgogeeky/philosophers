# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/12 20:26:46 by ramoussa          #+#    #+#              #
#    Updated: 2023/11/02 21:46:18 by ramoussa         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME:= philo_bonus

CFLAGS	:= -Wextra -Wall -Werror -g
HEADERS := -I ./include
SRCS := init.c action.c logger.c \
	offload.c state.c \
	memory.c time.c main.c \
	worker.c sadphilo.c dispatcher.c
OBJS := ${addprefix src/, ${SRCS:.c=.o}}

all: ${NAME}

%.o: %.c
	@$(CC) $(CFLAGS) $(HEADERS) -o $@ -c $<

${NAME}: ${OBJS}
	@$(CC) $(CFLAGS) $(OBJS) $(HEADERS) -o $(NAME) && echo "Successful build...!"

clean:
	@rm -rf $(OBJS)

fclean: clean
	rm -f ${NAME}

re: fclean all

.PHONY: all clean fclean re
