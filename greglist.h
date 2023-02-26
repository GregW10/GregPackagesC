#ifndef GREGLIST_H
#define GREGLIST_H

#ifdef __cplusplus
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cerrno>
#else
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#endif

#ifndef errno
extern int errno;
#endif

#ifndef __cplusplus
typedef enum boolean {
    false, true
} bool;
#endif

#define LIST_NODE_STRUCT(type) \
typedef struct list_node_##type##_t {   \
    type val;                           \
    struct list_node_##type##_t *prev;  \
    struct list_node_##type##_t *next;  \
} list_node_##type;

#define LIST_STRUCT(type) \
typedef struct list_##type##_t {               \
    size_t num_els; \
    list_node_##type *front; \
    list_node_##type *back; \
} list_##type;

#define INIT_LIST_FUNC(type) \
void init_list_##type(list_##type *list) { \
    if (!list) \
        return;              \
    list->num_els = 0;       \
    list->front = NULL;      \
    list->back = NULL;       \
}
#define PUSH_FRONT_REF_FUNC(type) \
list_node_##type *push_front_ref_##type(list_##type *list, const type *value) { \
    if (!list || !value) \
        return NULL;              \
    if (!list->num_els) {/* all mallocs are cast to allow C++ compatibility */         \
        list->front = (list_node_##type *) malloc(sizeof(list_node_##type));                         \
        if (!list->front)         \
            return NULL;          \
        list->back = list->front;\
        list->front->val = *value;\
        list->num_els = 1;        \
        list->front->prev = NULL;                                        \
        list->front->next = NULL;                                        \
        return list->front;\
    }               \
    list_node_##type *ptr = (list_node_##type *) malloc(sizeof(list_node_##type)); \
    if (!ptr)                     \
        return NULL;\
    ++list->num_els;          \
    ptr->prev = NULL;         \
    ptr->next = list->front;  \
    ptr->val = *value;\
    list->front->prev = ptr;  \
    list->front = ptr;            \
    return ptr;\
}
#define PUSH_FRONT_V_FUNC(type) \
list_node_##type *push_front_v_##type(list_##type *list, type value) { \
    return push_front_ref_##type(list, &value); \
}
#define PUSH_BACK_REF_FUNC(type) \
list_node_##type *push_back_ref_##type(list_##type *list, const type *value) { \
    if (!list || !value) \
        return NULL;              \
    if (!list->num_els) {         \
        list->back = (list_node_##type *) malloc(sizeof(list_node_##type));                         \
        if (!list->back)         \
            return NULL;         \
        list->front = list->back;\
        list->back->val = *value;\
        list->num_els = 1;        \
        list->back->prev = NULL;                                        \
        list->back->next = NULL;                                        \
        return list->back;\
    }               \
    list_node_##type *ptr = (list_node_##type *) malloc(sizeof(list_node_##type)); \
    if (!ptr)                     \
        return NULL;\
    ++list->num_els;          \
    ptr->prev = list->back;         \
    ptr->next = NULL;  \
    ptr->val = *value;\
    list->back->next = ptr;  \
    list->back = ptr;            \
    return ptr;\
}
#define PUSH_BACK_V_FUNC(type) \
list_node_##type *push_back_v_##type(list_##type *list, type value) { \
    return push_back_ref_##type(list, &value); \
}
#define POP_FRONT_FUNC(type) \
type pop_front_list_##type(list_##type *list) { \
    if (!list || !list->num_els)               \
        return (type) {0};   \
    type retval = list->front->val;             \
    list_node_##type *next = list->front->next;\
    free(list->front);       \
    if (!next) {             \
        list->num_els = 0;   \
        list->front = NULL;  \
        list->back = NULL;   \
        return retval;       \
    }                        \
    next->prev = NULL;       \
    list->front = next;      \
    --list->num_els;\
    return retval;           \
}
#define POP_BACK_FUNC(type) \
type pop_back_list_##type(list_##type *list) { \
    if (!list || !list->num_els)               \
        return (type) {0};   \
    type retval = list->back->val;             \
    list_node_##type *prev = list->back->prev;\
    free(list->back);       \
    if (!prev) {             \
        list->num_els = 0;   \
        list->front = NULL;  \
        list->back = NULL;   \
        return retval;       \
    }                        \
    prev->next = NULL;       \
    list->back = prev;      \
    --list->num_els;\
    return retval;           \
}
#define NODE_AT_INDEX_FUNC(type) \
list_node_##type *node_at_index_list_##type(const list_##type *list, size_t index) { \
    if (!list || !list->num_els || index >= list->num_els) {       \
        errno = EINVAL;\
        return NULL;   \
    }                       \
    list_node_##type *it;\
    if (index >= list->num_els/2) {            \
        it = list->back;    \
        index = list->num_els - index - 1;\
        while (index --> 0) \
            it = it->prev;  \
        return it;\
    }\
    it = list->front;        \
    while (index --> 0)     \
        it = it->next;      \
    return it;\
}
#define AT_INDEX_FUNC(type) \
type at_index_list_##type(const list_##type *list, size_t index) { \
    list_node_##type *node;         \
    if (!(node = node_at_index_list_##type(list, index)))          \
        return (type) {0};  \
    return node->val;\
}
#define INSERT_REF_AT_INDEX_FUNC(type)/*inserts BEFORE the index (so that the new element is at the index specified)*/ \
list_node_##type *insert_ref_at_index_list_##type(list_##type *list, size_t index, const type *value) { \
    if (!list || index > list->num_els) {       \
        errno = EINVAL;\
        return NULL;   \
    }                                                                                                                 \
    list_node_##type *new_node = (list_node_##type *) malloc(sizeof(list_node_##type));                                                    \
    if (!new_node)                                                                                                    \
        return NULL;                                                                                                   \
    new_node->val = *value;                                                                                            \
    if (!list->num_els) {                                                                                              \
        list->front = new_node;                                                                                        \
        list->back = new_node;                                                                                         \
        new_node->next = NULL;                                                                                         \
        new_node->prev = NULL;                                                                                         \
        list->num_els = 1;                                                                                               \
        return new_node;\
    }\
    if (index == list->num_els) {                                                                                      \
        list->back->next = new_node;                                                                                   \
        new_node->prev = list->back;                                                                                   \
        new_node->next = NULL;                                                                                         \
        list->back = new_node;                                                                                         \
        ++list->num_els;\
        return new_node;\
    }\
    list_node_##type *it;\
    if (index >= list->num_els/2) {            \
        it = list->back;           \
        index = list->num_els - index - 1;\
        while (index --> 0) \
            it = it->prev;                                                                                            \
        goto end;\
    }\
    it = list->front;        \
    while (index --> 0)     \
        it = it->next;                                                                                                \
    end:\
    new_node->prev = it->prev;                                                                                        \
    if (new_node->prev)\
        new_node->prev->next = new_node;\
    else                                                                                                               \
        list->front = new_node;\
    new_node->next = it;                                                                                          \
    it->prev = new_node;                                                                                              \
    ++list->num_els;\
    return new_node;\
}
#define INSERT_AT_INDEX_FUNC(type) \
list_node_##type *insert_at_index_list_##type(list_##type *list, size_t index, type value) { \
    return insert_ref_at_index_list_##type(list, index, &value);\
}
#define INSERT_REF_AT_NODE_FUNC(type) \
list_node_##type *insert_ref_at_node_list_##type(list_##type *list, list_node_##type *node, const type *value) { \
    /* it is not checked whether the node passed is actually within the list, in order to increase efficiency */ \
    if (!list || !node || !value)                      \
        return NULL;\
    if (node == list->front)          \
        return push_front_ref_##type(list, value);                                                            \
    list_node_##type *new_node = (list_node_##type *) malloc(sizeof(list_node_##type));                       \
    if (!new_node)                    \
        return NULL;                  \
    new_node->val = *value;           \
    node->prev->next = new_node;      \
    new_node->prev = node->prev;      \
    new_node->next = node;            \
    node->prev = new_node;            \
    ++list->num_els;                  \
    return new_node;\
}
#define INSERT_AT_NODE_FUNC(type) \
list_node_##type *insert_at_node_list_##type(list_##type *list, list_node_##type *node, type value) { \
    return insert_ref_at_node_list_##type(list, node, &value);\
}
#define BUBBLE_SORT_CP_FUNC(type) \
void bubble_sort_cp_list_##type(list_##type *list, int (*compare)(const type *a, const type *b)) { \
    /* Bubble sort algorithm where elements are copied. Should always be used for smaller data types. */ \
    if (!list || !list->num_els || list->num_els == 1)                 \
        return;                   \
    list_node_##type *it1;         \
    list_node_##type *it2;         \
    bool sorted = false;          \
    size_t maxim = list->num_els - 1;                                                              \
    size_t counter;               \
    type temp;\
    while (!sorted) {             \
        sorted = true;            \
        it1 = list->front;        \
        it2 = it1->next;          \
        counter = maxim--;\
        while (counter --> 0) {             \
            if (compare(&it1->val, &it2->val) > 0) {                                               \
                temp = it1->val;  \
                it1->val = it2->val;                                                               \
                it2->val = temp;  \
                sorted = false;\
            }                     \
            it1 = it2;            \
            it2 = it2->next;\
        }\
    }\
}
#define BUBBLE_SORT_RP_FUNC(type) \
void bubble_sort_rp_list_##type(list_##type *list, int (*compare)(const type *a, const type *b)) { \
    /* Bubble sort algorithm where pointers are repointed (instead of copying elements) - should be used for when
    * large data types are being stored and copying would be more expensive than redirecting pointers. */ \
    /* The algorithm seems long since an initial pass is done over the list to ensure the largest element ends up at the
    * back, and subsequent passes of the list do not need to check whether this has been dealt with already. */\
    if (!list || !list->num_els || list->num_els == 1)                 \
        return;                   \
    if (list->num_els == 2) {     \
        if (compare(&list->front->val, &list->back->val) > 0) {                                        \
            list->back->prev = NULL;                                                               \
            list->back->next = list->front;                                                        \
            list->front->prev = list->back;                                                        \
            list->front->next = NULL;                                                              \
            list->front = list->back;                                                              \
            list->back = list->back->next;\
        }                         \
        return;\
    }\
    bool sorted = true;       \
    list_node_##type *it1;                                                     \
    list_node_##type *it2;        \
    size_t maxim = list->num_els - 3;     \
    size_t counter;               \
    it1 = list->front;        \
    it2 = it1->next;          \
    if (compare(&it1->val, &it2->val) > 0) {                                                       \
        it1->next = it2->next;    \
        it2->next = it1;          \
        it1->prev = it2;          \
        it2->prev = NULL;         \
        it1->next->prev = it1;\
        list->front = it2;\
        it2 = it1->next;          \
        sorted = false;\
    }                         \
    else {                    \
        it1 = it2;            \
        it2 = it2->next;\
    }\
    while (it2->next) {   \
        if (compare(&it1->val, &it2->val) > 0) {                                               \
            it2->next->prev = it1;                                                             \
            it1->prev->next = it2;                                                             \
            it2->prev = it1->prev;                                                             \
            it1->prev = it2;  \
            it1->next = it2->next;                                                             \
            it2->next = it1;  \
            it2 = it1->next;      \
            sorted = false;\
            continue;\
        }                     \
        it1 = it2;            \
        it2 = it2->next;\
    }                             \
    if (compare(&it1->val, &it2->val) > 0) {                                                       \
        it1->next = NULL;         \
        it2->next = it1;          \
        it2->prev = it1->prev;    \
        it1->prev = it2;          \
        it2->prev->next = it2;    \
        list->back = it1;\
    }\
    while (!sorted) {             \
        sorted = true;            \
        it1 = list->front;        \
        it2 = it1->next;          \
        counter = maxim--;\
        if (compare(&it1->val, &it2->val) > 0) {                                                       \
            it1->next = it2->next;    \
            it2->next = it1;          \
            it1->prev = it2;          \
            it2->prev = NULL;         \
            it1->next->prev = it1;\
            list->front = it2;\
            it2 = it1->next;\
        }                         \
        else {                    \
            it1 = it2;            \
            it2 = it2->next;\
        }\
        while (counter --> 0) {   \
            /*printf("counter: %zu, it2->next: %p\n", counter, it2->next);*/\
            if (compare(&it1->val, &it2->val) > 0) {                                               \
                it2->next->prev = it1;                                                             \
                it1->prev->next = it2;                                                             \
                it2->prev = it1->prev;                                                             \
                it1->prev = it2;  \
                it1->next = it2->next;                                                             \
                it2->next = it1;  \
                sorted = false;   \
                it2 = it1->next;\
                continue;\
            }                     \
            it1 = it2;            \
            it2 = it2->next;\
        }                         \
    }\
}
#define CLEAR_LIST_FUNC(type) \
size_t clear_list_##type(list_##type *list) { \
    if (!list || !list->num_els)                \
        return 0;             \
    list_node_##type *it = list->front->next;       \
    while (it) {              \
        free(it->prev);       \
        it = it->next;\
    }                         \
    free(list->back);         \
    list->front = NULL;       \
    list->back = NULL;        \
    size_t retval = list->num_els;            \
    list->num_els = 0;        \
    return retval;\
}
bool list_empty(const void *list) {
    if (!list)
        return true;
    return !*((size_t *) list);
}
#define FOR_EACH_FUNC(type) \
void for_each_list_##type(list_##type *list, void (*func)(type *value)) { \
    if (!list || !func || !list->num_els)     \
        return;             \
    list_node_##type *it = list->front;                                   \
    while (it) {            \
        func(&it->val);     \
        it = it->next;\
    }\
}
#define PRINT_LIST_FUNC(type) \
size_t print_list_##type(const list_##type *list, const char *format) { \
    putchar('[');             \
    if (!list->num_els) {     \
        puts("]");         \
        return 0;\
    }                         \
    list_node_##type *it = list->front;         \
    while (it) {        \
        printf(format, it->val);                                        \
        fputs(", ", stdout);  \
        it = it->next;\
    }                         \
    puts("\b\b]");        \
    return list->num_els;\
}
#define SIZE_FUNC(type) \
size_t list_size_##type(list_##type *list) { \
    if (!list) \
        return -1;              \
    return list->num_els; \
}
#define init_list(type) init_list_##type
#define push_front_r(type) push_front_ref_##type
#define push_front(type) push_front_v_##type
#define push_back_r(type) push_back_ref_##type
#define push_back(type) push_back_v_##type
#define pop_front(type) pop_front_list_##type
#define pop_back(type) pop_back_list_##type
#define at_index(type) at_index_list_##type
#define node_at_index(type) node_at_index_list_##type
#define insert_at_index(type) insert_at_index_list_##type
#define insert_at_index_r(type) insert_ref_at_index_list_##type
#define insert_at_node(type) insert_at_node_list_##type
#define insert_at_node_r(type) insert_ref_at_node_list_##type
#define clear_list(type) clear_list_##type
#define for_each(type) for_each_list_##type
#define bubble_sort_rp(type) bubble_sort_rp_list_##type
#define bubble_sort_cp(type) bubble_sort_cp_list_##type
#define list_size(type) list_size_##type
#define print_list(type) print_list_##type

