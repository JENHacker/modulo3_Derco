#ifndef PURCHASES_H
#define PURCHASES_H

#include "store.h"

struct Purchases {
	int numberOfPurchaseProducts;

	int lenght;
	int indexFirstEmpty;
	struct Purchase **purchaseArray;
};

struct Purchase {
	int ID;
	struct ProductSharedData* productSharedData;
	int amount;
	float boughtPrice;
};

struct Purchases* CreatePurchases(int);
int AddPurchase(struct Purchases*, struct Purchase*);
int DeletePurchase(struct Purchases*, struct Purchase*);
int DeletePurchaseByIndex(struct Purchases*, int);
struct Purchase* FindPurchaseByID(struct Purchases*, int);

int ChangePurchaseProductSharedData(struct Purchase*, struct ProductSharedData*);
int ChangePurchaseAmount(struct Purchase*, int);
int ChangePurchaseBoughtPrice(struct Purchase*, float);

void PrintPurchases(struct Purchases*);
void PrintPurchase(struct Purchase*);
void PrintPurchaseIDs(struct Purchases*);

void Compact(struct Purchases*, int);

struct Purchase* CreatePurchase(int, struct ProductSharedData*, int, float);

#endif // !PURCHASES_H