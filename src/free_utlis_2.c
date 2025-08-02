/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utlis_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scesar <scesar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 23:54:27 by scesar            #+#    #+#             */
/*   Updated: 2025/08/02 14:37:17 by scesar           ###   ########.fr       */
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

void	reject_prog(int ac, char **av)
{
	if (av[1][0] != '-')
		ft_printf(2, "bash: %s: No such file or directory\n", av[1]);
	else
	{
		if (ft_strncmp(av[1], "-c", 2) == 0 && ft_strcmp(av[1], "-c") != 0)
			ft_printf(2, "bash: -%c: invalid option\n", av[1][2]);
		else if (ft_strcmp(av[1], "-c") != 0)
			ft_printf(2, "bash: -%c: invalid option\n", av[1][1]);
		else if (ac == 2)
			ft_printf(2, "bash: %s: option requires an argument", av[1]);
		else
			return ;
	}
	exit(2);
}
