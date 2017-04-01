#include "mmu.h"

#include "../data/avl.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

static void *hooks_duplicate(void *data)
{
    mmu_hook_t *result = malloc(sizeof(mmu_hook_t));
    assert(result);
    
    *result = *(mmu_hook_t *)data;
    return result;
}

static void hooks_dispose(void *data)
{
    free((mmu_hook_t *)data); // * the cast is not really needed but meh
}

static int hooks_compare(void *_one, void *_other)
{
    mmu_hook_t *one = (mmu_hook_t *)_one;
    mmu_hook_t *other = (mmu_hook_t *)_other;
    
    // * this avl implementation always calls 'compare' with the value being searched for as the first argument
    // * thus we can be sure that only 'one' can be of size 0 when a hook lookup is done
    if (!one->size && one->base >= other->base && one->base < other->base + other->size)
        return 0; // * the address in question is inside the hook region defined by 'other'
    
    if (one->base < other->base) return -1;
    if (one->base > other->base) return  1;
    return 0;
}

static avl_node_t *hooks_node_alloc()
{
    avl_node_t *result = malloc(sizeof(avl_node_t));
    assert(result);
    
    return result;
}

static void hooks_node_free(avl_node_t *node)
{
    free(node);
}

void mmu_open(mmu_t *mmu)
{
    mmu->code = malloc(sizeof(uint8_t) * mmu->code_size);
    assert(mmu->code);
    
    mmu->data = malloc(sizeof(uint8_t) * mmu->data_size);
    assert(mmu->data);
    
    mmu->hooks = malloc(sizeof(avl_tree_t));
    assert(mmu->hooks);
    
    avl_init_ex(
        mmu->hooks,
        hooks_duplicate,
        hooks_dispose,
        hooks_compare,
        hooks_node_alloc,
        hooks_node_free
    );
}

void mmu_close(mmu_t *mmu)
{
    avl_fini(mmu->hooks);
    
    free(mmu->hooks);
    free(mmu->data);
    free(mmu->code);
}

uint8_t mmu_code_read(mmu_t *mmu, size_t offs)
{
    return mmu->code[offs];
}

uint8_t mmu_data_read(mmu_t *mmu, size_t offs)
{
    // * TODO: find out if the memory operates with bytes or words
    // * (the manual suggests the latter, but idk, so hook receivers
    //   operate with bytes for now)
    
    mmu_hook_t search_for_this;
    search_for_this.base = offs;
    search_for_this.size = 0; // * this makes 'hooks_compare' return 0 if 'offs' is inside a hook region
    mmu_hook_t *hook_region = avl_search(mmu->hooks, &search_for_this);
    if (hook_region)
    {
        return hook_region->hook_receiver->read(hook_region->userdata, offs);
    }
    
    return mmu->data[offs];
}

void mmu_data_write(mmu_t *mmu, size_t offs, uint8_t value)
{
    // * TODO: find out if the memory operates with bytes or words
    // * (the manual suggests the latter, but idk, so hook receivers
    //   operate with bytes for now)
    
    mmu_hook_t search_for_this;
    search_for_this.base = offs;
    search_for_this.size = 0; // * this makes 'hooks_compare' return 0 if 'offs' is inside a hook region
    mmu_hook_t *hook_region = avl_search(mmu->hooks, &search_for_this);
    if (hook_region)
    {
        hook_region->hook_receiver->write(hook_region->userdata, offs, value);
        return;
    }
    
    mmu->data[offs] = value;
}



