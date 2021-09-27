add_executable(ISETests TestDriver.cpp Modules/ContainerModule.cpp)

target_link_libraries(ISETests gtest benchmark::benchmark AudioFiles)
target_include_directories(ISETests PUBLIC AudioFiles)