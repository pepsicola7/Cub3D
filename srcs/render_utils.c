#include "cub3d.h"

void ft_put_pixel(t_data *data, int x, int y, int color)
{
	if (x < 0 || x >= (int)data->mlx_data->img->width || y < 0
		|| y >= (int)data->mlx_data->img->height)
		return ;
	*(int *)(data->mlx_data->img->pixels + (y * data->mlx_data->img->width
		+ x)) = color;
}

void draw_quads(t_data *data, t_vec2i start, t_vec2i size, int color)
{
	t_vec2i	pixel;

	pixel.y = -size.y / 2;
	while (++pixel.y < size.y / 2)
	{
		pixel.x = -size.x / 2;
		while (++pixel.x < size.x / 2)
			ft_put_pixel(data, start.x + pixel.x, start.y + pixel.y, color);
	}
}

void	draw_circle(t_data *data, t_vec2i pos, int radius, int color)
{
	t_vec2i	pixel;
	float	dist;

	pixel.y = -radius;
	while (++pixel.y < radius)
	{
		pixel.x = -radius;
		while (++pixel.x < radius)
		{
			dist = sqrt(pixel.x * pixel.x + pixel.y * pixel.y);
			if (dist < radius)
				ft_put_pixel(data, pos.x + pixel.x, pos.y
				  + pixel.y, color);
		}
	}
}

static void	init_bresenham(t_bresenham *bres, t_vec2i start, t_vec2i end)
{
	bres->delta_x = abs(end.x - start.x);
	bres->delta_y = abs(end.y - start.y);
	if (start.x < end.x)
		bres->step_x = 1;
	else
		bres->step_x = -1;
	if (start.y < end.y)
		bres->step_y = 1;
	else
		bres->step_y = -1;
	bres->error = bres->delta_x - bres->delta_y;
}

void	draw_line(t_data *data, t_vec2i start, t_vec2i end, int color)
{
	t_bresenham	bres;

	init_bresenham(&bres, start, end);
	while (1)
	{
		ft_put_pixel(data, start.x, start.y, color);
		if (start.x == end.x && start.y == end.y)
			break ;
		bres.error_adjustment = 2 * bres.error;
		if (bres.error_adjustment > -bres.delta_y)
		{
			bres.error -= bres.delta_y;
			start.x += bres.step_x;
		}
		if (bres.error_adjustment < bres.delta_x)
		{
			bres.error += bres.delta_x;
			start.y += bres.step_y;
		}
	}
}
