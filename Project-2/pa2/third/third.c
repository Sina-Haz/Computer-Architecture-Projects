#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct vertex{
    char* Vname;
    struct vertex* next;

}vertex;

int isNewNode(char* name);
int enqueue(vertex** head, char* name);
void dequeue();
void freeVisits();

long numVertices;
vertex* visited;

void createVertex(vertex** list,char* name){
    int count = 0; 
    while((*list)[count].Vname != NULL && count < numVertices){
        count++;
    }
    (*list)[count].Vname = malloc(sizeof(char)*85);
    strcpy((*list)[count].Vname,name);
    return;
}

void createEdge(vertex** list,char* head, char* addOn){
    vertex* New = malloc(sizeof(vertex));
    New -> Vname = malloc(sizeof(char)*85);
    strcpy(New->Vname,addOn);
    New -> next = NULL;

    int count = 0;
    while(strcmp(head,(*list)[count].Vname) != 0 && count < numVertices){
        count++;
    }

    vertex* ptr = &(*list)[count];
    if (ptr -> next == NULL || strcmp(New -> Vname,ptr -> next -> Vname) < 0){
        New -> next = ptr -> next;
        ptr -> next = New;
    }
    else{
        while(ptr -> next != NULL && (strcmp(New->Vname,ptr -> next -> Vname) > 0)){
            ptr = ptr -> next;
        }
        New -> next = ptr -> next;
        ptr -> next = New;
    }
}

void BFS(vertex** list,char* startVertex,vertex** queueHead){
    int count = 0;
    while(strcmp(startVertex,(*list)[count].Vname) != 0 && count < numVertices){
        count++;
    }
    vertex* ptr = &(*list)[count];
    enqueue(queueHead,ptr -> Vname);

    while(ptr -> next != NULL){
        ptr = ptr -> next;
        enqueue(queueHead,ptr -> Vname);
    }
    dequeue(queueHead);

    if(*queueHead != NULL){
        BFS(list,(*queueHead)->Vname,queueHead);
    }
}


int enqueue(vertex** head,char* name){
    int num = 1;
    if(isNewNode(name) == 0){
        num = 0;
        vertex* new = malloc(sizeof(vertex));
        new -> Vname = malloc(sizeof(char)*85);
        strcpy(new->Vname,name);
        new -> next = NULL;

        vertex* newVisit = malloc(sizeof(vertex));
        newVisit -> Vname = malloc(sizeof(char)*85);
        strcpy(newVisit->Vname,name);
        newVisit -> next = NULL;

        vertex* ptr = *head;
        if(ptr->Vname == NULL){
            *head = new;
            free(ptr);
        }
        else{
            while(ptr -> next != NULL){
                ptr = ptr -> next;
            }
            ptr -> next = new;
        }

        if(visited -> Vname == NULL){
            vertex* temp = visited;
            visited = newVisit;
            free(temp);
        }
        else{
            newVisit -> next = visited -> next;
            visited -> next = newVisit;
        }
    }
    return num;
}

void dequeue(vertex** head){
    vertex* ptr = *head;
    *head = (*head)->next;
    printf("%s ",ptr->Vname);
    free(ptr->Vname);
    free(ptr);
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
        adjList[i].next = NULL;
    }
    char* newName = malloc(sizeof(char)*85);
    for (int i = 0;i < numVertices;i++){
        fscanf(graphFile,"%s\n",newName);
        createVertex(&adjList,newName);
    }

    char* name1 = malloc(sizeof(char)*85);
    char* name2 = malloc(sizeof(char)*85);
    while(fscanf(graphFile,"%s %s",name1,name2) != EOF){
        createEdge(&adjList,name1,name2);
        createEdge(&adjList,name2,name1);
    }
    if (argc > 2){
        FILE* queryFile = fopen(argv[2],"r");
        char* vertexName = malloc(sizeof(char)*85);

        while(fscanf(queryFile,"%s\n",vertexName) != EOF){
            vertex* queueHead = malloc(sizeof(vertex));
            queueHead -> Vname = NULL;
            queueHead -> next = NULL;

            visited = malloc(sizeof(vertex));
            visited -> Vname = NULL;
            visited -> next = NULL;

            BFS(&adjList,vertexName,&queueHead);
            freeVisits();
            printf("\n");
        }
        free(vertexName);
        fclose(queryFile);
    }
    free(name1);
    free(name2);
    free(newName);
    deleteAll(&adjList);
    fclose(graphFile);

}



    return EXIT_SUCCESS;
}