#ifndef CARD_H
#define CARD_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

typedef struct _card{
    int number;
    int suit;
}card;

typedef struct _player{
    card* inhand;
    int numofcard;// number of cards
    int scores;
    int rank;// the player's rank (e.g player1's rank is 1)
}player;

typedef struct _pile{
    card* allcard;
    int num;// number of cards
}pile;

typedef struct node{
    player *pl;
    struct node *next;
    struct node *front;
}node_t;

void generate(int n,pile *stockpiles);
void shuffle(pile *stockpiles,int n);
card drawcard(pile *stockpiles,player* theplayer);
void playcard(pile *discardpills,player* theplayer,card thecard);
void printhelp();
void printlog(pile *stockpiles,int mode,FILE *fp,int round,int deck,int nofplayers);
void printcard(FILE *fp,card thecard,int mode);
void addcard(pile *stockpiles,pile *discardpiles);
card manplaycard(FILE *fp,player *theplayer,card *canplay,int *turn,int j,int num,pile *stockpiles,pile *discardpiles,int *test);
void sortcard(card *inhand,int num);

#endif
