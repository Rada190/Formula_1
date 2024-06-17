#define _CRT_SECURE_NO_WARNINGS

#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>   //standardna biblioteka u C programskom jeziku koja definira makroe za sistemske greške

// pravilo br 8
#define SAFE_FREE(p) do { if ((p)) { free((p)); (p) = NULL; } } while (0)

static inline tipGuma stringToTyreType(const char* str) { //pravilo br 5 i pravilo br 8
    if (strcmp(str, "HARD") == 0) {
        return HARD;
    }
    if (strcmp(str, "MEDIUM") == 0) {
        return MEDIUM;
    }
    if (strcmp(str, "SOFT") == 0) {
        return SOFT;
    }
    return HARD; //Obicno je najbolje vratiti neku default vrednost a to je HARD u ovom slucaju
}

static inline const char* tyreTypeToString(tipGuma tyre) {
    switch (tyre) {
    case HARD: 
        return "HARD";
    case MEDIUM: 
        return "MEDIUM";
    case SOFT: 
        return "SOFT";
    default: 
        return "UNKNOWN";
    }
}

static inline void printRace(const Race* race) {
    printf("Godina: %d, Staza: %s, Vozac: %s, Vrijeme: %02d:%02d:%03d, Gume: %s, Krugovi: %d\n", race->year, race->trackName, race->driverName, race->minutes, race->seconds, race->hundredths, tyreTypeToString(race->tyre), race->laps);
}

