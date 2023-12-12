# Example 02: Kernels

## Kernels

```mermaid
mindmap
  root((Kernels))
    id(Basic)
      id("Notes")
        id("No access to local buffers (shared memory in CUDA)")
        id("No access to Barriers")
        id("No sub-groups")
        id("No work-group functionalities (shuffle instructions, ...)")
    id(ND Range)
      id(Notes)
        id(Just like CUDA and OpenCL)
        id(Warp is called Sub-group)
        id(Sub-group is designed to have flexible size if the hardware supports)
      id(Taxonomy)
        id("Work-item = Thread")
        id("Sub-group = Warp")
        id("Work-group = Block")
        id("Work-group = Block")
      id(Classes)
        id("sycl::nd_item")
        id("sycl::nd_range")
        id("sycl::group, contains methods for getting block related indices and sizes.")
        id("sycl::sub_group, contains methods for getting warp related indices and sizes.")
    id(Hierarchical Kernel)
      id(Nested Parallelism)

```

## ND Range vs. CUDA

For a lambda ND kernel such as:

```
[=](sycl::nd_item<2> item) {
    ...
}
```

| CUDA                                  | SYCL                    |
|---------------------------------------|-------------------------|
| threadIdx.x                           | item.get_local_id(0)    |
| blockIdx.x * blockDim.x + threadIdx.x | item.get_global_id(0)   |
| blockIdx.x                            | item.get_group(0)       |
| blockDim.x                            | item.get_local_range(0) |
| gridDim.x                             | item.get_group_range(0) |

So, when `*range` is used as a suffix, it refers to that quantity's size. 
  
**Be careful when dealing with 2D and 3D blocks** and grids, since the axes are defined differently.  
For SYCL, we have `nd_item` where axis=0 is used as the most valuable axis while axis=-1 is used as the least valuable
axis. To put it into perspective, for CUDA, `threadIdx.x` is the least valuable axis, and `threadIdx.z` is the most
valuable axis.

