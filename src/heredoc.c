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
	while (index < instru->nb_files_in)
	{
		silence_signals();
		treat_heredoc(minish, &instru->in_redir[index], &fd);
		if (fd == -2)
		{
			index++;
			continue ;
		}
		if (index == instru->nb_files_in - 1)
			instru->fd_in = fd;
		else
			close(fd);
		index++;
	}
	instru->fd_in = -1;
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
	ft_printf(2, "minish->number_of_commands :%d\n",
		minish->number_of_commands);
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

int	check_redir_sequence(t_instructions instr, t_minishell *minish)
{
	int	in_i;
	int	out_i;

	in_i = 0;
	out_i = 0;
	int loopger = 0;;
	while (in_i < instr.nb_files_in || out_i < instr.nb_files_out)
	{
		if (in_i < instr.nb_files_in &&
			(out_i >= instr.nb_files_out ||
			 instr.in_redir[in_i].index < instr.out_redir[out_i].index))
		{
			if (instr.in_redir[in_i].type == HEREDOC)
				in_i++;
			else if (!check_perm(instr.in_redir[in_i++], *minish))
				return (instr.skip = true, 0);
		}
		else
			if (!check_perm(instr.out_redir[out_i++], *minish))
				return (instr.skip = true, 0);
		if (++loopger == 15)
		{
			write(2, "WHAT\n", 5);
			ft_printf(2, "nb_files_in = %d, nb_files_out = %d\n",
				instr.nb_files_in, instr.nb_files_out);
			ft_printf(2, "Current in_i = %d, out_i = %d\n", in_i, out_i);
			for (int i = 0; i < instr.nb_files_in; i++)
				ft_printf(2, "in[%d]: type=%d index=%d file=%s\n",
					i, instr.in_redir[i].type, instr.in_redir[i].index,
					instr.in_redir[i].file_name);
			for (int i = 0; i < instr.nb_files_out; i++)
				ft_printf(2, "out[%d]: type=%d index=%d file=%s\n",
					i, instr.out_redir[i].type, instr.out_redir[i].index,
					instr.out_redir[i].file_name);
			break;
		}
	}
	return (1);
}
