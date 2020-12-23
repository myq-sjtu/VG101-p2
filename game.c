// these four function is to complete one round of game
#include"game.h"
#include"linkedlist.h"

// detemine the first one to play in the first round
int initialturn(FILE *fp,player *players,int nofplayer,pile *stockpiles,pile *discardpiles){
    // store the firstcard of each player
    card startcard[nofplayer],temp;
    int i,k=0;
    printf("Determining the playing order...");
    fprintf(fp,"Determining the playing order...");
    // everyone draw a card
    for (i=0;i<=nofplayer-1;i++){
        startcard[i]=drawcard(stockpiles,&(players[i]));
        printf("player %d: ",i+1);
        fprintf(fp,"player %d: ",i+1);
        printcard(fp,startcard[i],1);
        printf("\n");
        fprintf(fp,"\n");
        playcard(discardpiles,&(players[i]),startcard[i]);
    }
    // get the smallest card
    temp=startcard[0];
    for (i=0;i<=nofplayer-1;i++){
        if (startcard[i].suit<temp.suit){
            temp=*(startcard+i);
            k=i;
        } else {
            if ((startcard[i].suit==temp.suit)&&(startcard[i].number<temp.number)){
                temp=startcard[i];
                k=i;
            }
        }
    }
    return k;
}

// the original card in hand
void initialcard(FILE *fp,player *players,pile *stockpiles,int nofplayer,int nofcard,pile *discardpiles){
    int i,j;
    card thecard;
    for (i=0;i<=nofplayer-1;i++){
        if (stockpiles->num<nofcard){
            printf("stock piles exhausted\n");
            addcard(stockpiles,discardpiles);
            shuffle(stockpiles,stockpiles->num);
        }
        printf("player %d: ",i+1);
        fprintf(fp,"player %d: ",i+1);
        for (j=0;j<=nofcard-1;j++){
            thecard=drawcard(stockpiles,&(players[i]));    
            printcard(fp,thecard,1);
            if (j==nofcard-1){
                printf("\n");
                fprintf(fp,"\n");
            } else {
                printf(", ");
                fprintf(fp,", ");
            }
        }
        // if the stock pile exhausted during drawing, add card from the discard pile 
        if (i!=nofplayer-1){
            if(stockpiles->num+discardpiles->num<nofcard){
                printf("not enough card in total");
                exit(1);
            }
        }
    }
}

// play card in one turn
card oneturn(int *extraturn,FILE *fp,pile *stockpiles,player *theplayer,card lastcard,pile *discardpiles,int *test,int mode,int *qtest,int *jtest){
    int i,j=0;// jumptest is to check if the player can play the second card but refuse to play
    card thecard,*canplay=(card*)malloc((unsigned)theplayer->numofcard*sizeof(card));// canplay is the card can be play
    for (i=0;i<theplayer->numofcard;i++){
        canplay[i].number=0;
        canplay[i].suit=0;
    }
    sortcard(theplayer->inhand,theplayer->numofcard);// sort in order
    // when not under an attack
    if (((lastcard.number!=1) && (lastcard.number!=2) && (lastcard.number!=10) && (lastcard.number!=11))||(*test==0) ){      
         thecard=notattack(canplay,lastcard,fp, theplayer, mode,thecard, extraturn, stockpiles, discardpiles, test, qtest, jtest,&j);
    } else {
        // when under attack
        thecard=underattack(canplay,lastcard,fp,theplayer,mode,thecard,extraturn,stockpiles,discardpiles,test,qtest,jtest,&j);
    }
    // print the card in hand
    if (mode!=1){
        sortcard(theplayer->inhand,theplayer->numofcard);
        printf("player %d cards: ",theplayer->rank+1);
        fprintf(fp,"player %d cards: ",theplayer->rank+1);
        if (theplayer->numofcard!=0){
            for (i=0;i<theplayer->numofcard;i++){
                printcard(fp,theplayer->inhand[i],1);
            }
        } else {
            printf("no card!");
        }
        printf("\n");
        fprintf(fp,"\n");
    } else {
        sortcard(theplayer->inhand,theplayer->numofcard);
        fprintf(fp,"\nplayer %d cards: ",theplayer->rank+1);
        if (theplayer->numofcard!=0){
            for (i=0;i<theplayer->numofcard;i++){
                printcard(fp,theplayer->inhand[i],0);
            }
        }
        fprintf(fp,"\n");
        //check if the player can play a second card
        if (*extraturn==0){
            if (j!=0){
                j=0;
                lastcard=thecard;
                for (i=0;i<theplayer->numofcard;i++){
                    canplay[i].number=0;
                    canplay[i].suit=0;
                }
                // the condition when no attack
                if (*test==0){
                    for (i=0;i<theplayer->numofcard;i++){
                        if ((theplayer->inhand[i].number==lastcard.number) || (theplayer->inhand[i].suit==lastcard.suit)){
                            canplay[j]=theplayer->inhand[i];
                            j++;
                        }
                    }
                } else {
                    // the condition when under an attack
                    for (i=0;i<theplayer->numofcard;i++){
                        if ((theplayer->inhand[i].number==lastcard.number) || ((theplayer->inhand[i].number==10 || theplayer->inhand[i].number==11 || theplayer->inhand[i].number==2 || theplayer->inhand[i].number==1) && theplayer->inhand[i].suit==lastcard.suit)){
                            canplay[j]=theplayer->inhand[i];
                            j++;
                        }
                    }
                }
                if (j!=0){
                    *extraturn=1;
                } else {
                    *extraturn=0;
                }
            }
        } else {
            *extraturn=0;
        }
    }
    if (stockpiles->num+discardpiles->num==0){
            printf("not enough card, please start the game again.");
            exit(1);
        }
    if (stockpiles->num<=*test && discardpiles->num>=1){
        printf("Stock pile exhausted. Shuffling the discard pile and restore the stock pile\n");
        fprintf(fp,"Stock pile exhausted. Shuffling the discard pile and restore the stock pile\n");
        addcard(stockpiles,discardpiles);
        shuffle(stockpiles,stockpiles->num);
    }
    free(canplay);
    j=0;
    return thecard;
}

