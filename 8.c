//8. Napisati program koji omogućava rad s binarnim stablom pretraživanja.
//Treba omogućiti unošenje novog elementa u stablo, ispis elemenata(inorder, preorder, postorder ilevel order), brisanje i pronalaženje nekog elementa.

#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>



// Potrebne funkcije za zadatak;
// 1. Kreiranje novog člnana
// 2. Pronalaženje elementa
// 3. Ubacivanje elementa u Binarno Stablo
// 4. Inorder ispis
// 5. Preorder ispis
// 6. Postorder ispis
// 7. Level ispis
// 8. Brisanje elementa
// 9. Pronalaženje najmanjeg elementa (pomoćna funkcija za brisanje)
// 10. Visina (pomoćna funkcija za level ispis)
// 11. Main  



// Definicija strukture čvora stabla
typedef struct Node {
	int data;             //Vrijednost čvora
    struct Node* left;    //Pokazivač na lijevu stranu koja je manja od roditelja
    struct Node* right;   //Pokazivač na desnu stranu koja je veća ili jednaka roditelju
} Node;

// 1. Kreiranje novog čvora stabla
Node* createNode(int value) {
	Node* newNode = (Node*)malloc(sizeof(Node));  // Alokacija memorije za novi čvor
    if (newNode == NULL) {
        printf("Greska pri alokaciji memorije!\n");
        return NULL;
    }                                                   // Prvojera je li alokacija uspješna
    newNode->data = value;              //Pridružujemo vrijednost čvoru
	newNode->left = NULL;               // Inicijalizacija lijevog djeteta na NULL
	newNode->right = NULL;			    // Inicijalizacija desnog djeteta na NULL
    return newNode;
}

// 3. Ubacivanje elementa u Binarno Stablo
Node* insert(Node* root, int value) {       // Funkcija prima korijen stabla i vrijednost za ubacivanje
	if (root == NULL)                       //Ako je korijen NULL, znači da je stablo prazno, pa stvaramo NODE
        return createNode(value);

	if (value < root->data)                         //Ako je vrijednost manja od trenutnog čvora, idemo lijevo
        root->left = insert(root->left, value); 
	else if (value > root->data)					  //Ako je vrijednost veća ili jednaka od trenutnog čvora, idemo desno  
        root->right = insert(root->right, value);

    return root;
}

// 2. Pronalaženje elementa 
int search(Node* root, int value) {
    if (root == NULL)
        return 0;
	if (root->data == value)                    //Ako je vrijednost u čvoru jednaka traženoj vrijednosti, vraćamo 1 (true)
        return 1;
    if (value < root->data)
		return search(root->left, value);       //Ako je tražena vrijednost manja, tražimo u lijevom podstablu
	return search(root->right, value);			   //Ako je tražena vrijednost veća, tražimo u desnom podstablu 
}

// 4. Inorder ispis (Lijevo - Korijen - Desno)
int inorder(Node* root) {
    if (root == NULL) return;       
	inorder(root->left);                    //Rekurzivno ispisujemo lijevo podstablo
	printf("%d ", root->data);			    //Ispisujemo vrijednost čvora  
	inorder(root->right);   		        //Rekurzivno ispisujemo desno podstablo
    return 0;
}

// 5. Preorder ispis (Korijen - Lijevo - Desno)
int preorder(Node* root) {
    if (root == NULL) return;
	printf("%d ", root->data);          //Ispisujemo vrijednost čvora
	preorder(root->left);			    //Rekurzivno ispisujemo lijevo podstablo   
	preorder(root->right);              //Rekurzivno ispisujemo desno podstablo
    return 0;
}

// 6. Postorder ispis (Lijevo - Desno - Korijen)
int postorder(Node* root) {
    if (root == NULL) return;
	postorder(root->left);              //Rekurzivno ispisujemo lijevo podstablo
	postorder(root->right); 	     	//Rekurzivno ispisujemo desno podstablo
	printf("%d ", root->data);	        //Ispisujemo vrijednost čvora 
    return 0;
}

