# Example 00: HelloWorld!
`IntelSYCL` works great out of the box with CMake. I ran my tests on my Arch machine.  
Here is a list of installed packages:  
```
$ pacman -Q | grep oneapi
intel-oneapi-common 2023.2.0-1
intel-oneapi-compiler-dpcpp-cpp-runtime 2023.2.0-1
intel-oneapi-compiler-dpcpp-cpp-runtime-libs 2023.2.0-1
intel-oneapi-compiler-shared 2023.2.0-1
intel-oneapi-compiler-shared-runtime 2023.2.0-1
intel-oneapi-compiler-shared-runtime-libs 2023.2.0-1
intel-oneapi-dev-utilities 2021.9.0_44447-2
intel-oneapi-dpcpp-cpp 2023.2.0-1
intel-oneapi-dpcpp-debugger 2023.2.0_49330-1
intel-oneapi-mkl 2023.2.0_49495-2
intel-oneapi-openmp 2023.2.0-1
intel-oneapi-tbb 2021.10.0-3
```

Unfortunately, my CPU (`12100F`) does not have a iGPU. 
`opencl` worked but I couldn't confirm that it actually utilized my GPU! It felt like it was again running on my CPU, idk.  
I have to read some documents first. Ah, also, `ext_cuda` didn't work.
