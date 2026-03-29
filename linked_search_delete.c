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
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    strcpy(node->name, name);
    node->score = score;
    node->next = NULL;
    return node;
}

void append(Node **head, const char *name, int score) {
    Node *new_node = create_node(name, score);
    if (*head == NULL) {
        *head = new_node;
        return;
    }
    Node *current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_node;
}

void print_list(const Node *head) {
    if (head == NULL) {
        printf("The list is empty.\n");
        return;
    }
    const Node *current = head;
    int i = 0;
    while (current != NULL) {
        printf("%d. Name: %s, Score: %d (%p -> next: %p)\n", i, current->name, current->score, (void *)current, (void *)current->next);
        current = current->next;
        i++;
    }
}

void free_list(Node **head)
{
    Node *current = *head;
    while (current != NULL) {
        Node *next = current->next;
        free(current);
        current = next;
    }
    *head = NULL;
}

Node *search_by_name(Node *head, const char *name)
{
    Node *current = head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL; // Not found
}

int delete_by_name(Node **head, const char *name)
{
    if (*head == NULL) {
        return 0; // List is empty
    }

    Node *current = *head;
    Node *previous = NULL;

    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            if (previous == NULL) {
                printf("Deleting head node: %s\n", current->name);
                // Deleting the head node
                *head = current->next;
            } else {
                printf("Deleting node: %s (prev: %s)\n", current->name, previous->name);
                previous->next = current->next;
            }
            free(current);
            return 1; // Deletion successful
        }
        previous = current;
        current = current->next;
    }
    printf("Node not found: %s\n", name);
    return 0; // Node not found
}

int main(void)
{
    Node *head = NULL;

    append(&head, "Tanaka Trao", 85);
    append(&head, "Suzuki Hana", 92);
    append(&head, "Sato Jiro", 60);
    append(&head, "Takahashi Misa", 47);
    append(&head, "Ito Kenichi", 78);

    printf("Initial list:\n");
    print_list(head);

    printf("\nSearching for 'Sato Jiro':\n");
    const char *search_name = "Sato Jiro";
    Node *found = search_by_name(head, search_name);

    if (found != NULL) {
        printf("Found: Name: %s, Score: %d\n", found->name, found->score);
    } else {
        printf("Not found: %s\n", search_name);
    }

    found = search_by_name(head, "Nonexistent Name");
    if (found != NULL) {
        printf("Found: Name: %s, Score: %d\n", found->name, found->score);
    } else {
        printf("Not found: Nonexistent Name\n");
    }


    printf("\nDeleting 'Sato Jiro':\n");

    printf("List before deletion:\n");
    print_list(head);

    delete_by_name(&head, "Sato Jiro");

    printf("List after deletion:\n");
    print_list(head);


    printf("\nDeleting 'Tanaka Trao' (head node):\n");
    printf("List before deletion:\n");
    print_list(head);

    delete_by_name(&head, "Tanaka Trao");

    printf("List after deletion:\n");
    print_list(head);


    printf("\nDeleting 'Ito Kenichi' (tail node):\n");
    printf("List before deletion:\n");
    print_list(head);

    delete_by_name(&head, "Ito Kenichi");

    printf("List after deletion:\n");
    print_list(head);


    printf("\n freeing the list:\n");
    free_list(&head);
    printf(" head = %p (NULL)\n", (void *)head);

    return 0;
}