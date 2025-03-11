/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movements.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpolizzi <lpolizzi@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 20:35:56 by lpolizzi          #+#    #+#             */
/*   Updated: 2025/03/11 20:38:18 by lpolizzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	handle_movement(t_data *data)
{
	t_vec2f	new_pos;
	float	speed;

	if (data->player->key_state.w == data->player->key_state.s)
		return ;
	speed = MOVE_SPEED * data->mlx_data->mlx->delta_time;
	if (data->player->key_state.w)
		speed *= 1.0f;
	if (data->player->key_state.s)
		speed *= -1.0f;
	else if (data->player->key_state.shift)
		speed *= 2.0f;
	new_pos.x = data->player->pos.x + data->player->dir.x * speed;
	new_pos.y = data->player->pos.y + data->player->dir.y * speed;
	if (get_map_value(data, (int)new_pos.x, (int)data->player->pos.y) == '0')
		data->player->pos.x = new_pos.x;
	if (get_map_value(data, (int)data->player->pos.x, (int)new_pos.y) == '0')
		data->player->pos.y = new_pos.y;
}

void	handle_strafe(t_data *data)
{
	t_vec2f	new_pos;
	float	speed;

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
	if (get_map_value(data, (int)new_pos.x, (int)data->player->pos.y) == '0')
		data->player->pos.x = new_pos.x;
	if (get_map_value(data, (int)data->player->pos.x, (int)new_pos.y) == '0')
		data->player->pos.y = new_pos.y;
}

void	handle_rotation(t_data *data)
{
	float	angle;
	float	old_dir_x;
	float	old_plane_x;
	float	cos_angle;
	float	sin_angle;

	if (data->player->key_state.left == data->player->key_state.right)
		return ;
	if (data->player->key_state.right)
		angle = ROTATE_SPEED * data->mlx_data->mlx->delta_time;
	if (data->player->key_state.left)
		angle = -ROTATE_SPEED * data->mlx_data->mlx->delta_time;
	cos_angle = cosf(angle);
	sin_angle = sinf(angle);
	old_dir_x = data->player->dir.x;
	old_plane_x = data->player->plane.x;
	data->player->dir.x = data->player->dir.x * cos_angle - data->player->dir.y
		* sin_angle;
	data->player->dir.y = old_dir_x * sin_angle + data->player->dir.y
		* cos_angle;
	data->player->plane.x = data->player->plane.x * cos_angle
		- data->player->plane.y * sin_angle;
	data->player->plane.y = old_plane_x * sin_angle + data->player->plane.y
		* cos_angle;
}

void	handle_camera_tilt(t_data *data)
{
	int	offset;

	if (data->player->key_state.up == data->player->key_state.down)
		return ;
	offset = data->player->camera_y_offset;
	if (data->player->key_state.up)
		offset += (int)TILT_SPEED * data->mlx_data->mlx->delta_time;
	if (data->player->key_state.down)
		offset -= (int)TILT_SPEED * data->mlx_data->mlx->delta_time;
	if (offset > 600)
		offset = 600;
	if (offset < -600)
		offset = -600;
	data->player->camera_y_offset = offset;
}
