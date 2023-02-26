#include "greglist.h"
#include <math.h>
#include <time.h>

CREATE_LIST_TYPE(int)

typedef struct shite {
    int y;
    int z;
    char c;
} shit;

shit hi() {
    return (shit) {0};
}

static inline void square(int *val) {
    *val *= *val;
}

int compare(const int *one, const int *two) {
    return *one < *two ? -1 : (*one > *two ? 1 : 0);
}

int fompare(const int *one, const int *two) {
    return *one < *two ? 1 : (*one > *two ? -1 : 0);
}

int main(void) {
    LIST(int) list;
    init_list(int)(&list);
    int x = 4;
    push_front_r(int)(&list, &x);
    push_front(int)(&list, 10);
    for (int i = 0; i < 10000; ++i)
        push_back(int)(&list, i);
    // print_list(int)(&list, "%d");
    clear_list(int)(&list);
    printf("list size: %zu\n", list_size(int)(&list));
    while (list_size(int)(&list) > 5) {
        printf("first?\n");
        pop_back(int)(&list);
        printf("oncw?\n");
        print_list(int)(&list, "%d");
    }
    push_front(int)(&list, 3);
    push_back(int)(&list, 183);
    print_list(int)(&list, "%d");
    int y = insert_at_index(int)(&list, 2, 4829)->val;
    int z = insert_at_index(int)(&list, 0, 69)->val;
    // printf("val: %d\n", insert_at_index(int)(&list, 2, 448392829)->val);
    puts("fine?");
    print_list(int)(&list, "%d");
    for_each(int)(&list, &square);
    print_list(int)(&list, "%d");
    printf("list size: %zu\n", list_size(int)(&list));
    printf("At index %zu: %d\n", 3l, node_at_index(int)(&list, 3)->val);
    LIST_NODE(int) *node = node_at_index(int)(&list, 1);
    insert_at_node(int)(&list, node, 110);
    clear_list(int)(&list);
    int num_els = 100;
    for (int i = num_els; i >= 2; --i) {
        push_back(int)(&list, i);
    }
    struct timespec start;
    struct timespec end;
    // printf("List before bubble sort repoint:\n");
    // print_list(int)(&list, "%d");
    printf("NUMBER OF ELEMENTS: %d\n", num_els);
    clock_gettime(CLOCK_REALTIME, &start);
    bubble_sort_rp(int)(&list, &compare);
    clock_gettime(CLOCK_REALTIME, &end);
    printf("Sorted. Time taken: %Lf s\n", (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/((long double) 1000000000));
    printf("Print list? [y/n]: ");
    int c = getchar();
    while (getchar() != '\n');
    if (c == 'y') {
        printf("List after bubble sort repoint:\n");
        print_list(int)(&list, "%d");
    }
    // printf("List before bubble sort copy:\n");
    // print_list(int)(&list, "%d");
    clock_gettime(CLOCK_REALTIME, &start);
    bubble_sort_cp(int)(&list, &fompare);
    clock_gettime(CLOCK_REALTIME, &end);
    printf("Sorted. Time taken: %Lf s\n", (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/((long double) 1000000000));
    printf("Print list? [y/n]: ");
    c = getchar();
    while (getchar() != '\n');
    if (c == 'y') {
        printf("List after bubble sort copy:\n");
        print_list(int)(&list, "%d");
    }
    printf("list size: %zu\n", list_size(int)(&list));
    clear_list(int)(&list);
    return 0;
}
