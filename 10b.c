#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 128



// Struktura za grad u povezanoj listi
typedef struct _grad {									// Struktura za grad u povezanoj listi
	char naziv[MAX];									// Naziv grada
	int stanovnika;										// Broj stanovnika
	struct _grad* next;									// Pokazivač na sljedeći grad u listi						
} Grad;




// Struktura za državu u binarnom stablu
typedef struct _drzava {                    // Struktura za državu u binarnom stablu
	char naziv[MAX];					    // Naziv države
	Grad* gradovi;						    // Pokazivač na povezanu listu gradova						
	struct _drzava* left;					// Pokazivač na lijevo podstablo
	struct _drzava* right;				    // Pokazivač na desno podstablo			
} Drzava;




// Za ovaj zadatak potrebno je implementirati sledeće funkcije:
// 1. Funkcija koja učitava gradove iz datoteke i umeće ih u povezanu listu gradova
// 2. Funkcija koja ispisuje sve države i njihove gradove
// 3. Funkcija koja pronalazi državu u stablu po nazivu
// 4. Funkcija koja umeće državu u stablo po nazivu
// 5. Funkcija koja pretražuje gradove sa više stanovnika od unesenog minimuma
// 6. Funkcija koja ispisuje sve gradove iz povezane liste
// 7. Funkcija koja umeće grad u povezanu listu po broju stanovnika i nazivu
// 8. Funkcija koja kreira novi grad






//8. Funkcija koja kreira novi grad
Grad* createGrad(char* naziv, int br) {				// Kreiranje novog grada, funkcija prima naziv i broj stanovnika kao parametre
	Grad* g = (Grad*)malloc(sizeof(Grad));			// Alokacija memorije za novi grad	
	strcpy(g->naziv, naziv);					    // Kopiranje naziva grada u strukturu
	g->stanovnika = br;							    // Postavljanje broja stanovnika	
	g->next = NULL;								    // Inicijalizacija pokazivača na sljedeći grad kao NULL	
	return g;										// Povratak pokazivača na novi grad
}




// 7. Funkcija koja umeće grad u povezanu listu po broju stanovnika i nazivu
Grad* insertGrad(Grad* head, char* naziv, int br) {								// Umetanje grada u povezanu listu po broju stanovnika i nazivu
	Grad* g = createGrad(naziv, br);											// Kreiranje novog grada
															
	if (!head || br > head->stanovnika ||								        // Ako je lista prazna ili novi grad ima više stanovnika od prvog grada
		(br == head->stanovnika && strcmp(naziv, head->naziv) < 0)) {			// Ili ako novi grad ima isti broj stanovnika kao prvi, ali je naziv manji
		g->next = head;															// Umetanje novog grada na početak liste
        return g;
    }

	Grad* temp = head;															// Privremeni pokazivač za prolazak kroz listu
	while (temp->next &&														// Prolazak kroz listu dok ne nađemo odgovarajuće mjesto za umetanje
		(temp->next->stanovnika > br ||											// Novi grad ima manje stanovnika od trenutnog
			(temp->next->stanovnika == br &&									// Novi grad ima isti broj stanovnika kao trenutni, ali je naziv veći
				strcmp(temp->next->naziv, naziv) < 0)))							// Nastavak uslova za prolazak kroz listu
		temp = temp->next;														// Prelazak na sljedeći grad u listi								
		
	g->next = temp->next;														// Umetanje novog grada na odgovarajuće mjesto	
	temp->next = g;																// Povezivanje prethodnog grada sa novim gradom
	return head;																// Povratak glave liste
}	






// 6. Funkcija koja ispisuje sve gradove iz povezane liste
void printGradovi(Grad* head) {  									         // Ispis svih gradova iz povezane liste                                
    while (head) {                                                          
		printf("    %s (%d)\n", head->naziv, head->stanovnika);				 // Ispis naziva grada i broja stanovnika
		head = head->next;													 // Prelazak na sljedeći grad u listi
    }
}



// 5. Funkcija koja pretražuje gradove sa više stanovnika od unesenog minimuma
void searchGradovi(Grad* head, int min) {                                            // Pretraživanje gradova sa više stanovnika od unesenog minimuma
    while (head) {
		if (head->stanovnika > min)                                                 // Ako grad ima više stanovnika od minimuma, ispisuje se
			printf("    %s (%d)\n", head->naziv, head->stanovnika);                 // Ispis naziva grada i broja stanovnika
		head = head->next;
    }
}




