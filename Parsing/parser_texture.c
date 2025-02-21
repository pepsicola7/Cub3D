/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_texture.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:15:34 by peli              #+#    #+#             */
/*   Updated: 2025/02/21 18:39:06 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	get_texture(char *line, t_data *data)
{
	char			*path;
	mlx_texture_t	*texture;

	path = ft_strchr(line, ' '); //penser a ignrer tous les espaces blanches
	if (!path)
	{
		printf ("There have no name of texture");
		return (0);
	}
	texture = mlx_load_png(path);
	if (!texture)
		return (printf("The texture isn't exist"), 0);
	if (ft_strncmp(line, "NO ", 3))
		data->texture_data->north = texture;
	if (ft_strncmp(line, "SO ", 3))
		data->texture_data->south = texture;
	if (ft_strncmp(line, "WE ", 3))
		data->texture_data->west = texture;
	if (ft_strncmp(line, "EA ", 3))
		data->texture_data->east = texture;
	return (1);
}

void	free_split(char **arr)
{
	int i;

	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

int	get_color(char *line, t_data *data)
{
	char	**str;
	int		r;
	int		g;
	int		b;
	int		i;

	i = 0;
	str = ft_split(line, ',');
	while (str[i])
		i++;
	r = ft_atoi(str[0]);
	g = ft_atoi(str[1]);
	b = ft_atoi(str[2]);
	if (i != 3 || r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
	{
		free_split(str);
		return (printf("Erreur: Valeur RGB incorrecte\n"), 0);
	}
	if (ft_strncmp(line, "F ", 2) == 0)
		data->texture_data->floor_color = r << 16 | g << 8 | b;
	if (ft_strncmp(line, "C ", 2) == 0)
		data->texture_data->ceiling_color = r << 16 | g << 8 | b;
	free_split(str);
	return (1);
}

int	read_map_1(t_data *data)
{
	char	*line;

	line = get_next_line(data->map_data->map_fd);
	// printf("%d\n", data->map_data->map_fd);
	// printf("%s\n", line);
	// fflush(stdout);
	while (line)
	{
		if (ft_strncmp(line, "NO ", 3) == 0 || ft_strncmp(line, "SO ", 3) == 0
			|| ft_strncmp(line, "WE ", 3) || ft_strncmp(line, "EA ", 3) == 0)
		{
			if (!get_texture(line, data))
			{
				free(line);
				return (0);
			}
		}
		if (ft_strncmp(line, "F ", 2) == 0 || ft_strncmp(line, "C ", 2) == 0)
		{
			if (get_color(line, data))
			{
				free(line);
				return (0);
			}
		}
		free (line);
		line = get_next_line(data->map_data->map_fd);
	}
	return (1);
}
