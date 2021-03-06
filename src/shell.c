/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhri <aboukhri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/03 05:54:16 by onouaman          #+#    #+#             */
/*   Updated: 2019/12/19 21:12:59 by aboukhri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "read_line.h"

/*
** loop on cmds parsed by ";" , handle expansions
*/

static void	call_cmds(t_cmds *st_cmds, int bl_subsh)
{
	while (st_cmds)
	{
		handle_expansions(&st_cmds->st_tokens);
		ft_cmds_setup(st_cmds, bl_subsh);
		st_cmds = st_cmds->next;
	}
}

/*
** - Lexer - Check Syntax - Parse with ; , handling Alias and Expansion
*/

void		ft_multi_cmd(char *str_cmds, int bl_subsh)
{
	t_tokens	*st_tokens;
	t_cmds		*st_cmds;
	char		**args;

	if (!str_cmds)
		return ;
	st_cmds = NULL;
	args = ft_str_split_q(str_cmds, " \t\n");
	st_tokens = ft_lexer(args);
	handle_alias(&st_tokens);
	if (!error_syntax_lexer(st_tokens))
	{
		if (st_tokens)
			st_cmds = parse_semicolon(st_tokens);
		call_cmds(st_cmds, bl_subsh);
	}
	else
		g_exit_status = 258;
	free_list_cmds(st_cmds);
	free_tokens(st_tokens, (g_exit_status == 258) ? 1 : 0);
	ft_strrdel(args);
}

static void	initial_shell(t_select **select)
{
	char buff[1024];

	ft_bzero(buff, 1024);
	g_shellpid = getpid();
	while (1)
	{
		if (g_shellpid == tcgetpgrp(0))
			break ;
		kill(g_shellpid, SIGTTIN);
	}
	g_pwd = ft_strdup(getcwd(buff, 1024));
	g_pos.cmd = NULL;
	g_exit_status = 0;
	call_signal();
	initial_read_line(&g_history, select);
	init_fc_built();
	init_alias_hash();
}

char		*handle_backslash(char *str, t_select *select)
{
	char *tmp;

	tmp = NULL;
	if (!str)
		return (NULL);
	while (check_apply_esp(str) == 1)
	{
		tmp = str;
		str = ft_strdup(str);
		ft_strdel(&tmp);
		g_pos.cmd = NULL;
		ft_putstr("> ");
		ft_read_line(&g_history, select, 2);
		if (g_pos.cmd[0] == EXIT_CLD || g_pos.exit)
		{
			ft_strdel(&g_pos.cmd);
			return (str);
		}
		str[ft_strlen(str) - 1] = '\0';
		str = ft_strjoir(str, g_pos.cmd, 3);
		g_pos.cmd = NULL;
		str = completing_line(str, select);
	}
	return (str);
}

int			main(void)
{
	extern char	**environ;
	t_select	*select;

	set_export_env(environ);
	initial_shell(&select);
	if (ft_set_termcap() == -1)
		ft_err_exit("ERROR in setting Termcap parameters\n");
	while (1337)
	{
		ft_putstr("\033[0;32m42sh $>\033[0m ");
		if ((ft_read_line(&g_history, select, PR_S)) == NULL || !g_pos.cmd[0])
		{
			ft_job_processing();
			continue ;
		}
		g_pos.cmd = completing_line(g_pos.cmd, select);
		g_pos.cmd = handle_backslash(g_pos.cmd, select);
		if (!history_handling(&g_pos.cmd))
			continue ;
		(!g_pos.exit) ? ft_multi_cmd(g_pos.cmd, 0) : NULL;
		ft_job_processing();
		ft_strdel(&g_pos.cmd);
		g_proc_sub = 0;
	}
	return (0);
}
