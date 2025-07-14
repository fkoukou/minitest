#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <limits.h>
#include <sys/wait.h>
#include <limits.h>
#include <readline/readline.h>
#include <readline/history.h>
# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

#define CMD_NOT_FOUND 127

typedef struct s_env
{
    char            *key;
    char            *value;
    struct s_env    *next;
}   t_env;

typedef enum {
    T_WORD,
    T_PIPE,
    T_REDIR_IN,
    T_REDIR_OUT,
    T_REDIR_APPEND,
    T_HEREDOC
} t_type;

typedef enum {
    Q_NONE,
    Q_SINGLE,
    Q_DOUBLE,
	Q_DANDS,
} t_quote_type;

typedef struct	s_token
{
	int quote;
	char	*value;
	t_type type;
	struct	s_token *next;
} t_token;

typedef struct s_redirect
{
	int quote;
	t_type	type;
	char	*filename;
	struct s_redirect *next;
} t_redirect;

typedef struct	s_command
{
	char	**args;
        int heredoc_pipe_fd;
	t_redirect	*redirects;
	struct s_command *next_pipe;
} t_command;

typedef enum e_quote_type1
{
    NONE = 0,
    SINGLE = 1,
    DOUBLE = 2
} t_quote_type1;


char *find_in_path(char *cmd, t_env *env_list);
int ft_strlen(char *s1);
char *ft_strdup(char *s1);
t_redirect      *new_redirect(int type, char *file, int quote);
t_redirect      *add_redirect(t_redirect **red, t_redirect *new_red);
char    **add_arg(char *value, char **arg);
t_command       *parse_command(t_token **tokens);
t_command       *parse_pipeline(t_token **tokens);
char *ft_strjoin(char *s1, char *s2);
char *strnd(char *s1, int start, int end);
int ft_isalnum(int c);
int is_space(char c);
int     is_operator(char c);
int     check_quotes(char *input, int i);
t_token *tokenize(char *input, t_env *env_list);
char    *non_expand(char *input, int len);
t_type  opperator_type(char *input);
t_token *new_token(char *value, int type, int quote);
void    add_token(t_token **head, char *value, int type, int quote);
t_quote_type    ft_next_tq(int single_q, int double_q, int none_q);
t_quote_type    check_type_quotes(char *input, int len, int single_q, int double_q);
char *add_expansion(int start, int end, char *input);
char *expansion(int *i, int len, char *input, t_env *env_list);
 char *build_token_nq(int *i, char *input, char *token, int len, t_env *env_list);
 char *find_token(char *token, char *input, int len, t_env *env_list);
int     is_heredoc(t_token *head);
char *token(t_token *head, char *input, int len, t_type type, t_env *env_list);
int is_q_or_non(char *input, int len);



// builtins
int builtin_cd(t_env *env, char **args);
int builtin_echo(char **args, t_env *env_list);
int builtin_env(t_env *env_list, char **args);
int builtin_exit(char **args);
int builtin_export(char **args, t_env **env_list);
int builtin_pwd(void);
int builtin_unset(char **args, t_env **env_list);
// void execute_command(char **args, t_env *env_list);
// minishell.h
char *ft_strjoin_free(char *s1, const char *s2);
char *expand_arg(char *arg, t_env *env_list);
// Dans minishell.h (ou fichier header)
int execute_builtins(t_env **env_list, t_command *cmd);
void rediriger_heredoc(char *delimiter,t_env *env_list);
void rediriger_entree(const char *fichier);
void rediriger_sortie(const char *fichier);
void rediriger_sortie_append(const char *fichier);

// void    remove_quotes(char **str);
void    read_heredoc(char *delimiter);
int is_quoted(const char *str);

void execute_external(char **args, t_env *env_list);
char **tokenize_echo_args(char *input);
char **ft_split(char const *s, char c);
void free_array(char **array);
void free_env(t_env *env);
t_env *create_env(char **envp);
void print_env(t_env *env);
char **env_list_to_array(t_env *env_list);
void free_env_array(char **env_array);
char *get_env_value(t_env *env_list, const char *key);
t_quote_type1 check_type_quotes1(const char *input, int len);
void remove_surrounding_quotes(char **str);
void pipeline(t_env **env_list, t_command *cmd_list,size_t nb_cmd);
void	setup_signal_handlers(void);
void	signal_handler_sigquit(int signo);
void	signal_handler_sigint(int signo);
char **ft_split(char const *s, char c);
char *remove_quotes(const char *str);
int	ft_isdigit(int c);
int ft_strcmp(const char *s1, const char *s2);
char *expand_heredoc_line(const char *line, t_env *env_list);
void handle_heredoc(char *raw_delimiter, t_env *env_list, t_command *cmd);

#endif  