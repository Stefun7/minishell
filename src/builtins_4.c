/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scesar <scesar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 09:42:08 by scesar            #+#    #+#             */
/*   Updated: 2025/07/24 09:43:06 by scesar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	builtin_exit(char **executables)
{
	int	exit_status;

	if (executables[1] == NULL)
		exit_status = (0);
	else if (executables[2] != NULL)
	{
		write(2, "bash: exit: too many arguments\n", 31);
		exit_status = (2);
	}
	else if (ft_is_number(executables[1]) == 0)
	{
		ft_printf(2, "bash: exit: %s", executables[1]);
		ft_printf(2, ": numeric argument required\n");
		exit_status = 2;
	}
	else
	{
		exit_status = ft_atoi(executables[1]);
		if (exit_status < 0 || exit_status > 255)
			exit_status = exit_status % 256;
	}
	ft_printf(1, "exit %d\n", exit_status);
	exit(exit_status);
}

int	builtin_unset(char **executables, t_env **envp)
{
	int	i;

	i = 1;
	while (executables[i])
	{
		remove_env_var(envp, executables[i]);
		i++;
	}
	return (0);
}
