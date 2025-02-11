#include "cub3d.h"

int init_mlx(t_mlx *data)
{
	data->mlx = mlx_init();
	if (data->mlx == NULL)
		return (-1);
	data->win = mlx_new_window(data->mlx, WIN_WIDTH, WIN_HEIGHT, "cub3d");
	if (data->win == NULL)
		return (-1);
	data->img = mlx_new_image(data->mlx, WIN_WIDTH, WIN_HEIGHT);
	if (data->img == NULL)
		return (-1);
	data->addr = mlx_get_data_addr(data->img, &data->bits_per_pixel,
			&data->line_length, &data->endian);
	if (data->addr == NULL)
		return (-1);
	return (0);
}

int init_data(t_data *data, char *filename)
{
	data->map_fd = open(filename, O_RDONLY);
	if (data->map_fd == -1 || init_mlx(data->mlx_data) == -1)
		return (-1);
	return (0);
}
