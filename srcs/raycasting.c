#include "cub3d.h"

float	calculate_distance(float x1, float y1, float x2, float y2)
{
	return (sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
}

float	cast_vertical_ray(t_data *data, t_ray *ray)
{
	ray->depth_of_field = 0;
	if (ray->ray_angle == 0 || ray->ray_angle == M_PI) // Perfectly horizontal
		ray->depth_of_field = 8;
	else if (ray->ray_angle > M_PI) // Facing up
	{
		ray->ray_y = ((int)data->player_data->pos.y / 64) * 64 - 0.0001;
		ray->ray_x = data->player_data->pos.x + (data->player_data->pos.y
				- ray->ray_y) * (-1 / tan(ray->ray_angle));
		ray->y_offset = -64;
		ray->x_offset = -ray->y_offset * (-1 / tan(ray->ray_angle));
	}
	else // Facing down
	{
		ray->ray_y = ((int)data->player_data->pos.y / 64) * 64 + 64;
		ray->ray_x = data->player_data->pos.x + (data->player_data->pos.y
				- ray->ray_y) * (-1 / tan(ray->ray_angle));
		ray->y_offset = 64;
		ray->x_offset = -ray->y_offset * (-1 / tan(ray->ray_angle));
	}
	while (ray->depth_of_field < 8)
	{
		ray->map_x = (int)(ray->ray_x / 64);
		ray->map_y = (int)(ray->ray_y / 64);
		if (ray->map_y >= 0 && ray->map_y < data->map_data->height
			&& ray->map_x >= 0 && ray->map_x < data->map_data->width)
		{
			if (data->map_data->map[ray->map_y][ray->map_x] >= '1')
				ray->depth_of_field = 8;
			else
			{
				ray->ray_x += ray->x_offset;
				ray->ray_y += ray->y_offset;
				ray->depth_of_field++;
			}
		}
	}
	return (calculate_distance(data->player_data->pos.x,
			data->player_data->pos.y, ray->ray_x, ray->ray_y));
}

float	cast_horizontal_ray(t_data *data, t_ray *ray)
{
	ray->depth_of_field = 0;
	if (ray->ray_angle == PI_2 || ray->ray_angle == PI_3) // Perfectly vertical
		ray->depth_of_field = 8;
	else if (ray->ray_angle > PI_2 && ray->ray_angle < PI_3) // Facing left
	{
		ray->ray_x = ((int)data->player_data->pos.x / 64) * 64 - 0.0001;
		ray->ray_y = data->player_data->pos.y + (data->player_data->pos.x
				- ray->ray_x) * tan(ray->ray_angle);
		ray->x_offset = -64;
		ray->y_offset = -ray->x_offset * tan(ray->ray_angle);
	}
	else // Facing right
	{
		ray->ray_x = ((int)data->player_data->pos.x / 64) * 64 + 64;
		ray->ray_y = data->player_data->pos.y + (data->player_data->pos.x
				- ray->ray_x) * tan(ray->ray_angle);
		ray->x_offset = 64;
		ray->y_offset = -ray->x_offset * tan(ray->ray_angle);
	}
	while (ray->depth_of_field < 8)
	{
		ray->map_x = (int)(ray->ray_x / 64);
		ray->map_y = (int)(ray->ray_y / 64);
		if (ray->map_y >= 0 && ray->map_y < data->map_data->height
			&& ray->map_x >= 0 && ray->map_x < data->map_data->width)
		{
			if (data->map_data->map[ray->map_y][ray->map_x] >= '1')
				ray->depth_of_field = 8;
			else
			{
				ray->ray_x += ray->x_offset;
				ray->ray_y += ray->y_offset;
				ray->depth_of_field++;
			}
		}
	}
	return (calculate_distance(data->player_data->pos.x,
			data->player_data->pos.y, ray->ray_x, ray->ray_y));
}

void draw_ray(t_data *data, float ray_angle)
{
    t_ray ray;
    t_vec2i start, end;

    // Set the player's position as the starting point of the ray
    start.x = (int)data->player_data->pos.x;
    start.y = (int)data->player_data->pos.y;

    ray.ray_angle = ray_angle;

    // Calculate the ray's collision point based on ray_angle
    // This is an example of vertical raycasting logic for the ray's endpoint.
    // Adjust this part based on your raycasting logic for horizontal rays or other conditions.

    if (ray.ray_angle > M_PI) // Facing upwards
    {
        ray.ray_y = ((int)data->player_data->pos.y / 64) * 64 - 0.0001;
        ray.ray_x = data->player_data->pos.x + (data->player_data->pos.y - ray.ray_y) * (-1 / tan(ray.ray_angle));
    }
    else if (ray.ray_angle < M_PI) // Facing downwards
    {
        ray.ray_y = ((int)data->player_data->pos.y / 64) * 64 + 64;
        ray.ray_x = data->player_data->pos.x + (data->player_data->pos.y - ray.ray_y) * (-1 / tan(ray.ray_angle));
    }
    else if (ray.ray_angle == 0 || ray.ray_angle == M_PI) // Horizontal (left or right)
    {
        // Add code here for horizontal raycasting if needed
        ray.ray_x = data->player_data->pos.x;
        ray.ray_y = data->player_data->pos.y;
    }

    // Set the end point of the ray (where the ray hits an object or the wall)
    end.x = (int)ray.ray_x;
    end.y = (int)ray.ray_y;

    // Draw the ray from the player's position to the calculated endpoint
    draw_line(data, start, end, YELLOW);
}


void	raycaster(t_data *data)
{
	t_ray	ray_horizontal;
	t_ray	ray_vertical;
	t_ray	ray;

	ray.ray_angle = data->player_data->rotation - (FOV / 2 * DEGRE);
	ray.rays = -1;
	while (++(ray.rays) < FOV)
	{
		cast_horizontal_ray(data, &ray_horizontal);
		cast_vertical_ray(data, &ray_vertical);
		if (ray_horizontal.distance > ray_vertical.distance)
			ray = ray_horizontal;
		else
			ray = ray_vertical;
		printf("Ray %d: %f\n", ray.rays, ray.distance);
		draw_ray(data, ray.ray_angle * ray.rays);
	}
}
