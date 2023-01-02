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

unsigned long l1cacheHit = 0;
unsigned long l1cacheMiss = 0;
unsigned long l2cacheHit = 0;
unsigned long l2cacheMiss = 0;
unsigned long memRead = 0;
unsigned long memWrite = 0;
unsigned long global_time = 0;
unsigned long assoc1 = 0;
unsigned long assoc2 = 0;
unsigned long S1 = 0;
unsigned long S2 = 0;
unsigned long s1 = 0;
unsigned long s2 = 0;
unsigned long b = 0;
int pol1 = 0;
int pol2 = 0;

void initializeCache(cache* ptr,unsigned long S,unsigned long assoc){
    ptr->setArr = malloc(sizeof(set)*S);
    for(int i = 0;i < S;i++){
        ptr->setArr[i].blockArr = malloc(sizeof(block)*assoc);
        for (int j = 0;j < assoc;j++){
            (*ptr).setArr[i].blockArr[j].tag = 0;
            (*ptr).setArr[i].blockArr[j].valid = 0;
            (*ptr).setArr[i].blockArr[j].timeStamp = 0;
        }
    }
}

int isPowerTwo(unsigned long num){
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

unsigned long blockToAddr(block Block,unsigned long setInd,int cacheNum){
    unsigned long addr = 0;
    if(cacheNum == 1){
        addr = ((Block.tag << s1) + setInd) << b;
    }
    else if(cacheNum == 2){
        addr = ((Block.tag << s2) + setInd) << b;
    }
    return addr;
}

int isHit(cache* ptr, unsigned long addr,int cacheNum){
    unsigned long setInd = 0;
    unsigned long addrTag = 0;
    unsigned long assoc = 0;
    if(cacheNum == 1){
        setInd = (addr >> b) & (S1-1);
        addrTag = addr >> (b + s1);
        assoc = assoc1;
    }
    else if(cacheNum == 2){
        setInd = (addr >> b) & (S2-1);
        addrTag = addr >> (b + s2);
        assoc = assoc2;
    }
    for (long i = 0;i < assoc;i++){
        if(ptr->setArr[setInd].blockArr[i].valid == 1 && ptr->setArr[setInd].blockArr[i].tag == addrTag){
            return 1;
        }
    }
    return 0;
}

void updateTime(cache* ptr,unsigned long addr){
    unsigned long setInd = (addr >> b) & (S1-1);
    unsigned long addrTag = addr >> (b + s1);
    for(long i = 0;i < assoc1;i++){
        if(ptr->setArr[setInd].blockArr[i].valid != 0 && ptr->setArr[setInd].blockArr[i].tag == addrTag){
            ptr->setArr[setInd].blockArr[i].timeStamp = global_time;
        }
    }
}

int getMinTimeInd(cache* ptr,unsigned long addr,int cacheNum){
    unsigned long setInd = 0;
    unsigned long assoc = 0;
    if(cacheNum == 1){
        setInd = (addr >> b) & (S1-1);
        assoc = assoc1;
    }
    else if(cacheNum == 2){
        setInd = (addr >> b) & (S2-1);
        assoc = assoc2;
    }
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

int getEmptyInd(cache* ptr,unsigned long addr,int cacheNum){
    unsigned long setInd = 0;
    unsigned long assoc = 0;
    if(cacheNum == 1){
        setInd = (addr >> b) & (S1-1);
        assoc = assoc1;
    }
    else if(cacheNum == 2){
        setInd = (addr >> b) & (S2-1);
        assoc = assoc2;
    }
    for(long i = 0;i < assoc;i++){
        if(ptr->setArr[setInd].blockArr[i].valid == 0){
            return i;
        }
    }
    return -1;
}

int isSetFull(cache* ptr,unsigned long addr,int cacheNum){
    unsigned long setInd = 0;
    unsigned long assoc = 0;
    if(cacheNum == 1){
        setInd = (addr >> b) & (S1-1);
        assoc = assoc1;
    }
    else if(cacheNum == 2){
        setInd = (addr >> b) & (S2-1);
        assoc = assoc2;
    }
    for(long i = 0;i < assoc;i++){
        if(ptr->setArr[setInd].blockArr[i].valid == 0){
            return 0;
        }
    }
    return 1;
}

void setBlock(cache* ptr,unsigned long addr,int cacheNum,int index){
    unsigned long setInd = 0;
    unsigned long addrTag = 0;
    if(cacheNum == 1){
        setInd = (addr >> b) & (S1-1);
        addrTag = addr >> (b + s1);
    }
    else if(cacheNum == 2){
        setInd = (addr >> b) & (S2-1);
        addrTag = addr >> (b + s2);
    }
    ptr->setArr[setInd].blockArr[index].valid = 1;
    ptr->setArr[setInd].blockArr[index].tag = addrTag;
    ptr->setArr[setInd].blockArr[index].timeStamp = global_time;
}

void evictFromL1(cache* ptr1,cache* ptr2,unsigned long addr, long EvictInd){
    unsigned long setInd1 = (addr >> b) & (S1-1);
    block BlockToEvict = ptr1->setArr[setInd1].blockArr[EvictInd];
    unsigned long newAddr = blockToAddr(BlockToEvict,setInd1,1);
    if(isSetFull(ptr2,newAddr,2)){
        setBlock(ptr2,newAddr,2,getMinTimeInd(ptr2,newAddr,2));
    }
    else{
        setBlock(ptr2,newAddr,2,getEmptyInd(ptr2,newAddr,2));
    }
    setBlock(ptr1,addr,1,EvictInd);
}

void RemoveFromL2(cache* ptr,unsigned long addr){
    unsigned long setInd2 = (addr >> b) & (S2-1);
    unsigned long addrTag = addr >> (b+s2);
    for(long i = 0;i< assoc2;i++){
        if (ptr->setArr[setInd2].blockArr[i].tag == addrTag){
            ptr->setArr[setInd2].blockArr[i].tag = 0;
            ptr->setArr[setInd2].blockArr[i].valid = 0;
            ptr->setArr[setInd2].blockArr[i].timeStamp = 0;
        }
    }
}

void transferToL1(cache* ptr1,cache* ptr2,unsigned long addr,long L1Ind){
    setBlock(ptr1,addr,1,L1Ind);
    unsigned long setInd2 = (addr >> b) & (S2-1);
    unsigned long addrTag = addr >> (b+s2);
    for(long i = 0;i < assoc2;i++){
        if(ptr2->setArr[setInd2].blockArr[i].tag == addrTag){
            ptr2->setArr[setInd2].blockArr[i].valid = 0;
            ptr2->setArr[setInd2].blockArr[i].tag = 0;
            ptr2->setArr[setInd2].blockArr[i].timeStamp = 0;
            break;
        }
    }
}

void clearCache(cache* ptr, int S, long assoc){
    for(int i = 0;i < S;i++){
        free(ptr -> setArr[i].blockArr);
    }
    free(ptr -> setArr);
    free(ptr);
}

int main(int argc,char* argv[]){
    if(argc == 9){
        unsigned long l1cachesize = 0;
        sscanf(argv[1],"%lu",&l1cachesize);
        sscanf(argv[2],"assoc:%lu",&assoc1);
        if(!strcmp(argv[3],"fifo")){
            pol1 = 1;
        }
        else if(!strcmp(argv[3],"lru")){
            pol1 = 2;
        }
        unsigned long block_size = 0;
        sscanf(argv[4],"%lu",&block_size);
        unsigned long l2cachesize = 0;
        sscanf(argv[5],"%lu",&l2cachesize);
        sscanf(argv[6],"assoc:%lu",&assoc2);
        if(!strcmp(argv[7],"fifo")){
            pol2 = 1;
        }
        else if(!strcmp(argv[7],"lru")){
            pol2 = 2;
        }
        b = isPowerTwo(block_size);
        S1 = l1cachesize/(assoc1*block_size);
        s1 = isPowerTwo(S1);
        S2 = l2cachesize/(assoc2*block_size);
        s2 = isPowerTwo(S2);
        FILE* fp = fopen(argv[8],"r");
        if(b == -1 || s1 == -1 || s2 == -1 || pol1 == 0 || pol2 == 0){
            printf("error: wrong input\n");
            return EXIT_SUCCESS;
        }
        cache* l1ptr = malloc(sizeof(cache));
        cache* l2ptr = malloc(sizeof(cache));
        initializeCache(l1ptr,S1,assoc1);
        initializeCache(l2ptr,S2,assoc2);
        unsigned long addr;
        char mode;
        while(fscanf(fp,"%c %lx\n",&mode,&addr) != EOF){
            global_time++;
            if(isHit(l1ptr,addr,1) && isHit(l2ptr,addr,2)){
                printf("error: not exclusive");
            }
            if(mode == 'W'){memWrite++;}
            if(isHit(l1ptr,addr,1)){
                l1cacheHit++;
                if(pol1 == 2){
                    updateTime(l1ptr,addr);
                }
            }
            else{
                l1cacheMiss++;
                if(isHit(l2ptr,addr,2)){
                    l2cacheHit++;
                    RemoveFromL2(l2ptr,addr);
                    if(isSetFull(l1ptr,addr,1)){
                        long indToEvict = getMinTimeInd(l1ptr,addr,1);
                        evictFromL1(l1ptr,l2ptr,addr,indToEvict);
                    }
                    else{
                        long indToAdd = getEmptyInd(l1ptr,addr,1);
                        transferToL1(l1ptr,l2ptr,addr,indToAdd);
                    }
                }
                else{
                    l2cacheMiss++;
                    memRead++;
                    if(isSetFull(l1ptr,addr,1)){
                        long indToEvict = getMinTimeInd(l1ptr,addr,1);
                        evictFromL1(l1ptr,l2ptr,addr,indToEvict);
                    }
                    else{
                        setBlock(l1ptr,addr,1,getEmptyInd(l1ptr,addr,1));
                    }
                }
            }
        }
        printf("memread:%lu\nmemwrite:%lu\nl1cachehit:%lu\nl1cachemiss:%lu\nl2cachehit:%lu\nl2cachemiss:%lu\n",memRead,memWrite,l1cacheHit,l1cacheMiss,l2cacheHit,l2cacheMiss);
        clearCache(l1ptr,S1,assoc1);
        clearCache(l2ptr,S2,assoc2);
        fclose(fp);
    }

    return EXIT_SUCCESS;
}