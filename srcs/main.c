#include "cub3d.h"

void free_tab(char **tab)
{
	int i;

	i = 0;
	while (tab && tab[i])
		free(tab[i++]);
	free(tab);
}

void free_data(t_data *data)
{
	free(data->mlx_data);
	free(data->player_data);
	free(data->texture_data);
	free_tab(data->map_data->map);
	free(data->map_data);
	free(data);
}

void exit_program(t_data *data, int status)
{
	if (data->map_data->map_fd != -1)
		close(data->map_data->map_fd);
	if (data->mlx_data->mlx)
		mlx_terminate(data->mlx_data->mlx);
	free_data(data);
	exit(status);
}

void draw_square(t_data *data, int x, int y, int size, int color)
{
	int i;
	int j;

	i = -1;
	while (++i < size)
	{
		j = -1;
		while (++j < size)
			mlx_put_pixel(data->mlx_data->img, x + i, y + j, color);
	}
}

void render_minimap(t_data *data)
{
	int square_size;
	int i;
	int j;

	square_size = data->mlx_data->mlx->width / 4 / data->map_data->width;
	i = 0;
	printf("square_size: %d\n", square_size);
	printf("width: %d\n", data->mlx_data->mlx->width);
	printf("height: %d\n", data->mlx_data->mlx->height);
	while (i < data->map_data->width)
	{
		j = 0;
		while (j < data->map_data->height)
		{
			printf("[%c]", data->map_data->map[j][i]);
			if (data->map_data->map[j][i] == '1')
				draw_square(data, i * square_size, j * square_size, square_size, 0xFF00FF00);
			else if (data->map_data->map[j][i] == '0')
				draw_square(data, i * square_size, j * square_size, square_size, 0xFFFF0000);
			j++;
		}
		printf("\n");
		i++;
	}
}

int main(int ac, char **av)
{
	t_data *data;

	mlx_set_setting(MLX_FULLSCREEN, false);
	if (ac < 2)
	{
		ft_putstr_fd("Error: No map file provided\n", 2);
		return (1);
	}
	data = ft_calloc(1, sizeof(t_data));
	if (!data)
		return (1);
	if (init_data(data, av[1]) == -1)
		exit_program(data, 1);
	mlx_image_to_window(data->mlx_data->mlx, data->mlx_data->img, 0, 0);
	render_minimap(data);
	mlx_loop(data->mlx_data->mlx);
	exit_program(data, 0);
}
