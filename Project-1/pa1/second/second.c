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


void insert(int newData){
    if(head == NULL){
        node* newNode = (node*) malloc(sizeof(node));
        newNode -> data = newData;
        newNode -> next = NULL;
        head = newNode;
    }
    else{
        if (newData == head -> data){
            printList();
            return;
        }
        if (newData < head -> data){
            node* newNode = (node*) malloc(sizeof(node));
            newNode -> data = newData;
            newNode -> next = head;
            head = newNode;
            printList();
            return;
        }
        node* ptr = head -> next;
        node* prev = head;
        while(ptr!=NULL){
            if (ptr -> data == newData){
                printList();
                return;
            }
            if(ptr -> data > newData){
                break;
            }
            ptr = ptr -> next;
            prev = prev -> next;
        }
        node* newNode = (node*) malloc(sizeof(node));
        newNode -> data = newData;
        newNode -> next = ptr;
        prev -> next = newNode;
    }
    printList();
    return;
}


void printList(){
    if(head == NULL){
        printf("EMPTY\n");
    }
    else{
        node *ptr = head;
        while(ptr != NULL){
            printf("%d ",ptr -> data);
            ptr = ptr -> next;
        }
        printf("\n");
    }
}

void delete_node(int toDelete){
    node* ptr = head;
    node* prev = head;

    if(ptr -> data == toDelete){
        head = head -> next;
        free(ptr);
        printList();
        return;
    }
    ptr = ptr -> next;
    while(ptr != NULL){
        if(ptr -> data == toDelete){
            prev -> next = ptr -> next;
            free(ptr);
            printList();
            return;
        }
        ptr = ptr -> next;
        prev = prev -> next;
    }
    printList();
    return;
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
    char* command = malloc(sizeof(char*)*8);
    int num;
    if(fscanf(fp,"%s %d\n",command,&num)!=EOF){
    while(1==1){
       if(strcmp("INSERT",command) == 0){
           insert(num);
       }
       else if(strcmp("DELETE",command) == 0){
           delete_node(num);
       }
       if(fscanf(fp,"%s %d\n",command,&num) == EOF){
           break;
       }
    }
    }

    free(command);
    fclose(fp);
    delete_all();

    return EXIT_SUCCESS;
}
