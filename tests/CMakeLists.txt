add_executable(lirs ./src/lirs.cpp)
add_executable(perfect_cache ./src/perfect_cache.cpp)

target_include_directories(lirs
                           PRIVATE ${INCLUDE_DIR})
target_include_directories(perfect_cache
                           PRIVATE ${INCLUDE_DIR})

install(TARGETS lirs
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT lirs)

install(TARGETS perfect_cache
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT perfect_cache)
