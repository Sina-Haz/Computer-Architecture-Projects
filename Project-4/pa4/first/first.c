#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>

typedef struct block{
    unsigned long tag;
    int valid;
    unsigned long timeStamp;
}block;

typedef struct set{
    block* blockArr;
}set;

typedef struct cache{
    set* setArr;
}cache;

unsigned long cacheHit = 0;
unsigned long cacheMiss = 0;
unsigned long memRead = 0;
unsigned long memWrite = 0;
unsigned long global_time = 0;
int pol = 0;


void initializeCache(cache* ptr,int sets, long assoc){
    ptr->setArr = malloc(sizeof(set)*sets);
    for(int i = 0;i < sets;i++){
        ptr->setArr[i].blockArr = malloc(sizeof(block)*assoc);
        for (int j = 0;j < assoc;j++){
            (*ptr).setArr[i].blockArr[j].tag = 0;
            (*ptr).setArr[i].blockArr[j].valid = 0;
            (*ptr).setArr[i].blockArr[j].timeStamp = 0;
        }
    }
}

int allValid(cache* ptr, long setInd, long assoc){
    for(long i = 0;i < assoc;i++){
        if(ptr->setArr[setInd].blockArr[i].valid != 0){
            return 0;
        }
    }
    return 1;
}

int isHit(cache* ptr,unsigned long addr,long assoc,int b, int s,int S){
    unsigned long setInd = (addr >> b) & (S-1);
    unsigned long addrTag = addr >> (b+s);
    for(long i = 0;i < assoc;i++){
        if(ptr->setArr[setInd].blockArr[i].valid != 0 && ptr->setArr[setInd].blockArr[i].tag == addrTag){
            return 1;
        }
    }
    return 0;
}

void clearCache(cache* ptr, int sets, long assoc){
    for(int i = 0;i < sets;i++){
        free(ptr -> setArr[i].blockArr);
    }
    free(ptr -> setArr);
    free(ptr);
}

int isPowerTwo(long num){
    int log = 0;
    while((1<<log) < INT_MAX){
        if((1<<log) == num){
            return log;
        }
        else if((1<<log) > num){
            return -1;
        }
        log++;
    }
    return -1;
}

void updateTime(cache* ptr,unsigned long addr,long assoc,int b,int s,int S){
    unsigned long setInd = (addr >> b) & (S-1);
    unsigned long addrTag = addr >> (b+s);
    for(long i = 0;i < assoc;i++){
        if(ptr->setArr[setInd].blockArr[i].valid != 0 && ptr->setArr[setInd].blockArr[i].tag == addrTag){
            ptr->setArr[setInd].blockArr[i].timeStamp = global_time;
        }
    }
}

int isSetFull(cache* ptr,unsigned long addr,long assoc,int b,int S){
    unsigned long setInd = (addr >> b) & (S-1);
    for(long i = 0;i < assoc;i++){
        if(ptr->setArr[setInd].blockArr[i].valid != 1){
            return 0;
        }
    }
    return 1;
}

int getMinTimeInd(cache* ptr,unsigned long addr,long assoc,int b,int S){
    unsigned long setInd = (addr >> b) & (S-1);
    int min = ptr->setArr[setInd].blockArr[0].timeStamp;
    int minInd = 0;
    for(long i = 0;i < assoc;i++){
        if(ptr->setArr[setInd].blockArr[i].timeStamp < min){
            min = ptr->setArr[setInd].blockArr[i].timeStamp;
            minInd = i;
        }
    }
    return minInd;
}

int getEmptyInd(cache* ptr,unsigned long addr,long assoc,int b,int S){
    unsigned long setInd = (addr >> b) & (S-1);
    for(long i = 0;i < assoc;i++){
        if(ptr->setArr[setInd].blockArr[i].valid == 0){
            return i;
        }
    }
    return -1;
}

void setBlock(cache* ptr,unsigned long addr,long assoc,int b,int s,int S,int index){
    unsigned long setInd = (addr >> b) & (S-1);
    unsigned long addrTag = addr >> (b+s);
    ptr->setArr[setInd].blockArr[index].valid = 1;
    ptr->setArr[setInd].blockArr[index].tag = addrTag;
    ptr->setArr[setInd].blockArr[index].timeStamp = global_time;
}

int main(int argc,char* argv[]){
    if(argc == 6){
        long cache_size = 0;
        long associativity = 0;
        if(!strcmp(argv[3],"fifo")){
            pol = 1;
        }
        else if(!strcmp(argv[3],"lru")){
            pol = 2;
        }
        long block_size = 0;
        FILE* fp = fopen(argv[5],"r");
        sscanf(argv[1],"%li",&cache_size);
        sscanf(argv[2],"assoc:%li",&associativity);
        sscanf(argv[4],"%li",&block_size);
        int b = isPowerTwo(block_size);
        int S = cache_size/(associativity*block_size);
        int s = isPowerTwo(S);
        if(b == -1 || s==-1 || (pol != 1 && pol != 2)){
            printf("error: wrong input\n");
            return EXIT_SUCCESS;
        }
        cache* ptr = malloc(sizeof(cache));
        initializeCache(ptr,S,associativity);
        unsigned long addr;
        char mode;
        while(fscanf(fp,"%c %lx\n",&mode,&addr) != EOF){
            global_time++;
            int hitBool = isHit(ptr,addr,associativity,b,s,S);
            if(mode == 'W'){
                memWrite++;
            }
            if(hitBool == 1){
                cacheHit++;
                if(pol == 2){
                    updateTime(ptr,addr,associativity,b,s,S);
                }
            }
            else{
                cacheMiss++;
                memRead++;
                if(isSetFull(ptr,addr,associativity,b,S)){
                    setBlock(ptr,addr,associativity,b,s,S,getMinTimeInd(ptr,addr,associativity,b,S));
                }
                else{
                    setBlock(ptr,addr,associativity,b,s,S,getEmptyInd(ptr,addr,associativity,b,S));
                }
            }
        }
        printf("memread:%lu\nmemwrite:%lu\ncachehit:%lu\ncachemiss:%lu\n",memRead,memWrite,cacheHit,cacheMiss);
        fclose(fp);
        clearCache(ptr,S,associativity);
    }
    return EXIT_SUCCESS;
}