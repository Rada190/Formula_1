#define _CRT_SECURE_NO_WARNINGS

#include "header.h"
#include <stdio.h>
#include <stdlib.h>

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

FILE* file = NULL;

int main() {
    file = fopen(FILE_NAME, "a+");
    if (!file) {
        perror("Failed to open file");
        return 1;
    }

    meniOpcije option;

    while (1) {
        clearScreen();
        printf("#=======================#\n");
        printf("|   Formula 1 meni:     |\n");
        printf("*=======================*\n");
        printf("|1. Upisite Vozaca      |\n");
        printf("|2. Ispisi sve zapise   |\n");
        printf("|3. Azurirajte vozaca   |\n");
        printf("|4. Obrisite vozaca     |\n");
        printf("|5. Pretrazite zapise   |\n");
        printf("|6. Sortirajte zapise   |\n");
        printf("|7. Izlazak             |\n");
        printf("#=======================#\n");
        printf("\n");
        printf("Upisite vas izbor:");

        if (scanf("%d", &option) != 1) {
            printf("Neispravan unos!\n");
            while (getchar() != '\n'); // Ciscenje imput buffera
            continue;
        }

        switch (option) {
        case CREATE:
            createRecord(file);
            break;
        case READ:
            readRecords(file);
            break;
        case UPDATE:
            updateRecord(file);
            break;
        case DELETE:
            deleteRecord(file);
            break;
        case SEARCH:
            searchRecords(file);
            break;
        case SORT:
            sortRecords(file);
            break;
        case EXIT:
            fclose(file);
            return 0;
        default:
            printf("Neispravan unos!\n");
            break;
        }

        printf("\nPrititsnite enter za nastavak...");
        while (getchar() != '\n'); //Cekane da se prititsne enter
        getchar(); // Ciscenje na buffer
    }

    fclose(file);
    return 0;
}
