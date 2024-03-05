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

uint32_t lru_eviction_index(struct replacement_policy *replacement_policy,
                            struct cache_system *cache_system, uint32_t set_idx)
{
    // TODO return the index within the set that should be evicted.
    // We want to remove the last element in the linked list
    struct lru_node *set_head = ((struct lru_node **)replacement_policy->data)[set_idx];
    uint32_t eviction_index = 0;
    while (set_head->next != NULL) {
        set_head = set_head->next;
        eviction_index++;
    }
    return eviction_index;
}

void lru_cache_access(struct replacement_policy *replacement_policy,
                      struct cache_system *cache_system, uint32_t set_idx, uint32_t tag)
{
    // TODO update the LRU replacement policy state given a new memory access
    struct lru_node *set_head = ((struct lru_node **)replacement_policy->data)[set_idx];

    // First, if the set is empty, we'll just add the tag to the set
    if ((set_head->next == NULL) && (set_head->prev == NULL)) {
        set_head->tag = tag;
    }
    else {
        // Two scenarios occur: the tag is already in the set or it's not
        // Our plan is to move recently used tags to the front of the linked list and delete the last tag
        struct lru_node *temp = set_head;
        bool tag_found = false;
        while (temp->next != NULL) {
            if (temp->tag == tag) {
                tag_found = true;
                break;
            }
            temp = temp->next;
        }
        if (tag_found) {
            struct lru_node *current = set_head;
            while (current->next != NULL) {
                if (current->tag == tag) {
                    // In here, we'll have to account for the scenarios where the current node is either a) the head, b) the tail, or c) in the middle
                    // Scenario A: The current node is the head
                    if (current == set_head) {return;}
                    // Scenario B: The current node is the tail
                    else if (current->next == NULL) {
                        current->prev->next = NULL;
                        current->prev = NULL;
                        current->next = set_head;
                        set_head->prev = current;
                        set_head = current;
                        break;
                    }
                    // Scenario C: The current node is in the middle
                    else {
                        current->prev->next = current->next;
                        current->next->prev = current->prev;
                        current->prev = NULL;
                        current->next = set_head;
                        set_head->prev = current;
                        set_head = current;
                        break;
                    } 
                }
                current = current->next;
            }
        }
        else {
            // If the tag is not in the set, we'll add it to the front of the linked list
            struct lru_node *new_node = malloc(sizeof(struct lru_node));
            new_node->tag = tag;
            new_node->next = set_head;
            new_node->prev = NULL;
            set_head->prev = new_node;
            set_head = new_node;
        }
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
