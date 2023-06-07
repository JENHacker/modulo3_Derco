#ifndef CUSTOMERS_H
#define CUSTOMERS_H

#include "purchases.h"

struct CustomerNode {
	struct Customer *customer;
	struct CustomerNode *left;
	struct CustomerNode *right;
};

struct Customer {
	int ID;
	char* name;
	int age;

	struct Purchases *purchases;
};

int AddCustomer(struct CustomerNode**, struct Customer*);
int DeleteCustomer(struct CustomerNode**, int);
struct CustomerNode* FindCustomerByID(struct CustomerNode*, int);

int ChangeCustomerName(struct CustomerNode*, char*);
int ChangeCustomerAge(struct CustomerNode*, int);

void PrintCustomers(struct CustomerNode*);
void PrintCustomer(struct Customer*);
void PrintCustomerIDs(struct CustomerNode*);
void RecPrintCustomerIDs(struct CustomerNode*, int*);

struct CustomerNode* CreateCustomerNode(struct Customer*);
struct Customer* CreateCustomer(int, char*, int, int);

#endif // !CUSTOMERS_H