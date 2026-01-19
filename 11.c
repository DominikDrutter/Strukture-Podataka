#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 50
#define HASH_SIZE 11


// City Structure
typedef struct _city {			// Struktura za grad	
	char name[MAX_NAME];		// Ime grada
	int population;				// Populacija grada
	struct _city* left;			// Pokazivač na lijevo dijete u stablu
	struct _city* right;		// Pokazivač na desno dijete u stablu
} City;


// Country Structure
typedef struct _country {				// Struktura za državu
	char name[MAX_NAME];				// Ime države
	City* cityRoot;						// Pokazivač na korijen stabla gradova	
	struct _country* next;				// Pokazivač na sljedeću državu u lancu
} Country;	



// Hash Table
typedef struct {
    Country* buckets[HASH_SIZE];
} HashTable;



//Potrebne funkcije:
// 1. Free Hash Table, funkcija za oslobađanje memorije zauzete za hash tablicu
// 2. Search Function, funkcija za pretragu gradova po državi i minimalnoj populaciji
// 3. Insert Country Sorted, funkcija za umetanje države u hash tablicu na sortirani način
// 4. Read Cities From File, funkcija za čitanje gradova iz datoteke i umetanje u stablo
// 5. Hash Function, funkcija za izračunavanje hash vrijednosti na osnovu imena države
// 6. Free Cities, funkcija za oslobađanje memorije zauzete za stablo gradova
// 7. Print Cities, funkcija za ispis gradova iz stabla koji imaju populaciju veću od zadane vrijednosti
// 8. Insert City, funkcija za umetanje grada u stablo gradova
// 9. Create City, funkcija za kreiranje novog čvora grada




// 9. Create City, Funkcija za kreiranje novog čvora grada
City* createCity(char* name, int pop) {						// Funkcija prima ime grada i populaciju kao parametre
	City* newCity = (City*)malloc(sizeof(City));			// Alokacija memorije za novi čvor grada
	if (!newCity)											// Provjera je li alokacija memorije uspješna
        return NULL;	
	strcpy(newCity->name, name);							// Kopiranje imena grada u novi čvor	
	newCity->population = pop;								// Postavljanje populacije grada u novi čvor
	newCity->left = NULL;									// Inicijalizacija lijevog pokazivača na NULL
	newCity->right = NULL;									// Inicijalizacija desnog pokazivača na NULL		
	return newCity;											// Vraćanje pokazivača na novi čvor grada
}



// 8. Insert City, Funkcija za umetanje grada u stablo gradova
City* insertCity(City* root, char* name, int pop) {						// Funkcija prima korijen stabla gradova, ime grada i populaciju kao parametre
	if (root == NULL) {													// Provjera je li korijen stabla NULL
		return createCity(name, pop);									// Kreiranje novog čvora za grad ako je korijen NULL
    }

	if (pop < root->population) {										// Umetanje u lijevo podstablo ako je populacija manja od trenutnog čvora
		root->left = insertCity(root->left, name, pop);					// Rekurzivni poziv za lijevo podstablo
    }
	else if (pop > root->population) {									// Umetanje u desno podstablo ako je populacija veća od trenutnog čvora
		root->right = insertCity(root->right, name, pop);			    // Rekurzivni poziv za desno podstablo	
    }
    else {
		if (strcmp(name, root->name) < 0) {								// Umetanje u lijevo podstablo ako su populacije jednake, ali je ime manje
			root->left = insertCity(root->left, name, pop);				// Rekurzivni poziv za lijevo podstablo
        }
        else {
			root->right = insertCity(root->right, name, pop);			// Umetanje u desno podstablo ako su populacije jednake, ali je ime veće ili jednako
        }
    }
    return root;
}



// 7. Print Cities, Funkcija za ispis gradova iz stabla koji imaju populaciju veću od zadane vrijednosti
int printCities(City* root, int minPop) {										  // Funkcija prima korijen stabla gradova i minimalnu populaciju kao parametre                        
	if (root == NULL)   														  // Provjera je li korijen stabla NULL	
        return;
	printCities(root->left, minPop);											  // Rekurzivni poziv za lijevo podstablo
	if (root->population >= minPop) {											  // Provjera je li populacija grada veća ili jednaka minimalnoj populaciji	
		printf("\n  - %-15s | Populacija: %d", root->name, root->population);	  // Ispis imena grada i njegove populacije
    }
	printCities(root->right, minPop);											  // Rekurzivni poziv za desno podstablo	
    return 0;
}



