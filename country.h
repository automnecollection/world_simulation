#ifndef COUNTRY_H
#define COUNTRY_H

// country.h

/**
 * struct Country - Represents a nation state
 * @name: Human-readable name (e.g., "France")
 * @id: Unique numeric identifier
 * @tag: 3-letter country code (e.g., "FRA")
 * @provinces_num: Count of provinces owned by this country
 * @provinces: Array of province IDs owned by this country
 *
 * TODO: Add capital_province_id, government_type, wealth
 */

struct Country {
    char * name;
    int id;
    char * tag;
    int provinces_num;

    char provinces[];
};

struct CountryList {
    int countries_num;
    struct Country *countries;
};

// INITIALISATION
struct CountryList initialise_countries(FILE * file);
struct Country read_country(char * line, int id);

// HELPERS
int get_country_id_from_tag(const char * tag, struct Country countries[], int countries_size);

// SIMULATION

// MEMORY MANAGEMENT
void free_the_nations(struct Country countries[], int countries_num);

#endif
