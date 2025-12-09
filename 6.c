#define _CRT_SECURE_NO_WARNINGS
#define WORD 128
#define LINE 1024
#define ANSI_COLOR_BLUE    "\x1b[36m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>



//STRUKTURA UNUTAR RACUNA
typedef struct ArticleNode {
	char name[WORD];
	int quantity;
	double price;
	struct ArticleNode* next;
}ArticleNode;

//STRUKTURA IZVAN RACUNA
typedef struct ReceiptNode {
	char date[11];					//4 zanmenke za godinu, 2 za mjesec, 2 za dan i 2 za razmak
	struct ReceiptNode* next;
	struct ArticleNode* articleHead;
}ReceiptNode;



FILE* openFile(char name[WORD]);



//FUKCIJA ZA PRETRAZIVANJE ODREDENOG PROIZVODA
int search(ReceiptNode* head, char date1[WORD], char date2[WORD], char name[WORD], double article[2]) {
	article[0] = 0;
	article[1] = 0;
	ReceiptNode* tmp = head;

	while (tmp) {
		if ((strcmp(tmp->date, date1) >= 0) && (strcmp(tmp->date, date2) <= 0)) {
			ArticleNode* temp = tmp->articleHead;

			while (temp) {
				if (!strcmp(name, temp->name)) {
					article[0] += temp->price * temp->quantity;
					article[1] += temp->quantity;
				}
				temp = temp->next;
			}
		}
		tmp = tmp->next;
	}

	if (article[0] != 0 && article[1] != 0) {
		printf("\n\n============================================\n");
		printf(ANSI_COLOR_GREEN"Cijena: %.2lf eur\n"ANSI_COLOR_RESET, article[0]);
		printf(ANSI_COLOR_GREEN"Kolicina: %.lf kom.\n"ANSI_COLOR_RESET, article[1]);
		return 1;
	}
	else {
		return 0;
	}
}



//FUNKCIJA ZA PRAZNJENJE MEMORIJE RACUNA
int freeMemReceipt(ReceiptNode* head) {
	while (head) {
		ReceiptNode* tmp = head;
		freeMemArticle(head->articleHead);
		head = head->next;
		free(tmp);
	}
	return 0;
}



//FUNKCIJA ZA PRAZNJENJE MEMORIJE PROIZOVDA
int freeMemArticle(ArticleNode* head) {
	while (head) {
		ArticleNode* tmp = head;
		head = head->next;
		free(tmp);
	}
	return 0;
}

ArticleNode* sortName(ArticleNode* head, ArticleNode* newNode) {
	if (head == NULL) {
		newNode->next = head;
		head = newNode;
		return head;
	}
	else {
		ArticleNode* tmp = head;
		ArticleNode* prev = NULL;

		while (tmp) {
			if (strcmp(newNode->name, tmp->name) < 0) {
				if (prev == NULL) {
					newNode->next = head;
					head = newNode;
					return head;
				}
				else {
					newNode->next = prev->next;
					prev->next = newNode;
					return head;
				}
			}
			if (strcmp(newNode->name, tmp->name) > 0) {
				prev = tmp;
				tmp = tmp->next;
				if (tmp == NULL) {
					newNode->next = tmp;
					prev->next = newNode;
					return head;
				}
			}
		}
	}
}

ReceiptNode* sortDate(ReceiptNode* head, ReceiptNode* newNode) {
	if (head == NULL) {
		newNode->next = head;
		head = newNode;
		return head;
	}
	else {
		ReceiptNode* tmp = head;
		ReceiptNode* prev = NULL;

		while (tmp) {
			if (strcmp(newNode->date, tmp->date) <= 0) {
				if (prev == NULL) {
					newNode->next = head;
					head = newNode;
					return head;
				}
				else {
					newNode->next = prev->next;
					prev->next = newNode;
					return head;
				}
			}
			if (strcmp(newNode->date, tmp->date) > 0) {
				prev = tmp;
				tmp = tmp->next;
				if (tmp == NULL) {
					newNode->next = tmp;
					prev->next = newNode;
					return head;
				}
			}
		}
	}
}

ReceiptNode* readListFile(ReceiptNode* head, FILE* fpLR) {
	char line[LINE];

	while (fgets(line, LINE, fpLR) != NULL) {
		ReceiptNode* newNode = malloc(sizeof(ReceiptNode));
		if (newNode == NULL) {
			return NULL;
		}

		int len = strlen(line);
		if (len > 0 && line[len - 1] == '\n') {
			line[len - 1] = '\0';
		}
		FILE* fp = openFile(line);
		if (!fp) continue;

		char date[WORD];
		fgets(date, WORD, fp);
		len = strlen(date);
		if (len > 0 && date[len - 1] == '\n') {
			date[len - 1] = '\0';
		}

		strcpy(newNode->date, date);
		newNode->articleHead = NULL;

		head = sortDate(head, newNode);

		char article[WORD], buffer[LINE];
		int quantity;
		double price;

		while (fgets(buffer, LINE, fp) != NULL) {
			ArticleNode* newArticle = malloc(sizeof(ArticleNode));
			if (newArticle == NULL) {
				return NULL;
			}

			len = strlen(buffer);
			if (len > 0 && buffer[len - 1] == '\n') {
				buffer[len - 1] = '\0';
			}

			sscanf(buffer, "%[^,],%d,%lf", article, &quantity, &price);
			strcpy(newArticle->name, article);
			newArticle->quantity = quantity;
			newArticle->price = price;

			newNode->articleHead = sortName(newNode->articleHead, newArticle);
		}

		closeFile(fp);
	}

	return head;
}



//OTVARANJE DATOTEKE
FILE* openFile(char name[WORD]) {
	FILE* fp = fopen(name, "r");
	if (!fp) {
		printf(ANSI_COLOR_RED"\nNeuspjesno otvaranje datoteke"ANSI_COLOR_RESET);
		return NULL;
	}
	else {
		return fp;
	}
}




//ZATVARANJE DATOTEKE
int closeFile(FILE* fp) {
	fclose(fp);
	return 0;
}


//MAIN
int main() {

	ReceiptNode* head = NULL;

	FILE* fpLR = openFile("racuni.txt");

	head = readListFile(head, fpLR);

	double article[2] = { 0 };
	char name[WORD], date1[WORD], date2[WORD];

	printf(ANSI_COLOR_BLUE"Unesite artikl koji zelite traziti: \n" ANSI_COLOR_RESET);
	scanf("%s", name);
	printf("\n\nUnesite datum od kojeg zelite pretrazivati: ");
	scanf("%s", date1);
	printf("Unesite datum do kojeg zelite pretrazivati: ");
	scanf("%s", date2);

	if (!search(head, date1, date2, name, article)) {
		printf(ANSI_COLOR_RED"\nNije pronaden artikl koji zadovoljava uvjete!"ANSI_COLOR_RESET);
	}

	closeFile(fpLR);
	freeMemReceipt(head);

	return 0;
}

