/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scesar <scesar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 09:15:52 by scesar            #+#    #+#             */
/*   Updated: 2025/07/24 09:40:48 by scesar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_env	*find_nth(t_env *smallest, t_env *envp)
{
	t_env	*bigger;

	bigger = NULL;
	while (envp)
	{
		if (bigger == NULL && ft_strcmp(envp->var, smallest->var) > 0)
			bigger = envp;
		if (is_between_env(envp, smallest, bigger) == 0)
			bigger = envp;
		envp = envp->next;
	}
	return (bigger);
}

void	print_declare(t_env *envp)
{
	t_env	*smallest;

	smallest = find_first(envp);
	if (smallest == NULL)
		return ;
	while (smallest != NULL)
	{
		printf("declare -x %s=%s\n", smallest->var, smallest->value);
		smallest = find_nth(smallest, envp);
	}
}

int	edit_env(char *content, t_minishell *minish)
{
	char	*equal_pos;
	char	*var;
	char	*value;

	equal_pos = ft_strchr(content, '=');
	if (equal_pos != NULL)
	{
		var = ft_substr(content, 0, equal_pos - content);
		if (var == NULL)
			return (-1);
		value = ft_substr(equal_pos + 1, 0, ft_strlen(equal_pos + 1));
		if (value == NULL)
			return (free(value), -1);
		remove_env_var(&minish->envp, var);
		if (get_var(&minish->local_var, NULL, var) != NULL)
			update_env_value(minish->envp, var, value);
		add_env_back(&minish->envp, var, value);
		free(var);
		free(value);
	}
	return (0);
}

int	is_valid_identifier(const char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (0);
	if (!(ft_isalpha(str[i]) || str[i] == '_'))
		return (0);
	i++;
	while (str[i] && str[i] != '=')
	{
		if (!(ft_isalnum(str[i]) || str[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

int	builtin_export(char **executables, t_minishell *minish)
{
	int	index;

	index = 1;
	if (executables[1] == NULL)
		print_declare(minish->envp);
	else
	{
		while (executables[index])
		{
			if (!is_valid_identifier(executables[index]))
			{
				write(2, "bash: export: `", 15);
				write(2, executables[index], ft_strlen(executables[index]));
				write(2, "': not a valid identifier\n", 27);
			}
			else if (ft_strchr(executables[index], '=') != NULL)
				edit_env(executables[index], minish);
			else if (get_var(&minish->envp, NULL, executables[index]) == NULL)
				add_env_back(&minish->envp, executables[index], "");
			index++;
		}
	}
	return (0);
}
