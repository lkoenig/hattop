set(SRC
    hattop.c
    hattop.h
    response.c
    response.h
    request.c
    request.h
    socket.c
    socket.h
    thread.c
    thread.h)

add_library(hattop ${SRC})
IF (WIN32)
target_link_libraries(hattop WSock32.Lib)
ENDIF()

