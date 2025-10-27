#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"

// STRUKTURA
typedef struct ID {
    char Name[20];
    char Surname[20];
    int Year;
    struct ID* next;
} ID;

// FUNKCIJE

ID* newPerson(const char* Name, const char* Surname, int Year) {
    ID* new = (ID*)malloc(sizeof(ID));
    if (!new) {
        printf("Error!\n");
        return NULL;
    }
    strcpy(new->Name, Name);
    strcpy(new->Surname, Surname);
    new->Year = Year;
    new->next = NULL;
    return new;
}

int startPerson(ID** head, const char* Name, const char* Surname, int Year) {
    ID* new = newPerson(Name, Surname, Year);
    if (!new) return 0;

    new->next = *head;
    *head = new;
    return 1;
}

int endPerson(ID** head, const char* Name, const char* Surname, int Year) {
    ID* new = newPerson(Name, Surname, Year);
    if (!new) return 0;

    if (*head == NULL) {
        *head = new;
        return 1;
    }

    ID* temp = *head;
    while (temp->next != NULL)
        temp = temp->next;

    temp->next = new;
    return 1;
}

int printList(ID* head) {
    ID* temp = head;
    if (!temp) {
        printf("Lista je prazna.\n");
        return 0;
    }

    while (temp != NULL) {
        printf("%s %s (%d)\n", temp->Name, temp->Surname, temp->Year);
        temp = temp->next;
    }
    return 1;
}

