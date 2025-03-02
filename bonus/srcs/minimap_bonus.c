#include "cub3d.h"

void	draw_player(t_data *data, t_vec2i offset, int square_size)
{
	t_vec2f	pos;
	t_vec2i	center;
	t_vec2i	line_end;
	int		line_length;

	line_length = square_size / 2;
	if (line_length < 5)
		line_length = 5;
	pos.x = offset.x + data->player->pos.x * square_size;
	pos.y = offset.y + data->player->pos.y * square_size;
	center.x = (int)pos.x;
	center.y = (int)pos.y;
	line_end.x = center.x + (int)(data->player->dir.x * line_length * 1.5);
	line_end.y = center.y + (int)(data->player->dir.y * line_length * 1.5);
	draw_line(data, center, line_end, RED);
	draw_circle(data, center, square_size / 4, GREEN);
}

void	get_minimap_size(t_data *data, int *square_size, t_vec2i *offset)
{
	int	divider;

	if (data->player->key_state.tab)
		divider = 3;
	else
		divider = 5;
	*square_size = (data->mlx_data->mlx->width / divider) / data->map_data->width;
	if (*square_size < 2)
		*square_size = 2;
	offset->x = data->mlx_data->mlx->width - data->map_data->width
		* *square_size - 20;
	offset->y = 20;
}

void	draw_minimap_tile(t_data *data, t_vec2i pos, int square_size, int map_value)
{
	int	color;

	if (map_value == '0')
		color = BLUE;
	else if (map_value == '1')
		color = GREY;
	else if (map_value == '2')
		color = YELLOW;
	else if (map_value == '3')
		color = BROWN;
	else
		return ;
	draw_square(data, pos, square_size, color);
}

void	render_minimap(t_data *data)
{
	int		square_size;
	t_vec2i	offset;
	t_vec2i	grid;
	t_vec2i	pos;
	int map_value;

	get_minimap_size(data, &square_size, &offset);
	draw_background(data, offset, square_size);
	grid.y = -1;
	while (++grid.y < data->map_data->height)
	{
		grid.x = -1;
		while (++grid.x < data->map_data->width)
		{
			pos.x = offset.x + grid.x * square_size;
			pos.y = offset.y + grid.y * square_size;
			map_value = get_map_value(data, grid.x, grid.y);
			draw_minimap_tile(data, pos, square_size, map_value);
		}
	}
	draw_player(data, offset, square_size);
}
