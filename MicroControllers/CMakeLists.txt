# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.0.0)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
cmake_minimum_required(VERSION 3.13)

set(PROGRAM_NAME pico_main)
set(PICO_BOARD pico_w)

include(pico_sdk_import.cmake)

project(pico_main)

pico_sdk_init()



add_executable(${PROGRAM_NAME}
    main.c
    Drivers/cJSON.c
    Drivers/commandExec.c
    Drivers/Communication/myWifi.c
    Drivers/Communication/uart_communication.c
    Drivers/Actuators/buzzer.c
    Drivers/Actuators/TFT_arduino.c
    Drivers/Actuators/led_strip.c
    Drivers/Sensors/adc_temperature.c
    Drivers/Sensors/ultrasonic.c
    MCs/my_pico.c
    MCs/my_arduino.c
)

target_include_directories(${PROGRAM_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(${PROGRAM_NAME}
    pico_cyw43_arch_lwip_threadsafe_background
    pico_lwip_http
    pico_stdlib
    hardware_adc
    hardware_timer
    hardware_irq
    hardware_pwm
)

pico_enable_stdio_usb(${PROGRAM_NAME} TRUE)
pico_enable_stdio_uart(${PROGRAM_NAME} TRUE)

pico_add_extra_outputs(${PROGRAM_NAME})