#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int    picoshell(char **cmds[])
{
	int pid;
	int fd[2];
	int prev_fd = -1;
	int i = 0;

	while (cmds[i])
	{
		if (cmds[i + 1] && pipe(fd) == -1)
			return (1);
		pid = fork();
		if (pid == -1)
			return (1);
		if (pid == 0)
		{
			if(prev_fd != -1)
			{
				dup2(prev_fd, 0);
				close(prev_fd);
			}
			if(cmds[i + 1])
			{
				close(fd[0]);
				dup2(fd[1], 1);
				close(fd[1]);
			}
			execvp(cmds[i][0], cmds[i]);
			exit(1);
		}
		else
		{
			if (prev_fd != -1)
				close(prev_fd);
			if (cmds[i + 1])
			{
				close(fd[1]);
				prev_fd = fd[0];
			}
			else
			{
				close(fd[0]);
				close(fd[1]);
			}
		}
		i++;
		wait(&pid);	
	}
	return (0);
}
