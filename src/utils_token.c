/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scesar <scesar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 18:42:53 by scesar            #+#    #+#             */
/*   Updated: 2025/07/30 18:42:54 by scesar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	special_symb(char *input, size_t index)
{
	if (input[index] == '<' )
	{
		if (input[index + 1] && input[index + 1] == '<' )
			return (HEREDOC);
		else
			return (REDIR_IN);
	}
	else if (input[index] == '>' )
	{
		if (input[index + 1] && input[index + 1] == '>')
			return (APPEND);
		else
			return (REDIR_OUT);
	}
	if (input[index] == '|' )
		return (PIPE);
	else
		return (NONE);
}

int	special_symb_2(char *input)
{
	size_t	len;

	len = ft_strlen(input);
	if (ft_strncmp(input, "<", len) == 0)
		return (REDIR_IN);
	if (ft_strncmp(input, ">", len) == 0)
		return (REDIR_OUT);
	if (ft_strncmp(input, ">>", len) == 0)
		return (APPEND);
	if (ft_strncmp(input, "<<", len) == 0)
		return (HEREDOC);
	if (ft_strncmp(input, "|", len) == 0)
		return (PIPE);
	else
		return (NONE);
}

int	count_commands(t_commands *cmd_as_token)
{
	int			count;
	t_commands	*travel;

	count = 0;
	travel = cmd_as_token;
	while (travel)
	{
		count++;
		travel = travel->next_command;
	}
	return (count);
}

int	count_redir(t_commands **cmd_as_token, t_token_type redir_type)
{
	size_t		index;
	int			count;
	t_token		**travel;

	index = 0;
	count = 0;
	travel = (*cmd_as_token)->args;
	while (travel[index])
	{
		if (redir_type == REDIR_IN)
		{
			if (in_tok(travel[index]->type))
				count++;
		}
		else if (redir_type == REDIR_OUT)
		{
			if (out_tok(travel[index]->type))
				count++;
		}
		index ++;
	}
	return (count);
}

size_t	len_spe_symb(char *input, size_t input_index)
{
	if (special_symb(input, input_index) == HEREDOC
		|| special_symb(input, input_index) == APPEND)
		return (2);
	else
		return (1);
}
