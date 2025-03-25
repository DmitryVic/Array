cmake_minimum_required(VERSION 3.10)
project(Array)

# Добавить исходники
add_executable(Array main.cpp)

# Подключить заголовки из include/
target_include_directories(Array PRIVATE include/)