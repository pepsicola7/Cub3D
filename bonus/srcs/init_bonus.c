/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpolizzi <lpolizzi@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 11:36:10 by lpolizzi          #+#    #+#             */
/*   Updated: 2025/03/12 11:36:11 by lpolizzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_player_jump(t_player *player)
{
	player->gravity = 15.0f;
	player->jump_force = 6.0f;
	player->vertical_velocity = 0.0f;
	player->is_jumping = 0;
	player->is_grounded = 1;
	player->jump_height = 0.5f;
	player->base_height = 0.0f;
	player->current_height = 0.0f;
	player->ground_level = 0.0f;
	player->can_jump = true;
}

int	init_mlx(t_data *data)
{
	data->mlx_data = ft_calloc(1, sizeof(t_mlx));
	data->mlx_data->mlx = mlx_init(WIN_WIDTH, WIN_HEIGHT, "cub3d", true);
	data->mlx_data->img = mlx_new_image(data->mlx_data->mlx,
			data->mlx_data->mlx->width, data->mlx_data->mlx->height);
	data->mlx_data->img_buffer = mlx_new_image(data->mlx_data->mlx,
			data->mlx_data->mlx->width, data->mlx_data->mlx->height);
	if (!data->mlx_data->mlx || !data->mlx_data->img)
		return (0);
	return (1);
}
