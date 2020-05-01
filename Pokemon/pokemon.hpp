#pragma once
#include <string>
#include <unordered_map>
#include "helper_maps.hpp"
#include "helper_functions.hpp"

namespace util {
	void print(std::string text) {
		for (int i = 0; i < text.length(); i++) {
			std::cout << text[i];
			Sleep(50);
		}

		std::cout << std::endl;
	}

	std::vector<std::string> split(std::string string) {
		std::vector<std::string> ans;
		std::stringstream stream(string);
		std::string next_line;

		while (std::getline(stream, next_line, ' ')) {
			ans.push_back(next_line);
		}

		return ans;
	}
}

class pokemon {
private:
	int b_hp, b_attack, b_defense, b_special_attack, b_special_defense, b_speed;
	float type_effectivenss[18][18]{
		{1, 1, 1, 1, 1, 0.5, 1, 0, 0.5, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{2, 1, 0.5, 0.5, 1, 2, 0.5, 0, 2, 1, 1, 1, 1, 0.5, 2, 1, 2, 0.5},
		{1, 2, 1, 1, 1, 0.5, 2, 1, 0.5, 1, 1, 2, 0.5, 1, 1, 1, 1, 1},
		{1, 1, 1, 0.5, 0.5, 0.5, 1, 0.5, 0, 1, 1, 2, 1, 1, 1, 1, 1, 2},
		{1, 1, 0, 2, 1, 2, 0.5, 1, 2, 2, 1, 0.5, 2, 1, 1, 1, 1, 1},
		{1, 0.5, 2, 1, 0.5, 1, 2, 1, 0.5, 2, 1, 1, 1, 1, 2, 1, 1, 1},
		{1, 0.5, 0.5, 0.5, 1, 1, 1, 0.5, 0.5, 0.5, 1, 2, 1, 2, 1, 1, 2, 0.5},
		{0, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 0.5, 1},
		{1, 1, 1, 1, 1, 2, 1, 1, 0.5, 0.5, 0.5, 1, 0.5, 1, 2, 1, 1, 2},
		{1, 1, 1, 1, 1, 0.5, 2, 1, 2, 0.5, 0.5, 2, 1, 1, 2, 0.5, 1, 1},
		{1, 1, 1, 1, 2, 2, 1, 1, 1, 2, 0.5, 0.5, 1, 1, 1, 0.5, 1, 1},
		{1, 1, 0.5, 0.5, 2, 2, 0.5, 1, 0.5, 0.5, 2, 0.5, 1, 1, 1, 0.5, 1, 1},
		{1, 1, 2, 1, 0, 1, 1, 1, 1, 1, 2, 0.5, 0.5, 1, 1, 0.5, 1, 1},
		{1, 2, 1, 2, 1, 1, 1, 1, 0.5, 1, 1, 1, 1, 0.5, 1, 1, 0, 1},
		{1, 1, 2, 1, 2, 1, 1, 1, 0.5, 0.5, 0.5, 2, 1, 1, 0.5, 2, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 0.5, 1, 1, 1, 1, 1, 1, 2, 1, 0},
		{1, 0.5, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 0.5, 0.5},
		{1, 2, 1, 0.5, 1, 1, 1, 1, 0.5, 0.5, 1, 1, 1, 1, 1, 2, 2, 1}
	};
	std::unordered_map<std::string, int> type_indices{
		{"normal", 0},
		{"fighting", 1},
		{"flying", 2},
		{"poison", 3},
		{"ground", 4},
		{"rock", 5},
		{"bug", 6},
		{"ghost", 7},
		{"steel", 8},
		{"fire", 9},
		{"water", 10},
		{"grass", 11},
		{"electric", 12},
		{"psychic", 13},
		{"ice", 14},
		{"dragon", 15},
		{"dark", 16},
		{"fairy", 17}
	};
public:
	int s_attack, s_defense, s_special_attack, s_special_defense, s_speed;
	float max_hp, s_hp;
	std::string name, type1, type2, move1, move2, move3, move4, nick;
public:
	pokemon(std::string& _name, std::string& _nick, std::string& _move1, std::string& _move2, std::string& _move3, std::string& _move4) {
		name = _name;
		nick = _nick;
		std::vector<std::string> data = util::split(pokemon_data[_name]);
		type1 = data[2];
		type2 = data[3];
		b_hp = std::stoi(data[4]);
		b_attack = std::stoi(data[5]);
		b_defense = std::stoi(data[6]);
		b_special_attack = std::stoi(data[7]);
		b_special_defense = std::stoi(data[8]);
		b_speed = std::stoi(data[9]);
		s_hp = (2 * b_hp) + 110;
		max_hp = s_hp;
		s_attack = b_attack * 2 + 5;
		s_defense = b_defense * 2 + 5;
		s_special_attack = b_special_attack * 2 + 5;
		s_special_defense = b_special_defense * 2 + 5;
		s_speed = b_speed * 2 + 5;
		move1 = _move1;
		move2 = _move2;
		move3 = _move3;
		move4 = _move4;
	}
public:
	bool take_damage(float damage) {
		s_hp -= damage;
		return s_hp <= 0;
	}
	bool attack(std::string& move, pokemon& other, int player_num) {
		std::vector<std::string> data = move_data[move];
		float f_rand = ((rand() % 15) + 85.0) / 100.0;
		float STAB = (data[1] == type1 || data[1] == type2) ? 1.5 : 1;
		float type = type_effectivenss[type_indices[move]][type_indices[other.type1]] *
			type_effectivenss[type_indices[move]][type_indices[other.type2]];

		const float MODIFIER = f_rand * STAB * type;
		const float POWER = std::stoi(data[2]);
		const float ATTACK = data[1] == "p" ? s_attack : s_special_attack;
		const float DEFENSE = data[1] == "p" ? other.s_defense : other.s_special_defense;
		const float DAMAGE = ((52 * POWER * (ATTACK / DEFENSE) / 50) + 2) * MODIFIER;

		bool fainted = other.take_damage(DAMAGE);
		util::print("Player " + std::to_string(player_num) + "'s " + other.nick + " Has " + std::to_string((other.s_hp / other.max_hp) * 100) + "% Of It's HP Left");
		return fainted;
	}
	bool operator==(const pokemon& other) const {
		return (name == other.name) &&
			(move1 == other.move1) &&
			(move2 == other.move2) &&
			(move3 == other.move3) &&
			(move4 == other.move4);
	}
};
