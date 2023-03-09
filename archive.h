#ifndef ARCHIVE_H
#define ARCHIVE_H

struct tree {
    char *name;
    long int size;
    char *folder;
};

void archive(char *dir, struct tree *file);
void head();

#endif