#define _CRT_SECURE_NO_WARNINGS
#define MAX_BODOVA 100.0

#include <stdio.h>
#include <stdlib.h>


typedef struct {
	char ime[50];
	char prezime[50];
	int bodovi;
} Student;


//Funkcija za èitanje datoteke

int datoteka(const char* filename, Student** studenti) {
	FILE* dat = fopen("Studenti.txt", "r");
	if (!dat) return -1;

    Student* niz = NULL;
    int count = 0;
	
	while (1) {
		Student temp;
		if (fscanf(dat, "%s %s %d", temp.ime, temp.prezime, &temp.bodovi) != 3)
			break;

	Student* novi = realloc(niz, (count + 1) * sizeof(Student));
	if (!novi) {
		free(niz);
		fclose(dat);
		return 1;
	}
	niz = novi;
	niz[count] = temp;
	count++;

}
fclose(dat);
*studenti = niz;
return count;

}


//Ispis

int printData(Student* studenti, int broj_studenata) {
    if (!studenti) return -1;
    printf("Ime\tPrezime\tBodovi\tPostotak\n");
    printf("----------------------------\n");
    for (int i = 0; i < broj_studenata; i++) {
        double postotak = (studenti[i].bodovi / MAX_BODOVA) * 100.0;
        printf("%s\t%s\t%d\t%.2f\n",
            studenti[i].ime,
            studenti[i].prezime,
            studenti[i].bodovi,
            postotak);
    }
    return 0;
}

int main() {
    Student* studenti = NULL;
    int broj_studenata = datoteka("studenti.txt", &studenti);
    if (broj_studenata == -1) {
        printf("Greška pri èitanju datoteke ili alokaciji memorije.\n");
        return 1;
    }
    if (printData(studenti, broj_studenata) == -1) {
        printf("Greška pri ispisu podataka.\n");
        free(studenti);
        return 1;
    }
    free(studenti);
    return 0;
}