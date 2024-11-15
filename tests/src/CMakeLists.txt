add_executable(lirs ./src/lirs.cpp)
add_executable(perfect_cache ./src/perfect_cache.cpp)

target_include_directories(lirs
                           PRIVATE ${INCLUDE_DIR})
target_include_directories(perfect_cache
                           PRIVATE ${INCLUDE_DIR})
