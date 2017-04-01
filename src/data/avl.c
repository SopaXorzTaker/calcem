#include "avl.h"

#include <stdlib.h>

#define AVL_MAX(a, b) ((a) > (b) ? (a) : (b))
#define AVL_DEPTH(a) ((a) ? (a)->depth : 0)
#define AVL_FIX_DEPTH(a) ((a)->depth = AVL_MAX(AVL_DEPTH((a)->children[0]), AVL_DEPTH((a)->children[1])) + 1)

void avl_init_ex(avl_tree_t *tree, avl_duplicate_func_t duplicate_func, avl_dispose_func_t dispose_func, avl_compare_func_t compare_func, avl_node_alloc_func_t node_alloc_func, avl_node_free_func_t node_free_func)
{
    tree->duplicate = duplicate_func;
    tree->dispose = dispose_func;
    tree->compare = compare_func;
    tree->node_alloc = node_alloc_func;
    tree->node_free = node_free_func;
    
    tree->root = NULL;
    tree->count = 0;
}

void avl_init(avl_tree_t *tree, avl_duplicate_func_t duplicate_func, avl_dispose_func_t dispose_func, avl_compare_func_t compare_func)
{
    avl_init_ex(tree, duplicate_func, dispose_func, compare_func, avl_default_alloc, avl_default_free);
}

avl_node_t *avl_default_alloc()
{
    return malloc(sizeof(avl_node_t));
}

void avl_default_free(avl_node_t *node)
{
    free(node);
}

static void delete_i(avl_tree_t *tree, avl_node_t *node)
{
    if (!node) return;
    
    delete_i(tree, node->children[0]);
    delete_i(tree, node->children[1]);
    
    tree->dispose(node->data);
    tree->node_free(node);
}

void avl_fini(avl_tree_t *tree)
{
    delete_i(tree, tree->root);
}

static avl_node_t *search_i(avl_tree_t *tree, avl_node_t *node, void *data)
{
    if (!node) return NULL;
    
    int cmpres = tree->compare(data, node->data);
    if (!cmpres)
    {
        return node;
    }
    else
    {
        return search_i(tree, node->children[(cmpres < 0) ? 0 : 1], data);
    }
}

void *avl_search(avl_tree_t *tree, void *data)
{
    avl_node_t *node = search_i(tree, tree->root, data);
    return node ? node->data : NULL;
}

static void rotate_i(avl_node_t **node, int dir)
{
    avl_node_t *old_root = *node;
    avl_node_t *old_child0 = old_root->children[dir];
    avl_node_t *old_child0_child1 = old_child0->children[!dir];
    
    *node = old_child0;
    (*node)->parent = old_root->parent;
    
    old_root->children[dir] = old_child0_child1;
    if (old_child0_child1) old_child0_child1->parent = old_root;
    
    old_child0->children[!dir] = old_root;
    old_root->parent = old_child0;
    
    AVL_FIX_DEPTH(old_root);
    AVL_FIX_DEPTH(old_child0);
}

static void rebalance_i(avl_node_t **node)
{
    int depth_diff = AVL_DEPTH((*node)->children[0]) - AVL_DEPTH((*node)->children[1]);
    if (depth_diff > 1 || depth_diff < -1)
    {
        int dir = (depth_diff > 0) ? 0 : 1;
        if (AVL_DEPTH((*node)->children[dir]->children[dir]) < AVL_DEPTH((*node)->children[dir]->children[!dir]))
            rotate_i(&((*node)->children[dir]), !dir);
        rotate_i(node, dir);
    }
    
    AVL_FIX_DEPTH(*node);
}

static int insert_i(avl_tree_t *tree, avl_node_t *parent, avl_node_t **node, void *data)
{
    if (!*node)
    {
        *node = tree->node_alloc();
        if (!*node) return -1;
        
        (*node)->data = data;
        (*node)->parent = parent;
        (*node)->depth = 1;
        (*node)->children[0] = NULL;
        (*node)->children[1] = NULL;
        
        ++(tree->count);
        
        return 1;
    }
    
    int cmpres = tree->compare(data, (*node)->data);
    if (!cmpres)
    {
        (*node)->data = data;
        return 0;
    }
    else
    {
        int depth_change = insert_i(tree, (*node), &((*node)->children[(cmpres < 0) ? 0 : 1]), data);
        if (depth_change < 0) return -1;
        
        if (depth_change) rebalance_i(node);
        
        return depth_change;
    }
}

