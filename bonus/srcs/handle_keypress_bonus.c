/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_keypress_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpolizzi <lpolizzi@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 12:24:03 by lpolizzi          #+#    #+#             */
/*   Updated: 2025/03/12 12:24:39 by lpolizzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	handle_key_press(t_data *data, mlx_key_data_t keydata, bool is_pressed)
{
	if (keydata.key == MLX_KEY_W)
		data->player->key_state.w = is_pressed;
	else if (keydata.key == MLX_KEY_S)
		data->player->key_state.s = is_pressed;
	else if (keydata.key == MLX_KEY_A)
		data->player->key_state.a = is_pressed;
	else if (keydata.key == MLX_KEY_D)
		data->player->key_state.d = is_pressed;
	else if (keydata.key == MLX_KEY_LEFT)
		data->player->key_state.left = is_pressed;
	else if (keydata.key == MLX_KEY_RIGHT)
		data->player->key_state.right = is_pressed;
	else if (keydata.key == MLX_KEY_UP)
		data->player->key_state.up = is_pressed;
	else if (keydata.key == MLX_KEY_DOWN)
		data->player->key_state.down = is_pressed;
	else if (keydata.key == MLX_KEY_LEFT_SHIFT)
		data->player->key_state.shift = is_pressed;
	else if (keydata.key == MLX_KEY_SPACE)
		data->player->key_state.space = is_pressed;
	else if (keydata.key == MLX_KEY_LEFT_CONTROL)
		data->player->key_state.control = is_pressed;
	else if (keydata.key == MLX_KEY_TAB)
		data->player->key_state.tab = is_pressed;
}

void	door_actions(t_data *data)
{
	char	map_value;
	t_vec2f	ray;
	float	distance;

	if (get_map_value(data, (int)data->player->pos.x,
			(int)data->player->pos.y) == '3')
		return ;
	ray.x = data->player->pos.x + data->player->dir.x * 0.5;
	ray.y = data->player->pos.y + data->player->dir.y * 0.5;
	distance = sqrt(pow(data->player->pos.x - ray.x, 2)
			+ pow(data->player->pos.y - ray.y, 2));
	if (distance < 1.0)
	{
		map_value = get_map_value(data, (int)ray.x, (int)ray.y);
		if (map_value == '2')
			data->map_data->map_1d[(int)ray.y * data->map_data->width
				+ (int)ray.x] = '3';
		else if (map_value == '3')
			data->map_data->map_1d[(int)ray.y * data->map_data->width
				+ (int)ray.x] = '2';
	}
}

void	key_callback(mlx_key_data_t keydata, void *vdata)
{
	t_data	*data;

	data = (t_data *)vdata;
	if (keydata.key == MLX_KEY_ESCAPE)
		exit_program(data, 0);
	else if (keydata.key == MLX_KEY_E && (keydata.action == MLX_PRESS
			|| keydata.action == MLX_REPEAT))
		door_actions(data);
	else
		handle_key_press(data, keydata, keydata.action == MLX_PRESS
			|| keydata.action == MLX_REPEAT);
}
