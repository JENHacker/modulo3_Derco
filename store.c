#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "store.h"

struct Store* CreateStore(float maxCapacity) {
	struct Store* store;
	store = (struct Store*)malloc(sizeof(struct Store));

	if (!store) { return NULL; }

	store->numberOfProducts = 0;
	store->cbMetersMaxCapacity = maxCapacity;
	store->cbMetersSpaceUsed = 0;
	store->products = NULL;

	return store;
}

int AddProduct(struct ProductNode** root, struct Product* product) {
	if (!(*root)) {
		*root = CreateProductNode(product);
		return (*root) ? 0 : 1;
	}

	if (product->sharedData->ID < (*root)->product->sharedData->ID) { return AddProduct(&((*root)->left), product); }
	if ((*root)->product->sharedData->ID == product->sharedData->ID) { return 1; }
	return AddProduct(&((*root)->right), product);
}

int DeleteProduct(struct ProductNode** root, int ID) {
	if (!(*root)) { return 1; }

	if (ID < (*root)->product->sharedData->ID) { return DeleteProduct(&((*root)->left), ID); }

	if ((*root)->product->sharedData->ID == ID) {
		if ((*root)->left && (*root)->right) {
			struct ProductNode** parentLeftmostTraverse = &(*root)->right;

			while ((*parentLeftmostTraverse)->left) { parentLeftmostTraverse = &(*parentLeftmostTraverse)->left; }

			(*root)->product = (*parentLeftmostTraverse)->product;
			*parentLeftmostTraverse = (*parentLeftmostTraverse)->right;
		}
		else { *root = ((*root)->left) ? (*root)->left : (*root)->right; }
		return 0;
	}

	return DeleteProduct(&((*root)->right), ID);
}

struct ProductNode* FindProductByID(struct ProductNode* root, int ID) {
	if (!root) { return NULL; }

	if (ID < root->product->sharedData->ID) { return FindProductByID(root->left, ID); }
	if (root->product->sharedData->ID == ID) { return root; }
	return FindProductByID(root->right, ID);
}

int ChangeStoreMaxCapacity(struct Store* store, float maxCapacity) {
	if (!store || maxCapacity < store->cbMetersSpaceUsed || maxCapacity == -1) { return 1; }
	store->cbMetersMaxCapacity = maxCapacity;
	return 0;
}

int ChangeProductType(struct ProductNode* nodeToChange, enum ProductType type) {
	if (!nodeToChange) { return 1; }
	nodeToChange->product->sharedData->type = type;
	return 0;
}

int ChangeProductRecalls(struct ProductNode* nodeToChange, int recalls) {
	if (!nodeToChange) { return 1; }
	nodeToChange->product->sharedData->recalls = recalls;
	return 0;
}

int ChangeProductName(struct ProductNode* nodeToChange, char* name) {
	if (!nodeToChange) { return 1; }

	nodeToChange->product->sharedData->name = (char*)malloc(sizeof(char) * strlen(name));
	if (!nodeToChange->product->sharedData->name) { return 1; }
	strcpy(nodeToChange->product->sharedData->name, name);
	return 0;
}

int ChangeProductDescription(struct ProductNode* nodeToChange, char* description) {
	if (!nodeToChange) { return 1; }
	
	nodeToChange->product->sharedData->description = (char*)malloc(sizeof(char) * strlen(description));
	if (!nodeToChange->product->sharedData->description) { return 1; }
	strcpy(nodeToChange->product->sharedData->description, description);
	return 0;
}

int ChangeProductPrice(struct ProductNode* nodeToChange, float price) {
	if (!nodeToChange || price == (float)(-1)) { return 1; }
	nodeToChange->product->price = price;
	return 0;
}

int ChangeProductSpacePerUnit(struct ProductNode* nodeToChange, float spacePerUnit) {
	if (!nodeToChange || spacePerUnit == (float)(-1)) { return 1; }
	nodeToChange->product->sharedData->cbMetersSpacePerUnit = spacePerUnit;
	return 0;
}

