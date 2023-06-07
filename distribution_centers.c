#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "distribution_centers.h"
#include "store.h"

int AddDistributionCenter(struct DistributionCenterNode* distributionCenters, struct DistributionCenter* distributionCenter) {
	if (FindDistributionCenterByID(distributionCenters, distributionCenter->ID)) { return 1; }
	
	struct DistributionCenterNode *newNode, *traverse = distributionCenters;

	newNode = CreateDistributionCenterNode(distributionCenter);

	if (!newNode) { return 1; }

	while (traverse->next) { traverse = traverse->next; }

	traverse->next = newNode;
	newNode->prev = traverse;

	return 0;
}

int DeleteDistributionCenter(struct DistributionCenterNode* distributionCenters, struct DistributionCenterNode* nodeToDelete) {
	struct DistributionCenterNode* traverse = distributionCenters->next;

	while (traverse) {
		if (traverse == nodeToDelete) {
			traverse->prev->next = traverse->next;
			if (traverse->next) { traverse->next->prev = traverse->prev; }
			return 0;
		}
		traverse = traverse->next;
	}

	return 1;
}

struct DistributionCenterNode* FindDistributionCenterByID(struct DistributionCenterNode* distributionCenters, int ID) {
	struct DistributionCenterNode* traverse = distributionCenters->next;

	while (traverse) {
		if (traverse->distributionCenter->ID == ID) { return traverse; }
		traverse = traverse->next;
	}

	return NULL;
}

int ChangeDistributionCenterAddress(struct DistributionCenterNode* nodeToChange, char* address) {
	if (!nodeToChange) { return 1; }
	nodeToChange->distributionCenter->address = (char*)malloc(sizeof(char) * strlen(address));
	if (!nodeToChange->distributionCenter->address) { return 1; }
	strcpy(nodeToChange->distributionCenter->address, address);
	return 0;
}

int ChangeDistributionCenterNumberOfEmployes(struct DistributionCenterNode* nodeToChange, int numberOfEmployes) {
	if (!nodeToChange || numberOfEmployes == -1) { return 1; }
	nodeToChange->distributionCenter->numberOfEmployes = numberOfEmployes;
	return 0;
}

void PrintDistributionCenters(struct DistributionCenterNode* distributionCenters) {
	struct DistributionCenterNode *traverse = distributionCenters->next;

	while (traverse) {
		PrintDistributionCenter(traverse->distributionCenter);
		traverse = traverse->next;
	}
}

void PrintDistributionCenter(struct DistributionCenter* distributionCenter) {
	printf("ID = %d\ndireccion = \"%s\"\nnumero de empleados = %d\n\n", distributionCenter->ID, distributionCenter->address, distributionCenter->numberOfEmployes);
}

void PrintDistributionCenterIDs(struct DistributionCenterNode* distributionCenters) {
	if (!distributionCenters->next) { return; }
	
	struct DistributionCenterNode* traverse = distributionCenters->next;
	int i = 0;

	printf("--------------------------------------------------\n\n");
	printf("ID DE LOS CENTROS DE DISTRIBUCION CREADOS:\n\n");

	while (traverse) {
		printf("%d. %d\n", i++, traverse->distributionCenter->ID);
		traverse = traverse->next;
	}
	printf("\n--------------------------------------------------\n\n");
}

struct DistributionCenterNode* CreateDistributionCenterNode(struct DistributionCenter* distributionCenter) {
	struct DistributionCenterNode* node;
	node = (struct DistributionCenterNode*)malloc(sizeof(struct DistributionCenterNode));

	if (!node) { return NULL; }

	node->distributionCenter = distributionCenter;
	node->next = NULL;
	node->prev = NULL;

	return node;
}

struct DistributionCenter* CreateDistributionCenter(int ID, char* address, int numberOfEmployes, float storeMaxCapacity) {
	struct DistributionCenter* distributionCenter;
	distributionCenter = (struct DistributionCenter*)malloc(sizeof(struct DistributionCenter));

	if (!distributionCenter) { return NULL; }

	distributionCenter->ID = ID;

	distributionCenter->address = (char*)malloc(sizeof(char) * strlen(address));
	if (!distributionCenter->address) { return NULL; }
	strcpy(distributionCenter->address, address);
	
	distributionCenter->numberOfEmployes = numberOfEmployes;
	distributionCenter->store = CreateStore(storeMaxCapacity);
	distributionCenter->customers = NULL;

	return distributionCenter;
}