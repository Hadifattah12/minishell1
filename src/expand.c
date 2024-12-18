/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfattah <hfattah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 10:26:36 by hfattah           #+#    #+#             */
/*   Updated: 2024/12/17 10:20:57 by hfattah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*handle_variable_expansion(char *result, char *str, int i, t_prompt *p)
{
	char	*var_name;
	char	*var_value;
	char	*temp;
	int		j;

	j = i + 1;
	if (str[j] == '$' || str[j] == '?')
		return (handle_special_cases(result, str, j, p));
	while (str[j] && (ft_isalnum(str[j]) || str[j] == '_'))
		j++;
	var_name = ft_substr(str, i + 1, j - (i + 1));
	if (!var_name)
		return (result);
	var_value = mini_getenv(var_name, p->envp, ft_strlen(var_name));
	free(var_name);
	if (!var_value)
		var_value = ft_strdup("");
	if (!var_value)
		return (result);
	temp = result;
	result = ft_strjoin(result, var_value);
	free(temp);
	free(var_value);
	return (result);
}

int	update_index_after_expansion(char *str, int i)
{
	int	j;

	j = i + 1;
	if (str[j] == '$' || str[j] == '?')
		return (j);
	while (str[j] && (ft_isalnum(str[j]) || str[j] == '_'))
		j++;
	return (j - 1);
}

char	*handle_escaped_dollar(char *result, int *i)
{
	char	*temp;

	temp = result;
	result = ft_strjoin_char(result, '$');
	free(temp);
	(*i)++;
	return (result);
}

char	*append_character(char *result, char c)
{
	char	*temp;

	temp = result;
	result = ft_strjoin_char(result, c);
	free(temp);
	return (result);
}

char	*expand_vars(char *str, int i, int quotes[2], t_prompt *prompt)
{
	char	*result;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	quotes[0] = 0;
	quotes[1] = 0;
	while (str && str[++i])
	{
		quotes[0] = (quotes[0] + (!quotes[1] && str[i] == '\'')) % 2;
		quotes[1] = (quotes[1] + (!quotes[0] && str[i] == '\"')) % 2;
		if (!quotes[0] && str[i] == '\\' && str[i + 1] == '$')
			result = handle_escaped_dollar(result, &i);
		else if (!quotes[0] && str[i] == '$' && ft_isdigit(str[i + 1]))
			i++;
		else if (!quotes[0] && str[i] == '$' && str[i + 1])
		{
			result = handle_variable_expansion(result, str, i, prompt);
			i = update_index_after_expansion(str, i);
		}
		else
			result = append_character(result, str[i]);
	}
	free(str);
	return (result);
}
