/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_texture.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:15:34 by peli              #+#    #+#             */
/*   Updated: 2025/03/07 14:57:04 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	free_split(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

int	get_texture(char *line, t_data *data)
{
	char			**path;
	char			*path_texture;
	mlx_texture_t	*texture;

	path = ft_split(line, ' ');
	if (!path || !path[1] || path[2])
	{
		ft_putstr_fd("Error: missing path for texture\n", 2);
		return (0);
	}
	path_texture = ft_strtrim(path[1], "\n");
	texture = mlx_load_png(path_texture);
	if (!texture)
	{
		ft_putstr_fd("Error: unable to open texture\n", 2);
		return (0);
	}
	if (!ft_strncmp(line, "NO ", 3))
		data->texture->north = texture;
	if (!ft_strncmp(line, "SO ", 3))
		data->texture->south = texture;
	if (!ft_strncmp(line, "WE ", 3))
		data->texture->west = texture;
	if (!ft_strncmp(line, "EA ", 3))
		data->texture->east = texture;
	if (!ft_strncmp(line, "F ", 2))
		data->texture->floor = texture;
	if (!ft_strncmp(line, "C ", 2))
		data->texture->ceiling = texture;
	free_split(path);
	free(path_texture);
	return (1);
}

int	check_is_number(char **str)
{
	int	i;
	int	j;

	i = 0;
	while (str[i])
	{
		j = 0;
		if (i == 0)
			j += 2;
		while (str[i][j])
		{
			if (ft_isdigit(str[i][j] - '0') || str[i][j] == ' ')
				return (0);
			j++;
		}
		i++;
	}
	if (i != 3 || str[2][0] == '\n' || str[2][0] == '\0')
		return (0);
	return (1);
}

int	read_map_1(t_data *data, char *filename)
{
	char	*line;

	data->map_data->map_fd = open(filename, O_RDONLY);
	line = get_next_line(data->map_data->map_fd);
	while (line)
	{
		if (!ft_strncmp(line, "NO ", 3) || !ft_strncmp(line, "SO ", 3)
			|| !ft_strncmp(line, "WE ", 3) || !ft_strncmp(line, "EA ", 3)
			|| !ft_strncmp(line, "F ", 2) || !ft_strncmp(line, "C ", 2))
			if (!get_texture(line, data))
				return (free(line), 0);
		free(line);
		line = get_next_line(data->map_data->map_fd);
	}
	close(data->map_data->map_fd);
	return (1);
}
