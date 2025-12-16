#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ANSI_BG_WHITE      "\x1b[47m"
#define ANSI_COLOR_BLACK   "\x1b[30m"
#define ANSI_COLOR_GREY    "\x1b[38;5;244m"
#define ANSI_COLOR_BLUE    "\x1b[38;5;153m"
#define ANSI_COLOR_RED     "\x1b[38;5;210m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define NAME_LEN 50

// Struktura direktorija (stablo: dijete–brat)
typedef struct Dir {
    char name[NAME_LEN];
    struct Dir* parent;
    struct Dir* child;   // prvi pod-direktorij
    struct Dir* sibling; // sljedeći direktorij u istoj razini
} Dir;

Dir* createDir(const char* name, Dir* parent) {
    Dir* d = (Dir*)malloc(sizeof(Dir));
    if (!d) {
        printf("Greska pri alokaciji memorije!\n");
        exit(1);
    }
    strcpy(d->name, name);
    d->parent = parent;
    d->child = NULL;
    d->sibling = NULL;
    return d;
}

int md(Dir* current, const char* name) {
    Dir* newDir = createDir(name, current);

    if (current->child == NULL) {
        current->child = newDir;
    }
    else {
        Dir* temp = current->child;
        while (temp->sibling != NULL)
            temp = temp->sibling;
        temp->sibling = newDir;
    }
    return 0;
}

Dir* cd(Dir* current, const char* name) {
    Dir* temp = current->child;
    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0)
            return temp;
        temp = temp->sibling;
    }
    printf("Direktorij '%s' ne postoji!\n", name);
    return current;
}

Dir* cd_up(Dir* current) {
    if (current->parent != NULL)
        return current->parent;
    printf("Vec ste u root direktoriju!\n");
    return current;
}

int dir(Dir* current) {
    Dir* temp = current->child;
    if (temp == NULL) {
        printf("Direktorij je prazan.\n");
        return;
    }
    while (temp != NULL) {
        printf(ANSI_COLOR_GREY"<DIR>"ANSI_COLOR_RESET "%s\n", temp->name);
        temp = temp->sibling;
    }
    return 0;
}

int printPath(Dir* current) {
    if (current->parent != NULL)
        printPath(current->parent);
    printf("/%s", current->name);
    return 0;
}

int freeTree(Dir* root) {
    if (!root) return;
    freeTree(root->child);
    freeTree(root->sibling);
    free(root);
    return 0;
}

int main() {
    int choice;
    char name[NAME_LEN];

    Dir* root = createDir("root", NULL);
    Dir* current = root;

    do {
        printf(ANSI_BG_WHITE ANSI_COLOR_BLACK"Trenutni direktorij: ");
        printPath(current); 
        printf(ANSI_COLOR_RESET"\n");
        printf(ANSI_COLOR_GREY"---------------------------------------------------\n"ANSI_COLOR_RESET);
        printf(ANSI_COLOR_BLUE"    1"ANSI_COLOR_RESET ANSI_COLOR_GREY" -> "ANSI_COLOR_RESET "MD         (Napravi Direktorij)          "ANSI_COLOR_GREY"|"ANSI_COLOR_RESET"\n");
        printf(ANSI_COLOR_BLUE"    2"ANSI_COLOR_RESET ANSI_COLOR_GREY" -> "ANSI_COLOR_RESET "CD DIR     (Ulaz u Direktorij)           "ANSI_COLOR_GREY"|"ANSI_COLOR_RESET"\n");
        printf(ANSI_COLOR_BLUE"    3"ANSI_COLOR_RESET ANSI_COLOR_GREY" -> "ANSI_COLOR_RESET "CD         (Povratak)                    "ANSI_COLOR_GREY"|"ANSI_COLOR_RESET"\n");
        printf(ANSI_COLOR_BLUE"    4"ANSI_COLOR_RESET ANSI_COLOR_GREY" -> "ANSI_COLOR_RESET "DIR        (Ispis Sadrzaja)              "ANSI_COLOR_GREY"|"ANSI_COLOR_RESET"\n");
        printf(ANSI_COLOR_RED"    5"ANSI_COLOR_RESET ANSI_COLOR_GREY" -> "ANSI_COLOR_RESET "EXIT       (Izlazak iz Programa)         "ANSI_COLOR_GREY"|"ANSI_COLOR_RESET"\n");
        printf(ANSI_COLOR_GREY"---------------------------------------------------"ANSI_COLOR_RESET);
        printf("\nOdabir: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("Ime novog direktorija: ");
            scanf("%s", name);
            md(current, name);
            printf("\n\n\n\n\n");
            break;
        case 2:
            printf("Ime direktorija: ");
            scanf("%s", name);
            current = cd(current, name);
            printf("\n\n\n\n\n");
            break;
        case 3:
            current = cd_up(current);
            printf("\n\n\n\n\n");
            break;
        case 4:
            dir(current);
            printf("\n\n\n\n\n");
            break;
        case 5:
            printf("Izlaz iz programa.\n");
            printf("\n\n\n\n\n");
            break;
        default:
            printf("Neispravan odabir!\n");
            printf("\n\n\n\n\n");
        }
    } while (choice != 5);

    freeTree(root);
    return 0;
}
