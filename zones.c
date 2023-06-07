#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "zones.h"
#include "distribution_centers.h"

struct ZoneList* CreateZoneCircularList() {
	struct ZoneList* list;
	list = (struct ZoneList*)malloc(sizeof(struct ZoneList));

	if (!list) { return NULL; }

	list->zones = NULL;
	list->tail  = NULL;
	list->lenght = 0;

	return list;
}

int AddZone(struct ZoneList* zoneList, struct Zone* zone) {
	if (!zoneList || FindZoneByName(zoneList, zone->name)) { return 1; }
	
	struct ZoneNode *newNode;

	newNode = CreateZoneNode(zone);

	if (!newNode) { return 1; }

	if (!zoneList->zones) { 
		zoneList->zones = newNode; 
		zoneList->tail  = newNode;
	}
	newNode->next = zoneList->zones;
	zoneList->tail->next = newNode;
	
	zoneList->lenght++;
	zoneList->tail = newNode;

	return 0;
}

int DeleteZone(struct ZoneList* zoneList, struct ZoneNode* nodeToDelete) {
	if (!zoneList || !zoneList->zones) { return 1; }

	if (zoneList->zones == nodeToDelete) {

		if (zoneList->zones->next == zoneList->zones) { 
			zoneList->zones = NULL; 
			zoneList->tail  = NULL;
		} else { 
			zoneList->zones = zoneList->zones->next; 
			zoneList->tail->next = zoneList->zones;
		}
		zoneList->lenght--;
		return 0;
	}

	struct ZoneNode* traverse = zoneList->zones;

	do {
		if (traverse->next == nodeToDelete) {
			if (traverse->next == zoneList->tail) { zoneList->tail = traverse; }
			traverse->next = traverse->next->next;
			zoneList->lenght--;
			return 0;
		}
		traverse = traverse->next;
	} while (traverse->next != zoneList->zones);

	return 1;
}


struct ZoneNode* FindZoneByName(struct ZoneList* zoneList, char* name) {
	if (!zoneList || !zoneList->zones) { return NULL; }

	struct ZoneNode* traverse = zoneList->zones;

	do {
		if (!strcmp(traverse->zone->name, name)) { return traverse; }
		traverse = traverse->next;
	} while (traverse != zoneList->zones);

	return NULL;
}

int ChangeZoneName(struct ZoneNode* nodeToChange, char* name) {
	if (!nodeToChange) { return 1; }
	nodeToChange->zone->name = (char*)malloc(sizeof(char)*strlen(name));
	if (!nodeToChange->zone->name) { return 1; }
	strcpy(nodeToChange->zone->name, name);
	return 0;
}

int ChangeZonesNumberOfHabitants(struct ZoneNode* nodeToChange, int numberOfHabitants) {
	if (!nodeToChange || numberOfHabitants == -1) { return 1; }
	nodeToChange->zone->numberOfHabitants = numberOfHabitants;
	return 0;
}

int ChangeZonesHabitantsAverageAge(struct ZoneNode* nodeToChange, float habitantsAverageAge) {
	if (!nodeToChange || habitantsAverageAge == (float)(-1)) { return 1; }
	nodeToChange->zone->habitantsAverageAge = habitantsAverageAge;
	return 0;
}

void PrintZoneList(struct ZoneList* zoneList) {
	if (!zoneList || !zoneList->zones) { return; }

	struct ZoneNode* traverse = zoneList->zones;

	printf("---------------------------------------------------------\n\n");
	printf("longitud lista circular = %d\n\ncola de la lista:\n\nnombre = \"%s\"\nnumero de habitantes = %d\nedad promedio de los habitantes %.2f\n\n", zoneList->lenght, zoneList->tail->zone->name, zoneList->tail->zone->numberOfHabitants, zoneList->tail->zone->habitantsAverageAge);
	printf("---------------------------------------------------------\n\n");

	do {
		PrintZone(traverse->zone);
		traverse = traverse->next;
	} while (traverse != zoneList->zones);
}

void PrintZone(struct Zone* zone) {
	printf("nombre = \"%s\"\nnumero de habitantes = %d\nedad promedio de los habitantes %.2f\n\n", zone->name, zone->numberOfHabitants, zone->habitantsAverageAge);
}

void PrintZoneNames(struct ZoneList* zoneList) {
	if (!zoneList || !zoneList->zones) { return; }

	struct ZoneNode* traverse = zoneList->zones;
	int i = 0;

	printf("--------------------------------------------------\n\n");
	printf("NOMBRES DE LAS ZONAS CREADAS:\n\n");

	do {
		printf("%d. \"%s\"\n", i++, traverse->zone->name);
		traverse = traverse->next;
	} while (traverse != zoneList->zones);
	printf("\n--------------------------------------------------\n\n");
}

struct ZoneNode* CreateZoneNode(struct Zone* zone) {
	struct ZoneNode* node;
	node = (struct ZoneNode*)malloc(sizeof(struct ZoneNode));

	if (!node) { return NULL; }

	node->zone = zone;
	node->next = NULL;

	return node;
}

struct Zone* CreateZone(char* name, int numberOfHabitants, float habitantsAverageAge) {
	struct Zone* zone;
	zone = (struct Zone*)malloc(sizeof(struct Zone));

	if (!zone) { return NULL; }

	zone->name = (char*)malloc(sizeof(char) * strlen(name));
	if (!zone->name) { return NULL; }
	strcpy(zone->name, name);

	zone->numberOfHabitants = numberOfHabitants;
	zone->habitantsAverageAge = habitantsAverageAge;
	zone->distributionCenters = CreateDistributionCenterNode(NULL);

	return zone;
}