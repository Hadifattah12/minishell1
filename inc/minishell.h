/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfattah <hfattah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:34:47 by hfattah           #+#    #+#             */
/*   Updated: 2024/12/17 11:14:52 by hfattah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

# define MINISHELL_H

# include "../libft/inc/libft.h"
# include "colors.h"
# include "get_next_line.h"
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/wait.h>
# include <dirent.h>
# include <sys/ioctl.h>
# include <limits.h>
# define READ_END 0
# define WRITE_END 1

typedef struct s_prompt
{
	t_list	*cmds;
	char	**envp;
	pid_t	pid;
	char	**export;
	int		flag;
}			t_prompt;

typedef struct s_mini
{
	char	**full_cmd;
	char	*full_path;
	int		infile;
	int		outfile;
}			t_mini;

enum	e_mini_error
{
	QUOTE = 1,
	NDIR = 2,
	NPERM = 3,
	NCMD = 6,
	DUPERR = 7,
	FORKERR = 8,
	PIPERR = 9,
	PIPENDERR = 10,
	MEM = 11,
	IS_DIR = 12,
	NOT_DIR = 13
};

extern int	g_status;

void	handle_unset(char *argv, t_prompt *prompt);

void	sort_env(char **env);

int		env_cmp(const char *env_a, const char *env_b);

void	write_export(char **env, int i);

char	*ft_strjoin_char(char *str, char c);

char	*handle_special_cases(char *result, char *str, int i, t_prompt *prompt);

void	handle_signal_execute(t_prompt *prompt, t_list *cmd);

int		var_in_envp(char *argv, char **envp, int ij[2]);

void	exec_fork(t_prompt *prompt, t_list *cmd, int fd[2]);

int		builtin(t_prompt *prompt, t_list *cmd, int *is_exit, int n);

int		is_builtin(t_mini *n);

char	**get_trimmed(char **args);

void	print_error_export(t_prompt *prompt, char *argv);

void	extend_export(t_prompt *prompt, char *argv);

int		var_in_envp_unset(char *argv, char **envp, int ij[2]);

int		mini_cd(t_prompt *prompt);

void	cd_error(char **str[2]);

int		mini_pwd(void);

int		mini_echo(t_list *cmd);

int		ft_atoi2(const char *nptr, unsigned long long *nbr);

int		mini_export(t_prompt *prompt);

int		mini_unset(t_prompt *prompt);

int		mini_exit(t_list *cmd, int *is_exit);

void	*check_args(char *out, t_prompt *p);

char	**ft_cmdtrim(char const *s, char *set, t_prompt *p);

char	**ft_cmdsubsplit(char const *s, char *set);

char	*ft_strtrim_all(char const *s1, int squote, int dquote);

t_list	*fill_nodes(char **args, int i, t_prompt *prompt);

int		get_fd(int oldfd, char *path, int flags[2]);

t_mini	*get_outfile1(t_mini *node, char **args, int *i);

t_mini	*get_outfile2(t_mini *node, char **args, int *i);

t_mini	*get_infile1(t_mini *node, char **args, int *i);

t_mini	*get_infile2(t_mini *node, char **args, int *i, t_prompt *prompt);

void	*exec_cmd(t_prompt *prompt, t_list *cmd);

void	*check_to_fork(t_prompt *prompt, t_list *cmd, int fd[2]);

void	child_builtin(t_prompt *prompt, t_mini *n, int l, t_list *cmd);

void	exec_custom(char ***out, char *full, char *args, char **envp);

void	get_cmd(t_prompt *prompt, t_list *start, char **split_path, char *path);

char	*expand_vars(char *str, int i, int quotes[2], t_prompt *prompt);

char	*expand_path(char *str, int i, int quotes[2], char *var);

int		get_here_doc(char *str[2], char *aux[2], t_prompt *prompt);

void	*mini_perror(int err_type, char *param, int err);

char	*mini_getenv(char	*var, char **envp, int n);

char	**mini_setenv(char *var, char *value, char **envp, int n);

char	*mini_getprompt(t_prompt prompt);

void	free_content(void *content);

void	handle_sigint(int sig);

void	handle_export(char *argv, t_prompt *prompt);

char	*get_substr_var(char *str, int i, t_prompt *prompt);

t_mini	*get_params(t_mini *node, char **a[2], int *i, t_prompt *prompt);

int		has_invalid_redirection(char **tokens);

void	handle_found_export(t_prompt *prompt, char *argv);

void	handle_found(t_prompt *prompt, char *argv);

#endif
