/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfattah <hfattah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 14:10:33 by hfattah           #+#    #+#             */
/*   Updated: 2024/12/17 10:33:15 by hfattah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	handle_export(char *argv, t_prompt *prompt)
{
	int		i;
	char	**export;

	export = prompt->export;
	i = -1;
	if (ft_strchars_i("0123456789", argv) >= 0)
	{
		printf("minishell: '%s' not a valid identifier\n", argv);
		return ;
	}
	while (export[++i])
	{
		if (!ft_strncmp(argv, export[i], ft_strlen(argv)))
		{
			return ;
		}	
	}
	prompt->export = ft_extend_matrix(prompt->export, argv);
}

int	has_invalid_redirection(char **tokens)
{
	int		i;

	i = 0;
	while (tokens && tokens[i])
	{
		if (tokens[i][0] == '>' || tokens[i][0] == '<')
		{
			if (tokens[i + 1] && (tokens[i + 1][0] == '>'
				|| tokens[i + 1][0] == '<' || tokens[i + 1][0] == '|'))
			{
				if (tokens[i + 1][0] == '|')
					return (1);
				if (tokens[i + 2] && (tokens[i + 2][0] == '>'
					|| tokens[i + 2][0] == '<' || tokens[i + 2][0] == '|'))
					return (1);
			}
		}
		i++;
	}
	return (0);
}

void	handle_found_export(t_prompt *prompt, char *argv)
{
	int		i;
	char	*new_entry;

	i = -1;
	while (prompt->export[++i])
	{
		if (!ft_strncmp(argv, prompt->export[i],
				ft_strchars_i(prompt->export[i], "=")))
		{
			free(prompt->export[i]);
			new_entry = ft_strdup(argv);
			if (!new_entry)
				return ;
			prompt->export[i] = new_entry;
			return ;
		}
	}
}

void	handle_found(t_prompt *prompt, char *argv)
{
	int		i;
	char	*new_entry;

	i = -1;
	while (prompt->envp[++i])
	{
		if (!ft_strncmp(argv, prompt->envp[i],
				ft_strchars_i(prompt->envp[i], "=")))
		{
			free(prompt->envp[i]);
			new_entry = ft_strdup(argv);
			if (!new_entry)
				return ;
			prompt->envp[i] = new_entry;
			break ;
		}
	}
	handle_found_export(prompt, argv);
	return ;
}