// 6. Free Cities, Funkcija za oslobađanje memorije zauzete za stablo gradova
int freeCities(City* root) {            // Funkcija prima korijen stabla gradova kao parametar
	if (root == NULL)                   // Provjera je li korijen stabla NULL
        return;
	freeCities(root->left);			    // Rekurzivni poziv za lijevo podstablo        
	freeCities(root->right);		    // Rekurzivni poziv za desno podstablo
	free(root);                         // Oslobađanje memorije zauzete za trenutni čvor
    return 0;
}


// 5. Hash Function
int calculateHash(char* name) {                      // Funkcija za izračunavanje hash vrijednosti na osnovu imena države
	int sum = 0;                                     // Inicijalizacija sume na 0
	int i = 0;                                       // Brojač za petlju
	for (i = 0; i < 5 && name[i] != '\0'; i++) {     // Petlja kroz prvih 5 karaktera imena države ili do kraja stringa
		sum += name[i]; 						     // Dodavanje ASCII vrijednosti karaktera na sumu                       
    }
	return sum % HASH_SIZE;							 // Vraćanje hash vrijednosti kao ostatka pri dijeljenju s veličinom hash tabele
}



// 4. Read Cities From File, funkcija za čitanje gradova iz datoteke i umetanje u stablo
City* readCitiesFromFile(char* filename) {                          // Funkcija prima ime datoteke kao parametar
	FILE* fp = fopen(filename, "r");    			                // Otvaranje datoteke za čitanje         
	City* root = NULL;							            		// Inicijalizacija korijena stabla gradova na NULL
	char name[MAX_NAME];                                            // Varijabla za ime grada
	int pop = 0;    									            // Varijabla za populaciju grada

	if (!fp)                                                        // Provjera je li datoteka uspješno otvorena
        return NULL;

	while (fscanf(fp, " %[^,], %d", name, &pop) == 2) { 			// Čitanje imena grada i populacije iz datoteke
		root = insertCity(root, name, pop); 				    	// Umetanje grada u stablo
    }
	fclose(fp); 												    // Zatvaranje datoteke 
    return root;
}



// 3. Insert Country Sorted, funkcija za umetanje države u hash tablicu na sortirani način
int insertCountrySorted(HashTable* ht, char* countryName, char* fileName) {                         // Funkcija prima pokazivač na hash tablicu, ime države i ime datoteke kao parametre
	int index = calculateHash(countryName); 										                // Izračunavanje indeksa hash tabele za uneseno ime države        
	Country* newCountry = (Country*)malloc(sizeof(Country));    		                        	// Alokacija memorije za novu državu                       
	Country** current = &(ht->buckets[index]);  								                	// Pokazivač na pokazivač na trenutnu državu u bucket-u                        

	if (!newCountry)                // Provjera je li alokacija memorije uspješna
        return;

	strcpy(newCountry->name, countryName);                                          // Kopiranje imena države u novu državu
	newCountry->cityRoot = readCitiesFromFile(fileName);    	            		// Učitavanje gradova iz datoteke u stablo gradova nove države 
	newCountry->next = NULL;                                                        // Inicijalizacija pokazivača na sljedeću državu na NULL

	while (*current != NULL && strcmp((*current)->name, countryName) < 0) { 	    // Pronalaženje ispravne pozicije za umetanje nove države u sortirani redoslijed
		current = &((*current)->next);                                              // Prijelaz na sljedeću državu u bucket-u
    }
	newCountry->next = *current;													// Umetanje nove države na ispravno mjesto    
	*current = newCountry;                                                          // Ažuriranje pokazivača na početak bucket-a ili prethodnu državu
    return 0;
}



