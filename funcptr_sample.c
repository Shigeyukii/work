#include <stdio.h>
#include <string.h>

#define MAX_STUDENTS 5

typedef struct {
    char name[50];
    int score;
    char grade;
} Student;

int cmp_score_asc(const Student *a, const Student *b)
{
    return a->score - b->score;
}

int cmp_score_desc(const Student *a, const Student *b)
{
    return b->score - a->score;
}

int cmp_name_asc(const Student *a, const Student *b)
{
    return strcmp(a->name, b->name);
}

void bubble_sort(Student *arr, int n, int (*cmp)(const Student *, const Student *))
{
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (cmp(&arr[j], &arr[j + 1]) > 0) {
                Student tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

void report_simple(const Student *arr, int n)
{
    printf("[Simple]\n");
    for (int i = 0; i < n; i++)
        printf("   %d: %-14s %dten\n", i +1, arr[i].name, arr[i].score);
}

void report_grade(const Student *arr, int n)
{
    printf("[Grade]\n");
    for (int i = 0; i < n; i++) {
        char g;
        if (arr[i].score >= 90)
            g = 'A';
        else if (arr[i].score >= 75)
            g = 'B';
        else if (arr[i].score >= 60)
            g = 'C';
        else
            g = 'F';
        printf("   %d: %-14s %dten %c\n", i +1, arr[i].name, arr[i].score, g);
    }
}

void report_bar(const Student *arr, int n)
{
    printf(" 棒グラフ\n");
    for (int i = 0; i < n; i++) {
        printf(" %-14s |", arr[i].name);
        int bars = arr[i].score / 10;
        for (int b = 0; b < bars; b++) printf("■");
        printf(" %d\n", arr[i].score);
    }
}

int main(void)
{
    Student students[MAX_STUDENTS] = {
        {"Tanaka Taro", 85, ' '},
        {"Suzuki Hana", 92, ' '},
        {"Sato Jiro", 60, ' '},
        {"Takahashi Misa", 47, ' '},
        {"Ito Kenichi", 78, ' '}
    };

    printf("1.関数ポインタ変数への代入と呼び出し\n");

    int (*cmp)(const Student *, const Student *) = cmp_score_desc;

    printf(" 使用中の比較関数アドレス: %p\n", (void *)cmp);
    printf(" cmpscoredescのアドレス: %p\n", (void *)cmp_score_desc);

    bubble_sort(students, MAX_STUDENTS, cmp);
    report_simple(students, MAX_STUDENTS);


    printf("\n2.コールバック: ソート順を切り替える\n");

    printf("\n ▼ 点数 昇順:\n");
    bubble_sort(students, MAX_STUDENTS, cmp_score_asc);
    report_simple(students, MAX_STUDENTS);

    printf("\n ▼ 名前 昇順:\n");
    bubble_sort(students, MAX_STUDENTS, cmp_name_asc);
    report_simple(students, MAX_STUDENTS);


    printf("\n3.関数ポインタ配列でレポートを切り替える\n");

    void (*report_funcs[])(const Student *, int) = {
        report_simple,
        report_grade,
        report_bar,
    };
    const char *report_names[] = {"Simple", "Grade", "Bar"};
    int num_reports = sizeof(report_funcs) / sizeof(report_funcs[0]);

    bubble_sort(students, MAX_STUDENTS, cmp_score_desc);

    for (int i = 0; i < num_reports; i++) {
        printf("\n ▼ レポート種別[%d]: %s\n", i, report_names[i]);
        report_funcs[i](students, MAX_STUDENTS);
    }

    return 0;
}