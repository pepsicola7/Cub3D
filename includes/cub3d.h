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

# define FOV 60
# define PI_2 M_PI / 2
# define PI_3 M_PI * 3 / 2
# define DEGRE 0.0174533

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

typedef struct s_ray
{
	int				rays;
	int				map_x;
	int				map_y;
	int				map_1d;
	int				depth_of_field;
	float			ray_x;
	float			ray_y;
	float			ray_angle;
	float			x_offset;
	float			y_offset;
	float			distance;
}					t_ray;

typedef struct s_vec2f
{
	float			x;
	float			y;
}					t_vec2f;

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
	t_vec2f			pos;
	float			rotation;
	bool			sprint;
	t_vec2f			last_pos;
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
void				raycaster(t_data *data);
void				draw_line(t_data *data, t_vec2i start, t_vec2i end, int color);

#endif
