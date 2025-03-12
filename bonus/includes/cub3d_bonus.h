/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpolizzi <lpolizzi@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 11:34:10 by lpolizzi          #+#    #+#             */
/*   Updated: 2025/03/12 11:34:12 by lpolizzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_BONUS_H
# define CUB3D_BONUS_H

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

# define WIN_HEIGHT 1080
# define WIN_WIDTH 1920

# define FOV 60
# define MOVE_SPEED 1.0
# define ROTATE_SPEED 1.0
# define MOUSE_SENSITIVITY 0.1
# define TILT_SPEED 400
# define JUMP_SPEED 125
# define NUM_FRAMES 6

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
	char			*map_1d;
	char			**map;
	int				map_fd;
	int				ligne_total;
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
	bool			tab;
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
	mlx_texture_t	*doors[NUM_FRAMES];
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
void				init_player_jump(t_player *player);
int					init_mlx(t_data *data);

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
void				handle_cursor(double xpos, double ypos, void *param);

/*-----------------Rendering-------------------*/

void				draw_textured_vertical_line(t_data *data, t_ray *ray,
						int x);
void				set_wall_distance(t_data *data, t_ray *ray,
						t_floor_ceiling *fc);
void				init_draw_context(t_data *data, t_ray *ray,
						t_draw_context *ctx);
void				draw_ceiling(t_data *data, t_ray *ray, int x,
						int draw_start);
void				draw_wall(t_data *data, t_draw_context *ctx, int x);
void				draw_floor(t_data *data, t_ray *ray, int x, int draw_end);

void				render_raycast(void *param);
void				render_minimap(t_data *data);
void				init_door_textures(t_data *data);

int					init_data(t_data *data);
int					parsing(t_data *data, char *filename);
int					read_map_1(t_data *data, char *filename);
int					get_color(char *line, t_data *data);
void				free_split(char **arr);
int					get_texture(char *line, t_data *data);
int					read_map_2(t_data *data, char *filename);
int					check_description(t_data *data);
void				count_line(t_data *data, char *filename);
void				count_hors_map(t_data *data, char *filename);
void				free_map(char **map);
void				map_width(t_data *data);
void				free_map_partial(char **map, int index);
char				**copy_map(t_data *data, char **map, int height);
void				free_map(char **map);
int					check_element(char **map);
int					check_joueur(char **map);
int					check_espace(char **map, t_data *data);
int					check_line_vide(t_data *data, char **map);
int					check_mur(t_data *data);
void				position_player(t_data *data);
void				direction_ew(t_data *data, char direction);
void				direction_ns(t_data *data, char direction);
void				flood_fill(char **map, int x, int y, t_data *data);
int					check_flood(char **map, int height);
void				fill_map(t_data *data, char *line);
int					help_read_map_2(t_data *data, char **l, int *config_count);
#endif
