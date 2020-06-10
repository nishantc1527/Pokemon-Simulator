#include <iostream>
#include <fstream>
#include <sstream>
#include "helper_functions.hpp"
#include "gameplay_functions.hpp"
#include "helper_constants.hpp"

int main() {
	read_file();
	try {
	  start();
	} catch(const std::exception& e) {
		print("Quitting");
	}
}

[[noreturn]] void start() {
	while (true) {
		switch (get_input("Do You Want To Create A Team Or Battle Using Existing Teams?", false, {
			"Create Team",
			"Battle"
			})) {
		case CREATE_TEAM:
			create_team();
			break;
		case BATTLE:
			battle();
			break;
		}
	}
}

void create_team() {
	std::vector<std::string> u_team;
	while (u_team.size() < 6) {
		switch (get_input("Do You Want To Create A New Pokemon Or Add An Existing Pokemon To Your Team?", false, {
			"Create Pokemon",
			"Add Pokemon"
			})) {
		case CREATE_POKEMON:
			create_pokemon();
			break;
		case ADD_POKEMON:
			add_pokemon(u_team);
			break;
		}
	}

	print("What Do You Want To Name Your Team?");
	std::string team_name;
	std::getline(std::cin, team_name);
	add_team_to_file(team_name, u_team);
	u_teams[team_name] = u_team;
}

void battle() {
	std::vector<std::pair<std::string, std::vector<std::string>>> key_value = key_value_pairs(u_teams);
	std::vector<std::string> keys;

	for (auto& pair : key_value) {
		keys.push_back(pair.first);
	}

	int in = get_input("Player 1: Which Team Do You Want To Use?", false, keys);
	if (in == -1) {
		print("You Have No Teams To Battle With.");
		return;
	}
	std::string& p1_team_name = keys[in - 1];

	in = get_input("Player 2: Which Team Do You Want To Use?", false, keys);
	std::string& p2_team_name = keys[in - 1];

	std::vector<pokemon> p1_team;
	std::vector<std::string>& p1_team_pokemon = u_teams[p1_team_name];
	for (int i = 0; i < 6; i++) {
		p1_team.emplace_back(pokemon(u_pokemon[p1_team_pokemon[i]][0], p1_team_pokemon[i],
			u_pokemon[p1_team_pokemon[i]][1],
			u_pokemon[p1_team_pokemon[i]][2],
			u_pokemon[p1_team_pokemon[i]][3],
			u_pokemon[p1_team_pokemon[i]][4]));
	}

	std::vector<pokemon> p2_team;
	std::vector<std::string> p2_team_pokemon = u_teams[p2_team_name];
	for (int i = 0; i < 6; i++) {
		p2_team.emplace_back(pokemon(u_pokemon[p2_team_pokemon[i]][0], p2_team_pokemon[i],
			u_pokemon[p2_team_pokemon[i]][1],
			u_pokemon[p2_team_pokemon[i]][2],
			u_pokemon[p2_team_pokemon[i]][3],
			u_pokemon[p2_team_pokemon[i]][4]));
	}

	in = get_input("Player 1: Choose Your Lead", false, to_string_array(p1_team));
	pokemon& p1_lead = p1_team[in - 1];
	
	in = get_input("Player 2: Choose Your Lead", false, to_string_array(p2_team));
	pokemon& p2_lead = p2_team[in - 1];

	battle(p1_team, p2_team, p1_lead, p2_lead);
}

void create_pokemon() {
	std::string input_pokemon;
	std::vector<std::string> values;
	std::vector<std::pair<int, std::string>> key_value = key_value_pairs(available_pokemon);
	for (auto & i : key_value) {
		values.push_back(i.second + "\n" + pokemon_image(i.second));
	}
	int in = get_input("Which Pokemon Do You Want To Add?", true, values);
	add_user_pokemon(key_value[in - 1].second);
}

