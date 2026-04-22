NAME        = codexion

SRC_DIR     = srcs
INC_DIR     = include
OBJ_DIR     = .obj

FILES       = \
			codexion.c \
			init_variables.c \
			release.c \
			simulation.c \
            parse.c

SRCS        = $(addprefix $(SRC_DIR)/, $(FILES))
OBJS        = $(addprefix $(OBJ_DIR)/, $(FILES:.c=.o))
DEPS        = $(addprefix $(OBJ_DIR)/, $(FILES:.c=.d))

CC          = cc
CFLAGS      = -Wall -Wextra -Werror -MMD -I$(INC_DIR) -g3 -pthread -std=c89

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re