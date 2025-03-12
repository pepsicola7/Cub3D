/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_wall_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpolizzi <lpolizzi@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 12:24:15 by lpolizzi          #+#    #+#             */
/*   Updated: 2025/03/12 12:24:42 by lpolizzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

mlx_texture_t	*get_wall_texture(t_data *data, t_ray *ray)
{
	if (ray->hit == 2)
		return (data->texture->doors[(int)(mlx_get_time() * 8.0) % 6]);
	else if (ray->side == 0)
	{
		if (ray->step_x > 0)
			return (data->texture->east);
		else
			return (data->texture->west);
	}
	else
	{
		if (ray->step_y > 0)
			return (data->texture->south);
		else
			return (data->texture->north);
	}
}

void	init_draw_context(t_data *data, t_ray *ray, t_draw_context *ctx)
{
	ctx->line_height = (int)(data->mlx_data->mlx->height / ray->perp_wall_dist);
	ctx->draw_start = (data->mlx_data->mlx->height / 2) - (ctx->line_height / 2)
		+ data->player->camera_y_offset;
	ctx->draw_end = (data->mlx_data->mlx->height / 2) + (ctx->line_height / 2)
		+ data->player->camera_y_offset;
	if (ctx->draw_start < 0)
		ctx->draw_start = 0;
	if (ctx->draw_end >= data->mlx_data->mlx->height)
		ctx->draw_end = data->mlx_data->mlx->height;
	ctx->wall_texture = get_wall_texture(data, ray);
	if (ray->side == 0)
		ctx->wall_x = data->player->pos.y + ray->perp_wall_dist * ray->dir.y;
	else
		ctx->wall_x = data->player->pos.x + ray->perp_wall_dist * ray->dir.x;
	ctx->wall_x -= floor(ctx->wall_x);
	ctx->tex_x = (int)(ctx->wall_x * ctx->wall_texture->width);
	if ((ray->side == 0 && ray->dir.x > 0) || (ray->side == 1
			&& ray->dir.y < 0))
		ctx->tex_x = ctx->wall_texture->width - ctx->tex_x - 1;
	ctx->step = (float)ctx->wall_texture->height / ctx->line_height;
	ctx->tex_pos = (ctx->draw_start - data->player->camera_y_offset
			- data->mlx_data->mlx->height / 2 + ctx->line_height / 2)
		* ctx->step;
}

void	draw_wall(t_data *data, t_draw_context *ctx, int x)
{
	int			tex_y;
	uint8_t		*pixel;
	uint32_t	color;
	int			y;

	y = ctx->draw_start;
	while (y < ctx->draw_end)
	{
		tex_y = (int)ctx->tex_pos & (ctx->wall_texture->height - 1);
		ctx->tex_pos += ctx->step;
		pixel = ctx->wall_texture->pixels + (tex_y * ctx->wall_texture->width
				+ ctx->tex_x) * ctx->wall_texture->bytes_per_pixel;
		color = ((unsigned int)pixel[0] << 24)
			| ((unsigned int)pixel[1] << 16)
			| ((unsigned int)pixel[2] << 8)
			| ((unsigned int)pixel[3]);
		ft_put_pixel(data->mlx_data->img_buffer, x, y++, color);
	}
}
