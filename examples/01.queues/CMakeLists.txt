set(target_name E01_Queues)

add_executable(${target_name}InOrder main.inorder.cpp )
add_sycl_to_target(TARGET ${target_name}InOrder SOURCES main.inorder.cpp)

add_executable(${target_name}OutOfOrderImplicit main.outoforder.implicit.cpp)
add_sycl_to_target(TARGET ${target_name}OutOfOrderImplicit SOURCES main.outoforder.implicit.cpp)

add_executable(${target_name}OutOfOrderExplicit main.outoforder.explicit.cpp)
add_sycl_to_target(TARGET ${target_name}OutOfOrderExplicit SOURCES main.outoforder.explicit.cpp)
