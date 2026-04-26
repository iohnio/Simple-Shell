#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "utils.h"

#define HISTORY_FILE ".421history"

int main() {
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    //clear history at start of session
    FILE *clear_hist = fopen(HISTORY_FILE, "w");
    if (clear_hist) fclose(clear_hist);

    while (1) {
        printf("SimpleShell $ ");
        fflush(stdout);

        nread = getline(&line, &len, stdin);
        if (nread == -1) break;

        if (nread > 0 && line[nread - 1] == '\n') {
            line[nread - 1] = '\0';
        }

        if (strlen(line) == 0) continue;

	// history file
        FILE *hist_file = fopen(HISTORY_FILE, "a");
        if (hist_file != NULL) {
            fprintf(hist_file, "%s\n", line);
            fclose(hist_file);
        }

	//args parsing
	int arg_capacity = 10;
        char **args = calloc(arg_capacity, sizeof(char *));
        int arg_count = 0;
        char *remaining = line;

        while (remaining != NULL && *remaining != '\0') {
            while (*remaining == ' ') remaining++;
            if (*remaining == '\0') break;

            if (arg_count >= arg_capacity - 1) {
                arg_capacity *= 2;
                args = realloc(args, arg_capacity * sizeof(char *));
            }

            int space_idx = first_unquoted_space(remaining);
            char *token;
            if (space_idx == -1) {
                token = strdup(remaining);
                remaining = NULL;
            } else {
                token = strndup(remaining, space_idx);
                remaining += space_idx;
            }

            args[arg_count] = unescape(token, stderr);
            free(token);
            arg_count++;
        }
        args[arg_count] = NULL;

        if (arg_count == 0) {
            free(args);
            continue;
        }

        // exit builtin
        if (strcmp(args[0], "exit") == 0) {
            if (arg_count > 1) {
                printf("ERROR: exit does not take arguments\n");
            } else {
                for (int j = 0; j < arg_count; j++) free(args[j]);
		free(args);
                free(line);
                return 0;
            }
        }

        // history builtin
        else if (strcmp(args[0], "history") == 0) {
            FILE *read_hist = fopen(HISTORY_FILE, "r");
            if (read_hist) {
                char **stored_lines = NULL;
                size_t h_array_cap = 10;
                int total_cmds = 0;
                stored_lines = malloc(h_array_cap * sizeof(char *));

                char *h_line = NULL;
                size_t h_len = 0;

                while (getline(&h_line, &h_len, read_hist) != -1) {
                    if (total_cmds >= h_array_cap) {
                        h_array_cap *= 2;
                        stored_lines = realloc(stored_lines, h_array_cap * sizeof(char *));
                    }
                    stored_lines[total_cmds++] = strdup(h_line);
                }
                fclose(read_hist);
                free(h_line);

                int start = (total_cmds - 11 >= 0) ? total_cmds - 11 : 0;
                for (int i = start; i < total_cmds - 1; i++) {
                    printf("%s", stored_lines[i]);
                }

                for(int i = 0; i < total_cmds; i++) free(stored_lines[i]);
                free(stored_lines);
            }
        }
	// procread builtin
	else if (strcmp(args[0], "procread") == 0) {
    	    if (arg_count == 1) {
        	printf("ERROR: one argument is required.\n");
    	    } 
    	else if (arg_count > 2) {
            printf("ERROR: only a single argument is allowed.\n");
    	} 
    	else if (args[1][0] == '/') {
            printf("ERROR: the file cannot start with a slash.\n");
    	} else {
            char *path = malloc(strlen(args[1]) + 7);
            sprintf(path, "/proc/%s", args[1]);

            FILE *proc_file = fopen(path, "r");
            if (proc_file == NULL) {
                printf("ERROR: command failed, does the file exist?\n");
            } else {
                char buffer[1024];
                while (fgets(buffer, sizeof(buffer), proc_file)) {
                    char *ptr = buffer;
                    printf("%s", ptr);
                }
                fclose(proc_file);
            }
	    free(path);
        }
    }

        // system commands
        else {
            pid_t pid = fork();
            if (pid == 0) {
                execvp(args[0], args);
                fprintf(stderr, "ERROR: command not found!\n");
                // Child must cleanup its own memory before exiting
                for (int j = 0; j < arg_count; j++) free(args[j]);
                free(args);
		free(line);
                exit(1);
            } else {
                wait(NULL);
            }
        }
	// loop cleanup
        for (int j = 0; j < arg_count; j++) {
            free(args[j]);
            args[j] = NULL;
        }
	free(args);
    }

    free(line);
    return 0;
}
