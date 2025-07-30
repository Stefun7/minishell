/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokeniser_help.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scesar <scesar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 18:42:44 by scesar            #+#    #+#             */
/*   Updated: 2025/07/30 18:42:45 by scesar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

size_t	tab_size(char **tab)
{
	size_t	index;

	index = 0;
	while (tab[index])
		index++;
	return (index);
}

t_commands	*new_command_node(void)
{
	t_commands	*cmd;

	cmd = malloc(sizeof(t_commands));
	if (!cmd)
		return (NULL);
	cmd->as_str = NULL;
	cmd->args = NULL;
	cmd->next_command = NULL;
	return (cmd);
}

t_token_type	get_token_type_from_context(t_token *prev)
{
	if (!prev)
		return (CMD);
	if (prev->type == REDIR_IN || prev->type == REDIR_OUT
		|| prev->type == HEREDOC || prev->type == APPEND)
		return (FILENAME);
	else if (prev->type == CMD)
		return (ARG);
	return (CMD);
}

t_commands	*create_command_list(t_commands whole)
{
	t_commands	*first;
	t_commands	*current;
	t_commands	*temp;
	size_t		cmd_index;

	cmd_index = 0;
	first = new_command_node();
	if (!first)
		return (NULL);
	if (!fill_str(whole, first, cmd_index))
		return (free_commands(first), NULL);
	linker(whole, first, &cmd_index);
	current = first;
	while (whole.args[cmd_index])
	{
		temp = new_command_node();
		if (!temp)
			return (free_commands(first), NULL);
		if (!fill_str(whole, temp, cmd_index))
			return (free(temp), free_commands(first), NULL);
		linker(whole, temp, &cmd_index);
		current->next_command = temp;
		current = temp;
	}
	return (current->next_command = NULL, first);
}

int	end_spaces(char *input, size_t *index)
{
	while (input[*index] == ' ' || input[*index] == '	')
		(*index)++;
	if (!input[*index])
		return (0);
	return (1);
}
