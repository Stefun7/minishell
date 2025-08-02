/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scesar <scesar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 16:52:09 by scesar            #+#    #+#             */
/*   Updated: 2025/08/02 13:20:49 by scesar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

# define MINISHELL_H

# include "../Libft/inc/libft.h"
# include "../Libft/inc/ft_printf.h"
# include <stdbool.h>
# include <limits.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <stddef.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <termios.h>
# include <sys/stat.h>
///

# define YES 1
# define NOPE 0
# define INVALID_ARG -1
# define PATH_MAX 4096
# ifndef ECHOCTL
#  define ECHOCTL 0001000
# endif

typedef struct s_instructions	t_instructions;

typedef enum e_token_type
{
	NONE = -1,
	CMD,
	ARG,
	FLAG,
	PIPE,
	REDIR_OUT,
	REDIR_IN,
	HEREDOC,
	APPEND,
	FILENAME,
}			t_token_type;

typedef struct s_token
{
	char			*content;
	t_token_type	type;
}				t_token;

typedef struct s_commands
{
	char				*as_str;
	t_token				**args;
	struct s_commands	*next_command;

}				t_commands;

typedef struct s_redir
{
	t_token_type	type;
	char			*file_name;
	int				index;
}			t_redir;

typedef struct s_env
{
	char			*var;
	char			*value;
	struct s_env	*next;
}				t_env;

typedef struct s_instructions
{
	char	**exec;		// = whole str = "ls -la file.txt"
	int		pipe[2];
	t_redir	*in_redir;
	int		nb_files_in;
	t_redir	*out_redir;
	int		nb_files_out;
	bool	skip;
	int		fd_in;
	char	*path_command;
}			t_instructions;

typedef struct t_minishell
{
	t_env			*envp;
	t_env			*local_var;
	char			*parsed_string;
	int				non_inter;
	t_instructions	*instru;
	int				number_of_commands;
	int				(*fd_pipes)[2];
	int				last_exit_status;
}				t_minishell;

////////////////////////////////////PARSING////////////////////////////////////

int				main(int argc, char **argv, char **envp);
void			init_minish(t_minishell **minish, char **envp,
					int ac, char **av);
int				is_interactive(t_minishell *minish);
int				is_directory(const char *path);
int				check_all_redirs(t_minishell *minish);
int				check_redir_sequence(t_instructions instr, t_minishell *minish);
int				check_perm(t_redir redir, t_minishell *minish);
//prompt.c
char			*get_prompt(t_env **envp);
char			*get_curr_path(t_env **envp);
t_redir			*add_redir_out(t_redir *redir_list, t_token_type type,
					char *file, size_t *io_index);
t_redir			*add_redir_in(t_redir *redir_list, t_token_type type,
					char *file, size_t *io_index);

//ft_split_shell.c
char			**ft_split_shell(char *input);
size_t			nbr_of_elem(char *input);
int				put_elem_in_tab(char **tab, size_t tab_index, char *input,
					size_t *input_index);
void			put_elem_in_quotes(char	**elem, char **input,
					size_t *input_index, size_t *elem_index);
size_t			next_arg_len(char *input, size_t index);
size_t			end_quotes(char *input, size_t *index);

bool			is_expandable_dollar(const char *string, int str_ind,
					bool in_double);

//tokenizer
int				tok_type_init(char *content, t_commands *commands,
					size_t index);
t_commands		*tokenizer(char *input);
t_commands		*create_command_list(t_commands whole);
char			*fill_str(t_commands whole_commands,
					t_commands *current_command, size_t whole_index);
t_commands		*new_command_node(void);
bool			linker(t_commands w_c, t_commands *current,
					size_t *whole_index);
void			free_tokens_partial(t_token **tokens);
t_token_type	get_token_type_from_context(t_token *prev);

//checker
int				first_check(char *input);
int				second_check(t_commands *command);
int				redir_check(t_token *current, t_token *next);
int				pipe_check(t_token *current, t_token *next);
int				special_symb(char *input, size_t index);
int				special_symb_2(char *input);
int				unclosed_quotes(char *input);

//utils
char			*ft_delchar(char *str, char to_del);
size_t			ft_countchar(char *str, char to_count);
size_t			tab_size(char **tab);
void			free_tab(char **tab);
char			*ft_strjoinchar(char *str, char c);
int				count_redir(t_commands **cmd_as_token, t_token_type redir_type);
int				ft_is_number(const char *str);
size_t			len_spe_symb(char *input, size_t input_index);
bool			empty_input(char *input);
void			putstr_bsn(char *str, int fd, bool bsn);
int				end_spaces(char *input, size_t *index);
void			skip_quotes(char *str, size_t *index);
int				check_pipe_syntax(char *input, size_t index);
int				redir_check_syntax(char *input, size_t index);

//init_env
t_env			*set_envp(t_env **minish_env, char **envp);
int				set_next_var(t_env **next_envv, char *envv, char *equal);
int				var_already_there(t_env **minish_envp, t_env **minish_local_var,
					char *next_var);
bool			valid_var_add(char *input, char **equal);
t_env			*get_var(t_env **minish_envp, t_env **minish_local_var,
					char *VAR);
void			init_env_var(size_t *i, t_env **current_envv,
					t_env **next_envv);
char			*get_var_value(t_env **minish_envp,
					t_env **minish_local_var, char *VAR);

//loc_var
char			*fill_renew_str(char *last_str, size_t last_str_ind,
					size_t len_var, t_env *actual_var);
char			*replace_var(t_minishell minishell, char *string,
					size_t *str_ind, char *temp);
char			*get_new_string(t_minishell minishell, char *string);
int				add_loc_var(t_env **minish_envp, t_env **minish_local_var,
					char *input);
bool			is_expandable_dollar(const char *string, int str_ind,
					bool in_double);
void			append_char(char **dest, char c);
void			handle_single_quote(char **dest, const char *str, size_t *i);
void			handle_expand(char **dest, t_minishell ms, char *str,
					size_t *i);
int				update_in_double(const char *s, size_t *i, bool *in_double);
void			set_non_interactive(t_minishell **minish, int ac, char **av);

//init_instr
t_instructions	*init_insrtu(t_minishell *minish, t_commands	*cmd_as_tokens);
int				set_redir(t_instructions *instr, t_commands *cmd,
					t_minishell *minish, size_t i[3]);
int				prep_set_redir(t_instructions *instr,
					t_commands *cmd, t_minishell *minish);
int				count_commands(t_commands *cmd_as_token);
char			**tok_into_tab(t_minishell *minish, t_token **tokens);
size_t			tok_to_keep_tab_len(t_token **tokens);
t_token			**init_executable(t_token **cmd_as_tokens,
					t_instructions *instru, int index, t_minishell *minish);
bool			out_tok(t_token_type type);
bool			in_tok(t_token_type type);

//free everything
void			exit_shell(char *error_message, t_minishell **minish);
void			free_instructions(t_instructions *instru, int count);
void			free_minish_partial(t_minishell **minish);
void			free_minish_total(t_minishell **minish);
void			free_tokens(t_token **tokens);
void			free_redirs(t_redir *redir, int count);
void			free_commands(t_commands *cmd);
void			free_envp(t_env *env);
void			free_array(char ***array);
void			free_pipe(t_token **tokens);
int				cleanup_and_exit(t_minishell *minish);
int				treat_input(t_minishell **minish, char *input);
void			reject_prog(void);

///////////////////////////////////EXECUTION///////////////////////////////////

//family
int				run(t_minishell *minish);
void			process(t_minishell *minish);
void			child_process(t_minishell *minish, t_instructions *instr,
					int parser);
void			execute(t_minishell *minish, t_instructions *instr,
					int parse, char **exec);
void			close_parent(t_minishell *minish);
void			close_stuff(t_minishell *minish, int parser);
void			error(t_minishell *minish, char *reason, char *specific,
					int exit_stat);
char			*path_finding(char *pathed, t_env **envp);
void			path_not_found(char *pcommand, t_minishell *minish);
void			disable_echoctl(void);
int				handle_exit_status(int status);
void			child_signal(void);
void			heredoc_signals(void);

//access
void			access_test(t_minishell *minish, t_instructions *instr,
					int parser);
void			treat_redir_in(t_minishell *minish, t_redir *redir,
					int *fd);
void			do_ins(t_minishell *minish, t_instructions *instr);
void			treat_redir_out(t_minishell *minish, t_redir *redir, int parser,
					int *fd);
int				heredoc_handle(char *stop);
void			no_redirection_proc(t_minishell *minish, t_instructions *instr,
					int parser);
char			**shift_to_first_non_empty(char **args);
int				find_non_empty(char **str);
int				path_has_directory(const char *path);
int				dir_exists(const char *path);
void			here_wrap(t_minishell *minish);
void			heredoc_child(char *stop, int write_fd);
void			silence_signals(void);

//builtins
int				is_n_flag(const char *str);
int				check_n_flags(char **argv);
int				is_builtin(char *cmd);
int				built_in_parent(char *cmd);
int				builtin_env(t_env *envp);
int				exec_builtin(char **exec, t_minishell *shell, int parent);
int				builtin_echo(char **executables);
int				builtin_cd(char **executables, t_minishell *minish);
int				builtin_pwd(void);
t_env			*find_nth(t_env *smallest, t_env *envp);
void			print_declare(t_env *envp);
int				edit_env(char *content, t_minishell *minish);
int				is_valid_identifier(const char *str);
int				builtin_export(char **executables, t_minishell *minish);
void			builtin_exit(char **executables, t_minishell *minish);
int				builtin_unset(char **executables, t_minishell *minish);
void			print_env_array(char **envp);

//////////////////////////////////ENVIRONMENT//////////////////////////////////

int				env_list_length(t_env *traveler);
void			remove_var(t_env **envp_or_loc, char *to_remove);
t_env			*find_first(t_env *envp);
int				is_between_env(t_env *envp, t_env *smallest, t_env *bigger);
t_env			*create_env_node(char *var, char *value);
int				add_env_back(t_env **env_list, char *var, char *value);
void			add_to_envp(t_minishell *minish, char *var);
char			*join_var_value(char *var, char *value);
char			**env_list_to_array(t_env *env, int len);
int				update_env_value(t_env *env_list, const char *var_name,
					const char *new_value);
char			*get_path(char *command_to_path, char *paths, int index);
int				path_len(char *string, int index);
void			putcommand(char *command_to_path, char *potential_path,
					int size);
int				update_shlvl(t_env **next_envv);

//////signal
void			setup_signals(void);
void			sigquit_handler(int sig);
void			sigint_handler(int sig);
void			enable_echoctl(void);
///// tiny.c
bool			is_env_char_end(char c);
bool			is_executable_token(t_token_type type);

#endif
