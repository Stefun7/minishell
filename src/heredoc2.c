/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scesar <scesar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 18:37:30 by scesar            #+#    #+#             */
/*   Updated: 2025/07/30 18:37:43 by scesar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	heredoc_child(char *stop, int write_fd)
{
	char	*line;

	heredoc_signals();
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, stop) == 0)
		{
			free(line);
			break ;
		}
		write(write_fd, line, ft_strlen(line));
		write(write_fd, "\n", 1);
		free(line);
	}
	close(write_fd);
}

int	check_redir_sequence(t_instructions instr, t_minishell *minish)
{
	int	in_i;
	int	out_i;

	in_i = 0;
	out_i = 0;
	while (in_i < instr.nb_files_in || out_i < instr.nb_files_out)
	{
		if (in_i < instr.nb_files_in && (out_i
				>= instr.nb_files_out || instr.in_redir[in_i].index
				< instr.out_redir[out_i].index))
		{
			if (instr.in_redir[in_i].type == HEREDOC)
				in_i++;
			else if (!check_perm(instr.in_redir[in_i++], minish))
				return (instr.in_redir[in_i - 1].index = -1
					, instr.out_redir[out_i++].index = -1, 0);
		}
		else
			if (!check_perm(instr.out_redir[out_i++], minish))
				return (instr.in_redir[in_i++].index = -1
					, instr.out_redir[out_i - 1].index = -1, 0);
	}
	return (1);
}
