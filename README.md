# Simple-Shell
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
2. run 'make clean' to make sure no old object files are present
3. run 'make build' to compile shell.c and utils.c

## How to Run the Project
./shell

## Project Issues
1. When entering a very long text to test buffer size, Valgrind reports a "Syscall param execve(filename) points to uninitialised btye(s)" 
   warning despite a perfect heap summary (0 bytes lost).

## Example Usage
```bash
> echo "test"
test

> echo "meow meow"
meow meow

> echo "CMSC 421 is awesome!"
CMSC 421 is awesome!

> history
echo "test"
echo "meow meow"
echo "CMSC 421 is awesome!"

> history
echo "test"
echo "meow meow"
echo "CMSC 421 is awesome!"
history

> procread partitions
major minor  #blocks  name

   8        0  976762584 sda
   8        1    4194304 sda1
   8        2    8388608 sda2
   8        3  964177920 sda3
   8       16  976762584 sdb
   8       17  976760832 sdb1

>
```
