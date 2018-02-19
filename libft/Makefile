# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bpierce <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/04/25 10:27:14 by bpierce           #+#    #+#              #
#    Updated: 2017/07/04 18:18:42 by aderby           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

C_FIL = *.c
C_LOC = src/
C_SRC = $(addprefix $(C_LOC), $(C_FIL))

O_FIL = *.o
O_LOC = obj/
O_SRC = $(addprefix $(O_LOC), $(O_FIL))

H_SRC = inc/

all: $(NAME)

$(NAME):
	@gcc -c -Wall -Wextra -Werror -I $(H_SRC) $(C_SRC)
	@mkdir $(O_LOC)
	@ar rcs $(NAME) $(O_FIL)
	@mv $(O_FIL) $(O_LOC)

clean:
	@/bin/rm -f $(O_SRC)
	@/bin/rm -rf $(O_LOC)

fclean: clean
	@/bin/rm -f $(NAME)

re: fclean all
