#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct vertex{
    char* Vname;
    int edgeWeight;
    struct vertex* next;

}vertex;

long numVertices;
vertex* visited;
int isNewNode(char* name);
void Push(vertex** stackHead,char* name,int edgeWeight);
void Pop(vertex** stackHead);
void AddToVisit(char* name,int edgeWeight);


void createVertex(vertex** list,char* name){
    int count = 0;
    while((*list)[count].Vname != NULL && count < numVertices){
        count++;
    }
    (*list)[count].Vname = malloc(sizeof(char)*85);
    strcpy((*list)[count].Vname,name);
    (*list)[count].edgeWeight = 0;
    (*list)[count].next=NULL;
}

void createEdge(vertex** list,char* head, char* addOn,int weight){
    vertex* New = malloc(sizeof(vertex));
    New -> Vname = malloc(sizeof(char)*85);
    strcpy(New->Vname,addOn);
    New->edgeWeight = 0;
    New -> next = NULL;

    int count = 0;
    while(strcmp(head,(*list)[count].Vname) != 0 && count < numVertices){
        count++;
    }

    vertex* ptr = &(*list)[count];
    if (ptr -> next == NULL || strcmp(New -> Vname,ptr -> next -> Vname) < 0){
        New -> next = ptr -> next;
        New -> edgeWeight = weight;
        ptr -> next = New;
    }
    else{
        while(ptr -> next != NULL && (strcmp(New->Vname,ptr -> next -> Vname) > 0)){
            ptr = ptr -> next;
        }
        New -> next = ptr -> next;
        New -> edgeWeight = weight;
        ptr -> next = New;
    }
}

void DFS(vertex** list,char* start,int weight,vertex** stackHead){
    Push(stackHead,start,weight);

    int count = 0;
    while(strcmp((*list)[count].Vname,start) != 0 && count < numVertices){
        count++;
    }

    vertex* ptr = &(*list)[count];
    while(ptr -> next != NULL){
        ptr = ptr -> next;
        if(isNewNode(ptr->Vname) == 0){
            DFS(list,ptr->Vname,ptr->edgeWeight,stackHead);
        }
    }
    Pop(stackHead);
}

int isNewNode(char* name){
    int num = 0;
    vertex* ptr = visited;
    if(ptr -> Vname == NULL){
        return num;
    }
    else{
        while(ptr != NULL){
            if(strcmp(name,ptr->Vname) == 0){
                return 1;
            }
            ptr = ptr -> next;
        }
    }
    return num;
}


void Push(vertex** stackHead,char* name,int edgeWeight){
    vertex* tempStack = malloc(sizeof(vertex));
    tempStack -> Vname = malloc(sizeof(char)*85);
    strcpy(tempStack->Vname,name);
    tempStack->edgeWeight=edgeWeight;
    tempStack->next = NULL;

    if((*stackHead)->Vname == NULL){
        vertex* temp = (*stackHead);
        *stackHead = tempStack;
        free(temp);
    }
    else{
        tempStack->next = *stackHead;
        *stackHead = tempStack;
    }
    vertex* newVisit = malloc(sizeof(vertex));
    newVisit -> Vname = malloc(sizeof(char)*85);
    strcpy(newVisit->Vname,name);
    newVisit->edgeWeight = edgeWeight;
    newVisit -> next = NULL;

    vertex* ptr = visited;
    if(visited->Vname == NULL){
        visited = newVisit;
        free(ptr);
    }
    else{
        while(ptr -> next != NULL){
            ptr = ptr->next;
        }
        ptr->next = newVisit;
    }

}

void Pop(vertex** stackHead){
    vertex* temp;
    temp = *stackHead;
    if((*stackHead) -> Vname != NULL){
        *stackHead = (*stackHead) -> next;
        free(temp->Vname);
        free(temp);
    }
}

int compareFunction(const void* Vname1,const void* Vname2){
    return strcmp(*(const char**)Vname1,*(const char**)Vname2);
}

void sortGraph(vertex** list){
    qsort(*list,numVertices,sizeof(vertex),compareFunction);
    /*
    for(int i = 0;i < numVertices;i++){
        vertex* ptr = &(*list)[i];
        printf("%s ",ptr->Vname);
    }
    printf("\n");
    */
}

void visitExtras(vertex** list){
    for(int i = 0;i < numVertices;i++){
        vertex* listPtr = &(*list)[i];
        if(isNewNode(listPtr->Vname) == 0){
            vertex* stack = malloc(sizeof(vertex));
            stack -> Vname = NULL;
            stack ->edgeWeight = 0;
            stack -> next = NULL;
            DFS(list,listPtr->Vname,listPtr->edgeWeight,&stack);
        }
    }
}

void AddToVisit(char* name,int edgeWeight){
    vertex* newVis = malloc(sizeof(vertex));
    newVis->Vname = malloc(sizeof(char)*85);
    strcpy(newVis->Vname,name);
    newVis -> edgeWeight = edgeWeight;
    newVis->next = NULL;

    vertex* ptr = visited;
    if(ptr -> Vname == NULL){
        visited = newVis;
        free(ptr);
    }
    else{
    while(ptr -> next != NULL){
        ptr = ptr->next;
    }
    ptr -> next = newVis;
    }
}

void printVisits(){
    vertex* ptr = visited;
    while(ptr != NULL){
        printf("%s ",ptr->Vname);
        ptr = ptr->next;
    }
    printf("\n");
}

void freeVisits(){
    vertex* temp;
    while(visited != NULL){
        temp = visited;
        visited = visited -> next;
        free(temp->Vname);
        free(temp);
    }
}

void deleteAll(vertex** list){
    vertex* temp;
    for(int i = 0;i < numVertices;i++){
        vertex* ptr = &(*list)[i];
        free(ptr->Vname);
        ptr = ptr -> next;
        while(ptr != NULL){
            temp = ptr;
            ptr = ptr->next;
            free(temp -> Vname);
            free(temp);
        }
    }
    free(*list);
}

int main(int argc,char* argv[]){

    if(argc > 1){
        FILE* graphFile = fopen(argv[1],"r");
        
        long numVerts;
        fscanf(graphFile,"%ld\n",&numVerts);
        vertex* adjList = malloc(sizeof(vertex)*numVerts);
        numVertices = numVerts;

        for (int i = 0;i < numVertices;i++){
            adjList[i].Vname = NULL;
            adjList[i].edgeWeight = 0;
            adjList[i].next = NULL;
        }

        char* newName = malloc(sizeof(char)*85);
        for (int i = 0;i < numVertices;i++){
            fscanf(graphFile,"%s\n",newName);
            createVertex(&adjList,newName);
        }
        free(newName);
        
        sortGraph(&adjList);

        char* name1 = malloc(sizeof(char)*85);
        char* name2 = malloc(sizeof(char)*85);
        int weight;
        while(fscanf(graphFile,"%s %s %d\n",name1,name2,&weight) != EOF){
            createEdge(&adjList,name1,name2,weight);
        }

        vertex* stackHead = malloc(sizeof(vertex));
        stackHead->Vname = NULL;
        stackHead -> edgeWeight = 0;
        stackHead->next = NULL;

        visited = malloc(sizeof(vertex));
        visited->edgeWeight=0;
        visited -> Vname = NULL;
        visited -> next = NULL;

        DFS(&adjList,(*adjList).Vname,(*adjList).edgeWeight,&stackHead);
        visitExtras(&adjList);
        printVisits();

        freeVisits();
        deleteAll(&adjList);

        free(name1);
        free(name2);
    }

    return EXIT_SUCCESS;
}