
include_directories(../include)

list(APPEND WACTHDOG_DRV_SOURCE
    ../kmtest_drv/kmtest_standalone.c
    watchdog_drv.c
    wddpc.c
    wddeferred.c
    wdnormal.c)

add_library(watchdog_drv MODULE ${WACTHDOG_DRV_SOURCE})
set_module_type(watchdog_drv kernelmodedriver)
target_link_libraries(watchdog_drv kmtest_printf ${PSEH_LIB})
add_importlibs(watchdog_drv ntoskrnl hal watchdog)
target_compile_definitions(watchdog_drv PRIVATE KMT_STANDALONE_DRIVER)
#add_pch(WatchDog_drv ../include/kmt_test.h)
add_rostests_file(TARGET watchdog_drv)
