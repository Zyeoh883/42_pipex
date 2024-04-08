/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 21:00:00 by zyeoh             #+#    #+#             */
/*   Updated: 2024/03/31 14:52:44 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include "../libft/libft.h"
# include <stdio.h>
# include <sys/wait.h>

typedef struct s_pipex_flags
{
	int		fd_in;
	int		fd_out;
	int		here_doc;
	int		n_cmd;
	int		n_pipes;
	int		invalid_infile;
	int		*pipes;
	char	**cmd_paths;
}			t_pipex_flags;

void		output_error(char **cmd, char *failed_cmd);
void		close_pipes(t_pipex_flags *flags);
void		free_parent(t_pipex_flags *flags);
void		pipe_fail(t_pipex_flags *flags);

void		here_doc_input(int ac, char **av, int fd);
void		here_doc(int ac, char **av, t_pipex_flags *flags);
void		get_fd_out(int ac, char **av, t_pipex_flags *flags);
void		get_fd_in(int ac, char **av, t_pipex_flags *flags);
int			validate(int ac, char **av, t_pipex_flags *flags);

void		build_pipes(t_pipex_flags *flags);
void		coupling(t_pipex_flags *flags, int n);
char		*get_exec(char **cmd_path, char *cmd);
void		execute(char **av, char **env, t_pipex_flags *flags, int n);
void		do_pipe(int ac, char **av, char **env, t_pipex_flags *flags);

#endif