void add_pokemon(std::vector<std::string>& u_team) {
	std::vector<std::pair<std::string, std::vector<std::string>>> key_value = key_value_pairs(u_pokemon);
	std::vector<std::string> keys;

	for (auto& pair : key_value) {
		keys.push_back(pair.first);
	}

	if (keys.empty()) {
		print("You Have No Pokemon");
		return;
	}

	int in = get_input("Which Pokemon Do You Want To Add?", true, keys);
	u_team.push_back(keys[in - 1]);
}

void battle(std::vector<pokemon>& p1_team, std::vector<pokemon>& p2_team, pokemon& p1_lead, pokemon& p2_lead) {
	pokemon& curr_p1 = p1_lead;
	pokemon& curr_p2 = p2_lead;

	while (true) {
		std::ifstream file1(curr_p1.name + ".txt");
		std::ifstream file2(curr_p2.name + ".txt");

		std::string next_line;

		while(std::getline(file1, next_line)) {
			print("                                   " + next_line);
		}

		print("\n\n\n\n\n\n\n\n\n");

		while(std::getline(file2, next_line)) {
			print(next_line);
		}

		switch (get_input("Player 1: Do You Want To Attack Or Swtich?", false,
			{
				"Attack",
				"Switch"
			}
		)) {
		case ATTACK:
			switch (get_input("Player 2: Do You Want To Attack Or Swtich?", false,
				{
					"Attack",
					"Switch"
				}
			)) {
			case ATTACK:
			{
				std::string p1_move = choose_move(curr_p1, 1);
				std::string p2_move = choose_move(curr_p2, 2);

				if (curr_p1.s_speed > curr_p2.s_speed) {
					if (curr_p1.attack(p1_move, curr_p2, 2)) {
						remove(p2_team, curr_p2);
						int in = get_input(curr_p2.name + " Has Fainted. Player 2: Which Pokemon Do You Want To Switch Out To?", false, to_string_array(p2_team));
						if (in == -1) {
							print("Player 1 Has Won!");
							return;
						}
						curr_p2 = p2_team[in - 1];
					}
					else {
						if (curr_p2.attack(p2_move, curr_p1, 1)) {
							remove(p1_team, curr_p1);
							int in = get_input(curr_p1.name + " Has Fainted. Player 1: Which Pokemon Do You Want To Switch Out To?", false, to_string_array(p1_team));
							if (in == -1) {
								print("Player 2 Has Won!");
								return;
							}
							curr_p1 = p1_team[in - 1];
						}
					}
				}
				else {
					if (curr_p2.attack(p2_move, curr_p1, 1)) {
						remove(p1_team, curr_p1);
						int in = get_input(curr_p1.name + " Has Fainted. Player 1: Which Pokemon Do You Want To Switch Out To?", false, to_string_array(p1_team));
						if (in == -1) {
							print("Player 2 Has Won!");
							return;
						}
						curr_p1 = p1_team[in - 1];
					}
					else {
						if (curr_p1.attack(p1_move, curr_p2, 2)) {
							remove(p2_team, curr_p2);
							int in = get_input(curr_p2.nick + " Has Fainted. Player 2: Which Pokemon Do You Want To Switch Out To?", false, to_string_array(p2_team));
							if (in == -1) {
								print("Player 1 Has Won!");
								return;
							}
							curr_p2 = p2_team[in - 1];
						}
					}
				}
			}
				break;

			case SWITCH:
				std::vector<pokemon> removed = temp_remove(p2_team, curr_p2);
				int in = get_input("Player 2: Which Pokemon Do You Want To Switch Out To?", false, to_string_array(removed));
				curr_p2 = p2_team[in - 1];

				std::string p1_move = choose_move(curr_p1, 1);

				if (curr_p1.attack(p1_move, curr_p2, 2)) {
					remove(p2_team, curr_p2);
					in = get_input(curr_p2.nick + " Has Fainted. Player 2: Which Pokemon Do You Want To Switch Out To?", false, to_string_array(p2_team));
					if (in == -1) {
						print("Player 1 Has Won!");
						return;
					}
					curr_p2 = p1_team[in - 1];
				}
			}

			break;
		case SWITCH:
			switch (get_input("Player 2: Do You Want To Attack Or Swtich?", false,
				{
					"Attack",
					"Switch"
				}
			)) {
			case ATTACK:
			{
				std::vector<pokemon> removed = temp_remove(p1_team, curr_p1);
				int in = get_input("Player 1: Which Pokemon Do You Want To Switch Out To?", false, to_string_array(removed));
				curr_p1 = p1_team[in - 1];

				std::string p2_move = choose_move(curr_p2, 2);

				if (curr_p2.attack(p2_move, curr_p1, 1)) {
					remove(p1_team, curr_p1);
					in = get_input(curr_p1.name + " Has Fainted. Player 1: Which Pokemon Do You Want To Switch Out To?", false, to_string_array(p1_team));
					if (in == -1) {
						print("Player 2 Has Won!");
						return;
					}
					curr_p1 = p1_team[in - 1];
				}
			}
				
				break;
			case SWITCH:
				std::vector<pokemon> removed = temp_remove(p1_team, curr_p1);
				int in = get_input("Player 1: Which Pokemon Do You Want To Switch Out To?", false, to_string_array(removed));
				curr_p1 = p1_team[in - 1];

				removed = temp_remove(p2_team, curr_p2);
				in = get_input("Player 2: Which Pokemon Do You Want To Switch Out To?", false, to_string_array(removed));
				curr_p2 = p2_team[in - 1];
			}

			break;
		}
	}
}

