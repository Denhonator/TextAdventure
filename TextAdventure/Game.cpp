#include "Game.h"

Area CreateArea(std::string name);
Unit CreatePlayer(std::string name);
std::string PrintUnit(Unit* unit);

Game::Game()
{
	areas.push_back(CreateArea("test"));
	AddPlayer("player0");
}

Game::~Game()
{
}

std::string Game::ProcessCommand(std::string input, int player)
{
	curPlayer = player;
	while (input[input.size() - 1] == ' ')
		input = input.substr(0, input.size() - 1);
	std::string arg = "";
	if (input.find(" ") != std::string::npos) {
		arg = input.substr(input.find(" ")+1);
		input = input.substr(0, input.find(" "));
	}
	if (input == "player")
		return PrintUnit(&units.at(curPlayer));
	if (input == "look") {
		return LookTile(arg);
	}
	if (input == "move") {
		if(Move(arg, curPlayer))
			return LookTile();
		return LookTile(arg);
	}
	return "Invalid command";
}

void Game::AddPlayer(std::string name, Location loc)
{
	units.insert(units.begin()+players,CreatePlayer(name));
	AddUnitTo(loc, players);
	players++;
}

bool Game::AddUnitTo(Location loc, unsigned int index)
{
	if (loc.x >= 0 && loc.x < 10 && loc.y >= 0 && loc.y < 10) {
		if (loc.area == nullptr)
			loc.area = &areas.at(0);
		loc.tile = &loc.area->tiles[loc.x + loc.y * 10];
		loc.tile->units.push_back(index);
		units.at(index).loc = loc;
		if (units.at(index).type == 'p')
			loc.tile->seen = true;
		return true;
	}
	return false;
}

bool Game::RemoveUnitFrom(Tile* tile, unsigned int index)
{
	tile->units.erase(std::find(tile->units.begin(), tile->units.end(), index));
	return true;
}

std::string Game::LookTile(std::string arguments) {
	if (arguments == "") {
		return PrintTile(units.at(curPlayer).loc.tile);
	}
	Location l = units.at(curPlayer).loc;
	bool validArg = false;
	if (arguments == "east") {
		validArg = true;
		l.x += 1;
	}
	else if (arguments == "west") {
		validArg = true;
		l.x -= 1;
	}
	else if (arguments == "north") {
		validArg = true;
		l.y -= 1;
	}
	else if (arguments == "south") {
		validArg = true;
		l.y += 1;
	}
	if (!validArg)
		return "Invalid argument";
	if(l.x>=0&&l.x<10&&l.y>=0&&l.y<10)
		return PrintTile(&l.area->tiles[l.x + l.y * 10]);
	return PrintTile(&l.area->defaultTile);
}

bool Game::Move(std::string arguments, unsigned int index)
{
	Location l = units.at(index).loc;
	bool validArg = false;
	if (arguments == "east") {
		validArg = true;
		l.x += 1;
	}
	else if (arguments == "west") {
		validArg = true;
		l.x -= 1;
	}
	else if (arguments == "north") {
		validArg = true;
		l.y -= 1;
	}
	else if (arguments == "south") {
		validArg = true;
		l.y += 1;
	}
	if (!validArg)
		return "Invalid argument";
	if (l.x >= 0 && l.x < 10 && l.y >= 0 && l.y < 10) {
		if (RemoveUnitFrom(units.at(index).loc.tile, index))
			return AddUnitTo(l, index);
	}
	return false;
}

Area CreateArea(std::string name) {
	Area a;
	a.name = name;
	for (unsigned int i = 0; i < AreaSize; i++) {
		a.tiles[i].fluff = "Nothing noteworthy to see.";
		a.tiles[i].seen = false;
		a.tiles[i].walkable = true;
	}
	a.defaultTile.fluff = "This is a default tile, which surround the area.";
	a.defaultTile.seen = true;
	a.defaultTile.walkable = false;
	return a;
}

