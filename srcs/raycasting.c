#include "cub3d.h"

inline int	get_map_value(t_data *data, int x, int y)
{
	if (x < 0 || x >= data->map_data->width || y < 0 || y >= data->map_data->height)
		return (1);
	return (data->map_data->map[y * data->map_data->width + x]);
}

void	perform_dda(t_data *data, t_ray *ray)
{
	ray->hit = 0;
	while (ray->hit == 0)
	{
		// Jump to next square
		if (ray->side_dist.x < ray->side_dist.y)
		{
			ray->side_dist.x += ray->delta_dist.x;
			ray->map_x += ray->step_x;
			ray->side = 0;
		}
		else
		{
			ray->side_dist.y += ray->delta_dist.y;
			ray->map_y += ray->step_y;
			ray->side = 1;
		}
		// Check wall hit
		if (get_map_value(data, ray->map_x, ray->map_y))
			ray->hit = 1;
	}
	// Calculate perpendicular wall distance
	if (ray->side == 0)
		ray->perp_wall_dist = (ray->map_x - data->player->pos.x + (1
					- ray->step_x) / 2) / ray->dir.x;
	else
		ray->perp_wall_dist = (ray->map_y - data->player->pos.y + (1
					- ray->step_y) / 2) / ray->dir.y;
}

void	draw_vertical_line(t_data *data, t_ray *ray, int x)
{
	int			line_height;
	int			draw_start;
	int			draw_end;
	uint32_t	color;

	// Calculate wall height
	line_height = (int)(data->mlx_data->old_height / ray->perp_wall_dist);
	// Calculate lowest and highest pixel to draw
	draw_start = -line_height / 2 + data->mlx_data->old_height / 2;
	if (draw_start < 0)
		draw_start = 0;
	draw_end = line_height / 2 + data->mlx_data->old_height / 2;
	if (draw_end >= data->mlx_data->old_height)
		draw_end = data->mlx_data->old_height - 1;
	// Draw ceiling
	for (int y = 0; y < draw_start; y++)
		mlx_put_pixel(data->mlx_data->img, x, y, data->texture->ceiling_color);
	// Choose texture and color based on wall side
	if (ray->side == 0)
		color = (ray->step_x > 0) ? BLUE : RED; // East/West
	else
		color = (ray->step_y > 0) ? YELLOW : GREEN; // North/South
	// Draw wall
	for (int y = draw_start; y < draw_end; y++)
		mlx_put_pixel(data->mlx_data->img, x, y, color);
	// Draw floor
	for (int y = draw_end; y < data->mlx_data->old_height; y++)
		mlx_put_pixel(data->mlx_data->img, x, y, data->texture->floor_color);
}

void	cast_ray(t_data *data, int x)
{
	t_ray	ray;
	float	camera_x;

	// Calculate ray position and direction
	camera_x = 2 * x / (float)data->mlx_data->old_width - 1;
	ray.dir.x = data->player->dir.x + data->player->plane.x * camera_x;
	ray.dir.y = data->player->dir.y + data->player->plane.y * camera_x;
	// Set starting position
	ray.map_x = (int)data->player->pos.x;
	ray.map_y = (int)data->player->pos.y;
	// Calculate delta distances
	ray.delta_dist.x = fabsf(1 / ray.dir.x);
	ray.delta_dist.y = fabsf(1 / ray.dir.y);
	// Calculate step and initial side distances
	if (ray.dir.x < 0)
	{
		ray.step_x = -1;
		ray.side_dist.x = (data->player->pos.x - ray.map_x) * ray.delta_dist.x;
	}
	else
	{
		ray.step_x = 1;
		ray.side_dist.x = (ray.map_x + 1.0f - data->player->pos.x)
			* ray.delta_dist.x;
	}
	if (ray.dir.y < 0)
	{
		ray.step_y = -1;
		ray.side_dist.y = (data->player->pos.y - ray.map_y) * ray.delta_dist.y;
	}
	else
	{
		ray.step_y = 1;
		ray.side_dist.y = (ray.map_y + 1.0f - data->player->pos.y)
			* ray.delta_dist.y;
	}
	// Perform DDA and draw
	perform_dda(data, &ray);
	draw_vertical_line(data, &ray, x);
}

void	render(void *param)
{
	t_data	*data;

	data = (t_data *)param;
	// Cast rays
	printf("Player pos: %f, %f\n", data->player->pos.x, data->player->pos.y);
	for (int x = 0; x < data->mlx_data->old_width; x++)
	{
		// printf("Casting ray for x = %d\n", x);
		cast_ray(data, x);
	}
}

void	move_player(mlx_key_data_t keydata, void *vdata)
{
	t_data		*data;
	t_vec2f		new_pos;
	float		dir;
	float		angle;
	float		old_dir_x;
	float		old_plane_x;

	data = (t_data *)vdata;
	if (keydata.key == MLX_KEY_ESCAPE)
		exit_program(data, 0);
	// Forward / Backward
	if (keydata.key == MLX_KEY_W || keydata.key == MLX_KEY_S)
	{
		dir = (keydata.key == MLX_KEY_W) ? 1.0f : -1.0f;
		new_pos.x = data->player->pos.x + data->player->dir.x * MOVE_SPEED
			* dir;
		new_pos.y = data->player->pos.y + data->player->dir.y * MOVE_SPEED
			* dir;
		// Collision check
		if (!get_map_value(data, (int)new_pos.x,
				(int)data->player->pos.y))
			data->player->pos.x = new_pos.x;
		if (!get_map_value(data, (int)data->player->pos.x,
				(int)new_pos.y))
			data->player->pos.y = new_pos.y;
	}
	// Rotate Left / Right
	if (keydata.key == MLX_KEY_LEFT || keydata.key == MLX_KEY_RIGHT)
	{
		angle = (keydata.key == MLX_KEY_LEFT) ? ROTATE_SPEED : -ROTATE_SPEED;
		old_dir_x = data->player->dir.x;
		old_plane_x = data->player->plane.x;
		// Rotate direction vector
		data->player->dir.x = data->player->dir.x * cosf(angle)
			- data->player->dir.y * sinf(angle);
		data->player->dir.y = old_dir_x * sinf(angle) + data->player->dir.y
			* cosf(angle);
		// Rotate camera plane
		data->player->plane.x = data->player->plane.x * cosf(angle)
			- data->player->plane.y * sinf(angle);
		data->player->plane.y = old_plane_x * sinf(angle)
			+ data->player->plane.y * cosf(angle);
	}
}
