/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_utils_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scesar <scesar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 15:05:45 by scesar            #+#    #+#             */
/*   Updated: 2025/07/30 18:33:43 by scesar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_env	*get_var(t_env **minish_envp, t_env **minish_local_var, char *VAR)
{
	t_env	*trav;

	if (!VAR)
		return (NULL);
	if (minish_envp != NULL)
	{
		trav = *minish_envp;
		while (trav)
		{
			if (ft_strcmp(trav->var, VAR) == 0)
				return (trav);
			trav = trav->next;
		}
	}
	if (minish_local_var != NULL && *minish_local_var != NULL)
	{
		trav = *minish_local_var;
		while (trav)
		{
			if (ft_strncmp((trav)->var, VAR, ft_strlen(trav->var)) == 0)
				return (trav);
			trav = trav->next;
		}
	}
	return (NULL);
}

int	update_env_value(t_env *env_list, const char *var_name,
	const char *new_value)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->var, var_name) == 0)
		{
			if (current->value)
				free(current->value);
			current->value = ft_strdup(new_value);
			if (current->value == NULL)
				return (-1);
			return (0);
		}
		current = current->next;
	}
	return (1);
}

int	env_list_length(t_env *traveler)
{
	int	count;

	count = 0;
	while (traveler != NULL)
	{
		count++;
		traveler = traveler->next;
	}
	return (count);
}

void	remove_var(t_env **envp_or_loc, char *to_remove)
{
	t_env	*current;
	t_env	*prev;

	current = *envp_or_loc;
	prev = NULL;
	if (!envp_or_loc || !*envp_or_loc)
		return ;
	while (current)
	{
		if (ft_strcmp(current->var, to_remove) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*envp_or_loc = current->next;
			free(current->var);
			free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

char	*get_var_value(t_env **minish_envp, t_env **minish_local_var, char *VAR)
{
	t_env	*var_node;

	var_node = get_var(minish_envp, minish_local_var, VAR);
	if (var_node && var_node->value)
		return (var_node->value);
	return (NULL);
}
