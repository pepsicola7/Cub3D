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

void	draw_square(t_data *data, t_vec2i pos, int size, int color)
{
	t_vec2i	pixel;

	pixel.y = 0;
	while (++pixel.y < size - 1)
	{
		pixel.x = 0;
		while (++pixel.x < size - 1)
			mlx_put_pixel(data->mlx_data->img, pos.x + pixel.x, pos.y + pixel.y,
				color);
	}
}

void	draw_background(t_data *data, t_vec2i offset, int size)
{
	t_vec2i	pixel;

	pixel.y = -1;
	while (++pixel.y < data->map_data->height * size)
	{
		pixel.x = -1;
		while (++pixel.x < data->map_data->width * size)
			mlx_put_pixel(data->mlx_data->img, offset.x + pixel.x, offset.y
				+ pixel.y, BLACK);
	}
}

void	get_minimap_size(t_data *data, int *square_size, t_vec2i *offset)
{
	*square_size = (data->mlx_data->mlx->width / 6) / data->map_data->width;
	if (*square_size < 2)
		*square_size = 2;
	offset->x = data->mlx_data->mlx->width - data->map_data->width
		* *square_size - 20;
	offset->y = 20;
}

void	draw_player(t_data *data, t_vec2i offset, int square_size)
{
	t_vec2d	pos;
	t_vec2d	dir;
	t_vec2i	draw_pos;
	t_vec2i	line_end;

	pos.x = offset.x + data->player_data->pos.x * square_size;
	pos.y = offset.y + data->player_data->pos.y * square_size;
	dir.x = cos(data->player_data->rotation * M_PI / 180) * 4;
	dir.y = sin(data->player_data->rotation * M_PI / 180) * 4;
	draw_pos.x = (int)pos.x - 4;
	draw_pos.y = (int)pos.y - 4;
	line_end.x = (int)(pos.x + dir.x);
	line_end.y = (int)(pos.y + dir.y);
	draw_square(data, draw_pos, 8, GREEN);
}

void	render_minimap(t_data *data)
{
	int		square_size;
	t_vec2i	offset;
	t_vec2i	grid;
	t_vec2i	pos;

	get_minimap_size(data, &square_size, &offset);
	draw_background(data, offset, square_size);
	grid.y = -1;
	printf("Map size: %d x %d\n", data->map_data->width,
		data->map_data->height);
	printf("Square size: %d\n", square_size);
	printf("Offset: %d, %d\n", offset.x, offset.y);
	printf("Window size: %d x %d\n", data->mlx_data->mlx->width,
		data->mlx_data->mlx->height);
	while (++grid.y < data->map_data->height)
	{
		grid.x = -1;
		while (++grid.x < data->map_data->width)
		{
			pos.x = offset.x + grid.x * square_size;
			pos.y = offset.y + grid.y * square_size;
			if (data->map_data->map[grid.y][grid.x] == '1')
				draw_square(data, pos, square_size, GREY);
			else if (data->map_data->map[grid.y][grid.x] == '0')
				draw_square(data, pos, square_size, BROWN);
		}
	}
	draw_player(data, offset, square_size);
	mlx_image_to_window(data->mlx_data->mlx, data->mlx_data->img, 0, 0);
}

void	move_player(t_data *data, double speed, double y, double x)
{
	t_vec2d	new_pos;

	new_pos.x = data->player_data->pos.x + x * speed
		* data->mlx_data->mlx->delta_time;
	new_pos.y = data->player_data->pos.y + y * speed
		* data->mlx_data->mlx->delta_time;
	if (data->map_data->map[(int)new_pos.y][(int)data->player_data->pos.x]
		== '0')
		data->player_data->pos.y = new_pos.y;
	if (data->map_data->map[(int)data->player_data->pos.y][(int)new_pos.x]
		== '0')
		data->player_data->pos.x = new_pos.x;
}

void	render_all(void *vdata)
{
	t_data	*data;

	data = (t_data *)vdata;
	ft_memset(data->mlx_data->img->pixels, 255, data->mlx_data->img->width
		* data->mlx_data->img->height * sizeof(int32_t));
	render_minimap(data);
}

void	key_handling(struct mlx_key_data key_data, void *vdata)
{
	t_data	*data;

	data = (t_data *)vdata;
	printf("Key pressed: %d\n", key_data.key);
	// Debug print to get the key code
	if (key_data.key == MLX_KEY_ESCAPE)
		exit_program(data, 0);
	else if (key_data.key == MLX_KEY_F11)
		mlx_set_setting(MLX_FULLSCREEN, !data->mlx_data->fullscreen);
	else if (key_data.key == MLX_KEY_UP)
		move_player(data, 0.1, cos(data->player_data->rotation * M_PI / 180),
			sin(data->player_data->rotation * M_PI / 180));
	else if (key_data.key == MLX_KEY_DOWN)
		move_player(data, -0.1, cos(data->player_data->rotation * M_PI / 180),
			sin(data->player_data->rotation * M_PI / 180));
	else if (key_data.key == MLX_KEY_LEFT)
		data->player_data->rotation -= 5;
	else if (key_data.key == MLX_KEY_RIGHT)
		data->player_data->rotation += 5;
	render_all(data);
}

int	main(int ac, char **av)
{
	t_data	*data;

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
	render_all(data);
	mlx_key_hook(data->mlx_data->mlx, key_handling, data);
	mlx_loop_hook(data->mlx_data->mlx, render_all, data);
	mlx_loop(data->mlx_data->mlx);
	exit_program(data, 0);
}
