/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movements_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpolizzi <lpolizzi@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 12:24:06 by lpolizzi          #+#    #+#             */
/*   Updated: 2025/03/12 12:24:43 by lpolizzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	make_player_jump(t_data *data)
{
	data->player->is_jumping = 1;
	data->player->is_grounded = 0;
	data->player->vertical_velocity = data->player->jump_force;
	data->player->can_jump = false;
}

void	handle_jump(t_data *data, float d_time)
{
	float	previous_height;
	float	height_delta;

	previous_height = data->player->current_height;
	if (data->player->key_state.space && data->player->is_grounded
		&& data->player->can_jump)
		make_player_jump(data);
	if (!data->player->key_state.space)
		data->player->can_jump = true;
	if (!data->player->is_grounded)
	{
		data->player->vertical_velocity -= data->player->gravity * d_time;
		data->player->current_height += data->player->vertical_velocity
			* d_time;
		if (data->player->current_height <= data->player->ground_level)
		{
			data->player->current_height = data->player->ground_level;
			data->player->is_grounded = 1;
			data->player->is_jumping = 0;
			data->player->vertical_velocity = 0.0f;
		}
		height_delta = data->player->current_height - previous_height;
		data->player->camera_y_offset += height_delta * JUMP_SPEED;
	}
}

void	handle_movement(t_data *data)
{
	t_vec2f	new_pos;
	float	speed;
	int		map_value;

	if (data->player->key_state.w == data->player->key_state.s)
		return ;
	speed = MOVE_SPEED * data->mlx_data->mlx->delta_time;
	if (data->player->key_state.w)
		speed *= 1.0f;
	if (data->player->key_state.s)
		speed *= -1.0f;
	if (data->player->key_state.shift)
		speed *= 2.0f;
	new_pos.x = data->player->pos.x + data->player->dir.x * speed;
	new_pos.y = data->player->pos.y + data->player->dir.y * speed;
	map_value = get_map_value(data, (int)new_pos.x, (int)data->player->pos.y);
	if (map_value == '0' || map_value == '3')
		data->player->pos.x = new_pos.x;
	map_value = get_map_value(data, (int)data->player->pos.x, (int)new_pos.y);
	if (map_value == '0' || map_value == '3')
		data->player->pos.y = new_pos.y;
}

void	handle_strafe(t_data *data)
{
	t_vec2f	new_pos;
	float	speed;
	int		map_value;

	if (data->player->key_state.a == data->player->key_state.d)
		return ;
	speed = MOVE_SPEED * data->mlx_data->mlx->delta_time;
	if (data->player->key_state.shift)
		speed *= 2.0f;
	if (data->player->key_state.d)
		speed *= 1.0f;
	if (data->player->key_state.a)
		speed *= -1.0f;
	new_pos.x = data->player->pos.x + data->player->plane.x * speed;
	new_pos.y = data->player->pos.y + data->player->plane.y * speed;
	map_value = get_map_value(data, (int)new_pos.x, (int)data->player->pos.y);
	if (map_value == '0' || map_value == '3')
		data->player->pos.x = new_pos.x;
	map_value = get_map_value(data, (int)data->player->pos.x, (int)new_pos.y);
	if (map_value == '0' || map_value == '3')
		data->player->pos.y = new_pos.y;
}