ID* findPerson(ID* head, const char* Surname) {
    ID* temp = head;
    while (temp != NULL) {
        if (strcmp(temp->Surname, Surname) == 0)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

int deletePerson(ID** head, const char* Surname) {
    ID* temp = *head;
    ID* before = NULL;

    while (temp != NULL && strcmp(temp->Surname, Surname) != 0) {
        before = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Osoba s prezimenom '%s' nije pronadena.\n", Surname);
        return 0;
    }

    if (before == NULL)
        *head = temp->next;
    else
        before->next = temp->next;

    free(temp);
    printf("Osoba s prezimenom '%s' je izbrisana.\n", Surname);
    return 1;
}

// DRUGI DIO 

// A. Dodavanje novog elementa iza odreðenog elementa
int insertAfter(ID* head, const char* targetSurname, const char* Name, const char* Surname, int Year) {
    ID* target = findPerson(head, targetSurname);
    if (!target) {
        printf("Osoba s prezimenom '%s' nije pronadena.\n", targetSurname);
        return 0;
    }

    ID* new = newPerson(Name, Surname, Year);
    if (!new) return 0;

    new->next = target->next;
    target->next = new;

    printf("Dodana osoba '%s %s' iza '%s'.\n", Name, Surname, targetSurname);
    return 1;
}

// B. Dodavanje novog elementa ispred odreðenog elementa
int insertBefore(ID** head, const char* targetSurname, const char* Name, const char* Surname, int Year) {
    ID* new = newPerson(Name, Surname, Year);
    if (!new) return 0;

    if (*head == NULL) {
        *head = new;
        return 1;
    }

    if (strcmp((*head)->Surname, targetSurname) == 0) {
        new->next = *head;
        *head = new;
        printf("Dodana osoba '%s %s' ispred '%s'.\n", Name, Surname, targetSurname);
        return 1;
    }

    ID* temp = *head;
    while (temp->next != NULL && strcmp(temp->next->Surname, targetSurname) != 0)
        temp = temp->next;

    if (temp->next == NULL) {
        printf("Osoba s prezimenom '%s' nije pronadena.\n", targetSurname);
        free(new);
        return 0;
    }

    new->next = temp->next;
    temp->next = new;

    printf("Dodana osoba '%s %s' ispred '%s'.\n", Name, Surname, targetSurname);
    return 1;
}

// C. Sortiranje liste po prezimenima
void sortList(ID** head) {
    if (*head == NULL || (*head)->next == NULL) return;

    ID* sorted = NULL;

    while (*head != NULL) {
        ID* current = *head;
        *head = (*head)->next;

        if (sorted == NULL || strcmp(current->Surname, sorted->Surname) < 0) {
            current->next = sorted;
            sorted = current;
        }
        else {
            ID* temp = sorted;
            while (temp->next != NULL && strcmp(temp->next->Surname, current->Surname) < 0)
                temp = temp->next;

            current->next = temp->next;
            temp->next = current;
        }
    }

    *head = sorted;
    printf("Lista je sortirana po prezimenima.\n");
}

// D. Upis liste u datoteku
void saveToFile(ID* head, const char* filename) {
    FILE* fp = fopen(filename, "w");
    if (!fp) {
        printf("Greska pri otvaranju datoteke!\n");
        return;
    }

    ID* temp = head;
    while (temp != NULL) {
        fprintf(fp, "%s %s %d\n", temp->Name, temp->Surname, temp->Year);
        temp = temp->next;
    }

    fclose(fp);
    printf("Lista je uspjesno upisana u datoteku '%s'.\n", filename);
}

// E. Èitanje liste iz datoteke
void loadFromFile(ID** head, const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("Greska pri otvaranju datoteke!\n");
        return;
    }

    // Oslobodi trenutnu listu
    while (*head != NULL) {
        ID* temp = *head;
        *head = (*head)->next;
        free(temp);
    }

    char name[20], surname[20];
    int year;

    while (fscanf(fp, "%s %s %d", name, surname, &year) == 3) {
        endPerson(head, name, surname, year);
    }

    fclose(fp);
    printf("Lista je uspjesno ucitana iz datoteke '%s'.\n", filename);
}

// MAIN
int main() {
    ID* list = NULL;
    int selection = 1;

    while (selection > 0) {
        printf(ANSI_COLOR_CYAN"\nUnesite sto zelite napraviti:\n"ANSI_COLOR_RESET);
        printf(" [1] Unos nove osobe na pocetak liste\n");
        printf(" [2] Unos nove osobe na kraj liste\n");
        printf(" [3] Ispis liste\n");
        printf(" [4] Trazenje osobe u listi\n");
        printf(" [5] Brisanje osobe sa liste\n");
        printf(" [6] Dodavanje osobe iza odredene osobe\n");
        printf(" [7] Dodavanje osobe ispred odredene osobe\n");
        printf(" [8] Sortiranje liste po prezimenima\n");
        printf(" [9] Upis liste u datoteku\n");
        printf(" [10] Citanje liste iz datoteke\n");
        printf(" [0] Izlaz\n> ");
        scanf("%d", &selection);

        char name[20], surname[20], target[20], filename[50];
        int year;

        switch (selection) {
        case 1:
            printf("Unesite ime, prezime i godinu rodenja: ");
            scanf("%s %s %d", name, surname, &year);
            startPerson(&list, name, surname, year);
            break;
        case 2:
            printf("Unesite ime, prezime i godinu rodenja: ");
            scanf("%s %s %d", name, surname, &year);
            endPerson(&list, name, surname, year);
            break;
        case 3:
            printf("\nVasa lista:\n");
            printList(list);
            break;
        case 4:
            printf("Unesite prezime trazene osobe: ");
            scanf("%s", surname);
            {
                ID* p = findPerson(list, surname);
                if (p)
                    printf(ANSI_COLOR_GREEN"Pronadena: %s %s (%d)\n"ANSI_COLOR_RESET, p->Name, p->Surname, p->Year);
                else
                    printf(ANSI_COLOR_RED"Nije pronadena.\n"ANSI_COLOR_RESET);
            }
            break;
        case 5:
            printf("Unesite prezime osobe za brisanje: ");
            scanf("%s", surname);
            deletePerson(&list, surname);
            break;
        case 6:
            printf("Unesite prezime osobe iza koje dodajete novu: ");
            scanf("%s", target);
            printf("Unesite ime, prezime i godinu rodenja nove osobe: ");
            scanf("%s %s %d", name, surname, &year);
            insertAfter(list, target, name, surname, year);
            break;
        case 7:
            printf("Unesite prezime osobe ispred koje dodajete novu: ");
            scanf("%s", target);
            printf("Unesite ime, prezime i godinu rodenja nove osobe: ");
            scanf("%s %s %d", name, surname, &year);
            insertBefore(&list, target, name, surname, year);
            break;
        case 8:
            sortList(&list);
            break;
        case 9:
            printf("Unesite ime datoteke: ");
            scanf("%s", filename);
            saveToFile(list, filename);
            break;
        case 10:
            printf("Unesite ime datoteke: ");
            scanf("%s", filename);
            loadFromFile(&list, filename);
            break;
        case 0:
            printf("Izlaz iz programa.\n");
            break;
        default:
            printf(ANSI_COLOR_RED"Pogresan unos!\n"ANSI_COLOR_RESET);
            break;
        }
    }

    // Oslobadanje memorije
    while (list != NULL) {
        ID* temp = list;
        list = list->next;
        free(temp);
    }

    return 0;
}
