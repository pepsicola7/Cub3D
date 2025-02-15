#include "cub3d.h"

void example_map(t_data *data)
{
	data->map_data->map = ft_calloc(6, sizeof(char *));
	data->map_data->map[0] = ft_strdup("11111");
	data->map_data->map[1] = ft_strdup("10001");
	data->map_data->map[2] = ft_strdup("10001");
	data->map_data->map[3] = ft_strdup("10001");
	data->map_data->map[4] = ft_strdup("11111");
	data->map_data->width = 5;
	data->map_data->height = 5;
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
