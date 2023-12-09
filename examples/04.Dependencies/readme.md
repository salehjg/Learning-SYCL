# Example 04: Dependencies and Schedule

```mermaid
mindmap
root(("Dependencies In A Command Group")
  id("In-order Queue")
    id("One task after another = Implicitly Defined Dependencies.")
  id("Out-of-order Queue")
    id("Explicitly with handler::depends_on()")
    id("Explicitly with queue::parallel_for(range{N}, {event1, event2}, ...);")
    id("Explicitly with queue::single_task(event1, ...);")
    id("Implicitly with Accessors")
```


```mermaid
mindmap
root(("Command Group")
  id("To run a kernel")
    id("queue::submit()")
    id("queue::parallel_for()")
    id("queue::single_task()")
  id("To move data")
    id("Implicitly")
      id("USM")
      id("Buffers + Accessors")
    id("Explicitly")
      id("USM")
        id("queue::memcpy()")
        id("handler::memcpy()")
      id("Buffers")
        id("handler::copy() to move data between the host memory and an accessor.")
        id("handler::update_host() to update the actual host pointer of the buffer in the host memory. See footnote 1.")
```

```mermaid
mindmap
root(("Synchronization")
  id("Host Thread")
    id("queue::wait() and queue::wait_and_throw()")
    id("event1.wait()")
    id("host accessors for buffers. See footnote 2, 3, and 4.")
```

1. From DPCPP book: *"The update_host method is a very specialized form of copy. If a buffer was created around a host pointer, this method will copy the data represented by the accessor back to the original host memory. This can be useful if a program manually synchronizes host data with a buffer that was created with the special use_mutex property. However, this use case is not likely to occur in most programs."*

2. From DPCPP book: *"Note that if the buffer was constructed from existing host memory, this original memory is not guaranteed to contain the updated values."*

3. Host accessors are blocking on the host thread. From DPCPP book: *"Note that host accessors are blocking. Execution on the host may not proceed past the creation of the host accessor until the data is available. Likewise, a buffer cannot be used on a device while a host accessor exists and keeps its data available. A common pattern is to create host accessors inside additional C++ scopes in order to free the data once the host accessor is no longer needed. This is an example of the next method for host synchronization. Host accessors can make data remain on the host until they are destroyed."*

4. From DPCPP book: *"Once a buffer is no longer being used by any kernel or memory operation, the runtime may have to copy data back to the host. This copy occurs either if the buffer was initialized with a host pointer or if a host pointer was passed to the method set_ final_data. The runtime will then copy back the data for that buffer and update the host pointer before the object is destroyed."*

**Read more** about buffers that are created with the option `use_mutex`.
