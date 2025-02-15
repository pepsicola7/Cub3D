#include "cub3d.h"

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab && tab[i])
		free(tab[i++]);
	free(tab);
}

void	free_data(t_data *data)
{
	free(data->mlx_data);
	free(data->player_data);
	free(data->texture_data);
	free_tab(data->map_data->map);
	free(data->map_data);
	free(data);
}

void	exit_program(t_data *data, int status)
{
	if (data->map_data->map_fd != -1)
		close(data->map_data->map_fd);
	if (data->mlx_data->mlx)
		mlx_terminate(data->mlx_data->mlx);
	free_data(data);
	exit(status);
}

void	draw_square(t_data *data, int x, int y, int size, int color)
{
	int	row;
	int	col;

	row = 0;
	while (++row < size - 1)
	{
		col = 0;
		while (++col < size - 1)
			mlx_put_pixel(data->mlx_data->img, x + row, y + col, color);
	}
}

void	draw_background(t_data *data, int x_offset, int y_offset, int size)
{
	int	row;
	int	col;

	row = -1;
	while (++row < data->map_data->width * size)
	{
		col = -1;
		while (++col < data->map_data->height * size)
			mlx_put_pixel(data->mlx_data->img, x_offset + row, y_offset + col,
				0x000000FF);
	}
}

void	get_minimap_size(t_data *data, int *square_size, int *x_offset)
{
	*square_size = (data->mlx_data->mlx->width / 5) / data->map_data->width;
	if (*square_size < 2)
		*square_size = 2;
	*x_offset = data->mlx_data->mlx->width + 20;
}

void	render_minimap(t_data *data)
{
	int	square_size;
	int	x_offset;
	int	y_offset;
	int	row;
	int	col;

	y_offset = 20;
	get_minimap_size(data, &square_size, &x_offset);
	draw_background(data, x_offset, y_offset, square_size);
	row = -1;
	while (++row < data->map_data->width)
	{
		col = -1;
		while (++col < data->map_data->height)
		{
			if (data->map_data->map[col][row] == '1')
				draw_square(data, x_offset + row * square_size, y_offset + col
					* square_size, square_size, 0x909090FF);
			else if (data->map_data->map[col][row] == '0')
				draw_square(data, x_offset + row * square_size, y_offset + col
					* square_size, square_size, 0xFF0000FF);
		}
	}
	mlx_image_to_window(data->mlx_data->mlx, data->mlx_data->img, 0, 0);
}

int	main(int ac, char **av)
{
	t_data	*data;

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
	ft_memset(data->mlx_data->img->pixels, 255, data->mlx_data->img->width
		* data->mlx_data->img->height * sizeof(int32_t));
	render_minimap(data);
	mlx_loop(data->mlx_data->mlx);
	exit_program(data, 0);
}
