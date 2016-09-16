include_directories (../src)
file (GLOB_RECURSE SRC "src" "*.cpp")
list (REMOVE_ITEM SRC "${CMAKE_CURRENT_LIST_DIR}/asio/impl/src.cpp")
file (GLOB_RECURSE INC "src" "*.h")

find_package(Threads REQUIRED)
add_executable (bitto ${SRC} ${INC})
if(THREADS_HAVE_PTHREAD_ARG)
  target_compile_options(PUBLIC bitto "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
  target_link_libraries(bitto "${CMAKE_THREAD_LIBS_INIT}")
endif()

set_property(TARGET bitto PROPERTY CXX_STANDARD 11)
set_property(TARGET bitto PROPERTY CXX_STANDARD_REQUIRED ON)


# Credit to Florian @StackOverflow
function(assign_source_group)
    foreach(_source IN ITEMS ${ARGN})
        if (IS_ABSOLUTE "${_source}")
            file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
        else()
            set(source_rel "${_source}")
        endif()
        get_filename_component(_source_path "${_source_rel}" PATH)
        string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
        source_group("${_source_path_msvc}" FILES "${_source_rel}")
	message("${_source_path_msvc} - ${_source_rel}")
    endforeach()
endfunction(assign_source_group)

assign_source_group(${SRC})
assign_source_group(${INC})

set_target_properties (bitto PROPERTIES LINKER_LANGUAGE CXX)
install (TARGETS bitto DESTINATION ../bin)
