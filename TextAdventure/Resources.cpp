#include "Resources.h"
#define StatStream st.hp>>st.mp>>st.strength>>st.magic>>st.agility>>st.defense
#define EquipmentStream u.equipment[0] >> u.equipment[1] >> u.equipment[2] >> u.equipment[3] >> u.equipment[4] >> u.equipment[5]
#define InventoryStream u.inventory[0].first >> u.inventory[0].second >> u.inventory[1].first >> u.inventory[1].second >> u.inventory[2].first >> u.inventory[2].second \
>> u.inventory[3].first >> u.inventory[3].second >> u.inventory[4].first >> u.inventory[4].second >> u.inventory[5].first >> u.inventory[5].second

std::vector<Unit> Resources::units;
std::vector<Area> Resources::areas;
std::vector<Tile> Resources::tiles;
std::vector<Item> Resources::items;

void Resources::LoadUnits()
{
	std::ifstream file;
	Unit u;
	Stats st;
	file.open("Resources/units.txt");
	if (file) {
		while (file >> u.name >> StatStream >> u.loc.x >> u.loc.y >> u.loc.area >> EquipmentStream >> InventoryStream) {
			for (unsigned int i = 0; i < sizeof(u.equipment) / sizeof(u.equipment[0]); i++) {
				if (u.equipment[i].size() == 1)
					u.equipment[i] == "";
				if (u.equipment[i].size()>1 && GetItem(u.equipment[i],"","equipment") == nullptr) {
					Item* item = GetItem("", u.equipment[i], "equipment");
					if (item != nullptr)
						u.equipment[i] = item->name;
				}
			}
			for (unsigned int i = 0; i < sizeof(u.inventory) / sizeof(u.inventory[0]); i++) {
				if (u.inventory[i].first > 0 && GetItem(u.inventory[i].second) == nullptr) {
					Item* item = GetItem("", u.inventory[i].second, "use");
					if (item != nullptr)
						u.inventory[i].second = item->name;
				}
			}
			u.stats = st;
			units.push_back(u);
		}
	}
	else
		std::cout << "Failed to read Resources/units.txt" << std::endl;
}

void Resources::LoadAreas()
{
	areas.push_back(CreateArea("test"));
}

void Resources::LoadTiles()
{
	std::ifstream file;
	Tile t;
	file.open("Resources/tiles.txt");
	if (file) {
		while (file >> t.fluff >> t.walkable) {
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
	Stats st, st2;
	file.open("Resources/items.txt");
	if (file) {
		while (file >> i.name >> i.fluff >> i.type >> i.rarity >> StatStream >> i.value) {
			i.stats = st;
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

Area Resources::CreateArea(std::string name) {
	Area a;
	a.name = name;
	for (unsigned int i = 0; i < AreaSize; i++) {
		a.tiles[i] = 1;
		a.seenTiles[i] = false;
	}
	a.defaultTile = 0;
	return a;
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