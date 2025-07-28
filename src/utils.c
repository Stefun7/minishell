/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scesar <scesar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:51:19 by scesar            #+#    #+#             */
/*   Updated: 2025/07/24 17:02:04 by scesar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*ft_strjoinchar(char *str, char c)
{
	char	*new_str;
	char	temp[2];

	temp[0] = c;
	temp[1] = '\0';
	new_str = ft_strjoin(str, temp);
	if (!new_str)
		return (NULL);
	return (new_str);
}

int	ft_is_number(const char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

bool	empty_input(char *input)
{
	size_t	index;

	index = 0;
	if (!input || *input == '\0')
		return (true);
	while (input[index])
	{
		if (input[index] != ' ' && input[index] != '	')
			return (false);
		index ++;
	}
	return (true);
}

void	putstr_bsn(char *str, int fd, bool bsn)
{
	write(fd, str, ft_strlen(str));
	if (bsn == YES)
		write (fd, "\n", 1);
}

size_t	end_quotes(char *input, size_t *index)
{
	char	quote;
	size_t	size;

	size = 0;
	quote = input[*index];
	(*index)++;
	while (input[*index] && input[*index] != quote)
	{
		size++;
		(*index)++;
	}
	if (input[*index] != quote)
		return (0);
	if (input[*index] == quote)
		(*index)++;
	return (size + 2);
}
