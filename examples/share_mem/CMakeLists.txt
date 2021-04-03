add_executable(share_mem main.cc)
target_link_libraries(share_mem lemon)
target_include_directories(share_mem PRIVATE ${ROOT_DIR}/include)





