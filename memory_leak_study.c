/*
 * memory_leak_study.c
 * メモリリークを学ぶためのサンプルコード
 *
 * コンパイル:
 *   gcc -g -o memory_leak_study memory_leak_study.c
 *
 * Valgrindで検出:
 *   valgrind --leak-check=full ./memory_leak_study
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============================================================
 * 例1: 基本的なリーク - free()を呼び忘れる
 * ============================================================ */
void example1_leak(void) {
    printf("\n=== 例1: free()呼び忘れ (リークあり) ===\n");

    int *p = malloc(sizeof(int) * 10);
    if (p == NULL) return;

    for (int i = 0; i < 10; i++) {
        p[i] = i * 2;
    }
    printf("p[3] = %d\n", p[3]);

    /* NG: free(p) を呼ばずに関数が終わる → リーク */
}

void example1_fixed(void) {
    printf("\n=== 例1: free()呼び忘れ (修正版) ===\n");

    int *p = malloc(sizeof(int) * 10);
    if (p == NULL) return;

    for (int i = 0; i < 10; i++) {
        p[i] = i * 2;
    }
    printf("p[3] = %d\n", p[3]);

    free(p);  /* OK: 必ず解放する */
    p = NULL; /* 良習慣: ダングリングポインタ対策 */
}

/* ============================================================
 * 例2: 早期リターンによるリーク
 * ============================================================ */
int example2_leak(int x) {
    printf("\n=== 例2: 早期リターンによるリーク ===\n");

    char *buf = malloc(256);
    if (buf == NULL) return -1;

    if (x < 0) {
        printf("x が負です\n");
        return -1; /* NG: buf が解放されずにリターン */
    }

    snprintf(buf, 256, "x = %d", x);
    printf("%s\n", buf);
    free(buf);
    return 0;
}

int example2_fixed(int x) {
    printf("\n=== 例2: 早期リターンによるリーク (修正版) ===\n");

    char *buf = malloc(256);
    if (buf == NULL) return -1;

    if (x < 0) {
        printf("x が負です\n");
        free(buf); /* OK: リターン前に解放 */
        return -1;
    }

    snprintf(buf, 256, "x = %d", x);
    printf("%s\n", buf);
    free(buf);
    return 0;
}

/* ============================================================
 * 例3: ポインタの上書きによるリーク
 * ============================================================ */
void example3_leak(void) {
    printf("\n=== 例3: ポインタ上書きによるリーク ===\n");

    int *p = malloc(sizeof(int) * 5);
    if (p == NULL) return;

    p[0] = 100;
    printf("p[0] = %d\n", p[0]);

    /* NG: 古い p を free せずに新しいメモリを割り当て */
    p = malloc(sizeof(int) * 10); /* 最初の5個分がリーク! */
    if (p == NULL) return;

    p[0] = 200;
    printf("p[0] = %d\n", p[0]);

    free(p);
}

void example3_fixed(void) {
    printf("\n=== 例3: ポインタ上書きによるリーク (修正版) ===\n");

    int *p = malloc(sizeof(int) * 5);
    if (p == NULL) return;

    p[0] = 100;
    printf("p[0] = %d\n", p[0]);

    free(p); /* OK: 上書き前に解放 */
    p = malloc(sizeof(int) * 10);
    if (p == NULL) return;

    p[0] = 200;
    printf("p[0] = %d\n", p[0]);

    free(p);
}

/* ============================================================
 * 例4: 連結リストのリーク
 * ============================================================ */
typedef struct Node {
    int value;
    struct Node *next;
} Node;

Node *create_list(int n) {
    Node *head = NULL;
    Node *tail = NULL;
    for (int i = 0; i < n; i++) {
        Node *node = malloc(sizeof(Node));
        if (node == NULL) return head;
        node->value = i;
        node->next = NULL;
        if (head == NULL) {
            head = tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }
    return head;
}

void example4_leak(void) {
    printf("\n=== 例4: 連結リストのリーク ===\n");

    Node *head = create_list(5);

    /* NG: head だけを解放 → 残りのノードがリーク */
    printf("先頭の値: %d\n", head->value);
    free(head); /* 後続ノードが全部リーク! */
}

void free_list(Node *head) {
    while (head != NULL) {
        Node *next = head->next; /* 次を先に保持 */
        free(head);
        head = next;
    }
}

void example4_fixed(void) {
    printf("\n=== 例4: 連結リストのリーク (修正版) ===\n");

    Node *head = create_list(5);

    printf("先頭の値: %d\n", head->value);
    free_list(head); /* OK: すべてのノードを解放 */
}

/* ============================================================
 * 例5: realloc失敗によるリーク
 * ============================================================ */
void example5_leak(void) {
    printf("\n=== 例5: realloc失敗によるリーク ===\n");

    int *p = malloc(sizeof(int) * 10);
    if (p == NULL) return;

    /* NG: realloc失敗時に p が NULL になるが、
           元のメモリへのポインタが失われてリーク */
    p = realloc(p, sizeof(int) * 1000000000UL);
    if (p == NULL) {
        printf("realloc 失敗 (元メモリがリーク)\n");
        return;
    }

    free(p);
}

void example5_fixed(void) {
    printf("\n=== 例5: realloc失敗によるリーク (修正版) ===\n");

    int *p = malloc(sizeof(int) * 10);
    if (p == NULL) return;

    /* OK: 一時変数で受け取り、失敗時は元ポインタを保持 */
    int *tmp = realloc(p, sizeof(int) * 1000000000UL);
    if (tmp == NULL) {
        printf("realloc 失敗 (元メモリは保持)\n");
        free(p); /* 元のメモリを解放できる */
        return;
    }
    p = tmp;
    free(p);
}

/* ============================================================
 * main
 * ============================================================ */
int main(void) {
    printf("===== メモリリーク学習サンプル =====\n");
    printf("リークあり版と修正版を交互に実行します\n");

    example1_leak();
    example1_fixed();

    example2_leak(-1);   /* リーク発生 */
    example2_fixed(-1);  /* 修正版 */

    example3_leak();
    example3_fixed();

    example4_leak();
    example4_fixed();

    example5_leak();
    example5_fixed();

    printf("\n===== 完了 =====\n");
    printf("Valgrind で実行すると各リークが検出されます:\n");
    printf("  valgrind --leak-check=full ./memory_leak_study\n");
    return 0;
}
