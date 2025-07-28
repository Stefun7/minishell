/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loc_var_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scesar <scesar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 23:14:18 by scesar            #+#    #+#             */
/*   Updated: 2025/07/28 16:30:03 by scesar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	var_already_there(t_env **minish_envp, t_env **minish_local_var,
					char *next_var)
{
	t_env	*travel_var;

	travel_var = *minish_envp;
	while (travel_var)
	{
		if (ft_strncmp((travel_var)->var, next_var,
				ft_strlen(travel_var->var) == 0))
			return (2);
		travel_var = travel_var->next;
	}
	if (minish_local_var != NULL && *minish_local_var != NULL)
	{
		travel_var = *minish_local_var;
		while (travel_var)
		{
			if (ft_strncmp((travel_var)->var, next_var,
					ft_strlen(travel_var->var) == 0))
				return (1);
			travel_var = travel_var->next;
		}
	}
	return (0);
}

bool	valid_var_add(char *input, char **equal)	//check if enough protection
{
	size_t	i;

	if (!input || input[0] == '=' || ft_isdigit(input[0]))
		return (false);
	*equal = ft_strchr(input, '=');
	if (!*equal)
		return (false);
	i = 0;
	while (input[i] && input[i] != '=')
	{
		if (!ft_isalnum(input[i]) && input[i] != '_')
			return (false);
		i++;
	}
	return (true);
}
