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
            archive("folder1");
            if (fopen("dop_file.txt", "r") == NULL) {
                return 0;
            } else {
                head();
                remove("dop_file.txt");
            }
            unarchiver("archive.txt");
            break;
        case 2:
            scanf("%s", path);
            archive(path);
            chdir("/home/iren/MAI/OS");
            if (fopen("dop_file.txt", "r") == NULL) {
                return 0;
            } else {
                head();
                remove("dop_file.txt");
            }
            break;
        // case 3:
        //     unarchiver("archive.txt");
        //     break;
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
    // char full_path[384];
    FILE *in, *out, *dop;

    if ((dp = opendir(dir)) == NULL) {
        fprintf(stderr, "cannot open directory: %s\n", dir);
        return;
    }

    out = fopen("/home/iren/MAI/OS/dop_file.txt", "a");
    dop = fopen("/home/iren/MAI/OS/archive.txt", "a+");
    chdir(dir);
    while ((entry = readdir(dp)) != NULL) {
        lstat(entry->d_name, &statbuf);
        if (S_ISDIR(statbuf.st_mode)) {
            if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0) continue;
            archive(entry->d_name);
        } else {
            in = fopen(entry->d_name, "r");
            getcwd(path, 128);
            // sprintf(full_path, "%s/%s", path, entry->d_name);
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
    FILE *in = fopen("dop_file.txt", "r");
    FILE *out = fopen("archive.txt", "a+");
    fputc('\n', out);
    while ((c = fgetc(in)) != EOF) {
        fputc(c, out);
    }
    fclose(out);
    fclose(in);
}

void unarchiver(char *archive_name) {
    FILE *in = fopen(archive_name, "r");
    char header[512], *name, *size_str, *path, *name_dir = "folder_unarch";
    char c;
    int i = 0, size;
    mkdir(name_dir, 0777);
    DIR *dp = opendir(name_dir);
    chdir(name_dir);
    while ((c = fgetc(in)) != '\n') {
        header[i] = c;
        i++;
    }
    name = strtok(header, " ");
    size_str = strtok(NULL, " ");
    path = strtok(NULL, " ");
    while (name != NULL && size_str != NULL && path != NULL) {
        // printf("%s\n", path);
        // char *doub_path = path;
        // printf("%s\n", doub_path);
        // create_dir(doub_path);
        size = atoi(size_str);
        int size_file = 0;
        FILE *out = fopen(name, "a");
        while (size_file != size) {
            c = fgetc(in);
            fputc(c, out);
            size_file++;
        }
        fclose(out);
        chdir(name_dir);
        // printf("%s", path);
        name = strtok(NULL, " ");
        size_str = strtok(NULL, " ");
        path = strtok(NULL, " ");
    }
    fclose(in);
    closedir(dp);
}

// void create_dir(char *path) {
//     int slash = 0;
//     char *copy_path = path;
//     while (*path != '\0') {
//         if (*path == '/') {
//             slash++;
//         }
//         path++;
//     }
//     char *folder;
//     if (slash > 0) {
//         for (int i = 0; i < slash; i++) {
//             if (i == 0) {
//                 folder = strtok(copy_path, "/");
//             } else {
//                 folder = strtok(NULL, "/");
//             }
//             // printf("%s", folder);
//         }

//         if (opendir(folder) == NULL) {
//             mkdir(folder, 0777);
//         }
//         chdir(folder);
//     }
// }