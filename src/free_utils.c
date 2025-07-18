/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scesar <scesar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 23:45:29 by scesar            #+#    #+#             */
/*   Updated: 2025/07/18 23:50:52 by scesar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	free_tokens(t_token **tokens)
{
	int	i;

	if (!tokens)
		return ;
	i = 0;
	while (tokens[i])
	{
		// printf("free tok %p content %p\n", (void *)tokens[i],
		// 	(void *)tokens[i]->content);
		free(tokens[i]->content);
		free(tokens[i]);
		i++;
	}
	// printf("free_tokens: freeing tokens array at %p\n", (void *)tokens);
	free(tokens);
}

void	free_redirs(t_redir *redir, int count)
{
	int	i;

	i = 0;
	if (!redir)
		return ;
	while (i < count)
	{
		free(redir[i].file_name);
		i++;
	}
	free(redir);
}

void	free_envp(t_env *env)
{
	t_env	*next;

	while (env)
	{
		next = env->next;
		free(env->var);
		free(env->value);
		free(env);
		env = next;
	}
}

void	free_array(char ***array)
{
	int	i;

	if (!array || !*array)
		return ;
	i = 0;
	while ((*array)[i])
	{
		free((*array)[i]);
		i++;
	}
	free(*array);
	*array = NULL;
}

void	free_tokens_partial(t_token **tokens)
{
	if (!tokens)
		return ;
	free(tokens);
}
