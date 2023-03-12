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
    FILE *file1;

    printf("1: Archive and dearchive the directory specified in the program\n");
    printf("2: Archive and dearchive the directory using the keyboard. Enter the path.\n");
    scanf("%d", &menu);
    switch (menu) {
        case 1:
            archive("folder1");
            file1 = fopen("dop_file", "rb");
            if (file1 != NULL) {
                head();
                remove("dop_file");
                unarchiver("archive");
                fclose(file1);
            }

            break;
        case 2:
            scanf("%s", path);
            archive(path);
            chdir("/home/iren/MAI/OS");
            file1 = fopen("dop_file", "rb");
            if (file1 != NULL) {
                head();
                remove("dop_file");
                unarchiver("archive");
                fclose(file1);
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
    char path[128];
    FILE *in, *out, *dop;

    if ((dp = opendir(dir)) == NULL) {
        fprintf(stderr, "cannot open directory: %s\n", dir);
        return;
    }

    out = fopen("/home/iren/MAI/OS/dop_file", "ab");
    dop = fopen("/home/iren/MAI/OS/archive", "ab+");
    chdir(dir);
    while ((entry = readdir(dp)) != NULL) {
        lstat(entry->d_name, &statbuf);
        if (S_ISDIR(statbuf.st_mode)) {
            if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0) continue;
            archive(entry->d_name);
        } else {
            in = fopen(entry->d_name, "rb");
            getcwd(path, 128);
            fprintf(dop, "%s %ld %s ", entry->d_name, statbuf.st_size, path);

            while ((c = fgetc(in)) != EOF) {
                fputc(c, out);
            }
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
    FILE *in = fopen("dop_file", "rb");
    FILE *out = fopen("archive", "ab+");
    fputc('\n', out);
    while ((c = fgetc(in)) != EOF) {
        fputc(c, out);
    }
    fclose(out);
    fclose(in);
}

void unarchiver(char *archive_name) {
    FILE *in = fopen(archive_name, "rb");
    char header[512], *name, *size_str, *path, *name_dir = "folder_unarch";
    char c;
    int i = 0, size, slash;
    mkdir(name_dir, 0777);
    DIR *dp = opendir(name_dir);
    chdir(name_dir);
    while ((c = fgetc(in)) != '\n') {
        header[i] = c;
        i++;
    }
    char *saveptr;
    name = strtok_r(header, " ", &saveptr);
    size_str = strtok_r(NULL, " ", &saveptr);
    path = strtok_r(NULL, " ", &saveptr);

    while (name != NULL && size_str != NULL && path != NULL) {
        char *doub_path = path;

        slash = create_dir(doub_path);

        size = atoi(size_str);
        int size_file = 0;
        FILE *out = fopen(name, "ab");
        while (size_file != size) {
            c = fgetc(in);
            fputc(c, out);
            size_file++;
        }
        fclose(out);

        while (slash > 0) {
            chdir("..");
            slash--;
        }
        name = strtok_r(NULL, " ", &saveptr);
        size_str = strtok_r(NULL, " ", &saveptr);
        path = strtok_r(NULL, " ", &saveptr);
    }
    fclose(in);
    closedir(dp);
}

int create_dir(char *path) {
    int slash = 0;
    char *copy_path = strdup(path);
    char *path_ptr = copy_path;
    char *ptr = strstr(path_ptr, "/home/iren/MAI/OS/");
    ptr += strlen("/home/iren/MAI/OS/");
    while (*ptr != '\0') {
        if (*ptr == '/') {
            slash++;
        }
        ptr++;
    }
    path_ptr = copy_path;
    char *folder, *save_ptr;
    slash++;
    for (int i = 0; i < slash; i++) {
        if (i == 0) {
            char *ptr = strstr(path_ptr, "/home/iren/MAI/OS/");
            ptr += strlen("/home/iren/MAI/OS/");
            folder = strtok_r(ptr, "/", &save_ptr);
            if (opendir(folder) == NULL) {
                mkdir(folder, 0777);
            }
            chdir(folder);
        } else {
            folder = strtok_r(NULL, "/", &save_ptr);
            if (opendir(folder) == NULL) {
                mkdir(folder, 0777);
            }
            chdir(folder);
        }
    }
    free(copy_path);
    return slash;
}