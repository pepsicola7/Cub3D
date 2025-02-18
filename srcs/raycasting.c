#include "cub3d.h"

float	calculate_distance(float x1, float y1, float x2, float y2)
{
	return (sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
}

void	cast_vertical_ray(t_data *data, t_ray *ray)
{
	printf("Vertical raycast\n");
	ray->depth_of_field = 0;
	if (ray->ray_angle == 0 || ray->ray_angle == M_PI) // Perfectly horizontal
	{
		printf("Perfectly horizontal\n");
		ray->depth_of_field = 8;
	}
	else if (ray->ray_angle > M_PI) // Facing up
	{
		printf("Facing up\n");
		ray->ray_y = ((int)data->player_data->pos.y / 64) * 64 - EPSILON;
		// Prevent division by zero by checking tan(ray_angle)
		if (fabs(tan(ray->ray_angle)) < EPSILON)
		{
			ray->ray_x = data->player_data->pos.x; // Ray is vertical
		}
		else
		{
			ray->ray_x = data->player_data->pos.x + (data->player_data->pos.y
					- ray->ray_y) * (-1 / tan(ray->ray_angle));
		}
		ray->y_offset = -64;
		ray->x_offset = -ray->y_offset * (-1 / tan(ray->ray_angle));
	}
	else // Facing down
	{
		printf("Facing down\n");
		ray->ray_y = ((int)data->player_data->pos.y / 64) * 64 + 64;
		// Prevent division by zero by checking tan(ray_angle)
		if (fabs(tan(ray->ray_angle)) < EPSILON)
		{
			ray->ray_x = data->player_data->pos.x; // Ray is vertical
		}
		else
		{
			ray->ray_x = data->player_data->pos.x + (data->player_data->pos.y
					- ray->ray_y) * (-1 / tan(ray->ray_angle));
		}
		ray->y_offset = 64;
		ray->x_offset = -ray->y_offset * (-1 / tan(ray->ray_angle));
	}
	while (ray->depth_of_field < 8)
	{
		printf("Depth of field vertical: %d\n", ray->depth_of_field);
		printf("Ray x-y: %f-%f\n", ray->ray_x, ray->ray_y);
		ray->map_x = (int)(ray->ray_x / 64);
		ray->map_y = (int)(ray->ray_y / 64);
		if (ray->map_y >= 0 && ray->map_y < data->map_data->height
			&& ray->map_x >= 0 && ray->map_x < data->map_data->width)
		{
			if (data->map_data->map[ray->map_y][ray->map_x] >= '1') // Hit wall
			{
				printf("Hit wall at (%d, %d)\n", ray->map_x, ray->map_y);
				ray->depth_of_field = 8;
			}
			else
			{
				ray->ray_x += ray->x_offset;
				ray->ray_y += ray->y_offset;
				ray->depth_of_field++;
				printf("Moving to (%f, %f)\n", ray->ray_x, ray->ray_y);
			}
		}
		else
		{
			printf("Vertical ray out of bounds, breaking.\n");
			break ;
		}
	}
	ray->distance = calculate_distance(data->player_data->pos.x,
			data->player_data->pos.y, ray->ray_x, ray->ray_y);
}

void	cast_horizontal_ray(t_data *data, t_ray *ray)
{
	printf("Horizontal raycast\n");
	ray->depth_of_field = 0;
	if (ray->ray_angle == PI_2 || ray->ray_angle == PI_3) // Perfectly vertical
	{
		printf("Perfectly vertical\n");
		ray->depth_of_field = 8;
	}
	else if (ray->ray_angle > PI_2 && ray->ray_angle < PI_3) // Facing left
	{
		printf("Facing left\n");
		ray->ray_y = ((int)data->player_data->pos.y / 64) * 64 - EPSILON;
		// Prevent division by zero by checking tan(ray_angle)
		if (fabs(tan(ray->ray_angle)) < EPSILON)
		{
			ray->ray_x = data->player_data->pos.x; // Ray is vertical
		}
		else
		{
			ray->ray_x = data->player_data->pos.x + (data->player_data->pos.y
					- ray->ray_y) * tan(ray->ray_angle);
		}
		ray->x_offset = -64;
		ray->y_offset = -ray->x_offset * tan(ray->ray_angle);
	}
	else // Facing right
	{
		printf("Facing right\n");
		ray->ray_y = ((int)data->player_data->pos.y / 64) * 64 + 64;
		// Prevent division by zero by checking tan(ray_angle)
		if (fabs(tan(ray->ray_angle)) < EPSILON)
		{
			ray->ray_x = data->player_data->pos.x; // Ray is vertical
		}
		else
		{
			ray->ray_x = data->player_data->pos.x + (data->player_data->pos.y
					- ray->ray_y) * tan(ray->ray_angle);
		}
		ray->x_offset = 64;
		ray->y_offset = -ray->x_offset * tan(ray->ray_angle);
	}
	while (ray->depth_of_field < 8)
	{
		printf("Depth of field horizontal: %d\n", ray->depth_of_field);
		printf("Ray x-y: %f-%f\n", ray->ray_x, ray->ray_y);
		ray->map_x = (int)(ray->ray_x / 64);
		ray->map_y = (int)(ray->ray_y / 64);
		if (ray->map_y >= 0 && ray->map_y < data->map_data->height
			&& ray->map_x >= 0 && ray->map_x < data->map_data->width)
		{
			if (data->map_data->map[ray->map_y][ray->map_x] >= '1') // Hit wall
			{
				ray->depth_of_field = 8;
				printf("Hit wall at (%d, %d)\n", ray->map_x, ray->map_y);
			}
			else
			{
				ray->ray_x += ray->x_offset;
				ray->ray_y += ray->y_offset;
				ray->depth_of_field++;
			}
		}
		else
		{
			printf("Horizontal ray out of bounds, breaking.\n");
			break ;
		}
	}
	ray->distance = calculate_distance(data->player_data->pos.x,
			data->player_data->pos.y, ray->ray_x, ray->ray_y);
}

void	draw_ray(t_data *data, float ray_angle)
{
	t_ray	ray;

	t_vec2i start, end;
	start.x = (int)data->player_data->pos.x;
	start.y = (int)data->player_data->pos.y;
	ray.ray_angle = ray_angle;
	if (ray.ray_angle > M_PI) // Facing upwards
	{
		ray.ray_y = ((int)data->player_data->pos.y / 64) * 64 - 0.0001;
		ray.ray_x = data->player_data->pos.x + (data->player_data->pos.y
				- ray.ray_y) * (-1 / tan(ray.ray_angle));
	}
	else if (ray.ray_angle < M_PI) // Facing downwards
	{
		ray.ray_y = ((int)data->player_data->pos.y / 64) * 64 + 64;
		ray.ray_x = data->player_data->pos.x + (data->player_data->pos.y
				- ray.ray_y) * (-1 / tan(ray.ray_angle));
	}
	else if (ray.ray_angle == 0 || ray.ray_angle == M_PI)
	// Horizontal (left or right)
	{
		ray.ray_x = data->player_data->pos.x;
		ray.ray_y = data->player_data->pos.y;
	}
	end.x = (int)ray.ray_x;
	end.y = (int)ray.ray_y;
	// draw_line(data, start, end, YELLOW);
}

void	raycaster(t_data *data)
{
	t_ray	ray_horizontal;
	t_ray	ray_vertical;
	t_ray	ray;
	int		ray_count;

	ray.ray_angle = data->player_data->rotation - (FOV / 2 * DEGRE);
	ray_count = 0;
	while (ray_count < FOV)
	{
		printf("Raycaster function, ray number %d\n", ray_count);
		cast_horizontal_ray(data, &ray_horizontal);
		cast_vertical_ray(data, &ray_vertical);
		if (ray_horizontal.distance > ray_vertical.distance)
			ray = ray_horizontal;
		else
			ray = ray_vertical;
		printf("Ray %d: %f\n", ray_count, ray.distance);
		draw_ray(data, ray.ray_angle * ray_count);
		ray_count++;
	}
}
