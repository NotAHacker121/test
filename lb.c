#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>

void main(){
    int bsize,outrate;
    printf("Enter bucket size : ");
    scanf("%d" , &bsize);
    printf("Enter outgoing rate : ");
    scanf("%d" , &outrate);
    if(outrate > bsize)
    {
        printf("\nOutgoing rate greater than bukcet size ! Error");
        return;
    }
    int n;
    printf("Enter the number of inputs : ");
    scanf("%d", &n);
    int stored = 0;
    while(n!= 0){
        int incpac;
        printf("\n\nEnter incoming packet size : ");
        scanf("%d",&incpac);
        if(incpac<=(bsize - stored)){
            stored+=incpac;
            printf("\nbucket buffer size %d out of %d filled !",stored, bsize);
        }
        else{
            printf("\ndropped %d number of packets!",incpac-(bsize-stored));
            stored=bsize;
            printf("\nbucket buffer size %d out of %d filled !",stored, bsize);
        }
        stored-=outrate;
        if(stored<0)
            stored = 0;
        printf("\nAfter outgoing %d packet left out of %d in buffer",stored, bsize);
        n-=1;
    }
}