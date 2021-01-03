// all the structures are defined in card.h
#include "mode.h"

void getpara(int argc,char* argv[],int *players,int* cards, int* decks,int* rounds,int* modes,char *file,int* opentest);

// main function
int main(int argc,char* argv[]){
    // initial parameters
    int players=3,cards=3,rounds=2,decks=2,modes=1,opentest=0;
    FILE *fp;
	char file[2000];
    //reset seed
    srand((unsigned)time(NULL));
    getpara(argc,argv,&players,&cards,&decks,&rounds,&modes,file,&opentest);
    if (players*(cards+1)>=decks*52){
        printf("Won't be enough card, start again! Or add '-h' for help");
        }else{
            if(modes!=3){
                if(opentest==1){
                    printf("%s",file);
                    //fp=fopen(file,"w+");
                    /*if (fp==NULL){
                        printf("no such file!");
                        exit(1);
                    }*/
                    mode(cards,players,decks,rounds,fp,modes);
                } else {
                    //fp=fopen("onecard.log","w");
                    /*if (fp==NULL){
                        printf("no such file!");
                        exit(1);
                    }*/
                    mode(cards,players,decks,rounds,fp,modes);
                }
            }   
        }
    return 0;
}

// this function is to get the parameters from the command line
void getpara(int argc,char* argv[],int *players,int* cards, int* decks,int* rounds,int* modes,char *file,int* opentest){
    char tmp[5][20];// to store the partial string if enter something like "--player-number=n"
    int i,j,num=0,sum,k;
    if ((argc==2) && ((!strcmp(argv[1],"-h") || (!strcmp(argv[1],"--help"))))){
        printhelp();
        *modes=3;
    } else {
        // to get the parameters
        if (argc>=2){
            for (i=0;i<=argc-1;i++){
                if (!strcmp(argv[i],"-d")){
                    sum=0;
                    for (k=0;(unsigned)k<strlen(argv[i+1]);k++){
                        sum=sum*10+argv[i+1][k]-'0';
                    }
                    *decks=sum;
                }
                if (!strcmp(argv[i],"-r")){
                    sum=0;
                    for (k=0;(unsigned)k<strlen(argv[i+1]);k++){
                        sum=sum*10+argv[i+1][k]-'0';
                    }
                    *rounds=sum;
                }
                if (!strcmp(argv[i],"-c")){
                    sum=0;
                    for (k=0;(unsigned)k<strlen(argv[i+1]);k++){
                        sum=sum*10+argv[i+1][k]-'0';
                    }
                    *cards=sum;
                }
                if (!strcmp(argv[i],"-n")){
                    sum=0;
                    for (k=0;(unsigned)k<strlen(argv[i+1]);k++){
                        sum=sum*10+argv[i+1][k]-'0';
                    }
                    *players=sum;
                }
                if (!strcmp(argv[i],"-a")){
                    *modes=2;
                }
                if (!strcmp(argv[i],"--log")){
                    strcpy(file,argv[i+1]);
                    *opentest=1;
                }
                // get parameters if enter in the second way
				if (argv[i][0]=='-' && argv[i][1]=='-' && argv[i][2]!='l'){
					for (j=0;(unsigned)j<strlen(argv[i]);j++){
						if (argv[i][j]=='='){
							strncpy(tmp[num],argv[i],(unsigned)j+1);
							tmp[num][j+1]='\0';
							if (!strcmp(tmp[num],"--player-number=")){
								sum=0;
								for (k=j+1;(unsigned)k<strlen(argv[i]);k++){
									sum=sum*10+argv[i][k]-'0';
								}
								*players=sum;
								num++;
							}
							if (!strcmp(tmp[num],"--initial-cards=")){
								sum=0;
								for (k=j+1;(unsigned)k<strlen(argv[i]);k++){
									sum=sum*10+argv[i][k]-'0';
								}
								*cards=sum;
								num++;
							}
							if (!strcmp(tmp[num],"--decks=")){
								sum=0;
								for (k=j+1;(unsigned)k<strlen(argv[i]);k++){
									sum=sum*10+argv[i][k]-'0';
								}
								*decks=sum;
								num++;
							}
							if (!strcmp(tmp[num],"--rounds=")){
								sum=0;
								for (k=j+1;(unsigned)k<strlen(argv[i]);k++){
									sum=sum*10+argv[i][k]-'0';
								}
								*rounds=sum;
								num++;
							}
						}
					}
				}
            }
        }
    }
}
