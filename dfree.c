#include <stdio.h>
#include <stdlib.h>

int main() {
    int *p = malloc(sizeof(int) * 4);
    if (!p) return 1;

    p[0] =123;
    printf("p[0]=%d\n", p[0]);

    free(p);
    free(p); // double free, 避けるべきよ
    return 0;
}