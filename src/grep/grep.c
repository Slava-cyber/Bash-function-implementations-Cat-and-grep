#include "cat.h"
#include<regex.h>

int InitStruct_grep(Flags_grep *flag);
int IndicateFlag_grep(Flags_grep *flags, char c);
int ParserFlags_grep(Flags_grep *flags, int argc, char **argv, char *str, int *argv_number, char *search_str, int *search_number, int *count_flags);

int Parser(Flags_grep *flags, int argc, char **argv, char **search_str, int *search_number, int *count_flags, int *error);

int ProcessFile_grep(Flags_grep *flags, char *file, char **search_str, int search_number, int CountFile);
int CheckLine(Flags_grep *flags, char *line, char **search_str, int search_number);
int FileInit(int CountFile, char * file, char *FileName);
char FlagsProccess(Flags_grep *flags, char *line, char **search_str, int search_number, char *FileName, int *CountFlagN, int *CountFlagC, int *CountFlagL, int *print);

int main(int argc, char *argv[]) {
    char **search_str;
    search_str = (char**)malloc(1024 *sizeof(char*));
    for (int i = 0; i < 1024; i++) {
        *(search_str + i) = (char*) malloc(1024 * sizeof(char));
    }
    Flags_grep flags;
    InitStruct_grep(&flags);
    int search_number = 0, count_flags = 0, error = 0, CountFile;
    if (argc > 1) {
        CountFile = Parser(&flags, argc, argv, search_str, &search_number, &count_flags, &error);    
    } else {
        error = 1;
    }
    
    if (!error) {
        for (int i = 1; i < argc; i++) {
            if (argv[i][0] != '-')
                ProcessFile_grep(&flags, argv[i], search_str, search_number, CountFile);
        }
    }

    for (int i = 0; i < 1024; i++) {
        free(*(search_str+i));
    }
    free(search_str);
    return 0;
}

int Parser(Flags_grep *flags, int argc, char **argv, char **search_str, int *search_number, int *count_flags, int *error) {
    int CountFile = 0, i = 1;
    while (i < argc) {
            if (argv[i][0] == '-') {
                if (ParserFlags_grep(flags, argc, argv, argv[i], &i, search_str[*search_number], search_number, count_flags)) {
                    *error = 1;
                    break;
                }
            }
            i++;
        }
        if (!*count_flags) {
            sprintf(search_str[*search_number], "%s", argv[1]);
            argv[1][0] = '-';
            *search_number += 1;  
        } else if (flags->e == 0) {
            i = 1; 
            while(i < argc) {
                if (argv[i][0] != '-') {
                    sprintf(search_str[*search_number], "%s", argv[i]);
                    argv[i][0] = '-';
                    *search_number += 1;
                    break;  
                } 
                i++;    
            }
            if (*search_number == 0)
                *error = 1; 
        }
        // count files
        for (int i = 1; i < argc; i++) {
            if (argv[i][0] != '-')
                CountFile += 1;
        }
    return CountFile;
} 

int InitStruct_grep(Flags_grep *flag) {
    flag->e = 0;
    flag->i = 0;
    flag->v = 0;
    flag->n = 0;
    flag->c = 0;
    flag->l = 0;
    return 1;
}

int ParserFlags_grep(Flags_grep *flags, int argc, char **argv, char *str, int *argv_number, char *search_str, int *search_number, int *count_flags) {
    int error = 0, i = 1; 
        while (str[i] != '\0') {
            if (IndicateFlag_grep(flags, str[i])) {
                error = 1;
                break;
            } else {
                *count_flags += 1;
                if (str[i] == 'e') {
                    if (str[i + 1] != '\0') {
                        memmove(search_str, str + i + 1, strlen(str) - i);
                    } else if (*argv_number + 1 < argc) {
                        sprintf(search_str, "%s", argv[*argv_number + 1]);
                        argv[*argv_number + 1][0] = '-';
                        *argv_number += 1;
                    } else {
                        printf("grep: options requires an argument: e\n");
                        error = 1;
                        break;
                    }
                    *search_number += 1;
                    break;
                }
            }
            i++;
        }
    return error;
}

int IndicateFlag_grep(Flags_grep *flags, char c) {
    int error = 0;
    switch(c) {
        case 'e':
            flags->e = 1;
            break;
        case 'i':
            flags->i = 1;
            break;
        case 'v':
            flags->v = 1;
            break;
        case 'n':
            flags->n = 1;
            break;
        case 'c':
            flags->c = 1;
            break;
        case 'l':
            flags->l = 1;
            break;
        default:
        printf("cat: invalid option -- '%c'\n", c);
        printf("Try 'cat --help' for more information.\n");
        error = 1;        
    }
    return error;
}

char FlagsProccess(Flags_grep *flags, char *line, char **search_str, int search_number, char *FileName, int *CountFlagN, int *CountFlagC, int *CountFlagL, int *print) {
    char last = 'q';
    *print = 0;
    *CountFlagN += 1;
    if (CheckLine(flags, line, search_str, search_number)) {
        if (flags->l)
            *CountFlagL += 1;
        if (flags->c)
            *CountFlagC += 1;
        if (!flags->c && !flags->l) {
            if (flags->n)
                printf("%s%d:%s", FileName, *CountFlagN, line);
            else
                printf("%s%s", FileName, line);
            }
        *print += 1;
    }
    last = line[strlen(line) - 1];
    return last;
}


int ProcessFile_grep(Flags_grep *flags, char *file, char **search_str, int search_number, int CountFile) {
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    regex_t regex;
    char *FileName = (char*) malloc(512 * sizeof(char));
    FILE *fp;
    fp = fopen(file, "r");
    int print = 0;
    if (fp == NULL) {
            printf("grep: %s: No such file or directory\n", file);
    } else {
        FileInit(CountFile, file, FileName);
        int CountFlagC = 0, CountFlagN = 0, CountFlagL = 0;
        char last = 'q';
        while (getline(&line, &len, fp) != -1) {
            last = FlagsProccess(flags, line, search_str, search_number, FileName, &CountFlagN, &CountFlagC, &CountFlagL, &print);
        }

        if (flags->c) {
            if (!CountFlagL) {
                printf("%s%d\n", FileName, CountFlagC);
            } else if (!CountFlagC) {
                printf("%s%d\n", FileName, CountFlagC);
            } else { 
                printf("%s\n", file);
            }
        } else if (CountFlagL) { 
                printf("%s\n", file);
        }
        if (last != '\n' && print != 0 && flags->c == 0  && flags->l == 0)
            printf("\n");
        fclose(fp);
    }
    free(FileName);
    return 0;
}

int FileInit(int CountFile, char * file, char *FileName) {
        if (CountFile > 1) {
            sprintf(FileName, "%s", file);
            FileName[strlen(file)] = ':';
            FileName[strlen(file) + 1] = '\0';
        } else {
            FileName[0] = '\0';
        }
    return 1;
}

int CheckLine(Flags_grep *flags, char *line, char **search_str, int search_number) {
    int success = 0;
    regex_t regex;
    for (int i = 0; i < search_number; i++) {
        if (flags->i) { 
            regcomp(&regex, search_str[i], REG_ICASE);
        } else {
            regcomp(&regex, search_str[i], 0);
        }
        if ((regexec(&regex, line, 0, NULL, 0)) == 0) {
            success = 1;
            break;
        }
    }
    if (flags->v) {
        if (success)
            success = 0;
        else 
            success = 1;
    }
    return success;
}

