#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    char *p = malloc(16);
    if (!p) return 1;

    strcpy(p, "hello");
    printf("p=%s\n", p);

    free(p); // 解放

    // 以降は異常 : UAF
    printf("after free: %s\n", p);
    p[0] = 'H'; // 書き込みも異常
    printf("after write: %s\n", p);
    return 0;
}