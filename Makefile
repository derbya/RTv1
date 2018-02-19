# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aderby <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/07/08 14:20:14 by aderby            #+#    #+#              #
#    Updated: 2017/09/06 14:34:04 by aderby           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = RTv1

C_FIL = main.c parse_data.c additional_parsing.c hooks.c

O_FIL = *.o
O_LOC = obj

GCC_FLAGS = gcc -L miniLibx -lmlx -framework OpenGL \
			-framework AppKit

LIB = libft/libft.a

all: $(NAME)

$(NAME):
		cd libft && make re
		cd miniLibx && make re
			$(GCC_FLAGS) $(LIB) $(C_SRC) $(C_FIL) -o $(NAME)

clean:
		cd libft && make clean

fclean: clean
		cd libft && make fclean
			/bin/rm -f $(NAME)

re: fclean all
