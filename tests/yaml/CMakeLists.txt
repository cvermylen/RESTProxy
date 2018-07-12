
include_directories(SYSTEM /usr/local/include/)

add_executable(yaml_TEST yaml_TEST.c)
target_link_libraries(yaml_TEST ${CRITERION_LIBRARIES})

set_property (TARGET yaml_TEST PROPERTY C_STANDARD 99)

add_test(yaml_TEST ${EXECUTABLE_OUTPUT_PATH}/yaml_TEST)
