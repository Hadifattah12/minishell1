/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfattah <hfattah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 10:22:06 by hfattah           #+#    #+#             */
/*   Updated: 2024/12/18 10:24:33 by hfattah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*get_here_str(char *str[2], size_t len, char *limit, t_prompt *prompt)
{
	char	*temp;
	char	*expanded;
	int		quotes[2];

	while (g_status != 130 && (!str[0] || ft_strncmp(str[0], limit, len) \
		|| ft_strlen(limit) != len))
	{
		temp = str[1];
		str[1] = ft_strjoin(str[1], str[0]);
		free(temp);
		free(str[0]);
		str[0] = readline("heredoc> ");
		if (!str[0])
		{
			printf("minishell: warning: here-document delimited"
				" by end-of-file (wanted `%s\')\n", limit);
			break ;
		}
		if (!prompt->flag)
		{
			expanded = expand_vars(str[0], -1, quotes, prompt);
			str[0] = expanded;
		}
		temp = str[0];
		str[0] = ft_strjoin(str[0], "\n");
		free(temp);
		len = ft_strlen(str[0]) - 1;
	}
	free(str[0]);
	return (str[1]);
}


int	get_here_doc(char *str[2], char *aux[2], t_prompt *prompt)
{
	int		fd[2];

	g_status = 0;
	if (pipe(fd) == -1)
	{
		mini_perror(PIPERR, NULL, 1);
		return (-1);
	}
	str[1] = get_here_str(str, 0, aux[0], prompt);
	write(fd[WRITE_END], str[1], ft_strlen(str[1]));
	free(str[1]);
	close(fd[WRITE_END]);
	if (g_status == 130)
	{
		close(fd[READ_END]);
		return (-1);
	}
	return (fd[READ_END]);
}
