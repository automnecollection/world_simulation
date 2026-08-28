def data_analysis(countries):
    for c in countries:
        if c is not None:
            fifties_pop = c.total_population.get("1950")
            twenties_pop = c.total_population.get("2026")
            real_twenties_pop = c.real_total_population.get("2026")
            if twenties_pop and fifties_pop > 0:
                c.pop_percent_increase = (twenties_pop / fifties_pop) * 100
            if real_twenties_pop and twenties_pop:
                if real_twenties_pop and twenties_pop > 0:
                    c.real_accuracy = float(twenties_pop / real_twenties_pop)
