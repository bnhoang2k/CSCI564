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
#include "memory_system.h"
#include "time.h"

// LRU Replacement Policy
// ============================================================================
// TODO feel free to create additional structs/enums as necessary
// Create a linked list for each set to keep track of the order of the cache lines

struct lru_data {
    struct lru_node **head_set;
    uint32_t num_sets;
};

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
    struct lru_data *data = (struct lru_data *)replacement_policy->data;
    struct lru_node *head = data->head_set[set_idx], *eviction_node = NULL;
    
    for (eviction_node = head; 
         eviction_node->next != NULL; 
         eviction_node = eviction_node->next);
    
    uint32_t set_start = set_idx * cache_system->associativity;
    uint32_t eviction_index = -1;

    for (int i = 0; i < cache_system->associativity; i++) {
        if (cache_system->cache_lines[set_start + i].tag == eviction_node->tag) {
            eviction_index = i;
            break;
        }
    }

    // Free the memory of the eviction node
    if (eviction_node->prev) {eviction_node->prev->next = NULL;}
    free(eviction_node);

    return eviction_index;

}

void lru_cache_access(struct replacement_policy *replacement_policy, struct cache_system *cache_system, uint32_t set_idx, uint32_t tag) {
    // TODO implement the LRU cache access pattern
    struct lru_data *data = (struct lru_data *)replacement_policy->data;
    struct lru_node *head = data->head_set[set_idx];

    // Code for when the set is empty
    if (head->tag == -1) {
        head->tag = tag;
        head->next = NULL;
        head->prev = NULL;
        return;
    }

    struct lru_node *current_node = head;
    bool tag_found = false;
    while (current_node != NULL) {
        if (current_node->tag == tag) {
            tag_found = true;
            break;
        }
        current_node = current_node->next;
    }
    // Two scenarios emerge: the tag is found or the tag is not found
    if (tag_found) {
        // If the tag is found, the cache can be in one of three positions within the linked list:
        // 1. The head of the linked list
        // 2. The tail of the linked list
        // 3. Somewhere in the middle of the linked list
        if (current_node == head) {return;}
        else if (current_node->next == NULL) {
            current_node->prev->next = NULL;
            current_node->prev = NULL;
            current_node->next = head;
            head->prev = current_node;
            data->head_set[set_idx] = current_node;
        }
        else {
            current_node->prev->next = current_node->next;
            current_node->next->prev = current_node->prev;
            current_node->prev = NULL;
            current_node->next = head;
            head->prev = current_node;
            data->head_set[set_idx] = current_node;
        }
    }
    else {
        // If the tag is not found, we'll have to add it to the head of the linked list
        struct lru_node *new_node = calloc(1, sizeof(struct lru_node));
        new_node->tag = tag;
        new_node->next = head;
        new_node->prev = NULL;
        head->prev = new_node;
        data->head_set[set_idx] = new_node;
    }

}

void lru_replacement_policy_cleanup(struct replacement_policy *replacement_policy)
{
    // TODO cleanup any additional memory that you allocated in the
    // lru_replacement_policy_new function.sudo
    struct lru_data *data = (struct lru_data *)replacement_policy->data;
    for (int i = 0; i < data->num_sets; i++) {
        struct lru_node *head = data->head_set[i];
        while (head->next != NULL) {
            struct lru_node *temp = head;
            head = head->next;
            free(temp);
        }
        free(head);
    }
    free(data->head_set);
    free(data);
}

struct replacement_policy *lru_replacement_policy_new(uint32_t sets, uint32_t associativity)
{
    struct replacement_policy *lru_rp = malloc(sizeof(struct replacement_policy));
    lru_rp->cache_access = &lru_cache_access;
    lru_rp->eviction_index = &lru_eviction_index;
    lru_rp->cleanup = &lru_replacement_policy_cleanup;

    // TODO allocate any additional memory to store metadata here and assign to
    // lru_rp->data.

