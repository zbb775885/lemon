add_library(lemon SHARED
            lemon_semaphore.cc
            lemon_share_mem_mgr.cc)

target_include_directories(lemon PRIVATE ${ROOT_DIR}/include)

