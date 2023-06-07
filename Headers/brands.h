#ifndef BRANDS_H
#define BRANDS_H

#include "store.h"

struct BrandNode {
	struct Brand *brand;
	struct BrandNode *next;
	struct BrandNode *prev;
};

struct Brand {
	char** name;
	float ProductTravelTime;
	float status;

	struct ProductNode *stock;
};

int AddBrand(struct BrandNode**, struct Brand*);
int DeleteBrand(struct BrandNode**, struct BrandNode*);
struct BrandNode* FindBrandByName(struct BrandNode*, char*);

int ChangeBrandName(struct BrandNode*, char*);
int ChangeBrandProductTravelTime(struct BrandNode*, float);
int ChangeBrandStatus(struct BrandNode*, float);

void PrintBrands(struct BrandNode*);
void PrintBrand(struct Brand*);
void PrintBrandNames(struct BrandNode*);

struct BrandNode* CreateBrandNode(struct Brand*);
struct Brand* CreateBrand(char*, float, float);

#endif // !BRANDS_H