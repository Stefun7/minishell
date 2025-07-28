/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_utils_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scesar <scesar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 15:05:45 by scesar            #+#    #+#             */
/*   Updated: 2025/07/24 15:09:43 by scesar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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

int	remove_env_var(t_env **head, const char *var)
{
	t_env	*current;
	t_env	*prev;

	current = *head;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->var, var) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*head = current->next;
			free(current->var);
			free(current->value);
			free(current);
			return (1);
		}
		prev = current;
		current = current->next;
	}
	return (0);
}

char	*get_var_value(t_env **minish_envp, t_env **minish_local_var, char *VAR)
{
	t_env	*var_node;

	var_node = get_var(minish_envp, minish_local_var, VAR);
	if (var_node && var_node->value)
		return (var_node->value);
	return (NULL);
}