// play card in one round
int oneround(FILE *fp,pile *stockpiles,int nofplayer,int nofcard,pile *discardpiles,int winner,int mode,player *players){
    card thecard;
    /*qtest is to test how many Queen has been played
      jtest is to test if there is a Jack
      test if to test if there is an attack*/
    int i,test=0,qtest=0,jtest=1,extraturn=0,mod=mode;
    player *thisplayer;
    node_t *playerlist;
    initialcard(fp,players,stockpiles,nofplayer,nofcard,discardpiles);
    // connect the players in a double circular linked list
    for (i=0;i<nofplayer;i++){ 
        if (i==0){
            playerlist=Initialize(&(players[0]));
        } else {
            InsertLastList(&playerlist,&players[i]);
        }
    }
    // draw the first card
    thecard=stockpiles->allcard[0];
    for (i=0;i<stockpiles->num-1;i++){
        stockpiles->allcard[i]=stockpiles->allcard[i+1];
    }
    stockpiles->num--;
    // to find the first player to play in this round
    node_t *tmp=playerlist;
    for (i=0;i<winner;i++){
        tmp=tmp->next;
    }
    thisplayer=(tmp->pl);
    // clear the screen, in different operating system
    if (mod==1){
        #if defined(_WIN16) || defined(_WIN32) || defined(_WIN64)
        system("cls");
        #else
        system("clear");
        #endif
    }
    printf("the first card is ");
    fprintf(fp,"\n\n\nthe first card is ");
    printcard(fp,thecard,1);
    printf("\n");
    fprintf(fp,"\n");
    while (1){  
        // play one card
        thecard=oneturn(&extraturn,fp,stockpiles,thisplayer,thecard,discardpiles,&test,mod,&qtest,&jtest);
        // check if there is no card left(win)
        if (thisplayer->numofcard==0){
            printf("player%d win!\n",thisplayer->rank+1);
            fprintf(fp,"player%d win!\n",thisplayer->rank+1);
            winner=thisplayer->rank;
            break;
        }
        // clear the screen and make ready for the next player
        if (mode==1 && extraturn==0){
            // replace system("pause")
            int ch;
            while((ch=getchar())!=EOF && ch!='\n'){};
            getchar();
            #if defined(_WIN16) || defined(_WIN32) || defined(_WIN64)
            system("cls");
            #else 
            system("clear");
            #endif
            printf("the last card is ");
            printcard(fp,thecard,2);
            printf("\n");
            fprintf(fp,"\n");
        }
        if(test!=0){
            if (mode==1) printf("it is under attack now!, the number of cards is %d\n",test);
        }
        // determine the next player (check Jack and Queen)
        if (extraturn==0){
            if (qtest%2==0){
                if (jtest==2){
                    tmp->pl=thisplayer;
                    thisplayer=tmp->front->front->pl;
                    tmp=tmp->front->front;
                    jtest=1;
                } else {
                    if (jtest==1){
                        tmp->pl=thisplayer;
                        thisplayer=(tmp->front->pl);
                        tmp=tmp->front;
                    } else {
                        tmp->pl=thisplayer;
                        thisplayer=tmp->front->front->front->front->pl;
                        tmp=tmp->front->front->front->front;
                        jtest=1;
                    }
                } 
            } else {
                if (jtest==2){
                    thisplayer=(tmp->next->next->pl);
                    tmp=tmp->next->next;
                    jtest=1;
                } else {
                    if (jtest==1){
                        thisplayer=(tmp->next->pl);
                        tmp=tmp->next;
                    } else {
                        tmp->pl=thisplayer;
                        tmp=tmp->next->next->next->next;
                        thisplayer=tmp->pl;
                        jtest=1;
                    }
                }
            }
        } else {
            // extraturn==1 means that this player can play one more, so the next player is still the same
            printf("\n one more card available.\n");
            thisplayer=tmp->pl;
        }
        // add card if stock pile exhausted
    }
    // when one player won this round
    tmp=playerlist;
    printf("\n\n\n---- Stats ----\n");
    printf("This round result: \n\n");
    fprintf(fp,"---- Stats ----\n");
    fprintf(fp,"This round result: \n\n");
    // go through the list and print the scores
    do {
        tmp->pl->scores-=tmp->pl->numofcard;
        printf("player %d: %d, total: %d\n",tmp->pl->rank+1,-tmp->pl->numofcard,tmp->pl->scores);
        fprintf(fp,"player %d: %d, total: %d\n",tmp->pl->rank+1,-tmp->pl->numofcard,tmp->pl->scores);
        while(tmp->pl->numofcard!=0){
            playcard(discardpiles,(tmp->pl),tmp->pl->inhand[0]);
        }
        players[tmp->pl->rank]=*(tmp->pl);
        tmp=tmp->next;
    }while (tmp!=playerlist);
    FreeList(&playerlist);
    printf("\n\n");
    fprintf(fp,"\n\n");
    // replace system("pause");
    int ch;
    while((ch=getchar())!=EOF && ch!='\n'){};
    getchar();
    return winner;
}

