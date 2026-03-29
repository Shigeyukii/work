#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char name[50];
    int score;
    struct Node *next;
} Node;

Node *create_node(const char *name, int score) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL) {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }
    strcpy(node->name, name);
    node->score = score;
    node->next = NULL;

    printf(" [create] %-18s %3d address: %p\n", node->name, node->score, (void *)node);
    return node;
}

void append(Node **head, const char *name, int score)
{
    Node *new_node = create_node(name, score);

    if (*head == NULL) {
        *head = new_node;
        return;
    }

    Node *cur = *head;
    while (cur->next != NULL) {
        cur = cur->next;
    }
    cur->next = new_node;
}

void prepend(Node **head, const char *name, int score)
{
    Node *new_node = create_node(name, score);
    new_node->next = *head;
    *head = new_node;
}

void print_list(const Node *head)
{
    const Node *cur = head;
    int index = 0;
    while (cur != NULL) {
        printf(" [%d] %-18s %3d (address: %p next: %p)\n", index, cur->name, cur->score, (void *)cur, (void *)cur->next);
        cur = cur->next;
        index++;
    }
}

void free_list(Node **head)
{
    Node *cur = *head; 
    while (cur != NULL) {
        Node *next = cur->next;
        printf(" [free] %-18s address: %p\n", cur->name, (void *)cur);
        free(cur);
        cur = next;
    }
    *head = NULL;
}

int main(void)
{
    Node *head = NULL;

    printf("Appending nodes:\n");

    append(&head, "Tanaka Taro", 85);
    append(&head, "Suzuki Hana", 92);
    append(&head, "Sato Jiro", 60);

    printf("\n Current list:\n");
    print_list(head);


    printf("\nPrepending Nodes:\n");

    prepend(&head, "Ito Kenichi", 78);

    printf("\n Current list:\n");
    print_list(head);


    printf("\nChecking memory addresses:\n");

    printf("\n head -> %p\n", (void *)head);
    Node *cur = head;
    while (cur != NULL) {
        printf(" %p (%-16s) -> next: %p\n", (void *)cur, cur->name, (void *)cur->next);
        cur = cur->next;
    }
    printf("\n------------------\n");


    printf("\nFreeing list:\n");
    free_list(&head);
    printf("\n head = %p (should be NULL)\n", (void *)head);

    return 0;
}