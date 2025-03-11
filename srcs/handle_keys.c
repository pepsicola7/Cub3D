/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_keys.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpolizzi <lpolizzi@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 21:08:28 by lpolizzi          #+#    #+#             */
/*   Updated: 2025/03/11 21:08:29 by lpolizzi         ###   ########.fr       */
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
}

void	key_callback(mlx_key_data_t keydata, void *vdata)
{
	t_data	*data;

	data = (t_data *)vdata;
	if (keydata.key == MLX_KEY_ESCAPE)
		exit_program(data, 0);
	handle_key_press(data, keydata, keydata.action == MLX_PRESS
		|| keydata.action == MLX_REPEAT);
}
