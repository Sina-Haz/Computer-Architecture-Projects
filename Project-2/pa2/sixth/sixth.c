#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>

typedef struct vertex{
    char* Vname;
    int edgeWeight;
    struct vertex* next;

}vertex;

long numVertices;
vertex* visited;

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

void InitializeGraph(vertex** list){
    for(int i = 0;i < numVertices;i++){
        (*list)[i].Vname = NULL;
        (*list)[i].edgeWeight = 0;
        (*list)[i].next = NULL;
    }
}

void nameGraph(vertex** list,FILE* fp){
    char* name = malloc(sizeof(char)*85);
    for(int i = 0;i < numVertices;i++){
        fscanf(fp,"%s\n",name);
        createVertex(list,name);
    }
    free(name);
}

void makeEdges(vertex** list,FILE* fp){
    char* name1 = malloc(sizeof(char)*85);
    char* name2 = malloc(sizeof(char)*85);
    int weight;
    while(fscanf(fp,"%s %s %d\n",name1,name2,&weight) != EOF){
        createEdge(list,name1,name2,weight);
    }
    free(name1);
    free(name2);
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

int getNumVisited(){
    int count = 0;
    vertex* ptr = visited;
    if(ptr->Vname == NULL){
        return count;
    }
    else{
        while(ptr!= NULL){
            count++;
            ptr = ptr->next;
        }
    }
    return count;
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

int getArrInd(vertex** list,char* name){
    int ind = -1;
    for(int i = 0;i<numVertices;i++){
        if(strcmp((*list)[i].Vname,name) == 0){
            ind = i;
        }
    }
    return ind;
}

void AddToVisit(char* name){
    vertex* newVis = malloc(sizeof(vertex));
    newVis->Vname = malloc(sizeof(char)*85);
    strcpy(newVis->Vname,name);
    newVis -> edgeWeight = 0;
    newVis->next = NULL;

    vertex* ptr = visited;
    if(ptr -> Vname == NULL){
        visited = newVis;
    }
    else{
        newVis->next = visited->next;
        visited->next = newVis;
    }
}

void Djikstra(vertex** list,char* src,int** dist){
    for(int i = 0;i < numVertices;i++){
        if(strcmp((*list)[i].Vname,src) == 0){
            (*dist)[i] = 0;
        }
        else{
            (*dist)[i] = INT_MAX-1;
        }
    }

    while(getNumVisited() < numVertices){
        //this is to get index of shortest, unvisited path
        int min = INT_MAX;
        int index = -1;
        for(int i = 0;i < numVertices;i++){
            if((*dist)[i] < min && isNewNode((*list)[i].Vname) == 0){
                min = (*dist)[i];
                index = i;
            }
        }
        //add this index to visit
        AddToVisit((*list)[index].Vname);
        if((*dist)[index] != INT_MAX-1){
            vertex* ptr = &(*list)[index];
            while(ptr->next != NULL){
                ptr = ptr->next;
                if((*dist)[getArrInd(list,ptr->Vname)] > (*dist)[index] + ptr->edgeWeight){
                    (*dist)[getArrInd(list,ptr->Vname)] = (*dist)[index] + ptr->edgeWeight;
                }
            }
        }
    }
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

void printPaths(vertex** list,int* dist){
    for(int i = 0;i < numVertices;i++){
        int num = dist[i];
        if(num == INT_MAX-1){
            printf("%s %s\n",(*list)[i].Vname,"INF");
        }
        else{
            printf("%s %d\n",(*list)[i].Vname,num);
        }
    }
    printf("\n");
}

void initializePtr(vertex** ptr){
    *ptr = malloc(sizeof(vertex));
    (*ptr)->Vname = NULL;
    (*ptr)->edgeWeight = 0;
    (*ptr)->next = NULL;
}

int compareFunction(const void* Vname1,const void* Vname2){
    return strcmp(*(const char**)Vname1,*(const char**)Vname2);
}

void sortGraph(vertex** list){
    qsort(*list,numVertices,sizeof(vertex),compareFunction);
}

int main(int argc,char* argv[]){
    if(argc > 1){
        FILE* graphFile = fopen(argv[1],"r");
        fscanf(graphFile,"%ld\n",&numVertices);
        vertex* adjList = malloc(sizeof(vertex)*numVertices);

        InitializeGraph(&adjList);
        nameGraph(&adjList,graphFile);
        sortGraph(&adjList);
        makeEdges(&adjList,graphFile);

        if(argc > 2){
            char* srcName = malloc(sizeof(char)*85);
            int* dist = malloc(sizeof(int)*numVertices);
            FILE* queryFile = fopen(argv[2],"r");

            while(fscanf(queryFile,"%s\n",srcName)!= EOF){
                initializePtr(&visited);
                Djikstra(&adjList,srcName,&dist);
                printPaths(&adjList,dist);
                freeVisits();
            }
            free(srcName);
            free(dist);
        }

        deleteAll(&adjList);

    }
    return EXIT_SUCCESS;
}