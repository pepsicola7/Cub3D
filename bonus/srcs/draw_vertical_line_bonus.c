/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_vertical_line_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpolizzi <lpolizzi@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 12:24:17 by lpolizzi          #+#    #+#             */
/*   Updated: 2025/03/12 12:24:43 by lpolizzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	set_wall_distance(t_data *data, t_ray *ray, t_floor_ceiling *fc)
{
	if (ray->side == 0)
		fc->wall_dist = (ray->map_x - data->player->pos.x + (1 - ray->step_x)
				/ 2) / ray->dir.x;
	else
		fc->wall_dist = (ray->map_y - data->player->pos.y + (1 - ray->step_y)
				/ 2) / ray->dir.y;
	fc->wall_x = data->player->pos.x + fc->wall_dist * ray->dir.x;
	fc->wall_y = data->player->pos.y + fc->wall_dist * ray->dir.y;
}

void	draw_textured_vertical_line(t_data *data, t_ray *ray, int x)
{
	t_draw_context	ctx;

	init_draw_context(data, ray, &ctx);
	draw_ceiling(data, ray, x, ctx.draw_start);
	draw_wall(data, &ctx, x);
	draw_floor(data, ray, x, ctx.draw_end);
}
