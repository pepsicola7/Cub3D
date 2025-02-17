#include "cub3d.h"

int	init_data(t_data *data)
{
	data->mlx_data->mlx = mlx_init(WIN_WIDTH, WIN_HEIGHT, "cub3d", true);
	data->mlx_data->img = mlx_new_image(data->mlx_data->mlx, WIN_WIDTH,
			WIN_HEIGHT);
	if (data->map_data->map_fd == -1 || !data->mlx_data->mlx
		|| !data->mlx_data->img)
		return (-1);
	return (0);
}
