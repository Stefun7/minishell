/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SAM_signals2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scesar <scesar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 18:41:44 by scesar            #+#    #+#             */
/*   Updated: 2025/07/30 18:42:03 by scesar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	sigint_heredoc_handler(int signum)
{
	(void)signum;
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	write(1, "\n", 1);
	exit(130);
}

void	heredoc_signals(void)
{
	signal(SIGINT, sigint_heredoc_handler);
	signal(SIGTERM, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
	enable_echoctl();
}

void	silence_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGTERM, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	child_signal(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
