#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>
#include <windows.h>
#include <time.h>

// exit command
void clear(void);           // clear the screen
void cat(char fileName[]);  // cat command
long int findSize(char fileName[]);     // find the size of the file
void ls(char substr[]);     // list command
void pwd();                 // present working directory command
void info(void);            // information command
void help(char substr[]);            // help command
void cd(char substr[]);     // change directory command
void mkDir(char substr[]);  // create directory
void rm(char substr[]); // remove file or directory
void banner(void);          // banner of WCT
void textedit(void);        // open command line text editor
void currentTime(void);			// show current time


int main(void){

    // console text color
    HANDLE fontColor = GetStdHandle(STD_OUTPUT_HANDLE);
    printf("\t\t\t\t\t\tWindows Custom Terminal\n");
    SetConsoleTextAttribute(fontColor, 4);
    banner();
    SetConsoleTextAttribute(fontColor, 7);
    char cmd[32];
    static char str[16], sub[16];
    while(1){
        printf("[wct] ");
        pwd();
        printf("> ");
        SetConsoleTextAttribute(fontColor, 2);
        gets(cmd);
        SetConsoleTextAttribute(fontColor, 7); // default foreground color

        // trimming cmd into str and sub
        int length = strlen(cmd);
        int flag = 0;
        int count = 0;

        for(int i = 0; i < length; i++){
            if(cmd[i] == ' '){
                flag = 1;
            }
            else{
                if(flag == 0){
                    str[i] = cmd[i];
                }
                else{
                    sub[count] = cmd[i];
                    count++;
                }
            }
        }

        // command executions
        if(strcmp(str, "exit") == 0){
            printf("exiting...");
            break;
        }
        else if(strcmp(str, "cat") == 0){
            cat(sub);
        }
        else if(strcmp(str, "ls") == 0){
            ls(sub);
        }
        else if(strcmp(str, "pwd") == 0){
            SetConsoleTextAttribute(fontColor, 5);
            pwd();
            printf("\n\n");
            SetConsoleTextAttribute(fontColor, 7);
        }
        else if(strcmp(str, "info") == 0){
            SetConsoleTextAttribute(fontColor, 5);
            info();
            SetConsoleTextAttribute(fontColor, 7);
        }
        else if(strcmp(str, "clear") == 0){
            clear();
            printf("\t\t\t\t\t\tWindows Custom Terminal\n");
        }
        else if(strcmp(str, "help") == 0){
            SetConsoleTextAttribute(fontColor, 5);
            help(sub);
            SetConsoleTextAttribute(fontColor, 7);
        }
        else if(strcmp(str, "cd") == 0){
            cd(sub);
        }
        else if(strcmp(str, "mkdir") == 0){
            SetConsoleTextAttribute(fontColor, 10);
            mkDir(sub);
            SetConsoleTextAttribute(fontColor, 7);
        }
        else if(strcmp(str, "rm") == 0){
            SetConsoleTextAttribute(fontColor, 12);
            rm(sub);
            SetConsoleTextAttribute(fontColor, 7);
        }
        else if(strcmp(str, "textedit") == 0){
            textedit();
        }
		else if(strcmp(str, "time") == 0){
			SetConsoleTextAttribute(fontColor, 6);
			currentTime();
			SetConsoleTextAttribute(fontColor, 7);
		}
        else if(strcmp(str, "") == 0){
            continue;
        }
        else if(strcmp(str, "banner") == 0){
            SetConsoleTextAttribute(fontColor, 4);
            banner();
            SetConsoleTextAttribute(fontColor, 7);
        }
        else{
            printf("[%s] is not an internal or external command of WCT\n\n", str);
        }
        for(int i = 0; i < 16; i++){
            str[i] = '\0';
            sub[i] = '\0';
        }
    }
}

// clear command
void clear(void){
    system("cls");
}

