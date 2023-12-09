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
        id("handler::update_host() to update the actual host pointer of the buffer in the host memory.")
```



