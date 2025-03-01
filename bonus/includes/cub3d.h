#ifndef CUB3D_H
# define CUB3D_H

# include "../../MLX42/include/MLX42/MLX42.h"
# include "../../libft/libft.h"
# include <fcntl.h>
# include <math.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

# define WIN_HEIGHT 600
# define WIN_WIDTH 600

# define FOV 60
# define MOVE_SPEED 1.0
# define ROTATE_SPEED 1.0
# define TILT_SPEED 400
# define JUMP_SPEED 75.0

# define BLUE 0x0000FFFF
# define LIGHT_BLUE 0xADD8F6FF
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

typedef struct s_floor_ceiling
{
	float			wall_x;
	float			wall_y;
	float			wall_dist;
	float			weight;
	float			current_dist;
	float			floor_x;
	float			floor_y;
	float			adjusted_y;
	int				tex_x;
	int				tex_y;
	uint8_t			*pixel;
	uint32_t		color;
}					t_floor_ceiling;

typedef struct s_draw_context
{
	int				line_height;
	int				draw_start;
	int				draw_end;
	mlx_texture_t	*wall_texture;
	float			wall_x;
	int				tex_x;
	float			step;
	float			tex_pos;
}					t_draw_context;

typedef struct s_ray
{
	t_vec2f			dir;
	t_vec2f			side_dist;
	t_vec2f			delta_dist;
	int				map_x;
	int				map_y;
	int				step_x;
	int				step_y;
	int				hit;
	int				side;
	float			perp_wall_dist;
}					t_ray;

typedef struct s_bresenham
{
	int				delta_x;
	int				delta_y;
	int				step_x;
	int				step_y;
	int				error;
	int				error_adjustment;
}					t_bresenham;

typedef struct s_distance
{
	float			ax;
	float			ay;
	float			bx;
	float			by;
}					t_distance;

typedef struct s_mlx
{
	mlx_t			*mlx;
	mlx_image_t		*img;
	mlx_image_t		*img_buffer;
}					t_mlx;

typedef struct s_map
{
	char			*map;
	int				width;
	int				height;
}					t_map;

typedef struct s_key_state
{
	bool			w;
	bool			a;
	bool			s;
	bool			d;
	bool			left;
	bool			right;
	bool			up;
	bool			down;
	bool			shift;
	bool			space;
	bool			control;
}					t_key_state;

typedef struct s_player
{
	t_vec2f			pos;
	t_vec2f			dir;
	t_vec2f			plane;
	int				camera_y_offset;
	float			ws_speed;
	float			ad_speed;
	t_key_state		key_state;
	float			gravity;
	float			jump_force;
	float			vertical_velocity;
	int				is_jumping;
	int				is_grounded;
	float			jump_height;
	float			base_height;
	float			current_height;
	float			ground_level;
	bool			can_jump;

}					t_player;

typedef struct s_texture_data
{
	mlx_texture_t	*north;
	mlx_texture_t	*south;
	mlx_texture_t	*east;
	mlx_texture_t	*west;
	mlx_texture_t	*floor;
	mlx_texture_t	*ceiling;
}					t_texture_data;

typedef struct s_data
{
	t_mlx			*mlx_data;
	t_map			*map_data;
	t_player		*player;
	t_texture_data	*texture;
}					t_data;

/*-----------------Exit Program----------------*/

void				exit_program(t_data *data, int status);

/*----------------Initialization---------------*/

int					init_data(t_data *data);

/*------------------Math Utils-----------------*/

float				deg_to_rad(int degre);
int					get_map_value(t_data *data, int x, int y);

/*---------------Rendering Utils---------------*/

void				ft_put_pixel(mlx_image_t *img, uint32_t x, uint32_t y,
						uint32_t color);
void				draw_line(t_data *data, t_vec2i start, t_vec2i end,
						int color);
void				draw_circle(t_data *data, t_vec2i pos, int radius,
						int color);
void				draw_background(t_data *data, t_vec2i offset, int size);
void				draw_square(t_data *data, t_vec2i pos, int size, int color);

/*----------------Key Handling-----------------*/

void				key_callback(mlx_key_data_t keydata, void *vdata);
void				handle_movement(t_data *data);
void				handle_strafe(t_data *data);
void				handle_rotation(t_data *data);
void				handle_camera_tilt(t_data *data);
void				handle_jump(t_data *data, float delta_time);

/*-----------------Rendering-------------------*/

void				render_raycast(void *param);
void				render_minimap(t_data *data);

#endif