int ChangeProductAmount(struct ProductNode* nodeToChange, int amount) {
	if (!nodeToChange || amount == -1) { return 1; }
	nodeToChange->product->amount = amount;
	return 0;
}

void PrintStore(struct Store* store) {
	if (!store) { return; }

	printf("---------------------------------------------------------\n\n");
	printf("numero de productos = %d\n\ncapacidad maxima (m^3) = %.2f\nespacio usado (m^3) = %.2f\n\n", store->numberOfProducts, store->cbMetersMaxCapacity, store->cbMetersSpaceUsed);
	printf("---------------------------------------------------------\n\n");

	PrintProducts(store->products);
}

void PrintProducts(struct ProductNode* root) {
	if (!root) { return; }

	PrintProducts(root->left);
	PrintProduct(root->product);
	PrintProducts(root->right);
}

void PrintProduct(struct Product* product) {
	PrintProductSharedData(product->sharedData);
	printf("cantidad = %d\nprecio = %.2f\n\n", product->amount, product->price);
}

void PrintProductSharedData(struct ProductSharedData* productSharedData) {
	printf("ID = %d\ntipo de producto = ", productSharedData->ID);

	switch (productSharedData->type) {
	case CAR: printf("vehiculo\nrecalls = %d", productSharedData->recalls); break;
	case CAR_SPARE_PART: printf("pieza de repuesto"); break;
	case ACCESSORY: printf("accesorio"); break;
	}

	printf("\nnombre = \"%s\"\ndescripcion = \"%s\"\nnombre de la marca = \"%s\"\nvolumen por unidad (m^3) = %.2f\n\n", productSharedData->name, productSharedData->description, *productSharedData->brandName, productSharedData->cbMetersSpacePerUnit);
}

void PrintProductIDs(struct ProductNode* root) {
	if (!root) { return; }
	int i = 0;

	printf("--------------------------------------------------\n\n");
	printf("ID DE LOS PRODUCTOS CREADOS:\n\n");
	RecPrintProductIDs(root, &i);
	printf("\n--------------------------------------------------\n\n");
}

void RecPrintProductIDs(struct ProductNode* root, int* number) {
	if (!root) { return; }

	RecPrintProductIDs(root->left, number);
	printf("%d. %d\n", (*number)++, root->product->sharedData->ID);
	RecPrintProductIDs(root->right, number);
}

struct ProductNode* CreateProductNode(struct Product* product) {
	struct ProductNode* node;
	node = (struct ProductNode*)malloc(sizeof(struct ProductNode));

	if (!node) { return NULL; }

	node->product = product;
	node->left  = NULL;
	node->right = NULL;

	return node;
}

struct Product* CreateProduct(struct ProductSharedData* sharedData, int amount, float price) {
	struct Product* product;
	product = (struct Product*)malloc(sizeof(struct Product));

	if (!product) { return NULL; }

	product->sharedData = sharedData;
	product->price = price;
	product->amount = amount;

	return product;
}

struct ProductSharedData* CreateProductSharedData(int ID, enum ProductType type, char* name, char* description, char** brandName, float spacePerUnit) {
	struct ProductSharedData* sharedData;
	sharedData = (struct ProductSharedData*)malloc(sizeof(struct ProductSharedData));

	if (!sharedData) { return NULL; }

	sharedData->ID = ID;
	sharedData->type = type;
	sharedData->recalls = 0;

	sharedData->name = (char*)malloc(sizeof(char) * strlen(name));
	if (!sharedData->name) { return NULL; }
	strcpy(sharedData->name, name);

	sharedData->description = (char*)malloc(sizeof(char) * strlen(description));
	if (!sharedData->description) { return NULL; }
	strcpy(sharedData->description, description);

	sharedData->brandName = brandName;
	sharedData->cbMetersSpacePerUnit = spacePerUnit;

	return sharedData;
}