////////////////Helper Functions///////////////////

int get_input(const std::string& prompt, bool fast, const std::vector<std::string>& options) {
	if (options.empty()) {
		return -1;
	}

	print(prompt + " If you want to exit, type -2.");
	for (int i = 0, option_count = 1; i < options.size(); i++, option_count++) {
		print(std::to_string(option_count) + ": " + options[i]);
	}
	while (true) {
		std::string input;
		std::getline(std::cin, input);

		if (!is_valid(input, options.size())) {
			print("INVALID");
		} else {
			int num = std::stoi(input);

			if(num == -2) {
				throw std::exception();
			} else {
				return std::stoi(input);
			}
		}
	}
}

void print(const std::string& text) {
	for (char i : text) {
		std::cout << i;
	}

	std::cout << std::endl;
}

void read_file() {
	std::ifstream data("pokemon_data.txt");
	std::string next_line;
	std::string prev;
	int count = 1;

	while (std::getline(data, next_line)) {
		if (next_line[0] == '#') {
			std::string p_name = split(next_line, 1);
			pokemon_data[p_name] = next_line;
			available_pokemon[count++] = p_name;
			prev = p_name;
			continue;
		}

		pokemon_moves[prev].push_back(next_line);
	}

	data.close();

	std::ifstream u_pokemon_file("user_pokemon.txt");

	while (std::getline(u_pokemon_file, next_line)) {
		std::vector<std::string> u_pokemon_data;
		prev = next_line;

		for (int i = 0; i < 13; i++) {
			std::getline(u_pokemon_file, next_line);
			u_pokemon_data.push_back(next_line);
		}

		u_pokemon[prev] = u_pokemon_data;
	}

	u_pokemon_file.close();

	std::ifstream u_teams_file("user_teams.txt");

	while (std::getline(u_teams_file, next_line)) {
		std::vector<std::string> team_data;
		prev = next_line;

		for (int i = 0; i < 6; i++) {
			std::getline(u_teams_file, next_line);
			team_data.push_back(next_line);
		}

		u_teams[prev] = team_data;
	}

	std::ifstream move_file("move_data.txt");

	while (std::getline(move_file, next_line)) {
		std::vector<std::string> curr_move_data;
		prev = next_line;

		for (int i = 0; i < 5; i++) {
			std::getline(move_file, next_line);
			curr_move_data.push_back(next_line);
		}

		move_data[prev] = curr_move_data;
	}
}

std::vector<std::string> split(std::string& string) {
	std::vector<std::string> ans;
	std::stringstream stream(string);
	std::string next_line;

	while (std::getline(stream, next_line, ' ')) {
		ans.push_back(next_line);
	}

	return ans;
}

