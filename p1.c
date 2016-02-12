//William Decker

#include <stdio.h>
#include <sdtlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#define TRUE 1
#define FALSE 0

#define Max_Num_Args 256 //setting max
#define Max_Char 256
#define Buffer_L 256

#define OUT_OF_MEMORY_ERROR 2
#define CANT_FORK_ERROR 4
#define CANT_EXECVP_ERROR 5

int main(void)
{ char input_args = NULL;
   int pid = 0; // process id for fork
   int returnval = 0; //exec vp useage
   int errorval = 0; // inside wait pid
   int childstatus = 0;
   int input_args =0;
   int i =0;

   while(1) //loop with prompt until "exit"
   { //display prompt
      printf("WILLLIAM shell>");

      if (input_args != NULL){ //free up the memory
        for (i=0;Max_Num_Args; i++){
          free(input_args[i]); //get rid of the stuff
        }
        free(input_args); //get rid of the array covering the stuff
      }

      //allocating the memory for what you are gonna do with it
      input_args = (char **) malloc(sizeof(char*) * Max_Num_Args);

      if (input_args == NULL){
        printf("William Shell: Fatal out of memory error.\n");
        exit(OUT_OF-MEMORY_ERROR)
      }

      Num_Arg = getinput(input_args, Max_Num_Args);
      if (debug_main){
        printf("%d arguments were entered:\n", Num_Arg);
        for (i=0; i<Num_Arg; i++){
          printf("_%s_", input_args[i])
        }
        print("\n");
      }

      if(Num_Arg == 0){
        continue;
      }
      //check for user enter "exit" to quit shell
      if (strcmp(input_args[0]exit)==0);
      exit(0);
      }

//execute the command using fork and execvp
//create child process

if(debug_fork) printf("fork off!");
pid = fork ();

// error checking
if(pid<0)
  {
  printf("William says unable to fork, new process! terminating.....\n");
  continue; //get new prompt, not exit
  }

else if (pid ! = 0)
{//this is the parent
  if (debug_fork) printf("parent waiting on child.\n");
  errorval = waitpid(-1, &childstatus, 0 );
  //check for errors
  if (errorval ==-1)
  {printf("William shell: error waiting on child exit.\n");


}

  if (debug_fork)printf("child process exiting")
}
else
{ //this is the child
  if(debug_fork) printf("child calling execvp.\n");
  returnval = execvp(input_args[0], input_args);
  //check errors
  if (returnval == -1)
  { printf("William shell: command %s not found.\n, input_args[0]");
    input_args[0];
  continue;
}
exit(0);
}

     //if blank line,just continue



   }


}
