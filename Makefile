.PHONY: all re clean fclean

NAME = ft_traceroute

HEADER = ft_traceroute.h
SRC_PATH = ./srcs/
OBJ_PATH = ./objs/
LIBFT_PATH = ./libft/
LIBFT = $(LIBFT_PATH)libft.a
LIBFT_INC = -L $(LIBFT_PATH) -lft
INCLUDE = -I ./incs/ -I $(LIBFT_PATH)incs

CC = gcc

COLOR = echo

CC_FLAGS = -Wall -Werror -Wextra -g

SRC = 	main.c \

OBJ = $(addprefix $(OBJ_PATH), $(SRC:.c=.o))

all: $(NAME)

$(NAME): $(OBJ)
	@make -C $(LIBFT_PATH)
	@$(CC) $(CC_FLAGS) $(OBJ) -L $(LIBFT_PATH) -lft -o $(NAME)
	@printf "\033[1mCompilation de \033[36m$@%-26s\033[0m \033[1men \033[32m$@%-20s\033[0m ✅\n"

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	@mkdir -p $(OBJ_PATH)
	@$(CC) $(CC_FLAGS) $(INCLUDE) -o $@ -c $<
	@printf "\033[1mCompilation de \033[36m%-25s\033[37m\ten \033[32m%-26s\033[0m ✅\n" $< $@

clean:
	@/bin/rm -rf $(OBJ_PATH)
	@make -C $(LIBFT_PATH) clean

fclean: clean
	@/bin/rm -f $(NAME)
	@make -C $(LIBFT_PATH) fclean

re: fclean all
