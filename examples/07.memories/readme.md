# Memories
## Global
### Buffers
Buffers are designed to automate data movements by C++ RAII concept.  
You can inistantiate a `sycl::buffer` with or without the host array.  
For example:
```
sycl::buffer<float, 1> myBuff(sycl::range<1>(1024));
```
These buffers should be accessed on the host side by Host Accessors and on the device side by Accessors.
An example for the host side access:
```
auto acc = myBuff.get_host_access(sycl::read_only);
```
An example for the device side accessor:
```
auto acc = myBuff.get_access<sycl::access::mode::write>(cgh);
```

To have offset and range set for a buffer instance, either you need a kernel or a sub-buffer. A sub-buffer is practically a pointer to the given offset of the already allocated memory with an extra range (len) limit.  
An example of defining a sub-buffer:  
```
auto subBuf = sycl::buffer(
                        myBuff,
                        sycl::id<1>(512),     // offset in words
                        sycl::range<1>(64)    // len in words
                );
```
Be aware that oneMKL has some issues with sub-buffers. You might have to make identical copies of the data into another buffers (instead of making sub-buffers) in order to get it working. oneMKL works great with USM.

### Unified Shared Memory (USM)
This is closer to what we have in CUDA. There are three important variants. 
#### malloc_host
This variant allocates a memory that resides on the host and device could access it automatically (word-by-word) through PCIe. As you can guess, if you have a large buffer and your kernel only needs access to like 10 elements of this buffer, this variant of memory allocation could potentially be super fast. BUT, it becomes super slow if your kernel issues a lot a accesses (as each one individually has to be resolved through the bus).

#### malloc_shared
The other variant is to use `sycl::malloc_shared()`. This function allocates a memory that is managed by the compiler. Depending on how good the compiler is understanding your access patterns on the host and on the device codes, the automated data movements could be efficient or way worst than manual handling.
Be aware that both the host and the device can access this type of allocated memory as well.

#### malloc_device
This is practically the sycl version of cuda malloc. It allocates a memory that resides on the device global memory and only the device can access it.  
For moving data from a heap C++ array to a device memory pointer, you need to use `sycl::queue::memcpy(dest, src, size_in_bytes)`.

For a great comparison on the achievable performance for these three approaches, please refer to `SYCL-Bench 2020: Benchmarking SYCL 2020 on AMD, Intel, and NVIDIA GPUs` paper, Figure 3. You can access [the preprint here](https://cosenza.eu/papers/CrisciIWOCL24.pdf).


## Local
TODO.

## Private
TODO.

