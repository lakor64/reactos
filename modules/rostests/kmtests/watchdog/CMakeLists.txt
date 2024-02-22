
include_directories(../include)

list(APPEND EXAMPLE_DRV_SOURCE
    ../kmtest_drv/kmtest_standalone.c
    watchdog_drv.c)

add_library(watchdog_drv MODULE ${EXAMPLE_DRV_SOURCE})
set_module_type(watchdog_drv kernelmodedriver)
target_link_libraries(watchdog_drv kmtest_printf ${PSEH_LIB})
add_importlibs(watchdog_drv ntoskrnl hal watchdog)
target_compile_definitions(watchdog_drv PRIVATE KMT_STANDALONE_DRIVER)
#add_pch(example_drv ../include/kmt_test.h)
add_rostests_file(TARGET watchdog_drv)
