set(SRC
    hattop.c
    hattop.h
    request.c
    request.h
    socket.c
    socket.h
    structs.h
    thread.c
    thread.h)

add_library(hattop ${SRC})
IF (WIN32)
target_link_libraries(hattop Ws2_32.Lib)
ENDIF()

