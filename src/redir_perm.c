/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_perm.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scesar <scesar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 18:39:29 by scesar            #+#    #+#             */
/*   Updated: 2025/07/30 18:40:23 by scesar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	find_last_slash(const char *path)
{
	int	i;

	i = 0;
	while (path[i])
		i++;
	while (i > 0 && path[i] != '/')
		i--;
	return (i);
}

int	copy_dir_part(const char *path, char *dir_buf, int dir_buf_size)
{
	int	i;
	int	copy_len;
	int	j;

	i = find_last_slash(path);
	if (i <= 0)
		return (0);
	if (i < dir_buf_size - 1)
		copy_len = i;
	else
		copy_len = dir_buf_size - 1;
	j = 0;
	while (j < copy_len)
	{
		dir_buf[j] = path[j];
		j++;
	}
	dir_buf[j] = '\0';
	return (1);
}

int	check_directory_part(const char *path)
{
	char	dir[1024];

	if (!copy_dir_part(path, dir, 1024))
		return (1);
	return (is_directory(dir));
}

int	check_perm(char *path, t_token_type type)
{
	int	index;

	if (path[0] == 0)
		return (ft_printf(2, "bash: %s: No such file or directory\n", path), 2);
	if (!check_directory_part(path))
		return (ft_printf(2, "bash: %s: No such file or directory\n", path), 2);
	if (is_directory(path))
		return (ft_printf(2, "%s: Is a directory\n", path), 3);
	if (type == REDIR_IN && access(path, F_OK) != 0)
		return (ft_printf(2, "%s: No such file or directory\n", path), 2);
	if (access(path, F_OK) == 0 && out_tok(type) && (access(path, R_OK) != 0))
		return (ft_printf(2, "%s: Permission denied\n", path), 3);
	else if (type == REDIR_IN && (access(path, W_OK) != 0))
		return (ft_printf(2, "%s: Permission denied\n", path), 3);
	return (1);
}
