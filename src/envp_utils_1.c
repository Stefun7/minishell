/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_utils_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scesar <scesar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 10:22:47 by scesar            #+#    #+#             */
/*   Updated: 2025/07/24 15:09:40 by scesar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_env	*find_first(t_env *envp)

{
	t_env	*min;

	if (!envp)
		return (NULL);
	min = envp;
	envp = envp->next;
	while (envp)
	{
		if (ft_strcmp(envp->var, min->var) < 0)
			min = envp;
		else if (ft_strcmp(envp->var, min->var) == 0)
		{
			if (ft_strcmp(envp->value, min->value) < 0)
				min = envp;
		}
		envp = envp->next;
	}
	return (min);
}

int	is_between_env(t_env *envp, t_env *smallest, t_env *bigger)
{
	int	cmp_small;
	int	cmp_big;

	if (!envp || !smallest || !bigger)
		return (-1);
	cmp_small = ft_strcmp(envp->var, smallest->var);
	cmp_big = ft_strcmp(envp->var, bigger->var);
	if (cmp_small > 0 && cmp_big < 0)
		return (0);
	if (cmp_small == 0)
	{
		if (ft_strcmp(envp->value, smallest->value) > 0)
			return (0);
	}
	if (cmp_big == 0)
	{
		if (ft_strcmp(envp->value, bigger->value) < 0)
			return (0);
	}
	return (-1);
}

int	add_env_back(t_env **env_list, char *var, char *value)
{
	t_env	*new;
	t_env	*temp;

	if (!env_list || !var || !value)
		return (-1);
	new = create_env_node(var, value);
	if (!new)
		return (-1);
	if (*env_list == NULL)
	{
		*env_list = new;
		return (0);
	}
	temp = *env_list;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
	return (0);
}

char	**env_list_to_array(t_env *env, int len)
{
	char	**env_array;
	int		i;

	if (len <= 0)
		return (NULL);
	env_array = malloc(sizeof(char *) * (len + 1));
	if (!env_array)
		return (NULL);
	i = 0;
	while (env && i < len)
	{
		env_array[i] = join_var_value(env->var, env->value);
		if (!env_array[i])
		{
			free_tab(env_array);
			return (NULL);
		}
		env = env->next;
		i++;
	}
	env_array[i] = NULL;
	return (env_array);
}

char	*join_var_value(char *var, char *value)
{
	char	*result;
	size_t	len;

	len = ft_strlen(var) + 1 + ft_strlen(value) + 1;
	result = malloc(sizeof(char) * len);
	if (!result)
		return (NULL);
	ft_strlcpy(result, var, ft_strlen(var)+1);
	ft_strlcat(result, "=", ft_strlen(var) + 2);
	ft_strlcat(result, value, len);
	return (result);
}
