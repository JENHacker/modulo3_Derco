#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "purchases.h"

struct Purchases* CreatePurchases(int arrayLenght) {
	struct Purchases* purchases;
	int i;
	purchases = (struct Purchases*)malloc(sizeof(struct Purchases));

	if (!purchases) { return NULL; }

	purchases->numberOfPurchaseProducts = 0;

	purchases->lenght = arrayLenght;
	purchases->indexFirstEmpty = 0;
	purchases->purchaseArray = (struct Purchase**)malloc(sizeof(struct Purchase*) * arrayLenght);

	if (!purchases->purchaseArray) { return NULL; }

	for (i = 0; i < arrayLenght; i++) { purchases->purchaseArray[i] = NULL; }

	return purchases;
}

int AddPurchase(struct Purchases* purchases, struct Purchase* purchase) {
	if (purchases->indexFirstEmpty >= purchases->lenght || FindPurchaseByID(purchases, purchase->ID)) { return 1; }
	
	purchases->numberOfPurchaseProducts += purchase->amount;
	purchases->purchaseArray[purchases->indexFirstEmpty++] = purchase;

	return 0;
}

int DeletePurchase(struct Purchases* purchases, struct Purchase* purchase) {
	int i;

	for (i = 0; i < purchases->indexFirstEmpty; i++) {
		if (purchases->purchaseArray[i] == purchase) {
			purchases->numberOfPurchaseProducts -= purchase->amount;
			Compact(purchases, i);
			return 0;
		}
	}
	return 1;
}

int DeletePurchaseByIndex(struct Purchases* purchases, int index) {
	if (index >= purchases->indexFirstEmpty) { return 1; }
	purchases->numberOfPurchaseProducts -= purchases->purchaseArray[index]->amount;
	Compact(purchases, index);
	return 0;
}

void Compact(struct Purchases* purchases, int indexStartToCompact) {
	int i;

	purchases->indexFirstEmpty--;

	for (i = indexStartToCompact; i < purchases->indexFirstEmpty; i++) {
		purchases->purchaseArray[i] = purchases->purchaseArray[i + 1];
	}

	purchases->purchaseArray[purchases->indexFirstEmpty] = NULL;
}

struct Purchase* FindPurchaseByID(struct Purchases* purchases, int ID) {
	int i;

	for (i = 0; i < purchases->indexFirstEmpty; i++) {
		if (purchases->purchaseArray[i]->ID == ID) { return purchases->purchaseArray[i]; }
	}

	return NULL;
}

int ChangePurchaseProductSharedData(struct Purchase* purchase, struct ProductSharedData* productSharedData) {
	if (!purchase) { return 1; }
	purchase->productSharedData = productSharedData;
	return 0;
}

int ChangePurchaseAmount(struct Purchase* purchase, int amount) {
	if (!purchase) { return 1; }
	purchase->amount = amount;
	return 0;
}

int ChangePurchaseBoughtPrice(struct Purchase* purchase, float boughtPrice) {
	if (!purchase) { return 1; }
	purchase->boughtPrice = boughtPrice;
	return 0;
}

void PrintPurchases(struct Purchases* purchases) {
	int i;

	printf("---------------------------------------------------------\n\n");
	printf("numero de productos comprados = %d\n\nlongitud de arreglo = %d\nindice pLibre arreglo = %d\n\n", purchases->numberOfPurchaseProducts, purchases->lenght, purchases->indexFirstEmpty);
	printf("---------------------------------------------------------\n\n");

	for (i = 0; i < purchases->indexFirstEmpty; i++) {
		PrintPurchase(purchases->purchaseArray[i]);
	}
}

void PrintPurchase(struct Purchase* purchase) {
	printf("Producto comprado:\n\n");
	PrintProductSharedData(purchase->productSharedData);
	printf("numero de voleta de la compra = %d\ncantidad comprada = %d\nprecio de compra = %.2f\n\n", purchase->ID, purchase->amount, purchase->boughtPrice);
}

void PrintPurchaseIDs(struct Purchases* purchases) {
	if (!purchases->purchaseArray[0]) { return; }
	
	int i;

	printf("--------------------------------------------------\n\n");
	printf("NUMERO DE VOLETA DE LAS COMPRAS CREADAS:\n\n");

	for (i = 0; i < purchases->indexFirstEmpty; i++) {
		printf("%d. %d\n", i, purchases->purchaseArray[i]->ID);
	}
	printf("\n--------------------------------------------------\n\n");
}

struct Purchase* CreatePurchase(int ID, struct ProductSharedData* productSharedData, int amount, float boughtPrice) {
	struct Purchase* purchase;
	purchase = (struct Purchase*)malloc(sizeof(struct Purchase));

	if (!purchase) { return NULL; }

	purchase->ID = ID;
	purchase->productSharedData = productSharedData;
	purchase->amount = amount;
	purchase->boughtPrice = boughtPrice;

	return purchase;
}