// 9. Pronalaženje najmanjeg elementa 
Node* findMin(Node* root) {
	while (root->left != NULL)             //Tražimo najmanji element tako što idemo lijevo dok ne dođemo do čvora bez lijevog djeteta
        root = root->left;
    return root;
}

// 8. Brisanje elementa 
Node* deleteNode(Node* root, int value) {
    if (root == NULL)
        return NULL;

    if (value < root->data)
		root->left = deleteNode(root->left, value);         //Ako je vrijednost manja, idemo lijevo
    else if (value > root->data) 
		root->right = deleteNode(root->right, value);       //Ako je vrijednost veća, idemo desno   
    else {
        // Čvor s jednim ili bez djece 
        if (root->left == NULL) {
			Node* temp = root->right;       //Spremamo desno dijete u privremenu varijablu
            free(root);
            return temp;
        }
        else if (root->right == NULL) {
			Node* temp = root->left;		//Spremamo lijevo dijete u privremenu varijablu 
            free(root);
            return temp;
        }

        // Čvor s dvoje djece 
		Node* temp = findMin(root->right);	// Kopiramo vrijednost najmanjeg čvora iz desnog podstabla u trenutni čvor  
		root->data = temp->data;        // Brišemo najmanji čvor iz desnog podstabla
		root->right = deleteNode(root->right, temp->data);  //Rekurzivno brišemo najmanji čvor  
    }
    return root;
}

// 10. Visina (pomoćna funkcija za level ispis) 
int height(Node* root) {
    if (root == NULL) return 0;
	int l = height(root->left);         //Visina lijevog podstabla
	int r = height(root->right);	    //Visina desnog podstabla    
	return (l > r ? l : r) + 1;         //Visina je veća visina između lijevog i desnog podstabla plus 1 za trenutni čvor
}


// 7. Level ispis
int printLevel(Node* root, int level) {
    if (root == NULL) return;
    if (level == 1)
		printf("%d ", root->data);          //Ispisujemo vrijednost čvora na trenutnom nivou
    else {
		printLevel(root->left, level - 1);		  //Rekurzivno idemo lijevo smanjujući nivo za 1    
		printLevel(root->right, level - 1);	      //Rekurzivno idemo desno smanjujući nivo za 1 
    }
    return 0;
}

int levelOrder(Node* root) {
	int h = height(root);               //Dobijamo visinu stabla
	for (int i = 1; i <= h; i++)        //Ispisujemo svaki nivo stabla
        printLevel(root, i);
	return 0;
}

// 11. Main, pozivamo funkcije preko switch case-a
int main() {
    Node* root = NULL;
    int choice, value;

    do {
        printf("1. Unos elementa\n");
        printf("2. Pronalazenje elementa\n");
        printf("3. Brisanje elementa\n");
        printf("4. Inorder ispis\n");
        printf("5. Preorder ispis\n");
        printf("6. Postorder ispis\n");
        printf("7. Level order ispis\n");
        printf("0. Izlaz\n");
        printf("Izbor: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("Unesite vrijednost: ");
            scanf("%d", &value);
            root = insert(root, value);
            break;

        case 2:
            printf("Unesite vrijednost: ");
            scanf("%d", &value);
            if (search(root, value))
                printf("Element postoji u stablu.\n");
            else
                printf("Element NE postoji u stablu.\n");
            break;

        case 3:
            printf("Unesite vrijednost za brisanje: ");
            scanf("%d", &value);
            root = deleteNode(root, value);
            break;

        case 4:
            printf("Inorder: ");
            inorder(root);
            printf("\n");
            break;

        case 5:
            printf("Preorder: ");
            preorder(root);
            printf("\n");
            break;

        case 6:
            printf("Postorder: ");
            postorder(root);
            printf("\n");
            break;

        case 7:
            printf("Level order: ");
            levelOrder(root);
            printf("\n");
            break;
        }
    } while (choice != 0);

    return 0;
}
