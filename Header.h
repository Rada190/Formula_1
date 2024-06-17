#define _CRT_SECURE_NO_WARNINGS

#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>

#define FILE_NAME "races.txt"
#define MAX_RECORDS 1000 
#define MAX_NAME_LEN 100

typedef enum {
	GODINA = 1,
	IME,
    GUME,
    VRIJEME,
    VRACANJE
} pretrazivanje;
typedef enum { //pravilo br 3
    CREATE = 1,
    READ,
    UPDATE,
    DELETE,
    SEARCH,
    SORT,
    EXIT
} meniOpcije;
typedef enum {
    HARD,
    MEDIUM,
    SOFT
} tipGuma;
typedef enum {
    sortiranjeSilazno = 1,
    sortiranjeUzlazno,
    sortiranjeAbecedno,
    vrijemeSilazno,
    vrijemeUzlazno,
    povratak
} SortOption;
typedef struct {
    int year;
    char trackName[MAX_NAME_LEN];
    char driverName[MAX_NAME_LEN];
    int minutes;
    int seconds;
    int hundredths;
    tipGuma tyre;
    int laps;
} Race;
void createRecord(FILE* file);//pravilo br 4
void readRecords(FILE* file);
void updateRecord(FILE* file);
void deleteRecord(FILE* file);
void searchRecords(FILE* file);
void sortRecords(FILE* file);
int compareByYearDesc(const void* a, const void* b);
int compareByYearAsc(const void* a, const void* b);
int compareByDriver(const void* a, const void* b);
int compareByTimeAsc(const void* a, const void* b);
int compareByTimeDesc(const void* a, const void* b);
int compareByTrackName(const void* a, const void* b);
int compareByTyre(const void* a, const void* b);
void freeRace(Race* race);
tipGuma stringToTyreType(const char* str);
const char* tyreTypeToString(tipGuma tyre);
void printRace(const Race* race);
void quickSortDriver(Race* races, int low, int high);
int partitionDriver(Race* races, int low, int high);
extern FILE* file;//pravilo br 7

#endif // HEADER_H