/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cast_ray.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpolizzi <lpolizzi@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 20:35:47 by lpolizzi          #+#    #+#             */
/*   Updated: 2025/03/11 20:35:48 by lpolizzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	perform_dda(t_data *data, t_ray *ray)
{
	ray->hit = 0;
	while (ray->hit == 0)
	{
		if (ray->side_dist.x < ray->side_dist.y)
		{
			ray->side_dist.x += ray->delta_dist.x;
			ray->map_x += ray->step_x;
			ray->side = 0;
		}
		else
		{
			ray->side_dist.y += ray->delta_dist.y;
			ray->map_y += ray->step_y;
			ray->side = 1;
		}
		if (get_map_value(data, ray->map_x, ray->map_y) != '0')
			ray->hit = 1;
	}
	if (ray->side == 0)
		ray->perp_wall_dist = (ray->side_dist.x - ray->delta_dist.x);
	else
		ray->perp_wall_dist = (ray->side_dist.y - ray->delta_dist.y);
}

void	init_ray(t_data *data, t_ray *ray, int x)
{
	float	camera_x;

	camera_x = 2 * x / (float)data->mlx_data->mlx->width - 1;
	ray->dir.x = data->player->dir.x + data->player->plane.x * camera_x;
	ray->dir.y = data->player->dir.y + data->player->plane.y * camera_x;
	ray->map_x = (int)data->player->pos.x;
	ray->map_y = (int)data->player->pos.y;
	ray->delta_dist.x = fabsf(1 / ray->dir.x);
	ray->delta_dist.y = fabsf(1 / ray->dir.y);
}

void	init_ray_steps(t_data *data, t_ray *ray)
{
	if (ray->dir.x < 0)
	{
		ray->step_x = -1;
		ray->side_dist.x = (data->player->pos.x - ray->map_x)
			* ray->delta_dist.x;
	}
	else
	{
		ray->step_x = 1;
		ray->side_dist.x = (ray->map_x + 1.0f - data->player->pos.x)
			* ray->delta_dist.x;
	}
	if (ray->dir.y < 0)
	{
		ray->step_y = -1;
		ray->side_dist.y = (data->player->pos.y - ray->map_y)
			* ray->delta_dist.y;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dist.y = (ray->map_y + 1.0f - data->player->pos.y)
			* ray->delta_dist.y;
	}
}

void	cast_ray(t_data *data, int x)
{
	t_ray	ray;

	init_ray(data, &ray, x);
	init_ray_steps(data, &ray);
	perform_dda(data, &ray);
	draw_textured_vertical_line(data, &ray, x);
}

void	render_raycast(void *param)
{
	t_data	*data;
	int		x;

	x = -1;
	data = (t_data *)param;
	while (++x < data->mlx_data->mlx->width)
		cast_ray(data, x);
}
