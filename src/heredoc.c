/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scesar <scesar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 18:36:51 by scesar            #+#    #+#             */
/*   Updated: 2025/08/01 15:17:13 by scesar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	do_ins(t_minishell *minish, t_instructions *instr)
{
	int	index;
	int	fd;

	index = 0;
	while (index < instr->nb_files_in && instr->in_redir[index].index != -1)
	{
		treat_redir_in(minish, &instr->in_redir[index], &fd);
		if (fd < 0)
			error(minish, "couldn't open file",
				instr->in_redir[index].file_name, 126);
		if (index != instr->nb_files_in - 1 || instr->skip)
			close(fd);
		else
			instr->pipe[0] = fd;
		index++;
	}
}

void	treat_heredoc(t_minishell *minish, t_redir *redir, int *fd)
{
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
	fd = -1;
	while (index < instru->nb_files_in)
	{
		silence_signals();
		treat_heredoc(minish, &instru->in_redir[index], &fd);
		if (fd == -2)
		{
			index++;
			continue ;
		}
		if (instru->in_redir[index].type == HEREDOC
			&& index == instru->nb_files_in - 1)
			instru->fd_in = fd;
		else if (instru->in_redir[index].type == HEREDOC)
			close(fd);
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
	check_all_redirs(minish);
}

int	check_all_redirs(t_minishell *minish)
{
	int	i;

	i = 0;
	while (i < minish->number_of_commands)
	{
		if (!check_redir_sequence(minish->instru[i], minish))
			minish->instru[i].skip = true;
		else
			minish->instru[i].skip = false;
		i++;
	}
	return (1);
}
