/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scesar <scesar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 18:36:06 by scesar            #+#    #+#             */
/*   Updated: 2025/07/30 18:36:07 by scesar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	skip_quotes(char *str, size_t *index)
{
	char	quote;

	if (str[*index] == '\'' || str[*index] == '\"')
	{
		quote = str[*index];
		(*index)++;
		while (str[*index] && str[*index] != quote)
			(*index)++;
	}
}

int	check_pipe_syntax(char *input, size_t index)
{
	index++;
	while (input[index] == ' ')
		index++;
	if (input[index] == '\0' || input[index] == '|')
	{
		write(2, "Syntax error near unexpected token `|'\n", 40);
		return (0);
	}
	return (1);
}

int	redir_check_syntax(char *input, size_t index)
{
	char	redir;
	size_t	i;

	redir = input[index];
	i = index + 1;
	if (input[i] == redir)
		i++;
	while (input[i] == ' ')
		i++;
	if (input[i] == '|' || input[i]
		== '>' || input[i] == '<' || input[i] == '\0')
	{
		write(2, "Syntax error near unexpected token ", 36);
		if (input[i] == '\0')
			write(2, "newline", 7);
		else
			ft_printf(2, "'%c'", input[i]);
		write(2, "\n", 1);
		return (0);
	}
	return (1);
}
