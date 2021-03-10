
if (TARGET_NT LESS 0x600)
	remove_definitions(-D_WIN32_WINNT=${TARGET_NT} -DWINVER=${TARGET_NT})
	add_definitions(-D_WIN32_WINNT=0x600 -DWINVER=0x600)
endif()

include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
add_definitions(-D__WINESRC__)

list(APPEND SOURCE
    audio.c
    dispatch.c
    dllfunc.c
    enumpins.c
    filter.c
    mediatype.c
    outputqueue.c
    pin.c
    pospass.c
    qualitycontrol.c
    renderer.c
    seeking.c
    transform.c
    video.c
    window.c
    precomp.h)

add_library(strmbase ${SOURCE})
add_pch(strmbase precomp.h SOURCE)
add_dependencies(strmbase psdk dxsdk)