#define CREATE_LIST_TYPE(type) \
LIST_NODE_STRUCT(type)          \
LIST_STRUCT(type)                \
INIT_LIST_FUNC(type)              \
PUSH_FRONT_REF_FUNC(type)          \
PUSH_FRONT_V_FUNC(type)             \
PUSH_BACK_REF_FUNC(type)             \
PUSH_BACK_V_FUNC(type)                \
POP_FRONT_FUNC(type)                   \
POP_BACK_FUNC(type)                     \
NODE_AT_INDEX_FUNC(type)                 \
AT_INDEX_FUNC(type)                       \
INSERT_REF_AT_INDEX_FUNC(type)             \
INSERT_AT_INDEX_FUNC(type)                  \
INSERT_REF_AT_NODE_FUNC(type)                \
INSERT_AT_NODE_FUNC(type)                     \
CLEAR_LIST_FUNC(type)                          \
FOR_EACH_FUNC(type)                             \
BUBBLE_SORT_RP_FUNC(type)                        \
BUBBLE_SORT_CP_FUNC(type)                         \
SIZE_FUNC(type)                                    \
PRINT_LIST_FUNC(type)

#define LIST(type) list_##type
#define LIST_NODE(type) list_node_##type

CREATE_LIST_TYPE(int8_t)
CREATE_LIST_TYPE(uint8_t)
CREATE_LIST_TYPE(int16_t)
CREATE_LIST_TYPE(uint16_t)
CREATE_LIST_TYPE(int32_t)
CREATE_LIST_TYPE(uint32_t)
CREATE_LIST_TYPE(float)
CREATE_LIST_TYPE(double)
#ifdef INT64_MAX
CREATE_LIST_TYPE(int64_t)
#endif
#ifdef UINT64_MAX
CREATE_LIST_TYPE(uint64_t)
#endif
#endif
