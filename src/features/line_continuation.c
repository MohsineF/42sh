/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_complition.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: onouaman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/07 21:45:50 by onouaman          #+#    #+#             */
/*   Updated: 2019/10/12 17:54:10 by mfetoui          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "read_line.h"

int				increase_maps(char	**maps)
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

static int		correct_maps(char *maps)
{
	int i;
	int quoted[2];
	int rtn;
	int temp;

	i = -1;
	rtn = 0;
	ft_bzero(quoted, sizeof(int) * 2);
	if (!maps || !(*maps))
		return (0);
	while (maps[++i] > 0)
	{
		temp = 0;
		if (MATCH_CLOSED(maps[i], maps[i + 1]) && ++rtn)
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
		while (temp-- > 0 && ++rtn)
			maps[++i] = -1;
	}
	return (rtn);
}

static void		fill_maps(char *str_cmd, char **maps, int j, int len_map)
{
	int i;
	int quoted;

	if (!str_cmd || !maps || !*maps)
		return ;
	i = 0;
	quoted = 0;
	while (str_cmd[i])
	{
		if (j >= (len_map - 1) && (len_map = increase_maps(maps)) == -1)
			break ;
		if (str_cmd[i] == '\\' && (str_cmd[i + 1] != '\'' || quoted == 0 || !j))/*|| (j && maps[j - 1] != 'q')))*/
			i += (str_cmd[i + 1]) ? 1 : 0;
		else if (str_cmd[i] == '"')
			(*maps)[j++] = 'Q';
		else if (str_cmd[i] == '\'')
		{
			quoted = (quoted) ? 0 : 1;
			(*maps)[j++] = 'q';
		}
		else if (M_SUBSH(str_cmd[i]) && str_cmd[i + 1] == '(' && ++i)
			(*maps)[j++] = 'S';
		else if (str_cmd[i] == ')')
			(*maps)[j++] = 's';
		i += (str_cmd[i] != '\0');
	}
	while (correct_maps(*maps))
		clean_maps(*maps);
}

static void		ft_read_subsh(char **line, t_select *select, t_history *his)
{
	if (!line || !(*line))
		return ;
	*line = ft_strjoir(*line, "\n", 1);
	ft_putstr("sub> ");
	ft_read_line(his, select, 5);
	if (g_pos.cmd)
	{
		*line = ft_strjoir(*line, g_pos.cmd, 3);
		g_pos.cmd = NULL;
	}
}

static void		ft_read_quote(char **line, int quote,
	t_select *select, t_history *his)
{
	if (!line || !(*line))
		return ;
	*line = ft_strjoir(*line, "\n", 1);
	if (quote == '\'')
	{
		ft_putstr("quote> ");
		ft_read_line(his, select, 7);
	}
	else if (quote == '"')
	{
		ft_putstr("dquotes>> ");
		ft_read_line(his, select, 10);
	}
	if (!g_pos.exit && g_pos.cmd)
	{
		*line = ft_strjoir(*line, g_pos.cmd, 3);
		g_pos.cmd = NULL;
	}
}


char			*completing_line(char *str_cmds, t_select *select, t_history *his)
{
	char	*maps;
	int		i;
	int		index;
	char	*cmd;
	int		len;

	if (!(maps = ft_strnew(MAX_MAPS)))
		return (str_cmds);
	cmd = ft_strdup(str_cmds);
	ft_strdel(&g_pos.cmd);
	fill_maps(cmd, &maps, 0, MAX_MAPS);
	i = get_last_flag(maps);
	len = MAX_MAPS;
	while (i >= 0 && !g_pos.exit)
	{
		if (i >= (len - 1) && (len = increase_maps(&maps)) == -1)
			break ;
		if (maps[i] == 'Q' || maps[i] == 'q' || maps[i] == 'S')
		{
			index = ft_strlen(cmd);
			if (maps[i] == 'S')
				ft_read_subsh(&cmd, select, his);
			else
				ft_read_quote(&cmd, (maps[i] == 'Q') ? '"' : '\'', select, his);
			fill_maps(&cmd[index], &maps, i + 1, len);
			i = get_last_flag(maps);
			continue ;
		}
		i--;
	}
	free(maps);
	return (cmd);
}
