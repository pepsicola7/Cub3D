/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 14:43:10 by peli              #+#    #+#             */
/*   Updated: 2025/02/14 16:36:38 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	get_texture(char *line, t_data *data)
{
	char			*path;
	mlx_texture_t	*texture;

	path = ft_strchr(line, ' ');
	if (!path)
		return ("There have no name of texture");
	texture = mlx_load_png(path);
	if (!texture)
		return (printf("The texture isn't exist"), 0);
	if (ft_strncmp("NO ", 3))
		data->texture_data->north = path;
	if (ft_strncmp("SO ", 3))
		data->texture_data->south = path;
	if (ft_strncmp("WE ", 3))
		data->texture_data->west = path;
	if (ft_strncmp("EA ", 3))
		data->texture_data->east = path;
	return (0);
}

int	get_color(char *line, t_data *data)
{
	if (ft_strncmp(line, "F ", 2) == 0)
		
}

int	is_line_map(char *line)
{
	
}

int	check_description(char **map)
{
	
}

int	read_map_2(t_data *data)
{
	int		i;
	i = 0;
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
		return ;
}

int	read_map_1(t_data *data)
{
	char	*line;
	int		in_map;

	in_map = 0;
	while (get_next_line(data->map_data->map_fd))
	{
		line = get_next_line(data->map_data->map_fd);
		if ( ft_strncmp(line, "NO ", 3) == 0 || ft_strncmp(line, "SO ", 3) == 0
			|| ft_strncmp(line, "WE ", 3) || ft_strncmp(line, "EA ", 3) == 0)
		{
			if (!get_texture(line, data))
				return ;
		}
		if (ft_strncmp(line, "F ", 2) == 0 || ft_strncmp(line, "C ", 2) == 0)
		{
			if (get_color(line, data) == -1)
				return ;
		}
		free (line);
	}
}

int	parsing(t_data *data, char *filename)
{
	data->map_data->map_fd = open(filename, O_RDONLY);
	if (data->map_data->map_fd == -1)
		return ;
	read_map_1(data); // pour le texture et les couleur;
	read_map_2(data); // pour la description;
}