Unit CreatePlayer(std::string name) {
	Unit u;
	u.name = name;
	u.max.hp = 20;
	u.max.mp = 20;
	u.max.strength = 20;
	u.max.magic = 20;
	u.max.agility = 20;
	u.max.defense = 20;
	u.cur = u.max;
	u.type = 'p';
	return u;
}

std::string PrintUnit(Unit* unit) {
	return "Name: " + unit->name + "\n"
		+ std::to_string(unit->cur.hp) + "/" + std::to_string(unit->max.hp) + " HP"
		+ "      Strength: " + std::to_string(unit->cur.strength) + "\n"
		+ std::to_string(unit->cur.mp) + "/" + std::to_string(unit->max.mp) + " MP"
		+ "      Magic:    " + std::to_string(unit->cur.magic) + "\n"
		+ "              Defense:  " + std::to_string(unit->cur.defense) + "\n"
		+ "              Agility:  " + std::to_string(unit->cur.agility);
}

std::string Game::PrintTile(Tile* tile) {
	if (tile == nullptr)
		return "You can not go this way";
	std::string s = "";
	if (tile->fluff.size() > 1) {
		s = tile->fluff;
	}
	if (tile->seen) {
		for (unsigned int i = 0; i < tile->units.size(); i++) {
			if (tile->units.size() == 1 && tile->units.at(0) == curPlayer)
				break;
			if (i == 0) {
				s += "\nYou see these characters: ";
			}
			if(tile->units.at(i)!=curPlayer)
				s += "\n" + units.at(tile->units.at(i)).name;
		}
		for (unsigned int i = 0; i < tile->items.size(); i++) {
			if (i == 0) {
				s += "\nYou see these items: ";
			}
			s += "\n" + tile->items.at(i).name;
		}
	}
	else if (tile->walkable) {
		s += "\nYou do not know what you will find here.";
	}
	if(!tile->walkable) {
		s += "\nYou can not go this way.";
	}
	return s;
}

std::string FindWord(std::string s, unsigned int index, unsigned int finalIndex = 0) {
	std::string buf = "";
	bool atWord = false;
	finalIndex = finalIndex > index ? finalIndex : index;
	unsigned curWord = 0;
	for (unsigned int i = 0; i < s.size(); i++) {
		if (s[i] == ' ' && atWord) {
			atWord = false;
			if (curWord == finalIndex)
				return buf;
			curWord += 1;
			continue;
		}
		else if (curWord >= index && curWord <= finalIndex) {
			atWord = true;
			buf += s[i];
		}
	}
	return buf;
}

int FindInt(std::string s, unsigned int index) {
	return std::stoi(FindWord(s, index));
}

std::string StatsToString(Stats s) {
	return tstr(s.hp) + " " + tstr(s.mp) + " " + tstr(s.strength) + " " + tstr(s.magic) + " " + tstr(s.agility) + " " + tstr(s.defense);
}
Stats StringToStats(std::string s) {
	Stats st;
	st.hp = FindInt(s, 0);
	st.mp = FindInt(s, 1);
	st.strength = FindInt(s, 2);
	st.magic = FindInt(s, 3);
	st.agility = FindInt(s, 4);
	st.defense = FindInt(s, 5);
	return st;
}

