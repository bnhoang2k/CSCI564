1. Describe how your prefetcher works.

When a cache miss occurs, it stores the address of the missed block in a buffer. This buffer is called "eviction_ll" in my code. The buffer itself uses LRU replacement policy. When the buffer is full, it evicts the least recently used block. The LRU replacement policy logic is implemented in custom_handle_mem_access.

When a cache miss occurs, the prefetcher gets called. Then custom_handle_mem_access will check the eviction linked list to see if it's being stored. If it is, it'll prefetch the singular block. If it's not, the function creates a new node in the eviction linked list and does nothing. It'll only prefetch the block if it's in the eviction linked list.

2. Explain how you chose that prefetch strategy.

I thought it'd be good to create a separate data structure that holds recently evicted blocks. This is good because it covers the possibility that programs may need to re-access instructions that were recently evicted.

3. Discuss the pros and cons of your prefetch strategy.

Pros:
- Simple to implement because it's based on the LRU code already written in the previous project.
- The eviction linked list size is adjustable, so it can be tuned to the program's needs.
- It's a good strategy for programs that need to re-access recently evicted blocks. This occurs primarily at loops or when the program is accessing a large amount of data.

Cons:
- It's not a good strategy for programs that don't need to re-access recently evicted blocks, resulting in cache pollution. This occurs when the program is accessing a large amount of data that doesn't need to be re-accessed or the program doesn't use that many loops. Trace3 is an example of this because all of the memory locations are accessed one and never again.
- It's relatively straight-forward and doesn't take into account the program's behavior. It's a one-size-fits-all strategy.

4. Demonstrate that the prefetcher could be implemented in hardware (this can be
   as simple as pointing to an existing hardware prefetcher using the strategy
   or a paper describing a hypothetical hardware prefetcher which implements
   your strategy).

The prefetcher I implemented is basically another cache that holds recently evicted blocks. This is similar to the L2 cache in a computer system.

5. Cite any additional sources that you used to develop your prefetcher.

I got inspiration from the LRU code I wrote in the previous project.