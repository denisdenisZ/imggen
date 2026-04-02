add_library(imgen
    imgen.c
    imgfactory.c
    fxdpoint.c
    hist_calc.c
    imgio.c
    imgmath.c
    basic_patterns.c
)

target_link_libraries(imgen
    m
)

target_include_directories(imgen PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

add_executable(main
    main.c
)

target_link_libraries(main PRIVATE
    imgen
)

install(TARGETS main
    RUNTIME DESTINATION bin
)
