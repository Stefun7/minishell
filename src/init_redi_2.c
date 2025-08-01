/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_redi_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scesar <scesar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 15:18:42 by scesar            #+#    #+#             */
/*   Updated: 2025/08/01 15:19:17 by scesar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_redir	*add_redir_in(t_redir *redir_list, t_token_type type,
				char *file, size_t *io_index)
{
	t_redir	*added;

	redir_list[(io_index)[1]].file_name = ft_strdup(file);
	if (!redir_list[(io_index)[1]].file_name)
		return (NULL);
	redir_list[(io_index)[1]].type = type;
	added = &redir_list[(io_index)[1]];
	redir_list[(io_index)[1]].index = (io_index)[2] + (io_index)[1];
	(io_index)[1]++;
	return (added);
}

t_redir	*add_redir_out(t_redir *redir_list, t_token_type type,
				char *file, size_t *io_index)
{
	t_redir	*added;

	redir_list[io_index[2]].file_name = ft_strdup(file);
	if (!redir_list[io_index[2]].file_name)
		return (NULL);
	redir_list[io_index[2]].type = type;
	added = &redir_list[io_index[2]];
	redir_list[io_index[2]].index = io_index[2] + io_index[1];
	io_index[2]++;
	return (added);
}
