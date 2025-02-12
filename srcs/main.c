#include "cub3d.h"

void free_data(t_data *data)
{
	free(data->map_data);
	free(data->mlx_data);
	free(data->player_data);
	free(data->texture_data);
	free(data);
}

void exit_program(t_data *data, int status)
{
	if (data->map_data->map_fd != -1)
		close(data->map_data->map_fd);
	if (data->mlx_data->mlx)
		mlx_terminate(data->mlx_data->mlx);
	free_data(data);
	exit(status);
}

int main(int ac, char **av)
{
	t_data *data;
	(void)av;

	mlx_set_setting(MLX_FULLSCREEN, false);
	if (ac < 2)
	{
		ft_putstr_fd("Error: No map file provided\n", 2);
		return (1);
	}
	data = ft_calloc(1, sizeof(t_data));
	if (!data)
		return (1);
	if (init_data(data, av[1]) == -1)
		exit_program(data, 1);
	mlx_loop(data->mlx_data->mlx);
	exit_program(data, 0);
}
