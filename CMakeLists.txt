cmake_minimum_required(VERSION 3.5)

# Register Firmware/ as the main component
set(EXTRA_COMPONENT_DIRS Firmware)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(jiridium_fw)
