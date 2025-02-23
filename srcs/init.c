#include "cub3d.h"

void	example_map(t_data *data)
{
	data->map_data->map = ft_strdup("1111111111000000011000000011001000011000000011001000011000000011000000011111111111");
	data->map_data->width = 9;
	data->map_data->height = 9;
	data->player->pos = (t_vec2f){4.5f, 4.5f};
	data->player->dir = (t_vec2f){1.0f, 0.0f};
	data->player->plane = (t_vec2f){0.0f, 0.66f};
	data->mlx_data->old_height = data->mlx_data->mlx->height;
	data->mlx_data->old_width = data->mlx_data->mlx->width;
	data->texture->ceiling_color = LIGHT_BLUE;
	data->texture->floor_color = GREY;
}

int	init_data(t_data *data)
{
	data->mlx_data = ft_calloc(1, sizeof(t_mlx));
	data->map_data = ft_calloc(1, sizeof(t_map));
	data->player = ft_calloc(1, sizeof(t_player));
	data->texture = ft_calloc(1, sizeof(t_texture_data));
	data->mlx_data->mlx = mlx_init(WIN_WIDTH, WIN_HEIGHT, "cub3d", true);
	data->mlx_data->img = mlx_new_image(data->mlx_data->mlx, WIN_WIDTH,
			WIN_HEIGHT);
	data->mlx_data->img_buffer = NULL;
	if (!data->mlx_data->mlx || !data->mlx_data->img)
		return (-1);
	example_map(data);
	return (0);
}
