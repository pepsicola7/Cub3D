#include "cub3d.h"

void ft_put_pixel(t_data *data, int x, int y, int color)
{
	if (x < 0 || x >= (int)data->mlx_data->img->width || y < 0
		|| y >= (int)data->mlx_data->img->height)
		return ;
	*(int *)(data->mlx_data->img->pixels + (y * data->mlx_data->img->width
		+ x)) = color;
}