    // Create an array of structs to store the linked list for each set
    // However, the array has to be a double pointer to be able to store the head of the linked lists.
    struct lru_node **lru_lists = calloc(sets, sizeof(struct lru_node*));
    for (int i = 0; i < sets; i++) {
        struct lru_node *head = calloc(1, sizeof(struct lru_node));
        head->tag = -1;
        head->next = NULL;
        head->prev = NULL;
        lru_lists[i] = head;
    }
    struct lru_data *data = malloc(sizeof(struct lru_data));
    data->head_set = lru_lists;
    data->num_sets = sets;
    lru_rp->data = data;

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
struct lruc_data {
    struct lruc_node **head_set;
    uint32_t num_sets;
};
struct lruc_node {
    uint32_t tag;
    enum cache_status status;
    struct lruc_node *next;
    struct lruc_node *prev;
};

const char* get_enum_name(enum cache_status status) {
    switch (status) {
        case INVALID: return "INVALID";
        case EXCLUSIVE: return "EXCLUSIVE";
        case MODIFIED: return "MODIFIED";
        default: return "INVALID";
    }
}

void print_ll(struct lruc_node *head) {
    struct lruc_node *current_node = head;
    printf("Linked List: ");
    while (current_node != NULL) {  // Change from current_node->next to current_node to include last node
        printf(" {0x%x | %s} ", current_node->tag, get_enum_name(current_node->status));
        current_node = current_node->next;
    }
    printf("\n");
}

uint32_t lru_prefer_clean_eviction_index(struct replacement_policy *replacement_policy,
                                         struct cache_system *cache_system, uint32_t set_idx)
{
    // TODO return the index within the set that should be evicted.
    struct lruc_data *data = (struct lruc_data *)replacement_policy->data;
    struct lruc_node *head = data->head_set[set_idx], *tail_node = NULL;
    uint32_t set_start = set_idx * cache_system->associativity;
    uint32_t eviction_index = -1;

    // Find the tail node
    for (tail_node = head; tail_node->next != NULL; tail_node = tail_node->next);

    // We always evict the tail node, due to the way our linked list is structured
    // [MRU MODIFIED ----- LRU MODIFIED][MRU EXCLUSIVE -----LRU EXLUSIVE]
    for (int i = 0; i < cache_system->associativity; i++) {
        if (cache_system->cache_lines[set_start + i].tag == tail_node->tag) {
            eviction_index = i;
            break;
        }
    }
    
    if (tail_node->prev) {tail_node->prev->next = NULL;}
    free(tail_node);

