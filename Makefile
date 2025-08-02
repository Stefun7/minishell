CC = cc
CFLAGS = -Wall -Werror -Wextra
LIBFT = ./Libft/libft.a
SRCS = \
	src/access.c \
	src/builtins_1.c \
	src/builtins_2.c \
	src/builtins_3.c \
	src/builtins_4.c \
	src/checker.c \
	src/checker2.c \
	src/envp_utils_1.c \
	src/envp_utils_2.c \
	src/expansion_utils.c \
	src/family_1.c \
	src/family_2.c \
	src/free_all.c \
	src/free_utils.c \
	src/free_utlis_2.c \
	src/ft_split_shell_1.c \
	src/ft_split_shell_2.c \
	src/heredoc.c \
	src/heredoc2.c \
	src/init_env.c \
	src/init_instr.c \
	src/init_redir_1.c \
	src/init_redir_2.c \
	src/loc_var_1.c \
	src/loc_var_2.c \
	src/main.c \
	src/path_utils.c \
	src/path.c \
	src/prompt.c \
	src/redir_perm.c \
	src/signals_1.c \
	src/signals_2.c \
	src/tiny.c \
	src/tiny2.c \
	src/tokenization.c \
	src/tokenizer_help.c \
	src/utils_token.c \
	src/utils.c \

INC = inc
NAME = minishell

OBJ = $(SRCS:.c=.o)

all: $(NAME)

$(LIBFT):
	@make -C ./Libft

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(OBJ) ${LIBFT} -o $(NAME) -lreadline

%.o: %.c
	${CC} -I${INC} -c -o $@ $<

clean:
	rm -rf $(OBJ)
	@make clean -C ./Libft

fclean: clean
	rm -rf $(NAME)
	@make fclean -C ./Libft

re: fclean all

# Mark these as not actual files
.PHONY: fclean clean all re
