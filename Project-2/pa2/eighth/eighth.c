#include <stdio.h>
#include <stdlib.h>
#include<math.h>

int bits;
void getUnsigned(double decimal,int** arr,int sizeofArr){
    //double num = floor(decimal);

    for(int i = 0;i<sizeofArr;i++){
        long binVal = 1 << (sizeofArr-i-1);
        if (binVal <= decimal){
            (*arr)[i] = 1;
            decimal = decimal - binVal;
        }
        else{
            (*arr)[i] = 0;
        }
    }

}

void getFractional(double decimal, int** arr,int sizeofArr){
    double num = decimal - floor(decimal);
    for(int i = 0;i<sizeofArr;i++){
        num = 2*num;
        (*arr)[i] = floor(num);
        if(floor(num) == 1){
            num--;
        }
    }
}


int getStartIndex(int* arr1,int* arr2,double decimal,int sizeArr1,int sizeArr2){
    int startInd = 0;
    if(floor(decimal) > 1){
        while(startInd < sizeArr1){
            startInd++;
            if(arr1[startInd - 1] == 1){
                break;
            }
        }
    }
    else if(floor(decimal) == 1){
        startInd = bits;
    }
    else if(floor(decimal) == 0){
        startInd = startInd+bits;
        while((startInd-bits) < sizeArr2){
            startInd++;
            if(arr2[startInd - 1 - bits] == 1){
                break;
            }
        }
    }
    return startInd;
}

int MakeCanonicalArr(int* arr1,int* arr2,int** M,int startInd,int sizeArr1,int sizeArr2){
    int exp = bits - startInd;
    if(exp == 0){
        for(int i = 0;i < bits;i++){
            (*M)[i] = arr2[i];
        }
    }
    else if(exp > 0){
        int count = 0;
        //this for loop copies over arr1 while still space in M
        for(int i = startInd;i<sizeArr1;i++){
            if(count < bits){
                (*M)[count] = arr1[i];
                count++;
            }
        }
        for(int i = 0;i < sizeArr2;i++){
            if(count < bits){
                (*M)[count] = arr2[i];
                count++;
            }
        }
    }
    else{
        int count = 0;
        for(int i = startInd - bits;i < sizeArr2;i++){
            if(count < bits){
                (*M)[count] = arr2[i];
                count++;
            }
        }
        if(count < bits){
            while(count < bits){
                (*M)[count] = 0;
                count++;
            }
        }
    }
    return exp;
}

void printRep(int* arr,int exp){
    printf("%s","1.");
    for(int i = 0;i < bits;i++){
        printf("%d",arr[i]);
    }
    printf(" %d\n",exp);
}

int main(int argc, char* argv[]){
    if (argc > 1){
        FILE* fp = fopen(argv[1],"r");
        double decimal;
        while(fscanf(fp,"%lf %d\n",&decimal,&bits) != EOF){
            int exp = 0;
            long maxRep = (1<<bits)-1;
            double minRep = pow(2.0,bits*-1.0);
            int *M = malloc(sizeof(int)*bits);
            int *arr1;
            int *arr2;
            if(floor(decimal) < maxRep && decimal > minRep){
                arr1 = malloc(sizeof(int)*bits);
                getUnsigned(decimal,&arr1,bits);
                arr2 = malloc(sizeof(int)*bits);
                getFractional(decimal,&arr2,bits);
                exp = MakeCanonicalArr(arr1,arr2,&M,getStartIndex(arr1,arr2,decimal,bits,bits),bits,bits);
            }
            else if(floor(decimal) > maxRep){
                int num = bits;
                long comp = minRep;
                while(comp < decimal){
                    num++;
                    exp++;
                    comp = (1<<num) - 1;
                }
                arr1 = malloc(sizeof(int)*num);
                getUnsigned(decimal,&arr1,num);
                arr2 = malloc(sizeof(int)*bits);
                getFractional(decimal,&arr2,bits);
                exp = exp+MakeCanonicalArr(arr1,arr2,&M,getStartIndex(arr1,arr2,decimal,num,bits),num,bits);
            }
            else{
                int num = -bits;
                while(minRep > decimal){
                    num--;
                    minRep = pow(2.0,num);
                }
                arr1 = malloc(sizeof(int)*bits);
                getUnsigned(decimal,&arr1,bits);
                arr2 = malloc(sizeof(int)*(-num));
                getFractional(decimal,&arr2,-num);
                exp = MakeCanonicalArr(arr1,arr2,&M,getStartIndex(arr1,arr2,decimal,bits,-num),bits,-num);
            }
            printRep(M,exp);
            free(arr1);
            free(arr2);
            free(M);
        }
    }
    return EXIT_SUCCESS;
}