/*
Kamil Pek 231050
17.12.2016
gcc serwer.c -o serwer.out -Wall
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>

#define DICTSIZE 20
#define klucz 10

typedef struct {
   char polish[100];
   char english[100];
} dictionary;

typedef struct {
   long pID;
   char slowo[100];
} message;

dictionary dictPolEng[DICTSIZE] = {
   {"rower", "bike"}, {"kot", "cat"},
   {"pies", "dog"}, {"owca", "lamb"},
   {"stół", "table"}, {"krzesło", "chair"},
   {"mleko", "milk"}, {"zegar", "clock"},
   {"prysznic", "shower"}, {"blisko", "near"},
   {"dom", "house"}, {"samochód", "car"},
   {"grać", "play"}, {"brać", "take"},
   {"chwila", "while"}, {"drzewo", "tree"},
   {"dzwonić", "call"}, {"głowa", "head"},
   {"każdy", "each"}, {"który", "which"}
};

char *translate(char*);
void endWork(int);
void clQueve(int);

int queveIn, queveOt;

int main(int argc, char *argv[]) {

   message communique;
   queveIn = msgget(klucz, 0777 | IPC_CREAT);
   queveOt = msgget(klucz, 0777 | IPC_CREAT);

   while(1){
      if(msgrcv(queveIn, &communique, sizeof(char)*110, 0, 0) == -1){
         printf("Błąd podczas odbioru słowa od klienta.");
         exit(1); }

      printf("Odebrano: \"%s\", ", communique.slowo);
      strcpy(communique.slowo, translate(communique.slowo));
      printf("wysylanie do PID[%5ld] odpowiedz \"%s\".\n", communique.pID, communique.slowo);

      if(msgsnd(queveOt, &communique, sizeof(char)*110, 0) == -1){
         printf("\nBłąd! Przy wysyłaniu słowa do klienta");
         exit(2); }
   }
   return 0;
}

char *translate(char* word){
   int i;
   for ( i = 0; i < DICTSIZE; i++) {
      if(!strcmp(word, dictPolEng[i].polish))
         return dictPolEng[i].english; }
   return "Nie znam takiego slowa.";
}
