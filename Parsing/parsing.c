/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 14:43:10 by peli              #+#    #+#             */
/*   Updated: 2025/02/17 16:12:51 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	get_texture(char *line, t_data *data)
{
	char			*path;
	mlx_texture_t	*texture;

	path = ft_strchr(line, ' ');
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

int	get_color(char *line, t_data *data)
{
	if (ft_strncmp(line, "F ", 2) == 0)
	line = NULL;
	data = NULL;
	return (1);
}

int	is_line_map(char *line)
{
	line = NULL;
	return (1);
}

int	check_description(char **map)
{
	map = NULL;
	return (1);
}

int	read_map_2(t_data *data)
{
	int		i;
	char	*line;

	i = 0;
	line = NULL;
	while (get_next_line(data->map_data->map_fd))
	{
		if (is_line_map(line))
		{
			data->map_data->map[i] = line;
			i++;
		}
		free (line);
	}
	if (check_description(data->map_data->map))
		return (0);
	return (1);
}

int	read_map_1(t_data *data)
{
	char	*line;
	int		in_map;

	in_map = 0;
	line = NULL;
	while (get_next_line(data->map_data->map_fd))
	{
		line = get_next_line(data->map_data->map_fd);
		if ( ft_strncmp(line, "NO ", 3) == 0 || ft_strncmp(line, "SO ", 3) == 0
			|| ft_strncmp(line, "WE ", 3) || ft_strncmp(line, "EA ", 3) == 0)
		{
			if (!get_texture(line, data))
				return (0);
		}
		if (ft_strncmp(line, "F ", 2) == 0 || ft_strncmp(line, "C ", 2) == 0)
		{
			if (get_color(line, data) == -1)
				return (0);
		}
		free (line);
	}
	return (1);
}

int	parsing(t_data *data, char *filename)
{
	data->map_data->map_fd = open(filename, O_RDONLY);
	if (data->map_data->map_fd == -1)
		return (0);
	read_map_1(data); // pour le texture et les couleur;
	read_map_2(data); // pour la description;
	return (1);
}