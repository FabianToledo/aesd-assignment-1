
#include "systemcalls.h"

/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd)
{
    int ret = system(cmd);
    if (ret == -1 || WEXITSTATUS(ret))
    {
        return false;
    }
    return true;
}

/**
* @param count -The numbers of variables passed to the function. The variables are command to execute.
*   followed by arguments to pass to the command
*   Since exec() does not perform path expansion, the command to execute needs
*   to be an absolute path.
* @param ... - A list of 1 or more arguments after the @param count argument.
*   The first is always the full path to the command to execute with execv()
*   The remaining arguments are a list of arguments to pass to the command in execv()
* @return true if the command @param ... with arguments @param arguments were executed successfully
*   using the execv() call, false if an error occurred, either in invocation of the
*   fork, waitpid, or execv() command, or if a non-zero return value was returned
*   by the command issued in @param arguments with the specified arguments.
*/

bool do_exec(int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;

    va_end(args);

    pid_t pid;
    pid = fork();
    if (pid == -1)
    {
        return false;
    }
    // Child process
    if (pid == 0)
    {  
        execv(command[0], command);
        exit(EXIT_FAILURE);
    }
    // Wait for the child to finish
    int status;
    if (waitpid(pid, &status, 0) == -1)
    {
        return false;
    }
    if (!WIFEXITED(status))
    {
        return false;
    }
    if (WEXITSTATUS(status) != EXIT_SUCCESS)
    {
        return false;
    }

    return true;
}

/**
* @param outputfile - The full path to the file to write with command output.
*   This file will be closed at completion of the function call.
* All other parameters, see do_exec above
*/
bool do_exec_redirect(const char *outputfile, int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    va_end(args);

    pid_t pid;
    pid = fork();
    if (pid == -1)
    {
        return false;
    }
    // Child process
    if (pid == 0)
    {  
        int fd = creat(outputfile, 0664);
        if (fd == -1)
        {
            return false;
        }
        int dupfd = dup2(fd, STDOUT_FILENO);
        close(fd);
        if (dupfd == -1)
        {
            return false;
        }
        execv(command[0], command);
        exit(EXIT_FAILURE);
    }
    // Wait for the child to finish
    int status;
    if (waitpid(pid, &status, 0) == -1)
    {
        return false;
    }
    if (!WIFEXITED(status))
    {
        return false;
    }
    if (WEXITSTATUS(status) != EXIT_SUCCESS)
    {
        return false;
    }

    return true;
}
