/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scesar <scesar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:45:43 by scesar            #+#    #+#             */
/*   Updated: 2025/08/02 14:32:09 by scesar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	treat_input(t_minishell **minish, char *input)
{
	t_commands	*cmd_as_tokens;

	if (empty_input(input))
		return (1);
	if (!first_check(input))
		return ((*minish)->last_exit_status = 2, 3);
	if (add_loc_var(&(*minish)->envp, &(*minish)->local_var, input))
		return (1);
	cmd_as_tokens = tokenizer(input);
	if (!cmd_as_tokens)
		return (0);
	(*minish)->number_of_commands = count_commands(cmd_as_tokens);
	(*minish)->instru = init_insrtu((*minish), cmd_as_tokens);
	free_commands(cmd_as_tokens);
	if (!(*minish)->instru)
		return (0);
	(*minish)->fd_pipes = malloc(sizeof(int [2])
			* ((*minish)->number_of_commands));
	if (!(*minish)->fd_pipes)
		return (0);
	run(*minish);
	return (1);
}

void	init_minish(t_minishell **minish, char **envp, int ac, char **av)
{
	char	*shlvl[3];

	shlvl[0] = "export";
	shlvl[1] = "SHLVL=1";
	shlvl[2] = NULL;
	(*minish) = malloc(1 * sizeof(t_minishell));
	if (!(*minish))
		exit_shell("Something went wrong while initializing minish\n", NULL);
	(*minish)->envp = NULL;
	(*minish)->local_var = NULL;
	(*minish)->parsed_string = NULL;
	(*minish)->instru = NULL;
	(*minish)->number_of_commands = 0;
	(*minish)->fd_pipes = NULL;
	(*minish)->last_exit_status = 0;
	setup_signals();
	if (!set_envp(&(*minish)->envp, envp))
		exit_shell("Something went wrong while setting env\n", minish);
	if (get_var(&(*minish)->envp, NULL, "SHLVL") == NULL)
		exec_builtin(shlvl, (*minish), 0);
	set_non_interactive(minish, ac, av);
}

int	main(int ac, char **av, char **envp)
{
	t_minishell		*minish;
	char			*prompt;
	char			*input;

	if ((ac > 1))
		reject_prog(ac, av);
	init_minish(&minish, envp, ac, av);
	while (!(ac == 3 && ft_strcmp(av[1], "-c") == 0))
	{
		prompt = get_prompt(&minish->envp);
		if (!prompt)
			break ;
		input = readline(prompt);
		free(prompt);
		if (!input)
			break ;
		if (input && *input)
			add_history(input);
		if (treat_input(&minish, input) == 0)
			return (free(input), free_minish_total(&minish), 0);
		free(input);
	}
	return (cleanup_and_exit(minish));
}