// 4. Funkcija koja umeće državu u stablo po nazivu
Drzava* insertDrzava(Drzava* root, char* naziv) {           // Umetanje države u stablo po nazivu
	if (!root) {                                            // Ako je čvor NULL, kreira se novi čvor
		Drzava* d = (Drzava*)malloc(sizeof(Drzava));        // Alokacija memorije za novi čvor
		strcpy(d->naziv, naziv);                            // Kopiranje naziva države
		d->gradovi = NULL;      	                    	// Inicijalizacija pokazivača na listu gradova   
		d->left = d->right = NULL;  			    	    // Inicijalizacija lijevog i desnog podstabla              
        return d;
    }

	if (strcmp(naziv, root->naziv) < 0)                     // Ako je naziv države manji od naziva čvora, ide se lijevo
		root->left = insertDrzava(root->left, naziv);       // Inače, ide se desno
	else if (strcmp(naziv, root->naziv) > 0)                // Ako je naziv države veći od naziva čvora, ide se desno
		root->right = insertDrzava(root->right, naziv);     // Povratak korijena stabla

    return root;
}



// 3. Funkcija koja pronalazi državu u stablu po nazivu
Drzava* findDrzava(Drzava* root, char* naziv) {              // Pronalaženje države po nazivu
	if (!root) return NULL;                                  // Baza slučaja: ako je čvor NULL, vraća se NULL
	if (strcmp(naziv, root->naziv) == 0) return root;	     // Ako je naziv države jednak nazivu čvora, vraća se čvor
	if (strcmp(naziv, root->naziv) < 0)                      // Ako je naziv države manji od naziva čvora, traži se u lijevom podstablu
		return findDrzava(root->left, naziv);                // Inače, traži se u desnom podstablu
	return findDrzava(root->right, naziv);                   // Povratak rezultata iz desnog podstabla
}


// 2. Funkcija koja ispisuje sve države i njihove gradove
int printDrzave(Drzava* root) {                             // Ispis svih država i njihovih gradova
	if (!root) return;                                      // Baza slučaja: ako je čvor NULL, vraća se
	printDrzave(root->left);                                // Rekurzivni poziv za lijevo podstablo
	printf("%s\n", root->naziv);                            // Ispis naziva države
	printGradovi(root->gradovi);    			            // Ispis gradova države    
	printDrzave(root->right);							    // Rekurzivni poziv za desno podstablo
	return 0;   
}




// 1. Funkcija koja učitava gradove iz datoteke i umeće ih u povezanu listu gradova
int loadGradovi(char* file, Grad** head) {                                              // Učitavanje gradova iz datoteke
	FILE* f = fopen(file, "r");                                                         // Otvaranje datoteke
	char grad[MAX];                                                                     // Varijabla za naziv grada
	int br;         										                            // Varijabla za broj stanovnika                                    

	while (fscanf(f, " %[^,],%d", grad, &br) == 2)  							        // Učitavanje gradova i njihovih stanovnika iz datoteke                    
		*head = insertGrad(*head, grad, br);									    	// Umetanje grada u povezanu listu

	fclose(f);  																        // Zatvaranje datoteke
    return 0;
}








// Main
int main() {
	FILE* f = fopen("drzave.txt", "r");                     // Otvaranje datoteke sa državama
	char drzava[MAX], dat[MAX];                             // Varijable za državu i datoteku sa gradovima
	Drzava* root = NULL;                                    // Korijen stabla država

	while (fscanf(f, "%s %s", drzava, dat) == 2) {          // Učitavanje država i njihovih gradova
		root = insertDrzava(root, drzava);                  // Umetanje države u stablo
		Drzava* d = findDrzava(root, drzava);               // Pronalaženje države u stablu
		loadGradovi(dat, &d->gradovi);                      // Učitavanje gradova iz datoteke   
    }
	fclose(f);                                              // Zatvaranje datoteke sa državama

	printf("\nDRZAVE I GRADOVI\n");                 // Ispis svih država i njihovih gradova
    printDrzave(root);                                      

	char trazena[MAX];                                      // Varijabla za traženu državu                 
	int min;											    // Varijabla za minimalan broj stanovnika

	printf("\nUnesi drzavu: ");                             // Unos tražene države
	scanf("%s", trazena);                                   
	printf("Min stanovnika: ");                             // Unos minimalnog broja stanovnika
    scanf("%d", &min);

	Drzava* d = findDrzava(root, trazena);                  // Pronalaženje tražene države, poziv funkcije findDrzava
    if (d)
		searchGradovi(d->gradovi, min);                     // Poziv funkcije searchGradovi za ispis gradova sa više stanovnika od unesenog minimuma
    else
        printf("Drzava ne postoji.\n");

    return 0;
}
