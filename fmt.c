#include <stdio.h>

int main(int argc, char **argv) {
    const char *user = (argc > 1) ? argv[1] : "hello";
    // 良くない例 : printf("%s", user) を忘れて user をそのままフォーマット文字列に使用している
    printf(user);
    printf("\n");
    // 良い例 : printf("%s", user) 
    printf("%s", user);
    return 0;
}