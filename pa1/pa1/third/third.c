#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

typedef struct node{
    int data;
    struct node* next;
} node;

node* head;
void printList();

void enqueue(int newData){
    node* newNode = (node*) malloc(sizeof(node));
    newNode -> data = newData;
    newNode -> next = NULL;
    if(head == NULL){
        head = newNode;
    }
    else{
        node* ptr = head;
        while(ptr -> next != NULL){
            ptr = ptr -> next;
        }
        ptr -> next = newNode;
    }
    printList();
    return;
}

void push(int newData){
    node* newNode = (node*) malloc(sizeof(node));
    newNode -> data = newData;
    newNode -> next = head;
    head = newNode;
    printList();
    return;
}

void pop(){
    node* temp = head;
    head = head -> next;
    free(temp);
    printList();
    return;
}

void printList(){
    node* ptr = head;
    if(ptr == NULL){
        printf("EMPTY\n");
    }
    else{
        while(ptr != NULL){
            printf("%d ",ptr -> data);
            ptr = ptr -> next;
        }
        printf("\n");
    }
}

void delete_all(){
    struct node* temp;
    while(head != NULL){
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main(int argc,char* argv[]){
    head = NULL;
    if (access(argv[1],F_OK) != 0){
        printf("error\n");
        return EXIT_SUCCESS;
    }

    FILE* fp = fopen(argv[1],"r");
    char* command = (char*) malloc(sizeof(char*)*8);
    int num;

    if(fscanf(fp,"%s ",command) != EOF){
        while(1==1){
            if(strcmp("ENQUEUE",command) == 0){
                fscanf(fp,"%d\n",&num);
                enqueue(num);
            }
            else if(strcmp("PUSH",command) == 0){
                fscanf(fp,"%d\n",&num);
                push(num);
            }
            else if(strcmp("POP",command) == 0){
                fscanf(fp,"\n");
                pop();
            }
            if(fscanf(fp,"%s ",command) == EOF){
                break;
            }
        }
    }

    fclose(fp);
    delete_all();

    return EXIT_SUCCESS;
}