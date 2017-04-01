#ifndef DATA__AVL_H
#define DATA__AVL_H

typedef struct _avl_node_s
{
    struct _avl_node_s *parent, *children[2];
    void *data;
    int depth;
} avl_node_t;

typedef void *(*avl_duplicate_func_t)(void *);
typedef void (*avl_dispose_func_t)(void *);
typedef int (*avl_compare_func_t)(void *, void *);
typedef avl_node_t *(*avl_node_alloc_func_t)();
typedef void (*avl_node_free_func_t)(avl_node_t *);

typedef struct _avl_tree_s
{
    avl_node_t *root;
    avl_duplicate_func_t duplicate;
    avl_dispose_func_t dispose;
    avl_compare_func_t compare;
    avl_node_alloc_func_t node_alloc;
    avl_node_free_func_t node_free;
    long long int count;
} avl_tree_t;

void avl_init_ex(avl_tree_t *tree, avl_duplicate_func_t duplicate_func, avl_dispose_func_t dispose_func, avl_compare_func_t compare_func, avl_node_alloc_func_t node_alloc_func, avl_node_free_func_t node_free_func);
void avl_init(avl_tree_t *tree, avl_duplicate_func_t duplicate_func, avl_dispose_func_t dispose_func, avl_compare_func_t compare_func);
void avl_fini(avl_tree_t *tree);
avl_node_t *avl_default_alloc();
void avl_default_free(avl_node_t *node);
void *avl_search(avl_tree_t *tree, void *data);
int avl_insert(avl_tree_t *tree, void *data);
void avl_remove(avl_tree_t *tree, void *data);

typedef struct _avl_iterator_s
{
    avl_tree_t *tree;
    avl_node_t *node;
    void *data;
    int done[2];
} avl_iterator_t;

void avl_iterator_init(avl_iterator_t *iterator, avl_tree_t *tree);
void avl_iterator_fini(avl_iterator_t *iterator);
void avl_find(avl_iterator_t *iterator, void *data);
void avl_find_first(avl_iterator_t *iterator);
void avl_find_last(avl_iterator_t *iterator);
void avl_find_before_first(avl_iterator_t *iterator);
void avl_find_after_last(avl_iterator_t *iterator);
void *avl_iterator_prev(avl_iterator_t *iterator);
void *avl_iterator_next(avl_iterator_t *iterator);
#define avl_iterator_current(iter) ((iter)->data)
int avl_iterator_before_first(avl_iterator_t *iterator);
int avl_iterator_after_last(avl_iterator_t *iterator);

#endif // DATA__AVL_H

