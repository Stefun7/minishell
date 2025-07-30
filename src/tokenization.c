/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scesar <scesar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 18:42:48 by scesar            #+#    #+#             */
/*   Updated: 2025/07/30 18:42:49 by scesar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*fill_str(t_commands w_c, t_commands *current, size_t whole_index)
{
	char	*res;
	char	*temp;

	res = NULL;
	temp = NULL;
	res = ft_strdup(w_c.args[whole_index]->content);
	if (!res)
		return (NULL);
	whole_index++;
	while (w_c.args[whole_index] && w_c.args[whole_index]->type != PIPE)
	{
		temp = ft_strjoin(res, " ");
		free(res);
		if (!temp)
			return (NULL);
		res = ft_strjoin(temp, w_c.args[whole_index]->content);
		if (!res)
			return (NULL);
		free(temp);
		whole_index++;
	}
	current->as_str = res;
	return (res);
}

size_t	count_next_tokens(t_token **whole_commands, size_t index)
{
	size_t	tokens;

	tokens = 0;
	while (whole_commands[index] && whole_commands[index]->type != PIPE)
	{
		tokens++;
		index++;
	}
	return (tokens);
}

int	tok_type_init(char *content, t_commands *commands, size_t index)
{
	t_token	*previous_arg;

	previous_arg = NULL;
	commands->args[index] = malloc(sizeof(t_token));
	if (!commands->args[index])
		return (0);
	commands->args[index]->content = ft_strdup(content);
	if (!commands->args[index]->content)
		return (free_tokens(&commands->args[index]), 0);
	if (index > 0)
		previous_arg = commands->args[index - 1];
	if (special_symb_2(content) != NONE)
		commands->args[index]->type = special_symb_2(content);
	else if (previous_arg && content[0] == '-' && previous_arg->type == CMD)
		commands->args[index]->type = FLAG;
	else
		commands->args[index]->type = get_token_type_from_context(previous_arg);
	return (1);
}

bool	linker(t_commands w_c, t_commands *current, size_t *whole_index)
{
	size_t	curr_index;
	size_t	count;

	if (!w_c.args || !current)
		return (false);
	curr_index = 0;
	count = count_next_tokens(w_c.args, *whole_index);
	if (count == 0)
		return (false);
	current->args = malloc(sizeof(t_token *) * (count + 1));
	if (!current->args)
		return (false);
	while (curr_index < count)
	{
		if (!w_c.args[*whole_index])
			return (free(current->args), current->args = NULL, false);
		current->args[curr_index] = w_c.args[*whole_index];
		curr_index++;
		(*whole_index)++;
	}
	current->args[curr_index] = NULL;
	if (w_c.args[*whole_index] && w_c.args[*whole_index]->type == PIPE)
		(*whole_index)++;
	return (true);
}

t_commands	*tokenizer(char *input)
{
	t_commands	w_c;
	t_commands	*first;
	char		**tab_input;
	size_t		tab_index;

	if (!input || *input == '\0')
		return (NULL);
	w_c.args = NULL;
	tab_input = ft_split_shell(input);
	if (!tab_input || !tab_input[0])
		return (NULL);
	tab_index = 0;
	w_c.args = malloc(sizeof(t_token *) * (tab_size(tab_input) + 1));
	if (!w_c.args)
		return (free_tokens(w_c.args), free_tab(tab_input), NULL);
	while (tab_input[tab_index])
	{
		if (!tok_type_init(tab_input[tab_index], &w_c, tab_index))
			return (free_tokens(w_c.args), free_tab(tab_input), NULL);
		tab_index++;
	}
	w_c.args[tab_index] = NULL;
	first = create_command_list(w_c);
	return (free_pipe(w_c.args), free(w_c.args), free_tab(tab_input), first);
}
