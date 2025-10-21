#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"





// Definiranje strukture sa imenom, prezimenom i godinom rodenja

typedef struct ID {				// Moramo napisati ime strukture ispred zagrade jer se referencira ispred zagrade
	char Name[20];
	char Surname[20];
	int Year;
	struct ID* next;			// Povezivanje clanova u listu
} ID;





// Funkcija za unos nove osobe, koristimo je unutar funkcija za dodavanje na pocetak i kraj liste

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





// Funkcija za dodavanje na pocetku liste

int startPerson(ID** head, const char* Name, const char* Surname, int Year) {

	ID* new = newPerson(Name, Surname, Year);
	if (!new) return;

	new->next = *head;
	*head = new;
	}






// Funkcija za dodavanje na kraj liste

int endPerson(ID** head, const char* Name, const char* Surname, int Year) {

	ID* new = newPerson(Name, Surname, Year);
	if (!new) return;

	if (*head == NULL) {
		*head = new;
		return;
	}

	ID* temp = *head;
	while (temp->next != NULL)
		temp = temp->next;

	temp->next = new;
}





// Funkcija za ispis liste

int printList(ID* head) {
	
	ID* temp = head;
	if (!temp) {
		printf("Lista je prazna.\n");
		return;
	}

	while (temp != NULL) {
		printf("%s %s (%d)\n", temp->Name, temp->Surname, temp->Year);
		temp = temp->next;
	}
}






// Funkcija za pretrazivanje

ID* findPerson(ID* head, const char* Surname) {

	ID* temp = head;
	while (temp != NULL) {
		if (strcmp(temp->Surname, Surname) == 0)
			return temp;
		temp = temp->next;
	}
	return NULL;
}





// Brise elemente iz liste

int deletePerson(ID** head, const char* Surname) {
	ID* temp = *head;
	ID* before = NULL;

	while (temp != NULL && strcmp(temp->Surname, Surname) != 0) {
		before = temp;
		temp = temp->next;
	}

	if (temp == NULL) {
		printf("Osoba s prezimenom '%s' nije pronadena.\n", Surname);
		return;
	}

	if (before == NULL)
		*head = temp->next;  // brisanje prvog elementa
	else
		before->next = temp->next;

	free(temp);
	printf("Osoba s prezimenom '%s' je izbrisana.\n", Surname);
	return;
}









int main() {

	ID* list = NULL;

	int selection = 1;

	while (selection > 0 && selection < 6) {

		printf(ANSI_COLOR_CYAN"Unesite sto zelite napraviti;\n"ANSI_COLOR_RESET);
		printf(" [1] Unos nove osobe na pocetak liste\n [2] Unos nove osobe na kraj liste\n [3] Ispis liste\n [4] Trazenje osobe u listi\n [5] Brisanje osobe sa liste\n");
		scanf("%d", &selection);


		if (selection == 1) {
			printf("Unesite ime;");
			char name1[20];
			scanf("%s", &name1);
			printf("Unesite prezime;");
			char surname1[20];
			scanf("%s", &surname1);
			printf("Unesite godinu rodenja;");
			int year1;
			scanf("%d", &year1);
			printf("\n");


			startPerson(&list, name1, surname1, year1);
		};

		if (selection == 2) {
			printf("Unesite ime;");
			char name1[20];
			scanf("%s", &name1);
			printf("Unesite prezime;");
			char surname1[20];
			scanf("%s", &surname1);
			printf("Unesite godinu rodenja;");
			int year1;
			scanf("%d", &year1);
			printf("\n");


			endPerson(&list, name1, surname1, year1);
		};

		if (selection == 3) {
			printf("\nVasa Lista:\n");
			printList(list);
			printf("\n");
		};

		if (selection == 4) {
			printf("\nUnesite prezime osobe koju trazite:\n");
			char str[20];
			scanf("%s", &str);
			ID* trazena = findPerson(list, str);
			if (trazena)
				printf(ANSI_COLOR_GREEN"Pronadena: %s %s (%d)\n"ANSI_COLOR_RESET, trazena->Name, trazena->Surname, trazena->Year);
			else
				printf("Nije pronadena.\n");
			printf("\n");

		};

		if (selection == 5) {
			printf("\nUnesite prezime osobe koju zelize izbrisati\n");
			char str[20];
			scanf("%s", &str);
			deletePerson(&list, str);;
			printf("\n");
		};

		if (selection > 5 || selection < 1) {
			printf(ANSI_COLOR_RED"Pogresan unos!\n"ANSI_COLOR_RESET);
		};

	};



	// Oslobaðanje memorije
	while (list != NULL) {
		ID* temp = list;
		list = list->next;
		free(temp);
	}

	return 0;

}