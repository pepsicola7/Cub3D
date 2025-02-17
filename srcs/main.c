/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpolizzi <lpolizzi@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 15:12:44 by lpolizzi          #+#    #+#             */
/*   Updated: 2025/02/16 17:01:33 by lpolizzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void	draw_circle(t_data *data, t_vec2i pos, int radius, int color)
{
	t_vec2i	pixel;
	float	dist;

	pixel.y = -radius;
	while (++pixel.y < radius)
	{
		pixel.x = -radius;
		while (++pixel.x < radius)
		{
			dist = sqrt(pixel.x * pixel.x + pixel.y * pixel.y);
			if (dist < radius)
				mlx_put_pixel(data->mlx_data->img, pos.x + pixel.x, pos.y
					+ pixel.y, color);
		}
	}
}

void	init_bresenham_params(t_bresenham_params *params, t_vec2i start,
		t_vec2i end)
{
	params->dx = abs(end.x - start.x);
	params->dy = abs(end.y - start.y);
	if (start.x < end.x)
		params->step_x = 1;
	else
		params->step_x = -1;
	if (start.y < end.y)
		params->step_y = 1;
	else
		params->step_y = -1;
	params->x = start.x;
	params->y = start.y;
	params->err = params->dx - params->dy;
}

void	draw_line(t_data *data, t_vec2i start, t_vec2i end, int color)
{
	t_bresenham_params	params;
	int					err2;

	init_bresenham_params(&params, start, end);
	while (params.x != end.x || params.y != end.y)
	{
		mlx_put_pixel(data->mlx_data->img, params.x, params.y, color);
		err2 = params.err * 2;
		if (err2 > -params.dy)
		{
			params.err -= params.dy;
			params.x += params.step_x;
		}
		if (err2 < params.dx)
		{
			params.err += params.dx;
			params.y += params.step_y;
		}
	}
	mlx_put_pixel(data->mlx_data->img, params.x, params.y, color);
}

void	draw_player(t_data *data, t_vec2i offset, int square_size)
{
	t_vec2f	pos;
	t_vec2f	dir;
	t_vec2i	center;
	t_vec2i	line_end;

	pos.x = offset.x + data->player_data->pos.x * square_size;
	pos.y = offset.y + data->player_data->pos.y * square_size;
	center.x = (int)pos.x;
	center.y = (int)pos.y;
	dir.x = cos(data->player_data->rotation) * 12;
	dir.y = sin(data->player_data->rotation) * 12;
	line_end.x = center.x + (int)dir.x;
	line_end.y = center.y + (int)dir.y;
	draw_line(data, center, line_end, RED);
	draw_circle(data, center, square_size / 10, GREEN);
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
				draw_square(data, pos, square_size, BLUE);
		}
	}
	draw_player(data, offset, square_size);
	mlx_image_to_window(data->mlx_data->mlx, data->mlx_data->img, 0, 0);
}

void	move_player(t_data *data, float speed, float y, float x)
{
	t_vec2f	new_pos;

	new_pos.x = data->player_data->pos.x + x * speed;
	new_pos.y = data->player_data->pos.y + y * speed;
	if (new_pos.x < 0 || new_pos.y < 0 || new_pos.x >= data->map_data->width
		|| new_pos.y >= data->map_data->height)
		return ;
	if (data->map_data->map[(int)new_pos.y][(int)data->player_data->pos.x] == '0')
		data->player_data->pos.y = new_pos.y;
	if (data->map_data->map[(int)data->player_data->pos.y][(int)new_pos.x] == '0')
		data->player_data->pos.x = new_pos.x;
}

void	render_all(void *vdata)
{
	t_data	*data;

	data = (t_data *)vdata;
	if (data->mlx_data->mlx->width != data->mlx_data->old_width
		|| data->mlx_data->mlx->height != data->mlx_data->old_height)
	{
		data->mlx_data->old_height = data->mlx_data->mlx->height;
		data->mlx_data->old_width = data->mlx_data->mlx->width;
		ft_memset(data->mlx_data->img->pixels, 255, data->mlx_data->img->width
			* data->mlx_data->img->height * sizeof(int32_t));
		render_minimap(data);
	}
	else if (data->player_data->last_pos.x != data->player_data->pos.x
		|| data->player_data->last_pos.y != data->player_data->pos.y
		|| data->player_data->last_rotation != data->player_data->rotation)
	{
		render_minimap(data);
		data->player_data->last_pos = data->player_data->pos;
		data->player_data->last_rotation = data->player_data->rotation;
		// raycaster(data);
	}
}

void	key_handling(struct mlx_key_data key_data, void *vdata)
{
	t_data	*data;
	float	move_speed;

	data = (t_data *)vdata;
	move_speed = 0.2 * (2 * data->player_data->sprint + 1)
		* data->mlx_data->mlx->delta_time;
	if (key_data.key == MLX_KEY_ESCAPE)
		exit_program(data, 0);
	else if (key_data.key == MLX_KEY_LEFT_SHIFT)
		data->player_data->sprint = !data->player_data->sprint;
	else if (key_data.key == MLX_KEY_UP)
		move_player(data, move_speed, sin(data->player_data->rotation),
			cos(data->player_data->rotation));
	else if (key_data.key == MLX_KEY_DOWN)
		move_player(data, -move_speed, sin(data->player_data->rotation),
			cos(data->player_data->rotation));
	else if (key_data.key == MLX_KEY_LEFT)
		data->player_data->rotation -= 0.0174533;
	else if (key_data.key == MLX_KEY_RIGHT)
		data->player_data->rotation += 0.0174533;
	if (data->player_data->rotation < 0)
		data->player_data->rotation += 2 * M_PI;
	else if (data->player_data->rotation >= 2 * M_PI)
		data->player_data->rotation -= 2 * M_PI;
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
	render_minimap(data);
	mlx_key_hook(data->mlx_data->mlx, key_handling, data);
	mlx_loop_hook(data->mlx_data->mlx, render_all, data);
	mlx_loop(data->mlx_data->mlx);
	free_data(data);
}
