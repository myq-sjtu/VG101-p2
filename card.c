// this c file is all the basic function in the game
#include"card.h"

// generate the stock pile
void generate(int n,pile *stockpiles){
    int i,j,k;
    stockpiles->num=0;
    card thecard;
    for (k=0;k<=n-1;k++){
        for (i=0;i<=12;i++){
            for (j=0;j<=3;j++){
                thecard.number=i+1;
                thecard.suit=j+1;
                stockpiles->allcard[stockpiles->num]=thecard;
                stockpiles->num++;
            }
        }
    }
}

// shuffle the pile
void shuffle(pile *stockpiles,int n){
    int i,j,num1,num2;
    card temp;
    srand((unsigned)time(NULL));
    for (i=0;i<=10000;i++){
        for (j=0;j<=10;j++){
            num1=rand()%(n);
            num2=rand()%(n);
            temp=stockpiles->allcard[num1];
            stockpiles->allcard[num1]=stockpiles->allcard[num2];
            stockpiles->allcard[num2]=temp;
        }
    }
}

// one player draw one card
card drawcard(pile *stockpiles,player *theplayer){
    card thecard;
    int i;
    if (stockpiles->allcard[0].number<14 && stockpiles->allcard[0].number>0 && stockpiles->allcard[0].suit>0 && stockpiles->allcard[0].suit<5){
        thecard=stockpiles->allcard[0];
    } else {
        for (i=0;i<stockpiles->num-1;i++){
            stockpiles->allcard[i]=stockpiles->allcard[i+1];
        }
        stockpiles->num--;
        thecard=stockpiles->allcard[0];
    }
    *(theplayer->inhand+theplayer->numofcard)=thecard;
    theplayer->numofcard++;
    for (i=0;i<stockpiles->num-1;i++){
        stockpiles->allcard[i]=stockpiles->allcard[i+1];
    }
    stockpiles->num--;
    if (stockpiles->num<0){
        printf("not enough card");
        exit(1);
    }
    return thecard;
}

// player play card
void playcard(pile *discardpile,player *theplayer,card thecard){
    int i,j;
    for (i=0;i<theplayer->numofcard;i++){
        if ((thecard.number==theplayer->inhand[i].number&&(thecard.suit==theplayer->inhand[i].suit))){
            if (i!=theplayer->numofcard-1){
                for (j=i;j<theplayer->numofcard;j++){
                    theplayer->inhand[j]=theplayer->inhand[j+1];
                }
                theplayer->numofcard--;
                break;
            } else {
                theplayer->numofcard--;
            }
        }
    }
    for (i=discardpile->num-1;i>0;i--){
        discardpile->allcard[i]=discardpile->allcard[i-1];
    }
    discardpile->allcard[0]=thecard;
    discardpile->num++;
}

// print the card (int->string)
void printcard(FILE *fp, card thecard,int mode){
    switch (thecard.suit){
        case 1:{
            if (mode!=0) printf("Spades ");
            if (mode!=2) fprintf(fp,"Spades ");
            break;
        }
        case 2:{
            if (mode!=0) printf("Hearts ");
            if (mode!=2) fprintf(fp,"Hearts ");
            break;
        }
        case 3:{
            if (mode!=0) printf("Diamonds ");
            if (mode!=2)fprintf(fp,"Diamonds ");
            break;
        }
        case 4:{
            if (mode!=0) printf("Clubs ");
            if (mode!=2) fprintf(fp,"Clubs ");
            break;
        }
        default:
            printf("Not enough card,start again");
            exit(1);
            break;
    }
    switch (thecard.number){
        case 10:{
            if (mode!=0) printf("Jack ");
            if (mode!=2) fprintf(fp,"Jack ");
            break;
        }
        case 11:{
            if (mode!=0) printf("Queen ");
            if (mode!=2) fprintf(fp,"Queen ");
            break;
        }
        case 12:{
            if (mode!=0) printf("King ");
            if (mode!=2) fprintf(fp,"King ");
            break;
        }
        case 13:{
            if (mode!=0) printf("Ace ");
            if (mode!=2)fprintf(fp,"Ace ");
            break;
        }
        default:{
            if (mode!=0) printf("%d ",thecard.number+1);
            if (mode!=2) fprintf(fp,"%d ",thecard.number+1);
            break;
        }
    }
}

