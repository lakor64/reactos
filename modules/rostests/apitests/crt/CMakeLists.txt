
if (GCC)
    add_compile_options(-fno-builtin)
endif()

include(ntdll_crt_apitest.cmake)
include(msvcrt_crt_apitest.cmake)
include(static_crt_apitest.cmake)

if(NOT ARCH STREQUAL "amd64" AND NOT ARCH STREQUAL "arm")
  include(crtdll_crt_apitest.cmake)
endif()
