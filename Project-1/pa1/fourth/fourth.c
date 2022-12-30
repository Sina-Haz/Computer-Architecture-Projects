#include<stdio.h>
#include<stdlib.h>

int** createSquare(int n){
    int** arr = (int**) malloc(sizeof(int*)*n);
    for(int i = 0;i < n;i++){
        int* tempArr = (int*) malloc(sizeof(int)*n);
        for(int j = 0;j<n;j++){
            tempArr[j] = 0;
        }
        arr[i] = tempArr;
    }

    int row = 0;
    int col = (int) n/2;
    int count = 1;
    arr[row][col] = count;

    while(count < (n*n)){
        count++;
        row = row-1;
        col = col+1;
        if(row < 0 && col < n){
            if(arr[n-1][col] != 0){
                row = row+2;
                col = col -1;
            }
            else{
                row = n-1;
            }
        }
        if(col == n && row >= 0){
            if(arr[row][0] != 0){
                row = row+2;
                col = col-1;
            }
            else{
                col = 0;
            }
        }
        if(row < 0 && col == n){
            if(arr[n-1][0] != 0){
                row = row+2;
                col = col-1;
            }
            else{
                row = n-1;
                col = 0;
            }
        }
        else if(arr[row][col] != 0){
            row = row+2;
            col = col-1;
        }
        
        arr[row][col] = count;
    }
    return arr;
}

void printSquare(int** square,int len){
    for(int i = 0;i < len;i++){
        for(int j = 0;j < len;j++){
            printf("%d\t",square[i][j]);
        }
        printf("\n");
    }
}

void freeMatrix(int** matrix, int n){
    for(int i = 0;i<n;i++){
        free(matrix[i]);
    }
    free(matrix);
}

int main(int argc, char* argv[]){

    int n;
    if(argc < 2){
    n = 0;
    }
    else{
     n = atoi(argv[1]);
    }
    if(n%2==0){
        printf("error\n");
    }
    else{
        int** arr= createSquare(n);
        printSquare(arr,n);
        freeMatrix(arr,n);
    }

    return EXIT_SUCCESS;
}
