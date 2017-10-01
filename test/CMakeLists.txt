enable_testing()

find_package(Check)
if(${CHECK_FOUND} STREQUAL "TRUE") 
    message(INFO "Check  found")
else()
    message(WARNING "Check not found, cannot run unit tests")
endif()
