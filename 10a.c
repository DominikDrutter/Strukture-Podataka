#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 128






typedef struct _grad {					// Struktura za čvor stabla gradova
	char naziv[MAX];					// Naziv grada
	int stanovnika;						// Broj stanovnika grada	
	struct _grad* left;					// Pokazivač na lijevo podstablo
	struct _grad* right;			    // Pokazivač na desno podstablo
} Grad;

typedef struct _drzava {			    // Struktura za čvor liste država	
	char naziv[MAX];					// Naziv države
	Grad* gradovi;					    // Pokazivač na korijen stabla gradova
	struct _drzava* next;				// Pokazivač na sljedeću državu u listi
} Drzava;













// 7. Funkcija koja kreira novi čvor stabla gradova
Grad* createGrad(char* naziv, int br) {					   // Definicija funkcije createGrad koja prima naziv grada i broj stanovnika
	Grad* g = (Grad*)malloc(sizeof(Grad));			       // Alokacija memorije za novi čvor stabla gradova
	strcpy(g->naziv, naziv);							   // Kopiranje naziva grada u strukturu
	g->stanovnika = br;								       // Postavljanje broja stanovnika u strukturu
	g->left = g->right = NULL;							   // Inicijalizacija pokazivača na lijevo i desno podstablo na NULL
    return g;
}




// 6. Funkcija koja umeće grad u stablo gradova
Grad* insertGrad(Grad* root, char* naziv, int br) {                       // Definicija funkcije insertGrad koja prima korijen stabla gradova, naziv grada i broj stanovnika
    if (!root)
		return createGrad(naziv, br);								      // Ako je korijen NULL, kreiramo novi čvor sa zadatim nazivom i brojem stanovnika

	if (br < root->stanovnika ||										  // Umetanje u lijevo podstablo ako je broj stanovnika manji od trenutnog čvora
		(br == root->stanovnika && strcmp(naziv, root->naziv) < 0))		  // Ili ako je broj stanovnika isti, ali je naziv grada manji leksikografski
        root->left = insertGrad(root->left, naziv, br);					  // Rekurzivni poziv za lijevo podstablo
	else                                                                  // Umetanje u desno podstablo u suprotnom slučaju
		root->right = insertGrad(root->right, naziv, br);				  // Rekurzivni poziv za desno podstablo

    return root;
}



// 5. Funkcija koja ispisuje sve gradove iz stabla u rastućem redosljedu po broju stanovnika
int printGradovi(Grad* root) {										// Definicija funkcije printGradovi koja prima korijen stabla gradova
	if (!root) return;											    // Ako je korijen NULL, vraćamo se iz funkcije
	printGradovi(root->left);									    // Rekurzivni poziv za lijevo podstablo
	printf("    %s (%d)\n", root->naziv, root->stanovnika);      	// Ispis naziva grada i broja stanovnika	
	printGradovi(root->right);								        // Rekurzivni poziv za desno podstablo
	return 0;
}



// 4. Funkcija koja pretražuje stablo gradova i ispisuje one sa više stanovnika od unetog minimuma
int searchGradovi(Grad* root, int min) {                            // Definicija funkcije searchGradovi koja prima korijen stabla gradova i minimalan broj stanovnika
	if (!root) return;                                              // Ako je korijen NULL, vraćamo se iz funkcije
	searchGradovi(root->left, min);									// Rekurzivni poziv za lijevo podstablo               
	if (root->stanovnika > min) 								    // Provjera ima li grad više stanovnika od unetog minimuma						
		printf("    %s (%d)\n", root->naziv, root->stanovnika);		// Ispis naziva grada i broja stanovnika
	searchGradovi(root->right, min);								// Rekurzivni poziv za desno podstablo
    return 0;
}



