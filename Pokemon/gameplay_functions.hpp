#pragma once
#include <string>
#include <vector>
#include "pokemon.hpp"

void start();
void create_team();
void battle();
void create_pokemon();
void add_pokemon(std::vector<std::string>& u_team);
void battle(std::vector<pokemon>& p1_team, std::vector<pokemon>& p2_team,
	pokemon& p1_lead, pokemon& p2_lead);