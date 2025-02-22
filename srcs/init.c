#include "cub3d.h"

void	example_map(t_data *data)
{
	data->map_data->map = ft_strdup("1111111111000000011000000011000000011000000011000000011000000011000000011111111111");
	data->map_data->width = 9;
	data->map_data->height = 9;
	data->map_data->tile_size = 64;
	data->player_data->pos_x = 3.5;
	data->player_data->pos_y = 3.5;
	data->player_data->rotation = 0;
	data->mlx_data->old_height = data->mlx_data->mlx->height;
	data->mlx_data->old_width = data->mlx_data->mlx->width;
	data->player_data->sprint = false;
}

int	init_data(t_data *data, char *filename)
{
	data->mlx_data = ft_calloc(1, sizeof(t_mlx));
	data->map_data = ft_calloc(1, sizeof(t_map));
	data->player_data = ft_calloc(1, sizeof(t_player));
	data->texture_data = ft_calloc(1, sizeof(t_texture_data));
	data->map_data->map_fd = open(filename, O_RDONLY);
	data->mlx_data->mlx = mlx_init(WIN_WIDTH, WIN_HEIGHT, "cub3d", true);
	data->mlx_data->img = mlx_new_image(data->mlx_data->mlx, WIN_WIDTH,
			WIN_HEIGHT);
	if (data->map_data->map_fd == -1 || !data->mlx_data->mlx
		|| !data->mlx_data->img)
		return (-1);
	example_map(data);
	return (0);
}
