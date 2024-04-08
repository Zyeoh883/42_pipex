/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 15:01:05 by zyeoh             #+#    #+#             */
/*   Updated: 2024/04/01 15:57:51 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	build_pipes(t_pipex_flags *flags)
{
	int	n;

	n = -1;
	while (++n < flags->n_cmd - 1)
	{
		if (pipe(flags->pipes + (2 * n)) == -1)
		{
			free(flags->pipes);
			pipe_fail(flags);
		}
	}
}

void	coupling(t_pipex_flags *flags, int n)
{
	if (n == 0)
	{
		dup2(flags->fd_in, STDIN_FILENO);
		dup2(flags->pipes[1], STDOUT_FILENO);
	}
	else if (n == flags->n_cmd - 1)
	{
		dup2(flags->pipes[2 * (n - 1)], STDIN_FILENO);
		dup2(flags->fd_out, STDOUT_FILENO);
	}
	else
	{
		dup2(flags->pipes[2 * (n - 1)], STDIN_FILENO);
		dup2(flags->pipes[2 * n + 1], STDOUT_FILENO);
	}
}

char	*get_exec(char **cmd_path, char *cmd)
{
	char	*exec;
	char	*path_part;

	if (access(cmd, F_OK) == 0)
		return (cmd);
	while (*cmd_path)
	{
		path_part = ft_strjoin(*cmd_path, "/");
		exec = ft_strjoin(path_part, cmd);
		free(path_part);
		if (access(exec, F_OK | X_OK) == 0)
			return (exec);
		free(exec);
		cmd_path++;
	}
	return (NULL);
}

void	execute(char **av, char **env, t_pipex_flags *flags, int n)
{
	pid_t	pid;
	char	**cmd;
	char	*cmd_lst;

	if (flags->invalid_infile == 1)
	{
		flags->invalid_infile = 0;
		return ;
	}
	pid = fork();
	if (pid == 0)
	{
		coupling(flags, n);
		close_pipes(flags);
		cmd = ft_split(av[2 + n + flags->here_doc], ' ');
		cmd_lst = get_exec(flags->cmd_paths, cmd[0]);
		if (execve(cmd_lst, cmd, env) == -1)
			output_error(cmd, av[2 + n + flags->here_doc]);
	}
}

void	do_pipe(int ac, char **av, char **env, t_pipex_flags *flags)
{
	int	n;

	flags->n_cmd = ac - (3 + flags->here_doc);
	flags->n_pipes = 2 * (flags->n_cmd - 1);
	flags->pipes = calloc(flags->n_pipes, sizeof(int));
	if (!flags->pipes)
		pipe_fail(flags);
	n = 0;
	while (ft_strnstr(env[n], "PATH", 4) == 0)
		n++;
	flags->cmd_paths = ft_split(env[n] + 5, ':');
	if (!flags->cmd_paths)
	{
		free(flags->pipes);
		pipe_fail(flags);
	}
	build_pipes(flags);
	n = -1;
	while (++n < flags->n_cmd)
		execute(av, env, flags, n);
}
