#include "minishell.h"

int get_equal_idx(char *env_line)
{
	int i;

	i = -1;
	while(env_line[++i])
	{
		if (env_line[i] == '=')
			return (i);
	}
	return (-1);
}

char *get_value(char *key, char **envp)
{
	int i;
	int equal_idx;

	i = -1;
	if (key[0] == '$' && key[1] == 0)
		return (ft_itoa(getpid()));
	// else if (key[0] == '?' && key[1] == 0)

	while (envp[++i])
	{
		equal_idx = get_equal_idx(envp[i]);
		if (ft_strlen(key) == equal_idx && ft_strncmp(envp[i], key, equal_idx) == 0)
			return (ft_strdup(envp[i] + (ft_strlen(key) + 1)));
	}
	return (ft_strdup(""));
}

