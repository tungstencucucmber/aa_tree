#include <iostream>
using namespace std;

struct vessel {
	string name; // a ship without a name goes nowhere 
	unsigned long long int id; // vessel unique number
	unsigned tonnage; // carrying capacity
	unsigned current_load; // weight of the cargo the ship is carrying at the moment
	short route; // vessels are always on route between some destinations
	short location; // obvious

	unsigned short level; // the technical
	vessel *parent; // stuff
	vessel *left; // for the tree
	vessel *right;
};

vessel* skew(vessel *ship) {
	if((ship->left != NULL) && (ship->left->level == ship->level)) {
		vessel *t = ship->left;

		if (ship->parent != NULL) {
			if (ship == ship->parent->right) { // changing parents
				t->parent = ship->parent->right;
				t->parent->right = t;
			}
			else {
				t->parent = ship->parent->left;
				t->parent->left = t;
			}
		}

		ship->left = t->right; // child handover
		if(ship->left != NULL)
			ship->left->parent = ship;

		t->right = ship; // changing link from left to right
		ship->parent = t;
		return t;
	}
}

vessel* split(vessel *ship) {
	if((ship->right != NULL) && (ship->right->right != NULL) && (ship->right->right->level == ship->level)) {
		vessel *r = ship->right;

		if(ship->parent != NULL) {
			ship->parent->right = r; // changing parents
			r->parent = ship->parent;
		}

		ship->right = r->left; // child handover
		if (ship->right != NULL)
			ship->right->parent = ship;
		
		r->left = ship; // changing link from right to left
		if(ship->parent != NULL) {
			ship->parent = r;
		}
		r->level++; // lifting up the central node
		cout << "Lift up!" << endl;
		return r;
	}
}

vessel* create_vessel(string n, unsigned long long int i, unsigned t, short loc) {
	vessel *ship = new vessel;
	ship->name = n;
	ship->id = i;
	ship->tonnage = t;
	ship->current_load = 0;
	ship->route = 0;
	ship->location = loc;
	ship->parent = NULL;
	ship->left = NULL;
	ship->right = NULL;
	ship->level = 0;
	return ship;
}

// attach a vessel to a certain fleet (a node to a tree)
void attach_to_fleet(vessel *flagship, vessel *ship) {
	if (ship->id <= flagship->id) {
		if (flagship->left == NULL) {
			flagship->left = ship;
			ship->parent = flagship;
			ship->level = flagship->level;
		} else attach_to_fleet(flagship->left, ship);
	} else {
		if (flagship->right == NULL) {
			flagship->right = ship;
			ship->parent = flagship;
			ship->level = flagship->level;
		} else {
			if (flagship->right->level == flagship->level)
				attach_to_fleet(flagship->right, ship);
			else attach_to_fleet(flagship->right, ship);
		}
		if (flagship != NULL) {
			flagship = skew(flagship);
			if(flagship->parent != NULL)
				flagship->parent = split(flagship->parent);
		}
	}
}

// add function overloadd for name, etc?
vessel* find_vessel(vessel *flagship, unsigned long long int target_id) {
	if(target_id < flagship->id)
		find_vessel(flagship->left, target_id);
	else if (target_id > flagship->id)
		find_vessel(flagship->right, target_id);
	else return flagship;
}

// print tree
void fleet_report(vessel *flagship) {
	if(flagship->left != NULL)
		fleet_report(flagship->left);
	cout << flagship->name << " report:" << endl;
	cout << "ID: " << flagship->id << endl;
	cout << "Level: " << flagship->level << endl;
	// cout << "Route: " << flagship->route << endl;
	// cout << "Location: " << flagship->location << endl;
	// cout << "Load: " << flagship->current_load << " t out of " << flagship->tonnage << " t"<< endl;
	cout << endl;
	if(flagship->right != NULL)
		fleet_report(flagship->right);
}

struct record { // the log entry written when ship arrives or leaves the station
	unsigned long long int id;
	short route;
	short event; // indicates the type of event - arrival (+n) or departure (-n) from exact location (n is inteeger != 0)
	float time; // obvious
	record *next; // pointer to the next record in the list
};

int main() {
	vessel *normandy = create_vessel("SSV Normandy", 1000003, 30, 2);
	vessel *luna = create_vessel("NSV Luna", 1000001, 25, 6);
	vessel *cruiser = create_vessel("NMV Icarus", 1000000, 60, 10);
	vessel *marines = create_vessel("USS Sulaco", 1000002, 60, 12);
	vessel *pirates = create_vessel("Pirates Shuttle", 1000004, 20, 4);
	attach_to_fleet(normandy, luna);
	attach_to_fleet(normandy, cruiser);
	attach_to_fleet(normandy, marines);
	attach_to_fleet(normandy, pirates);
	fleet_report(normandy);
	fleet_report(luna);
	return 0;
}