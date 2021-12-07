#include "cat.h"
#include<regex.h>

int InitStruct_grep(Flags_grep *flag);
int IndicateFlag_grep(Flags_grep *flags, char c);
int ParserFlags_grep(Flags_grep *flags, int argc, char **argv, char *str, int *argv_number, char *search_str, int *search_number, int *count_flags);

int ProcessFile_grep(Flags_grep *flags, char *file, char **search_str, int search_number, int CountFile);
int CheckLine(Flags_grep *flags, char *line, char **search_str, int search_number);

int main(int argc, char *argv[]) {
    int order = 1, shift = 1, error = 0;
    char **search_str;
    search_str = (char**)malloc(1024 *sizeof(char*));
    for (int i = 0; i < 1024; i++) {
        *(search_str + i) = (char*) malloc(1024 * sizeof(char));
    }
    int CountFile = 0;
    int search_number = 0, count_flags = 0;
    Flags_grep flags;
    InitStruct_grep(&flags);
    int i = 1;
    // parser
    while (i < argc) {
        if (argv[i][0] == '-') {
            if (ParserFlags_grep(&flags, argc, argv, argv[i], &i, search_str[search_number], &search_number, &count_flags)) {
                error = 1;
                break;
            }
        }
        i++;
    }
    if (!count_flags) {
        strcat(search_str[search_number], argv[1]);
        argv[1][0] = '-';
        search_number += 1;  
    } else if (flags.e == 0) {
        i = 1; 
        while( i < argc) {
            if (argv[i][0] != '-') {
                strcat(search_str[search_number], argv[i]);
                argv[i][0] = '-';
                search_number += 1;
                break;  
            } 
            i++;    
        }
        if (search_number == 0)
            error = 1; 
    }
    // count files
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-')
            CountFile += 1;
    }

    
    // printf("l:%d\n", flags.l);
    // printf("c:%d\n", flags.c);

    // printf("\ncount_flags:%d\n", count_flags);
    // printf("str1:%s\n", search_str[0]);
    // printf("str2:%s\n", search_str[1]);
    // printf("str3:%s\n", search_str[2]);
    
    if (!error) {
        for (int i = 1; i < argc; i++) {
            if (argv[i][0] != '-')
                order = ProcessFile_grep(&flags, argv[i], search_str, search_number, CountFile);
        }
    }

    for (int i = 0; i < 1024; i++) {
        free(*(search_str+i));
    }
    free(search_str);
    return 0;
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
            // printf("%c", str[i]);
            if (IndicateFlag_grep(flags, str[i])) {
                error = 1;
                break;
            } else {
                // printf("in:%c", str[i]);
                *count_flags += 1;
                if (str[i] == 'e') {
                    if (str[i + 1] != '\0') {
                        // printf("tek:%c\n", str[i]);
                        memmove(search_str, str + i + 1, strlen(str) - i);
                    } else if (*argv_number + 1 < argc) {
                        strcat(search_str, argv[*argv_number + 1]);
                        argv[*argv_number + 1][0] = '-';
                        *argv_number += 1;
                    } else {
                        printf("grep: options requires an argument :e");
                        error = 1;
                        break;
                    }
                    *search_number += 1;
                    break;
                }
            }
            i++;
        }
        // printf("\n");
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


int ProcessFile_grep(Flags_grep *flags, char *file, char **search_str, int search_number, int CountFile) {
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    regex_t regex;
    char *FileName = (char*) malloc(512 * sizeof(char));
    FILE *fp;
    fp = fopen(file, "r");
    if (fp == NULL) {
        printf("grep: %s: No such file or directory", file);
    } else {
        if (CountFile > 1) {
            strcat(FileName, file);
            FileName[strlen(file)] = ':';
            FileName[strlen(file) + 1] = '\0';
        } else {
            FileName[0] = '\0';
        }
        int CountFlagC = 0, CountFlagN = 0, CountFlagL = 0;
        while (getline(&line, &len, fp) != -1) {
            CountFlagN += 1;
            if (CheckLine(flags, line, search_str, search_number)) {
                if (flags->l)
                    CountFlagL += 1;
                if (flags->c)
                    CountFlagC += 1;
                if (!flags->c && !flags->l) {
                    if (flags->n)
                        printf("%s%d:%s", FileName, CountFlagN, line);
                    else
                        printf("%s%s", FileName, line);
                }
            }
        }
        // printf("l:%d\n", CountFlagL);
        // printf("c:%d\n", flags->c);
        // printf("countc:%d\n", CountFlagC);

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
    }
    free(FileName);
    fclose(fp);
    return 0;
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

