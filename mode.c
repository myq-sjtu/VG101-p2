#include"mode.h"

void mode(int nofcard,int nofplayer,int nofdeck,int nofround,/*FILE *fp,*/int mode){
    pile stockpiles;
    pile discardpile;
    player players[nofplayer];
    stockpiles.allcard=malloc((unsigned)(nofdeck*52)*sizeof(card));
    discardpile.allcard=malloc((unsigned)(nofdeck*52)*sizeof(card));
    stockpiles.num=0;
    discardpile.num=0;
    int i,winner,round;
    for (i=0;i<=nofplayer-1;i++){
        (players[i]).inhand=malloc(100*sizeof(card));
        (players[i]).numofcard=0;  
        players[i].scores=0;
        players[i].rank=i;
        players[i].isman=0;
    }
    players[0].isman=1;
    generate(nofdeck,&stockpiles);
    shuffle(&stockpiles,nofdeck*52);
    printlog(&stockpiles,2,nofround,nofdeck,nofplayer);
    winner=initialturn(players,nofplayer,&stockpiles,&discardpile);
    printf("---- Game start ----\n");
    for (round=1;round<=nofround;round++){
        printf("round%d: start with player%d\n",round,winner+1);
        if (round!=1){
            if (mode!=1){
                for (i=0;i<stockpiles.num;i++){
                    printcard(stockpiles.allcard[i],1);
                }
            } else {
                for (i=0;i<stockpiles.num;i++){
                    printcard(stockpiles.allcard[i],0);
                }
            }
            printf("\n");
        }
        winner=oneround(&stockpiles,nofplayer,nofcard,&discardpile,winner,mode,players);
        generate(nofdeck,&stockpiles);
        shuffle(&stockpiles,nofdeck*52);
        discardpile.num=0;
    }
    printf("In total\n");
    int max=players[0].scores;
    for (i=0;i<=nofplayer-1;i++){
        printf("player%d get %d points\n",i+1,players[i].scores);
        if (max<players[i].scores){
            max=players[i].scores;
        }
    }
    printf("the winner is: ");
    for (i=0;i<=nofplayer-1;i++){
        if (players[i].scores==max){
            printf("player %d  ",players[i].rank+1);
        }
    }
    for (i=0;i<=nofplayer-1;i++){
        free((players[i]).inhand);
    }
    free(stockpiles.allcard);
    free(discardpile.allcard);
}
