#include <stdio.h>
#include <stdlib.h>
#include<math.h>
#include<limits.h>

void makeBinary(int** arr, long decimal,long numBits){
   unsigned long temp = decimal;
   for(int i = numBits-1; temp > 0;i--){
       (*arr)[i] = temp%2;
       temp /= 2;
   }
}

void getFParr(int* BinaryArr,int** FParr,int FPbits,int Binarybits){
    for(int i = Binarybits-1;i >= 0;i--){
        if(FPbits-1>=0){
            (*FParr)[FPbits-1] = BinaryArr[i];
            FPbits--;
        }
    }

}


int isNormal(int* FParr,int Ebits){
    for(int i = 0;i < Ebits;i++){
        if(FParr[i+1] == 1){
            return 1;
        }
    }
    return 0;
}

int getExp(int* arr,int Ebits){
    long bias = (1<<(Ebits-1))-1;
    long Eval = 0;
    for(int i = 0;i < Ebits;i++){
        long mult = 1<<(Ebits-1-i);
        Eval += mult*arr[i+1];
    }
    if(Eval == 0){
        return 1-bias;
    }
    return Eval-bias;
}

void getM(int* arr,int** M,int Mbits,int Ebits){
    for(int i = 0;i < Mbits;i++){
        (*M)[i] = arr[i+1+Ebits];
    }
}

double convToDouble(int** M,int exp,int isNormal,int sizeofM){
    double multiplier = pow(2.0,1.0*exp);
    double num = 0;
    if(isNormal){num+=multiplier;}
    for(int i = 0;i < sizeofM;i++){
        multiplier /= 2;
        num += (*M)[i] * multiplier;
    }
    return num;
}

void printBinary(int* arr,int arrSize){
    for(int i = 0;i < arrSize;i++){
        printf("%d",arr[i]);
    }
    printf("\n");
}

int main(int argc,char* argv[]){
    if(argc > 1){
        FILE* fp = fopen(argv[1],"r");
        int totalBits;
        int Ebits;
        int Mbits;
        unsigned long hexVal;
        int precision;
        while(fscanf(fp,"%d %d %d %lx %d\n",&totalBits,&Ebits,&Mbits,&hexVal,&precision) != EOF){

            int* BinaryArr = malloc(sizeof(int)*64);
            for(int i = 0;i < 64;i++){
                BinaryArr[i] = 0;
            }
            makeBinary(&BinaryArr,hexVal,64);
            int* FParr = malloc(sizeof(int)*totalBits);
            getFParr(BinaryArr,&FParr,totalBits,64);
            free(BinaryArr);

            int* M = malloc(sizeof(int)*Mbits);
            int exp = getExp(FParr,Ebits);
            getM(FParr,&M,Mbits,Ebits);
            double decimal = convToDouble(&M,exp,isNormal(FParr,Ebits),Mbits);
            if(FParr[0] == 1){decimal = -1.0*decimal;}
            printf("%0.*lf\n",precision,decimal);

            free(FParr);
            free(M);
        }
    }

    return EXIT_SUCCESS;
}