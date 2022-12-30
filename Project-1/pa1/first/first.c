#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include<math.h>

long isPrime(long num){
   if (num == 1){
	return 0;
   }
   if ( num == 2){
     return 1;
   }
   for(long i = 2;i <= sqrt(num);i++){
       if (num % i == 0){
           return 0;
       }
   }
   return 1;
}


int main(int argc,char* argv[]){

if (access(argv[1],F_OK) != 0){
    printf("error\n");
    return EXIT_SUCCESS;
}

FILE* fp = fopen(argv[1],"r");
long num_primes;
fscanf(fp,"%ld\n",&num_primes);
for(long i = 0;i<num_primes;i++){
    long isprime;
    int isAllPrime = 1;

    if(fscanf(fp,"%ld\n",&isprime) != EOF){
        while(isprime > 0){
            if(isPrime(isprime) != 1){
                isAllPrime = 0;
                break;
            }
            isprime = isprime/10;
        }
        if(isAllPrime == 1){
            printf("yes\n");
        }
        else{
            printf("no\n");
        }
    }
}

fclose(fp);

return EXIT_SUCCESS;

}
