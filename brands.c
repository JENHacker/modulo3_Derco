#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "brands.h"

int AddBrand(struct BrandNode** brands, struct Brand* brand) {
	if (FindBrandByName(*brands, *brand->name)) { return 1; }

	struct BrandNode *newNode;

	newNode = CreateBrandNode(brand);
	if (!newNode) { return 1; }

	if (!(*brands)) {
		*brands = newNode;
		newNode->next = newNode;
		newNode->prev = newNode;
		return 0;
	}

	newNode->next = *brands;
	newNode->prev = (*brands)->prev;
	newNode->prev->next = newNode;
	newNode->next->prev = newNode;

	return 0;
}

int DeleteBrand(struct BrandNode** brands, struct BrandNode* nodeToDelete) {
	if (!(*brands)) { return 1; }
	
	struct BrandNode* traverse = *brands;

	do {
		if (traverse == nodeToDelete) {
			if (traverse->next == traverse) { *brands = NULL; }
			else {
				if (traverse == *brands) { *brands = (*brands)->next; }
				traverse->prev->next = traverse->next;
				traverse->next->prev = traverse->prev;
			}
			return 0;
		}
		traverse = traverse->next;
	} while (traverse != *brands);

	return 1;
}

struct BrandNode* FindBrandByName(struct BrandNode* brands, char* name) {
	if (!brands) { return NULL; }

	struct BrandNode* traverse = brands;

	do {
		if (!strcmp(*traverse->brand->name, name)) { return traverse; }
		traverse = traverse->next;
	} while (traverse != brands);

	return NULL;
}

int ChangeBrandName(struct BrandNode* nodeToChange, char* name) {
	if (!nodeToChange) { return 1; }

	(*nodeToChange->brand->name) = (char*)malloc(sizeof(char) * strlen(name));
	if (!(*nodeToChange->brand->name)) { return 1; }
	strcpy(*nodeToChange->brand->name, name);
	return 0;
}

int ChangeBrandProductTravelTime(struct BrandNode* nodeToChange, float productTravelTime) {
	if (!nodeToChange || productTravelTime == (float)(-1)) { return 1; }
	nodeToChange->brand->productTravelTime = productTravelTime;
	return 0;
}

int ChangeBrandStatus(struct BrandNode* nodeToChange, float status) {
	if (!nodeToChange || status == -1) { return 1; }
	nodeToChange->brand->status = status;
	return 0;
}

void PrintBrands(struct BrandNode* brands) {
	if (!brands) { return; }

	struct BrandNode* traverse = brands;

	do {
		PrintBrand(traverse->brand);
		traverse = traverse->next;
	} while (traverse != brands);
}

void PrintBrand(struct Brand* brand) {
	printf("nombre = \"%s\"\ntiempo de desplazamiento de mercancias (dias) = %.2f\nvaloracion = %.2f\n\n", *brand->name, brand->productTravelTime, brand->status);
}

void PrintBrandNames(struct BrandNode* brands) {
	if (!brands) { return; }

	struct BrandNode* traverse = brands;
	int i = 0;

	printf("--------------------------------------------------\n\n");
	printf("NOMBRE DE LAS MARCAS/PROVEEDORES CREADOS:\n\n");

	do {
		printf("%d. \"%s\"\n", i++, *traverse->brand->name);
		traverse = traverse->next;
	} while (traverse != brands);
	printf("\n--------------------------------------------------\n\n");
}

struct BrandNode* CreateBrandNode(struct Brand* brand) {
	struct BrandNode* node;
	node = (struct BrandNode*)malloc(sizeof(struct BrandNode));

	if (!node) { return NULL; }

	node->brand = brand;
	node->next = NULL;
	node->prev = NULL;

	return node;
}

struct Brand* CreateBrand(char* name, float productTravelTime, float status) {
	struct Brand* brand;
	brand = (struct Brand*)malloc(sizeof(struct Brand));

	if (!brand) { return NULL; }

	brand->name = (char**)malloc(sizeof(char*));
	if (!brand->name) { return NULL; }

	(*brand->name) = (char*)malloc(sizeof(char) * strlen(name));
	if (!(*brand->name)) { return NULL; }
	strcpy(*brand->name, name);

	brand->productTravelTime = productTravelTime;
	brand->status = status;
	brand->stock = NULL;

	return brand;
}