/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_path.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfattah <hfattah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 11:41:29 by hfattah           #+#    #+#             */
/*   Updated: 2024/12/17 10:21:09 by hfattah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*ft_strjoin_char(char *str, char c)
{
	size_t	len;
	char	*result;

	len = ft_strlen(str);
	result = malloc(len + 2);
	if (!result)
		return (NULL);
	ft_strlcpy(result, str, len + 1);
	result[len] = c;
	result[len + 1] = '\0';
	return (result);
}

char	*handle_special_cases(char *result, char *str, int i, t_prompt *prompt)
{
	char	*var;
	char	*temp;

	if (str[i] == '$')
		var = ft_itoa(prompt->pid);
	else if (str[i] == '?')
		var = ft_itoa(g_status);
	else
		var = ft_strdup("");
	if (!var)
		return (result);
	temp = result;
	result = ft_strjoin(result, var);
	free(temp);
	free(var);
	return (result);
}

static int	validate_rules(char *str, int i, int quotes[2])
{
	if ((!quotes[0] && !quotes[1] && str[i] == '~')
		&& (i == 0 || (str[i - 1] == ' '))
		&& (str[i + 1] == '\0' || str[i + 1] == ' ' || str[i + 1] == '/'))
		return (1);
	return (0);
}

char	*expand_path(char *str, int i, int quotes[2], char *var)
{
	char	*path;
	char	*aux;

	quotes[0] = 0;
	quotes[1] = 0;
	while (str && str[++i])
	{
		quotes[0] = (quotes[0] + (!quotes[1] && str[i] == '\'')) % 2;
		quotes[1] = (quotes[1] + (!quotes[0] && str[i] == '\"')) % 2;
		if (validate_rules(str, i, quotes))
		{
			aux = ft_substr(str, 0, i);
			path = ft_strjoin(aux, var);
			free(aux);
			aux = ft_substr(str, i + 1, ft_strlen(str));
			free(str);
			str = ft_strjoin(path, aux);
			free(aux);
			free(path);
			return (expand_path(str, i + ft_strlen(var) - 1, quotes, var));
		}
	}
	free(var);
	return (str);
}

int	mini_pwd(void)
{
	char	*buf;

	buf = getcwd(NULL, 0);
	ft_putendl_fd(buf, 1);
	free(buf);
	return (0);
}
