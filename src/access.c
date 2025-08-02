/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   access.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scesar <scesar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 17:11:14 by scesar            #+#    #+#             */
/*   Updated: 2025/08/01 15:15:36 by scesar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	heredoc_handle(char *stop)
{
	int		fd[2];
	pid_t	pid;
	int		status;

	if (pipe(fd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		close(fd[0]);
		heredoc_signals();
		heredoc_child(stop, fd[1]);
		exit(0);
	}
	close(fd[1]);
	waitpid(pid, &status, 0);
	return (fd[0]);
}

void	treat_redir_in(t_minishell *minish, t_redir *redir, int *fd)
{
	if (redir->type == REDIR_IN)
	{
		(*fd) = open(redir->file_name, O_RDONLY);
		if ((*fd) == -1)
			error(minish, "couldn't open file", redir->file_name, 126);
	}
}

void	treat_redir_out(t_minishell *minish, t_redir *redir,
		int parser, int *fd)
{
	if (redir->type == REDIR_OUT)
		(*fd) = open(redir->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == APPEND)
		(*fd) = open(redir->file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if ((*fd) == -1)
		error(minish, "couldn't open file", redir->file_name, 126);
}

void	access_test(t_minishell *minish, t_instructions *instr, int parser)
{
	int	index;
	int	fd;

	if (instr->fd_in == -1)
		do_ins(minish, instr);
	else
		instr->pipe[0] = instr->fd_in;
	index = 0;
	while (index < instr->nb_files_out && instr->out_redir[index].index != -1)
	{
		treat_redir_out(minish, &instr->out_redir[index], parser, &fd);
		if (fd < 0)
			error(minish, "couldn't open file",
				instr->out_redir[index].file_name, 126);
		if (fd > 0 && (index != instr->nb_files_out - 1 || instr->skip))
			close(fd);
		else
			instr->pipe[1] = fd;
		index++;
	}
}

void	no_redirection_proc(t_minishell *minish, t_instructions *instr,
int parser)
{
	if (instr->nb_files_in > 0 && instr->pipe[0] >= 0)
		dup2(instr->pipe[0], STDIN_FILENO);
	else if (parser != 0 && minish->fd_pipes[parser - 1][0] >= 0)
		dup2(minish->fd_pipes[parser - 1][0], STDIN_FILENO);
	if (instr->nb_files_out > 0 && instr->pipe[1] >= 0)
		dup2(instr->pipe[1], STDOUT_FILENO);
	else if (parser != minish->number_of_commands - 1
		&& minish->fd_pipes[parser][1] >= 0)
		dup2(minish->fd_pipes[parser][1], STDOUT_FILENO);
	close_stuff(minish, parser);
	if (instr->nb_files_in > 0 && instr->pipe[0] >= 0)
		close(instr->pipe[0]);
	if (instr->nb_files_out > 0 && instr->pipe[1] >= 0)
		close(instr->pipe[1]);
}
