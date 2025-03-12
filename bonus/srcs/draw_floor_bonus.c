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
	fc->adjusted_y = y - data->player->camera_y_offset;
	fc->current_dist = data->mlx_data->mlx->height / (2.0 * fc->adjusted_y
			- data->mlx_data->mlx->height);
	fc->weight = fc->current_dist / fc->wall_dist;
	fc->floor_x = fc->weight * fc->wall_x + (1.0 - fc->weight)
		* data->player->pos.x;
	fc->floor_y = fc->weight * fc->wall_y + (1.0 - fc->weight)
		* data->player->pos.y;
	fc->tex_x = (int)(fc->floor_x * data->texture->floor->width)
		% data->texture->floor->width;
	fc->tex_y = (int)(fc->floor_y * data->texture->floor->height)
		% data->texture->floor->height;
	if (fc->tex_x < 0)
		fc->tex_x += data->texture->floor->width;
	if (fc->tex_y < 0)
		fc->tex_y += data->texture->floor->height;
}

void	draw_floor_pixel(t_data *data, t_floor_ceiling *fc, int x, int y)
{
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
