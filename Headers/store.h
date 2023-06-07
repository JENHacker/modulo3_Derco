#ifndef STORE_H
#define STORE_H

#include "brands.h"

enum ProductType {
	CAR,
	CAR_SPARE_PART,
	ACCESSORY,
	END
};

struct Store {
	int numberOfProducts;
	float cbMetersMaxCapacity;
	float cbMetersSpaceUsed;

	struct ProductNode *products;
};

struct ProductNode {
	struct Product *product;
	struct ProductNode *left;
	struct ProductNode *right;
};

struct ProductSharedData {
	int ID;
	enum ProductType type;
	int recalls;
	char* name;
	char* description;
	char** brandName;
	float cbMetersSpacePerUnit;
};

struct Product {
	struct ProductSharedData *sharedData;
	int amount;
	float price;
};

struct Store* CreateStore(float);
int AddProduct(struct ProductNode**, struct Product*);
int DeleteProduct(struct ProductNode**, int);
struct ProductNode* FindProductByID(struct ProductNode*, int);

int ChangeStoreMaxCapacity(struct Store*, float);

int ChangeProductType(struct ProductNode*, enum ProductType type);
int ChangeProductRecalls(struct ProductNode*, int);
int ChangeProductName(struct ProductNode*, char*);
int ChangeProductDescription(struct ProductNode*, char*);
int ChangeProductPrice(struct ProductNode*, float);
int ChangeProductSpacePerUnit(struct ProductNode*, float);
int ChangeProductAmount(struct ProductNode*, int);

void PrintStore(struct Store*);
void PrintProducts(struct ProductNode*);
void PrintProduct(struct Product*);
void PrintProductSharedData(struct ProductSharedData*);
void PrintProductIDs(struct ProductNode*);
void RecPrintProductIDs(struct ProductNode*, int*);

struct ProductNode* CreateProductNode(struct Product*);
struct Product* CreateProduct(struct ProductSharedData*, int, float);
struct ProductSharedData* CreateProductSharedData(int, enum ProductType type, char*, char*, char**, float);

#endif // !STORE_H