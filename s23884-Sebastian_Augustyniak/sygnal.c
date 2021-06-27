#include<stdio.h>
#include<unistd.h>
#include<signal.h>

void signalHandler(int);

int main(){

    int x=1;


        while(x!=2)
	{
                signal(SIGINT,&signalHandler);
                printf("Jeśli chcesz zamknąć program wpisz 2 = ");
		scanf("%d",&x);
        }

        return 0;
}

 void signalHandler(int s){

    printf("możesz wierzyć że zadziała %d\n",s);
 }


