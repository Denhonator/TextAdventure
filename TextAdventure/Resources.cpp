#include "Resources.h"

std::vector<Unit> Resources::units;
std::vector<Item> Resources::items;

void Resources::LoadUnits()
{
	std::ifstream file;
	Unit u;
	Stats st;
	std::string it;
	unsigned int count;
	file.open("Resources/units.txt");
	if (file) {
		while (file >> u.name >> st.hp >> st.mp >> st.strength >> st.magic >> st.agility >> st.defense) {
			while (file >> it >> count) {
				if(GetItem(it)!=nullptr)
					u.inventory[it] = count;
				else if (GetItem("", it) != nullptr)
					u.inventory[GetItem("", it)->name] = count;
			}
			u.stats = st;
			units.push_back(u);
		}
	}
	else
		std::cout << "Failed to read Resources/units.txt" << std::endl;
}

void Resources::LoadItems()
{
	std::ifstream file;
	Item i;
	file.open("Resources/items.txt");
	if (file) {
		while (file >> i.name >> i.fluff >> i.type >> i.rarity >> i.value >> i.stats.physical >> i.stats.fire >> i.stats.ice >> i.stats.lightning >> i.stats.neutral) {
			items.push_back(i);
		}
	}
	else
		std::cout << "Failed to read Resources/items.txt" << std::endl;
}

Item* Resources::GetItem(std::string name, std::string rarity, std::string type, bool randomize)
{
	if (name.size() < 2)
		name = "";
	if (rarity.size() < 2)
		rarity = "";
	if (type.size() < 2)
		type = "";
	std::vector<Item*> viable;
	for (unsigned int i = 0; i < items.size(); i++) {
		if (((name == "" && randomize) || items.at(i).name == name) && (rarity == "" || items.at(i).rarity == rarity) && (type == "" || items.at(i).type == type)) {
			viable.push_back(&items.at(i));
		}
	}
	if (viable.size() > 0) {
		return viable.at(rand() % viable.size());
	}
	return nullptr;
}

Unit Resources::GetUnit(std::string name)
{
	for (unsigned int i = 0; i < units.size(); i++) {
		if (units.at(i).name == name) {
			return units.at(i);
		}
	}
	return Unit();
}

Encounter Resources::GetEncounter(std::string type)
{
	Encounter enc;
	enc.objective = type;
	if (type == "enemy") {
		enc.stats.hp = rand() % 20 + 1;
		enc.stats.mp = rand() % (20 - enc.stats.hp) + 1;
		enc.stats.strength = rand() % (20 - enc.stats.mp) + 1;
		enc.stats.magic = rand() % (20 - enc.stats.strength) + 1;
		enc.stats.agility = rand() % (20 - enc.stats.magic) + 1;
		enc.stats.defense = rand() % (20 - enc.stats.agility) + 1;
		if (enc.stats.strength > enc.stats.magic) {
			enc.attack.physical = 1;
		}
		else {
			switch (rand() % 3) {
			case 0:
				enc.attack.fire = 1;
				enc.resist.fire = 0;
				break;
			case 1:
				enc.attack.ice = 1;
				enc.resist.ice = 0;
				break;
			case 2:
				enc.attack.lightning = 1;
				enc.resist.lightning = 0;
				break;
			}
		}
	}
	return enc;
}

Unit Resources::CreatePlayer(std::string name) {
	Unit u;
	u.name = name;
	u.stats.hp = 20;
	u.stats.mp = 20;
	u.stats.strength = 20;
	u.stats.magic = 20;
	u.stats.agility = 20;
	u.stats.defense = 20;
	return u;
}