// playcard when not under an attack
card notattack(card* canplay,card lastcard,FILE* fp, player* theplayer, int mode,card thecard, int* extraturn,pile* stockpiles,pile* discardpiles,int* test,int* qtest,int* jtest,int *j){
    int i,choice,jumptest=0;
    for (i=0;i<theplayer->numofcard;i++){
        if ((theplayer->inhand[i].number==lastcard.number) || (theplayer->inhand[i].suit==lastcard.suit)){
            canplay[*j]=theplayer->inhand[i];
            (*j)++;
        }
    }
    // if no card can be play, need to draw
    if (*j==0) {
        if (mode==1){
            thecard=manplaycard(fp,theplayer,canplay,extraturn,*j,theplayer->numofcard,stockpiles,discardpiles,test);
            while (1){
                printf("\nno choice, please enter 0:");
                if (scanf("%d",&choice)==1){};
                if (choice==0){
                    break;
                } else {
                    printf("\nNot valid, try again");
                }
            }
        } 
        thecard=drawcard(stockpiles,theplayer);
        printf("player %d draws: ",theplayer->rank+1);
        fprintf(fp,"player %d draws: ",theplayer->rank+1);
        printcard(fp,thecard,1);
        printf("\n");
        fprintf(fp,"\n");
        thecard=lastcard;
    } else {
        // have card(s) to play
        if (mode!=1){
            thecard=canplay[rand()%(*j)];
            playcard(discardpiles,theplayer,thecard);
            printf("player %d plays: ",theplayer->rank+1);
            fprintf(fp,"player %d plays: ",theplayer->rank+1);
            printcard(fp,thecard,1);
        } else {
            thecard=manplaycard(fp,theplayer,canplay,extraturn,*j,theplayer->numofcard,stockpiles,discardpiles,test);
            if (thecard.number==0){
                thecard=lastcard;
                jumptest=1;
            }
        }           
        if (jumptest==0){
            // check if it is a functional card
            switch (thecard.number){
                case 1:{
                    *test=2;
                    printf("attack begin\n");
                    fprintf(fp,"attack begin\n");
                    break;
                }
                case 2:{
                    *test=3;
                    printf("attack begin\n");
                    fprintf(fp,"attack begin\n");
                    break;
                }
                case 11:{
                    (*qtest)++;
                    printf("player%d play a queen, reverse\n",theplayer->rank+1);
                    fprintf(fp,"player%d play a queen, reverse\n",theplayer->rank+1);
                    break;
                }
                case 10:{
                    (*jtest)++;
                    printf("player%d play a jack, jump\n",theplayer->rank+1);
                    fprintf(fp,"player%d play a jack, jump\n",theplayer->rank+1);
                    break;
                }
                default:{            
                    printf("\n");
                    fprintf(fp,"\n");
                    break;
                }
            }
        } else{
            jumptest=0;
        }
    }
    return thecard;
}

