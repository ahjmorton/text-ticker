file(GLOB plugins_common_src "plugins/common/*.h" "plugins/common/*.c")
add_library(pluginCommons SHARED ${plugins_common_src})

file(GLOB plugins "plugins/*.c")
foreach(plugin ${plugins})
    get_filename_component(plugin_name ${plugin} NAME_WE)
    add_library(${plugin_name} MODULE ${plugin})
    target_link_libraries(${plugin_name} pluginCommons)
endforeach()

file(GLOB ticker_src "*.h" "*.c")
ADD_EXECUTABLE(text-ticker ${ticker_src})
target_link_libraries(text-ticker ${CMAKE_DL_LIBS})
