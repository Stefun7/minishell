/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tiny.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scesar <scesar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 20:12:09 by scesar            #+#    #+#             */
/*   Updated: 2025/07/30 18:42:29 by scesar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

bool	is_env_char_end(char c)
{
	return (c == ' ' || c == '\"' || c == '$' || c == '>' || c == '<'
		|| c == '\'' || c == '|' || c == '\0');
}

bool	is_executable_token(t_token_type type)
{
	return (type == CMD || type == ARG || type == FLAG);
}

bool	out_tok(t_token_type type)
{
	return (type == REDIR_OUT || type == APPEND);
}

bool	in_tok(t_token_type type)
{
	return (type == REDIR_IN || type == HEREDOC);
}

int	update_in_double(const char *s, size_t *i, bool *in_double)
{
	if (s[*i] == '\"')
	{
		*in_double = !(*in_double);
		(*i)++;
		return (1);
	}
	return (0);
}
