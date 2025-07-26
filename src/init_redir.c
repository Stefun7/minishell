/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scesar <scesar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 23:15:09 by scesar            #+#    #+#             */
/*   Updated: 2025/07/26 23:16:03 by scesar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	init_redir(t_instructions *instr, t_commands *cmd, size_t *in_index,
	size_t *out_index)
{
	int	in_count;
	int	out_count;

	in_count = count_redir(&cmd, REDIR_IN);
	out_count = count_redir(&cmd, REDIR_OUT);
	(*in_index) = 0;
	(*out_index) = 0;
	instr->in_redir = malloc(sizeof(t_redir) * (in_count + 1));
	if (!instr->in_redir)
		return (0);
	instr->out_redir = malloc(sizeof(t_redir) * (out_count + 1));
	if (!instr->out_redir)
		return (0);
	instr->nb_files_in = in_count;
	instr->nb_files_out = out_count;
	instr->in_redir[in_count].file_name = NULL;
	instr->out_redir[out_count].file_name = NULL;
	return (1);
}

t_redir	*add_redir(t_redir *redir_list, t_commands *cmd, size_t index,
		size_t *io_index)
{
	t_redir	*added;

	redir_list[*io_index].file_name = ft_strdup(cmd->args[index + 1]->content);
	if (!redir_list[*io_index].file_name)
		return (NULL);
	redir_list[*io_index].type = cmd->args[index]->type;
	added = &redir_list[*io_index];
	(*io_index)++;
	return (added);
}

bool	is_redir_in(t_token **token, size_t index)
{
	if (!token[index + 1])
		return (false);
	if (token[index]->type == REDIR_IN || token[index]->type == HEREDOC)
		return (true);
	else
		return (false);
}

bool	is_redir_out(t_token **token, size_t index)
{
	if (!token[index + 1])
		return (false);
	if (token[index]->type == REDIR_OUT || token[index]->type == APPEND)
		return (true);
	else
		return (false);
}

int	set_redir(t_instructions *instr, t_commands *cmd)
{
	size_t	index;
	size_t	in_index;
	size_t	out_index;

	index = 0;
	if (!init_redir(instr, cmd, &in_index, &out_index))
		return (0);
	while (cmd->args[index])
	{
		if (is_redir_in(cmd->args, index))
		{
			if (!add_redir(instr->in_redir, cmd, index, &in_index))
				return (0);
			index += 2;
		}
		else if (is_redir_out(cmd->args, index))
		{
			if (!add_redir(instr->out_redir, cmd, index, &out_index))
				return (0);
			index += 2;
		}
		else
			index ++;
	}
	return (1);
}
