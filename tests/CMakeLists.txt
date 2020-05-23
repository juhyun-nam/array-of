
add_executable(
  array_of_test
  array_of_test.cpp
)

add_test(
  NAME array_of_test
  COMMAND $<TARGET_FILE:array_of_test> --success
)
