#ifndef ARCHIVE_H
#define ARCHIVE_H

struct tree {
    char *name;
    long int size;
    char *folder;
};

void archive(char *dir);
void unarchiver(char *archive_name);
int create_dir(char *path);
void head();

#endif