set(target_name E02_Kernels)

add_executable(${target_name}Basic main.basic.cpp)
add_sycl_to_target(TARGET ${target_name}Basic SOURCES main.basic.cpp)

add_executable(${target_name}NdRange main.ndrange.cpp)
add_sycl_to_target(TARGET ${target_name}NdRange SOURCES main.ndrange.cpp)