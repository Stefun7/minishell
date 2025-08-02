/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utlis_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scesar <scesar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 23:54:27 by scesar            #+#    #+#             */
/*   Updated: 2025/08/02 13:20:01 by scesar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	free_pipe(t_token **tokens)
{
	size_t	i;

	if (!tokens)
		return ;
	i = 0;
	while (tokens[i])
	{
		if (tokens[i]->type == PIPE)
		{
			free(tokens[i]->content);
			free(tokens[i]);
			tokens[i] = NULL;
		}
		i++;
	}
}

void	free_tab(char **tab)
{
	size_t	i;

	i = 0;
	if (!tab)
		return ;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

void	reject_prog(void)
{
	printf("minishell : no argument required\n");
	exit(-1);
}
