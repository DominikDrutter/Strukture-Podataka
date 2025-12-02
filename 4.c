#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>


typedef struct Node {
    int coef;
    int exp;
    struct Node* next;
} Node;

/* Dodaje član sortiran po eksponentu (opadajuće), spaja ako exp već postoji */
void insertSorted(Node** head, int coef, int exp) {
    if (coef == 0) return;

    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->coef = coef;
    newNode->exp = exp;
    newNode->next = NULL;

    Node** curr = head;

    while (*curr != NULL && (*curr)->exp > exp)
        curr = &((*curr)->next);

    if (*curr != NULL && (*curr)->exp == exp) {
        (*curr)->coef += coef;
        free(newNode);
        if ((*curr)->coef == 0) {
            Node* temp = *curr;
            *curr = (*curr)->next;
            free(temp);
        }
    }
    else {
        newNode->next = *curr;
        *curr = newNode;
    }
}

/* Učitavanje polinoma iz datoteke */
Node* readPolynomial(FILE* f) {
    Node* p = NULL;
    int coef, exp;

    while (fscanf(f, "%d %d", &coef, &exp) == 2)
        insertSorted(&p, coef, exp);

    return p;
}

/* Zbrajanje polinoma */
Node* addPoly(Node* a, Node* b) {
    Node* result = NULL;
    while (a) {
        insertSorted(&result, a->coef, a->exp);
        a = a->next;
    }
    while (b) {
        insertSorted(&result, b->coef, b->exp);
        b = b->next;
    }
    return result;
}

/* Množenje polinoma */
Node* mulPoly(Node* a, Node* b) {
    Node* result = NULL;

    for (Node* p = a; p != NULL; p = p->next) {
        for (Node* q = b; q != NULL; q = q->next) {
            insertSorted(&result, p->coef * q->coef, p->exp + q->exp);
        }
    }
    return result;
}

/* Ispis polinoma */
void printPoly(Node* p) {
    if (!p) { printf("0\n"); return; }
    while (p) {
        printf("%dx^%d", p->coef, p->exp);
        if (p->next && p->next->coef >= 0) printf(" + ");
        else if (p->next) printf(" ");
        p = p->next;
    }
    printf("\n");
}

/* Oslobađanje memorije */
void freePoly(Node* p) {
    while (p) {
        Node* t = p;
        p = p->next;
        free(t);
    }
}

int main() {
    FILE* f1 = fopen("polinomi1.txt", "r");
    if (!f1) {
        printf("Ne mogu otvoriti datoteku.\n");
        return 1;
    }

    FILE* f2 = fopen("polinomi2.txt", "r");
    if (!f2) {
        printf("Ne mogu otvoriti datoteku.\n");
        return 1;
    }

    printf("Ucitavanje prvog polinoma...\n");
    Node* poly1 = readPolynomial(f1);

    printf("Ucitavanje drugog polinoma...\n");
    Node* poly2 = readPolynomial(f2);

    fclose(f1);
    fclose(f2);


    printf("Polinom 1: ");
    printPoly(poly1);
    printf("Polinom 2: ");
    printPoly(poly2);

    Node* sum = addPoly(poly1, poly2);
    Node* product = mulPoly(poly1, poly2);

    printf("\nZbroj:      ");
    printPoly(sum);
    printf("Umnozak:    ");
    printPoly(product);

    freePoly(poly1);
    freePoly(poly2);
    freePoly(sum);
    freePoly(product);

    return 0;
}
