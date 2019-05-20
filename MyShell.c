// Carl Argabright
// Assignment 01
// Completed extra credit

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <limits.h>

#define MAX_USER_INPUT INT_MAX

/*****************************************************************************
  @file         MyShell.c
  @author       Carl Argabright

*******************************************************************************/

int countspaces(const char *str) {
    int count = 0 ;
    char c ;
    while((c = *str++) != 0) {
        if (c == ' ')
            count++ ;
    }
    return count ;
}

/**
  @brief Fork a child to execute the command using execvp. The parent should wait for the child to terminate
  @param args Null terminated list of arguments (including program).
  @return returns 1, to continue execution and 0 to terminate the MyShell prompt.
 */
int execute(char **args) {
    // char** args is obtained from the parse function
    pid_t pid, wpid;
    int status = 1;

    // You should fork a child and execute the command typed in by the user using EXECVP.
    if ((pid = fork()) < 0) {
        // When fork fails, an error message should be printed. However, the next MyShell> prompt should be shown.
        fprintf(stderr, "Fork failed\n");
        fflush(stderr);
        exit(EXIT_FAILURE);
    } else if (pid == 0) { // If this as new child process
        if (execvp(args[0], args) == -1) { // runs the users process
            perror("Execution failed");
        }
        // When execvp is not successful, an error message should be printed, and the child should terminate.
        // However, the next MyShell> prompt should be shown.
        exit(EXIT_FAILURE);
        // The parent goes down this path
    } else {
        if (WIFEXITED(status)) {
            int exit_status = WEXITSTATUS(status);
            if (exit_status > 0) {
                printf("Exit status of the child was %d\n", status);
                fflush(stdout);
            }
        }

        // The parent should wait for the child to terminate before it accepts another command. You can use
        // wait or waitpid for this.
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}


/**
  @brief gets the input from the prompt and splits it into tokens. Prepares the arguments for execvp
  @return returns char** args to be used by execvp
 */
char** parse(void) {
    // allocate space for up to 10 pointers to strings to be used with arguments
    char **result = NULL;
    char *userInput = malloc(MAX_USER_INPUT * sizeof(char*));
    size_t len = 0;
    // First argument should be pointer to command string and the second arguments should be a pointer to an
    // array which contains the command string as arg[0] and the other arguments as arg[1] through arg[n].
    int number_of_args = 0;
//    fgets(userInput, MAX_USER_INPUT, stdin);
    ssize_t line_size = getline(&userInput, &len, stdin);

    // If there is user input
    // Swap the newline character at the end of the input for a null terminating character
    if (userInput) {
        char *p;
        if ( (p = strchr(userInput, '\n')) ) {
            // If there is a new line character replace it with a null character.
            *p = '\0';
        }
    }

    // If we have a token allocate enough space to store 10 strings of the users input
    number_of_args = countspaces(userInput) + 1;
    char* token = strtok(userInput, " ");
    if (token) {
        result = malloc((number_of_args) * sizeof(char*));
    }

    // Tokenize the users input and store it into the result 2d-array of strings/chars
    int i = 0;
    while (token != NULL) {
        result[i] = (char*) malloc(sizeof(char) * (strlen(token) + 1));
        strcpy(result[i], token);
        token = strtok(NULL, " ");
        i++;
    }

    // The tokens are used to create a char** pointer which will serve as an argument for execvp
    free(userInput);
    return result;
}

void displayShell() {
    fprintf(stdout, "MyShell> ");
    fflush(stdout);
}


/**
   @brief Main function should run infinitely until terminated manually using CTRL+C or typing in the exit command
   It should call the parse() and execute() functions
   @param argc Argument count.
   @param argv Argument vector.
   @return status code
 */
int main(int argc, char **argv, char **env_var_ptr) {
    // Declare variables needed for program.
    char **userInput;     // Maximum length of user input.
    int isRunning = 1;

    do {
        displayShell();         // Display the shell
        userInput = parse();    // Parse the users input

        if (userInput) {
            // When the command exit is entered by the user, the main program must exit
            if (strcmp(userInput[0], "exit") == 0) {
                isRunning = 0;
            } else {
                execute(userInput);
            }
        }

        free(userInput);

    } while (isRunning);

    printf("\n\nProgram finished correctly!\n");
    return EXIT_SUCCESS;
}