int avl_insert(avl_tree_t *tree, void *data)
{
    void *data_dup = tree->duplicate(data);
    if (!data_dup) return -2;
    return insert_i(tree, NULL, &(tree->root), data_dup);
    // * returns -2 on duplicate fail
    // * returns -1 on node_alloc fail
    // * returns non-negative on success
}

static avl_node_t *remove_min_i(avl_node_t **node)
{
    if (!(*node)->children[0])
    {
        avl_node_t *result = *node;
        
        *node = (*node)->children[1];
        if (*node) (*node)->parent = result->parent;
        
        return result;
    }
    
    avl_node_t *result = remove_min_i(&((*node)->children[0]));
    
    rebalance_i(node);
    
    return result;
}

static void remove_i(avl_tree_t *tree, avl_node_t **node, void *data)
{
    if (!*node) return;
    
    int cmpres = tree->compare(data, (*node)->data);
    if (!cmpres)
    {
        tree->dispose((*node)->data);
        avl_node_t *node_to_free = *node;
        
        if (node_to_free->children[1])
        {
            *node = remove_min_i(&(node_to_free->children[1]));
            
            (*node)->children[0] = node_to_free->children[0];
            if (node_to_free->children[0]) node_to_free->children[0]->parent = *node;
            
            (*node)->children[1] = node_to_free->children[1];
            if (node_to_free->children[1]) node_to_free->children[1]->parent = *node;
        }
        else
        {
            *node = node_to_free->children[0];
        }
        if (*node) (*node)->parent = node_to_free->parent;
        
        tree->node_free(node_to_free);
        
        --(tree->count);
    }
    else
    {
        remove_i(tree, &((*node)->children[(cmpres < 0) ? 0 : 1]), data);
    }
    
    if (*node) rebalance_i(node);
}

void avl_remove(avl_tree_t *tree, void *data)
{
    remove_i(tree, &(tree->root), data);
}

void avl_iterator_init(avl_iterator_t *iterator, avl_tree_t *tree)
{
    iterator->tree = tree;
}

void avl_iterator_fini(avl_iterator_t *iterator)
{
}

static void *iterator_step(avl_iterator_t *iterator, int dir)
{
    if (iterator->done[dir]) return NULL;
    
    if (iterator->done[!dir])
    {
        iterator->node = iterator->tree->root;
        if (!iterator->node) return NULL;
        while (iterator->node->children[!dir]) iterator->node = iterator->node->children[!dir];
        
        iterator->data = iterator->node->data;
        
        iterator->done[!dir] = 0;
    }
    else if (iterator->node->children[dir])
    {
        iterator->node = iterator->node->children[dir];
        while (iterator->node->children[!dir]) iterator->node = iterator->node->children[!dir];
    }
    else
    {
        avl_node_t *old_node;
        do
        {
            old_node = iterator->node;
            
            iterator->node = iterator->node->parent;
            if (!iterator->node)
            {
                iterator->done[dir] = 1;
                break;
            }
        } while (old_node != iterator->node->children[!dir]);
    }
    
    iterator->data = iterator->node ? iterator->node->data : NULL;
    return iterator->data;
}

void avl_find(avl_iterator_t *iterator, void *data)
{
    iterator->done[0] = 0;
    iterator->done[1] = 1;
    
    avl_node_t *node = search_i(iterator->tree, iterator->tree->root, data);
    if (node)
    {
        iterator->done[0] = 0;
        iterator->done[1] = 0;
        iterator->node = node;
        iterator->data = node->data;
    }
}

void avl_find_first(avl_iterator_t *iterator)
{
    iterator->done[0] = 1;
    iterator->done[1] = 0;
    iterator_step(iterator, 1);
}

void avl_find_last(avl_iterator_t *iterator)
{
    iterator->done[0] = 0;
    iterator->done[1] = 1;
    iterator_step(iterator, 0);
}

void avl_find_before_first(avl_iterator_t *iterator)
{
    iterator->done[0] = 1;
    iterator->done[1] = 0;
}

void avl_find_after_last(avl_iterator_t *iterator)
{
    iterator->done[0] = 0;
    iterator->done[1] = 1;
}

void *avl_iterator_prev(avl_iterator_t *iterator)
{
    return iterator_step(iterator, 0);
}

void *avl_iterator_next(avl_iterator_t *iterator)
{
    return iterator_step(iterator, 1);
}

int avl_iterator_before_first(avl_iterator_t *iterator)
{
    return iterator->done[0];
}

int avl_iterator_after_last(avl_iterator_t *iterator)
{
    return iterator->done[1];
}


