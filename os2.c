#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include<string.h>

#define DELIM " \t\n\a"

int main (int argc, char *argv[]){
    
    char x, alldata[200];
    pid_t pid;
    int i=0;

    char* somefile = argv[1];   //Taking the file to read from command line

    FILE *fp = fopen (somefile, "r");
    fseek(fp, 0, SEEK_SET);             //to make sure file pointer points to beginning of the file.

    if (fp == NULL){
        printf("Couldn't open the file\n");
    }
    else{
        while(fscanf(fp,"%[^\n]", alldata)==1){  //Entire data copied into alldata
        
            pid=fork();	
               
            if(pid==0) {   //child process    
                char* arr[20];
                for(char* token = strtok(alldata, DELIM); token != NULL; token = strtok(NULL, DELIM)){ 
                    arr[i] = token;
                    //printf("%s\n", arg[i]);      //tokenising our file content
                    i++; 
                }    
                arr[i] = NULL;
                execvp(arr[0], arr);
                return 0; 
            }   

            else {      //parent process
	            x=getc(fp);
	            wait(NULL);    
            } 
        }     
    }
    fclose(fp);
}