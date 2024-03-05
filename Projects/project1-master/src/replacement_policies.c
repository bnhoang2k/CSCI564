//
// This file contains all of the implementations of the replacement_policy
// constructors from the replacement_policies.h file.
//
// It also contains stubs of all of the functions that are added to each
// replacement_policy struct at construction time.
//
// ============================================================================
// NOTE: It is recommended that you read the comments in the
// replacement_policies.h file for further context on what each function is
// for.
// ============================================================================
//

#include "replacement_policies.h"
#include "time.h"

// LRU Replacement Policy
// ============================================================================
// TODO feel free to create additional structs/enums as necessary
// Create a linked list for each set to keep track of the order of the cache lines

struct lru_node {
    uint32_t tag;
    struct lru_node *next;
    struct lru_node *prev;
};

void print_data(struct lru_node **list, uint32_t sets) {
    int i = 0;
    while (i < sets) {
        struct lru_node *current_head = list[i];
        while (current_head->next != NULL) {
            printf("%d ", current_head->tag);
            current_head = current_head->next;
        }
        printf("\n");
        i++;
    }
}

uint32_t lru_eviction_index(struct replacement_policy *replacement_policy,
                            struct cache_system *cache_system, uint32_t set_idx)
{
    // TODO return the index within the set that should be evicted.
    // We want to remove the last element in the linked list

    uint32_t set_start = set_idx * cache_system->associativity;
    uint32_t eviction_index = 0;

    // Pointer to the head of the linked list
    struct lru_node **set_heads = (struct lru_node **)replacement_policy->data;
    struct lru_node *current = set_heads[set_idx], *prev = NULL;
    while (current && current->next != NULL) {
        prev = current;
        current = current->next;
    }

    for (int i = 0; i < cache_system->associativity; i++) {
        if (cache_system->cache_lines[set_start + i].tag == current->tag) {
            eviction_index = i;
            break;
        }
    }

    if (prev) {prev->next = NULL;}
    free(current);

    return eviction_index;
}

void lru_cache_access(struct replacement_policy *replacement_policy, struct cache_system *cache_system, uint32_t set_idx, uint32_t tag) {

    // Pointer to the head of the linked list
    struct lru_node **set_heads = (struct lru_node **)replacement_policy->data;
    struct lru_node *set_head = set_heads[set_idx];

    // First, if the set is empty, we will just add the tag to the set
    if ((set_head->next == NULL) && (set_head->prev)) {
        set_head->tag = tag;
        return;
    }
    // Otherwise, two scenarios occur: the tag is already in the set or it's not
    // Our plan is to move recently used tags to the front of the linked list and delete the last tag
    struct lru_node *current = set_head, *prev = NULL;
    bool tag_found = false;
    while (current->next != NULL) {
        if (current->tag == tag) {
            tag_found = true;
            break;
        }
        prev = current;
        current = current->next;
    }

    // If the tag is found, we will move it to the front of the linked list
    // Three scenarios emerge: the tag is the head, the tag is the tail, or the tag is in the middle
    if (tag_found) {
        if (current == set_head) {return;}
        else if (current->next == NULL) {
            prev->next = NULL;
            current->next = set_head;
            current->prev = NULL;
            set_head->prev = current;
            set_heads[set_idx] = current;
        }
        else {
            prev->next = current->next;
            current->next->prev = prev;
            current->next = set_head;
            current->prev = NULL;
            set_head->prev = current;
            set_heads[set_idx] = current;
        }
    }
    else {
        // If the tag is not found, we will add it to the front of the linked list
        struct lru_node *new_node = malloc(sizeof(struct lru_node));
        new_node->tag = tag;
        new_node->next = set_head;
        new_node->prev = NULL;
        set_head->prev = new_node;
        set_heads[set_idx] = new_node;

    }
}

void lru_replacement_policy_cleanup(struct replacement_policy *replacement_policy)
{
    // TODO cleanup any additional memory that you allocated in the
    // lru_replacement_policy_new function.sudo
    free(replacement_policy->data);
}

struct replacement_policy *lru_replacement_policy_new(uint32_t sets, uint32_t associativity)
{
    struct replacement_policy *lru_rp = calloc(1, sizeof(struct replacement_policy));
    lru_rp->cache_access = &lru_cache_access;
    lru_rp->eviction_index = &lru_eviction_index;
    lru_rp->cleanup = &lru_replacement_policy_cleanup;

    // TODO allocate any additional memory to store metadata here and assign to
    // lru_rp->data.

    // Create an array of structs to store the linked list for each set
    // However, the array has to be a double pointer to be able to store the head of the linked lists.
    struct lru_node **lru_lists = calloc(sets, sizeof(struct lru_node*));
    for (int i = 0; i < sets; i++) {
        struct lru_node *head = malloc(sizeof(struct lru_node));
        head->tag = 0;
        head->next = NULL;
        head->prev = NULL;
        lru_lists[i] = head;
    }
    lru_rp->data = lru_lists;
    return lru_rp;
}

// RAND Replacement Policy
// ============================================================================

uint32_t rand_eviction_index(struct replacement_policy *replacement_policy,
                             struct cache_system *cache_system, uint32_t set_idx)
{
    // TODO return the index within the set that should be evicted.
    return rand() % cache_system->associativity;
}

void rand_cache_access(struct replacement_policy *replacement_policy,
                       struct cache_system *cache_system, uint32_t set_idx, uint32_t tag)
{}

void rand_replacement_policy_cleanup(struct replacement_policy *replacement_policy)
{}

struct replacement_policy *rand_replacement_policy_new(uint32_t sets, uint32_t associativity)
{
    // Seed randomness
    srand(time(NULL));

    struct replacement_policy *rand_rp = malloc(sizeof(struct replacement_policy));
    rand_rp->cache_access = &rand_cache_access;
    rand_rp->eviction_index = &rand_eviction_index;
    rand_rp->cleanup = &rand_replacement_policy_cleanup;

    return rand_rp;
}

// LRU_PREFER_CLEAN Replacement Policy
// ============================================================================
void lru_prefer_clean_cache_access(struct replacement_policy *replacement_policy,
                                   struct cache_system *cache_system, uint32_t set_idx,
                                   uint32_t tag)
{
    // TODO update the LRU_PREFER_CLEAN replacement policy state given a new
    // memory access
    // NOTE: you may be able to share code with the LRU policy
}

uint32_t lru_prefer_clean_eviction_index(struct replacement_policy *replacement_policy,
                                         struct cache_system *cache_system, uint32_t set_idx)
{
    // TODO return the index within the set that should be evicted.

    return 0;
}

void lru_prefer_clean_replacement_policy_cleanup(struct replacement_policy *replacement_policy)
{
    // TODO cleanup any additional memory that you allocated in the
    // lru_prefer_clean_replacement_policy_new function.
}

struct replacement_policy *lru_prefer_clean_replacement_policy_new(uint32_t sets,
                                                                   uint32_t associativity)
{
    struct replacement_policy *lru_prefer_clean_rp = malloc(sizeof(struct replacement_policy));
    lru_prefer_clean_rp->cache_access = &lru_prefer_clean_cache_access;
    lru_prefer_clean_rp->eviction_index = &lru_prefer_clean_eviction_index;
    lru_prefer_clean_rp->cleanup = &lru_prefer_clean_replacement_policy_cleanup;

    // TODO allocate any additional memory to store metadata here and assign to
    // lru_prefer_clean_rp->data.

    return lru_prefer_clean_rp;
}
