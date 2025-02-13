#include "cub3d.h"

char **example_map(void)
{
	char **map = ft_calloc(5, sizeof(char *));
	map[0] = ft_strdup("11111");
	map[1] = ft_strdup("10001");
	map[2] = ft_strdup("10001");
	map[3] = ft_strdup("10001");
	map[4] = ft_strdup("11111");
	return (map);
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
	data->map_data->map = example_map();
	return (0);
}
