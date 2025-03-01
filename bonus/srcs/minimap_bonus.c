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
	*square_size = (data->mlx_data->mlx->width / 5) / data->map_data->width;
	if (*square_size < 2)
		*square_size = 2;
	offset->x = data->mlx_data->mlx->width - data->map_data->width
		* *square_size - 20;
	offset->y = 20;
}

void	render_minimap(t_data *data)
{
	int		square_size;
	t_vec2i	offset;
	t_vec2i	grid;
	t_vec2i	pos;

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
			if (get_map_value(data, grid.x, grid.y) == '1')
				draw_square(data, pos, square_size, GREY);
			else if (get_map_value(data, grid.x, grid.y) == '0')
				draw_square(data, pos, square_size, BLUE);
		}
	}
	draw_player(data, offset, square_size);
}
