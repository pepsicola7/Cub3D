BLUE  = \033[1;34m
GREEN = \033[1;32m
RED   = \033[1;31m
RESET = \033[0m

NAME = cub3d

SRCS =

OBJS = $(SRCS:.c=.o)

MLX_DIR = MLX42
MLX_LIB = $(MLX_DIR)/build/libmlx42.a

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

CC = cc
CFLAGS = -Wall -Wextra -Werror -O3 -I./includes -L./libft -lft -L./MLX42 -lmlx42

dev: CFLAGS += -fsanitize=address,leak,undefined -g3

dev: re
	@echo "$(RED)Cleaning object files for dev mode...$(RESET)"
	@rm -f $(OBJS)
	@echo "$(GREEN)Object files cleaned!$(RESET)"
	@echo "$(RED)Development mode enabled!$(RESET)"

all: $(NAME)

$(NAME): $(OBJS) $(MLX_LIB) $(LIBFT)
	@echo "$(BLUE)Compiling $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS)
	@echo "$(GREEN)$(NAME) compiled successfully!$(RESET)"

$(LIBFT):
	@if [ ! -d "$(LIBFT_DIR)" ]; then echo "$(BLUE)Cloning libft...$(RESET)" && git clone https://github.com/LeoFalkins/Libft.git $(LIBFT_DIR) && echo "$(GREEN)Libft cloned successfully!$(RESET)"; fi
	@echo "$(BLUE)Compiling libft...$(RESET)"
	@make -C libft bonus > /dev/null
	@echo "$(GREEN)Libft compiled successfully!$(RESET)"

$(MLX_LIB):
	@if [ ! -d "$(MLX_DIR)" ]; then echo "$(BLUE)Cloning MLX42...$(RESET)" && git clone https://github.com/codam-coding-college/MLX42.git $(MLX_DIR) && echo "$(GREEN)MLX42 cloned successfully!$(RESET)"; fi
	@echo "$(BLUE)Building MLX42...$(RESET)"
	@cmake -B $(MLX_DIR)/build -S $(MLX_DIR) > /dev/null
	@cmake --build $(MLX_DIR)/build --config Release > /dev/null
	@echo "$(GREEN)MLX42 compiled successfully!$(RESET)"

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "$(BLUE)Cleaning object files...$(RESET)"
	@rm -f $(OBJS)
	@echo "$(GREEN)Object files cleaned!$(RESET)"

fclean: clean
	@echo "$(BLUE)Cleaning $(NAME)...$(RESET)"
	@rm -f $(NAME)
	@echo "$(GREEN)$(NAME) cleaned!$(RESET)"

re: fclean all

.PHONY: all clean fclean re dev $(MLX_LIB)
