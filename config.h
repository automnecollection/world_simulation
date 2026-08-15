#ifndef WORLD_SIM_CONFIG_H
#define WORLD_SIM_CONFIG_H

// General settings
    #define DEBUG                       false
    #define SAVE_WORLD                  false

// Simulation parameters
    #define LOAD_FROM_RESULTS           false
    #define FILE_TYPE                   ".wrld"                //
    #define COUNTRIES_FILE              "countries"            //
    #define PROVINCES_FILE              "provinces"            //
    #define POPULATIONS_FILE            "populations.wrld"     //

    #define DAY                         1                      // Starting day of year (1-365)
    #define YEAR                        1950                   // Starting year, the sim will be designed around starting in 1950
    #define SIM_DAYS                    365                    // How many days the simulation will run for, 27740 days = 76 years (runs to 2026 from 1950)
    #define BASE_BIRTH_RATE             1.0001                 // The base birth rate for dummy population increases

    // Default starting 1950 values
    #define DEF_URBANISATION            30                     //
    #define DEF_COLLEGE_EDU             3                      //
    #define DEF_LITERACY                40                     //
    #define DEF_SECULARISM              5                      //

    // Default target 2026 values
    #define DEF_TARGET_URBANISATION     58                     //
    #define DEF_TARGET_COLLEGE_EDU      20                     //
    #define DEF_TARGET_LITERACY         88                     //
    #define DEF_TARGET_SECULARISM       16                     //

#endif //WORLD_SIM_CONFIG_H
