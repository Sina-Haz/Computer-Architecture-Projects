#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct vertex{
    char* Vname;
    int edgeWeight;
    struct vertex* next;

}vertex;

long numVertices;

void createVertex(vertex** list,char* name){
    int count = 0;
    while((*list)[count].Vname != NULL && count < numVertices){
        count++;
    }
    (*list)[count].Vname = malloc(sizeof(char)*85);
    strcpy((*list)[count].Vname,name);
}

void createEdge(vertex** list,char* head, char* addOn,int weight){
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
        ptr -> edgeWeight = weight;
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

void printOutDegree(vertex** list,char* nameofVertex){
    int count = 0;
    while(strcmp((*list)[count].Vname,nameofVertex) != 0 && count < numVertices){
        count++;
    }
    vertex* ptr = &(*list)[count];
    int outDegree = 0;
    while(ptr->next != NULL){
        ptr = ptr->next;
        outDegree++;
    }
    printf("%d\n",outDegree);
}

void printAdjacency(vertex** list,char* nameofVertex){
    int count = 0;
    while(strcmp((*list)[count].Vname,nameofVertex) != 0 && count < numVertices){
        count++;
    }
    vertex* ptr = &(*list)[count];
    while(ptr->next != NULL){
        ptr = ptr->next;
        printf("%s ",ptr->Vname);
    }
    printf("\n");
}

void printInDegree(vertex** list,char* nameofVertex){
    int inDegree = 0;
    for(int i = 0;i < numVertices;i++){
        vertex* ptr = &(*list)[i];
        if (strcmp(ptr -> Vname,nameofVertex) != 0){
            while(ptr->next != NULL){
                ptr = ptr->next;
                if(strcmp(ptr -> Vname,nameofVertex) == 0){
                    inDegree++;
                }
            }
        }
    }
    printf("%d\n",inDegree);
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

        char* name1 = malloc(sizeof(char)*85);
        char* name2 = malloc(sizeof(char)*85);
        int weight;
        while(fscanf(graphFile,"%s %s %d\n",name1,name2,&weight) != EOF){
            createEdge(&adjList,name1,name2,weight);
        }

        if(argc > 2){
            FILE* queryFile = fopen(argv[2],"r");
            char* query = malloc(sizeof(char)+1);
            char* vertexName = malloc(sizeof(char)*85);
            while(fscanf(queryFile,"%s %s\n",query,vertexName) != EOF){
                if(strcmp(query,"o") == 0){
                    printOutDegree(&adjList,vertexName);
                }
                if(strcmp(query,"a") == 0){
                    printAdjacency(&adjList,vertexName);
                }
                if(strcmp(query,"i") == 0){
                    printInDegree(&adjList,vertexName);
                }
            }
            free(query);
            free(vertexName);
            fclose(queryFile);
        }

        fclose(graphFile);
        free(newName);
        free(name1);
        free(name2);
        deleteAll(&adjList);
    }

    return EXIT_SUCCESS;
}