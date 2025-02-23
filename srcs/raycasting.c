#include "cub3d.h"

inline int	get_map_value(t_data *data, int x, int y)
{
	if (x < 0 || x >= data->map_data->width || y < 0
		|| y >= data->map_data->height)
		return (1);
	return (data->map_data->map[y * data->map_data->width + x]);
}

void	perform_dda(t_data *data, t_ray *ray)
{
	ray->hit = 0;
	while (ray->hit == 0)
	{
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
		if (get_map_value(data, ray->map_x, ray->map_y) == '1')
			ray->hit = 1;
	}
	if (ray->side == 0)
		ray->perp_wall_dist = (ray->side_dist.x - ray->delta_dist.x);
	else
		ray->perp_wall_dist = (ray->side_dist.y - ray->delta_dist.y);
}

void	init_vertical_line(t_data *data, t_ray *ray, int *line_height)
{
	*line_height = (int)(data->mlx_data->mlx->height / ray->perp_wall_dist);
}

void	calculate_draw_limits(t_data *data, int line_height, int *draw_start,
		int *draw_end)
{
	*draw_start = -line_height / 2 + data->mlx_data->mlx->height / 2.5;
	*draw_end = line_height / 2 + data->mlx_data->mlx->height / 2.5;
	if (*draw_start < 0)
		*draw_start = 0;
	if (*draw_end >= data->mlx_data->mlx->height)
		*draw_end = data->mlx_data->mlx->height - 1;
}

inline uint32_t	get_wall_color(t_ray *ray)
{
	if (ray->side == 0)
	{
		if (ray->step_x > 0)
			return (BLUE);
		else
			return (RED);
	}
	else
	{
		if (ray->step_y > 0)
			return (YELLOW);
		else
			return (GREEN);
	}
}

void	draw_vertical_line(t_data *data, t_ray *ray, int x)
{
	int			line_height;
	int			draw_start;
	int			draw_end;
	uint32_t	color;
	int			y;

	init_vertical_line(data, ray, &line_height);
	calculate_draw_limits(data, line_height, &draw_start, &draw_end);
	y = 0;
	while (y < draw_start)
		ft_put_pixel(data->mlx_data->img_buffer, x, y++, data->texture->ceiling_color);
	color = get_wall_color(ray);
	while (y < draw_end)
		ft_put_pixel(data->mlx_data->img_buffer, x, y++, color);
	while (y < data->mlx_data->mlx->height)
		ft_put_pixel(data->mlx_data->img_buffer, x, y++, data->texture->floor_color);
}

void	init_ray(t_data *data, t_ray *ray, int x)
{
	float	camera_x;

	camera_x = 2 * x / (float)data->mlx_data->old_width - 1;
	ray->dir.x = data->player->dir.x + data->player->plane.x * camera_x;
	ray->dir.y = data->player->dir.y + data->player->plane.y * camera_x;
	ray->map_x = (int)data->player->pos.x;
	ray->map_y = (int)data->player->pos.y;
	ray->delta_dist.x = fabsf(1 / ray->dir.x);
	ray->delta_dist.y = fabsf(1 / ray->dir.y);
}

void	init_ray_steps(t_data *data, t_ray *ray)
{
	if (ray->dir.x < 0)
	{
		ray->step_x = -1;
		ray->side_dist.x = (data->player->pos.x - ray->map_x)
			* ray->delta_dist.x;
	}
	else
	{
		ray->step_x = 1;
		ray->side_dist.x = (ray->map_x + 1.0f - data->player->pos.x)
			* ray->delta_dist.x;
	}
	if (ray->dir.y < 0)
	{
		ray->step_y = -1;
		ray->side_dist.y = (data->player->pos.y - ray->map_y)
			* ray->delta_dist.y;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dist.y = (ray->map_y + 1.0f - data->player->pos.y)
			* ray->delta_dist.y;
	}
}

void	cast_ray(t_data *data, int x)
{
	t_ray	ray;

	init_ray(data, &ray, x);
	init_ray_steps(data, &ray);
	perform_dda(data, &ray);
	draw_vertical_line(data, &ray, x);
}

void	render_raycast(void *param)
{
	t_data	*data;
	int		x;

	x = -1;
	data = (t_data *)param;
	while (++x < data->mlx_data->old_width)
		cast_ray(data, x);
}

void	handle_movement(t_data *data, mlx_key_data_t keydata)
{
	t_vec2f	new_pos;
	float	dir;

	if (keydata.key == MLX_KEY_W)
		dir = 1.0f;
	else if (keydata.key == MLX_KEY_S)
		dir = -1.0f;
	else
		return ;
	new_pos.x = data->player->pos.x + data->player->dir.x * MOVE_SPEED * dir;
	new_pos.y = data->player->pos.y + data->player->dir.y * MOVE_SPEED * dir;
	if (get_map_value(data, (int)new_pos.x, (int)data->player->pos.y) == '0')
		data->player->pos.x = new_pos.x;
	if (get_map_value(data, (int)data->player->pos.x, (int)new_pos.y) == '0')
		data->player->pos.y = new_pos.y;
}

void	handle_strafe(t_data *data, mlx_key_data_t keydata)
{
	t_vec2f	new_pos;
	float	dir;

	if (keydata.key == MLX_KEY_A)
		dir = -1.0f;
	else if (keydata.key == MLX_KEY_D)
		dir = 1.0f;
	else
		return ;
	new_pos.x = data->player->pos.x + data->player->plane.x * MOVE_SPEED * dir;
	new_pos.y = data->player->pos.y + data->player->plane.y * MOVE_SPEED * dir;
	if (get_map_value(data, (int)new_pos.x, (int)data->player->pos.y) == '0')
		data->player->pos.x = new_pos.x;
	if (get_map_value(data, (int)data->player->pos.x, (int)new_pos.y) == '0')
		data->player->pos.y = new_pos.y;
}

void	handle_rotation(t_data *data, mlx_key_data_t keydata)
{
	float	angle;
	float	old_dir_x;
	float	old_plane_x;

	if (keydata.key == MLX_KEY_LEFT)
		angle = -ROTATE_SPEED;
	else if (keydata.key == MLX_KEY_RIGHT)
		angle = ROTATE_SPEED;
	else
		return ;
	old_dir_x = data->player->dir.x;
	old_plane_x = data->player->plane.x;
	data->player->dir.x = data->player->dir.x * cosf(angle)
		- data->player->dir.y * sinf(angle);
	data->player->dir.y = old_dir_x * sinf(angle) + data->player->dir.y
		* cosf(angle);
	data->player->plane.x = data->player->plane.x * cosf(angle)
		- data->player->plane.y * sinf(angle);
	data->player->plane.y = old_plane_x * sinf(angle) + data->player->plane.y
		* cosf(angle);
}

void	move_player(mlx_key_data_t keydata, void *vdata)
{
	t_data	*data;

	data = (t_data *)vdata;
	if (keydata.key == MLX_KEY_ESCAPE)
		exit_program(data, 0);
	handle_movement(data, keydata);
	handle_rotation(data, keydata);
	handle_strafe(data, keydata);
}
