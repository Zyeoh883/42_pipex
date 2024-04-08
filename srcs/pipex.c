/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 14:54:26 by zyeoh             #+#    #+#             */
/*   Updated: 2024/04/01 13:42:27 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int ac, char **av, char **env)
{
	t_pipex_flags	flags;

	ft_bzero(&flags, sizeof(t_pipex_flags));
	if (!validate(ac, av, &flags))
		return (0);
	get_fd_in(ac, av, &flags);
	get_fd_out(ac, av, &flags);
	do_pipe(ac, av, env, &flags);
	close_pipes(&flags);
	while (1)
	{
		waitpid(-1, NULL, 0);
		if (waitpid(-1, NULL, WNOHANG) == -1)
			break ;
	}
	free_parent(&flags);
	return (0);
}
