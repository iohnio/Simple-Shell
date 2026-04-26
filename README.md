## Student Information
- Name: John Jo
- Email: sunghyj1@umbc.edu
- Student ID: XA75336
- Class Section: 2

## Project Description
SimpleShell is a custom Linux command-line interpreter written in C that operates through a continuous while control loop 
designed to manage input tokenization and process execution. The shell utilizes a prioritized if-else if-else logic chain 
to distinguish between builtins and system commands. It first intercepts built-in commands like 'exit' to gracefully 
terminate the shell, 'history' to display the last 10 commands entered during a session, and 'procread' to view information 
from the proc filesystem. For all non-builtin commands, the shell forks a child process and uses the exec family of functions 
to run system binaries like ls or grep while the parent process synchronizes via wait(). Lastly, the shell features input 
parsing that correctly handles complex input, including quoted strings and escaped characters, to run commands.

## How to Compile
1. open terminal
2. navigate to project directory 'cd project1-sp26-iohnio'
3. run 'make clean' to make sure no old object files are present
4. run 'make build' to compile shell.c and utils.c

## How to Run the Project
./shell

## Project Issues
1. When entering a very long text to test buffer size, Valgrind reports a "Syscall param execve(filename) points to uninitialised btye(s)" 
   warning despite a perfect heap summary (0 bytes lost).

## LLM/AI Prompts Used
1. Is a command line simply a text input for commands and the shell the director that tells the kernel what to do?
2. Summarize the project requirements
3. What is size_t?
4. Why am I getting this error? utils.c:62:31: error: unused variable ‘i’ [-Werror=unused-variable]
5. Give me an example of using fork() and wait()
6. Why isn't .421history in the file structure?
7. How do I free memory in C?

## Sources Used
1. google gemini - used to ask questions on meeting project requirements and debugging 
2. https://docs.kernel.org/filesystems/proc.html - used to understand the proc filesystem
3. https://pubs.opengroup.org/onlinepubs/9699919799/functions/contents.html - used to understand and implement standard library functions
   and system calls like fgetc, malloc, fork, and exec family

