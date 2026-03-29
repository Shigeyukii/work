#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct human {
    char name[50];
    int age;
} human;

int main() {
    printf("Hello, World!\n");

    human *h1 = (human *)malloc(sizeof(human));
    if (h1 == NULL) {
        fprintf(stderr, "malloc failed\n");
        return 1;
    }
    strcpy(h1->name, "Alice");
    h1->age = 30;

    printf("Name: %s, Age: %d\n", h1->name, h1->age);

    free(h1);

    return 0;
}