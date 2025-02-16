#ifndef CUB3D_H
# define CUB3D_H

# include "../MLX42/include/MLX42/MLX42.h"
# include "../libft/libft.h"
# include <fcntl.h>
# include <math.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

# define WIN_HEIGHT 1080
# define WIN_WIDTH 1920

# define BLUE 0x0000FFFF
# define GREEN 0x00FF00FF
# define RED 0xFF0000FF
# define WHITE 0xFFFFFFFF
# define BLACK 0x000000FF
# define YELLOW 0xFFFF00FF
# define CYAN 0x00FFFFFF
# define MAGENTA 0xFF00FFFF
# define ORANGE 0xFFA500FF
# define PURPLE 0x800080FF
# define PINK 0xFFC0CBFF
# define BROWN 0xA52A2AFF
# define GREY 0x808080FF

# define M_PI 3.14159265358979323846

typedef struct s_bresenham_params
{
	int				dx;
	int				dy;
	int				step_x;
	int				step_y;
	int				err;
	int				x;
	int				y;
}					t_bresenham_params;

typedef struct s_vec2d
{
	double			x;
	double			y;
}					t_vec2d;

typedef struct s_vec2i
{
	int				x;
	int				y;
}					t_vec2i;

typedef struct s_mlx
{
	mlx_t			*mlx;
	mlx_image_t		*img;
	int32_t		old_height;
	int32_t		old_width;
}					t_mlx;

typedef struct s_map
{
	char			**map;
	int				map_fd;
	int				width;
	int				height;
}					t_map;

typedef struct s_player
{
	t_vec2d			pos;
	int				rotation;
	bool			sprint;
	t_vec2d			last_pos;
	int				last_rotation;
}					t_player;

typedef struct s_texture_data
{
	mlx_texture_t	*north;
	mlx_texture_t	*south;
	mlx_texture_t	*east;
	mlx_texture_t	*west;
	int				floor_color;
	int				ceiling_color;
}					t_texture_data;

typedef struct s_data
{
	t_mlx			*mlx_data;
	t_map			*map_data;
	t_player		*player_data;
	t_texture_data	*texture_data;
}					t_data;

int					init_data(t_data *data, char *filename);

#endif
