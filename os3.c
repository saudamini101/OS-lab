#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<sys/wait.h>
#include<unistd.h>
#define DELIM " \t\n\a"

//We want to be able to write into the cmd & read and execute what's written

int main (int argc, char *argv[]){
    pid_t pid;
    FILE *fp;
    char x, alldata[50], content[100];

    char* somefile = argv[1];

    fp = fopen (somefile, "r+");
    fseek(fp, 0, SEEK_SET);       //to make sure file pointer points to beginning of the file.

    //pipe(int fds[2]) - fds[0] for read - this is one end of the pipe
    //                 - fds[1] for write - while this is the other end

    while(fscanf(fp,"%[^\n]", alldata)==1){
        int fd[2];

        if(pipe(fd) < 0)   // returns -1 on failure.
            exit(1);        

        pid = fork();
        if (pid == 0){     //child process
            close(fd[1]);    //stop writing
        //    char arr[100];
            read(fd[0], content, 100);   //read contents into data.

            char* arg[20];           //tokenizing
            int i=0;
            for(char* token = strtok(content, " "); token != NULL; token = strtok(NULL, " ")) { 
                arg[i] = token;
                i++; 
            }
            arg[i] = NULL;
            
            //compare str to quit, if its there, return 0;
            
            execvp(arg[0], arg);     //read file contents and execute, just like in lab2

            close(fd[0]);         // stop reading.
        }

        else{                           //in the parent process
            fscanf(fp, "%[^\n]", alldata);
            x = getc(fp);
            close(fd[0]);
            write(fd[1], alldata, strlen(alldata)+1);
            close(fd[1]);
            wait(NULL);    
        }
    }
    fclose(fp);
}   