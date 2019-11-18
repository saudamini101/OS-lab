#include <stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include <string.h>
#define DELIM " \t\n\a"

//We're reading from multiple files, so for repetition, we're using a function.
// Rest just like lab2

// HISTORY BRIEF, HISTORY FULL - Prints all commands until then, so we store 
// all the values until we hit that 'History brief' in the file.

// If multiple files are entered on the command line, the HISTORY BRIEF, HISTORY FULL
// commands work only for the file entered last.


char histfull[50][100];
char histbrief[50][100];
int i, j;
char *arr[10];
//char j[10];

int execute(char* somefile){
	i = -1;
	char c, alldata[100];
	int cnt=0;
    pid_t pid;

    FILE *fp;
	fp=fopen(somefile,"r+");
	fseek(fp, 0, SEEK_SET);
	
	while(fscanf(fp,"%[^\n]", alldata)==1){
    	pid = fork(); 

	    if(pid < 0){
            printf("There's some problem with fork");
    		return 0;
	    }	

    	if(pid == 0){	
            int f = 0;
    	    for(char* token = strtok(alldata, DELIM); token != NULL; token = strtok(NULL, DELIM)){ 
                    arr[f] = token;
                    f++; 
                }
		    arr[f]=NULL;
        	execvp(arr[0],arr); 
		    return 0;	
    	}
	
	    else{
    		i++;
	    	c = getc(fp);
    		strcpy(histfull[i], alldata);

            //for history brief we only need first word of a command
	    	char* tok = strtok(alldata, " ");
		    strcpy(histbrief[i], tok);
    
    		wait(NULL);
	    }
	}	
	fclose(fp);
}

int main(int argc,char* argv[]){

	for(int i = 1; i < argc; i++)
		execute(argv[i]);         //all the commands on file being executed

    char command[100];
	char copycom[100];

	printf("\nEnter the command : ");
	gets(command);
	strcpy(copycom, command);

	while(strcmp(copycom, "STOP") != 0){  //as long as we're not asked to stop

		if(strcmp(copycom,"HISTORY BRIEF") == 0){
			for(int k = 0; k <= i; k++)
	    		printf("%d. %s \n", k+1, histbrief[k]);
		}

		if(strcmp(copycom,"HISTORY FULL") == 0){
			for(int k = 0;k <= i; k++)
	    		printf("%d. %s \n",k+1,histfull[k]);
		}

		if(strncmp(copycom, "EXEC", 4) == 0){

			char* tok1 = strtok(command," ");
			tok1 = strtok(NULL," ");

            // we can either be asked to execute a previous command as 
            // EXEC ls or 
            // The number of the command as
            // EXEC 1

            //For number case, 
			int k = atoi(tok1) - 1;
			if(isdigit(tok1[0]) != 0){	
				if(k > i){
                	printf("Command not found \n");
					break;
				}
                printf("Executing %d. %s... \n", k+1, histfull[k]);
			}

            // Command directly entered, instead of number
			else{
				while (strncmp(tok1, histfull[k], strlen(tok1))!=0 && k<=i)
					k++;
				if(strncmp(tok1, histfull[k], strlen(tok1))!=0){	
                    printf("Command not found \n");
					break;
				}
                printf("Executing %d. %s... \n", k+1, histfull[k]);
			}
	
            //execution of the command following 'EXEC'
			pid_t pid;
			pid=fork();
			
			if (pid == 0){
                int f=0;
                for(char* token = strtok(histfull[k], DELIM); token != NULL; token = strtok(NULL, DELIM)){ 
                    arr[f] = token;
                    f++; 
                }
				arr[f]=NULL;
	        	execvp(arr[0], arr);
				return 0; 
            }
			else if(pid > 0)
				wait(NULL);	
		}
		printf("\nEnter the command : ");
		gets(command);
		strcpy(copycom,command);
	}
	if(strncmp(copycom,"STOP", strlen("stop")) == 0){
		printf("Exiting Normally...byeee\n");
		exit(0);
	}
}	