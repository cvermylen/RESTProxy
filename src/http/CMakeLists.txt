set(HTTP_SOURCE_FILES
  http_first_line.c
  http_headers.c
  http_message.c
)
add_library(http ${HTTP_SOURCE_FILES})