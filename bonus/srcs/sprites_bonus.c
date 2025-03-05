#include "cub3d.h"

void	init_door_textures(t_data *data)
{
	mlx_texture_t **doors;

	doors = data->texture->doors;
	doors[0] = mlx_load_png("textures/door1.png");
	doors[1] = mlx_load_png("textures/door2.png");
	doors[2] = mlx_load_png("textures/door3.png");
	doors[3] = mlx_load_png("textures/door4.png");
	doors[4] = mlx_load_png("textures/door5.png");
	doors[5] = mlx_load_png("textures/door6.png");
}
