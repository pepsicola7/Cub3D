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
			mlx_put_pixel(data->mlx_data->img, pos.x + pixel.x, pos.y + pixel.y,
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
				mlx_put_pixel(data->mlx_data->img, pos.x + pixel.x, pos.y
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
		mlx_put_pixel(data->mlx_data->img, start.x, start.y, color);
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

void	draw_background(t_data *data, t_vec2i pos, int size)
{
	t_vec2i	pixel;

	pixel.y = 0;
	while (++pixel.y < size)
	{
		pixel.x = 0;
		while (++pixel.x < size)
			mlx_put_pixel(data->mlx_data->img, pos.x + pixel.x, pos.y + pixel.y,
				BLACK);
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
	t_map_drawing draw;

	// Calculate half of the minimap size for centering the player
	draw.half_display = MAP_SIZE / 2;

	// Calculate the top-left corner of the minimap (with respect to player's position)
	draw.top_left.x = (int)data->player_data->pos.x - draw.half_display;
	draw.top_left.y = (int)data->player_data->pos.y - draw.half_display;

	// Clamp the minimap's top-left position to prevent scrolling beyond the map's edges
	if (draw.top_left.x < 0)
		draw.top_left.x = 0;
	if (draw.top_left.y < 0)
		draw.top_left.y = 0;

	// Ensure the minimap doesn't scroll beyond the bottom/right edge of the map
	if (draw.top_left.x + MAP_SIZE > data->map_data->width)
		draw.top_left.x = data->map_data->width - MAP_SIZE;
	if (draw.top_left.y + MAP_SIZE > data->map_data->height)
		draw.top_left.y = data->map_data->height - MAP_SIZE;

	// Calculate fractional part of player's position for smooth scrolling
	draw.pixel_offset.x = (int)((data->player_data->pos.x - (int)data->player_data->pos.x) * SQUARE_SIZE);
	draw.pixel_offset.y = (int)((data->player_data->pos.y - (int)data->player_data->pos.y) * SQUARE_SIZE);

	// Minimap screen offset (adjusted to reduce padding)
	draw.offset.x = 5; // Reduced padding
	draw.offset.y = 5; // Reduced padding

	// Draw background
	draw_background(data, draw.offset, MAP_SIZE * SQUARE_SIZE);

	// Iterate through the grid and draw each tile
	draw.grid.y = -1;
	while (++(draw.grid.y) < MAP_SIZE)
	{
		draw.grid.x = -1;
		while (++(draw.grid.x) < MAP_SIZE)
		{
			// Calculate map coordinates for each grid tile
			draw.map_x = draw.top_left.x + draw.grid.x;
			draw.map_y = draw.top_left.y + draw.grid.y;

			// Get the map index (ensure it's within bounds)
			draw.map_index = get_map_index(data, draw.map_x, draw.map_y);
			if (draw.map_index == -1) {
				// Skip this tile if the index is invalid
				draw.grid.x++;
				continue;
			}

			// Calculate the screen position of the current tile
			draw.pos.x = draw.offset.x + draw.grid.x * SQUARE_SIZE - draw.pixel_offset.x;
			draw.pos.y = draw.offset.y + draw.grid.y * SQUARE_SIZE - draw.pixel_offset.y;

			// Draw the map square based on the map data
			if (data->map_data->map[draw.map_index] == '1')
				draw_square(data, draw.pos, SQUARE_SIZE, GREY); // No padding for squares
			else if (data->map_data->map[draw.map_index] == '0')
				draw_square(data, draw.pos, SQUARE_SIZE, BLUE); // No padding for squares
		}
	}

	// Draw the player on the minimap (small square, centered)
	draw_player(data, draw.offset, 7); // Small player square with no padding
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
