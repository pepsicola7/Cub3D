#ifndef CUB3D_H
# define CUB3D_H

# include "../libft/libft.h"
# include "../MLX42/include/MLX42/MLX42.h"
# include <math.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

# define WIN_HEIGHT 1080
# define WIN_WIDTH 1920

typedef struct s_vec2
{
	double			x;
	double			y;
}					t_vec2;

typedef struct s_mlx
{
	mlx_t			*mlx;
	mlx_image_t		*img;
}					t_mlx;

typedef struct s_map
{
	char			**map;
	int				map_fd;
	int				ligne_total;
	int				width;
	int				height;
}					t_map;

typedef struct s_player
{
	t_vec2			pos;
	t_vec2			dir;
	t_vec2			plane;
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

int		init_data(t_data *data);
int		parsing(t_data *data, char *filename);
void	ft_printf_map(char **map);
int		read_map_1(t_data *data, char *filename);
int		get_color(char *line, t_data *data);
void	free_split(char **arr);
int		get_texture(char *line, t_data *data);
int		read_map_2(t_data *data, char *filename);
int		check_description(t_data *data);
void	count_line(t_data *data, char *filename);
void	count_hors_map(t_data *data, char *filename);
void	free_map(char **map);
void	map_width(t_data *data);
void	free_map_partial(char **map, int index);
char	**copy_map(char **map, int height);
void	free_map(char **map);
int		check_element(char **map);
int		check_joueur(char **map);
int		check_espace(char **map, t_data *data);
int		check_line_vide(char **map);
int		check_mur(t_data *data);
void	position_player(t_data *data);
void	flood_fill(char **map, int x, int y, t_data *data);
int		check_flood(char **map, int height);
void	direction_ew(t_data *data, char direction);
void	direction_ns(t_data *data, char direction);
#endif
