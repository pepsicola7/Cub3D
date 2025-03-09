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
	char *path_texture;
	mlx_texture_t	*texture;

	path = ft_split(line, ' ');
	if (!path || !path[1] || path[2])
	{
		ft_putstr_fd("Error: missing path for texture\n", 2);
		return (0);
	}
	path_texture = ft_strtrim(path[1], "\n");
	texture = mlx_load_png(path_texture);
	free_split(path);
	free(path_texture);
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

int	get_color(char *line, t_data *data)
{
	char	**str;
	int		r;
	int		g;
	int		b;

	str = ft_split(line, ',');
	if (!check_is_number(str))
		return (ft_putstr_fd("Error: Color format is incorrect.", 2), 0);
	r = ft_atoi(str[0] + 2);
	g = ft_atoi(str[1]);
	b = ft_atoi(str[2]);
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
	{
		free_split(str);
		return (ft_putstr_fd("Error: RGB value is incorrect.", 2), 0);
	}
	if (ft_strncmp(line, "F ", 2) == 0)
		data->texture->floor_color = r << 16 | g << 8 | b;
	if (ft_strncmp(line, "C ", 2) == 0)
		data->texture->ceiling_color = r << 16 | g << 8 | b;
	free_split(str);
	return (1);
}

int	read_map_1(t_data *data, char *filename)
{
	char	*line;

	data->map_data->map_fd = open(filename, O_RDONLY);
	line = get_next_line(data->map_data->map_fd);
	while (line)
	{
		if (ft_strncmp(line, "NO ", 3) == 0 || ft_strncmp(line, "SO ", 3) == 0
			|| ft_strncmp(line, "WE ", 3) == 0 || ft_strncmp(line, "EA ", 3) == 0)
		{
			if (!get_texture(line, data))
				return (free(line), 0);
		}
		if (ft_strncmp(line, "F ", 2) == 0 || ft_strncmp(line, "C ", 2) == 0)
		{
			if (!get_color(line, data))
				return (free(line), 0);
		}
		free (line);
		line = get_next_line(data->map_data->map_fd);
	}
	close(data->map_data->map_fd);
	return (1);
}
