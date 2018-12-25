#include<stdio.h>
#include<stdlib.h>

#include "linklist.h"

//can use hash table as well
//using floyd's cycle finding algo
void detect_loop(Node *head)
{
    Node *slow=head, *fast=head;
    while (slow!=NULL && fast!=NULL && fast->next!=NULL) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast)
            return 1;
    }      
}
void midpoint(Node **head)
{
    Node *slow=*head, *fast=*head;
    while(fast!=NULL && fast->next!=NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }
    printf("Mid point is : %d\n",slow->data);
}
void reverse_linklist(Node **head)
{
    Node *next, *prev=NULL,*temp=*head;
    while(temp!=NULL) {
          next = temp->next;
          temp->next = prev;
          prev = temp;
          temp = next;
    }
    *head =  prev;
}

void insert_node_pos(Node **head, int data, int pos)
{
    Node *node, *temp;
    int index=1;
  
    temp = *head; 
    node = (Node*)malloc(sizeof(Node));
    node->data = data;
    if (index == pos) {
        node->next = temp;
    } else {
	 while (index<pos-1 && temp->next!=NULL) {
	    index++;
	    temp = temp->next;
	 }
         if (index < pos -1) {
             printf("Not valid position\n");
             free(node);
             return;
         }
	 node->next = temp->next;
	 temp->next = node;
    }
}

void insert_node(Node **head, int data) 
{
    Node *node, *temp;

    node = (Node*)malloc(sizeof(Node)); 
    node->data = data;
    node->next = NULL;

    if(*head == NULL) {
        *head = node;
    } else {
        temp = *head;
        while(temp->next!=NULL) {
            temp = temp->next;
        }
        temp->next = node; 
    }
}


void delete_node(Node *head, int pos)
{

}

void display(Node *head)
{
   Node *temp=head;
   while(temp!=NULL) {
       printf("%d \t",temp->data);
       temp = temp->next;
   }
   printf("\n");
}

void delete_list(Node *head)
{
    Node *node,*temp=head;
    while(temp!=NULL) {
        node = temp;
        temp = temp->next;
        free(node);
    }
}

int main()
{
    Node *head=NULL;
    insert_node(&head,6);
    insert_node(&head,7);
    insert_node(&head,8);
    insert_node(&head,10);
    midpoint(&head);
    display(head);
    insert_node(&head,1);
    insert_node_pos(&head,3,2);
    insert_node_pos(&head,11,6);
    insert_node_pos(&head,100,100);
    midpoint(&head);
    display(head);
    reverse_linklist(&head);
    display(head);

    delete_list(head);
    return 0;
}
