/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpolizzi <lpolizzi@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 11:35:54 by lpolizzi          #+#    #+#             */
/*   Updated: 2025/03/12 11:35:58 by lpolizzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	ft_put_pixel(mlx_image_t *image, uint32_t x, uint32_t y, uint32_t color)
{
	uint8_t	*pixel;

	if (!image || !image->pixels || x >= image->width || y >= image->height)
		return ;
	pixel = &image->pixels[(y * image->width + x) * sizeof(uint32_t)];
	pixel[0] = (uint8_t)((color >> 24) & 0xFF);
	pixel[1] = (uint8_t)((color >> 16) & 0xFF);
	pixel[2] = (uint8_t)((color >> 8) & 0xFF);
	pixel[3] = (uint8_t)(color & 0xFF);
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
				ft_put_pixel(data->mlx_data->img_buffer, pos.x + pixel.x, pos.y
					+ pixel.y, color);
		}
	}
}

static void	init_bresenham(t_bresenham *bres, t_vec2i start, t_vec2i end)
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
		ft_put_pixel(data->mlx_data->img_buffer, start.x, start.y, color);
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

void	draw_square(t_data *data, t_vec2i pos, int size, int color)
{
	t_vec2i	pixel;

	pixel.y = 0;
	while (++pixel.y < size - 1)
	{
		pixel.x = 0;
		while (++pixel.x < size - 1)
			ft_put_pixel(data->mlx_data->img_buffer, pos.x + pixel.x, pos.y
				+ pixel.y, color);
	}
}
