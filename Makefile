# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/12 20:26:46 by ramoussa          #+#    #+#              #
#    Updated: 2023/10/14 13:13:23 by ramoussa         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME:= philo

CFLAGS	:= -Wextra -Wall -Werror -g
HEADERS := -I ./philo/include -I
SRCS :=  main.c
OBJS := ${addprefix philo/src/, ${SRCS:.c=.o}}

all: ${NAME}

%.o: %.c
	@$(CC) $(CFLAGS) $(HEADERS) -o $@ -c $<

${NAME}: ${OBJS}
	@$(CC) $(CFLAGS) $(OBJS) $(LIBS) $(HEADERS) -o $(NAME) && echo "Successful build...!"


clean:
	@rm -rf $(OBJS)

fclean: clean
	rm -f philo/${NAME}
# rm -f ${B_NAME}

re: fclean all

.PHONY: all clean fclean re