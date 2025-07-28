/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scesar <scesar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:52:56 by scesar            #+#    #+#             */
/*   Updated: 2025/07/28 11:52:57 by scesar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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
	instr->nb_files_in = in_count;
	instr->nb_files_out = out_count;
	instr->in_redir[in_count].file_name = NULL;
	instr->out_redir[out_count].file_name = NULL;
	return (1);
}

t_redir	*add_redir(t_redir *redir_list, t_token_type type,
				char *file, size_t *io_index)
{
	t_redir	*added;

	redir_list[*io_index].file_name = ft_strdup(file);
	if (!redir_list[*io_index].file_name)
		return (NULL);
	redir_list[*io_index].type = type;
	added = &redir_list[*io_index];
	(*io_index)++;
	return (added);
}

int	prep_set_redir(t_instructions *instr, t_commands *cmd, t_minishell *minish)
{
	size_t	indexes[3];

	indexes[0] = 0;
	indexes[1] = 0;
	indexes[2] = 0;
	if (!init_redir(instr, cmd))
		return (0);
	if (!set_redir(instr, cmd, minish, indexes))
		return (0);
	return (1);
}

int	set_redir(t_instructions *instr, t_commands *cmd,
				t_minishell *minish, size_t i[3])
{
	char	*s;

	while (cmd->args[i[0]])
	{
		if (in_tok(cmd->args[i[0]]->type) && cmd->args[i[0] + 1])
		{
			s = get_new_string(*minish, cmd->args[i[0] + 1]->content);
			if (!add_redir(instr->in_redir, cmd->args[i[0]]->type, s, &i[1]))
				return (free(s), 0);
			free(s);
			i[0] += 2;
		}
		else if (out_tok(cmd->args[i[0]]->type) && cmd->args[i[0] + 1])
		{
			s = get_new_string(*minish, cmd->args[i[0] + 1]->content);
			if (!add_redir(instr->out_redir, cmd->args[i[0]]->type, s, &i[2]))
				return (free(s), 0);
			free(s);
			i[0] += 2;
		}
		else
			i[0]++;
	}
	return (1);
}