std::string split(std::string& string, int index) {
	return split(string)[index];
}

template<typename K, typename V>
std::vector<std::pair<K, V>> key_value_pairs(std::unordered_map<K, V>& map) {
	std::vector<std::pair<K, V>> ans;

	for (auto& x : map) {
		ans.push_back(std::make_pair(x.first, x.second));
	}

	return ans;
}

void add_user_pokemon(std::string& p_name) {
	std::vector<std::string> u_moves = get_user_moves(p_name);
	print("What Do You Want To Name Your Pokemon? Name Must Be Unique");
	std::string name;
	std::getline(std::cin, name);
	std::ofstream file("user_pokemon.txt", std::ios_base::app);
	std::vector<std::string> data = split(pokemon_data[p_name]);
	file << name << "\n" <<
		p_name << "\n" <<
		u_moves[0] << "\n" <<
		u_moves[1] << "\n" <<
		u_moves[2] << "\n" <<
		u_moves[3] << "\n" <<
		data[2] << "\n" <<
		data[3] << "\n" <<
		data[4] << "\n" <<
		data[5] << "\n" <<
		data[6] << "\n" <<
		data[7] << "\n" <<
		data[8] << "\n" <<
		data[9] << "\n";
	file.close();
	u_pokemon[name] = {
		p_name,
		u_moves[0],
		u_moves[1],
		u_moves[2],
		u_moves[3],
		data[2],
		data[3],
		data[4],
		data[5],
		data[6],
		data[7],
		data[8],
		data[9]
	};
}

std::vector<std::string> get_user_moves(std::string& p_name) {
	std::vector<std::string> available_moves = pokemon_moves[p_name];
	std::vector<std::string> u_moves;
	int move_count = 5;
	while (move_count-- > 1) {
		int in = get_input("Choose " + std::to_string(move_count) + " Moves", true, available_moves);
		u_moves.push_back(available_moves[in - 1]);
		available_moves.erase(available_moves.begin() + in - 1);
	}

	return u_moves;
}

void add_team_to_file(std::string& team_name, std::vector<std::string>& p_names) {
	std::ofstream team_file("user_teams.txt", std::ios_base::app);
	team_file << team_name << "\n" <<
		p_names[0] << "\n" <<
		p_names[1] << "\n" <<
		p_names[2] << "\n" <<
		p_names[3] << "\n" <<
		p_names[4] << "\n" <<
		p_names[5] << "\n";
	team_file.close();
}

template<typename T>
void remove(std::vector<T>& vec, T& element) {
	for (int i = 0; i < vec.size(); i++) {
		if (vec[i] == element) {
			vec.erase(vec.begin() + i);
			return;
		}
	}
}

std::vector<std::string> to_string_array(const std::vector<pokemon>& vec) {
	std::vector<std::string> ans;

	for (const auto & i : vec) {
		ans.push_back(i.nick);
	}

	return ans;
}

std::string choose_move(pokemon& p, int player_num) {
	std::string move;

	switch (get_input("Player " + std::to_string(player_num)  + ": Which Move Do You Want To Use?", false,
		{
			p.move1,
			p.move2,
			p.move3,
			p.move4
		}
	)) {
	case MOVE_1:
		move = p.move1;
		break;
	case MOVE_2:
		move = p.move2;
		break;
	case MOVE_3:
		move = p.move3;
		break;
	case MOVE_4:
		move = p.move4;
		break;
	}

	return move;
}

template<typename T>
std::vector<T> temp_remove(std::vector<T> vec, T& to_remove) {
	remove(vec, to_remove);
	return vec;
}

bool is_valid(const std::string& to_check, int max_num) {
	try {
		int num = std::stoi(to_check);
		return num == -2 || (num >= 1 && num <= max_num);
	} catch (std::exception& e) {
		return false;
	}
}

std::string pokemon_image(const std::string& p_name) {
	std::ifstream file(p_name + ".txt");
	std::string next_line;
	std::stringstream ss;
	
	while(std::getline(file, next_line)) {
		ss << next_line << std::endl;
	}

	return ss.str();	
}
