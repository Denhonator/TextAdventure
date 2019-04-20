#pragma once
#include "Util.h"

class Resources {
public:
	static std::vector<Unit> units;
	static std::vector<Area> areas;
	static std::vector<Tile> tiles;
	static std::vector<Item> items;
	static void LoadUnits();
	static void LoadAreas();
	static void LoadTiles();
	static void LoadItems();
private:
	static Unit CreatePlayer(std::string name);
	static Area CreateArea(std::string name);
};
/*
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
		buf += tstr(t.items.at(i)) + " ";
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
	unsigned int curWord = 3;
	for (unsigned int i = 0; i < itemCount; i++) {
		ti.items.push_back(FindInt(s, curWord));
		curWord++;
	}
	int unitCount = FindInt(s, curWord);
	curWord++;
	for (unsigned int i = 0; i < unitCount; i++) {
		ti.units.push_back(FindInt(s, curWord));
		curWord++;
	}
	ti.seen = FindInt(s, curWord);
	curWord++;
	ti.walkable = FindInt(s, curWord);
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
Location StringToLoc(std::string s, Area * a) {
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
Unit StringToUnit(std::string s, Area * a) {
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
}*/