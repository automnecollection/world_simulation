# PLAN

## **AI**
The simulation will have many different agents that will be allocated many different decisions to make and different ways to influence the simulation world. 

### **Agent Types**
- Characters
- - This simulation will be aimed at having characters drive the simulation. All other agent types will have character decision-making embedded inside the agent, characters that would be considered important to influence decision-making (leaders, relevant rebels), etc., but with most the collective decision of all other people being abstracted instead of simulated character by character.
- Governments
- - All relevant governors will be simulated through individual characters (like Crusader Kings).
- - Governments will make decisions about law, economy, war, everything relevant to real-life countries. 
- - The decision results of passing laws and policies will be determined by votes by governors, their votes are individual decisions.
- - Governments will mainly come in the form of countries, but could represent self-governing militias, non-recognised states, terrorist groups, very organised crime / drug trafficking syndicates etc. 
- - Rebels groups will have the ability to control province land in the country they rebelled in, becoming a government entity like all others.
- Military units
- - Unit decision-making will be influenced by commanders and direct government orders, but parts of / full units will have the ability to disobey orders, even rebel and split off into other countries depending on game conditions.

### PLANNING

population_income_pool = building_employee_income - demand.
building_income_pool = sold (sold = demand (max supply)) * item_cost.
government_income_pool = building_income_pool * tax_rate.
government_income_pool -= funds_spent_on_building_levels.
building_levels creates employment and increases income generated from building type.
subsequently increases government income.

What should determine desired building level for each building type in a province?
Overall country demand should factor into the calculation somehow
Determine the best provinces to build that type of building in.
Calculate where is unemployment high, where is it most profitable (most demand) to build up levels for a b_type to determine best provinces for building level increase.
Also calculate the overall amount of employment needed for every province to determine if the government is overbuilding.
Try and separate building into private and public (government) sectors.
Somehow have a system where building types will have a calculation of whether a government (pro or anti private business) will be touched by government construction and or intervention. 
This system is needed because having the government building and running casinos and other types of buildings would be strange typically.
And some governments choose intervene into housing, construction, farming, industry, medical or illicit drugs etc. to a higher extent than others typically do, socialist and communist governments for example.
Western pro-capitalist governments in the simulation will typically want to avoid building in these industries less, unless the simulation is divergent enough to cause them to do so.
