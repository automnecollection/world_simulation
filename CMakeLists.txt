cmake_minimum_required(VERSION 4.2)
project(world_sim C)

set(CMAKE_C_STANDARD 17)

add_executable(
        world_sim main.c
        province.h
        province.c
        country.h
        country.c
        population.h
        population.c
        world_functions.h
        world_functions.c
        natural_resource.c
        natural_resource.h
        building_type.c
        building_type.h
        world.c
        world.h
        item.h
        item.c
        create_save.c
        create_save.h
        building.c
        building.h
        save_analysis.c
        save_analysis.h
        config.h
        language.h
        simulation.c
        simulation.h
        list_init.c
        list_init.h
        list_init.c
        unit.c
        unit.h
        list_init.c
        create_save_json.c
        create_save_json.h
)

# Request assembly output generation (-S) and use readable Intel syntax
if(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -save-temps=obj -fdump-tree-optimized -masm=intel")
endif()

if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-fsanitize=address,undefined -g)
    add_link_options(-fsanitize=address,undefined)
endif()