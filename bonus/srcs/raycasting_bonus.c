#include "cub3d.h"

int	get_map_value(t_data *data, int x, int y)
{
	if (x < 0 || x >= data->map_data->width || y < 0
		|| y >= data->map_data->height)
		return (1);
	return (data->map_data->map[y * data->map_data->width + x]);
}

inline void	perform_dda(t_data *data, t_ray *ray)
{
	int	map_value;

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
		map_value = get_map_value(data, ray->map_x, ray->map_y);
		if (map_value == '1' || map_value == '2')
			ray->hit = map_value - '0';
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
	if (ray->hit == 2)
		return (data->texture->doors);
	else if (ray->side == 0)
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

void	set_wall_distance(t_data *data, t_ray *ray, t_floor_ceiling *fc)
{
	if (ray->side == 0)
		fc->wall_dist = (ray->map_x - data->player->pos.x + (1 - ray->step_x)
				/ 2) / ray->dir.x;
	else
		fc->wall_dist = (ray->map_y - data->player->pos.y + (1 - ray->step_y)
				/ 2) / ray->dir.y;
	fc->wall_x = data->player->pos.x + fc->wall_dist * ray->dir.x;
	fc->wall_y = data->player->pos.y + fc->wall_dist * ray->dir.y;
}

void	calculate_ceiling_tex_coords(t_data *data, t_floor_ceiling *fc, int y)
{
	fc->adjusted_y = y - data->player->camera_y_offset;
	fc->current_dist = data->mlx_data->mlx->height / (2.0
			* (data->mlx_data->mlx->height - fc->adjusted_y)
			- data->mlx_data->mlx->height);
	fc->weight = fc->current_dist / fc->wall_dist;
	fc->floor_x = fc->weight * fc->wall_x + (1.0 - fc->weight)
		* data->player->pos.x;
	fc->floor_y = fc->weight * fc->wall_y + (1.0 - fc->weight)
		* data->player->pos.y;
	fc->tex_x = (int)((fc->floor_x - (int)fc->floor_x)
			* data->texture->ceiling->width);
	fc->tex_y = (int)((fc->floor_y - (int)fc->floor_y)
			* data->texture->ceiling->height);
}

void	draw_ceiling_pixel(t_data *data, t_floor_ceiling *fc, int x, int y)
{
	fc->pixel = data->texture->ceiling->pixels + (fc->tex_y
			* data->texture->ceiling->width + fc->tex_x)
		* data->texture->ceiling->bytes_per_pixel;
	fc->color = (fc->pixel[0] << 24) | (fc->pixel[1] << 16) | (fc->pixel[2] << 8) | fc->pixel[3];
	ft_put_pixel(data->mlx_data->img_buffer, x, y, fc->color);
}

void	draw_ceiling(t_data *data, t_ray *ray, int x, int draw_start)
{
	int				y;
	t_floor_ceiling	fc;

	set_wall_distance(data, ray, &fc);
	y = 0;
	while (y < draw_start)
	{
		calculate_ceiling_tex_coords(data, &fc, y);
		draw_ceiling_pixel(data, &fc, x, y);
		y++;
	}
}

void	calculate_floor_tex_coords(t_data *data, t_floor_ceiling *fc, int y)
{
	fc->adjusted_y = y - data->player->camera_y_offset;
	fc->current_dist = data->mlx_data->mlx->height / (2.0 * fc->adjusted_y
			- data->mlx_data->mlx->height);
	fc->weight = fc->current_dist / fc->wall_dist;
	fc->floor_x = fc->weight * fc->wall_x + (1.0 - fc->weight)
		* data->player->pos.x;
	fc->floor_y = fc->weight * fc->wall_y + (1.0 - fc->weight)
		* data->player->pos.y;
	fc->tex_x = (int)(fc->floor_x * data->texture->floor->width)
		% data->texture->floor->width;
	fc->tex_y = (int)(fc->floor_y * data->texture->floor->height)
		% data->texture->floor->height;
	if (fc->tex_x < 0)
		fc->tex_x += data->texture->floor->width;
	if (fc->tex_y < 0)
		fc->tex_y += data->texture->floor->height;
}

void	draw_floor_pixel(t_data *data, t_floor_ceiling *fc, int x, int y)
{
	fc->pixel = data->texture->floor->pixels + (fc->tex_y
			* data->texture->floor->width + fc->tex_x)
		* data->texture->floor->bytes_per_pixel;
	fc->color = (fc->pixel[0] << 24) | (fc->pixel[1] << 16) | (fc->pixel[2] << 8) | fc->pixel[3];
	ft_put_pixel(data->mlx_data->img_buffer, x, y, fc->color);
}

void	draw_floor(t_data *data, t_ray *ray, int x, int draw_end)
{
	int				y;
	t_floor_ceiling	fc;

	set_wall_distance(data, ray, &fc);
	y = draw_end;
	while (y < data->mlx_data->mlx->height)
	{
		if (y - data->player->camera_y_offset <= 0)
		{
			y++;
			continue ;
		}
		calculate_floor_tex_coords(data, &fc, y);
		draw_floor_pixel(data, &fc, x, y);
		y++;
	}
}

void	draw_textured_vertical_line(t_data *data, t_ray *ray, int x)
{
	t_draw_context	ctx;

	init_draw_context(data, ray, &ctx);
	draw_ceiling(data, ray, x, ctx.draw_start);
	draw_wall(data, &ctx, x);
	draw_floor(data, ray, x, ctx.draw_end);
}

void	init_ray(t_data *data, t_ray *ray, int x)
{
	float	camera_x;

	camera_x = 2 * x / (float)data->mlx_data->mlx->width - 1;
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
	while (++x < data->mlx_data->mlx->width)
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
	else if (keydata.key == MLX_KEY_SPACE)
		data->player->key_state.space = is_pressed;
	else if (keydata.key == MLX_KEY_LEFT_CONTROL)
		data->player->key_state.control = is_pressed;
	else if (keydata.key == MLX_KEY_TAB)
		data->player->key_state.tab = is_pressed;
}

void	make_player_jump(t_data *data)
{
	data->player->is_jumping = 1;
	data->player->is_grounded = 0;
	data->player->vertical_velocity = data->player->jump_force;
	data->player->can_jump = false;
}

void	handle_jump(t_data *data, float d_time)
{
	float	previous_height;
	float	height_delta;

	previous_height = data->player->current_height;
	if (data->player->key_state.space && data->player->is_grounded
		&& data->player->can_jump)
		make_player_jump(data);
	if (!data->player->key_state.space)
		data->player->can_jump = true;
	if (!data->player->is_grounded)
	{
		data->player->vertical_velocity -= data->player->gravity * d_time;
		data->player->current_height += data->player->vertical_velocity
			* d_time;
		if (data->player->current_height <= data->player->ground_level)
		{
			data->player->current_height = data->player->ground_level;
			data->player->is_grounded = 1;
			data->player->is_jumping = 0;
			data->player->vertical_velocity = 0.0f;
		}
		height_delta = data->player->current_height - previous_height;
		data->player->camera_y_offset += height_delta * JUMP_SPEED;
	}
}

void	handle_movement(t_data *data)
{
	t_vec2f	new_pos;
	float	speed;
	int		map_value;

	if (data->player->key_state.w == data->player->key_state.s)
		return ;
	speed = MOVE_SPEED * data->mlx_data->mlx->delta_time;
	if (data->player->key_state.w)
		speed *= 1.0f;
	if (data->player->key_state.s)
		speed *= -1.0f;
	if (data->player->key_state.shift)
		speed *= 2.0f;
	new_pos.x = data->player->pos.x + data->player->dir.x * speed;
	new_pos.y = data->player->pos.y + data->player->dir.y * speed;
	map_value = get_map_value(data, (int)new_pos.x, (int)data->player->pos.y);
	if (map_value == '0' || map_value == '3')
		data->player->pos.x = new_pos.x;
	map_value = get_map_value(data, (int)data->player->pos.x, (int)new_pos.y);
	if (map_value == '0' || map_value == '3')
		data->player->pos.y = new_pos.y;
}

void	handle_strafe(t_data *data)
{
	t_vec2f	new_pos;
	float	speed;
	int		map_value;

	if (data->player->key_state.a == data->player->key_state.d)
		return ;
	speed = MOVE_SPEED * data->mlx_data->mlx->delta_time;
	if (data->player->key_state.shift)
		speed *= 2.0f;
	if (data->player->key_state.d)
		speed *= 1.0f;
	if (data->player->key_state.a)
		speed *= -1.0f;
	new_pos.x = data->player->pos.x + data->player->plane.x * speed;
	new_pos.y = data->player->pos.y + data->player->plane.y * speed;
	map_value = get_map_value(data, (int)new_pos.x, (int)data->player->pos.y);
	if (map_value == '0' || map_value == '3')
		data->player->pos.x = new_pos.x;
	map_value = get_map_value(data, (int)data->player->pos.x, (int)new_pos.y);
	if (map_value == '0' || map_value == '3')
		data->player->pos.y = new_pos.y;
}

void	change_player_yaw(t_data *data, float yaw)
{
	float	old_dir_x;
	float	old_plane_x;
	float	cos_yaw;
	float	sin_yaw;

	cos_yaw = cosf(yaw);
	sin_yaw = sinf(yaw);
	old_dir_x = data->player->dir.x;
	old_plane_x = data->player->plane.x;
	data->player->dir.x = data->player->dir.x * cos_yaw - data->player->dir.y
		* sin_yaw;
	data->player->dir.y = old_dir_x * sin_yaw + data->player->dir.y * cos_yaw;
	data->player->plane.x = data->player->plane.x * cos_yaw
		- data->player->plane.y * sin_yaw;
	data->player->plane.y = old_plane_x * sin_yaw + data->player->plane.y
		* cos_yaw;
}

void	change_player_pitch(t_data *data, float pitch)
{
	data->player->camera_y_offset -= pitch;
	if (data->player->camera_y_offset > 600)
		data->player->camera_y_offset = 600;
	if (data->player->camera_y_offset < -600)
		data->player->camera_y_offset = -600;
}

void	handle_cursor(double xpos, double ypos, void *param)
{
	static bool	first_call = true;
	t_data		*data;
	float		yaw;
	float		pitch;

	data = (t_data *)param;
	if (first_call)
	{
		mlx_set_mouse_pos(data->mlx_data->mlx, data->mlx_data->mlx->width / 2,
			data->mlx_data->mlx->height / 2);
		first_call = false;
		return ;
	}
	yaw = (xpos - (data->mlx_data->mlx->width / 2)) * MOUSE_SENSITIVITY;
	pitch = (ypos - (data->mlx_data->mlx->height / 2));
	change_player_yaw(data, yaw * data->mlx_data->mlx->delta_time);
	change_player_pitch(data, pitch);
	mlx_set_mouse_pos(data->mlx_data->mlx, data->mlx_data->mlx->width / 2,
		data->mlx_data->mlx->height / 2);
}

void	handle_rotation(t_data *data)
{
	float	yaw;

	if (data->player->key_state.left == data->player->key_state.right)
		return ;
	if (data->player->key_state.right)
		yaw = ROTATE_SPEED * data->mlx_data->mlx->delta_time;
	else if (data->player->key_state.left)
		yaw = -ROTATE_SPEED * data->mlx_data->mlx->delta_time;
	change_player_yaw(data, yaw);
}

void	handle_camera_tilt(t_data *data)
{
	int	offset;

	if (data->player->key_state.up == data->player->key_state.down)
		return ;
	offset = data->player->camera_y_offset;
	if (data->player->key_state.up)
		offset += (int)TILT_SPEED * data->mlx_data->mlx->delta_time;
	if (data->player->key_state.down)
		offset -= (int)TILT_SPEED * data->mlx_data->mlx->delta_time;
	if (offset > 600)
		offset = 600;
	if (offset < -600)
		offset = -600;
	data->player->camera_y_offset = offset;
}

void	door_actions(t_data *data)
{
	char	map_value;
	t_vec2f	ray;
	float	distance;

	if (get_map_value(data, (int)data->player->pos.x,
			(int)data->player->pos.y) == '3')
		return ;
	ray.x = data->player->pos.x + data->player->dir.x * 0.5;
	ray.y = data->player->pos.y + data->player->dir.y * 0.5;
	distance = sqrt(pow(data->player->pos.x - ray.x, 2)
			+ pow(data->player->pos.y - ray.y, 2));
	if (distance < 1.0)
	{
		map_value = get_map_value(data, (int)ray.x, (int)ray.y);
		if (map_value == '2')
			data->map_data->map[(int)ray.y * data->map_data->width
				+ (int)ray.x] = '3';
		else if (map_value == '3')
			data->map_data->map[(int)ray.y * data->map_data->width
				+ (int)ray.x] = '2';
	}
}

void	key_callback(mlx_key_data_t keydata, void *vdata)
{
	t_data	*data;

	data = (t_data *)vdata;
	if (keydata.key == MLX_KEY_ESCAPE)
		exit_program(data, 0);
	else if (keydata.key == MLX_KEY_E && (keydata.action == MLX_PRESS
			|| keydata.action == MLX_REPEAT))
		door_actions(data);
	else
		handle_key_press(data, keydata, keydata.action == MLX_PRESS
			|| keydata.action == MLX_REPEAT);
}
