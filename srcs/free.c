/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 13:29:59 by zyeoh             #+#    #+#             */
/*   Updated: 2024/04/01 18:45:49 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	output_error(char **cmd, char *failed_cmd)
{
	int	flag;

	flag = 0;
	while (*cmd)
	{
		if (ft_strchr(*cmd, '/') && access(*cmd, F_OK) != 0)
		{
			flag = 1;
			break ;
		}
		cmd++;
	}
	if (flag == 0)
		ft_putstr_fd("Command not found: ", 2);
	else
		ft_putstr_fd("No such file or directory: ", 2);
	ft_putendl_fd(failed_cmd, 2);
	exit(128);
}

void	free_parent(t_pipex_flags *flags)
{
	int	n;

	if (flags->here_doc)
		unlink(".heredoc_temp");
	if (flags->fd_in != -1)
		close(flags->fd_in);
	if (flags->fd_out != -1)
		close(flags->fd_out);
	free(flags->pipes);
	n = -1;
	while (flags->cmd_paths[++n])
		free(flags->cmd_paths[n]);
	free(flags->cmd_paths);
}

void	close_pipes(t_pipex_flags *flags)
{
	int	n;

	n = -1;
	while (++n < flags->n_pipes)
		close(flags->pipes[n]);
}

void	pipe_fail(t_pipex_flags *flags)
{
	if (flags->here_doc)
		unlink(".heredoc_temp");
	if (flags->fd_in != -1)
		close(flags->fd_in);
	if (flags->fd_out != -1)
		close(flags->fd_out);
	perror("Pipe Error");
	exit(1);
}

// free(flags->pipes);