// 2. Search Function
int search(HashTable* ht) {                                                     // Funkcija za pretragu gradova po državi i minimalnoj populaciji, funkcija prima pokazivač na hash tablicu kao parametar
	char target[MAX_NAME];                                                      // Varijabla za unos imena države
	int minPop = 0; 													    	// Varijabla za unos minimalne populacije             
	int index = 0;                                                              // Varijabla za indeks hash tabele                          
	Country* curr = NULL;   					            		   		    // Pokazivač na trenutnu državu u hash tablici                             

    printf("\n\n--- PRETRAGA ---");
    printf("\nUnesite ime drzave: ");
    scanf("%s", target);
    printf("Minimalan broj stanovnika: ");
    scanf("%d", &minPop);

	index = calculateHash(target);                                             // Izračunavanje indeksa hash tabele za uneseno ime države
	curr = ht->buckets[index];  								           	   // Postavljanje pokazivača na prvu državu u odgovarajućem bucket-u                       

	while (curr != NULL) {											                      // Petlja kroz sve države u trenutnom bucket-u
		if (strcmp(curr->name, target) == 0) {  							              // Provjera je li trenutna država ona koja se traži               
			printf("\nGradovi u %s s populacijom vecom od %d:", target, minPop);          // Ispis rezultata pretrage
			printCities(curr->cityRoot, minPop);                                          // Ispis gradova u državi s populacijom većom od unesenog minimuma
            return;
        }
		curr = curr->next;  									                      // Prijelaz na sljedeću državu u bucket-u         
    }
	printf("\nDrzava '%s' nije pronadjena.", target);                                 // Ispis poruke ako država nije pronađena
    return 0;
}



// 1. Free Hash Table
int freeHashTable(HashTable* ht) {                  // Oslobađanje memorije zauzete za hash tablicu
	int i = 0;                                      // Brojač za petlju
	for (i = 0; i < HASH_SIZE; i++) {   	     	// Petlja kroz sve bucket-e hash tabele        
		Country* curr = ht->buckets[i]; 	 		// Pokazivač na trenutnu državu u bucket-u
		while (curr != NULL) {  			        // Petlja kroz sve države u trenutnom bucket-u     
			Country* temp = curr;                   // Privremeni pokazivač na trenutnu državu
			curr = curr->next;                      // Prijelaz na sljedeću državu
			freeCities(temp->cityRoot); 	    	// Oslobađanje memorije zauzete za stabla gradova    
			free(temp);							    // Oslobađanje memorije zauzete za državu  
        }
		ht->buckets[i] = NULL;					    // Postavljanje bucket-a na NULL nakon oslobađanja   
        return 0;
    }
}







// Main
int main() {
	HashTable ht;                                                               // Inicijalizacija hash tabele
	int i = 0;                                                                  // Brojac za petlje
	FILE* fp = NULL;                                                            // Pokazivac na datoteku
	char countryName[MAX_NAME], fileName[MAX_NAME];                             // Imena drzave i datoteke

	for (i = 0; i < HASH_SIZE; i++) {                                           // Inicijalizacija hash tabele
		ht.buckets[i] = NULL;                                                   // Postavljanje svakog bucket-a na NULL
    }

	fp = fopen("drzave.txt", "r");                                              // Otvaranje datoteke drzave.txt
	if (!fp) {                                                                  // Provjera je li datoteka uspješno otvorena
        printf("Greska: Nije moguce otvoriti datoteku drzave.txt\n");
        return 1;
    }

	while (fscanf(fp, "%s %s", countryName, fileName) == 2) {                   // Čitanje imena drzave i datoteke iz datoteke
		insertCountrySorted(&ht, countryName, fileName);					    // Umetanje drzave u hash tablicu
    }
	fclose(fp);                                                                 // Zatvaranje datoteke

	printf("--- ISPIS SVIH DRZAVA I GRADOVA ---\n");                            // Ispis svih država i njihovih gradova
	for (i = 0; i < HASH_SIZE; i++) {                                           // Petlja kroz sve bucket-e hash tabele
		Country* curr = ht.buckets[i];                                          // Pokazivač na trenutnu državu u bucket-u
		while (curr) {                                                          // Petlja kroz sve države u trenutnom bucket-u
			printf("\nDRZAVA: %s (Index: %d)", curr->name, i);                  // Ispis imena države i njenog indeksa
			printCities(curr->cityRoot, 0); 								    // Ispis svih gradova u državi                                
			curr = curr->next;          			                            // Prijelaz na sljedeću državu u bucket-u                             
        }
    }

	search(&ht);                                                                // Poziv funkcije za pretragu gradova po državi i minimalnoj populaciji
	freeHashTable(&ht);                                                         // Oslobađanje memorije zauzete za hash tablicu                 

    return 0;
}