std::string ItemToString(Item i) {
	std::replace(i.name.begin(), i.name.end(), ' ', '_');
	return i.name + " " + i.equipType + " " + StatsToString(i.use) + " " + StatsToString(i.equip) + " " + tstr(i.value);
}
Item StringToItem(std::string s) {
	Item it;
	it.name = FindWord(s, 0);
	it.equipType = FindWord(s, 1);
	it.use = StringToStats(FindWord(s, 2, 2 + StatIndex));
	it.equip = StringToStats(FindWord(s, 3 + StatIndex, 3 + StatIndex * 2));
	return it;
}
std::string TileToString(Tile t) {
	unsigned int objCount = 5;
	std::replace(t.fluff.begin(), t.fluff.end(), ' ', '_');
	std::string buf = t.fluff + " " + tstr(t.items.size()) + " ";
	for (unsigned int i = 0; i < t.items.size(); i++) {
		buf += ItemToString(t.items.at(i)) + " ";
		objCount++;
	}
	buf += tstr(t.units.size()) + " ";
	for (unsigned int i = 0; i < t.units.size(); i++) {
		buf += tstr(t.units.at(i)) + " ";
		objCount++;
	}
	buf += tstr(t.seen) + " " + tstr(t.walkable);
	return tstr(objCount) + " " + buf;
}
Tile StringToTile(std::string s) {
	Tile ti;
	ti.fluff = FindWord(s, 1);
	int itemCount = FindInt(s, 2);
	for (unsigned int i = 0; i < itemCount; i++) {
		ti.items.push_back(StringToItem(FindWord(s, (3 + i) + ItemIndex * i, (3 + i) + ItemIndex * (i + 1))));
	}
	int unitCount = FindInt(s, 3 + itemCount * (ItemIndex + 1));
	for (unsigned int i = 0; i < unitCount; i++) {
		ti.units.push_back(FindInt(s, 4 + i + itemCount * (ItemIndex + 1)));
	}
	ti.seen = FindInt(s, 4 + unitCount + itemCount * (ItemIndex + 1));
	ti.walkable = FindInt(s, 5 + unitCount + itemCount * (ItemIndex + 1));
	return ti;
}
std::string AreaToString(Area a) {
	std::replace(a.name.begin(), a.name.end(), ' ', '_');
	std::string buf = a.name + " ";
	for (unsigned int i = 0; i < AreaSize; i++) {
		buf += TileToString(a.tiles[i]) += " ";
	}
	buf += TileToString(a.defaultTile);
	return buf;
}
Area StringToArea(std::string s) {
	Area ar;
	ar.name = FindWord(s, 0);
	unsigned int curWord = 1;
	unsigned int objCount = 5;
	for (unsigned int i = 0; i < AreaSize; i++) {
		objCount = FindInt(s, curWord);
		ar.tiles[i] = StringToTile(FindWord(s, curWord, curWord + objCount - 1));
		curWord += objCount + 1;
	}
	ar.defaultTile = StringToTile(FindWord(s, curWord, curWord + FindInt(s, curWord) - 1));
	return ar;
}

std::string LocToString(Location l) {
	return tstr(l.x) + " " + tstr(l.y);
}
Location StringToLoc(std::string s, Area* a) {
	Location lo;
	lo.x = FindInt(s, 0);
	lo.y = FindInt(s, 1);
	lo.area = a;
	lo.tile = &a->tiles[lo.x + lo.y * 10];
	return lo;
}

std::string UnitToString(Unit u) {
	std::replace(u.name.begin(), u.name.end(), ' ', '_');
	std::string buf = u.name + " " + StatsToString(u.max) + " " + StatsToString(u.cur) + " " + tstr(u.inventory.size()) + " ";
	for (unsigned int i = 0; i < u.inventory.size(); i++) {
		buf += ItemToString(u.inventory.at(i)) + " ";
	}
	buf += LocToString(u.loc) + " " + u.type;
	return buf;
}
Unit StringToUnit(std::string s, Area* a) {
	Unit un;
	un.name = FindWord(s, 0);
	un.max = StringToStats(FindWord(s, 1, 1 + StatIndex));
	un.cur = StringToStats(FindWord(s, 2 + StatIndex, 2 + StatIndex * 2));
	unsigned int curWord = 3 + StatIndex * 2;
	int itemCount = FindInt(s, curWord);
	curWord++;
	for (unsigned int i = 0; i < itemCount; i++) {
		un.inventory.push_back(StringToItem(FindWord(s, curWord, curWord + ItemIndex)));
		curWord += ItemIndex + 1;
	}
	un.loc = StringToLoc(FindWord(s, curWord, curWord + 1), a);
	curWord += 2;
	un.type = FindWord(s, curWord)[0];
	return un;
}