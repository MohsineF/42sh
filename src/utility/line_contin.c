/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_contin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: onouaman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/02 10:51:44 by onouaman          #+#    #+#             */
/*   Updated: 2019/12/02 10:51:48 by onouaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "read_line.h"

int				increase_maps(char **maps)
{
	char	*new_buff;
	int		len;

	if (!maps || !(*maps))
		return (-1);
	len = ft_strlen(*maps) + MAX_MAPS;
	if ((new_buff = ft_strnew(len)) == NULL)
		ft_putendl_fd("Error to allocate new area", 2);
	ft_strcpy(new_buff, *maps);
	ft_strdel(maps);
	if ((*maps = new_buff) == NULL)
		return (-1);
	return (len);
}

int				correct_maps_(int i, int quoted[2], int *rtn, char *maps)
{
	int temp;

	temp = 0;
	if (MATCH_CLOSED(maps[i], maps[i + 1]) && ++(*rtn))
		bchar(&maps[i], 2, -1);
	else if (maps[i] == 'Q')
	{
		if (!quoted[1])
		{
			temp = count_key(&maps[i + 1], 'q');
			temp = (temp == 0) ? closed_dquot(&maps[i + 1]) : temp;
		}
		quoted[1] = (quoted[1]) ? 0 : 1;
	}
	else if (maps[i] == 'q' && !quoted[1])
	{
		if ((temp = find_flag(&maps[i + 1], 'q')) == -1)
			temp = (!quoted[0]) ? get_last_flag(&maps[i]) : temp;
		quoted[0] = (quoted[0]) ? 0 : 1;
	}
	else if (maps[i] == 'S')
		ft_bzero(quoted, sizeof(int) * 2);
	return (temp);
}

int				check_apply_esp(char *cmd)
{
	int	count;
	int i;

	if (!cmd)
		return (0);
	i = ft_strlen(cmd);
	count = 0;
	while (--i >= 0)
	{
		if (cmd[i] == '\\')
			count++;
		else
			break ;
	}
	if ((count % 2) != 0)
		return (1);
	return (0);
}

static int		incorrect_map(int *index, char **cmd)
{
	int	bl;

	bl = 0;
	*index = ft_strlen(*cmd);
	if (check_apply_esp(*cmd) && (bl = 1))
	{
		*index -= (*index == 0) ? 0 : 1;
		(*cmd)[ft_strlen(*cmd) - 1] = '\0';
	}
	return (bl);
}

void			completing_line_(char **maps, char **cmd, t_select *slc,
	int bl)
{
	int		i;
	int		index;
	int		len;
	int		rtn;

	rtn = 0;
	len = MAX_MAPS;
	i = get_last_flag(*maps);
	while (i >= 0 && !g_pos.exit && rtn != -1)
	{
		if (i >= (len - 1) && (len = increase_maps(maps)) == -1)
			break ;
		if ((*maps)[i] == 'Q' || (*maps)[i] == 'q' || (*maps)[i] == 'S')
		{
			bl = incorrect_map(&index, cmd);
			if ((*maps)[i] == 'S')
				rtn = read_subsh(cmd, slc, bl);
			else
				rtn = read_quote(cmd, ((*maps)[i] == 'Q') ? 34 : 39, slc, bl);
			fill_maps(&(*cmd)[index], maps, i + 1, len);
			i = get_last_flag(*maps);
			continue ;
		}
		i--;
	}
}
