/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   family_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scesar <scesar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 23:38:06 by scesar            #+#    #+#             */
/*   Updated: 2025/08/02 12:48:53 by scesar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	execute(t_minishell *minish, t_instructions *instr,
			int parse, char **exec)
{
	int		execror;
	char	**valid_envp;
	int		len;

	len = env_list_length(minish->envp);
	valid_envp = env_list_to_array(minish->envp, len);
	if (valid_envp == NULL)
		error(minish, "Failed to convert environment variables to array",
			NULL, 255);
	execror = execve(instr->path_command, exec, valid_envp);
	if (execror == -1)
		error(minish, "execution failed", NULL, 1);
}

void	close_parent(t_minishell *minish)
{
	int	i;

	i = 0;
	while (i < minish->number_of_commands)
	{
		close(minish->fd_pipes[i][0]);
		close(minish->fd_pipes[i][1]);
		i++;
	}
}

void	close_stuff(t_minishell *minish, int parser)
{
	int	i;

	i = 0;
	while (i < minish->number_of_commands)
	{
		if (i != parser - 1)
			close(minish->fd_pipes[i][0]);
		if (i != parser)
			close(minish->fd_pipes[i][1]);
		i++;
	}
}

void	error(t_minishell *minish, char *reason, char *specific, int exit_stat)
{
	if (specific == NULL)
		ft_printf(2, "bash: %s\n", reason);
	else
		ft_printf(2, "bash: %s :%s", specific, reason);
	exit(exit_stat);
}

int	find_non_empty(char **str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
	{
		if (str[i][0] != '\0')
			return (i);
		i++;
	}
	return (0);
}
