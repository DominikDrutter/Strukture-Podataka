#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



// Definiranje strukture

typedef struct Node {
	int coef;
	int exp;
	struct Node* next;
} Node;


// ZA OVAJ ZADATAK POTREBNE SU:
// 1. Funkcija koja opadajuci sortira eksponente po clanovima i spaja ih ako jednaki eksponent vec postoji
// 2. Funkcija za citanje polinoma iz datoteke
// 3. Funkcija za ispis polinoma
// 4. Funkcija za zbrajanje polinoma
// 5. Funkcija za mnozenje polinoma
// 6. Funkcija za oslobadanje memorije





// 1. Funkcija koja opadajuci sortira eksponente po clanovima i spaja ih ako jednaki eksponent vec postoji

int Sort(Node** head, int coef, int exp) {

	if (coef == 0) return;

	Node* NewNode = (Node*)malloc(sizeof(Node));
	NewNode->coef = coef;
	NewNode->exp = exp;
	NewNode->next = NULL;

	Node** curr = head;

	while (*curr != NULL && (*curr)->exp > exp)
		curr = &((*curr)->next);

	if (*curr != NULL && (*curr)->exp == exp) {
		(*curr)->coef += coef;
		free(NewNode);
		if ((*curr)->coef == 0) {
			Node* temp = *curr;
			*curr = (*curr)->next;
			free(temp);
		}
	}
	else {
		NewNode->next = *curr;
		*curr = NewNode;
	}
	return EXIT_SUCCESS;
}



// 2. Funkcija za citanje polinoma iz datoteke

int ReadPoly(FILE* f) {
	Node* p = NULL;
	int coef, exp;
	while (fscanf(f, "%d %d", &coef, &exp) == 2) 
		Sort(&p, coef, exp);
	return p;
}



// 3. Funkcija za ispis polinoma

int PrintPoly(Node* p) {
	if (!p) { printf("0\n"); return; }
	while (p) {
		printf("%dX^%d", p->coef, p->exp);
		if (p->next && p->next->coef >= 0) printf(" + ");
		else if (p->next) printf(" ");
		p = p->next;
	}
	printf("\n");
	return EXIT_SUCCESS;
}



// 4. Funkcija za zbrajanje polinoma

Node* AddPoly(Node* a, Node* b) {
	Node* Result = NULL;
	while (a) { Sort(&Result, a->coef, a->coef); a = a->next; }
	while (b) { Sort(&Result, b->coef, b->coef); b = b->next; }
	return Result;
}



// 5. Funkcija za mnozenje polinoma

Node* MultiPoly(Node* a, Node* b) {
	Node* Result = NULL;
	for (Node* p = a; p != NULL; p = p->next) {
		for (Node* q = b; q != NULL; q = q->next) {
			Sort(&Result, p->coef * q->coef, p->exp + q->exp);
		}
	}
	return Result;
}



// 6. Funkcija za oslobadanje memorije

int FreePoly(Node* p) {
	while (p) {
		Node* t = p;
		p = p->next;
		free(t);
	}
	return EXIT_SUCCESS;
}



// Main

int main() {

	FILE* f1 = fopen("polinomi1.txt", "r");
	FILE* f2 = fopen("polinomi2.txt", "r");

	//Provjera jesu li datoteke ispravno otvoren
	if (!f1) { printf("Pogreska pri otvaranju datoteke polinomi1.txt!\n"); return 1; }
	if (!f2) { printf("Pogreska pri otvaranju datoteke polinomi2.txt!\n"); return 1; }


	//Citanje prvog i drugog polinoma
	Node* Poly1 = ReadPoly(f1);
	Node* Poly2 = ReadPoly(f2);

	fclose(f1);
	fclose(f2);

	//Ispis polinoma prije izvodenja zbrajanja i mnozenja
	printf("Prvi polinom:\n"); PrintPoly(Poly1);
	printf("Drugi polinom:\n"); PrintPoly(Poly2);

	//Zbrajanje i mnozenje polinoma
	Node* Sum = AddPoly(Poly1, Poly2);
	Node* Product = MultiPoly(Poly1, Poly2);

	//Ispis zbroja i umnoska polinoma
	printf("Zbroj polnoma:\n"); PrintPoly(Sum);
	printf("Drugi polnom:\n"); PrintPoly(Product);

	//Brisanje memorije
	FreePoly(Poly1);
	FreePoly(Poly2);
	FreePoly(Sum);
	FreePoly(Product);

	return 0;
}
