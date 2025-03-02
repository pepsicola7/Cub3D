#include "cub3d.h"

void	init_sprite(t_data *data)
{
	int	i;

	i = -1;
	data->texture->sprite = ft_calloc(1, sizeof(t_sprite));
	data->texture->sprite->frames_texture[0] = mlx_load_png("textures/TorchGIF/frame_00_delay-0.04s.png");
	data->texture->sprite->frames_texture[1] = mlx_load_png("textures/TorchGIF/frame_10_delay-0.04s.png");
	data->texture->sprite->frames_texture[2] = mlx_load_png("textures/TorchGIF/frame_20_delay-0.04s.png");
	data->texture->sprite->frames_texture[3] = mlx_load_png("textures/TorchGIF/frame_30_delay-0.04s.png");
	data->texture->sprite->frames_texture[4] = mlx_load_png("textures/TorchGIF/frame_40_delay-0.04s.png");
	data->texture->sprite->frames_texture[5] = mlx_load_png("textures/TorchGIF/frame_50_delay-0.04s.png");
	data->texture->sprite->frames_texture[6] = mlx_load_png("textures/TorchGIF/frame_60_delay-0.04s.png");
	data->texture->sprite->frames_texture[7] = mlx_load_png("textures/TorchGIF/frame_70_delay-0.04s.png");
	while (++i < 8)
	{
		data->texture->sprite->frames_image[i] = mlx_texture_to_image(data->mlx_data->mlx,
				data->texture->sprite->frames_texture[i]);
		mlx_image_to_window(data->mlx_data->mlx,
			data->texture->sprite->frames_image[i], 100, 100);
		data->texture->sprite->frames_image[i]->instances[0].enabled = false;
	}
	data->texture->sprite->current_frame_index = 0;
	data->texture->sprite->frames_image[0]->instances[0].enabled = true;
}

void	draw_sprite(t_data *data)
{
	int	frame_index;

	frame_index = (int)mlx_get_time() % 8;
	if (data->texture->sprite->current_frame_index != frame_index)
	{
		data->texture->sprite->frames_image[data->texture->sprite->current_frame_index]->instances[0].enabled = false;
		data->texture->sprite->frames_image[frame_index]->instances[0].enabled = true;
		data->texture->sprite->current_frame_index = frame_index;
	}
}