// cat command
void cat(char fileName[]){
    HANDLE fontColor = GetStdHandle(STD_OUTPUT_HANDLE);
    FILE *fp;
    fp = fopen(fileName, "r");
    if(fp == NULL){
        SetConsoleTextAttribute(fontColor, 12);
        printf("[%s] named file does not found\n", fileName);
    }
    else{
        printf("File Name : %s\t\tFile Size : %d bytes\n", fileName, findSize(fileName));
        printf("\t\t\t\t\t\t----%s----\n", fileName);
        SetConsoleTextAttribute(fontColor, 10);
        char txt;
        while(1){
            txt = fgetc(fp);
            if(txt == EOF)
                break;
            printf("%c", txt);
        }
        printf("\n");
        fclose(fp);
    }
    printf("\n");
    SetConsoleTextAttribute(fontColor, 7);
}

// file size
long int findSize(char fileName[]){
    FILE* fp = fopen(fileName, "r");
    fseek(fp, 0L, SEEK_END);
    long int fSize = ftell(fp);
    fclose(fp);
    return fSize;
}

// list command, attributes : -a, -l
void ls(char substr[]){
    HANDLE fontColor = GetStdHandle(STD_OUTPUT_HANDLE);
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    
    if(strcmp(substr, "-a") == 0){
        if(d){
            while((dir = readdir(d)) != NULL){
                long int fileSize = findSize(dir -> d_name);
                if(fileSize == -1){
                    SetConsoleTextAttribute(fontColor, 9);
                    printf("%s\t\t", dir -> d_name);
                    SetConsoleTextAttribute(fontColor, 7);
                }
                else{
                    printf("%s\t\t", dir -> d_name);
                }
            }
            printf("\n\n");
            closedir(d);
        }
    }
    else if(strcmp(substr, "-l") == 0){
        if(d){
            char *title[3] = {"[Type]", "[Name]", "[Size]"};
            printf("%*s %*s %*s\n", 0, title[0], -30, title[1], -30, title[2]);
            while((dir = readdir(d)) != NULL){
                long int fileSize = findSize(dir -> d_name);
                if(fileSize == -1){
                    SetConsoleTextAttribute(fontColor, 9);
                    char type = 'D';
                    printf("%-6c %-30s\n", type, dir->d_name);
                    SetConsoleTextAttribute(fontColor, 7);
                }
                else{
                    char type = 'F';
                    printf("%-6c %-30s %-0ld bytes\n", type, dir->d_name, fileSize);
                }
            }
            printf("\n");
            closedir(d);
        }
    }
    else{
        if(strcmp(substr, "") == 0){
            if(d){
                while((dir = readdir(d)) != NULL){
                    if(dir->d_name[0] != '.'){
                        long int fileSize = findSize(dir -> d_name);
                        if(fileSize == -1){
                            SetConsoleTextAttribute(fontColor, 9);
                            printf("%s\t\t", dir->d_name);
                            SetConsoleTextAttribute(fontColor, 7);
                        }
                        else{
                            printf("%s\t\t", dir->d_name);
                        }
                    }
                }
            }
            printf("\n\n");
            closedir(d);
            //printf("no valid attribute for [ls] command\n\n");
        }
        else{
            printf("[%s] is not a valid attribute of [ls]\n\n", substr);
        }
    }
}

// present working directory
void pwd(void){
    char cwd[PATH_MAX];
    if(getcwd(cwd, sizeof(cwd)) != NULL){
        printf("%s", cwd);
    }
}

// information
void info(void){
    printf("Created by \t: Rakesh Kr Yadav\n");
    printf("Created on \t: 24/04/2022, 16:45:34\n");
    printf("System \t\t: Windows\n\n");
}

// change directory command
void cd(char substr[]){
    if(strcmp(substr, "..") == 0){
        chdir(substr);
    }
    else{
        if(strcmp(substr, "") == 0){
            printf("no valid attribute for [cd] command\n\n");
        }
        else{
            chdir(substr);
        }
    }
}

