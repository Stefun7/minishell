/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scesar <scesar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 20:16:35 by scesar            #+#    #+#             */
/*   Updated: 2025/07/23 20:40:55 by scesar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	update_shlvl(t_env **env)
{
	char	*new_value;
	int		level;

	if (!(*env)->value)
		level = 0;
	else
		level = ft_atoi((*env)->value);
	if (level < 0)
		level = 0;
	else if (level > 1000)
		level = 999;
	level++;
	new_value = ft_itoa(level);
	if (!new_value)
		return (0);
	if ((*env)->value)
		free(((*env))->value);
	(*env)->value = new_value;
	return (1);
}

int	set_next_var(t_env **next_envv, char *envv, char *equal)
{
	if (!envv)
		return (0);
	*next_envv = malloc(sizeof(t_env));
	(*next_envv)->var = NULL;
	(*next_envv)->value = NULL;
	if (!*next_envv)
		return (0);
	(*next_envv)->var = ft_substr(envv, 0, equal - envv);
	if (!(*next_envv)->var)
		return (0);
	(*next_envv)->value = ft_strdup(equal + 1);
	if (ft_strcmp((*next_envv)->var, "shlvl") == 0)
		return (update_shlvl(next_envv));
	if (!(*next_envv)->value)
	{
		free((*next_envv)->var);
		free(*next_envv);
		return (0);
	}
	return (1);
}

// bool	build_env(char **envp, int *i, t_env *minish_env, )

void	init_env_var(size_t *i, t_env *current_envv, t_env *next_envv)
{
	i = 0;
	current_envv = NULL;
	next_envv = NULL;
}

t_env	*set_envp(t_env **minish_env, char **envp)
{
	size_t	i;
	char	*equal_is_there;
	t_env	*current_envv;
	t_env	*next_envv;

	init_env_var(&i, current_envv, next_envv);
	while (envp[i])
	{
		equal_is_there = ft_strchr(envp[i], '=');
		if (!equal_is_there)
			i++;
		if (!equal_is_there)
			continue ;
		if (!set_next_var(&next_envv, envp[i], equal_is_there))
			return (NULL);
		if (*minish_env == NULL)
			*minish_env = next_envv;
		else
			current_envv->next = next_envv;
		current_envv = next_envv;
		i++;
	}
	if (current_envv)
		current_envv->next = NULL;
	return (*minish_env);
}

t_env	*get_var(t_env **minish_envp, t_env **minish_local_var, char *VAR)
{
	t_env	*travel_var;

	if (!VAR)
		return (NULL);
	travel_var = *minish_envp;
	while (travel_var)
	{
		if (ft_strcmp(travel_var->var, VAR) == 0)
			return (travel_var);
		travel_var = travel_var->next;
	}
	if (minish_local_var != NULL && *minish_local_var != NULL)
	{
		travel_var = *minish_local_var;
		while (travel_var)
		{
			if (ft_strncmp((travel_var)->var, VAR,
					ft_strlen(travel_var->var)) == 0)
				return (travel_var);
			travel_var = travel_var->next;
		}
	}
	return (NULL);
}
