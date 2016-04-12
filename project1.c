/* project1.c
* JAnuary 2016
* Jennifer Coy and Rob Deadman and William Decker
* Purpose:  to gain experience with process creation, and the execvp command
*           by writing a simple shell.  */

/* NEED TO::::
 --------------> allow whitespace after the command, before the \n
 --------------> allow blank lines
 --------------> check to make sure all return values/errors are handled
                 waitpid, free, and malloc.
*/

#include <stdio.h>
#include <stdlib.h>     /* malloc */
#include <string.h>     /* string libraries */
#include <sys/types.h>  /* for fork */
#include <unistd.h>     /* for fork and execvp */

#define TRUE 1
#define FALSE 0

/**** debugging printf statements ****/
#define DEBUG_MAIN 0
#define DEBUG_GETINPUT 0
#define DEBUG_FORK 0

/**** array limits ****/
#define MAX_NUM_ARGS 256
#define MAX_NUM_CHAR 256
#define BUFFER_LEN 256

/**** return error codes...that are not used ****/
#define OUT_OF_MEMORY_ERROR 2   /* malloc has failed to get memory */
#define CANT_FORK_ERROR   4	/* error with fork() */
#define CANT_EXECVP_ERROR 5	/* error with execvp() */

/* function prototypes */
int getinput(char **args, int maxargs);

int main(void)
{  char **input_args=NULL;  	/* args for user command */
   int num_args=0;		/* number of arguments read (including 0th) */
   int i=0;			/* iteration counter */
   int pid=0;			/* process ID, used with fork() */
   int retval=0;		/* return value from execvp() */
   int errorval=0;		/* return value from waitpid() */
   int childstatus=0;		/* value returned from child for waitpid() */


   /* NOT REQUIRED: set up the signal handler (for implementing &) */

   /* loop until the user types exit */
   while(1)
   {  /* display the prompt */
      printf("WillyWorld>");

      /**** initialize array of strings for input arguments ****/
      /* we need to do this each time, since execvp requires a null pointer
       * at the end of the argument list */
      /* first check to see if we need to free the memory */
      if (input_args != NULL)
      {  /* free the memory for each array */
         for (i=0; i<MAX_NUM_ARGS; i++)
         {  /* note:  for the one with the NULL pointer, free just returns */
            free(input_args[i]);
         }
         /* free the memory for input_args array of arrays */
         free(input_args);
      }
      /* now allocate the memory */
      input_args=(char **)malloc(sizeof(char*) * MAX_NUM_ARGS);
      /* check return value for error */
      if (input_args == NULL)
      {  printf("WillyWorld had a Fatal out of memory error.\n");
         exit(OUT_OF_MEMORY_ERROR);
      }
      for (i=0; i<MAX_NUM_ARGS; i++)
      {  /* malloc memory for array of arguments */
         input_args[i]=(char*)malloc(sizeof(char) * BUFFER_LEN);
         /* check return value for error */
         if (input_args[i] == NULL)
         {  printf("WillyWorld had a Fatal out of memory error.\n");
            exit(OUT_OF_MEMORY_ERROR);
         }
      }

      /**** get the input, break into arguments ****/
      num_args = getinput(input_args, MAX_NUM_ARGS);
      if (DEBUG_MAIN)
      {  printf("%d arguments were entered:\n", num_args);
         for (i=0; i<num_args; i++)
           printf("_%s_", input_args[i]);
         printf("\n");
      }

      /* if blank line, just continue, so we get another prompt */
      if (num_args == 0)
         continue;

      /* see if they typed exit, if so, quit the shell */
      if (strcmp(input_args[0], "exit")==0) //if you type exit then it
                                            //  will exit the program
        exit(0);

      /* NOT REQUIRED: see if they typed &, set flag to run in background */
      /* NOT REQUIRED: check for redirection of stdin, set flag */
      /* NOT REQUIRED: check for redirection of stdout, set flag */

      /**** execute the command using fork and execvp ****/
      /* based on Figure 10-7 in the text */
      /* fork off a child process */
      if (DEBUG_FORK) printf("Forking!!!!\n");
      pid = fork();
      /* check for errors */
      if (pid < 0)
      {  printf("WillyWorld is Unable to fork a new process! Terminating...\n");
         continue;  /* get a new prompt, rather than quitting */
      }
      else if (pid != 0)
      {  /* this is the parent */
         if (DEBUG_FORK) {
         printf("Parent waiting on child.\n");
         errorval = waitpid(-1, &childstatus, 0);
        }
         /* check for error */
         if (errorval == -1)
         {  printf("WillyWorld had an Error waiting for child to exit.\n");
            /* continue...not a fatal error? */
         }
         if (DEBUG_FORK) printf("Child process exited.\n");
      }
      else
      {  /* this is the child */
         if (DEBUG_FORK) printf("Child calling execvp.\n");
         retval = execvp(input_args[0], input_args);
         /* check for error in execvp */
         if (retval == -1)
         {  printf("WillyWorld says: Command %s not found.\n",
                   input_args[0]);
            continue;  /* get a new prompt, rather than quitting */
         }
         /* if there is no error, the function execvp will never return, so
          * we do not strictly need the following line. */
         exit(0);
      }
   }  /* while(1) loop until user types exit */
}


