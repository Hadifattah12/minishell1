/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfattah <hfattah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 10:45:29 by hfattah           #+#    #+#             */
/*   Updated: 2024/12/05 10:06:30 by hfattah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	execute_env(t_prompt *prompt, t_mini *n, int l)
{
	int	i;

	i = 0;
	while (n->full_cmd[i])
	{
		if (!ft_strncmp(n->full_cmd[i], "env",
				ft_strlen(n->full_cmd[i])) && l == 3)
			i++;
		else
		{
			g_status = 127;
			printf("env: '%s': no such file or directory\n", n->full_cmd[i]);
			break ;
		}
	}
	if (i == ft_matrixlen(n->full_cmd))
	{
		ft_putmatrix_fd(prompt->envp, 1, 1);
		g_status = 0;
	}
}

void	child_builtin(t_prompt *prompt, t_mini *n, int l, t_list *cmd)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (!is_builtin(n) && n->full_cmd)
		execve(n->full_path, n->full_cmd, prompt->envp);
	else if (n->full_cmd && !ft_strncmp(*n->full_cmd, "pwd", l) && l == 3)
		g_status = mini_pwd();
	else if (is_builtin(n) && n->full_cmd
		&& !ft_strncmp(*n->full_cmd, "echo", l) && l == 4)
		g_status = mini_echo(cmd);
	else if (is_builtin(n) && n->full_cmd
		&& !ft_strncmp(*n->full_cmd, "env", l) && l == 3)
	{
		execute_env(prompt, n, l);
	}
}

static void	*child_redir(t_list *cmd, int fd[2])
{
	t_mini	*node;

	node = cmd->content;
	if (node->infile != STDIN_FILENO)
	{
		if (dup2(node->infile, STDIN_FILENO) == -1)
			return (mini_perror(DUPERR, NULL, 1));
		close(node->infile);
	}
	if (node->outfile != STDOUT_FILENO)
	{
		if (dup2(node->outfile, STDOUT_FILENO) == -1)
			return (mini_perror(DUPERR, NULL, 1));
		close(node->outfile);
	}
	else if (cmd->next && dup2(fd[WRITE_END], STDOUT_FILENO) == -1)
		return (mini_perror(DUPERR, NULL, 1));
	close(fd[WRITE_END]);
	return ("");
}

void	*child_process(t_prompt *prompt, t_list *cmd, int fd[2])
{
	t_mini	*n;
	int		l;

	n = cmd->content;
	l = 0;
	if (n->full_cmd)
		l = ft_strlen(*n->full_cmd);
	child_redir(cmd, fd);
	close(fd[READ_END]);
	child_builtin(prompt, n, l, cmd);
	ft_lstclear(&prompt->cmds, free_content);
	exit(g_status);
}

void	exec_fork(t_prompt *prompt, t_list *cmd, int fd[2])
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		close(fd[READ_END]);
		close(fd[WRITE_END]);
		mini_perror(FORKERR, NULL, 1);
	}
	else if (!pid)
		child_process(prompt, cmd, fd);
}
