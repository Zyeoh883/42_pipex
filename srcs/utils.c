/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 14:01:12 by zyeoh             #+#    #+#             */
/*   Updated: 2024/03/31 19:45:20 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	here_doc_input(int ac, char **av, int fd)
{
	char	*line;
	int		num_pipe;

	while (1)
	{
		num_pipe = ac - 4;
		while (--num_pipe > 0)
			ft_putstr_fd("pipe ", 1);
		ft_putstr_fd("heredoc> ", 1);
		line = get_next_line(STDIN_FILENO);
		if (line)
		{
			if (ft_strncmp(line, av[2], ft_strlen(av[2])) == 0 && *(line
					+ ft_strlen(av[2])) == '\n')
				break ;
			ft_putstr_fd(line, fd);
			free(line);
		}
	}
}

void	here_doc(int ac, char **av, t_pipex_flags *flags)
{
	int	fd;

	fd = open(".heredoc_temp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("Here_doc Error");
		exit(EXIT_FAILURE);
	}
	here_doc_input(ac, av, fd);
	close(fd);
	flags->fd_in = open(".heredoc_temp", O_RDONLY);
	if (flags->fd_in == -1)
	{
		unlink(".heredoc_temp");
		perror("Here_doc Error");
		exit(EXIT_FAILURE);
	}
}

void	get_fd_out(int ac, char **av, t_pipex_flags *flags)
{
	if (flags->here_doc == 1)
	{
		flags->fd_out = open(av[ac - 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (flags->fd_out == -1)
		{
			perror("Outfile Error");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		flags->fd_out = open(av[ac - 1], O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (flags->fd_out == -1)
		{
			perror("Outfile Error");
			exit(EXIT_FAILURE);
		}
	}
}

void	get_fd_in(int ac, char **av, t_pipex_flags *flags)
{
	if (ft_strncmp(av[1], "here_doc", 9) == 0)
		here_doc(ac, av, flags);
	else
	{
		flags->fd_in = open(av[1], O_RDONLY);
		if (flags->fd_in == -1)
		{
			flags->invalid_infile = 1;
			ft_putstr_fd("No such file or directory: ", 2);
			ft_putendl_fd(av[1], 2);
		}
	}
}

int	validate(int ac, char **av, t_pipex_flags *flags)
{
	if (ac > 1 && ft_strncmp(av[1], "here_doc", 9) == 0)
	{
		if (ac < 6)
		{
			ft_putendl_fd("Format Error: here_doc LIMITER"
				" cmd0 cmd1 cmd(...) outfile", 2);
			return (0);
		}
		flags->here_doc = 1;
	}
	if (ac < 5)
	{
		ft_putendl_fd("Format Error: infile cmd0 cmd1 cmd(..) outfile", 2);
		return (0);
	}
	return (1);
}
