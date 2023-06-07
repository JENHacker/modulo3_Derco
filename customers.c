#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "customers.h"
#include "store.h"
#include "purchases.h"

int AddCustomer(struct CustomerNode** root, struct Customer* customer) {
	if (!(*root)) {
		*root = CreateCustomerNode(customer);
		return (*root) ? 0 : 1;
	}

	if ((*root)->customer->ID == customer->ID) { return 1; }

	return (customer->ID < (*root)->customer->ID) ? AddCustomer(&((*root)->left), customer) : AddCustomer(&((*root)->right), customer);
}

int DeleteCustomer(struct CustomerNode** root, int ID) {
	if (!(*root)) { return 1; } 

	if ((*root)->customer->ID == ID) {
		if ((*root)->left && (*root)->right) { 
			struct CustomerNode** parentLeftmostTraverse = &(*root)->right;

			while ((*parentLeftmostTraverse)->left) { parentLeftmostTraverse = &(*parentLeftmostTraverse)->left; }

			(*root)->customer = (*parentLeftmostTraverse)->customer;
			*parentLeftmostTraverse = (*parentLeftmostTraverse)->right;
		} else { *root = ((*root)->left) ? (*root)->left : (*root)->right; }
		return 0;
	}

	return (ID < (*root)->customer->ID) ? DeleteCustomer(&((*root)->left), ID) : DeleteCustomer(&((*root)->right), ID);
}

struct CustomerNode* FindCustomerByID(struct CustomerNode* root, int ID) {
	if (!root || root->customer->ID == ID) { return root; }
	return (ID < root->customer->ID) ? FindCustomerByID(root->left, ID) : FindCustomerByID(root->right, ID) ;
}

int ChangeCustomerName(struct CustomerNode* nodeToChange, char* name) {
	if (!nodeToChange) { return 1; }
	nodeToChange->customer->name = (char*)malloc(sizeof(char) * strlen(name));
	if (!nodeToChange->customer->name) { return 1; }
	strcpy(nodeToChange->customer->name, name);
}

int ChangeCustomerAge(struct CustomerNode* nodeToChange, int age) {
	if (!nodeToChange || age == -1) { return 1; }
	nodeToChange->customer->age = age;
}

void PrintCustomers(struct CustomerNode* root) {
	if (!root) { return; }

	PrintCustomer(root->customer);
	PrintCustomers(root->left);
	PrintCustomers(root->right);
}

void PrintCustomer(struct Customer* customer) {
	printf("ID = %d\nnombre = \"%s\"\nedad = %d\n\n", customer->ID, customer->name, customer->age);
}

void PrintCustomerIDs(struct CustomerNode* root) {
	if (!root) { return; }

	int i = 0;

	printf("--------------------------------------------------\n\n");
	printf("ID DE LOS CLIENTES CREADOS:\n\n");
	RecPrintCustomerIDs(root, &i);
	printf("\n--------------------------------------------------\n\n");
}

void RecPrintCustomerIDs(struct CustomerNode* root, int* number) {
	if (!root) { return; }

	printf("%d. %d\n", (*number)++, root->customer->ID);
	RecPrintCustomerIDs(root->left, number);
	RecPrintCustomerIDs(root->right, number);
}

struct CustomerNode* CreateCustomerNode(struct Customer* customer) {
	struct CustomerNode* node;
	node = (struct CustomerNode*)malloc(sizeof(struct CustomerNode));

	if (!node) { return NULL; }

	node->customer = customer;
	node->left = NULL;
	node->right = NULL;

	return node;
}

struct Customer* CreateCustomer(int ID, char* name, int age, int maxPurchases) {
	struct Customer* customer;
	customer = (struct Customer*)malloc(sizeof(struct Customer));

	if (!customer) { return NULL; }

	customer->ID = ID;

	customer->name = (char*)malloc(sizeof(char) * strlen(name));
	if (!customer->name) { return NULL; }
	strcpy(customer->name, name);

	customer->age = age;
	customer->purchases = CreatePurchases(maxPurchases);

	return customer;
}