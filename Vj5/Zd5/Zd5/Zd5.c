#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

///////////////////////////////STRUKTURA
typedef struct Node {
    int data;
    struct Node* next;
} Node;

// Funkcija za stvaranje novog èvora
Node* createNode(int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = value;
    newNode->next = NULL;
    return newNode;
}





///////////////////////////////PUSH
void push(Node** top, int value) {
    Node* newNode = createNode(value);
    newNode->next = *top;
    *top = newNode;
}

///////////////////////////////POP
int pop(Node** top) {
    if (*top == NULL) {
        printf("Error: Stack underflow!\n");
        exit(1);
    }
    Node* temp = *top;
    int value = temp->data;
    *top = temp->next;
    free(temp);
    return value;
}





// Funkcija koja provjerava je li operator
int isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

// Funkcija koja raèuna operaciju
int performOperation(int a, int b, char op) {
    switch (op) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/':
        if (b == 0) {
            printf("Ne možemo dijeliti sa nulom!\n");
            exit(1);
        }
        return a / b;
    default:
        printf("Nepoznati simbol %c\n", op);
        exit(1);
    }
}






int main() {
    FILE* file = fopen("postfix.txt", "r");
    if (file == NULL) {
        printf("Greška pri otvaranju datoteke!\n");
        return 1;
    }

    Node* stack = NULL;
    char token[50];

    // Provjera je li broj ili operator)
    while (fscanf(file, "%s", token) != EOF) {

        if (isdigit(token[0]) ||
            (token[0] == '-' && isdigit(token[1]))) { // negativan broj
            int num = atoi(token);
            push(&stack, num);
        }
        // Ako je operator -> uzmi dva broja i izraèunaj
        else if (isOperator(token[0]) && strlen(token) == 1) {
            int b = pop(&stack);
            int a = pop(&stack);
            int result = performOperation(a, b, token[0]);
            push(&stack, result);
        }
        else {
            printf("Greška; '%s'\n", token);
            fclose(file);
            return 1;
        }
    }

    fclose(file);


    int result = pop(&stack);



    printf("Rezultat izraza: %d\n", result);
    return 0;
}
