#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    int a[4] = {10, 20, 30, 40};

    int idx = (argc >= 2) ? atoi(argv[1]) : 2;

    // 正常なら 0..3 の範囲でアクセスするべき
    printf("idx=%d\n", idx);
    printf("a[%d]=%d\n", idx, a[idx]); // 範囲外なら OOB read (未定義動作)
    
    a[idx] = 999; // 範囲外なら OOB write (未定義動作)
    printf("wrote a[%d]=999\n", idx);
    return 0;
}