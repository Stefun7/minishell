/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scesar <scesar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 23:32:23 by scesar            #+#    #+#             */
/*   Updated: 2025/07/18 23:33:10 by scesar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	append_char(char **dest, char c)
{
	char	*temp;

	temp = *dest;
	*dest = ft_strjoinchar(temp, c);
	free(temp);
}

void	handle_single_quote(char **dest, const char *str, size_t *i)
{
	char	*temp;

	(*i)++;
	while (str[*i] && str[*i] != '\'')
	{
		temp = *dest;
		*dest = ft_strjoinchar(temp, str[(*i)++]);
		free(temp);
	}
	if (str[*i] == '\'')
		(*i)++;
}

void	handle_expand(char **dest, t_minishell ms, char *str, size_t *i)
{
	char	*temp;

	temp = *dest;
	*dest = replace_var(ms, str, i, temp);
	free(temp);
}
