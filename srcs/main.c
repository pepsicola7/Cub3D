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

void	free_data(t_data *data)
{
	free(data->mlx_data);
	free(data->player_data);
	free(data->texture_data);
	free(data->map_data->map);
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
			ft_put_pixel(data, pos.x + pixel.x, pos.y + pixel.y,
				color);
	}
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
				ft_put_pixel(data, pos.x + pixel.x, pos.y
				  + pixel.y, color);
		}
	}
}

void	init_bresenham(t_bresenham *bres, t_vec2i start, t_vec2i end)
{
	bres->delta_x = abs(end.x - start.x);
	bres->delta_y = abs(end.y - start.y);
	if (start.x < end.x)
		bres->step_x = 1;
	else
		bres->step_x = -1;
	if (start.y < end.y)
		bres->step_y = 1;
	else
		bres->step_y = -1;
	bres->error = bres->delta_x - bres->delta_y;
}

void	draw_line(t_data *data, t_vec2i start, t_vec2i end, int color)
{
	t_bresenham	bres;

	init_bresenham(&bres, start, end);
	while (1)
	{
		ft_put_pixel(data, start.x, start.y, color);
		if (start.x == end.x && start.y == end.y)
			break ;
		bres.error_adjustment = 2 * bres.error;
		if (bres.error_adjustment > -bres.delta_y)
		{
			bres.error -= bres.delta_y;
			start.x += bres.step_x;
		}
		if (bres.error_adjustment < bres.delta_x)
		{
			bres.error += bres.delta_x;
			start.y += bres.step_y;
		}
	}
}

void	draw_background(t_data *data, t_vec2i pos, t_vec2i end, int color)
{
	int tmp_x;

	tmp_x = pos.x;
	while (pos.y < end.y)
	{
		pos.x = tmp_x;
		while (pos.x < end.x)
			ft_put_pixel(data, pos.x++, pos.y, color);
		pos.y++;
	}
}

void	draw_player(t_data *data, t_vec2i pos, int size)
{
	t_vec2i	center;
	t_vec2i	end;

	center.x = pos.x + size / 2;
	center.y = pos.y + size / 2;
	end.x = center.x + cos(deg_to_rad(data->player_data->rotation)) * size / 2;
	end.y = center.y + sin(deg_to_rad(data->player_data->rotation)) * size / 2;
	draw_line(data, center, end, RED);
	draw_circle(data, center, size / 2, GREEN);
}

int	get_map_index(t_data *data, int x, int y)
{
	if (x < 0 || x >= data->map_data->width || y < 0
		|| y >= data->map_data->height)
		return (-1);
	return (y * data->map_data->width + x);
}

void render_minimap(t_data *data)
{
	t_vec2i	pos;
	int		index;

	pos.y = 0;
	while (pos.y < data->map_data->height)
	{
		pos.x = 0;
		while (pos.x < data->map_data->width)
		{
			index = get_map_index(data, pos.x, pos.y);
			if (data->map_data->map[index] == '1')
				draw_square(data, pos, data->map_data->tile_size, GREY);
			else if (data->map_data->map[index] == '0')
				draw_square(data, pos, data->map_data->tile_size, BLUE);
			pos.x += data->map_data->tile_size;
		}
		pos.y += data->map_data->tile_size;
	}
	/*draw_player(data, data->player_data->pos, data->map_data->tile_size);*/
}

void render_all(void *vdata)
{
	t_data	*data;

	data = (t_data *)vdata;
	ft_memset(data->mlx_data->img->pixels, 255, data->mlx_data->img->width
		* data->mlx_data->img->height * sizeof(int32_t));
	render_minimap(data);
	mlx_image_to_window(data->mlx_data->mlx, data->mlx_data->img, 0, 0);
}

void	key_handling(struct mlx_key_data key_data, void *vdata)
{
	t_data	*data;

	data = (t_data *)vdata;
	if (key_data.key == MLX_KEY_ESCAPE)
		exit_program(data, 0);
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
