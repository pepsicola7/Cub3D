/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_floor_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpolizzi <lpolizzi@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 12:14:03 by lpolizzi          #+#    #+#             */
/*   Updated: 2025/03/12 12:24:40 by lpolizzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	calculate_floor_tex_coords(t_data *data, t_floor_ceiling *fc, int y)
{
	double	denominator;

	fc->adjusted_y = y - data->player->camera_y_offset;
	denominator = 2.0 * fc->adjusted_y - data->mlx_data->mlx->height;
	if (denominator == 0)
		fc->current_dist = 1;
	else
		fc->current_dist = data->mlx_data->mlx->height / denominator;
	if (fc->wall_dist == 0)
		fc->weight = 0.5;
	else
		fc->weight = fc->current_dist / fc->wall_dist;
	fc->floor_x = fc->weight * fc->wall_x + (1.0 - fc->weight)
		* data->player->pos.x;
	fc->floor_y = fc->weight * fc->wall_y + (1.0 - fc->weight)
		* data->player->pos.y;
	if (!isfinite(fc->floor_x))
		fc->floor_x = 0;
	if (!isfinite(fc->floor_y))
		fc->floor_y = 0;
	fc->tex_x = (int)((fc->floor_x - (int)fc->floor_x)
			* data->texture->floor->width);
	fc->tex_y = (int)((fc->floor_y - (int)fc->floor_y)
			* data->texture->floor->height);
}

void	draw_floor_pixel(t_data *data, t_floor_ceiling *fc, int x, int y)
{
	if (fc->tex_x < 0 || (uint32_t)fc->tex_x >= data->texture->floor->width
		|| fc->tex_y < 0 || (uint32_t)fc->tex_y >= data->texture->floor->height)
		return ;
	fc->pixel = data->texture->floor->pixels + (fc->tex_y
			* data->texture->floor->width + fc->tex_x)
		* data->texture->floor->bytes_per_pixel;
	fc->color = ((unsigned int)fc->pixel[0] << 24)
		| ((unsigned int)fc->pixel[1] << 16)
		| ((unsigned int)fc->pixel[2] << 8)
		| ((unsigned int)fc->pixel[3]);
	ft_put_pixel(data->mlx_data->img_buffer, x, y, fc->color);
}

void	draw_floor(t_data *data, t_ray *ray, int x, int draw_end)
{
	int				y;
	t_floor_ceiling	fc;

	set_wall_distance(data, ray, &fc);
	y = draw_end;
	while (y < data->mlx_data->mlx->height)
	{
		if (y - data->player->camera_y_offset <= 0)
		{
			y++;
			continue ;
		}
		calculate_floor_tex_coords(data, &fc, y);
		draw_floor_pixel(data, &fc, x, y);
		y++;
	}
}
