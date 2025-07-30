/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scesar <scesar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 09:42:08 by scesar            #+#    #+#             */
/*   Updated: 2025/07/30 18:35:31 by scesar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	builtin_exit(char **executables, t_minishell *minish)
{
	int	exit_status;

	if (is_interactive(minish))
		ft_printf(1, "exit\n");
	if (executables[1] == NULL)
		exit_status = (0);
	else if (ft_is_number(executables[1]) == 0)
	{
		ft_printf(2, "bash: exit: %s", executables[1]);
		ft_printf(2, ": numeric argument required\n");
		exit_status = 2;
	}
	else if (executables[2] != NULL)
	{
		write(2, "bash: exit: too many arguments\n", 31);
		exit_status = (1);
	}
	else
	{
		exit_status = ft_atoi(executables[1]);
		if (exit_status < 0 || exit_status > 255)
			exit_status = exit_status % 256;
	}
	exit(exit_status);
}

int	builtin_unset(char **executables, t_minishell *minish)
{
	size_t	index;
	t_env	*target;

	index = 1;
	while (executables[index])
	{
		remove_var(&minish->envp, executables[index]);
		remove_var(&minish->local_var, executables[index]);
		index++;
	}
	return (0);
}

void	add_to_envp(t_minishell *minish, char *var)
{
	t_env	*last;
	t_env	*in_loc;
	t_env	*new;

	last = minish->envp;
	in_loc = get_var(NULL, &minish->local_var, var);
	if (!in_loc)
		return ;
	new = create_env_node(in_loc->var, in_loc->value);
	if (!new)
		return ;
	if (!minish->envp)
		minish->envp = new;
	else
	{
		while (last->next)
			last = last->next;
		last->next = new;
	}
	remove_var(&minish->local_var, var);
}