// 3. Funkcija koja umeće državu u sortiranu listu država
Drzava* insertDrzava(Drzava* head, char* naziv) {       // Definicija funkcije insertDrzava koja prima glavu liste država i naziv nove države
	Drzava* d = (Drzava*)malloc(sizeof(Drzava));        // Alokacija memorije za novu državu
	strcpy(d->naziv, naziv);                            // Kopiranje naziva nove države u strukturu
	d->gradovi = NULL;								    // Inicijalizacija pokazivača na stablo gradova na NULL   
	d->next = NULL;                                     // Inicijalizacija pokazivača na sljedeću državu na NULL

	if (!head || strcmp(naziv, head->naziv) < 0) {      // Umetanje na početak liste ako je lista prazna ili ako je naziv nove države manji od naziva prve države u listi
		d->next = head;                                 // Povezivanje nove države sa trenutnom glavom liste
        return d;
    }

	Drzava* temp = head;								            // Pokazivač za prolazak kroz listu država   
	while (temp->next && strcmp(naziv, temp->next->naziv) > 0)      // Petlja za pronalaženje tačne pozicije za umetanje nove države
		temp = temp->next;                                          // Prelazimo na sledeću državu u listi

	d->next = temp->next;   				 // Povezivanje nove države sa sljedećom državom u listi 
	temp->next = d; 				         // Povezivanje prethodne države sa novom državom
    return head;
}





// 2. Funkcija koja pronalazi državu u listi po nazivu
Drzava* findDrzava(Drzava* head, char* naziv) {
	while (head && strcmp(head->naziv, naziv) != 0)         // Petlja koja prolazi kroz listu dok ne pronađe državu sa zadatim nazivom
		head = head->next;                                  // Prelazimo na sledeću državu u listi
    return head;
}




// 1. Funkcija koja učitava gradove iz datoteke i umeće ih u stablo gradova
int loadGradovi(char* file, Grad** root) {                      // Definicija funkcije loadGradovi koja prima naziv datoteke i pokazivač na korijen stabla gradova
	FILE* f = fopen(file, "r");								    // Otvaranje datoteke za čitanje 
    char grad[MAX];                                         
    int br;

	while (fscanf(f, " %[^,],%d", grad, &br) == 2)              // Učitavanje naziva grada i broja stanovnika iz datoteke
		*root = insertGrad(*root, grad, br);					// Umetanje grada u stablo, pozivanje funkcije insertGrad

	fclose(f);												    // Zatvaranje datoteke  
	return 0;
}






// Main

int main() {
	FILE* f = fopen("drzave.txt", "r");                   // Otvaranje datoteke drzave.txt
	char drzava[MAX], dat[MAX];                           // Varijable za naziv drzave i naziv datoteke gradova
	Drzava* head = NULL;                                  // Glava liste drzava

	while (fscanf(f, "%s %s", drzava, dat) == 2) {        // Učitavanje naziva države i datoteke gradova
		head = insertDrzava(head, drzava);                // Umetanje države u listu, pozivanje funkcije insertDrzava
		Drzava* d = findDrzava(head, drzava);             // Pronalaženje umetnute države, pozivanje funkcije findDrzava
		loadGradovi(dat, &d->gradovi);					  // Učitavanje gradova iz datoteke, pozivanje funkcije loadGradovi  
    }
	fclose(f);              // Zatvaranje datoteke drzave.txt

    printf("\nDRŽAVE I GRADOVI\n");
	for (Drzava* p = head; p; p = p->next) {        // Ispis država i njihovih gradova
		printf("%s\n", p->naziv);                       // Ispis naziva države
		printGradovi(p->gradovi);                       // Ispis gradova države, pozivanje funkcije printGradovi
    }

	char trazena[MAX];      // Varijabla za traženu državu
	int min;				// Varijabla za minimalan broj stanovnika 

	printf("\nUnesi drzavu: ");         // Unos naziva tražene države
    scanf("%s", trazena);
	printf("Min stanovnika: ");         // Unos minimalnog broja stanovnika
    scanf("%d", &min);

	Drzava* d = findDrzava(head, trazena);  // Pronalaženje tražene države
	if (d)                                  // Ako država postoji, ispis gradova sa više stanovnika od unetog minimuma  
		searchGradovi(d->gradovi, min);     // Pozivanje funkcije searchGradovi, funkcija prima korijen stabla i minimalan broj stanovnika
    else
		printf("Drzava ne postoji.\n");     // Ako država ne postoji, ispis poruke

    return 0;
}
