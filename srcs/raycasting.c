#include "cub3d.h"

float	deg_to_rad(int degre)
{
	return (degre * (M_PI / 180));
}

float	calculate_distance(t_vec2f a, t_vec2f b, float angle)
{
	return (cos(deg_to_rad(angle)) * (b.x - a.x) - sin(deg_to_rad(angle))
		* (b.y - a.y));
}
