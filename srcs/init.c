#include "cub3d.h"

int	init_mlx(t_data *data)
{
	data->mlx_data = ft_calloc(1, sizeof(t_mlx));
	data->mlx_data->mlx = mlx_init(WIN_WIDTH, WIN_HEIGHT, "cub3d", true);
	data->mlx_data->img = mlx_new_image(data->mlx_data->mlx, WIN_WIDTH,
			WIN_HEIGHT);
	if (!data->mlx_data->mlx || !data->mlx_data->img)
		return (0);
	return (1);
}
