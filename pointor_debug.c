#include <stdio.h>

#define MAX_STUDENTS 5

typedef struct {
    char name[50];
    int score;
    int grade;
} Student;

void set_grade(Student *s);
Student *find_top(Student *arr, int n);

#define ADDR(label, ptr) \
    printf("  [ADDR] %-30s : %p\n", label, (void *)(ptr))

int main() {
    Student students[MAX_STUDENTS] = {
        {"田中 太郎",  85, ' '},
        {"鈴木 花子",  92, ' '},
        {"佐藤 次郎",  60, ' '},
        {"高橋 美咲",  47, ' '},
        {"伊藤 健一",  78, ' '},
    };
 
    printf("========================================\n");
    printf("【1】main: 配列の各要素のアドレス\n");
    printf("========================================\n");
    for (int i = 0; i < MAX_STUDENTS; i++) {
        printf("  students[%d] %-12s : %p\n",
               i, students[i].name, (void *)&students[i]);
    }
 
    /*
     * ポインタ p に students[0] のアドレスを代入
     * → p が指す場所は students[0] そのもの
     */
    Student *p = students;
 
    printf("\n========================================\n");
    printf("【2】main → set_grade() へ渡す前後の確認\n");
    printf("========================================\n");
 
    for (int i = 0; i < MAX_STUDENTS; i++) {
        printf("\n--- students[%d] : %s ---\n", i, p->name);
        ADDR("main で渡す直前 (p)", p);     /* 渡す前のアドレス */
        set_grade(p);                        /* 関数へ渡す */
        ADDR("main で渡した直後 (p)", p);   /* 渡した後のアドレス → 変わらない */
        p++;
    }
 
    printf("\n========================================\n");
    printf("【3】find_top() が返すポインタの確認\n");
    printf("========================================\n");
 
    Student *top = find_top(students, MAX_STUDENTS);
 
    ADDR("main で受け取った top", top);
    printf("  → 指している学生: %s (%d点)\n", top->name, top->score);
 
    /* top が本当に配列の要素を指しているか確認 */
    printf("\n  [検証] top は students の何番目を指しているか?\n");
    int index = (int)(top - students);   /* ポインタ同士の引き算 = 要素数の差 */
    printf("  top - students = %d  → students[%d] を指している\n",
           index, index);
    ADDR("  students[index] のアドレス", &students[index]);
    ADDR("  top のアドレス            ", top);
 
    return 0;
}
 
 
/* ===== 関数の実装 ===== */
 
void set_grade(Student *s)
{
    /*
     * 引数 s に渡されたアドレスを表示
     * → main の p と同じ値になるはず
     */
    ADDR("  set_grade 内で受け取った s", s);
 
    if      (s->score >= 90) s->grade = 'A';
    else if (s->score >= 75) s->grade = 'B';
    else if (s->score >= 60) s->grade = 'C';
    else                     s->grade = 'F';
}
 
Student *find_top(Student *arr, int n)
{
    Student *top = arr;
 
    ADDR("  find_top 内: arr (先頭)", arr);
 
    for (int i = 1; i < n; i++) {
        if (arr[i].score > top->score) {
            top = &arr[i];
        }
    }
 
    ADDR("  find_top 内: 返す top  ", top);
    return top;
}