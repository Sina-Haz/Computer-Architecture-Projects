#include <stdio.h>
#include <stdlib.h>

void makeBinary(int** arr, long decimal,long numBits){
    /*double max = pow(2.0,numBits-1.0)-1;
    double min = -1*pow(2.0,numBits-1.0);
    */
   
    long max = (1<<(numBits-1))-1;
    long min = -1*(1<<(numBits-1));
   
    if (decimal >= max){
        (*arr)[0] = 0;
        for (int i = 1;i < numBits;i++){
            (*arr)[i] = 1;
        }
        return;
    }
    else if(decimal <= min){
        (*arr)[0] = 1;
        for (int i = 1;i < numBits;i++){
            (*arr)[i] = 0;
        }
        return;
    }
    else{
        if(decimal >= 0){
            (*arr)[0] = 0;
            for(int i = 1;i < numBits;i++){
                long num = 1<<(numBits-i-1);
                if (num <= decimal){
                    (*arr)[i] = 1;
                    decimal = decimal - num;
                }
                else{
                    (*arr)[i] = 0;
                }
            }
            return;
        }
        else{
            (*arr)[0] = 1;
            for(int i = 1;i < numBits;i++){
                long num = 1<<(numBits-i-1);
                if((min+num) <= decimal){
                    min = min+num;
                    (*arr)[i] = 1;
                }
                else{
                    (*arr)[i] = 0;
                }
            }
            return;
        }
    }
}

void printRep(int* arr,int numBits){
    for(int i = 0;i < numBits;i++){
        printf("%d",arr[i]);
    }
    printf("\n");
}

int main(int argc, char* argv[]){

if(argc > 1){
FILE* fp = fopen(argv[1],"r");
long convNum;
long numBits;
while(fscanf(fp,"%ld %ld\n",&convNum,&numBits) != EOF){
    int* arrRep = malloc(sizeof(int)*numBits);
    makeBinary(&arrRep,convNum,numBits);
    printRep(arrRep,numBits);
    free(arrRep);
}
}


return EXIT_SUCCESS;
}