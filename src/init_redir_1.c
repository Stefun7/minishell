/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_redir_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scesar <scesar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 18:38:01 by scesar            #+#    #+#             */
/*   Updated: 2025/08/01 15:19:34 by scesar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	process_redir(t_redir *redir_list,
					t_token_type type, char *filename, size_t *redir_index)
{
	if (!filename)
		return (0);
	if (in_tok(type))
	{
		if (!add_redir_in(redir_list, type, filename, redir_index))
			return (0);
	}
	else if (out_tok(type))
	{
		if (!add_redir_out(redir_list, type, filename, redir_index))
			return (0);
	}
	return (1);
}

int	init_redir(t_instructions *instr, t_commands *cmd)
{
	int	in_count;
	int	out_count;

	in_count = count_redir(&cmd, REDIR_IN);
	out_count = count_redir(&cmd, REDIR_OUT);
	instr->in_redir = malloc(sizeof(t_redir) * (in_count + 1));
	if (!instr->in_redir)
		return (0);
	instr->out_redir = malloc(sizeof(t_redir) * (out_count + 1));
	if (!instr->out_redir)
		return (0);
	ft_memset(instr->in_redir, 0, sizeof(t_redir) * (in_count + 1));
	ft_memset(instr->out_redir, 0, sizeof(t_redir) * (out_count + 1));
	instr->nb_files_in = in_count;
	instr->nb_files_out = out_count;
	instr->in_redir[in_count].file_name = NULL;
	instr->out_redir[out_count].file_name = NULL;
	return (1);
}

int	prep_set_redir(t_instructions *instr, t_commands *cmd, t_minishell *minish)
{
	size_t	indexes[3];
	int		result;

	indexes[0] = 0;
	indexes[1] = 0;
	indexes[2] = 0;
	if (!init_redir(instr, cmd))
		return (0);
	instr->skip = false;
	result = (set_redir(instr, cmd, minish, indexes));
	if (result == 2)
		return (instr->skip = true, minish->last_exit_status = 1, 2);
	if (result == 3)
		return (instr->skip = true, minish->last_exit_status = 1, 2);
	return (1);
}

int	set_redir(t_instructions *instr, t_commands *cmd,
				t_minishell *minish, size_t i[3])
{
	int		res;
	char	*s;

	res = 1;
	while (cmd->args[i[0]])
	{
		if ((in_tok(cmd->args[i[0]]->type) || out_tok(cmd->args[i[0]]->type))
			&& (cmd->args[i[0] + 1]))
		{
			s = get_new_string(*minish, cmd->args[i[0] + 1]->content);
			if (in_tok(cmd->args[i[0]]->type))
				res = (process_redir(instr->in_redir,
							cmd->args[i[0]]->type, s, i));
			if (out_tok(cmd->args[i[0]]->type))
				res = (process_redir(instr->out_redir,
							cmd->args[i[0]]->type, s, i));
			if (res != 1)
				return (free(s), res);
			free(s);
			i[0]++;
		}
		i[0]++;
	}
	return (res);
}
