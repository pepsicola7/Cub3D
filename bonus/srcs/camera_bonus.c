/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpolizzi <lpolizzi@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 12:24:25 by lpolizzi          #+#    #+#             */
/*   Updated: 2025/03/12 12:24:38 by lpolizzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	change_player_yaw(t_data *data, float yaw)
{
	float	old_dir_x;
	float	old_plane_x;
	float	cos_yaw;
	float	sin_yaw;

	cos_yaw = cosf(yaw);
	sin_yaw = sinf(yaw);
	old_dir_x = data->player->dir.x;
	old_plane_x = data->player->plane.x;
	data->player->dir.x = data->player->dir.x * cos_yaw - data->player->dir.y
		* sin_yaw;
	data->player->dir.y = old_dir_x * sin_yaw + data->player->dir.y * cos_yaw;
	data->player->plane.x = data->player->plane.x * cos_yaw
		- data->player->plane.y * sin_yaw;
	data->player->plane.y = old_plane_x * sin_yaw + data->player->plane.y
		* cos_yaw;
}

void	change_player_pitch(t_data *data, float pitch)
{
	data->player->camera_y_offset -= pitch;
	if (data->player->camera_y_offset > 600)
		data->player->camera_y_offset = 600;
	if (data->player->camera_y_offset < -600)
		data->player->camera_y_offset = -600;
}

void	handle_cursor(double xpos, double ypos, void *param)
{
	static int	call_num = -2;
	t_data		*data;
	float		yaw;
	float		pitch;

	data = (t_data *)param;
	if (call_num++ < 0)
	{
		mlx_set_mouse_pos(data->mlx_data->mlx, data->mlx_data->mlx->width / 2,
			data->mlx_data->mlx->height / 2);
		return ;
	}
	yaw = (xpos - (data->mlx_data->mlx->width / 2)) * MOUSE_SENSITIVITY;
	pitch = (ypos - (data->mlx_data->mlx->height / 2));
	change_player_yaw(data, yaw * data->mlx_data->mlx->delta_time);
	change_player_pitch(data, pitch);
	mlx_set_mouse_pos(data->mlx_data->mlx, data->mlx_data->mlx->width / 2,
		data->mlx_data->mlx->height / 2);
}

void	handle_rotation(t_data *data)
{
	float	yaw;

	yaw = 0.0f;
	if (data->player->key_state.left == data->player->key_state.right)
		return ;
	if (data->player->key_state.right)
		yaw = ROTATE_SPEED * data->mlx_data->mlx->delta_time;
	else if (data->player->key_state.left)
		yaw = -ROTATE_SPEED * data->mlx_data->mlx->delta_time;
	change_player_yaw(data, yaw);
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