    return eviction_index;
}

void lru_prefer_clean_cache_access(struct replacement_policy *replacement_policy,
                                   struct cache_system *cache_system, uint32_t set_idx,
                                   uint32_t tag)
{
    // TODO update the LRU_PREFER_CLEAN replacement policy state given a new
    // memory access
    // NOTE: you may be able to share code with the LRU policy
    struct lruc_data *data = (struct lruc_data *)replacement_policy->data;
    struct lruc_node *head = data->head_set[set_idx];

    // Need to find cache line within cache_system to determine if it's clean or dirty
    struct cache_line *cl = cache_system_find_cache_line(cache_system, set_idx, tag);

    // Code for when the set is empty
    if (head && head->tag == -1) {
        head->tag = tag;
        head->status = cl->status;
        head->next = NULL;
        head->prev = NULL;
        return;
    }

    // Next we must check if the tag is already in the linked list
    struct lruc_node *current_node = head;
    bool tag_found = false;
    while (current_node) {
        if (current_node->tag == tag) {
            tag_found = true;
            break;
        }
        current_node = current_node->next;
    }

    // Cache is split into two sections: [modified][exclusive]
    // Next we must find the head for the exclusive section
    struct lruc_node *exclusive_head = NULL, *temp = head;
    temp = head;
    while (temp) {
        if (temp->status == EXCLUSIVE) {
            exclusive_head = temp;
            break;
        }
        temp = temp->next;
    }
    if (exclusive_head) {printf("Exclusive head: 0x%x\n", exclusive_head->tag);}
    else {printf("Exclusive head: NULL\n");}
    print_ll(data->head_set[set_idx]);

    if (tag_found) {
        // This is status of the cache line in the linked list, not the cache line in the cache_system
        enum cache_status status = current_node->status;
        if (status == MODIFIED) {
            if (current_node == head) {return;}
            else if (current_node->next == exclusive_head) {
                current_node->prev->next = exclusive_head;
                exclusive_head->prev = current_node->prev;
                current_node->prev = NULL;
                current_node->next = head;
                head->prev = current_node;
                data->head_set[set_idx] = current_node;
            }
            else {
                current_node->prev->next = current_node->next;
                current_node->next->prev = current_node->prev;
                current_node->prev = NULL;
                current_node->next = head;
                head->prev = current_node;
                data->head_set[set_idx] = current_node;
            }
        }
        else if (status == EXCLUSIVE) {
            if (current_node == exclusive_head) {return;}
            else if (current_node->next == NULL) {
                current_node->prev->next = NULL;
                current_node->prev = NULL;
                current_node->next = exclusive_head;
                exclusive_head->prev = current_node;
                data->head_set[set_idx] = current_node;
            }
            else {
                current_node->prev->next = current_node->next;
                current_node->next->prev = current_node->prev;
                current_node->prev = exclusive_head->prev;
                current_node->next = exclusive_head;
                exclusive_head->prev = current_node;
                data->head_set[set_idx] = current_node;
            }
        }
    }
    else {
        // If the tag is not found, we'll have to add it to the head of the linked list
        struct lruc_node *new_node = calloc(1, sizeof(struct lruc_node));
        new_node->tag = tag;
        new_node->status = cl->status;
        if (cl->status == MODIFIED) {
            new_node->next = head;
            new_node->prev = NULL;
            head->prev = new_node;
            data->head_set[set_idx] = new_node;
        }
        else {
            if (exclusive_head) {
                printf("bruh\n");
                new_node->next = exclusive_head;
                new_node->prev = exclusive_head->prev;
                exclusive_head->prev = new_node;
                exclusive_head->prev->next = new_node;
                exclusive_head = new_node;
            }
            else {
                if (head) {
                    new_node->next = NULL;
                    new_node->prev = head;
                    head->next = new_node;
                    head = new_node;
                }
                else {
                    new_node->next = NULL;
                    new_node->prev = NULL;
                }
            }
        }
    }
    print_ll(data->head_set[set_idx]);
}

void lru_prefer_clean_replacement_policy_cleanup(struct replacement_policy *replacement_policy)
{
    // TODO cleanup any additional memory that you allocated in the
    // lru_prefer_clean_replacement_policy_new function.
    struct lruc_data *data = (struct lruc_data *)replacement_policy->data;
    for (int i = 0; i < data->num_sets; i++) {
        struct lruc_node *head = data->head_set[i];
        while (head->next != NULL) {
            struct lruc_node *temp = head;
            head = head->next;
            free(temp);
        }
        free(head);
    }
    free(data->head_set);
    free(data);
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
    struct lruc_node **lruc_lists = calloc(sets, sizeof(struct lruc_node*));
    for (int i = 0; i < sets; i++) {
        struct lruc_node *head = calloc(1, sizeof(struct lruc_node));
        head->tag = -1;
        head->status = INVALID;
        head->next = NULL;
        head->prev = NULL;
        lruc_lists[i] = head;
    }
    struct lruc_data *data = malloc(sizeof(struct lruc_data));
    data->head_set = lruc_lists;
    data->num_sets = sets;
    lru_prefer_clean_rp->data = data;

    return lru_prefer_clean_rp;
}
