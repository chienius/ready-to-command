link_directories(
    ${GTKMM_LIBRARY_DIRS}  )

include_directories(
    ${GTKMM_INCLUDE_DIRS}  )

add_executable(ready-to-command
    main.cpp
    main-window.cpp
    main-window.cpp
    main-application.hpp
    execution-thread.cpp
    execution-thread.hpp
    command-config.hpp
)

configure_file(../res/main-window.glade res/main-window.glade COPYONLY)

target_link_libraries(ready-to-command
    ${GTKMM_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT})
