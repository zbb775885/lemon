add_executable(demo main.cc)
target_link_libraries(demo PRIVATE  dl)
target_include_directories(demo PRIVATE ${ROOT_DIR}/include)


include_directories(${ROOT_DIR}/include ./)
add_library(derive1 SHARED derive1.cc)
add_library(derive2 SHARED derive2.cc)
add_library(tt SHARED test.cc)




