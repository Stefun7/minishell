/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_shell_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scesar <scesar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 00:23:46 by scesar            #+#    #+#             */
/*   Updated: 2025/07/24 10:22:29 by scesar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

bool	is_expandable_dollar(const char *string, int str_ind, bool in_double)
{
	if (string[str_ind] != '$')
		return (false);
	if (string[str_ind + 1] == '\0')
		return (false);
	if (string[str_ind + 1] == '$')
		return (false);
	if (string[str_ind + 1] == ' ')
		return (false);
	if (string[str_ind + 1] == '\"' && in_double)
		return (false);
	if (string[str_ind + 1] == '\'')
		return (false);
	return (true);
}
