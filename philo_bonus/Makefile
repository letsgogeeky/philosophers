# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/12 20:26:46 by ramoussa          #+#    #+#              #
#    Updated: 2023/10/14 13:12:33 by ramoussa         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME:= philo_bonus

CFLAGS	:= -Wextra -Wall -Werror -g
HEADERS := -I ./include
SRCS :=  main.c
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