/******* GETINPUT *******/
/* get the input, break into arguments
 * use getc() in order to allow for easier error checking
 * Test for exceeding the buffer length, and exceeding the maximum
 * number of allowed argumetns
 *
 * INPUT:  args = a pre-allocated array that this function will fill with
 *                arguments
 *         maxargs = the maximum number of arguments to be read
 *
 * RETURNS:  an integer that is the number of arguments read.
 */
int getinput(char **args, int maxargs)
{  int curchar = 0;      /* the current character */
   int numchar = 0;      /* the number of characters entered so far */
   int numargs = -1;     /* index to current word & number of arguments read */
   int letterindex = 0;  /* index to current letter in the word */
   int inword  = FALSE;  /* flag: are we are currently reading a word? */
   int i = 0;		 /* loop counter */

   /* print debugging info */
   if (DEBUG_GETINPUT) printf("Entering getinput().\n");

   /* loop until the if statement detects a newline */
   while (1)
   {  curchar = getc(stdin);
      if (DEBUG_GETINPUT) printf("Read %c...", curchar);
      numchar++;    /* we have one more character */

      /* check to see if we've exceeded the max allowed for this shell */
      if (numchar > MAX_NUM_CHAR)
      {  printf("WillyWorld says: Your Command too long.\n");
         /* grab the rest of the input and trash it */
         while (curchar != '\n')
         {  curchar = getc(stdin);
            printf("discarding the %c...\n", curchar);
            continue;
         }
         /* exit this function, reporting 0 arguments to get a new prompt */
         return(0);
      }

      /* read until whitespace */
      if ((curchar == ' ') || (curchar == '\t') || (curchar == '\n'))
      {  /* if this is the first space past the end of a word, null terminate,
          * reset flags, and print debugging info */
         if (inword == TRUE)
         {  if (DEBUG_GETINPUT)
               printf("That word had %d letters.\n", letterindex+1);
            /* add null terminator */
            if (letterindex<(BUFFER_LEN-1))
               args[numargs][letterindex+1] = '\0';
            else
            {  /* the user entered a word that was barely too long! */
               printf("Error -- argument exceeds maximum allowed length.\n");
               return(numargs+1);  /* exit this function */
            }
            if (DEBUG_GETINPUT)
              printf("Read the %d-th word %s.\n", numargs, args[numargs]);

            /* reset flags */
            inword = FALSE;
            letterindex = 0;   /* not strictly necessary */

            /* if that was the '\n' end of line character, return! */
            if (curchar == '\n')
            {  if (DEBUG_GETINPUT) printf("Reached end of line.\n");
               /* exit the while loop */
               break;
            }
         }
         else  /* not currently in a word */
         {  if (curchar == '\n')
            {  /* at end of line, with no command entered */
               break;  /* exit while loop */
            }
            else
            {  /* otherwise, we have whitespace before the first word, or
                * extra whitespace between words, so just continue to the
                * next letter */
               continue;
            }
         }
      }  /* if whitespace */
      else
      {  /* add to the word */
         if (inword == FALSE)
         {  /* create a new word */
            inword = TRUE;  /* we are now in a word, set flag */
            /* increment the number of arguments, and test < MAX_NUM_ARGS */
            if (numargs < (MAX_NUM_ARGS-1))
               numargs++;
            else  /* error! */
            {  printf("Error -- number of arguments exceeds the limit.\n");
               /* exit function, returning 0 to get a new prompt */
               return(0);
            }
            /* overwrite this word with null characters to empty it out */
            for (i=0; i<BUFFER_LEN; i++)
               args[numargs][i] = '\0';
            args[numargs][0] = curchar;  /* this is the first letter */
            letterindex = 0;
         }
         else
         {  /* add to the current word */
            letterindex++;  /* move to the next letter */
            args[numargs][letterindex] = curchar;  /* add this letter */
         }
         if (DEBUG_GETINPUT)
            printf("The %d-th word so far is %s.\n",
                   numargs, args[numargs]);
      } /* else not whitespace */

   } /* while(1) */

   /* set the last argument to be a NULL pointer, for execvp compatability */
   free(args[numargs+1]);	/* free the memory now, since we lose it! */
   args[numargs+1] = NULL;

   /* return the number of arguments read */
   return (numargs+1);  /* the +1 is because we start counting from -1 */
}
