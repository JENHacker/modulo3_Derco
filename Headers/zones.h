#ifndef ZONES_H
#define ZONES_H

#include "distribution_centers.h"

struct ZoneList {
	int lenght;
	struct ZoneNode *tail;
	struct ZoneNode *zones;
};

struct ZoneNode {
	struct Zone *zone;
	struct ZoneNode *next;
};

struct Zone {
	char *name;
	int numberOfHabitants;
	float habitantsAverageAge;
	struct DistributionCenterNode *distributionCenters;
};

struct ZoneList* CreateZoneCircularList();
int AddZone(struct ZoneList*, struct Zone*);
int DeleteZone(struct ZoneList*, struct ZoneNode*);
struct ZoneNode* FindZoneByName(struct ZoneList*, char*);

int ChangeZoneName(struct ZoneNode*, char*);
int ChangeZonesNumberOfHabitants(struct ZoneNode*, int);
int ChangeZonesHabitantsAverageAge(struct ZoneNode*, float);

void PrintZoneList(struct ZoneList*);
void PrintZone(struct Zone*);
void PrintZoneNames(struct ZoneList*);

struct Zone* CreateZone(char*, int, float);
struct ZoneNode* CreateZoneNode(struct Zone*);

#endif // !ZONES_H