/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhri <aboukhri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 21:27:03 by hlamhidr          #+#    #+#             */
/*   Updated: 2019/11/22 15:13:27 by aboukhri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef READ_LINE_H
# define READ_LINE_H

# include "libft.h"
# include <unistd.h>
# include <stdio.h>
# include <sys/wait.h>
# include <dirent.h>
# include <sys/stat.h>
# include <string.h>
# include <fcntl.h>
# include <termios.h>
# include <term.h>
# include <stdlib.h>
# include <curses.h>
# include <sys/ioctl.h>
# include "shell.h"

# define MAX_HISTORY 20
# define MAX_LINES 20

/*
** - selection right 		Alt + Right.
** - selection left 		Alt + Left
** - COPY					Alt + C
** - PASTE					Alt + P
** - CUT					Alt + X
** - moving by words		Alt + Q  // Alt + W
** - moving by lines		Alt + UP // Alt + Down
*/

# define ENTER 10
# define DEL 127
# define UP 4283163
# define DO 4348699
# define RI 4414235
# define LE 4479771
# define SEL_RI 1130044187
# define SEL_LE 1146821403
# define COPY 42947
# define PASTE 32975
# define CUT 8948194
# define LE_WOR	37829
# define RI_WOR 9537762
# define HOME 4741915
# define END 4610843
# define ALT_UP 1096489755
# define ALT_DO 1113266971
# define CTRL_D 4
# define CTRL_T 20
# define TAB 9
# define CTRL_U 21
# define CTRL_L 12
# define CTRL_R 18

# define CAST(x) *((int *)x)

typedef struct s_history t_history;

typedef struct		s_cursor
{
	int				index;
	int				p;
	int				x;
	int				y;
	int				num_lines;
	int				num_col;
	int				*end;
	char			*cmd;
	int				exit;
	int				i;
	struct termios	enable;
	struct termios	disable;
}					t_cursor;

typedef	struct		s_select
{
	int				start;
	int				end;
	char			*save;
}					t_select;

typedef	struct s_info
{
	int index;
	char *cmd;
	struct s_info *prev;
	struct s_info *next;
}			t_info;

struct s_history
{
	struct s_info *head;
	struct s_info *tail;
	int len;
	int bg;
};


t_cursor			g_pos;
t_history 			g_history;

int					g_sign;

char				*ft_read_line(t_history *his, t_select *select, int p);
void				ft_free_tab(char **tableau);
void				ft_catch_signal(int signal);
void				ft_see_touch(char *buf, char *s, t_cursor *pos);
int					ft_get_size_windz(void);
int					ft_set_termcap(void);
void				ft_stock_history(char **history, char *line, int his_count);
int					my_outc(int c);
void				ft_selection(char *s, t_cursor *pos, \
char *key, t_select *select);
void				ft_movecur_up_and_right(int up, int size);
int					ft_get_num_of_lines(int num_col, char *s, int p);
void				ft_set_last_position(t_cursor pos, int num_lines);
void				ft_move_cursor_zero(t_cursor pos);
void				ft_copy_paste(char *buf, char **s, \
t_cursor *pos, t_select *select);
void				ft_init(int **d, int size);
void				ft_putstr_term(int num_col, char *s, t_cursor *pos);
void				ft_get_end_of_line_pos(t_cursor *pos, char *s, int num_col);
void				ft_get_new_pos(t_cursor *pos, int len_sa);
void				ft_move_by_word(t_cursor *pos, char *s, char *buf);
void				ft_home_end(t_cursor *pos, char *s, char *buf);
void				ft_move_by_lines(t_cursor *pos, char *s, char *buf);
void				ft_move_left(int n);
void				ft_quotes(char **line, t_select *select, t_history *his);
char				**ft_alloc_tab(void);
void				ft_win_change(int signal);
void				ft_print_with_reverse_mode(char *s, \
int start, int end, t_cursor *pos);
void				ft_left_selection(char *s, t_cursor *pos, t_select *select);
void				ft_remove_selections(t_cursor *pos, char *s);
char				*ft_delcolomn(char *s, t_cursor *pos);
void				ft_initial(int p);
void				ft_print_history(t_history *his, \
char *buf, char **s, t_cursor *pos);
void				ft_print_touch_and_join(t_cursor *pos, char *buf, char **s);
void				ft_move_right(int n);
char				*ft_ctrl_d(t_cursor *pos, \
t_history *his, t_select *select, char *s);
char				*ft_auto_completion(t_cursor *pos, char *s);
void				save_address(t_history **his, t_select **select);
void				ft_clear_readline_struct(void);
char				*ft_read_heredoc(char *eol);
void				ft_cut(t_cursor *pos, t_select *select, char **s);
void				ft_get_save(char *s, t_select *select);
void				ft_init_size_end_line(t_cursor *pos);
char				*ft_inside_line(char *s, t_cursor *pos, char *buf);
void				ft_enable(void);
void				ft_disable(void);
char				*ft_clear(t_cursor *pos, char *s, char *buf);

/*
** read for sub_shell
*/
int					ft_check_subsh(int i, char **line, t_select *select, t_history *his);

/*
**	history readline, '!' expansion and fc builtuin
*/

void    insert_history(t_history *history, char *cmd);
t_info  *history_index(t_info *history, int index, int len);
t_info  *history_keyword(t_info *history, char *keyword, int dir);
void    history_readline(t_history *history, int key, char **cmd);
void init_history(t_history *history);
void    restore_history(t_history *history);
void    save_history(t_history *history);
t_info  *history_value(t_history his, char *keyword);
void    rev_his_list(t_history *lst);
void    display_his_list(t_history his, int order);
int		history_handling(char **str_cmds);
char    *history_content(t_history his);

char		*compliting_line(char *str_cmds, t_select *select, t_history *his);

/*history expansion */
char    *history_expansion(t_history his, char *keyword);
char    is_shell_delimiter(char c);
char *get_delimiter(char *keyword);
char    *str_notnumber(char *keyword);
/* fc buit */
char    *read_fc();
void    write_fc(char *content);
void    fc_flag_l(t_history history, char *flags, char **args);
void    fc_built(char **args, t_history history);
void    fc_flag_l(t_history history, char *flags, char **args);
void    fc_flag_s(t_history his, char *arg);
void    fc_flag_e(t_history his, char **args);
t_info  *fc_value(t_history his, char *keyword);
int    read_fc_flags(char **args, char **fl, char *err);
int		fc_exec_flag(char *str_cmds);
void    fc_usage(char c);
int    fc_edit(t_history his, char *editor, char **args);
void    exec_fc();
#endif
