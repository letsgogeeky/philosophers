# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/12 20:26:46 by ramoussa          #+#    #+#              #
#    Updated: 2023/10/06 18:31:15 by ramoussa         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME:= philo
# B_NAME := philo_bonus

CFLAGS	:= -Wextra -Wall -Werror -g
BASELIB := ./lib/ft-baselib

HEADERS := -I ./philo/include -I ${BASELIB}/include
LIBS := ${BASELIB}/baselib.a

SRCS :=  main.c
# BONUS := bonus/checker.c
OBJS := ${addprefix philo/src/, ${SRCS:.c=.o}}
# BONUS_OBJS := ${addprefix src/, ${SRCS:.c=.o} ${BONUS:.c=.o}}

all: BASELIB ${NAME}

%.o: %.c
	@$(CC) $(CFLAGS) $(HEADERS) -o $@ -c $<

${NAME}: ${OBJS}
	@$(CC) $(CFLAGS) $(OBJS) $(LIBS) $(HEADERS) -o $(NAME) && echo "Successful build...!"

# ${B_NAME}: ${BONUS_OBJS}
# 	@$(CC) $(CFLAGS) $(BONUS_OBJS) $(LIBS) $(HEADERS) -o $(B_NAME) && echo "Bonus Successful build...!"

BASELIB:
	@if [ -d ${BASELIB} ]; then\
		echo "${BASELIB} already exists... proceeding to next step.";\
	else\
		mkdir lib;\
		git clone git@github.com:letsgogeeky/ft-baselib.git ${BASELIB};\
	fi
	make --directory=${BASELIB}

# bonus: BASELIB ${B_NAME}

clean:
	@rm -rf $(OBJS)
# @rm -rf ${BONUS_OBJS}

fclean: clean
	@if [ -d ${BASELIB} ]; then\
		make fclean --directory=${BASELIB};\
	fi
	rm -f philo/${NAME}
# rm -f ${B_NAME}

re: fclean all

.PHONY: all clean fclean re