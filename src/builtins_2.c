/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scesar <scesar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 20:46:12 by scesar            #+#    #+#             */
/*   Updated: 2025/07/24 09:43:02 by scesar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	exec_builtin(char **exec, t_minishell *shell)
{
	if (!is_builtin(exec[0]))
		return (-1);
	if (ft_strcmp(exec[0], "echo") == 0)
		return (builtin_echo(exec));
	if (ft_strcmp(exec[0], "pwd") == 0)
		return (builtin_pwd());
	if (ft_strcmp(exec[0], "exit") == 0)
		builtin_exit(exec);
	if (ft_strcmp(exec[0], "unset") == 0)
		return (builtin_unset(exec, &shell->envp));
	if (ft_strcmp(exec[0], "cd") == 0)
		return (builtin_cd(exec, shell));
	if (ft_strcmp(exec[0], "env") == 0)
		return (builtin_env(shell->envp));
	if (ft_strcmp(exec[0], "export") == 0)
		return (builtin_export(exec, shell));
	return (-1);
}

int	builtin_echo(char **executables)
{
	int	i;
	int	j;
	int	newline;

	i = 1;
	j = 1;
	newline = 1;
	i = check_n_flags(executables);
	if (i > 1)
		newline = 0;
	while (executables[i])
	{
		ft_printf("%s", executables[i]);
		if (executables[i + 1])
			ft_printf(" ");
		i++;
	}
	if (newline)
		ft_printf("\n");
	return (0);
}

int	chdir_fail(char *path)
{
	if (chdir(path) != 0)
	{
		perror("cd");
		return (1);
	}
	else
		return (0);
}

int	builtin_cd(char **executables, t_minishell *minish)
{
	t_env	*home_var;
	char	*path;

	if (!executables[1])
	{
		home_var = get_var(&minish->envp, NULL, "HOME");
		if (home_var == NULL)
		{
			write(2, "bash: cd: HOME not set\n", 23);
			return (1);
		}
		return (chdir_fail(home_var->value));
	}
	else
	{
		path = executables[1];
		if (executables[2])
		{
			write(2, "bash: cd: too many arguments\n", 30);
			return (1);
		}
		return (chdir_fail(path));
	}
	return (1);
}

int	builtin_pwd(void)
{
	char	cwd[500];

	if (getcwd(cwd, sizeof(cwd)))
		printf("%s\n", cwd);
	else
		perror("pwd");
	return (0);
}