// playcard when under an attack
card underattack(card* canplay,card lastcard,FILE *fp,player* theplayer, int mode,card thecard, int* extraturn,pile* stockpiles,pile* discardpiles,int* test,int* qtest,int* jtest,int *j){
    int i,k,choice;
    int othernumber1,othernumber2;
    switch(lastcard.number){
        case 1: {othernumber1=2,othernumber2=2;break;}
        case 2: {othernumber1=1,othernumber2=1;break;}
        default: {othernumber1=1,othernumber2=2;break;}
    }
    for (i=0;i<theplayer->numofcard;i++){
        if (((theplayer->inhand[i].number==11 || theplayer->inhand[i].number==10 || theplayer->inhand[i].number==othernumber1 || theplayer->inhand[i].number==othernumber2 || theplayer->inhand[i].number==6) && theplayer->inhand[i].suit==lastcard.suit) || theplayer->inhand[i].number==lastcard.number){
            canplay[*j]=theplayer->inhand[i];
            (*j)++;
        }
    }
    // need to take the attack
    if (*j==0) {
        if (mode==1){
            thecard=manplaycard(fp,theplayer,canplay,extraturn,*j,theplayer->numofcard,stockpiles,discardpiles,test);
            while (1){
                printf("\nno choice, please enter 0:");
                if (scanf("%d",&choice)==1){};
                if (choice==0){
                    break;
                } else {
                    printf("\nNot valid, try again");
                }
            }
        }
        printf("player %d draws: ",theplayer->rank+1);
        fprintf(fp,"player %d draws: ",theplayer->rank+1);
        for (k=0;k<=(*test)-1;k++){
            thecard=drawcard(stockpiles,theplayer); 
            printcard(fp,thecard,1);
            if (k!=(*test)-1){
                printf(", ");
                fprintf(fp,", ");
            } else {
                fprintf(fp,"\n");
                printf("\n");
            }
        }
        *test=0;
        thecard=lastcard;
    } else {
        // can react to the attack
        if (mode!=1){
            thecard=canplay[rand()%(*j)];
            playcard(discardpiles,theplayer,thecard);
            printf("player %d plays ",theplayer->rank+1);
            fprintf(fp,"player %d plays ",theplayer->rank+1);
            printcard(fp,thecard,1);
        } else {
            thecard=manplaycard(fp,theplayer,canplay,extraturn,*j,theplayer->numofcard,stockpiles,discardpiles,test);
        }
        if (thecard.number!=0){
            if(*test!=0){
                switch (thecard.number){
                    case 1:{
                        printf("add to next player\n");
                        fprintf(fp,"add to next player\n");
                        (*test)+=2;
                        break;
                    }
                    case 2:{
                        printf("add to next player\n");
                        fprintf(fp,"add to next player\n");
                        (*test)+=3;
                        break;
                    }
                    case 6:{
                        printf("cancel attack\n");
                        fprintf(fp,"cancel attack\n");
                        (*test)=0;
                        break;
                    }
                    case 10:{
                        printf("jump attack\n");
                        fprintf(fp,"jump attack\n");
                        (*jtest)++;
                        break;
                    }
                    case 11:{
                        printf("reverse attack\n");
                        fprintf(fp,"reverse attack\n");
                        (*qtest)++;
                        break;
                    }
                    default: break;
                }
            }
        } else {
            thecard=lastcard;
        }
    }
    return thecard;
}
