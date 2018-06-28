cmake_minimum_required (VERSION 3.8)
project (LogonRenderer)

# Including external libs
include_directories(
                    libs/assimp/4.1.0/include/
                    libs/glm/0.9.9.0/include/
                    libs/imagemagick/7.0.8-3/include/
                    )

# Macro to include all header files

MACRO(HEADER_DIRECTORIES return_list)
    FILE(GLOB_RECURSE new_list *.h)
    SET(dir_list "")
    FOREACH(file_path ${new_list})
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        SET(dir_list ${dir_list} ${dir_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES dir_list)
    SET(${return_list} ${dir_list})
ENDMACRO()

HEADER_DIRECTORIES("headers/")


add_executable(LogonRenderer logonRenderer.cxx)
