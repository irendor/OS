#include "archive.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    char path[200];
    int menu;

    printf("1: Archive the directory specified in the program\n");
    printf("2: Enter the path to the directory using the keyboard\n");
    scanf("%d", &menu);
    switch (menu) {
        case 1:
            archive("123");
            if (fopen("dop_file.txt", "r") == NULL) {
                return 0;
            } else {
                head();
                remove("dop_file.txt");
            }
            break;
        case 2:
            scanf("%s", path);
            archive(path);
            if (fopen("dop_file.txt", "r") == NULL) {
                return 0;
            } else {
                head();
                remove("dop_file.txt");
            }
            break;
        default:
            printf("ERROR! Wrong menu item selected");
            return 0;
    }

    return 0;
}

void archive(char *dir) {
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    char c;
    FILE *in, *out, *dop;

    if ((dp = opendir(dir)) == NULL) {
        fprintf(stderr, "cannot open directory: %s\n", dir);
        return;
    }

    out = fopen("/home/iren/MAI/OS/dop_file.txt", "a");
    fputc('\n', out);
    dop = fopen("/home/iren/MAI/OS/archive.txt", "a+");
    chdir(dir);
    while ((entry = readdir(dp)) != NULL) {
        lstat(entry->d_name, &statbuf);
        if (S_ISDIR(statbuf.st_mode)) {
            if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0) continue;
            archive(entry->d_name);
        } else {
            in = fopen(entry->d_name, "r");
            fprintf(dop, "%s %ld ", entry->d_name, statbuf.st_size);
            while ((c = fgetc(in)) != EOF) {
                fputc(c, out);
            }
            fputc('\n', out);
            fclose(in);
        }
    }
    fclose(out);
    fclose(dop);
    chdir("..");
    closedir(dp);
}

void head() {
    char c;
    FILE *in = fopen("dop_file.txt", "r");
    FILE *out = fopen("archive.txt", "a+");
    while ((c = fgetc(in)) != EOF) {
        fputc(c, out);
    }
    fclose(out);
    fclose(in);
}
