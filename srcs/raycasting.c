#include "cub3d.h"

inline int	get_map_value(t_data *data, int x, int y)
{
	if (x < 0 || x >= data->map_data->width || y < 0
		|| y >= data->map_data->height)
		return (1);
	return (data->map_data->map[y * data->map_data->width + x]);
}

inline void	perform_dda(t_data *data, t_ray *ray)
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
	*draw_start = (data->mlx_data->mlx->height / 2) - (line_height / 2)
		+ data->player->camera_y_offset;
	*draw_end = (data->mlx_data->mlx->height / 2) + (line_height / 2)
		+ data->player->camera_y_offset;
	if (*draw_start < 0)
		*draw_start = 0;
	if (*draw_end >= data->mlx_data->mlx->height)
		*draw_end = data->mlx_data->mlx->height - 1;
}

float	calculate_wall_x(t_data *data, t_ray *ray)
{
	float	wall_x;

	if (ray->side == 0)
		wall_x = data->player->pos.y + ray->perp_wall_dist * ray->dir.y;
	else
		wall_x = data->player->pos.x + ray->perp_wall_dist * ray->dir.x;
	wall_x -= floor(wall_x);
	return (wall_x);
}

mlx_texture_t	*get_wall_texture(t_data *data, t_ray *ray)
{
	if (ray->side == 0)
	{
		if (ray->step_x > 0)
			return (data->texture->east);
		else
			return (data->texture->west);
	}
	else
	{
		if (ray->step_y > 0)
			return (data->texture->south);
		else
			return (data->texture->north);
	}
}

void	init_draw_context(t_data *data, t_ray *ray, t_draw_context *ctx)
{
	init_vertical_line(data, ray, &ctx->line_height);
	calculate_draw_limits(data, ctx->line_height, &ctx->draw_start,
		&ctx->draw_end);
	ctx->wall_texture = get_wall_texture(data, ray);
	ctx->wall_x = calculate_wall_x(data, ray);
	ctx->tex_x = (int)(ctx->wall_x * ctx->wall_texture->width);
	if ((ray->side == 0 && ray->dir.x > 0) || (ray->side == 1
			&& ray->dir.y < 0))
		ctx->tex_x = ctx->wall_texture->width - ctx->tex_x - 1;
	ctx->step = (float)ctx->wall_texture->height / ctx->line_height;
	ctx->tex_pos = (ctx->draw_start - data->player->camera_y_offset
			- data->mlx_data->mlx->height / 2 + ctx->line_height / 2)
		* ctx->step;
}

void	draw_wall(t_data *data, t_draw_context *ctx, int x)
{
	int			tex_y;
	uint8_t		*pixel;
	uint32_t	color;
	int			y;

	y = ctx->draw_start;
	while (y < ctx->draw_end)
	{
		tex_y = (int)ctx->tex_pos & (ctx->wall_texture->height - 1);
		ctx->tex_pos += ctx->step;
		pixel = ctx->wall_texture->pixels + (tex_y * ctx->wall_texture->width
				+ ctx->tex_x) * ctx->wall_texture->bytes_per_pixel;
		color = (pixel[0] << 24) | (pixel[1] << 16) | (pixel[2] << 8) | pixel[3];
		ft_put_pixel(data->mlx_data->img_buffer, x, y++, color);
	}
}

void	draw_ceiling(t_data *data, int x, int draw_start)
{
	int	y;

	y = 0;
	while (y < draw_start)
		ft_put_pixel(data->mlx_data->img_buffer, x, y++,
			data->texture->ceiling_color);
}

void	draw_floor(t_data *data, int x, int draw_end)
{
	int	y;

	y = draw_end;
	while (y < data->mlx_data->mlx->height)
		ft_put_pixel(data->mlx_data->img_buffer, x, y++,
			data->texture->floor_color);
}

