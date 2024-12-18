/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfattah <hfattah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 10:07:36 by hfattah           #+#    #+#             */
/*   Updated: 2024/12/17 11:14:37 by hfattah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	env_cmp(const char *env_a, const char *env_b)
{
	while (*env_a && *env_b && *env_a == *env_b)
	{
		env_a++;
		env_b++;
	}	
	if (((unsigned char)*env_a) == ((unsigned char)*env_b))
		return (*env_a - *env_b);
	return (((unsigned char)*env_a) - ((unsigned char)*env_b));
}

void	write_export(char **env, int i)
{
	int		j;

	j = 0;
	while (j < i)
	{
		if (ft_strncmp(env[j], "_=", 2) != 0)
		{
			if (ft_strchars_i(env[j], "=") == -1)
			{
				printf("declare -x %s\n", env[j]);
			}
			else
			{
				write(1, "declare -x ", 11);
				write(1, env[j], ft_strchars_i(env[j], "="));
				write(1, "=\"", 2);
				write(1, &env[j][ft_strchars_i(env[j], "=") + 1],
					ft_strlen(env[j]) - ft_strchars_i(env[j], "="));
				write(1, "\"\n", 2);
			}
		}
		j++;
	}
}

void	sort_env(char **env)
{
	int		i;
	int		j;
	char	*temp;
	int		pass;

	i = 0;
	pass = 0;
	while (env[i] != NULL)
		i++;
	while (pass < i - 1)
	{
		j = 0;
		while (j < i - pass - 1)
		{
			if (env_cmp(env[j], env[j + 1]) > 0)
			{
				temp = env[j];
				env[j] = env[j + 1];
				env[j + 1] = temp;
			}
			j++;
		}
		pass++;
	}
	write_export(env, i);
}

int	mini_export(t_prompt *prompt)
{
	int		ij[2];
	int		pos;
	char	**argv;

	argv = ((t_mini *)prompt->cmds->content)->full_cmd;
	if (!argv[1] && (ft_matrixlen(argv) == 1))
		sort_env(prompt->export);
	if (ft_matrixlen(argv) >= 2)
	{
		ij[0] = 0;
		while (argv[++ij[0]])
		{
			pos = var_in_envp(argv[ij[0]], prompt->envp, ij);
			if (pos == 1)
				handle_found(prompt, argv[ij[0]]);
			else if (pos == -1 && ft_strchars_i(argv[ij[0]],
					"~{}-#?%$!+@.") < 0)
				handle_export(argv[ij[0]], prompt);
			else if (ft_strchars_i(argv[ij[0]], "~{}$%?+@.") >= 0 || pos == -2)
				print_error_export(prompt, argv[ij[0]]);
			else if (!pos)
				extend_export(prompt, argv[ij[0]]);
		}
	}
	return (0);
}

void	handle_unset(char *argv, t_prompt *prompt)
{
	int		i;

	i = -1;
	while (prompt->export[++i])
	{
		if (!ft_strncmp(argv, prompt->export[i],
				ft_strchars_i(prompt->export[i], "=")))
		{
			ft_matrix_replace_in(&prompt->export, NULL, i);
			return ;
		}
	}
}
