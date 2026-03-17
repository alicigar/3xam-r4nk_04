#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int ft_popen(const char *file, char *const argv[], char type)
{
	int	pipefd[2];
	pid_t	my_pid;
	
	if (!file || !argv || (type != 'r' && type != 'w'))
		return (-1);
	if (pipe(pipefd) == -1)
		return (-1);
	my_pid = fork();
	if (my_pid == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	if (my_pid == 0)
	{
		if(type == 'r')
		{
			if (dup2(pipefd[1], 1) == -1)
			{
				close(pipefd[0]);
				close(pipefd[1]);
				exit(1);
			}
		}
		else
		{
			if (dup2(pipefd[0], 0) == -1)
			{
				close(pipefd[0]);
				close(pipefd[1]);
				exit(1)
			}
		}
		close(pipefd[0]);
		close(pipefd[1]);
		execvp(file, argv);
		exit(1);
	}
	else
	{
		if (type == 'r')
		{
			close(pipefd[1]);
			return (pipefd[0]);
		}
		else
		{
			close(pipefd[0]);
			return (pipefd[1]);
		}
	}	
}