void createRecord(FILE* file) {
    Race* race = (Race*)malloc(sizeof(Race));//dinamico zauzimanje memorije pravilo br 14 i 15
    char tyreStr[10] = { 0 };

    printf("Unesite godinu: ");
    if (scanf("%d", &race->year) != 1) {
        printf("Neispravan unos\n");
        SAFE_FREE(race);//sigurnoso oslobadjanje memorije pravilo br 16
        return;
    }
    printf("Unesite ime staze: ");
    if (scanf("%99s", race->trackName) != 1) {
        printf("Neispravan unos\n");
        SAFE_FREE(race);
        return;
    }
    printf("Unesite ime vozaca: ");
    if (scanf("%99s", race->driverName) != 1) {
        printf("Neispravan unos\n");
        SAFE_FREE(race);
        return;
    }
    printf("Unesite vrijeme (minute:sekunde:stotinjke): ");
    if (scanf("%d:%d:%d", &race->minutes, &race->seconds, &race->hundredths) != 3) {
        printf("Neispravan unos\n");
        SAFE_FREE(race);
        return;
    }
    printf("Unesite tip guma (HARD, MEDIUM, SOFT): ");
    if (scanf("%9s", tyreStr) != 1) {
        printf("Neispravan unos\n");
        SAFE_FREE(race);
        return;
    }
    race->tyre = stringToTyreType(tyreStr);

    printf("Unesite broj krugova: ");
    if (scanf("%d", &race->laps) != 1) {
        printf("Neispravan unos\n");
        SAFE_FREE(race);
        return;
    }
    fprintf(file, "%d %s %s %02d:%02d:%03d %s %d\n",
        race->year, race->trackName, race->driverName, race->minutes, race->seconds, race->hundredths,
        tyreTypeToString(race->tyre), race->laps);
    fflush(file); // koristi se za praznjenje buffera
    SAFE_FREE(race);//sigurnoso oslobadjanje memorije pravilo br 16
}
void readRecords(FILE* file) {
    rewind(file);// pravilo br 18 koristiti rewind... koristi se za vraćanje pokazivača pozicije u datoteci na početak datoteke
    Race race = { 0 };
    char tyreStr[10] = { 0 };

    while (fscanf(file, "%d %99s %99s %d:%d:%d %9s %d", &race.year, race.trackName, race.driverName, &race.minutes, &race.seconds, &race.hundredths, tyreStr, &race.laps) == 8) {
        race.tyre = stringToTyreType(tyreStr);
        printRace(&race);
    }
}
void updateRecord(FILE* file) {
    int year;
    char driverName[MAX_NAME_LEN] = { 0 };
    Race race = { 0 };
    char tyreStr[10] = { 0 };
    int found = 0;

    printf("Unesite godinu za azuriranje: ");
    if (scanf("%d", &year) != 1) {
        printf("Neispravan unos\n");
        return;
    }
    printf("Unesite ime vozaca za azuriranje: ");
    if (scanf("%99s", driverName) != 1) {
        printf("Neispravan unos\n");
        return;
    }
    FILE* tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        perror("Nemoguce otvorite temp datoteku"); //prvilo br 20...koristi se za ispisivanje opisa posljednje greške koja se dogodila tokom poziva funkcije
        return;
    }
    rewind(file);
    while (fscanf(file, "%d %99s %99s %d:%d:%d %9s %d", &race.year, race.trackName, race.driverName, &race.minutes, &race.seconds, &race.hundredths, tyreStr, &race.laps) == 8) {
        race.tyre = stringToTyreType(tyreStr);
        if (race.year == year && strcmp(race.driverName, driverName) == 0) {
            found = 1;
            printf("Unesite novo ime staze: ");
            if (scanf("%99s", race.trackName) != 1) {
                printf("Neispravan unos\n");
                fclose(tempFile);
                return;
            }
            printf("Unesite novo vrijeme (minute:sekunde:stotinjke): ");
            if (scanf("%d:%d:%d", &race.minutes, &race.seconds, &race.hundredths) != 3) {
                printf("Neispravan unos\n");
                fclose(tempFile);
                return;
            }
            printf("Unesite novi tip gume (HARD, MEDIUM, SOFT): ");
            if (scanf("%9s", tyreStr) != 1) {
                printf("Neispravan unos\n");
                fclose(tempFile);
                return;
            }
            race.tyre = stringToTyreType(tyreStr);
            printf("Unesite novi broj krugova: ");
            if (scanf("%d", &race.laps) != 1) {
                printf("Neispravan unos\n");
                fclose(tempFile);
                return;
            }
        }
        fprintf(tempFile, "%d %s %s %02d:%02d:%03d %s %d\n", race.year, race.trackName, race.driverName, race.minutes, race.seconds, race.hundredths, tyreTypeToString(race.tyre), race.laps);
    }
    fclose(file);
    fclose(tempFile);
    remove("races.txt");//pravilo br 19...koristi se za brisanje datoteke
    if (rename("temp.txt", "races.txt") != 0) {
        perror("Pogreska u preimenovanju datoteke!");
        return;
    }
    file = fopen("races.txt", "a+");
    if (file == NULL) {
        perror("Pogreska u otvaranju datoteke!");
        return;
    }
    if (!found) {
        printf("Vozac nije pronadjen!\n");
    }
}
void deleteRecord(FILE* file) {
    int year;
    char driverName[MAX_NAME_LEN] = { 0 };
    Race race = { 0 };
    char tyreStr[10] = { 0 };
    int found = 0;
    printf("Upisite godinu za obrisati: ");
    if (scanf("%d", &year) != 1) {
        printf("Neispravan unos\n");
        return;
    }
    printf("Unesite ime vozaca za obrisati: ");
    if (scanf("%99s", driverName) != 1) {
        printf("Neispravan unos\n");
        return;
    }
    FILE* tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        perror("Pogreska u otvaranju datoteke");
        return;
    }
    rewind(file);
    while (fscanf(file, "%d %99s %99s %d:%d:%d %9s %d", &race.year, race.trackName, race.driverName, &race.minutes, &race.seconds, &race.hundredths, tyreStr, &race.laps) == 8) {
        race.tyre = stringToTyreType(tyreStr);
        if (!(race.year == year && strcmp(race.driverName, driverName) == 0)) {
            fprintf(tempFile, "%d %s %s %02d:%02d:%03d %s %d\n", race.year, race.trackName, race.driverName,
                race.minutes, race.seconds, race.hundredths, tyreTypeToString(race.tyre), race.laps);
        }
        else {
            found = 1;
        }
    }
    fclose(file);
    fclose(tempFile);
    remove("races.txt");
    if (rename("temp.txt", "races.txt") != 0) {
        perror("Pogreska u otvaranju temp datoteke");
        return;
    }
    file = fopen("races.txt", "a+");
    if (file == NULL) {
        perror("Pogreska u otvaranju datoteke");
        return;
    }
    if (found) {
        printf("Vozas uspjesno obrisan.\n");
    }
    else {
        printf("Vozac nije pronadjen!\n");
    }
}
void searchRecords(FILE* file) {
    int choice, year, minutes, seconds, hundredths;
    char trackName[MAX_NAME_LEN] = { 0 }, driverName[MAX_NAME_LEN] = { 0 }, tyreStr[10] = { 0 }, searchTyreStr[10] = { 0 };// staticka zauzeta polja pravilo br 13
    tipGuma searchTyre;
    static Race races[MAX_RECORDS];
    int numRecords = 0;

    //Ucitava sve zapise u polja
    rewind(file);
    while (fscanf(file, "%d %99s %99s %d:%d:%d %9s %d", &races[numRecords].year, races[numRecords].trackName, races[numRecords].driverName, &races[numRecords].minutes, &races[numRecords].seconds, &races[numRecords].hundredths, tyreStr, &races[numRecords].laps) == 8) {
        races[numRecords].tyre = stringToTyreType(tyreStr);
        numRecords++;
    }
    if (numRecords == 0) {
        printf("Vozac nije pronadjen!\n");
        return;
    }
    pretrazivanje izbor;
    printf("#===================#\n");
    printf("|   Pretrazuj po:   |\n");
    printf("*===================*\n");
    printf("|1. Godini          |\n");
    printf("|2. Ime staze       |\n");
    printf("|3. Tip guma        |\n");
    printf("|4. Vrijeme         |\n");
    printf("|5. Povratak        |\n");
    printf("#===================#\n");
    printf("\nUnesite vas izbor: ");
    if (scanf("%d", &izbor) != 1) {
        printf("Pogresan unos!\n");
        return;
    }
    int (*compare)(const void*, const void*);// Pokazivac na funkciju za usporedbu pravilo br 24
    Race key;

    switch (izbor) {
    case GODINA:
        printf("Unesite godinu: ");
        if (scanf("%d", &year) != 1) {
            printf("Neispravan unos\n");
            return;
        }
        key.year = year;
        compare = compareByYearAsc;// Pokazivac na funkciju za usporedbu
        qsort(races, numRecords, sizeof(Race), compare);
        break;
    case IME:
        printf("Unesite ime staze: ");
        if (scanf("%99s", trackName) != 1) {
            printf("Neispravan unos\n");
            return;
        }
        strncpy(key.trackName, trackName, MAX_NAME_LEN);
        compare = compareByTrackName;// Pokazivac na funkciju za usporedbu
        qsort(races, numRecords, sizeof(Race), compare);
        break;
    case GUME:
        printf("Unesite tip guma (HARD, MEDIUM, SOFT): ");
        if (scanf("%9s", searchTyreStr) != 1) {
            printf("Neispravan unos\n");
            return;
        }
        searchTyre = stringToTyreType(searchTyreStr);
        key.tyre = searchTyre;
        compare = compareByTyre;// Pokazivac na funkciju za usporedbu
        qsort(races, numRecords, sizeof(Race), compare);
        break;
    case VRIJEME:
        printf("Unesite vrijeme (minute:sekunde:stotinjke): ");
        if (scanf("%d:%d:%d", &minutes, &seconds, &hundredths) != 3) {
            printf("Neispravan unos\n");
            return;
        }
        key.minutes = minutes;
        key.seconds = seconds;
        key.hundredths = hundredths;
        compare = compareByTimeAsc;
        qsort(races, numRecords, sizeof(Race), compare);
        break;
    case VRACANJE:
        return;
    default:
        printf("Pogresan unos!\n");
        return;
    }

    //Koristimo funkciju bsearch da njadjemo iste rezultate
    Race* result = bsearch(&key, races, numRecords, sizeof(Race), compare);//pravilo br 22 moramo koristit funkciju bsearch za pretragu
    if (result) {
        // Pronalazenje indeksa pronadjenog zapisa
        int index = result - races;
        // Ispisivanje svih zapisa koji se poklapaju
        for (int i = index; i >= 0; i--) {
            if (compare(&races[i], &key) == 0) {
                printRace(&races[i]);
            }
            else {
                break;
            }
        }
        // Ispisivanje svih zapisa koji se poklapaju
        for (int i = index + 1; i < numRecords; i++) {
            if (compare(&races[i], &key) == 0) {
                printRace(&races[i]);
            }
            else {
                break;
            }
        }
    }
    else {
        printf("Zapis nije pronadjen!\n");
    }
}
int compareByTrackName(const void* a, const void* b) {
    return strcmp(((Race*)a)->trackName, ((Race*)b)->trackName);
}
int compareByTyre(const void* a, const void* b) {
    return ((Race*)a)->tyre - ((Race*)b)->tyre;
}
int compareByYearDesc(const void* a, const void* b) {
    const Race* raceA = (const Race*)a;
    const Race* raceB = (const Race*)b;
    return raceB->year - raceA->year;
}
int compareByYearAsc(const void* a, const void* b) {
    const Race* raceA = (const Race*)a;
    const Race* raceB = (const Race*)b;
    return raceA->year - raceB->year;
}
int compareByDriver(const void* a, const void* b) {
    const Race* raceA = (const Race*)a;
    const Race* raceB = (const Race*)b;
    return strcmp(raceA->driverName, raceB->driverName);
}
int compareByTimeAsc(const void* a, const void* b) {
    const Race* raceA = (const Race*)a;
    const Race* raceB = (const Race*)b;
    int timeA = raceA->minutes * 60000 + raceA->seconds * 1000 + raceA->hundredths;//raceA->minutes * 60000 pretvara minute u milisekunde... raceA->seconds * 1000 pretvara sekunde u milisekunde...raceA->hundredths dodaje stotinke sekunde direktno u ukupno vrijeme u milisekundama.
    int timeB = raceB->minutes * 60000 + raceB->seconds * 1000 + raceB->hundredths;
    return timeA - timeB;
}
int compareByTimeDesc(const void* a, const void* b) {
    const Race* raceA = (const Race*)a;
    const Race* raceB = (const Race*)b;
    int timeA = raceA->minutes * 60000 + raceA->seconds * 1000 + raceA->hundredths;
    int timeB = raceB->minutes * 60000 + raceB->seconds * 1000 + raceB->hundredths;
    return timeB - timeA;
}
void quickSortDriver(Race* races, int low, int high) {//rekurzivna funkcija pravilo br 23
    if (low < high) {
        int pi = partitionDriver(races, low, high);
        quickSortDriver(races, low, pi - 1);
        quickSortDriver(races, pi + 1, high);
    }
}
int partitionDriver(Race* races, int low, int high) {
    char* pivot = races[high].driverName;
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (strcmp(races[j].driverName, pivot) < 0) {
            i++;
            Race temp = races[i];
            races[i] = races[j];
            races[j] = temp;
        }
    }
    Race temp = races[i + 1];
    races[i + 1] = races[high];
    races[high] = temp;
    return (i + 1);
}
void sortRecords(FILE* file) {
    FILE* inputFile = fopen("races.txt", "r");
    if (!inputFile) {
        perror("Pogreska u otvaranju datoteke!");
        return;
    }
    static Race buffer[MAX_RECORDS];
    int numRecords = 0;
    char tyreStr[10];
    while (fscanf(inputFile, "%d %99s %99s %d:%d:%d %9s %d", &buffer[numRecords].year, buffer[numRecords].trackName, buffer[numRecords].driverName, &buffer[numRecords].minutes, &buffer[numRecords].seconds, &buffer[numRecords].hundredths, tyreStr, &buffer[numRecords].laps) == 8) {
        buffer[numRecords].tyre = stringToTyreType(tyreStr);
        numRecords++;
    }
    fclose(inputFile);

    SortOption sortOption;
    printf("#==========================#\n");
    printf("|        Sortiraj po:      |\n");
    printf("*==========================*\n");
    printf("|1. Godina (Silazno)       |\n");
    printf("|2. Godina (Uzlazno)       |\n");
    printf("|3. Ime vozaca (Abecedno)  |\n");
    printf("|4. Vrijeme (Silazno)      |\n");
    printf("|5. Vrijeme (Uzlazno)      |\n");
    printf("|6. Povratak               |\n");
    printf("#==========================#\n");
    printf("\nEnter your choice: ");

    if (scanf("%d", &sortOption) != 1) {
        printf("Invalid input\n");
        return;
    }
    switch (sortOption) {
    case sortiranjeSilazno:
        qsort(buffer, numRecords, sizeof(Race), compareByYearDesc);//pravilo br 21 koriscenje qsort funkcije
        break;
    case sortiranjeUzlazno:
        qsort(buffer, numRecords, sizeof(Race), compareByYearAsc);
        break;
    case sortiranjeAbecedno:
        quickSortDriver(buffer, 0, numRecords - 1);//Pozivanje rekurzivne funkcije
        break;
    case vrijemeSilazno:
        qsort(buffer, numRecords, sizeof(Race), compareByTimeAsc);
        break;
    case vrijemeUzlazno:
        qsort(buffer, numRecords, sizeof(Race), compareByTimeDesc);
        break;
    case povratak:
        return;
    default:
        printf("Neispravan unos!\n");
        return;
    }
    FILE* outputFile = fopen("races.txt", "w");
    if (!outputFile) {
        perror("Pogreska u otvaranju datoteke!");
        return;
    }
    for (int i = 0; i < numRecords; i++) {
        fprintf(outputFile, "%d %s %s %02d:%02d:%03d %s %d\n", buffer[i].year, buffer[i].trackName, buffer[i].driverName, buffer[i].minutes, buffer[i].seconds, buffer[i].hundredths, tyreTypeToString(buffer[i].tyre), buffer[i].laps);
    }
    fclose(outputFile);
    printf("\nzapisi uspjesno sortirani!\n");
    // printanje sortiranih zapisa
    inputFile = fopen("races.txt", "r");
    if (!inputFile) {
        perror("Pograska u otvaranju datoteke");
        return;
    }
    readRecords(inputFile);
    fclose(inputFile);
}
void freeRace(Race* race) { //sigurnoso oslobadjanje memorije pravilo br 16
    race->trackName[0] = '\0';//postavlja prvi znak na nulu
    race->driverName[0] = '\0';
}
