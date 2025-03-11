/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_vertical_line.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpolizzi <lpolizzi@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 20:38:25 by lpolizzi          #+#    #+#             */
/*   Updated: 2025/03/11 21:03:43 by lpolizzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	draw_ceiling(t_data *data, int x, int draw_start)
{
	int	y;

	y = 0;
	while (y < draw_start)
		ft_put_pixel(data->mlx_data->img_buffer, x, y++,
			data->texture->ceiling_color);
}

void	draw_floor(t_data *data, int x, int draw_end)
{
	int	y;

	y = draw_end;
	while (y < data->mlx_data->mlx->height)
		ft_put_pixel(data->mlx_data->img_buffer, x, y++,
			data->texture->floor_color);
}

void	draw_textured_vertical_line(t_data *data, t_ray *ray, int x)
{
	t_draw_context	ctx;

	init_draw_context(data, ray, &ctx);
	draw_ceiling(data, x, ctx.draw_start);
	draw_wall(data, &ctx, x);
	draw_floor(data, x, ctx.draw_end);
}
