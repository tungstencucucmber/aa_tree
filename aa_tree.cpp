#include <iostream>
using namespace std;

struct vessel {
	string name; // a ship without a name goes nowhere 
	unsigned long long int id; // vessel unique number

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
				t->parent = ship->parent;
				t->parent->right = t;
			}
			else {
				t->parent = ship->parent;
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
	return ship;
}

vessel* split(vessel *ship) {
	if(ship->right != NULL) {
	if(ship->right->right != NULL) {
	if(ship->right->right->level == ship->level) {
		vessel *r = ship->right;

		if(ship->parent != NULL) {
			if (ship->parent->right == ship)
				ship->parent->right = r; // changing parents
			else ship->parent->left = r;
			r->parent = ship->parent;
		}

		ship->right = r->left; // child handover
		if (ship->right != NULL)
			ship->right->parent = ship;
		
		r->left = ship; // changing link from right to left
		ship->parent = r;

		r->level++; // lifting up the central node
		return r;
	}
	}
	}
	return ship;
}

vessel* create_vessel(string n, unsigned long long int i) {
	vessel *ship = new vessel;
	ship->name = n;
	ship->id = i;
	ship->parent = NULL;
	ship->left = NULL;
	ship->right = NULL;
	ship->level = 0;
	return ship;
}

// attach a vessel to a certain fleet (a node to a tree)
vessel* attach_to_fleet(vessel *flagship, vessel *ship) {
	if (ship->id <= flagship->id) {
		if (flagship->left == NULL) {
			flagship->left = ship;
			ship->parent = flagship;
			ship->level = 0;
		} else attach_to_fleet(flagship->left, ship);
	} else {
		if (flagship->right == NULL) {
			flagship->right = ship;
			ship->parent = flagship;
			ship->level = 0;
		} else attach_to_fleet(flagship->right, ship);
	}
	if (flagship != NULL) {
		flagship = skew(flagship);
	}
	if (flagship != NULL) {
		flagship = split(flagship);
	}
	return flagship;
}

vessel* find_vessel(vessel *flagship, unsigned long long int target_id) {
	if (flagship == NULL)
		return NULL;
	else if (target_id < flagship->id)
		find_vessel(flagship->left, target_id);
	else if (target_id > flagship->id)
		find_vessel(flagship->right, target_id);
	else return flagship;
}

vessel* detach_from_fleet(vessel *flagship, unsigned long long int target_id) {
	if (find_vessel(flagship, target_id) != NULL) {
		vessel *deleted = find_vessel(flagship, target_id);
		if (deleted->left == NULL) {
			if (deleted->parent != NULL)
				deleted->parent->right = NULL;
			delete deleted;
		} else {
			vessel *replaced = deleted->left;
			while (replaced->right != NULL)
				replaced = replaced->right;
			vessel *t = replaced->parent;

			replaced->parent->right = NULL;
			replaced->parent = deleted->parent;
			if (replaced->parent != NULL)
				replaced->parent->right = replaced;
			replaced->left = deleted->left;
			replaced->right = deleted->right;
			delete deleted;
			while(t != NULL) {
				t = skew(t);
				t = split(t);
				t = t->parent;
			}
		}
	}
	return flagship;
}

void dismiss_fleet(vessel *flagship) {
	while (flagship->left != NULL)
		dismiss_fleet(flagship->left);
	while(flagship->right != NULL)
		dismiss_fleet(flagship->right);
	flagship = NULL;
	delete flagship;
}

void fleet_report(vessel *flagship) {
	if(flagship->left != NULL)
		fleet_report(flagship->left);
	cout << flagship->name << " report:" << endl;
	cout << "ID: " << flagship->id << endl;
	cout << "Level: " << flagship->level << endl;
	cout << endl;
	if(flagship->right != NULL)
		fleet_report(flagship->right);
}

int main() {
	vessel *normandy = create_vessel("SSV Normandy", 2);
	vessel *luna = create_vessel("NSV Luna", 9);
	vessel *cruiser = create_vessel("NMV Icarus", 5);
	vessel *strange_cruiser = create_vessel("Galactica", 7);
	vessel *smugglers = create_vessel("Millenium Falcon", 12);
	vessel *starman = create_vessel("Tesla Roadster", 14);
	vessel *my_fleet = normandy;
	my_fleet = attach_to_fleet(my_fleet, luna);
	my_fleet = attach_to_fleet(my_fleet, cruiser);
	my_fleet = attach_to_fleet(my_fleet, strange_cruiser);
	my_fleet = attach_to_fleet(my_fleet, smugglers);
	my_fleet = attach_to_fleet(my_fleet, starman);
	// my_fleet = detach_from_fleet(my_fleet, 9);
	fleet_report(my_fleet);
	// dismiss_fleet(my_fleet);
	return 0;
}