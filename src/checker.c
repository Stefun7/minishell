/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scesar <scesar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 17:41:28 by scesar            #+#    #+#             */
/*   Updated: 2025/07/24 10:18:48 by scesar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	unclosed_quotes(char *input)
{
	size_t	index;

	index = 0;
	while (input[index])
	{
		if (input[index] == '\'' || input[index] == '\"')
		{
			if (end_quotes(input, &index) == 0)
				return (YES);
		}
		else
			index ++;
	}
	return (NOPE);
}

int	first_check(char *input)
{
	size_t	index;

	index = 0;
	if (unclosed_quotes(input))
	{
		write(2, "Syntax error: Unclosed quotes.\n", 32);
		return (0);
	}
	while (input[index] == ' ')
		index ++;
	if (input[index] == '|')
	{
		write(2, "Syntax error near unexpected token `|'\n", 40);
		return (0);
	}
	return (1);
}

int	second_check(t_commands *command)
{
	size_t	index;
	t_token	*next;

	next = NULL;
	while (command)
	{
		index = 0;
		while (command->args[index])
		{
			next = command->args[index + 1];
			if (!redir_check(command->args[index], next))
				return (0);
			if (!pipe_check(command->args[index], next))
				return (0);
			index ++;
		}
		command = command->next_command;
	}
	return (1);
}

int	redir_check(t_token *current, t_token *next)
{
	if (current->type == (REDIR_IN) || current->type == (REDIR_OUT)
		|| current->type == (HEREDOC) || current->type == (APPEND))
	{
		if (!next)
		{
			printf("Syntax error near unexpected token `newline'\n");
			return (0);
		}
		if (next->type != FILENAME)
		{
			printf("Syntax error near unexpected token `%s'\n", next->content);
			return (0);
		}
	}
	return (1);
}

int	pipe_check(t_token *current, t_token *next)
{
	if (current->type == PIPE)
	{
		if (!next)
		{
			printf("Syntax error near unexpected token `newline'\n");
			return (0);
		}
		if (next->type == PIPE)
		{
			printf("Syntax error near unexpected token `|'\n");
			return (0);
		}
	}
	return (1);
}
