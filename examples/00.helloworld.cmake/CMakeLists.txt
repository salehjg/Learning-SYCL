set(target_name E00_HelloWorld)
add_executable(${target_name} main.cpp)
add_sycl_to_target(TARGET ${target_name} SOURCES main.cpp)
