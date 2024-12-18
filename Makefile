# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hfattah <hfattah@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/03 10:15:55 by hfattah           #+#    #+#              #
#    Updated: 2024/12/14 11:41:41 by hfattah          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

AR = ar rcs
CFLAGS = -Wall -Wextra -Werror -g
RM = rm -f
CC = gcc
SRC_DIR = src
OBJ_DIR = obj
LIBFT = libft/libft.a
NAME = minishell
LIBFT_DIR = libft
LIBFT_OBJ_DIR = libft/obj

GREEN = \033[0;92m
RESET = \033[0m

SRC = main.c builtins.c ft_strtrim_all.c exec.c helper.c         \
      fill_node.c get_params.c ft_cmdtrim.c unset_export.c \
      expand.c heredoc.c error.c env.c get_user.c       \
      get_next_line.c get_next_line_utils.c get_user_home.c    \
      ft_cmdsubsplit.c signal.c parse_args.c get_cmd.c expand_path.c

OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

all: $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -lreadline -o $@
	@echo "$(GREEN)minishell is up to date!$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT): | $(LIBFT_OBJ_DIR)
	@make -C $(LIBFT_DIR)

clean: | $(LIBFT_DIR)
	@make clean -C $(LIBFT_DIR)
	@$(RM) -r $(OBJ_DIR)

fclean: clean
	@$(RM) $(LIBFT)
	@$(RM) $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(LIBFT_OBJ_DIR):
	@mkdir -p $(LIBFT_OBJ_DIR)

re: fclean
	@make all

.PHONY: all clean fclean re