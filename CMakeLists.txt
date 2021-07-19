add_library(mbed-weact-h743vi INTERFACE)

target_sources(mbed-weact-h743vi
    INTERFACE
        PeripheralPins.c
        system_clock.c
)

target_include_directories(mbed-weact-h743vi
    INTERFACE
        .
)

target_link_libraries(mbed-weact-h743vi INTERFACE mbed-stm32h743xi)
