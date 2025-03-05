/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpolizzi <lpolizzi@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 15:12:44 by lpolizzi          #+#    #+#             */
/*   Updated: 2025/02/26 18:01:06 by lpolizzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	free_data(t_data *data)
{
	free(data->mlx_data);
	free(data->player);
	free(data->texture);
	free(data->map_data->map);
	free(data->map_data);
	free(data);
}

void delete_door_textures(t_data *data)
{
	int	i;

	i = -1;
	while (++i < NUM_FRAMES)
		mlx_delete_texture(data->texture->doors[i]);
}

void	exit_program(t_data *data, int status)
{
	if (data->mlx_data->mlx)
		mlx_terminate(data->mlx_data->mlx);
	mlx_delete_texture(data->texture->north);
	mlx_delete_texture(data->texture->south);
	mlx_delete_texture(data->texture->east);
	mlx_delete_texture(data->texture->west);
	mlx_delete_texture(data->texture->floor);
	mlx_delete_texture(data->texture->ceiling);
	delete_door_textures(data);
	free_data(data);
	exit(status);
}

void	ft_swap_pointers(void **ptr1, void **ptr2)
{
	void	*tmp;

	tmp = *ptr1;
	*ptr1 = *ptr2;
	*ptr2 = tmp;
}

void	render_all(void *vdata)
{
	t_data	*data;

	data = (t_data *)vdata;
	handle_movement(data);
	handle_strafe(data);
	handle_rotation(data);
	handle_camera_tilt(data);
	handle_jump(data, data->mlx_data->mlx->delta_time);
	if (!data->mlx_data->img_buffer)
		data->mlx_data->img_buffer = mlx_new_image(data->mlx_data->mlx,
				data->mlx_data->mlx->width, data->mlx_data->mlx->height);
	render_raycast(data);
	render_minimap(data);
	ft_swap_pointers((void **)&data->mlx_data->img->pixels,
		(void **)&data->mlx_data->img_buffer->pixels);
}

int	main(int ac, char **av)
{
	t_data	*data;

	(void)av;
	if (ac < 2)
	{
		ft_putstr_fd("Error: No map file provided\n", 2);
		return (1);
	}
	data = ft_calloc(1, sizeof(t_data));
	if (!data)
		return (1);
	mlx_set_setting(MLX_FULLSCREEN, true);
	if (init_data(data) == -1 || mlx_errno)
		exit_program(data, 1);
	mlx_set_cursor_mode(data->mlx_data->mlx, MLX_MOUSE_DISABLED);
	mlx_set_mouse_pos(data->mlx_data->mlx, data->mlx_data->mlx->width / 2,
				   data->mlx_data->mlx->height / 2);
	mlx_image_to_window(data->mlx_data->mlx, data->mlx_data->img, 0, 0);
	mlx_key_hook(data->mlx_data->mlx, key_callback, data);
	mlx_cursor_hook(data->mlx_data->mlx, handle_cursor, data);
	mlx_loop_hook(data->mlx_data->mlx, render_all, data);
	mlx_loop(data->mlx_data->mlx);
	free_data(data);
}