void	draw_textured_vertical_line(t_data *data, t_ray *ray, int x)
{
	t_draw_context	ctx;

	init_draw_context(data, ray, &ctx);
	draw_ceiling(data, x, ctx.draw_start);
	draw_wall(data, &ctx, x);
	draw_floor(data, x, ctx.draw_end);
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
	draw_textured_vertical_line(data, &ray, x);
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

void	handle_key_press(t_data *data, mlx_key_data_t keydata, bool is_pressed)
{
	if (keydata.key == MLX_KEY_W)
		data->player->key_state.w = is_pressed;
	else if (keydata.key == MLX_KEY_S)
		data->player->key_state.s = is_pressed;
	else if (keydata.key == MLX_KEY_A)
		data->player->key_state.a = is_pressed;
	else if (keydata.key == MLX_KEY_D)
		data->player->key_state.d = is_pressed;
	else if (keydata.key == MLX_KEY_LEFT)
		data->player->key_state.left = is_pressed;
	else if (keydata.key == MLX_KEY_RIGHT)
		data->player->key_state.right = is_pressed;
	else if (keydata.key == MLX_KEY_UP)
		data->player->key_state.up = is_pressed;
	else if (keydata.key == MLX_KEY_DOWN)
		data->player->key_state.down = is_pressed;
	else if (keydata.key == MLX_KEY_LEFT_SHIFT)
		data->player->key_state.shift = is_pressed;
}

void	handle_movement(t_data *data)
{
	t_vec2f	new_pos;
	float	speed;

	if (data->player->key_state.w == data->player->key_state.s)
		return ;
	speed = MOVE_SPEED;
	if (data->player->key_state.w)
		speed *= 1.0f;
	if (data->player->key_state.s)
		speed *= -1.0f;
	else if (data->player->key_state.shift)
		speed *= 2.0f;
	new_pos.x = data->player->pos.x + data->player->dir.x * speed;
	new_pos.y = data->player->pos.y + data->player->dir.y * speed;
	if (get_map_value(data, (int)new_pos.x, (int)data->player->pos.y) == '0')
		data->player->pos.x = new_pos.x;
	if (get_map_value(data, (int)data->player->pos.x, (int)new_pos.y) == '0')
		data->player->pos.y = new_pos.y;
}

void	handle_strafe(t_data *data)
{
	t_vec2f	new_pos;
	float	speed;

	if (data->player->key_state.a == data->player->key_state.d)
		return ;
	speed = MOVE_SPEED;
	if (data->player->key_state.shift)
		speed *= 2.0f;
	if (data->player->key_state.d)
		speed *= 1.0f;
	if (data->player->key_state.a)
		speed *= -1.0f;
	new_pos.x = data->player->pos.x + data->player->plane.x * speed;
	new_pos.y = data->player->pos.y + data->player->plane.y * speed;
	if (get_map_value(data, (int)new_pos.x, (int)data->player->pos.y) == '0')
		data->player->pos.x = new_pos.x;
	if (get_map_value(data, (int)data->player->pos.x, (int)new_pos.y) == '0')
		data->player->pos.y = new_pos.y;
}

void	handle_rotation(t_data *data)
{
	float	angle;
	float	old_dir_x;
	float	old_plane_x;
	float	cos_angle;
	float	sin_angle;

	if (data->player->key_state.left == data->player->key_state.right)
		return ;
	if (data->player->key_state.right)
		angle = ROTATE_SPEED;
	if (data->player->key_state.left)
		angle = -ROTATE_SPEED;
	cos_angle = cosf(angle);
	sin_angle = sinf(angle);
	old_dir_x = data->player->dir.x;
	old_plane_x = data->player->plane.x;
	data->player->dir.x = data->player->dir.x * cos_angle - data->player->dir.y
		* sin_angle;
	data->player->dir.y = old_dir_x * sin_angle + data->player->dir.y
		* cos_angle;
	data->player->plane.x = data->player->plane.x * cos_angle
		- data->player->plane.y * sin_angle;
	data->player->plane.y = old_plane_x * sin_angle + data->player->plane.y
		* cos_angle;
}

void	handle_camera_tilt(t_data *data)
{
	int	offset;

	if (data->player->key_state.up == data->player->key_state.down)
		return ;
	offset = data->player->camera_y_offset;
	if (data->player->key_state.up)
		offset += 10;
	if (data->player->key_state.down)
		offset -= 10;
	if (offset > 600)
		offset = 600;
	if (offset < -600)
		offset = -600;
	data->player->camera_y_offset = offset;
}

void	key_callback(mlx_key_data_t keydata, void *vdata)
{
	t_data	*data;

	data = (t_data *)vdata;
	if (keydata.key == MLX_KEY_ESCAPE)
		exit_program(data, 0);
	handle_key_press(data, keydata, keydata.action == MLX_PRESS
		|| keydata.action == MLX_REPEAT);
}
