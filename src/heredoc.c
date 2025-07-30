/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scesar <scesar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 18:36:51 by scesar            #+#    #+#             */
/*   Updated: 2025/07/30 18:37:26 by scesar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	do_ins(t_minishell *minish, t_instructions *instr)
{
	int	index;
	int	fd;

	index = 0;
	while (index < instr->nb_files_in)
	{
		treat_redir_in(minish, &instr->in_redir[index], &fd);
		if (index != instr->nb_files_in - 1)
			close(fd);
		else
			instr->pipe[0] = fd;
		index++;
	}
}

void	treat_heredoc(t_minishell *minish, t_redir *redir, int *fd)
{
	(*fd) = -2;
	if (redir->type == HEREDOC)
	{
		(*fd) = heredoc_handle(redir->file_name);
		if ((*fd) == -1)
			error(minish, "heredoc error", NULL, 130);
	}
}

void	wrap_up(t_minishell *minish, t_instructions *instru)
{
	int	index;
	int	fd;

	index = 0;
	instru->fd_in = -1;
	fd = -2;
	while (index < instru->nb_files_in)
	{
		silence_signals();
		treat_heredoc(minish, &instru->in_redir[index], &fd);
		if (fd != -2 && index != instru->nb_files_in - 1)
			close(fd);
		else
			instru->fd_in = fd;
		index++;
	}
	setup_signals();
}

void	here_wrap(t_minishell *minish)
{
	int	index;

	index = 0;
	while (index < minish->number_of_commands)
	{
		wrap_up(minish, &minish->instru[index]);
		index++;
	}
}
