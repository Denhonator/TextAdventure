#include "Resources.h"

std::vector<Unit> Resources::units;
std::vector<Tile> Resources::tiles;
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
		if (file >> u.name >> st.hp >> st.mp >> st.strength >> st.magic >> st.agility >> st.defense) {
			while (file >> it >> count) {
				if(GetItem(it)!=nullptr)
					u.inventory[it] = count;
				else if (GetItem("", it) != nullptr)
					u.inventory[GetItem("", it)->name] = count;
			}
			u.type = 1;
			u.stats = st;
			units.push_back(u);
		}
	}
	else
		std::cout << "Failed to read Resources/units.txt" << std::endl;
}

void Resources::LoadTiles()
{
	std::ifstream file;
	Tile t;
	std::string u[6];
	file.open("Resources/tiles.txt");
	if (file) {
		if (file >> t.fluff >> t.type) {
			std::string unitName = "";
			while (file >> unitName) {
				t.units.push_back(GetUnit(unitName));
				t.units.back().type = t.type;
			}
			std::replace(t.fluff.begin(), t.fluff.end(), '_', ' ');
			tiles.push_back(t);
		}
	}
	else
		std::cout << "Failed to read Resources/tiles.txt" << std::endl;
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

Unit Resources::CreatePlayer(std::string name) {
	Unit u;
	u.name = name;
	u.stats.hp = 20;
	u.stats.mp = 20;
	u.stats.strength = 20;
	u.stats.magic = 20;
	u.stats.agility = 20;
	u.stats.defense = 20;
	u.type = 'p';
	return u;
}