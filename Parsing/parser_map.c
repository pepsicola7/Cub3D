/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_map.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:16:27 by peli              #+#    #+#             */
/*   Updated: 2025/02/26 20:06:29 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	check_joueur(char **map)
{
	int	i;
	int	j;
	int	player_count;

	i = 0;
	player_count = 0;
	while (map[i])
	{
		j = 0;
		while (map[i][j] != '\0' && map[i][j] != '\n')
		{
			if (map[i][j] == 'E' || map[i][j] == 'S' || map[i][j] == 'N'
			|| map[i][j] == 'W')
				player_count++;
			j++;
		}
		i++;
	}
	if (player_count == 1)
		return (1);
	if (player_count == 0)
		printf("Error: Aucun joueur trouvé sur la carte\n");
	else
		printf("Error: Trop de joueurs trouvés (%d)\n", player_count);
	return (0);
}
int	check_element(char **map)
{
	int	i;
	int	j;

	i = 0;
	while (map[i])
	{
		j = 0;
		while (map[i][j] != '\0' && map[i][j] != '\n')
		{
			if (map[i][j] != '1' && map[i][j] != '0' && map[i][j] != 'N'
			&& map[i][j] != 'S' && map[i][j] != 'E' && map[i][j] != 'W' && map[i][j] != ' ')
			{
				printf("Error of the element\n");
				return (0);
			}
			j++;
		}
		i++;
	}
	return (1);
}
void	map_width(t_data *data)
{
	int		i;
	int		j;
	char	**map;

	i = 0;
	j = 0;
	map = data->map_data->map;
	while (map[i])
	{
		while (map[i][j])
			j++;
		i++;
	}
	data->map_data->width = j - 1;
	// printf("width is %d", j - 1);
}


int check_espace(char **map, t_data *data)
{
	int i, j;
	int line_length;

	i = 0;
	while (map[i])
	{
		j = 0;
		line_length = (int)ft_strlen(map[i]);
		while (map[i][j])
		{
			if (map[i][j] == ' ')
			{
				if (i == 0 || i == data->map_data->height - 1 || j == 0 || j == line_length - 1)
				{
					if ((i > 0 && map[i - 1][j] == '0') || (i < data->map_data->height - 1 && map[i + 1][j] == '0')
						|| (j > 0 && map[i][j - 1] == '0') || (j < line_length - 1 && map[i][j + 1] == '0'))
						return (0);
				}
				else if ((i > 0 && map[i - 1][j] == '0') || (i < data->map_data->height - 1 && map[i + 1][j] == '0')
					|| (j > 0 && map[i][j - 1] == '0') || (j < line_length - 1 && map[i][j + 1] == '0'))
					return (0);
			}
			j++;
		}
		i++;
	}
	return (1);
}

int	check_line_vide(char **map)
{
	int	i;

	i = 0;
	while (map[i])
	{
		if (map[i][0] == '\0' || ft_strcmp(map[i], "\n") == 0)
		{
			printf("Error: empty line detected\n");
			return (0);
		}
		i++;
	}
	return (1);
}
void	remplace_map(t_data *data)
{
	char	**map;
	int		i;
	int		j;
	int		line_len;
	char	*new_line;

	i = 0;
	map = data->map_data->map;
	while (map[i])
	{
		line_len = ft_strlen(map[i]);
		new_line = ft_calloc(data->map_data->width + 1, sizeof(char));
		if (!new_line)
			return ;
		j = 0;
		while (map[i][j])
		{
			if (map[i][j] == '\t' || map[i][j] == ' ')
				new_line[j] = '1';
			else
				new_line[j] = map[i][j];
			j++;
		}
		while (j < data->map_data->width)
		{
			new_line[j] = '1';
			j++;
		}
		new_line[j] = '\0';
		free(data->map_data->map[i]); 
		data->map_data->map[i] = new_line;
		i++;
	}
	ft_printf_map(data->map_data->map);
}

void	position_player(t_data *data)
{
	char	**map;
	int		i;
	int		j;

	i = 0;
	map = data->map_data->map;
	data->player_data = ft_calloc(1, sizeof(t_player));
	while (map[i])
	{
		j = 0;
		while (map[i][j])
		{
			if (map[i][j] =='N' || map[i][j] =='S' || map[i][j] =='E' || map[i][j] =='W')
			{
				data->player_data->pos.x = i;
				data->player_data->pos.y = j + 1;
				printf("position of x : %f\n", data->player_data->pos.x);
				printf("position of y : %f\n", data->player_data->pos.y);
				return ;
			}
			j++;
		}
		i++;
	}
}

int	check_description(t_data *data)
{
	char	**map;

	map_width(data);
	map = data->map_data->map;
	if (!check_element(map))
		return (0);
	if (!check_joueur(map))
		return (0);
	if (!check_espace(map, data))
		return (printf("error of the espace\n"), 0);
	if (!check_line_vide(map))
		return (0);
	remplace_map(data);
	position_player(data);
	
	return (1);
}
