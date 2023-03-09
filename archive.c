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
    struct tree file[20];

    // int *fl = 0;
    // file[0].name = "\0";
    // file[0].size = 0;
    // file[0].folder = "\0";
    // file = (struct tree *)malloc(20 * sizeof(struct tree));

    printf("1: Archive the directory specified in the program\n");
    printf("2: Enter the path to the directory using the keyboard\n");
    scanf("%d", &menu);
    switch (menu) {
        case 1:
            archive("folder1", file);
            if (fopen("dop_file.txt", "r") == NULL) {
                // free(file);
                return 0;
            } else {
                head();
                remove("dop_file.txt");
            }
            break;
        case 2:
            scanf("%s", path);
            archive(path, file);
            chdir("/home/iren/MAI/OS");
            if (fopen("dop_file.txt", "r") == NULL) {
                // free(file);

                return 0;
            } else {
                head();
                remove("dop_file.txt");
            }
            break;
        default:
            printf("ERROR! Wrong menu item selected");
            // free(file);
            return 0;
    }

    // free(file);

    return 0;
}

void archive(char *dir, struct tree *file) {
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    char c;
    // int i = 0;
    FILE *in, *out, *dop;

    if ((dp = opendir(dir)) == NULL) {
        fprintf(stderr, "cannot open directory: %s\n", dir);
        return;
    }
    // if (*fl == 0) {
    //     i = 0;
    // }
    // *fl = 1;
    out = fopen("/home/iren/MAI/OS/dop_file.txt", "a");
    fputc('\n', out);
    dop = fopen("/home/iren/MAI/OS/archive.txt", "a+");
    chdir(dir);
    while ((entry = readdir(dp)) != NULL) {
        lstat(entry->d_name, &statbuf);
        if (S_ISDIR(statbuf.st_mode)) {
            if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0) continue;
            archive(entry->d_name, file);
        } else {
            in = fopen(entry->d_name, "r");
            // file[i].name = entry->d_name;
            // file[i].size = statbuf.st_size;
            // file[i].folder = dir;
            // i++;
            // printf("%d", i);
            fprintf(dop, "%s %ld %s ", entry->d_name, statbuf.st_size, dir);

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
