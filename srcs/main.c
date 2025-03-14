/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 15:12:44 by lpolizzi          #+#    #+#             */
/*   Updated: 2025/03/13 16:53:46 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	free_data(t_data *data)
{
	free(data->mlx_data);
	free(data->player);
	if (data->texture->north)
		mlx_delete_texture(data->texture->north);
	if (data->texture->south)
		mlx_delete_texture(data->texture->south);
	if (data->texture->east)
		mlx_delete_texture(data->texture->east);
	if (data->texture->west)
		mlx_delete_texture(data->texture->west);
	free(data->texture);
	close(data->map_data->map_fd);
	free_split(data->map_data->map);
	free(data->map_data->map_1d);
	free(data->map_data);
	free(data);
}

void	exit_program(t_data *data, int status)
{
	if (data->mlx_data->mlx)
		mlx_terminate(data->mlx_data->mlx);
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
	render_raycast(data);
	ft_swap_pointers((void **)&data->mlx_data->img,
		(void **)&data->mlx_data->img_buffer);
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
	mlx_set_setting(MLX_FULLSCREEN, true);
	if (!data || !parsing(data, av[1]) || !init_mlx(data))
	{
		free_data(data);
		return (1);
	}
	mlx_image_to_window(data->mlx_data->mlx, data->mlx_data->img, 0, 0);
	mlx_key_hook(data->mlx_data->mlx, key_callback, data);
	mlx_loop_hook(data->mlx_data->mlx, render_all, data);
	mlx_loop(data->mlx_data->mlx);
	free_data(data);
}
