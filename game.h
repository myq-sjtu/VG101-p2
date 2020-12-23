#ifndef GAME_H
#define GAME_H

#include"card.h"
#include"linkedlist.h"

int initialturn(FILE *fp,player *players,int nofplayer,pile *stockpiles,pile *discardpiles);
void initialcard(FILE *fp,player *players,pile *stockpiles,int nofplayer,int nofcard,pile *discardpiles);
card oneturn(int *extraturn,FILE *fp,pile *stockpiles,player *theplayer,card lastcard,pile *discardpiles,int *test,int mode,int *qtest,int *jtest);
int oneround(FILE *fp,pile *stockpiles,int nofplayer,int nofcard,pile *discardpiles,int winner,int mode,player* players);
card notattack(card* canplay,card lastcard,FILE* fp, player* theplayer, int mode,card thecard, int* extraturn,pile* stockpiles,pile* discardpiles,int* test,int* qtest,int* jtest,int* j);
card underattack(card* canplay,card lastcard,FILE *fp,player* theplayer, int mode,card thecard, int* extraturn,pile* stockpiles,pile* discardpiles,int* test,int* qtest,int* jtest,int *j);

#endif
