/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_instr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scesar <scesar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 17:26:38 by scesar            #+#    #+#             */
/*   Updated: 2025/07/25 17:27:15 by scesar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

size_t	tok_to_keep_tab_len(t_token **tokens)
{
	size_t	index;
	size_t	to_keep;

	index = 0;
	to_keep = 0;
	while (tokens[index])
	{
		if (is_executable_token(tokens[index]->type))
			to_keep++;
		index ++;
	}
	return (to_keep);
}

char	**tok_into_tab(t_minishell *minish, t_token **tokens)
{
	size_t	i;
	size_t	index;
	size_t	to_keep;
	char	**tab;

	i = 0;
	index = 0;
	to_keep = tok_to_keep_tab_len(tokens);
	tab = malloc(sizeof(char *) * (to_keep + 1));
	if (!tab)
		return (NULL);
	while (tokens[i])
	{
		if (is_executable_token(tokens[i]->type) && tokens[i]->content
			&& tokens[i]->content[0] != '\0')
		{
			tab[index] = get_new_string(*minish, tokens[i]->content);
			if (!tab[index])
				return ((free_array(&tab)), NULL);
			index++;
		}
		i++;
	}
	return (tab[index] = NULL, tab);
}

t_instructions	*init_insrtu(t_minishell *minish, t_commands	*cmd_as_tokens)
{
	size_t			index;
	t_instructions	*instru;

	index = 0;
	instru = malloc((minish->number_of_commands + 1) * sizeof(t_instructions));
	if (!instru)
		return (NULL);
	while (cmd_as_tokens)
	{
		ft_bzero(&instru[index], sizeof(t_instructions));
		instru[index].path_command = NULL;
		instru[index].in_redir = NULL;
		instru[index].out_redir = NULL;
		if (!prep_set_redir(&instru[index], cmd_as_tokens, minish))
			return (NULL);
		instru[index].exec = tok_into_tab(minish, cmd_as_tokens->args);
		if (!instru[index].exec)
			return (NULL);
		cmd_as_tokens = cmd_as_tokens->next_command;
		index ++;
	}
	return (instru);
}
