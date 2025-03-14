BLUE  	:= \033[1;34m
GREEN 	:= \033[1;32m
RED   	:= \033[1;31m
PURPLE 	:= \033[1;35m
CYAN  	:= \033[1;36m
YELLOW	:= \033[1;33m
RESET 	:= \033[0m

NAME    := cub3d
CFLAGS  := -Wall -Wextra -Werror -O3 -ffast-math -march=native -flto -fno-math-errno -funroll-loops -fno-stack-protector -fomit-frame-pointer #-g3 -fsanitize=address,leak,undefined
LIBMLX  := MLX42
LIBFT   := libft

HEADERS := -I./includes -I $(LIBMLX)/include
HEADERS_BONUS := -I./bonus/includes -I $(LIBMLX)/include
LIBS    := $(LIBFT)/libft.a $(LIBMLX)/build/libmlx42.a -ldl -lglfw -lm

SRC_DIR     := srcs/
PARSING_DIR := srcs/parsing/
BONUS_DIR   := bonus/srcs/
BONUS_PARSING_DIR := bonus/srcs/parsing_bonus/

SRCS := $(addprefix $(SRC_DIR), $(addsuffix .c, \
        init cast_ray render_utils main draw_vertical_line draw_wall movements map_and_color_utils handle_keys)) \
        $(addprefix $(PARSING_DIR), $(addsuffix .c, \
        parsing parser_texture parser_map utils check_map parser_util position_player))

SRCS_BONUS := $(addprefix $(BONUS_DIR), $(addsuffix .c, \
        camera_bonus cast_ray_bonus draw_ceiling_bonus draw_floor_bonus draw_vertical_line_bonus draw_wall_bonus \
        handle_keypress_bonus init_bonus main_bonus map_utils_bonus movements_bonus render_utils_bonus minimap_bonus sprites_bonus)) \
        $(addprefix $(BONUS_PARSING_DIR), $(addsuffix .c, \
        check_map parser_map parser_texture parser_util parsing position_player utils))

OBJS    := $(SRCS:.c=.o)
OBJS_BONUS := $(SRCS_BONUS:.c=.o)

CC      := cc

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
