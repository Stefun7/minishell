/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   family_1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scesar <scesar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 23:33:53 by scesar            #+#    #+#             */
/*   Updated: 2025/07/25 18:12:28 by scesar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	run(t_minishell *minish)
{
	int	i;

	i = 0;
	if (minish->instru[0].exec[0] && built_in_parent(minish->instru[0].exec[0])
		&& minish->number_of_commands == 1)
		minish->last_exit_status = exec_builtin(minish->instru[0].exec, minish);
	else
	{
		while (i < minish->number_of_commands)
		{
			if (pipe(minish->fd_pipes[i]) == -1)
				perror("bablda");
			i++;
		}
		process(minish);
	}
	free_minish_partial(&minish);
	return (0);
}

void	wait_exit(t_minishell *minish, pid_t	last_pid)
{
	int		index;
	pid_t	wait_pid;
	int		status;

	index = 0;
	while (index < minish->number_of_commands)
	{
		wait_pid = wait(&status);
		if (wait_pid == last_pid)
		{
			if (WIFEXITED(status))
				minish->last_exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				minish->last_exit_status = 128 + WTERMSIG(status);
		}
		index++;
	}
}

void	process(t_minishell *minish)
{
	pid_t	forked;
	int		parser;
	pid_t	last_pid;

	last_pid = -1;
	parser = 0;
	signal(SIGINT, SIG_IGN);
	while (parser < minish->number_of_commands)
	{
		forked = fork();
		if (forked == -1)
			perror("couldn't fork");
		else if (forked == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			child_process(minish, &minish->instru[parser], parser);
		}
		else if (parser == minish->number_of_commands - 1)
			last_pid = forked;
		parser++;
	}
	close_parent(minish);
	wait_exit(minish, last_pid);
	signal(SIGINT, sigint_handler);
}

void	path_not_found(char *pcommand, t_minishell *minish)
{
	write(2, "bash: ", 6);
	write(2, pcommand, ft_strlen(pcommand));
	write(2, ": command not found\n", 21);
	close_parent(minish);
	free_minish_total(&minish);
	exit(127);
}

void	child_process(t_minishell *minish, t_instructions *instr, int parser)
{
	access_test(minish, instr, parser);
	no_redirection_proc(minish, instr, parser);
	if (instr->exec[0] != NULL && is_builtin(instr->exec[0]))
		instr->path_command = instr->exec[0];
	else if (instr->exec[0] != NULL)
		instr->path_command = path_finding(instr->exec[0], &minish->envp);
	if (instr->exec[0] != NULL && instr->path_command == NULL)
		path_not_found(instr->exec[0], minish);
	if (instr->exec[0] != NULL && is_builtin(instr->path_command))
		exec_builtin(instr->exec, minish);
	else if (instr->exec[0] != NULL)
		execute(minish, instr, parser);
	close_stuff(minish, parser);
	exit(0);
}
