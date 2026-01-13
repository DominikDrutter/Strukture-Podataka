//Zadan je niz brojeva 2, 5, 7, 8, 11, 1, 4, 2, 3, 7 koji su spremljeni u čvorove binarnog stabla.
// 
//      a) Napisati funkciju insert koja dodaje element u stablo tako da se pozivima te funkcije za
//      sve element zadanog niza brojeva stvori stablo kao na slici Slika 1. Funkcije vraća
//      pokazivač na korijen stabla.
// 
//      b) Napisati funkciju replace koja će svaki element stabla zamijeniti sumom elemenata u
//      njegovom lijevom i desnom podstablu (tj.sumom svih potomaka prije zamjene
//      vrijednosti u tim potomcima).Npr.stablo sa slike Slika 1 transformirat će se u stablo na
//      slici Slika 2.
// 
//      c) Prepraviti program na način da umjesto predefiniranog cjelobrojnog polja korištenjem
//      funkcije rand() generira slučajne brojeve u rasponu <10, 90>.Također, potrebno je
//      upisati u datoteku sve brojeve u inorder prolasku nakon korištenja funkcije iz a), zatim b)
//      dijela zadatka.



// Potrebne funkcije za zadatak;
// 1. Funkcija za oslobađanje memorije stabla
// 2. Funkcija za inorder ispis stabla u datoteku
// 3. Funkcija za zamjenu vrijednosti čvorova
// 4. Funkcija za umetanje čvora u binarno stablo
// 5. Funkcija za kreiranje novog čvora


#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>



typedef struct Node {                   // Definicija strukture čvora
	int data;                           // Podaci čvora
	struct Node* left, * right;			// Pokazivači na lijevog i desnog potomka    
} Node;

// 5. Funkcija za kreiranje novog čvora
Node* newNode(int value) {                                            // Kreiranje novog čvora
	Node* n = (Node*)malloc(sizeof(Node));                            // Alokacija memorije za novi čvor
	n->data = value;                                                  // Postavljanje vrijednosti čvora
	n->left = NULL;                                                   // Inicijalizacija lijevog potomka na NULL
	n->right = NULL;        			                              // Inicijalizacija desnog potomka na NULL  
    return n;
}

// 4. Funkcija za umetanje čvora u binarno stablo
Node* insert(Node* root, int value) {                           // Umetanje čvora u binarno stablo
    if (root == NULL)
        return newNode(value);

	Node* queue[100];                                           // Red za prlazak kroz stablo
    int front = 0, rear = 0;

	queue[rear++] = root;                                       // Inicijalizacija reda s korijenom stabla

	while (front < rear) {                                      // Prolazak kroz stablo
		Node* temp = queue[front++];                            // Uzimanje čvora iz reda

        if (temp->left == NULL) {
			temp->left = newNode(value);                        // Umetanje novog čvora kao lijevog djeteta
            break;
        }
        else {
			queue[rear++] = temp->left;                         // Dodavanje lijevog djeteta u red
        }

		if (temp->right == NULL) {                              // Umetanje novog čvora kao desnog djeteta
            temp->right = newNode(value);
            break;
        }
        else {
			queue[rear++] = temp->right;						// Dodavanje desnog djeteta u red       
        }
    }
    return root;
}

// 3. Funkcija za zamjenu vrijednosti čvorova
int replace(Node* root) {
    if (root == NULL)
        return 0;

	int leftSum = replace(root->left);                         // Rekurzivni poziv za lijevo podstablo
	int rightSum = replace(root->right);                       // Rekurzivni poziv za desno podstablo  

	int oldValue = root->data;                                 // Spremanje stare vrijednosti čvora
	root->data = leftSum + rightSum;						   // Zamjena vrijednosti čvora sumom djece

	return root->data + oldValue;					           // Vraćanje nove sume za roditelja
}

// 2. Funkcija za inorder ispis stabla u datoteku
int inorder(Node* root, FILE* f) {
    if (root == NULL)
        return;

	inorder(root->left, f);                 // Rekurzivni poziv za lijevo podstablo
	fprintf(f, "%d ", root->data);		    // Ispis vrijednosti čvora u datoteku  
	inorder(root->right, f);                // Rekurzivni poziv za desno podstablo
    return 0;
}

// 1. Funkcija za oslobađanje memorije stabla
int freeTree(Node* root) {
    if (root == NULL)
        return;

	freeTree(root->left);           // Rekurzivni poziv za lijevo podstablo
	freeTree(root->right);          // Rekurzivni poziv za desno podstablo
    free(root);                     // Pražnjenje Roota
    return 0;
}








int main() {
    Node* root = NULL;
    int n = 10;

	srand((unsigned)time(NULL));       // inicijalizacija generatora slučajnih brojeva

    for (int i = 0; i < n; i++) {
        int x = rand() % 81 + 10;   // raspon <10, 90>
		root = insert(root, x);     // umetanje u stablo
    }

	FILE* f = fopen("output.txt", "w");                 // otvaranje datoteke za pisanje
    if (f == NULL) {
        printf("Greska pri otvaranju datoteke!\n");
        return 1;
    }

    fprintf(f, "Inorder nakon insert:\n");          
	inorder(root, f);                               // pozivanje funkcije inorder za ispis
    fprintf(f, "\n");

	replace(root);                                  // pozivanje funkcije replace

    fprintf(f, "Inorder nakon replace:\n");
	inorder(root, f);                               // pozivanje funkcije inorder za ispis

    fclose(f);
	freeTree(root);                                 // oslobađanje memorije

    return 0;
}
