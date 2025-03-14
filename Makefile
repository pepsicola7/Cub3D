BLUE  	:= \033[1;34m
GREEN 	:= \033[1;32m
RED   	:= \033[1;31m
PURPLE 	:= \033[1;35m
CYAN  	:= \033[1;36m
YELLOW	:= \033[1;33m
RESET 	:= \033[0m

NAME    := cub3d
CFLAGS  := -Wall -Wextra -Werror -O3 -march=native -flto -fno-math-errno -funroll-loops -fno-stack-protector -fomit-frame-pointer -g3 -fsanitize=address,leak,undefined
LIBMLX  := MLX42
LIBFT   := libft

HEADERS := -I./includes -I $(LIBMLX)/include
HEADERS_BONUS := -I./bonus/includes -I $(LIBMLX)/include
LIBS    := $(LIBFT)/libft.a $(LIBMLX)/build/libmlx42.a -ldl -lglfw -lm

SRCS    := srcs/init.c srcs/cast_ray.c srcs/render_utils.c srcs/main.c srcs/draw_vertical_line.c srcs/draw_wall.c srcs/movements.c srcs/map_and_color_utils.c srcs/handle_keys.c srcs/parsing/parsing.c srcs/parsing/parser_texture.c srcs/parsing/parser_map.c srcs/parsing/printf.c srcs/parsing/check_map.c srcs/parsing/parser_util.c srcs/parsing/position_player.c
SRCS_BONUS := bonus/srcs/camera_bonus.c bonus/srcs/cast_ray_bonus.c bonus/srcs/draw_ceiling_bonus.c bonus/srcs/draw_floor_bonus.c bonus/srcs/draw_vertical_line_bonus.c bonus/srcs/draw_wall_bonus.c bonus/srcs/handle_keypress_bonus.c bonus/srcs/init_bonus.c bonus/srcs/main_bonus.c bonus/srcs/map_utils_bonus.c bonus/srcs/movements_bonus.c bonus/srcs/render_utils_bonus.c bonus/srcs/minimap_bonus.c bonus/srcs/sprites_bonus.c bonus/srcs/parsing_bonus/check_map.c  bonus/srcs/parsing_bonus/parser_map.c  bonus/srcs/parsing_bonus/parser_texture.c  bonus/srcs/parsing_bonus/parser_util.c  bonus/srcs/parsing_bonus/parsing.c  bonus/srcs/parsing_bonus/position_player.c  bonus/srcs/parsing_bonus/printf.c

OBJS    := $(SRCS:.c=.o)
OBJS_BONUS := $(SRCS_BONUS:.c=.o)

CC      := clang

all: libft libmlx $(NAME)

libft:
	@if [ ! -d "$(LIBFT)" ]; then \
		echo "$(CYAN)Cloning libft...$(RESET)"; \
		git clone https://github.com/LeoFalkins/Libft.git $(LIBFT) > /dev/null; \
		echo "$(GREEN)Libft cloned successfully!$(RESET)"; \
	fi
	@echo "$(CYAN)Building libft...$(RESET)"
	@make -C $(LIBFT) -j4 > /dev/null
	@echo "$(GREEN)Libft built successfully!$(RESET)"

libmlx:
	@if [ ! -d "$(LIBMLX)" ]; then \
		echo "$(CYAN)Cloning MLX42...$(RESET)"; \
		git clone https://github.com/codam-coding-college/MLX42.git $(LIBMLX) > /dev/null; \
		echo "$(GREEN)MLX42 cloned successfully!$(RESET)"; \
	fi
	@echo "$(CYAN)Building MLX42...$(RESET)"
	@cmake $(LIBMLX) -B $(LIBMLX)/build > /dev/null && make -C $(LIBMLX)/build -j4 > /dev/null
	@echo "$(GREEN)MLX42 built successfully!$(RESET)"

%.o: %.c
	@$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS) && printf "$(YELLOW)Compiling: $(PURPLE)$(notdir $<)\n$(RESET)"

bonus/%.o: bonus/%.c
	@$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS_BONUS) && printf "$(YELLOW)Compiling Bonus: $(PURPLE)$(notdir $<)\n$(RESET)"

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBS) $(HEADERS) -o $(NAME)
	@echo "$(GREEN)$(NAME) compiled successfully!$(RESET)"

bonus: libft libmlx $(OBJS_BONUS)
	@echo "$(CYAN)Compiling $(NAME) with bonus...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS_BONUS) $(LIBS) $(HEADERS_BONUS) -o $(NAME)
	@echo "$(GREEN)$(NAME) compiled successfully!$(RESET)"

dev: fclean libft libmlx bonus
	@echo "$(CYAN)Compiling $(NAME) with sanitizers (bonus included)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS_BONUS) $(LIBS) $(HEADERS_BONUS) -o $(NAME)
	@echo "$(GREEN)$(NAME) compiled successfully!$(RESET)"
	@echo "$(RED)Development mode enabled!$(RESET)"

clean:
	@echo "$(CYAN)Cleaning object files...$(RESET)"
	@rm -rf $(OBJS) $(OBJS_BONUS)
	@echo "$(GREEN)Object files cleaned successfully!$(RESET)"

fclean: clean
	@echo "$(CYAN)Cleaning libft...$(RESET)"
	@echo "$(GREEN)Libft cleaned successfully!$(RESET)"
	@echo "$(CYAN)Cleaning MLX42...$(RESET)"
	@echo "$(GREEN)MLX42 cleaned successfully!$(RESET)"
	@echo "$(CYAN)Cleaning $(NAME)...$(RESET)"
	@rm -f $(NAME)
	@echo "$(GREEN)$(NAME) cleaned successfully!$(RESET)"

re: fclean all

rebonus: fclean bonus

help:
	@echo "$(CYAN)Available commands:$(RESET)"
	@echo "$(YELLOW)make$(RESET) - compiles the project"
	@echo "$(YELLOW)make bonus$(RESET) - compiles the project with bonus source files"
	@echo "$(YELLOW)make dev$(RESET) - compiles the project with bonus + sanitizers"
	@echo "$(YELLOW)make clean$(RESET) - removes object files"
	@echo "$(YELLOW)make fclean$(RESET) - removes object files, libraries and the executable"
	@echo "$(YELLOW)make re$(RESET) - removes object files, libraries and the executable, then recompiles the project"
	@echo "$(YELLOW)make help$(RESET) - displays this message"

.PHONY: all clean fclean re libft libmlx bonus dev help rebonus
