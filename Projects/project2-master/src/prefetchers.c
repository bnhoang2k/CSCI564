//
// This file defines the function signatures necessary for creating the three
// cache systems and defines the prefetcher struct.
//

#include "prefetchers.h"

// Null Prefetcher
// ============================================================================
uint32_t null_handle_mem_access(struct prefetcher *prefetcher, struct cache_system *cache_system,
                                uint32_t address, bool is_miss)
{
    return 0; // No lines prefetched
}

void null_cleanup(struct prefetcher *prefetcher) {}

struct prefetcher *null_prefetcher_new()
{
    struct prefetcher *null_prefetcher = calloc(1, sizeof(struct prefetcher));
    null_prefetcher->handle_mem_access = &null_handle_mem_access;
    null_prefetcher->cleanup = &null_cleanup;
    return null_prefetcher;
}

// Sequential Prefetcher
// ============================================================================
// TODO feel free to create additional structs/enums as necessary

uint32_t sequential_handle_mem_access(struct prefetcher *prefetcher,
                                      struct cache_system *cache_system, uint32_t address,
                                      bool is_miss)
{
    // TODO: Return the number of lines that were prefetched.
    if (*(uint32_t*)prefetcher->data == 0) {return 0;}
    for (uint32_t i = 1; i <= *(uint32_t*)prefetcher->data; i++) {
        uint32_t next_address = address + (cache_system->line_size * i);
        cache_system_mem_access(cache_system, next_address, 'R', true);
    }
    return *(uint32_t*)prefetcher->data;
}

void sequential_cleanup(struct prefetcher *prefetcher)
{
    // TODO cleanup any additional memory that you allocated in the
    // sequential_prefetcher_new function.
    free(prefetcher->data);
}

struct prefetcher *sequential_prefetcher_new(uint32_t prefetch_amount)
{
    struct prefetcher *sequential_prefetcher = calloc(1, sizeof(struct prefetcher));
    sequential_prefetcher->handle_mem_access = &sequential_handle_mem_access;
    sequential_prefetcher->cleanup = &sequential_cleanup;

    // TODO allocate any additional memory needed to store metadata here and
    // assign to sequential_prefetcher->data.
    
    // Store prefetch amount in data
    uint32_t *data = malloc(sizeof(uint32_t));
    *data = prefetch_amount;
    sequential_prefetcher->data = data;
    return sequential_prefetcher;
}

// Adjacent Prefetcher
// ============================================================================
uint32_t adjacent_handle_mem_access(struct prefetcher *prefetcher,
                                    struct cache_system *cache_system, uint32_t address,
                                    bool is_miss)
{
    // TODO perform the necessary prefetches for the adjacent strategy.
    // TODO: Return the number of lines that were prefetched.
    // For the adjacent prefetcher, whenever a cache line is accessed, the next
    // cache line should be prefetched.
    uint32_t next_address = address + cache_system->line_size;
    cache_system_mem_access(cache_system, next_address, 'R', true);
    return 1;
}

void adjacent_cleanup(struct prefetcher *prefetcher) {}

struct prefetcher *adjacent_prefetcher_new()
{
    struct prefetcher *adjacent_prefetcher = calloc(1, sizeof(struct prefetcher));
    adjacent_prefetcher->handle_mem_access = &adjacent_handle_mem_access;
    adjacent_prefetcher->cleanup = &adjacent_cleanup;
    return adjacent_prefetcher;
}

// Custom Prefetcher
// ============================================================================

#define EVICTED_CACHE_SIZE 64

struct evicted_ll {
    struct node *head;
    struct node *tail;
    uint32_t size;
};

struct node {
    uint32_t address;
    struct node *next;
    struct node *prev;
};

uint32_t get_ll_size(struct evicted_ll *evicted_ll) {
    uint32_t size = 0;
    struct node *current = evicted_ll->head;
    while (current != NULL) {
        size++;
        current = current->next;
    }
    return size;
}

uint32_t custom_handle_mem_access(struct prefetcher *prefetcher, struct cache_system *cache_system,
                                  uint32_t address, bool is_miss) {
    
    if (!is_miss) return 0; // Only prefetch on misses

    struct evicted_ll *evicted_ll = prefetcher->data;
    struct node *current = evicted_ll->head;
    struct node *previous = NULL;

    while (current != NULL) {
        if (current->address == address) {
            if (current != evicted_ll->head) {
                // Remove current from its position
                if (current->next) current->next->prev = current->prev;
                if (current->prev) current->prev->next = current->next;
                if (current == evicted_ll->tail) evicted_ll->tail = current->prev;

                // Move current to the front
                current->next = evicted_ll->head;
                current->prev = NULL;
                evicted_ll->head->prev = current;
                evicted_ll->head = current;
            }
            // Perform prefetch for the current address
            cache_system_mem_access(cache_system, current->address, 'R', true);
            return 1;
        }
        previous = current;
        current = current->next;
    }

    // Address not found, create new node at the front
    struct node *new_node = malloc(sizeof(struct node));
    new_node->address = address;
    new_node->next = evicted_ll->head;
    new_node->prev = NULL;
    if (evicted_ll->head) {
        evicted_ll->head->prev = new_node;
    }
    evicted_ll->head = new_node;
    if (evicted_ll->tail == NULL) {
        evicted_ll->tail = new_node;
    }

    if (++evicted_ll->size > EVICTED_CACHE_SIZE) {
        // Remove last node
        struct node *last = evicted_ll->tail;
        if (last->prev) {
            evicted_ll->tail = last->prev;
            evicted_ll->tail->next = NULL;
        }
        free(last);
        evicted_ll->size--;
    }

    return 0;
}

void custom_cleanup(struct prefetcher *prefetcher)
{
    // TODO cleanup any additional memory that you allocated in the
    // custom_prefetcher_new function.
    struct evicted_ll *evicted_ll = prefetcher->data;
    struct node *current = evicted_ll->head;
    while (current != NULL) {
        struct node *next = current->next;
        free(current);
        current = next;
    }
    free(evicted_ll);
}

struct prefetcher *custom_prefetcher_new()
{
    struct prefetcher *custom_prefetcher = calloc(1, sizeof(struct prefetcher));
    custom_prefetcher->handle_mem_access = &custom_handle_mem_access;
    custom_prefetcher->cleanup = &custom_cleanup;

    // TODO allocate any additional memory needed to store metadata here and
    // assign to custom_prefetcher->data.
    struct evicted_ll *evicted_ll = malloc(sizeof(struct evicted_ll));
    evicted_ll->head = NULL;
    evicted_ll->tail = NULL;
    evicted_ll->size = EVICTED_CACHE_SIZE;
    custom_prefetcher->data = evicted_ll;

    return custom_prefetcher;
}