// print the template
void printlog(pile *stockpiles,int mode,FILE *fp,int round,int deck,int nofplayers){
    int i;
    fprintf(fp,"########################\n"
           "#                      #\n"
           "# Welcome to One Card! #\n"
           "#                      #\n"
           "########################\n\n"
           "---- Initial setup ----\n");
    fprintf(fp,"Number of rounds: %d\n",round);
    fprintf(fp,"Number of decks: %d\n",deck);
    fprintf(fp,"Number of players: %d\n\n",nofplayers);
    fprintf(fp,"Shuffling cards...\n");
    fprintf(fp,"Shuffle result: ");
    printf("########################\n"
           "#                      #\n"
           "# Welcome to One Card! #\n"
           "#                      #\n"
           "########################\n\n"
           "---- Initial setup ----\n");
    printf("Number of rounds: %d\n",round);
    printf("Number of decks: %d\n",deck);
    printf("Number of players: %d\n\n",nofplayers);
    printf("Shuffling cards...\n");
    printf("Shuffle result: ");
    if (mode!=1){
        for (i=0;i<stockpiles->num;i++){
            printcard(fp,stockpiles->allcard[i],1);
        }
    }else {
        for (i=0;i<stockpiles->num;i++){
            printcard(fp,stockpiles->allcard[i],0);
        }
    }
    printf("\nDealing cards...\n\n\n");
    fprintf(fp,"\nDealing cards...\n\n\n");
}

// print the help
void printhelp(){
    printf("-h|--help                print this help message\n");
    printf("--log filename           write the logs in filename (default: onecard.log)\n");
    printf("-n n|--player-number=n   n players, n must be larger than 2 (default: 4)\n");
    printf("-c c|--initial-cards=c   deal c cards per player, c must be at least 2 (default: 5)\n");
    printf("-d d|--decks=d           use d decks 52 cards each, d must be at least 2 (default: 2)\n");
    printf("-r r|--rounds=r          play r rounds, r must be at least 1 (default: 1)\n");
    printf("-a|--auto                run in demo mode\n\n");
}

// add card from discard pile to stock pile
void addcard(pile *stockpiles,pile *discardpiles){
    int i=0;
    for (i=stockpiles->num;i<stockpiles->num+discardpiles->num;i++){
        stockpiles->allcard[i]=discardpiles->allcard[i-stockpiles->num];
        discardpiles->allcard[i-stockpiles->num].number=0;
        discardpiles->allcard[i-stockpiles->num].suit=0;
    }
    stockpiles->num+=discardpiles->num;
    discardpiles->num=0;
}

// in main mode prompt the user to play card
card manplaycard(FILE *fp,player *theplayer,card *canplay,int *turn,int j,int num,pile *stockpiles, pile *discardpiles,int *test){
    int i,choice,valid=0;
    card thecard;
    printf("player%d, You now have\n",theplayer->rank+1);
    for (i=0;i<=num-1;i++){
        printcard(fp,theplayer->inhand[i],2);
    }
    fprintf(fp,"\n");
    if (j!=0){
        while (1){
            printf("\nchoose from 1 to %d, or enter 0 to jump the round:",num);
            if (scanf("%d",&choice)==1){};
            if (choice>0 && choice<=theplayer->numofcard){
                for (i=0;i<theplayer->numofcard;i++){
                    if ((theplayer->inhand[choice-1].number==canplay[i].number) && (theplayer->inhand[choice-1].suit==canplay[i].suit)){
                        thecard=theplayer->inhand[choice-1];
                        valid=1;
                    }
                }
                if(valid==1){
                    playcard(discardpiles,theplayer,thecard);
                    printf("player %d plays: ",theplayer->rank+1);
                    fprintf(fp,"player %d plays: ",theplayer->rank+1);
                    printcard(fp,thecard,1);
                }
            } else {
                if (choice==0){
                    if (*test==0){
                        if (*turn!=1){
                            thecard=drawcard(stockpiles,theplayer);
                            printf("player %d draws: ",theplayer->rank+1);
                            fprintf(fp,"player %d draws: ",theplayer->rank+1);
                            printcard(fp,thecard,1);
                            *turn=1;
                        }
                        valid=1;
                        thecard.number=0;
                    } else {
                        if(*turn!=1){
                            printf("player %d draws: ",theplayer->rank+1);
                            fprintf(fp,"player %d draws: ",theplayer->rank+1);
                            for (i=0;i<=(*test)-1;i++){
                                thecard=drawcard(stockpiles,theplayer); 
                                printcard(fp,thecard,1); 
                            }
                            *test=0;
                            *turn=1;
                        }
                        thecard.number=0;
                        valid=1;
                    }
                }
            }
            if (valid==1){
                break;
            } else {
                printf("not valid, try again!\n");
            }
        }
    }
    return thecard;
}

// sort the card
void sortcard(card *inhand,int num){
    int i,j,rank;
    card tmp,min;
    for (i=0;i<num;i++){
        min=inhand[i];
        for (j=i;j<num;j++){
            if (inhand[j].suit*13+inhand[j].number<=min.suit*13+min.number){
                min=inhand[j];
                rank=j;
            }
        }
        tmp=inhand[i];
        inhand[i]=inhand[rank];
        inhand[rank]=tmp;
    }
}