void mkDir(char substr[]){
    if(mkdir(substr) == 0){
        printf("[%s] named directory created SUCCESSFULLY\n\n", substr);
    }
    else{
        printf("[%s] named directory alread exist\n\n", substr);
    }
}

void rm(char substr[]){
    if(remove(substr) == 0){
        printf("[%s] named file removed SUCCESSFULLY\n\n", substr);
    }
    else if(rmdir(substr) == 0){
        printf("[%s] named directory removed SUCCESSFULLY\n\n", substr);
    }
    else{
        printf("[%s] named file/directory does not exist\n\n", substr);
    }
}

// banner
void banner(void){
    printf("\t _          __   ____    ________\n");
    printf("\t| |        / /  / __ \\  |___  ___|\n");
    printf("\t| |  __   / /  | /  \\_\\    | |\n");
    printf("\t| | /  | / /   | |   __    | |\n");
    printf("\t| |/   |/ /    | \\__/ /    | |\n");
    printf("\t|___/|___/      \\____/     |_|\n");
    printf("\n");
}

// textedit
void textedit(void){
    char fileName[32];
    printf("enter file name : ");
    gets(fileName);
    //exception handling
    clear();
    printf("\t\t\t\t\t\tText Editor\n");
    printf("/* your text should be here\n");
    printf(" * write anything you like\n");
    printf(" * enjoy! */\n\n");
}

// time
void currentTime(void){
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	printf("%d-%02d-%02d %02d:%02d:%02d\n\n",tm.tm_mday, tm.tm_mon + 1,tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
}

// help command
void help(char substr[]){
    if(strcmp(substr, "") == 0){
        printf("banner \t\t: display banner of WCT\n");
        printf("cat \t\t: show the content of the file\n");
        printf("cd \t\t: change directory\n");
        printf("clear \t\t: clear the WCT screen\n");
        printf("exit \t\t: quit the WCT\n");
        printf("help \t\t: show all the commands of WTC\n");
        printf("info \t\t: show information\n");
        printf("ls \t\t: show the list of file and directory\n");
        printf("mkdir \t\t: create directory\n");
        printf("pwd \t\t: display present working directory\n");
        printf("rm \t\t: remove file or directory\n");
        printf("textedit \t: open command line text editor\n");
		printf("time \t\t: show current time\n");

        printf("\n");
    }
    else if(strcmp(substr, "cd") == 0){
        printf("command : cd\n----------------\n");
        printf("use : cd [path]\n\n");
        printf("attributes\n----------------\n");
        printf(".. \t\t: go to the previous directory\n");
        printf("[dirName] \t: go to the given directory if it exist\n\n");
        printf("discription\n----------------\n");
        printf("change the current directory to the given attribute\n\n");
    }
    else if(strcmp(substr, "ls") == 0){
        printf("command : ls\n----------------\n");
        printf("use : ls [attribute]\n\n");
        printf("attributes\n----------------\n");
        printf("-a \t\t: show all the files/directories including the hidden files/directories\n");
        printf("-l \t\t: show the list of all files/directories with [Type][Name][Size]\n\n");
        printf("discription\n----------------\n");
        printf("show the list of files and directories of the current directory\n\n");
    }
    else if(strcmp(substr, "mkdir") == 0){
        printf("command : mkdir\n----------------\n");
        printf("use : mkdir [dirName]\n\n");
        printf("discription\n----------------\n");
        printf("make a directory of given name in the current directory\n\n");
    }
    else if(strcmp(substr, "rm") == 0){
        printf("command : rm\n----------------\n");
        printf("use : rm [fileName/dirName]\n\n");
        printf("discription\n----------------\n");
        printf("remove the given file or directory in the current directory\n\n");
    }
    else if(strcmp(substr, "cat") == 0){
        printf("command : cat\n----------------\n");
        printf("use : cat [fileName]\n\n");
        printf("discription\n----------------\n");
        printf("display the content of the file\n\n");
    }
    else{
        printf("SORRY! no information available\n\n");
    }
}
