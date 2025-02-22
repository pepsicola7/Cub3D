#include "cub3d.h"

int	fix_angle(int angle)
{
	if (angle < 0)
		angle += 360;
	if (angle >= 360)
		angle -= 360;
	return (angle);
}

float	deg_to_rad(int degre)
{
	return (degre * (M_PI / 180));
}

float	calculate_distance(t_vec2f a, t_vec2f b, float angle)
{
	return (cos(deg_to_rad(angle)) * (b.x - a.x) - sin(deg_to_rad(angle)) * (b.y
			- a.y));
}
