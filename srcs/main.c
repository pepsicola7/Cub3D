#include "cub3d.h"

int main(int ac, char **av)
{
	t_data *data;
	(void)av;

	mlx_set_setting(MLX_FULLSCREEN, true);
	if (ac < 2)
	{
		ft_putstr_fd("Error: No map file provided\n", 2);
		return (1);
	}
	data = ft_calloc(1, sizeof(t_data));
	if (!data)
		return (1);
	parsing(data, av[1]);
	// if (init_data(data, av[1]) == -1)
	// 	exit_program(data, 1);
	mlx_loop(data->mlx_data->mlx);
	mlx_terminate(data->mlx_data->mlx);
}
