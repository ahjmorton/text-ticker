file(GLOB plugins "plugins/*.c")
foreach(plugin ${plugins})
    get_filename_component(plugin_name ${plugin} NAME_WE)
    add_library(${plugin_name} MODULE ${plugin})
endforeach()

file(GLOB ticker_src "*.h" "*.c")
ADD_EXECUTABLE(text-ticker ${ticker_src})
