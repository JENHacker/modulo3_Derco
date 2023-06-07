#ifndef DISTRIBUTION_CENTER_H
#define DISTRIBUTION_CENTER_H

#include "store.h"
#include "customers.h"

struct DistributionCenterNode {
	struct DistributionCenter* distributionCenter;
	struct DistributionCenterNode *next;
	struct DistributionCenterNode *prev;
};

struct DistributionCenter {
	int ID;
	char *address;
	int numberOfEmployes;

	struct Store *store;
	struct CustomerNode *customers;
};

int AddDistributionCenter(struct DistributionCenterNode*, struct DistributionCenter*);
int DeleteDistributionCenter(struct DistributionCenterNode*, struct DistributionCenterNode*);
struct DistributionCenterNode* FindDistributionCenterByID(struct DistributionCenterNode*, int);

int ChangeDistributionCenterAddress(struct DistributionCenterNode*, char*);
int ChangeDistributionCenterNumberOfEmployes(struct DistributionCenterNode*, int);

void PrintDistributionCenters(struct DistributionCenterNode*);
void PrintDistributionCenter(struct DistributionCenter*);
void PrintDistributionCenterIDs(struct DistributionCenterNode*);

struct DistributionCenterNode* CreateDistributionCenterNode(struct DistributionCenter*);
struct DistributionCenter* CreateDistributionCenter(int, char*, int, float);

#endif // !DISTRIBUTION_CENTER_H