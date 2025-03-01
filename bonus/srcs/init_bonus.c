#include "cub3d.h"

void init_player_jump(t_player *player)
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

void	example_map(t_data *data)
{
	data->map_data->map = ft_strdup(
	"1111111111111111111111111"
    "1000000000000000000000001"
    "1001111100111111001111101"
    "1001000100100020002000101"
    "1001000100100010001000101"
    "1001111100112111001111101"
    "1000000000000000000000001"
    "1011111011112101111110101"
    "1000001000000001000000101"
    "1000001001112101000000101"
    "1011101001000101011100101"
    "1010001001000101000000101"
    "1011111001111101111100101"
    "1000000000000000000000001"
    "1111011111110111111101111"
    "1000010000000100000000001"
    "1000010111110101111101111"
    "1000000000000000000000001"
    "1111111111111111111111111");
	data->map_data->width = 25;
	data->map_data->height = 19;
	data->player->pos = (t_vec2f){1.5f, 1.5f};
	data->player->dir = (t_vec2f){1.0f, 0.0f};
	data->player->plane = (t_vec2f){0.0f, 0.66f};
	data->texture->north = mlx_load_png("textures/north.png");
	data->texture->south = mlx_load_png("textures/south.png");
	data->texture->east = mlx_load_png("textures/east.png");
	data->texture->west = mlx_load_png("textures/west.png");
	data->texture->floor = mlx_load_png("textures/floor.png");
	data->texture->ceiling = mlx_load_png("textures/ceiling.png");
	ft_memset(&data->player->key_state, 0, sizeof(t_key_state));
	init_player_jump(data->player);
}

int	init_data(t_data *data)
{
	data->mlx_data = ft_calloc(1, sizeof(t_mlx));
	data->map_data = ft_calloc(1, sizeof(t_map));
	data->player = ft_calloc(1, sizeof(t_player));
	data->texture = ft_calloc(1, sizeof(t_texture_data));
	if (!data->mlx_data || !data->map_data || !data->player || !data->texture)
		return (-1);
	data->mlx_data->mlx = mlx_init(WIN_WIDTH, WIN_HEIGHT, "cub3d", true);
	data->mlx_data->img = mlx_new_image(data->mlx_data->mlx, WIN_WIDTH,
			WIN_HEIGHT);
	data->mlx_data->img_buffer = NULL;
	if (!data->mlx_data->mlx || !data->mlx_data->img)
		return (-1);
	example_map(data);
	return (0);
}
