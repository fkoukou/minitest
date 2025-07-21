/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fakoukou <fakoukou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 08:03:11 by fakoukou          #+#    #+#             */
/*   Updated: 2025/07/21 09:56:10 by fakoukou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <errno.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>
# include <fcntl.h>
# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

# define CMD_NOT_FOUND 127
extern int g_exit_status;

typedef struct s_env
{
	char				*key;
	char				*value;
	struct s_env		*next;
}						t_env;

typedef enum
{
	T_WORD,
	T_PIPE,
	T_REDIR_IN,
	T_REDIR_OUT,
	T_REDIR_APPEND,
	T_HEREDOC
}						t_type;

typedef enum
{
	Q_NONE,
	Q_SINGLE,
	Q_DOUBLE,
	Q_DANDS,
}						t_quote_type;

typedef struct s_token
{
	int					quote;
	char				*value;
	t_type				type;
	struct s_token		*next;
}						t_token;

typedef struct s_redirect
{
	int					quote;
	t_type				type;
	char				*filename;
		char				*delimiter;  // mot-clé de fin (ex: EOF)
	struct s_redirect	*next;
}						t_redirect;

typedef struct s_command
{
	char				**args;
	int					heredoc_pipe_fd;
	t_redirect			*redirects;
	struct s_command	*next_pipe;
}						t_command;

typedef enum e_quote_type1
{
	NONE = 0,
	SINGLE = 1,
	DOUBLE = 2
}						t_quote_type1;

typedef struct s_fork_exec_params
{
	t_env				**env_list;
	t_command			*cmd;
	int					prev_fd;
	int					pipefd[2];
	size_t				i;
	size_t				nb_cmd;
}						t_fork_exec_params;

typedef struct s_prepare_args
{
	t_env		**env_list;
	t_command	*cmd;
	int			prev_fd;
	int			pipefd[2];
	size_t		i;
	size_t		nb_cmd;
}	t_prepare_args;
typedef struct s_process_args
{
	t_env **env_list;
	t_fork_exec_params *params;
	int *prev_fd;
	size_t nb_cmd;
	t_command *cmd;
	size_t i;
	
	/* data */
}t_process_args;

char					*find_in_path(char *cmd, t_env *env_list);
int						ft_strlen(char *s1);
char					*ft_strdup(char *s1);
t_redirect				*new_redirect(int type, char *file, int quote,t_env **env_list);
t_redirect				*add_redirect(t_redirect **red, t_redirect *new_red);
char					**add_arg(char *value, char **arg);
t_command				*parse_command(t_token **tokens,t_env **env_list);
t_command				*parse_pipeline(t_token **tokens, t_env **env_list);
char					*ft_strjoin(char *s1, char *s2);
char					*strnd(char *s1, int start, int end);
int						ft_isalnum(int c);
int						is_space(char c);
int						is_operator(char c);
int						check_quotes(char *input, int i);
t_token					*tokenize(char *input, t_env *env_list);
t_type					opperator_type(char *input);
t_token					*new_token(char *value, int type, int quote);
void					add_token(t_token **head, char *value, int type,
							int quote);
t_quote_type			ft_next_tq(int single_q, int double_q, int none_q);
t_quote_type			check_type_quotes(char *input, int len, int single_q,
							int double_q);
char					*add_expansion(int start, int end, char *input);
char					*expansion(int *i, int len, char *input,
							t_env *env_list);
char					*build_token_nq(int *i, char *input, char *token,
							int len, t_env *env_list);
char					*find_token(char *token, char *input, int len,
							t_env *env_list);
int						is_heredoc(t_token *head);

int						is_q_or_non(char *input, int len);

int						builtin_cd(t_env *env, char **args);
int builtin_echo(char **args, t_env *env_list);
int						builtin_env(t_env *env_list, char **args);
int						builtin_exit(char **args);
int						builtin_export(char **args, t_env **env_list);
int						builtin_pwd(void);
int						builtin_unset(char **args, t_env **env_list);

char					*expand_arg(char *arg, t_env *env_list);
int						execute_builtins(t_env **env_list, t_command *cmd);
void					rediriger_entree(const char *fichier);
void					rediriger_sortie(const char *fichier);
void					rediriger_sortie_append(const char *fichier);

void					read_heredoc(char *delimiter);
int						is_quoted(const char *str);

void					execute_external(char **args, t_env *env_list);
char					**tokenize_echo_args(char *input);
char					**ft_split(char const *s, char c);
void					free_array(char **array);
void					free_env(t_env *env);
t_env					*create_env(char **envp);
void					print_env(t_env *env);
char					**env_list_to_array(t_env *env_list);
void					free_env_array(char **env_array);
char					*get_env_value(t_env *env_list, const char *key);
t_quote_type1			check_type_quotes1(const char *input, int len);
void					remove_surrounding_quotes(char **str);
void					pipeline(t_env **env_list, t_command *cmd_list,
							size_t nb_cmd);
void					setup_signal_handlers(void);
void					signal_handler_sigquit(int signo);
void					signal_handler_sigint(int signo);
char					**ft_split(char const *s, char c);
char					*remove_quotes(const char *str);
int						ft_isdigit(int c);
int						ft_strcmp(const char *s1, const char *s2);

char					*ft_strjoin_free(char *s1, const char *s2);
// char					*expand_variable(const char *arg, int *i,
// 							t_env *env_list);
char					*get_env_path(t_env *env_list);
void					print_export_reverse(t_env *env_list);
size_t					size_cmd(t_command *cmd);
void					set_env_var(t_env **env_list, const char *key,
							const char *value);
int						is_valid_env_var_key(const char *key);
void					redirections_pipe(int index, int nb_cmd, int pipefd[2]);
int						get_exit_status(int id);
void					test(char **args, t_env *env_list);
void					fork_and_exec(t_fork_exec_params *params);
void	prepare_params(t_fork_exec_params *params, t_prepare_args *args);

void					close_prev_fd(int prev_fd);
void					handle_pipe_and_prev_fd(long unsigned int i,
							size_t nb_cmd, int pipefd[2], int *prev_fd);

void	sigint_handler(int signo);
char	*non_expand(char *input, int len, int heredoc);
char	*token(t_token **head, char *input, int len, t_type type,
		t_env *env_list);
char *ft_itoa(int n); // Déclaration manuelle si pas de header
int ft_strisspace(const char *str);
void	read_heredoc_content(char *delimiter, int expand, char *filename,
		t_env *env_list);
int	handle_heredoc(char *raw_delimiter, t_env *env_list);
// void	register_heredoc(t_redirect *redir, t_env **env_list);
int	handle_all_heredocs(t_redirect *redir, t_env **env_list);
void	child_redirects(int prev_fd, int pipefd[2], size_t i, size_t nb_cmd);
int	open_herdoc(t_redirect *red, t_env *env_list);
void rediriger_heredoc(const char *fichier);
#endif