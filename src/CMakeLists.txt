file (GLOB Sources "src" "*.cpp")
add_executable (bitto ${Sources})
set_target_properties (bitto PROPERTIES LINKER_LANGUAGE CXX)
install (TARGETS bitto DESTINATION ../bin)
