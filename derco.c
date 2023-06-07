#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <conio.h>

#include "zones.h"
#include "distribution_centers.h"
#include "customers.h"
#include "store.h"
#include "purchases.h"
#include "brands.h"

#define CLEARSCREEN system("cls")
#define CLEARINPUTBUFFER while (getchar() != '\n')
#define ACTIONTIME 2000
#define STRINGBUFFERSIZE 100

enum Menu {
	mADD = 1,
	mDELETE,
	mFIND,
	mMODIFY1,
	mPRINT,
	mSPECIALOPTION
};

enum Find {
	fDELETE = 1,
	fMODIFY,
	fDATA_STRUCTURE1,
	fDATA_STRUCTURE2
};

struct Derco {
	struct ZoneList* zoneList;
	struct BrandNode* brands;
} *derco;

struct BrandNameNode {
	char* brandName;
	struct BrandNameNode* next;
};

struct ProductIDNode {
	struct ProductSharedData* ID;
	struct ProductIDNode* next;
};

struct BrandNode* actualBrand;
struct DistributionCenterNode* actualDistributionCenter;

void MenuZoneList(struct ZoneList*);
int MenuAddZone(struct ZoneList*);
int MenuDeleteZone(struct ZoneList*);
int MenuFindZone(struct ZoneList*);
int MenuModifyZone(struct ZoneList*, struct ZoneNode*);

void MenuDistributionCenters(struct DistributionCenterNode*);
int MenuAddDistributionCenter(struct DistributionCenterNode*);
int MenuDeleteDistributionCenter(struct DistributionCenterNode*);
int MenuFindDistributionCenter(struct DistributionCenterNode*);
int MenuModifyDistributionCenter(struct DistributionCenterNode*, struct DistributionCenterNode*);

void MenuCustomers(struct CustomerNode**);
int MenuAddCustomer(struct CustomerNode**);
int MenuDeleteCustomer(struct CustomerNode**);
int MenuFindCustomer(struct CustomerNode**);
int MenuModifyCustomer(struct CustomerNode**, struct CustomerNode*);

void MenuStore(struct Store*);
int MenuAddProductToStore(struct Store*);
int MenuDeleteStoreProduct(struct Store*);
int MenuFindStoreProduct(struct Store*);
int MenuModifyStoreProduct(struct Store*, struct ProductNode*);

void MenuPurchases(struct Purchases*);
int MenuAddPurchase(struct Purchases*);
int MenuDeletePurchase(struct Purchases*);
int MenuFindPurchase(struct Purchases*);
int MenuModifyPurchase(struct Purchases*, struct Purchase*);

void MenuBrands(struct BrandNode**);
int MenuAddBrand(struct BrandNode**);
int MenuDeleteBrand(struct BrandNode**);
int MenuFindBrand(struct BrandNode**);
int MenuModifyBrand(struct BrandNode**, struct BrandNode*);

void MenuStock(struct ProductNode**);
int MenuAddProductToStock(struct ProductNode**);
int MenuDeleteStockProduct(struct ProductNode**);
int MenuFindStockProduct(struct ProductNode**);
int MenuModifyStockProduct(struct ProductNode**, struct ProductNode*);

// Funcion enunciado: Encontrar consumidor que mas ha gastado en los productos de Derco.
struct Customer* CustomerThatHasSpendTheMost();
void CustomerSpendTheMostTraverse(struct CustomerNode*, struct Customer**, float*);
float TotalMoneySpend(struct Purchases*);

// Funcion enunciado: Mostrar autos vendidos por zona
void PrintCarsSoldInZone(struct DistributionCenterNode*);
void CustomerTraversePrintCars(struct CustomerNode*, struct ProductIDNode**);
void PrintProductBought(struct Purchases*, struct ProductIDNode**);

// Funcion enunciado: Encontrar la marcar/proveedor de autos cuyos autos se hayan vendido mas en una zona especifica.
char* BestSellingCarBrandInZone(struct DistributionCenterNode*);
struct BrandNameNode* AddBrandNameNode(struct BrandNameNode*, char*);
struct BrandNameNode* CreateCarBrandNameList(struct DistributionCenterNode*, struct BrandNameNode*);
void CustomerTraverseCarBrandNames(struct CustomerNode*, struct BrandNameNode**);
struct BrandNameNode* AddNewCarBrandNames(struct Purchases*, struct BrandNameNode*);
void CustomerCarMostSellBrandTraverse(struct CustomerNode*, char*, int*);
int FindBrandNameInList(struct BrandNameNode*, char*);
int CountCarSalesOfBrand(struct Purchases*, char*);

// Funcion enunciado: Auto con mas recalls
struct Product* FindCarWithMoreRecalls();
void ProductTraverseCarWithMoreRecalls(struct ProductNode*, struct Product**, int*);

// Funcion enunciado: Centro de distribucion con mayor distribucion
struct DistributionCenter* DistributionCenterWithMoreSales();
void CustomerTraverseDistributionCenterWithMoreSales(struct CustomerNode*, int*);

// Funcion extra 1: Encontrar marca/proovedor con peor valoracion
struct Brand* FindBrandWithLowestStatus();

// Funcion extra 2: Encontrar producto mas vendido en base al tipo en todas las zonas
struct ProductSharedData* FindBestSellingProductByTypeInAllZones(enum ProductType type);
struct ProductSharedData* FindBestSellingProductByType(struct DistributionCenterNode*, enum ProductType type, int*);
void CustomerTraverseBestSellingProduct(struct CustomerNode*, struct ProductSharedData*, int*);
int CountProductSales(struct Purchases*, struct ProductSharedData*);
struct ProductIDNode* CreateProductIDsList(struct DistributionCenterNode*, enum ProductType type);
void CustomerTraverseProductIDs(struct CustomerNode*, enum ProductType type, struct ProductIDNode**);
struct ProductIDNode* AddNewProductIDs(struct Purchases*, enum ProductType type, struct ProductIDNode*);
int FindProductIDInList(struct ProductIDNode*, struct ProductSharedData*);
struct ProductIDNode* AddProductIDNode(struct ProductIDNode*, struct ProductSharedData*);

// Utility Funtions: Encontrar producto por ID en todas las marcas
struct Product* FindProductByIDInAllBrands(int);

// Utility Funtions: Mostrar el ID de todos los productos por cada marca
void PrintProductIDsInAllBrands();

// Utility Funtions: Retornar el entero ingresado por el usuario
int EnterInteger(char*);

// Utility Funtions: Retornar el punto flotante ingresado por el usuario
float EnterFloatingPoint(char*);

// Utility Funtions: Retornar el texto ingresado por el usuario
char* EnterText(char*);

int main() {
	int input;

	derco = (struct Derco*)malloc(sizeof(struct Derco));

	if (!derco) { return 1; }

	derco->zoneList = CreateZoneCircularList();
	derco->brands   = NULL;

	do {
		CLEARSCREEN;
		printf("MENU PRINCIPAL DERCO:\n\n0. Cerrar programa.\n1. Modificar lista de zonas.\n2. Modificar marcas/proveedores.\n3. Cliente que mas ha gastado.\n4. Auto con mas recalls.\n5. Centro de distribucion con mayor numero de ventas\n6. Mostrar marca/proveedor con una valoracion mas baja.\n7. Mostar producto de cierto tipo que Derco mas haya vendido.\n\n");
		input = EnterInteger("Ingrese un numero : ");
		if (input == -1) { continue; }

		switch (input) {
		case 0: break;
		case 1: MenuZoneList(derco->zoneList); break;
		case 2: MenuBrands(&derco->brands); break;
		case 3: 
			CLEARSCREEN;
			struct Customer* customer;
			customer = CustomerThatHasSpendTheMost();
			if (!customer) {
				printf("No se ha encontrado ningun cliente.\n\n");
			} else {
				printf("--------------------------------------------------\n\n");
				PrintCustomer(customer);
				printf("--------------------------------------------------\n\n");
			}
			printf("Presione enter para devolverse: ");
			CLEARINPUTBUFFER;
			break;
		case 4:
			CLEARSCREEN;
			struct Product* car;
			car = FindCarWithMoreRecalls();
			if (!car) {
				printf("No se ha encontrado ningun auto.\n\n");
			}
			else {
				printf("--------------------------------------------------\n\n");
				PrintProduct(car);
				printf("--------------------------------------------------\n\n");
			}
			printf("Presione enter para devolverse: ");
			CLEARINPUTBUFFER;
			break;
		case 5:
			CLEARSCREEN;
			struct DistributionCenter* distributionCenter;
			distributionCenter = DistributionCenterWithMoreSales();
			if (!distributionCenter) {
				printf("No se ha encontrado ningun centro de distribucion.\n\n");
			}
			else {
				printf("--------------------------------------------------\n\n");
				PrintDistributionCenter(distributionCenter);
				printf("--------------------------------------------------\n\n");
			}
			printf("Presione enter para devolverse: ");
			CLEARINPUTBUFFER;
			break;
		case 6:
			CLEARSCREEN;
			struct Brand* brand;
			brand = FindBrandWithLowestStatus();
			if (!brand) {
				printf("No se ha encontrado ninguna marca/proveedor.\n\n");
			}
			else {
				printf("--------------------------------------------------\n\n");
				PrintBrand(brand);
				printf("--------------------------------------------------\n\n");
			}
			printf("Presione enter para devolverse: ");
			CLEARINPUTBUFFER;
			break;
		case 7:
			printf("0. Vehiculo\n1. Pieza de repuesto\n2. Accesorio\n3. Todos los tipos\n\n");
			enum ProductType type = EnterInteger("Ingrese el tipo de producto: ");
			if (type == -1 || type > END) {
				printf("Valor invalido.");
				Sleep(ACTIONTIME);
			} else {
				CLEARSCREEN;
				struct ProductSharedData* product;
				product = FindBestSellingProductByTypeInAllZones(type);
				if (!product) {
					printf("No se ha encontrado ningun producto de ese tipo.\n\n");
				}
				else {
					printf("--------------------------------------------------\n\n");
					PrintProductSharedData(product);
					printf("--------------------------------------------------\n\n");
				}
				printf("Presione enter para devolverse: ");
				CLEARINPUTBUFFER;
			}
			break;
		default: printf("Valor invalido."); Sleep(ACTIONTIME); break;
		}
	} while (input);
	
	return 0;
}

//-----------------------------------ZONAS-----------------------------------

void MenuZoneList(struct ZoneList* zoneList) {
	enum Menu input;

	do {
		CLEARSCREEN;
		printf("MENU PRINCIPAL ZONAS:\n\n0. Atras.\n1. Agregar zona.\n2. Eliminiar zona.\n3. Buscar zona.\n4. Modificar zona.\n5. Mostrar zonas.\n\n");
		input = EnterInteger("Ingrese un numero: ");
		if (input == -1) { continue; }

		if (0 < input && input < 6) { CLEARSCREEN; }
		if (0 < input && input < 5) { PrintZoneNames(zoneList); }

		switch (input) {
		case 0: break;
		case mADD:
			if (MenuAddZone(zoneList)) {
				printf("La zona no se ha podido agregar correctamente.\n");
			} else { printf("La zona se ha agregado exitosamente.\n"); }
			Sleep(ACTIONTIME);
			break;
		case mDELETE:
			if (MenuDeleteZone(zoneList)) {
				printf("La zona no se ha podido eliminar correctamente.\n");
			} else { printf("La zona se ha eliminado exitosamente.\n"); }
			Sleep(ACTIONTIME);
			break;
		case mFIND:
			if (MenuFindZone(zoneList)) {
				printf("No se ha podido encontrar la zona.\n");
				Sleep(ACTIONTIME);
			}
			break;
		case mMODIFY1:
			if (MenuModifyZone(zoneList, NULL)) {
				printf("No se ha podido encontrar la zona.\n");
				Sleep(ACTIONTIME);
			}
			break;
		case mPRINT:
			PrintZoneList(zoneList);
			printf("Presione enter para devolverse: ");
			CLEARINPUTBUFFER;
			break;
		default: printf("Valor invalido."); Sleep(ACTIONTIME); break;
		}
	} while (input);
}

int MenuAddZone(struct ZoneList* zoneList) {
	char* name;
	int numberOfHabitants;
	float habitantsAverageAge;

	name = EnterText("Ingrese el nombre de la zona: ");
	if (!name) { return 1; }
	if (FindZoneByName(zoneList, name)) {
		printf("No se puede repetir el nombre.\n\n");
		Sleep(ACTIONTIME);
		return 1;
	}
	numberOfHabitants = EnterInteger("Ingrese el numero de habitantes de la zona: ");
	if (numberOfHabitants == -1) { return 1; }
	habitantsAverageAge = EnterFloatingPoint("Ingrese la edad promedio de los habitantes de la zona: ");
	if (habitantsAverageAge == -1) { return 1; }

	return AddZone(zoneList, CreateZone(name, numberOfHabitants, habitantsAverageAge));
}

int MenuDeleteZone(struct ZoneList* zoneList) {
	return DeleteZone(zoneList, FindZoneByName(zoneList, EnterText("Ingrese el nombre de la zona que desea eliminar: ")));
}

int MenuFindZone(struct ZoneList* zoneList) {
	enum Find input;
	struct ZoneNode* zone;
	char* carMostSellBrandName;
	struct BrandNode* carMostSellBrand;
	struct ProductSharedData* product;
	enum ProductType type;
	int bestSeller;

	zone = FindZoneByName(zoneList, EnterText("Ingrese el nombre de la zona que quieres buscar: "));
	if (!zone) { return 1; }

	do {
		CLEARSCREEN;
		printf("--------------------------------------------------\n\n");
		PrintZone(zone->zone);
		printf("--------------------------------------------------\n\n");
		printf("MENU ZONA:\n\n0. Atras.\n1. Eliminiar zona.\n2. Modificar datos de la zona.\n3. Modificar centros de distribucion.\n4. Mostrar marca/proveedor con mas autos vendidos\n5. Mostrar producto mas vendido en base al tipo de producto.\n6. Mostrar modelos de autos vendidos\n\n");
		input = EnterInteger("Ingrese un numero: ");
		if (input == -1) { continue; }

		switch (input) {
		case 0: break;
		case fDELETE:
			if (DeleteZone(zoneList, zone)) {
				printf("La zona no se ha podido eliminar correctamente.");
				Sleep(ACTIONTIME);
			}
			else {
				printf("La zona se ha eliminado exitosamente.");
				Sleep(ACTIONTIME);
				return 0;
			}
			break;
		case fMODIFY:
			MenuModifyZone(zoneList, zone);
			break;
		case fDATA_STRUCTURE1:
			MenuDistributionCenters(zone->zone->distributionCenters);
			break;
		case 4:
			CLEARSCREEN;
			carMostSellBrandName = BestSellingCarBrandInZone(zone->zone->distributionCenters);
			if (!carMostSellBrandName) { printf("No se ha vendido ningun auto tadavia.\n\n"); }
			else {
				printf("Nombre de la marca/proveedor con mas autos vendidos: \"%s\"\n\n", carMostSellBrandName);
				carMostSellBrand = FindBrandByName(derco->brands, carMostSellBrandName);
				if (carMostSellBrand) { PrintBrand(carMostSellBrand->brand); }	
				else { printf("Esta marca ya no esta asociada con Derco.\n\n"); }
			}
			printf("Presione enter para devolverse: ");
			CLEARINPUTBUFFER;
			break;
		case 5:
			printf("0. Vehiculo\n1. Pieza de repuesto\n2. Accesorio\n3. Todos los tipos\n\n");
			type = EnterInteger("Ingrese el tipo de producto: ");
			if (type == -1 || type > END) {
				printf("Valor invalido."); 
				Sleep(ACTIONTIME);
			} else {
				CLEARSCREEN;
				product = FindBestSellingProductByType(zone->zone->distributionCenters, type, &bestSeller);
				if (!product) {
					printf("No se ha encontrado un producto de ese tipo.\n\n");
				} else {
					PrintProductSharedData(product);
				}
				printf("Presione enter para devolverse: ");
				CLEARINPUTBUFFER;
			}
			break;
		case 6:
			CLEARSCREEN;
			PrintCarsSoldInZone(zone->zone->distributionCenters);
			printf("Presione enter para devolverse: ");
			CLEARINPUTBUFFER;
			break;
		default: printf("Valor invalido."); Sleep(ACTIONTIME); break;
		}
	} while (input);

	return 0;
}

int MenuModifyZone(struct ZoneList* zoneList, struct ZoneNode* zone) {
	int input;
	char* name;

	if (!zone) {
		zone = FindZoneByName(zoneList, EnterText("Ingrese el nombre de la zona a modificar: "));
		if (!zone) { return 1; }
	}

	do {
		CLEARSCREEN;
		printf("--------------------------------------------------\n\n");
		PrintZone(zone->zone);
		printf("--------------------------------------------------\n\n");
		printf("MENU MODIFICAR ZONA:\n\n0. Atras.\n1. Modificar nombre de la zona.\n2. Modificar numero de habitantes de la zona.\n3. Modificar edad promedio de habitantes de la zona.\n\n");
		input = EnterInteger("Ingrese un numero: ");
		if (input == -1) { continue; }

		switch (input) {
		case 0: break;
		case 1:
			name = EnterText("Ingrese el nombre de la zona: ");
			if (!name) { break; }
			if (FindZoneByName(zoneList, name)) {
				printf("No se puede repetir el nombre.\n\n");
				Sleep(ACTIONTIME);
			}
			else { ChangeZoneName(zone, name); }
			break;
		case 2: ChangeZonesNumberOfHabitants(zone, EnterInteger("Ingrese el numero de habitantes de la zona: ")); break;
		case 3: ChangeZonesHabitantsAverageAge(zone, EnterFloatingPoint("Ingrese la edad promedio de los habitantes de la zona: ")); break;
		default: printf("Valor invalido."); Sleep(ACTIONTIME); break;
		}

	} while (input);

	return 0;
}

//-----------------------------------CENTROS DE DISTRIBUCION-----------------------------------

void MenuDistributionCenters(struct DistributionCenterNode* distributionCenters) {
	enum Menu input;

	do {
		CLEARSCREEN;
		printf("MENU PRINCIPAL CENTROS DE DISTRIBUCION:\n\n0. Atras.\n1. Agregar centro de distribucion.\n2. Eliminiar centro de distribucion.\n3. Buscar centro de distribucion.\n4. Modificar centro de distribucion.\n5. Mostrar centros de distribucion.\n\n");
		input = EnterInteger("Ingrese un numero: ");
		if (input == -1) { continue; }

		if (0 < input && input < 6) { CLEARSCREEN; }
		if (0 < input && input < 5) { PrintDistributionCenterIDs(distributionCenters); }

		switch (input) {
		case 0: break;
		case mADD:
			if (MenuAddDistributionCenter(distributionCenters)) {
				printf("El centro de distribucion no se ha podido agregar correctamente.");
			} else { printf("El centro de distribucion se ha agregado exitosamente."); }
			Sleep(ACTIONTIME);
			break;
		case mDELETE:
			if (MenuDeleteDistributionCenter(distributionCenters)) {
				printf("El centro de distribucion no se ha podido eliminar correctamente.");
			} else { printf("El centro de distribucion se ha eliminado exitosamente."); }
			Sleep(ACTIONTIME);
			break;
		case mFIND:
			if (MenuFindDistributionCenter(distributionCenters)) {
				printf("No se ha podido encontrar el centro de distribucion.");
				Sleep(ACTIONTIME);
			}
			break;
		case mMODIFY1:
			if (MenuModifyDistributionCenter(distributionCenters, NULL)) {
				printf("No se ha podido encontrar el centro de distribucion.");
				Sleep(ACTIONTIME);
			}
			break;
		case mPRINT:
			PrintDistributionCenters(distributionCenters);
			printf("Presione enter para devolverse: ");
			CLEARINPUTBUFFER;
			break;
		default: printf("Valor invalido."); Sleep(ACTIONTIME); break;
		}
	} while (input);
}

int MenuAddDistributionCenter(struct DistributionCenterNode* distributionCenters) {
	int ID;
	char* address;
	int numberOfEmployes;
	float storeMaxCapacity;

	ID = EnterInteger("Ingrese la ID del centro de distribucion: ");
	if (ID == -1) { return 1; }
	if (FindDistributionCenterByID(distributionCenters, ID)) {
		printf("No se puede repetir el ID.\n\n");
		Sleep(ACTIONTIME);
		return 1;
	}
	address = EnterText("Ingrese la direccion del centro de distribucion: ");
	if (!address) { return 1; }
	numberOfEmployes = EnterInteger("Ingrese el numero de empleados del centro de distribucion: ");
	if (numberOfEmployes == -1) { return 1; }
	storeMaxCapacity = EnterFloatingPoint("Ingrese la capacidad maxima del almacen (m^3): ");
	if (storeMaxCapacity == -1) { return 1; }

	return AddDistributionCenter(distributionCenters, CreateDistributionCenter(ID, address, numberOfEmployes, storeMaxCapacity));
}

int MenuDeleteDistributionCenter(struct DistributionCenterNode* distributionCenters) {
	return DeleteDistributionCenter(distributionCenters, FindDistributionCenterByID(distributionCenters, EnterInteger("Ingrese el ID del centro de distribucion que desea eliminar: ")));
}

int MenuFindDistributionCenter(struct DistributionCenterNode* distributionCenters) {
	enum Find input;
	struct DistributionCenterNode* distributionCenter;

	distributionCenter = FindDistributionCenterByID(distributionCenters, EnterInteger("Ingrese el ID del centro de distribucion que quiere buscar: "));
	if (!distributionCenter) { return 1; }

	actualDistributionCenter = distributionCenter;

	do {
		CLEARSCREEN;
		printf("--------------------------------------------------\n\n");
		PrintDistributionCenter(distributionCenter->distributionCenter);
		printf("--------------------------------------------------\n\n");
		printf("MENU CENTRO DE DISTRIBUCION:\n\n0. Atras.\n1. Eliminiar centro de distribucion.\n2. Modificar datos del centro de distribucion.\n3. Modificar los clientes del centro de distribucion.\n4. Modificar el almacen del centro de distribucion.\n\n");
		input = EnterInteger("Ingrese un numero: ");
		if (input == -1) { continue; }

		switch (input) {
		case 0: break;
		case fDELETE:
			if (DeleteDistributionCenter(distributionCenters, distributionCenter)) {
				printf("El centro de distribucion no se ha podido eliminar correctamente.");
				Sleep(ACTIONTIME);
			}
			else {
				printf("El centro de distribucion se ha eliminado exitosamente.");
				Sleep(ACTIONTIME);
				return 0;
			}
			break;
		case fMODIFY:
			MenuModifyDistributionCenter(distributionCenters, distributionCenter);
			break;
		case fDATA_STRUCTURE1:
			MenuCustomers(&distributionCenter->distributionCenter->customers);
			break;
		case fDATA_STRUCTURE2:
			MenuStore(distributionCenter->distributionCenter->store);
			break;
		default: printf("Valor invalido."); Sleep(ACTIONTIME); break;
		}
	} while (input);

	return 0;
}

int MenuModifyDistributionCenter(struct DistributionCenterNode* distributionCenters, struct DistributionCenterNode* distributionCenter) {
	int input;

	if (!distributionCenter) {
		distributionCenter = FindDistributionCenterByID(distributionCenters, EnterInteger("Ingrese el ID del centro de distribucion a modificar: "));
		if (!distributionCenter) { return 1; }
	}

	do {
		CLEARSCREEN;
		printf("--------------------------------------------------\n\n");
		PrintDistributionCenter(distributionCenter->distributionCenter);
		printf("--------------------------------------------------\n\n");
		printf("MENU MODIFICAR CENTRO DE DISTRIBUCION:\n\n0. Atras.\n1. Modificar direccion del centro de distribucion.\n2. Modificar numero de empleados del centro de distribucion.\n\n");
		input = EnterInteger("Ingrese un numero: ");
		if (input == -1) { continue; }

		switch (input) {
		case 0: break;
		case 1: ChangeDistributionCenterAddress(distributionCenter, EnterText("Ingrese la direccion del centro de distribucion: ")); break;
		case 2: ChangeDistributionCenterNumberOfEmployes(distributionCenter, EnterInteger("Ingrese el numero de empleados del centro de distribucion: ")); break;
		default: printf("Valor invalido."); Sleep(ACTIONTIME); break;
		}
	} while (input);

	return 0;
}

//-----------------------------------CLIENTES-----------------------------------

void MenuCustomers(struct CustomerNode** customers) {
	enum Menu input;

	do {
		CLEARSCREEN;
		printf("MENU PRINCIPAL CLIENTES:\n\n0. Atras.\n1. Agregar cliente.\n2. Eliminiar cliente.\n3. Buscar cliente.\n4. Modificar cliente.\n5. Mostrar clientes.\n\n");
		input = EnterInteger("Ingrese un numero: ");
		if (input == -1) { continue; }

		if (0 < input && input < 6) { CLEARSCREEN; }
		if (0 < input && input < 5) { PrintCustomerIDs(*customers); }

		switch (input) {
		case 0: break;
		case mADD:
			if (MenuAddCustomer(customers)) {
				printf("El cliente no se ha podido agregar correctamente.");
			}
			else { printf("El cliente se ha agregado exitosamente."); }
			Sleep(ACTIONTIME);
			break;
		case mDELETE:
			if (MenuDeleteCustomer(customers)) {
				printf("El cliente no se ha podido eliminar correctamente.");
			}
			else { printf("El cliente se ha eliminado exitosamente."); }
			Sleep(ACTIONTIME);
			break;
		case mFIND:
			if (MenuFindCustomer(customers)) {
				printf("No se ha podido encontrar el cliente.");
				Sleep(ACTIONTIME);
			}
			break;
		case mMODIFY1:
			if (MenuModifyCustomer(customers, NULL)) {
				printf("No se ha podido encontrar el cliente.");
				Sleep(ACTIONTIME);
			}
			break;
		case mPRINT:
			PrintCustomers(*customers);
			printf("Presione enter para devolverse: ");
			CLEARINPUTBUFFER;
			break;
		default: printf("Valor invalido."); Sleep(ACTIONTIME); break;
		}
	} while (input);
}

int MenuAddCustomer(struct CustomerNode** customers) {
	int ID;
	char* name;
	int age;
	int maxPurchases;

	ID = EnterInteger("Ingrese el ID del cliente: ");
	if (ID == -1) { return 1; }
	if (FindCustomerByID(*customers, ID)) {
		printf("No se puede repetir el ID.\n\n");
		Sleep(ACTIONTIME);
		return 1;
	}
	name = EnterText("Ingrese el nombre del cliente: ");
	if (!name) { return 1; }
	age = EnterInteger("Ingrese la edad del cliente: ");
	if (age == -1) { return 1; }
	maxPurchases = EnterInteger("Ingrese la cantidad maxima de compras que puede realizar el cliente: ");
	if (maxPurchases == -1) { return 1; }

	return AddCustomer(customers, CreateCustomer(ID, name, age, maxPurchases));
}

int MenuDeleteCustomer(struct CustomerNode** customers) {
	return DeleteCustomer(customers, EnterInteger("Ingrese el ID del cliente que desea eliminar: "));
}

int MenuFindCustomer(struct CustomerNode** customers) {
	enum Find input;
	struct CustomerNode* customer;

	customer = FindCustomerByID(*customers, EnterInteger("Ingrese el ID del cliente que quiere buscar: "));
	if (!customer) { return 1; }

	do {
		CLEARSCREEN;
		printf("--------------------------------------------------\n\n");
		PrintCustomer(customer->customer);
		printf("--------------------------------------------------\n\n");
		printf("MENU CLIENTE:\n\n0. Atras.\n1. Eliminiar cliente.\n2. Modificar datos del cliente.\n3. Modificar las compras del cliente.\n\n");
		input = EnterInteger("Ingrese un numero: ");
		if (input == -1) { continue; }

		switch (input) {
		case 0: break;
		case fDELETE:
			if (DeleteCustomer(customers, customer->customer->ID)) {
				printf("El cliente no se ha podido eliminar correctamente.");
				Sleep(ACTIONTIME);
			}
			else {
				printf("El cliente se ha eliminado exitosamente.");
				Sleep(ACTIONTIME);
				return 0;
			}
			break;
		case fMODIFY:
			MenuModifyCustomer(customers, customer);
			break;
		case fDATA_STRUCTURE1:
			MenuPurchases(customer->customer->purchases);
			break;
		default: printf("Valor invalido."); Sleep(ACTIONTIME); break;
		}
	} while (input);

	return 0;
}

int MenuModifyCustomer(struct CustomerNode** customers, struct CustomerNode* customer) {
	int input;

	if (!customer) {
		customer = FindCustomerByID(*customers, EnterInteger("Ingrese el ID del cliente a modificar: "));
		if (!customer) { return 1; }
	}

	do {
		CLEARSCREEN;
		printf("--------------------------------------------------\n\n");
		PrintCustomer(customer->customer);
		printf("--------------------------------------------------\n\n");
		printf("MENU MODIFICAR CLIENTE:\n\n0. Atras.\n1. Modificar nombre del cliente.\n2. Modificar edad del cliente.\n\n");
		input = EnterInteger("Ingrese un numero: ");
		if (input == -1) { continue; }

		switch (input) {
		case 0: break;
		case 1: ChangeCustomerName(customer, EnterText("Ingrese el nombre del cliente: ")); break;
		case 2: ChangeCustomerAge(customer, EnterInteger("Ingrese la edad del cliente: ")); break;
		default: printf("Valor invalido."); Sleep(ACTIONTIME); break;
		}
	} while (input);

	return 0;
}

//-----------------------------------ALMACEN-----------------------------------

void MenuStore(struct Store* store) {
	int input;
	int maxCapacity;

	do {
		CLEARSCREEN;
		printf("MENU PRINCIPAL ALMACEN:\n\n0. Atras.\n1. Agregar producto.\n2. Eliminiar producto.\n3. Buscar producto.\n4. Modificar producto.\n5. Mostrar almacen.\n6. Modificar capacidad maxima del almacen.\n\n");
		input = EnterInteger("Ingrese un numero: ");
		if (input == -1) { continue; }

		if (0 < input && input < 7) { CLEARSCREEN; }
		if (0 < input && input < 5) { PrintProductIDs(store->products); }

		switch (input) {
		case 0: break;
		case mADD:
			if (MenuAddProductToStore(store)) {
				printf("El producto no se ha podido agregar correctamente.");
			}
			else { printf("El producto se ha agregado exitosamente."); }
			Sleep(ACTIONTIME);
			break;
		case mDELETE:
			if (MenuDeleteStoreProduct(store)) {
				printf("El producto no se ha podido eliminar correctamente.");
			}
			else { printf("El producto se ha eliminado exitosamente."); }
			Sleep(ACTIONTIME);
			break;
		case mFIND:
			if (MenuFindStoreProduct(store)) {
				printf("No se ha podido encontrar el producto.");
				Sleep(ACTIONTIME);
			}
			break;
		case mMODIFY1:
			if (MenuModifyStoreProduct(store, NULL)) {
				printf("No se ha podido encontrar el producto.");
				Sleep(ACTIONTIME);
			}
			break;
		case mPRINT:
			PrintStore(store);
			printf("Presione enter para devolverse: ");
			CLEARINPUTBUFFER;
			break;
		case mSPECIALOPTION:
			CLEARSCREEN;
			printf("---------------------------------------------------------\n\n");
			printf("Actual capacidad maxima del almacen : %.2f\nEspacio utilizado en el almacen: %.2f\n\n", store->cbMetersMaxCapacity, store->cbMetersSpaceUsed);
			printf("---------------------------------------------------------\n\n");
			if (ChangeStoreMaxCapacity(store, EnterFloatingPoint("Ingrese la capacidad maxima del almacen: "))) {
				printf("La capacidad maxima del almacen no se ha podido modificar correctamente.");
			} else { printf("La capacidad maxima del almacen se ha modificado exitosamente."); }
			Sleep(ACTIONTIME);
			break;
		default: printf("Valor invalido."); Sleep(ACTIONTIME); break;
		}
	} while (input);
}

int MenuAddProductToStore(struct Store* store) {
	int productID;
	int amount;
	float price;
	struct Product* product;

	PrintProductIDsInAllBrands();

	productID = EnterInteger("Ingrese el ID del producto a almacenar: ");

	if (FindProductByID(store->products, productID)) {
		printf("El producto ya se encuentra en el almacen\n\n");
		Sleep(ACTIONTIME);
		return 1;
	}

	product = FindProductByIDInAllBrands(productID);
	if (!product || !product->amount) {
		printf("No se ha encontrado el producto.\n\n");
		Sleep(ACTIONTIME);
		return 1;
	}

	CLEARSCREEN;

	printf("--------------------------------------------------\n\n");
	printf("Cantidad de producto en stock de marca/proveedor: %d\n\n", product->amount);
	printf("--------------------------------------------------\n\n");

	amount = EnterInteger("Ingrese la cantidad que quiere almacenar: ");
	if (amount == -1) { return 1; }
	if (amount > product->amount) {
		printf("La cantidad solicitada excede la cantidad presente en el stock de la marca/proveedor.\n\n");
		Sleep(ACTIONTIME);
		return 1;
	}
	if (amount * product->sharedData->cbMetersSpacePerUnit > store->cbMetersMaxCapacity - store->cbMetersSpaceUsed) {
		printf("Al almacen no le queda espacio suficiente para almacenar todos los productos solicitados.\n\n");
		Sleep(ACTIONTIME);
		return 1;
	}

	store->numberOfProducts += amount;
	store->cbMetersSpaceUsed += amount * product->sharedData->cbMetersSpacePerUnit;
	product->amount -= amount;

	price = EnterFloatingPoint("Ingrese el precio que quiere darle al producto: ");
	if (price == -1) { return 1; }

	return AddProduct(&store->products, CreateProduct(product->sharedData, amount, price));
}

int MenuDeleteStoreProduct(struct Store* store) {
	int ID;
	struct ProductNode* product;

	ID = EnterInteger("Ingrese el ID del producto que desea eliminar: ");

	product = FindProductByID(store->products, ID);
	if (!product) { return 1; }

	if (DeleteProduct(&store->products, ID)) { return 1; }

	store->numberOfProducts -= product->product->amount;
	store->cbMetersSpaceUsed -= product->product->amount * product->product->sharedData->cbMetersSpacePerUnit;

	return 0;
}

int MenuFindStoreProduct(struct Store* store) {
	enum Find input;
	struct ProductNode* product;

	product = FindProductByID(store->products, EnterInteger("Ingrese el ID del producto que quiere buscar: "));
	if (!product) { return 1; }

	do {
		CLEARSCREEN;
		printf("--------------------------------------------------\n\n");
		PrintProduct(product->product);
		printf("--------------------------------------------------\n\n");
		printf("MENU PRODUCTO EN ALMACEN:\n\n0. Atras.\n1. Eliminiar producto.\n2. Modificar datos del producto.\n\n");
		input = EnterInteger("Ingrese un numero: ");
		if (input == -1) { continue; }

		switch (input) {
		case 0: break;
		case fDELETE:
			if (DeleteProduct(&store->products, product->product->sharedData->ID)) {
				printf("El producto no se ha podido eliminar correctamente.");
				Sleep(ACTIONTIME);
			}
			else {
				store->numberOfProducts -= product->product->amount;
				store->cbMetersSpaceUsed -= product->product->amount * product->product->sharedData->cbMetersSpacePerUnit;
				printf("El producto se ha eliminado exitosamente.");
				Sleep(ACTIONTIME);
				return 0;
			}
			break;
		case fMODIFY:
			MenuModifyStoreProduct(store, product);
			break;
		default: printf("Valor invalido."); Sleep(ACTIONTIME); break;
		}
	} while (input);

	return 0;
}

int MenuModifyStoreProduct(struct Store* store, struct ProductNode* product) {
	int input;
	int amount;
	struct Product* stockProduct;

	if (!product) {
		product = FindProductByID(store->products, EnterInteger("Ingrese el ID del producto a modificar: "));
		if (!product) { return 1; }
	}

	stockProduct = FindProductByIDInAllBrands(product->product->sharedData->ID);

	do {
		CLEARSCREEN;
		printf("--------------------------------------------------\n\n");
		PrintProduct(product->product);
		printf("--------------------------------------------------\n\n");
		printf("MENU MODIFICAR PRODUCTO EN ALMACEN:\n\n0. Atras.\n1. Modificar cantidad del producto en almacen.\n2. Modificar precio del producto en centro de distribucion.\n\n");
		input = EnterInteger("Ingrese un numero: ");
		if (input == -1) { continue; }

		switch (input) {
		case 0: break;
		case 1:
			amount = EnterInteger("Ingrese la cantidad del producto en almacen: ");
			if (amount == -1) { break; }
			if (amount - product->product->amount > stockProduct->amount) {
				printf("La cantidad solicitada excede la cantidad presente en el stock de la marca/proveedor.\n\n");
				Sleep(ACTIONTIME);
			}
			else if (amount - product->product->amount > store->cbMetersMaxCapacity - store->cbMetersSpaceUsed) {
				printf("No hay suficiente espacio en el almacen.\n");
				Sleep(ACTIONTIME);
			}
			else {
				stockProduct->amount += amount - product->product->amount;
				store->cbMetersSpaceUsed += (amount - product->product->amount) * product->product->sharedData->cbMetersSpacePerUnit;
				store->numberOfProducts += amount - product->product->amount;
				ChangeProductAmount(product, amount);
			}
			break;
		case 2: ChangeProductPrice(product, EnterFloatingPoint("Ingrese el precio del production en almacen: ")); break;
		default: printf("Valor invalido."); Sleep(ACTIONTIME); break;
		}
	} while (input);

	return 0;
}

//-----------------------------------COMPRAS-----------------------------------

void MenuPurchases(struct Purchases* purchases) {
	enum Menu input;

	do {
		CLEARSCREEN;
		printf("MENU PRINCIPAL COMPRAS:\n\n0. Atras.\n1. Agregar compra.\n2. Eliminiar compra.\n3. Buscar compra.\n4. Modificar compra.\n5. Mostrar compras.\n\n");
		input = EnterInteger("Ingrese un numero: ");
		if (input == -1) { continue; }

		if (0 < input && input < 6) { CLEARSCREEN; }

		switch (input) {
		case 0: break;
		case mADD:
			if (MenuAddPurchase(purchases)) {
				printf("La compra no se ha podido agregar correctamente.");
			} else { printf("La compra se ha agregado exitosamente."); }
			Sleep(ACTIONTIME);
			break;
		case mDELETE:
			PrintPurchaseIDs(purchases);
			if (MenuDeletePurchase(purchases)) {
				printf("La compra no se ha podido eliminar correctamente.");
			} else { printf("La compra se ha eliminado exitosamente."); }
			Sleep(ACTIONTIME);
			break;
		case mFIND:
			PrintPurchaseIDs(purchases);
			if (MenuFindPurchase(purchases)) {
				printf("La compra no se ha podido encontrar.");
				Sleep(ACTIONTIME);
			}
			break;
		case mMODIFY1:
			PrintPurchaseIDs(purchases);
			if (MenuModifyPurchase(purchases, NULL)) {
				printf("La compra no se ha podido encontrar.");
				Sleep(ACTIONTIME);
			}
			break;
		case mPRINT:
			PrintPurchases(purchases);
			printf("Presione enter para devolverse: ");
			CLEARINPUTBUFFER;
			break;
		default: printf("Valor invalido."); Sleep(ACTIONTIME); break;
		}
	} while (input);
}

int MenuAddPurchase(struct Purchases* purchases) {
	int ID;
	int productID;
	int amount;
	struct ProductNode* boughtProduct;

	PrintProductIDs(actualDistributionCenter->distributionCenter->store->products);

	productID = EnterInteger("Ingrese el ID del producto que quiere comprar: ");

	boughtProduct = FindProductByID(actualDistributionCenter->distributionCenter->store->products, productID);
	if (!boughtProduct) {
		printf("No se ha encontrado el producto.\n\n");
		Sleep(ACTIONTIME);
		return 1;
	}

	CLEARSCREEN;
	PrintPurchaseIDs(purchases);

	ID = EnterInteger("Ingrese el numero de boleta de la compra: ");
	if (ID == -1) { return 1; }
	if (FindPurchaseByID(purchases, ID)) {
		printf("No se puede repetir el numero de boleta.\n\n");
		Sleep(ACTIONTIME);
		return 1;
	}

	printf("--------------------------------------------------\n\n");
	printf("Cantidad del producto en almacen: %d\n\n", boughtProduct->product->amount);
	printf("--------------------------------------------------\n\n");

	amount = EnterInteger("Ingrese la cantidad de productos de la compra: ");
	if (amount == -1) { return 1; }

	if (boughtProduct->product->amount < amount) {
		printf("El almacen tiene menos productos de los que se desea comprar.\n\n");
		Sleep(ACTIONTIME);
		return 1;
	}

	if (AddPurchase(purchases, CreatePurchase(ID, boughtProduct->product->sharedData, amount, boughtProduct->product->price))) { return 1; }

	boughtProduct->product->amount -= amount;
	actualDistributionCenter->distributionCenter->store->numberOfProducts -= amount;
	actualDistributionCenter->distributionCenter->store->cbMetersSpaceUsed -= amount * boughtProduct->product->sharedData->cbMetersSpacePerUnit;

	return 0;
}

int MenuDeletePurchase(struct Purchases* purchases) {
	return DeletePurchase(purchases, FindPurchaseByID(purchases, EnterInteger("Ingrese el numero de boleta de la compra que desea eliminar: ")));
}

int MenuFindPurchase(struct Purchases* purchases) {
	enum Find input;
	struct Purchase* purchase;

	purchase = FindPurchaseByID(purchases, EnterInteger("Ingrese el numero de boleta de la compra que quiere buscar: "));
	if (!purchase) { return 1; }

	do {
		CLEARSCREEN;
		printf("--------------------------------------------------\n\n");
		PrintPurchase(purchase);
		printf("--------------------------------------------------\n\n");
		printf("MENU COMPRA:\n\n0. Atras.\n1. Eliminiar compra.\n2. Modificar datos de la compra.\n\n");
		input = EnterInteger("Ingrese un numero: ");
		if (input == -1) { continue; }

		switch (input) {
		case 0: break;
		case fDELETE:
			if (DeletePurchase(purchases, purchase)) {
				printf("La compra no se ha podido eliminar correctamente.");
				Sleep(ACTIONTIME);
			}
			else {
				printf("La compra se ha eliminado exitosamente.");
				Sleep(ACTIONTIME);
				return 0;
			}
			break;
		case fMODIFY:
			MenuModifyPurchase(purchases, purchase);
			break;
		default: printf("Valor invalido."); Sleep(ACTIONTIME); break;
		}
	} while (input);

	return 0;
}

int MenuModifyPurchase(struct Purchases* purchases, struct Purchase* purchase) {
	int input;
	int amount;
	struct ProductNode* product;

	if (!purchase) {
		purchase = FindPurchaseByID(purchases, EnterInteger("Ingrese el numero de boleta de la compra a modificar: "));
		if (!purchase) { return 1; }
	}

	do {
		CLEARSCREEN;
		printf("--------------------------------------------------\n\n");
		PrintPurchase(purchase);
		printf("--------------------------------------------------\n\n");
		printf("MENU MODIFICAR COMPRA:\n\n0. Atras.\n1. Modificar el producto comprado.\n2. Modificar la cantidad de la compra.\n3. Modificar el precio de compra.\n\n");
		input = EnterInteger("Ingrese un numero: ");
		if (input == -1) { continue; }

		switch (input) {
		case 0: break;
		case 1:
			product = FindProductByID(actualDistributionCenter->distributionCenter->store->products, EnterInteger("Ingrese el numero de boleta del producto comprado: "));
			if (!product) {
				printf("El producto no se ha podido encontrar.\n\n");
				Sleep(ACTIONTIME);
			}
			else { ChangePurchaseProductSharedData(purchase, product->product->sharedData); }
			break;
		case 2:
			amount = EnterInteger("Ingrese la cantidad de la compra: ");
			if (amount != -1) {
				purchases->numberOfPurchaseProducts += amount - purchase->amount;
				ChangePurchaseAmount(purchase, amount);
			}
			break;
		case 3: ChangePurchaseBoughtPrice(purchase, EnterFloatingPoint("Ingrese el precio de la compra: ")); break;
		default: printf("Valor invalido."); Sleep(ACTIONTIME); break;
		}
	} while (input);

	return 0;
}

//-----------------------------------MARCAS/PROVEEDORES-----------------------------------

void MenuBrands(struct BrandNode** brands) {
	enum Menu input;

	do {
		CLEARSCREEN;
		printf("MENU PRINCIPAL MARCAS/PROVEEDORES:\n\n0. Atras\n1. Agregar marca/proveedor\n2. Eliminiar marca/proveedor\n3. Buscar marca/proveedor\n4. Modificar marca/proveedor\n5. Mostrar marcas/proveedores.\n\n");
		input = EnterInteger("Ingrese un numero: ");
		if (input == -1) { continue; }

		if (0 < input && input < 6) { CLEARSCREEN; }
		if (0 < input && input < 5) { PrintBrandNames(*brands); }

		switch (input) {
		case 0: break;
		case mADD:
			if (MenuAddBrand(brands)) {
				printf("La marca/proveedor no se ha podido agregar correctamente.");
			} else { printf("La marca/proveedor se ha agregado exitosamente."); }
			Sleep(ACTIONTIME);
			break;
		case mDELETE:
			if (MenuDeleteBrand(brands)) {
				printf("La marca/proveedor no se ha podido eliminar correctamente.");
			} else { printf("La marca/proveedor se ha eliminado exitosamente."); }
			Sleep(ACTIONTIME);
			break;
		case mFIND:
			if (MenuFindBrand(brands)) {
				printf("La marca/proveedor no se ha podido encontrar.");
				Sleep(ACTIONTIME);
			}
			break;
		case mMODIFY1:
			if (MenuModifyBrand(brands, NULL)) {
				printf("La marca/proveedor no se ha podido encontrar.");
				Sleep(ACTIONTIME);
			}
			break;
		case mPRINT:
			PrintBrands(*brands);
			printf("Presione enter para devolverse: ");
			CLEARINPUTBUFFER;
			break;
		default: printf("Valor invalido."); Sleep(ACTIONTIME); break;
		}
	} while (input);
}

int MenuAddBrand(struct BrandNode** brands) {
	char* name;
	float productTravelTime;
	float status;

	name = EnterText("Ingrese el nombre de la marca/proveedor: ");
	if (!name) { return 1; }
	if (FindBrandByName(*brands, name)) {
		printf("No se puede repetir el nombre.\n\n");
		Sleep(ACTIONTIME);
		return 1;
	}
	productTravelTime = EnterFloatingPoint("Ingrese el tiempo de desplazamiento de mercancias (dias) de la marca/proveedor: ");
	if (productTravelTime == -1) { return 1; }
	status = EnterFloatingPoint("Ingrese la valoracion (0.0 - 5.0) de la marca/proveedor: ");
	if (status == -1) { return 1; }
	if (status > 5) {
		printf("Valor invalido\n\n");
		Sleep(ACTIONTIME);
		return 1;
	}

	return AddBrand(brands, CreateBrand(name, productTravelTime, status));
}

int MenuDeleteBrand(struct BrandNode** brands) {
	return DeleteBrand(brands, FindBrandByName(*brands, EnterText("Ingrese el nombre de la marca/proveedor que desea eliminar: ")));
}

int MenuFindBrand(struct BrandNode** brands) {
	enum Find input;
	struct BrandNode* brand;

	brand = FindBrandByName(*brands, EnterText("Ingrese el nombre de la marca/proveedor que quieres buscar: "));
	if (!brand) { return 1; }

	actualBrand = brand;

	do {
		CLEARSCREEN;
		printf("--------------------------------------------------\n\n");
		PrintBrand(brand->brand);
		printf("--------------------------------------------------\n\n");
		printf("MENU MARCA/PROVEEDOR:\n\n0. Atras.\n1. Eliminiar marca/proveedor.\n2. Modificar datos de la marca/proveedor.\n3. Modificar stock de la marca/proveedor.\n\n");
		input = EnterInteger("Ingrese un numero: ");
		if (input == -1) { continue; }

		switch (input) {
		case 0: break;
		case fDELETE:
			if (DeleteBrand(brands, brand)) {
				printf("La marca/proveedor no se ha podido eliminar correctamente.");
				Sleep(ACTIONTIME);
			}
			else {
				printf("La marca/proveedor se ha eliminado exitosamente.");
				Sleep(ACTIONTIME);
				return 0;
			}
			break;
		case fMODIFY:
			MenuModifyBrand(brands, brand);
			break;
		case fDATA_STRUCTURE1:
			MenuStock(&brand->brand->stock);
			break;
		default: printf("Valor invalido."); Sleep(ACTIONTIME); break;
		}
	} while (input);

	return 0;
}

int MenuModifyBrand(struct BrandNode** brands, struct BrandNode* brand) {
	int input;
	float status;
	char* name = NULL;

	if (!brand) {
		brand = FindBrandByName(*brands, EnterText("Ingrese el nombre de la marca/proveedor: "));
		if (!brand) { return 1; }
	}

	do {
		CLEARSCREEN;
		printf("--------------------------------------------------\n\n");
		PrintBrand(brand->brand);
		printf("--------------------------------------------------\n\n");
		printf("MENU MODIFICAR MARCA/PROVEEDOR:\n\n0. Atras.\n1. Modificar el nombre de la marca/proveedor.\n2. Modificar el tiempo de desplazamiento de mercancias de la marca/proveedor.\n3. Modificar la valoracion de la marca/proveedor.\n\n");
		input = EnterInteger("Ingrese un numero: ");
		if (input == -1) { continue; }

		switch (input) {
		case 0: break;
		case 1:
			name = EnterText("Ingrese el nombre de la marca/proveedor: ");
			if (!name) { break; }
			if (FindBrandByName(*brands, name)) {
				printf("No se puede repetir el nombre.\n\n");
				Sleep(ACTIONTIME);
			}
			else { ChangeBrandName(brand, name); }
			break;
		case 2: ChangeBrandProductTravelTime(brand, EnterFloatingPoint("Ingrese el tiempo de desplazamiento de mercancias (dias) de la marca/proveedor: ")); break;
		case 3:
			status = EnterFloatingPoint("Ingrese la valoracion (0.0 - 5.0) de la marca/proveedor: ");
			if (status > 5) {
				printf("\nValor invalido\n");
				Sleep(ACTIONTIME);
			}
			else { ChangeBrandStatus(brand, status); }
			break;
		default: printf("Valor invalido."); Sleep(ACTIONTIME); break;
		}
	} while (input);

	return 0;
}

//-----------------------------------STOCK-----------------------------------

void MenuStock(struct ProductNode** stock) {
	enum Menu input;

	do {
		CLEARSCREEN;
		printf("MENU PRINCIPAL STOCK:\n\n0. Atras\n1. Agregar producto\n2. Eliminiar producto\n3. Buscar producto\n4. Modificar producto\n5. Mostrar stock.\n\n");
		input = EnterInteger("Ingrese un numero: ");
		if (input == -1) { continue; }

		if (0 < input && input < 6) { CLEARSCREEN; }
		if (1 < input && input < 5) { PrintProductIDs(*stock); }

		switch (input) {
		case 0: break;
		case mADD:
			if (MenuAddProductToStock(stock)) {
				printf("El producto no se ha podido agregar correctamente.");
			} else { printf("El producto se ha agregado exitosamente."); }
			Sleep(ACTIONTIME);
			break;
		case mDELETE:
			if (MenuDeleteStockProduct(stock)) {
				printf("El producto no se ha podido eliminar correctamente.");
			} else { printf("El producto se ha eliminado exitosamente."); }
			Sleep(ACTIONTIME);
			break;
		case mFIND:
			if (MenuFindStockProduct(stock)) {
				printf("El producto no se ha podido encontrar.");
				Sleep(ACTIONTIME);
			}
			break;
		case mMODIFY1:
			if (MenuModifyStockProduct(stock, NULL)) {
				printf("El producto no se ha podido encontrar.");
				Sleep(ACTIONTIME);
			}
			break;
		case mPRINT:
			PrintProducts(*stock);
			printf("Presione enter para devolverse: ");
			CLEARINPUTBUFFER;
			break;
		default: printf("Valor invalido."); Sleep(ACTIONTIME); break;
		}
	} while (input);
}

int MenuAddProductToStock(struct ProductNode** products) {
	int ID;
	enum ProductType type;
	char* name = NULL;
	char* description = NULL;
	float price;
	float spacePerUnit;
	int amount;

	PrintProductIDsInAllBrands();

	ID = EnterInteger("Ingrese el ID del producto que quiere agregar: ");
	if (ID == -1) { return 1; }
	if (FindProductByIDInAllBrands(ID)) {
		printf("No se puede repetir el ID entre marcas/proveedores.\n\n");
		Sleep(ACTIONTIME);
		return 1;
	}
	printf("0. Vehiculo\n1. Pieza de repuesto\n2. Accesorio\n\n");
	type = EnterInteger("Ingrese el numero del tipo producto que quiere agregar: ");
	if (type >= END) { return 1; }
	name = EnterText("Ingrese el nombre del producto que quiere agregar: ");
	if (!name) { return 1; }
	description = EnterText("Ingrese la descripcion del producto que quiere agregar: ");
	if (!description) { return 1; }
	spacePerUnit = EnterFloatingPoint("Ingrese el espacio por unidad del producto (m^3) que quiere agregar: ");
	if (spacePerUnit== -1) { return 1; }
	amount = EnterInteger("Ingrese la cantidad del producto que quiere agregar: ");
	if (amount == -1) { return 1; }
	price = EnterFloatingPoint("Ingrese el precio del producto que quiere agregar: ");
	if (price == -1) { return 1; }

	return AddProduct(products, CreateProduct(CreateProductSharedData(ID, type, name, description, actualBrand->brand->name, spacePerUnit), amount, price));
}

int MenuDeleteStockProduct(struct ProductNode** stock) {
	return DeleteProduct(stock, EnterInteger("Ingrese el ID del producto que desea eliminar: "));
}

int MenuFindStockProduct(struct ProductNode** stock) {
	enum Find input;
	struct ProductNode* product;

	product = FindProductByID(*stock, EnterInteger("Ingrese el ID del producto que quiere buscar: "));
	if (!product) { return 1; }

	do {
		CLEARSCREEN;
		printf("--------------------------------------------------\n\n");
		PrintProduct(product->product);
		printf("--------------------------------------------------\n\n");
		printf("MENU PRODUCTO EN STOCK:\n\n0. Atras.\n1. Eliminiar producto.\n2. Modificar datos del producto.\n\n");
		input = EnterInteger("Ingrese un numero: ");
		if (input == -1) { continue; }

		switch (input) {
		case 0: break;
		case fDELETE:
			if (DeleteProduct(stock, product->product->sharedData->ID)) {
				printf("El producto no se ha podido eliminar correctamente.");
				Sleep(ACTIONTIME);
			}
			else {
				printf("El producto se ha eliminado exitosamente.");
				Sleep(ACTIONTIME);
				return 0;
			}
			break;
		case fMODIFY:
			MenuModifyStockProduct(stock, product);
			break;
		default: printf("Valor invalido."); Sleep(ACTIONTIME); break;
		}
	} while (input);

	return 0;
}

int MenuModifyStockProduct(struct ProductNode** products, struct ProductNode* product) {
	int input;
	enum ProductType type;

	if (!product) {
		product = FindProductByID(*products, EnterInteger("Ingrese el ID del cliente a modificar: "));
		if (!product) { return 1; }
	}

	do {
		CLEARSCREEN;
		printf("--------------------------------------------------\n\n");
		PrintProduct(product->product);
		printf("--------------------------------------------------\n\n");
		printf("MENU MODIFICAR PRODUCTO EN STOCK:\n\n0. Atras.\n1. Modificar tipo de producto.\n2. Modificar nombre del producto.\n3. Modificar descripcion del producto.\n4. Modificar espacio por unidad del producto.\n5. Modificar la cantidad del producto.\n6. Modificar precio del producto.\n");

		if (product->product->sharedData->type == CAR) {
			printf("7. Modificar recalls del vehiculo.\n");
		}
		printf("\n");

		input = EnterInteger("Ingrese un numero: ");
		if (input == -1) { continue; }

		switch (input) {
		case 0: break;
		case 1:
			printf("0. Vehiculo\n1. Pieza de repuesto\n2. Accesorio\n\n");
			type = EnterInteger("Ingrese el numero del tipo producto que quiere agregar: ");
			if (type >= END) {
				printf("Valor invalido.\n\n");
				Sleep(ACTIONTIME);
			} else { ChangeProductType(product, type); }
			break;
		case 2: ChangeProductName(product, EnterText("Ingrese el nombre del producto: ")); break;
		case 3: ChangeProductDescription(product, EnterText("Ingrese la descripcion del producto: ")); break;
		case 4: ChangeProductSpacePerUnit(product, EnterFloatingPoint("Ingrese el espacio por unidad del producto: ")); break;
		case 5: ChangeProductAmount(product, EnterInteger("Ingrese la cantidad del producto: ")); break;
		case 6: ChangeProductPrice(product, EnterFloatingPoint("Ingrese el precio del producto: ")); break;
		}

		if (product->product->sharedData->type == CAR && input == 7) {
			ChangeProductRecalls(product, EnterInteger("Ingrese recall del auto: "));
		}
		else if ((input > 7 && product->product->sharedData->type == CAR) || (input > 6 && product->product->sharedData->type != CAR)) {
			printf("Valor invalido.");
			Sleep(ACTIONTIME);
		}

	} while (input);

	return 0;
}

//-----------------------------------FUNCIONES ENUNCIADO-----------------------------------

// Funcion enunciado: Encontrar consumidor que mas ha gastado en los productos de Derco.
struct Customer* CustomerThatHasSpendTheMost() {
	if (!derco->zoneList->zones) { return NULL; }

	struct ZoneNode* zoneTraverse = derco->zoneList->zones;
	struct DistributionCenterNode* distributionCenterTraverse = NULL;
	struct Customer* customer = NULL;
	float moneySpend = -1;
	
	do {
		distributionCenterTraverse = zoneTraverse->zone->distributionCenters->next;
		while (distributionCenterTraverse) {
			CustomerSpendTheMostTraverse(distributionCenterTraverse->distributionCenter->customers, &customer, &moneySpend);
			distributionCenterTraverse = distributionCenterTraverse->next;
		}
		zoneTraverse = zoneTraverse->next;
	} while (zoneTraverse != derco->zoneList->zones);

	return customer;
}

void CustomerSpendTheMostTraverse(struct CustomerNode* root, struct Customer** customer, float *moneySpend) {
	if (!root) { return; }

	float aux;

	aux = TotalMoneySpend(root->customer->purchases);
	if (*moneySpend < aux) {
		*moneySpend = aux;
		*customer = root->customer;
	}

	CustomerSpendTheMostTraverse(root->left, customer, moneySpend);
	CustomerSpendTheMostTraverse(root->right, customer, moneySpend);
}

float TotalMoneySpend(struct Purchases* purchases) {
	int i;
	float acumulator = 0;

	for (i = 0; i < purchases->indexFirstEmpty; i++) {
		acumulator += purchases->purchaseArray[i]->amount * purchases->purchaseArray[i]->boughtPrice;
	}

	return acumulator;
}

//Funcion enunciado: Mostrar autos vendidos por zona
void PrintCarsSoldInZone(struct DistributionCenterNode* distributionCenters) {
	struct ProductIDNode* productIDs = NULL;
	struct DistributionCenterNode* distributionCenterTraverse = distributionCenterTraverse = distributionCenters->next;

	while (distributionCenterTraverse) {
		CustomerTraversePrintCars(distributionCenterTraverse->distributionCenter->customers, &productIDs);
		distributionCenterTraverse = distributionCenterTraverse->next;
	}
}

void CustomerTraversePrintCars(struct CustomerNode* root, struct ProductIDNode** productIDs) {
	if (!root) { return; }

	PrintProductBought(root->customer->purchases, productIDs);
	CustomerTraversePrintCars(root->left, productIDs);
	CustomerTraversePrintCars(root->right, productIDs);
}

void PrintProductBought(struct Purchases* purchases, struct ProductIDNode** productIDs) {
	int i;

	for (i = 0; i < purchases->indexFirstEmpty; i++) {
		if (FindProductIDInList(*productIDs, purchases->purchaseArray[i]->productSharedData)) {
			*productIDs = AddProductIDNode(*productIDs, purchases->purchaseArray[i]->productSharedData);
			PrintProductSharedData(purchases->purchaseArray[i]->productSharedData);
		}
	}
}

// Funcion enunciado: Encontrar la marcar/proveedor de autos que se han vendido mas por zona.
char* BestSellingCarBrandInZone(struct DistributionCenterNode* distributionCenters) {
	struct BrandNameNode *brandNames = NULL, *brandNameTraverse = NULL;
	struct DistributionCenterNode *distributionCenterTraverse;
	char* carMostSellBrandName = NULL;
	int carBestSeller = -1, carSales;
	
	brandNames = CreateCarBrandNameList(distributionCenters, brandNames);

	brandNameTraverse = brandNames;

	while (brandNameTraverse) {
		distributionCenterTraverse = distributionCenters->next;
		carSales = 0;
		while (distributionCenterTraverse) {
			CustomerCarMostSellBrandTraverse(distributionCenterTraverse->distributionCenter->customers, brandNameTraverse->brandName, &carSales);
			distributionCenterTraverse = distributionCenterTraverse->next;
		}
		if (carBestSeller < carSales) {
			carBestSeller = carSales;
			carMostSellBrandName = brandNameTraverse->brandName;
		}
		brandNameTraverse = brandNameTraverse->next;
	}

	return carMostSellBrandName;
}

void CustomerCarMostSellBrandTraverse(struct CustomerNode* root, char* brandName, int* carSales) {
	if (!root) { return; }

	*carSales += CountCarSalesOfBrand(root->customer->purchases, brandName);
	CustomerCarMostSellBrandTraverse(root->left, brandName, carSales);
	CustomerCarMostSellBrandTraverse(root->right, brandName, carSales);
}

int CountCarSalesOfBrand(struct Purchases* purchases, char* brandName) {
	int i, acumulator = 0;

	for (i = 0; i < purchases->indexFirstEmpty; i++) {
		if (purchases->purchaseArray[i]->productSharedData->type == CAR && !strcmp(*purchases->purchaseArray[i]->productSharedData->brandName, brandName)) { 
			acumulator += purchases->purchaseArray[i]->amount;
		}
	}

	return acumulator;
}

struct BrandNameNode* CreateCarBrandNameList(struct DistributionCenterNode* distributionCenters, struct BrandNameNode* brandNames) {
	struct DistributionCenterNode* distributionCenterTraverse = distributionCenters->next;
	
	while (distributionCenterTraverse) {
		CustomerTraverseCarBrandNames(distributionCenterTraverse->distributionCenter->customers, &brandNames);
		distributionCenterTraverse = distributionCenterTraverse->next;
	}

	return brandNames;
}

void CustomerTraverseCarBrandNames(struct CustomerNode* root, struct BrandNameNode** brandNames) {
	if (!root) { return; }

	*brandNames = AddNewCarBrandNames(root->customer->purchases, *brandNames);
	CustomerTraverseCarBrandNames(root->left, brandNames);
	CustomerTraverseCarBrandNames(root->right, brandNames);
}

struct BrandNameNode* AddNewCarBrandNames(struct Purchases* purchases, struct BrandNameNode* brandNames) {
	int i;

	for (i = 0; i < purchases->indexFirstEmpty; i++) {
		if (purchases->purchaseArray[i]->productSharedData->type == CAR && FindBrandNameInList(brandNames, *purchases->purchaseArray[i]->productSharedData->brandName)) {
			brandNames = AddBrandNameNode(brandNames, *purchases->purchaseArray[i]->productSharedData->brandName);
		}
	}

	return brandNames;
}

int FindBrandNameInList(struct BrandNameNode* brandNames, char* name) {
	struct BrandNameNode* traverse = brandNames;

	while (traverse) {
		if (!strcmp(traverse->brandName, name)) { return 0; }
		traverse = traverse->next;
	}

	return 1;
}

struct BrandNameNode* AddBrandNameNode(struct BrandNameNode* brandNames, char* brandName) {
	struct BrandNameNode* newNode;
	newNode = (struct BrandNameNode*)malloc(sizeof(struct BrandNameNode));

	if (!newNode) { return NULL; }

	newNode->brandName = brandName;
	newNode->next = brandNames;

	return newNode;
}

// Funcion enunciado: Auto con mas recalls
struct Product* FindCarWithMoreRecalls() {
	if (!derco->brands) { return NULL; }

	struct BrandNode* brandTraverse = derco->brands;
	struct Product* carWithMoreRecalls = NULL;
	int carRecalls = -1;

	do {
		ProductTraverseCarWithMoreRecalls(brandTraverse->brand->stock, &carWithMoreRecalls, &carRecalls);
		brandTraverse = brandTraverse->next;
	} while (brandTraverse != derco->brands);

	return carWithMoreRecalls;
}

void ProductTraverseCarWithMoreRecalls(struct ProductNode* root, struct Product** carWithMoreRecalls, int* carRecalls) {
	if (!root) { return; }

	ProductTraverseCarWithMoreRecalls(root->left, carWithMoreRecalls, carRecalls);

	if (root->product->sharedData->type == CAR && *carRecalls < root->product->sharedData->recalls) {
		*carRecalls = root->product->sharedData->recalls;
		*carWithMoreRecalls = root->product;
	}

	ProductTraverseCarWithMoreRecalls(root->right, carWithMoreRecalls, carRecalls);
}

// Funcion enunciado: Centro de distribucion con mayor distribucion
struct DistributionCenter* DistributionCenterWithMoreSales() {
	if (!derco->zoneList->zones) { return NULL; }

	struct ZoneNode* zoneTraverse = derco->zoneList->zones;
	struct DistributionCenterNode* distributionCenterTraverse;
	struct DistributionCenter* distributionCenterWithMoreSales = NULL;
	int numberOfSoldProducts, greaterSales = -1;

	do {
		distributionCenterTraverse = zoneTraverse->zone->distributionCenters->next;
		while (distributionCenterTraverse) {
			numberOfSoldProducts = 0;
			CustomerTraverseDistributionCenterWithMoreSales(distributionCenterTraverse->distributionCenter->customers, &numberOfSoldProducts);
			
			if (greaterSales < numberOfSoldProducts) {
				greaterSales = numberOfSoldProducts;
				distributionCenterWithMoreSales = distributionCenterTraverse->distributionCenter;
			}

			distributionCenterTraverse = distributionCenterTraverse->next;
		}
		zoneTraverse = zoneTraverse->next;
	} while (zoneTraverse != derco->zoneList->zones);

	return distributionCenterWithMoreSales;
}

void CustomerTraverseDistributionCenterWithMoreSales(struct CustomerNode* root, int* numberOfSoldProducts) {
	if (!root) { return; }

	*numberOfSoldProducts += root->customer->purchases->numberOfPurchaseProducts;
	CustomerTraverseDistributionCenterWithMoreSales(root->left, numberOfSoldProducts);
	CustomerTraverseDistributionCenterWithMoreSales(root->right, numberOfSoldProducts);
}

// Funcion extra 1: Encontrar marca/proovedor con peor valoracion
struct Brand* FindBrandWithLowestStatus() {
	if (!derco->brands) { return NULL; }

	struct BrandNode* brandTraverse = derco->brands;
	struct Brand* brandWithLowestStatus = NULL;
	float lowestStatus = 6;

	do {
		if (brandTraverse->brand->status < lowestStatus) {
			lowestStatus = brandTraverse->brand->status;
			brandWithLowestStatus = brandTraverse->brand;
		}
		brandTraverse = brandTraverse->next;
	} while (brandTraverse != derco->brands);

	return brandWithLowestStatus;
}

// Funcion extra 2: Encontrar producto mas vendido en base al tipo en todas las zonas
struct ProductSharedData* FindBestSellingProductByTypeInAllZones(enum ProductType type) {
	if (!derco->zoneList->zones) { return NULL; }
	
	struct ZoneNode* zoneTraverse = derco->zoneList->zones;
	struct ProductSharedData* bestSellingProduct = NULL, *bestSellingProductInZone = NULL;
	int bestSeller = -1, sales;

	do {
		bestSellingProductInZone = FindBestSellingProductByType(zoneTraverse->zone->distributionCenters, type, &sales);
		if (bestSeller < sales) {
			bestSeller = sales;
			bestSellingProduct = bestSellingProductInZone;
		}
		zoneTraverse = zoneTraverse->next;
	} while (zoneTraverse != derco->zoneList->zones);

	return bestSellingProduct;
}

struct ProductSharedData* FindBestSellingProductByType(struct DistributionCenterNode* distributionCenters, enum ProductType type, int* bestSeller) {
	struct ProductIDNode* ProductIDS = NULL, *ProductIDTraverse = NULL;
	struct DistributionCenterNode* distributionCenterTraverse;
	struct ProductSharedData* bestSellingProduct = NULL;
	int sales;

	*bestSeller = -1;

	ProductIDS = CreateProductIDsList(distributionCenters, type);

	ProductIDTraverse = ProductIDS;
	
	while (ProductIDTraverse) {
		distributionCenterTraverse = distributionCenters->next;
		sales = 0;
		while (distributionCenterTraverse) {
			CustomerTraverseBestSellingProduct(distributionCenterTraverse->distributionCenter->customers, ProductIDTraverse->ID, &sales);
			distributionCenterTraverse = distributionCenterTraverse->next;
		}
		if (*bestSeller < sales) {
			*bestSeller = sales;
			bestSellingProduct = ProductIDTraverse->ID;
		}
		ProductIDTraverse = ProductIDTraverse->next;
	}

	return bestSellingProduct;
}

void CustomerTraverseBestSellingProduct(struct CustomerNode* root, struct ProductSharedData* ID, int* sales) {
	if (!root) { return; }

	*sales += CountProductSales(root->customer->purchases, ID);
	CustomerTraverseBestSellingProduct(root->left, ID, sales);
	CustomerTraverseBestSellingProduct(root->right, ID, sales);
}

int CountProductSales(struct Purchases* purchases, struct ProductSharedData* ID) {
	int i, acumulator = 0;

	for (i = 0; i < purchases->indexFirstEmpty; i++) {
		if (purchases->purchaseArray[i]->productSharedData == ID) {
			acumulator += purchases->purchaseArray[i]->amount;
		}
	}

	return acumulator;
}

struct ProductIDNode* CreateProductIDsList(struct DistributionCenterNode* distributionCenters, enum ProductType type) {
	struct ProductIDNode* productIDs = NULL;
	struct DistributionCenterNode* distributionCenterTraverse = distributionCenters->next;

	while (distributionCenterTraverse) {
		CustomerTraverseProductIDs(distributionCenterTraverse->distributionCenter->customers, type, &productIDs);
		distributionCenterTraverse = distributionCenterTraverse->next;
	}

	return productIDs;
}

void CustomerTraverseProductIDs(struct CustomerNode* root, enum ProductType type, struct ProductIDNode** productIDs) {
	if (!root) { return; }

	*productIDs = AddNewProductIDs(root->customer->purchases, type, *productIDs);
	CustomerTraverseProductIDs(root->left, type, productIDs);
	CustomerTraverseProductIDs(root->right, type, productIDs);
}

struct ProductIDNode* AddNewProductIDs(struct Purchases* purchases, enum ProductType type, struct ProductIDNode* productIDs) {
	int i;

	for (i = 0; i < purchases->indexFirstEmpty; i++) {
		if ((purchases->purchaseArray[i]->productSharedData->type == type || type == END) && FindProductIDInList(productIDs, purchases->purchaseArray[i]->productSharedData)) {
			productIDs = AddProductIDNode(productIDs, purchases->purchaseArray[i]->productSharedData);
		}
	}

	return productIDs;
}

int FindProductIDInList(struct ProductIDNode* productIDs, struct ProductSharedData* productSharedData) {
	struct ProductIDNode* traverse = productIDs;

	while (traverse) {
		if (traverse->ID == productSharedData) { return 0; }
		traverse = traverse->next;
	}

	return 1;
}

struct ProductIDNode* AddProductIDNode(struct ProductIDNode* productIDs, struct ProductSharedData* productSharedData) {
	struct ProductIDNode* newNode;
	newNode = (struct ProductIDNode*)malloc(sizeof(struct ProductIDNode));

	if (!newNode) { return NULL; }

	newNode->ID = productSharedData;
	newNode->next = productIDs;

	return newNode;
}

struct Product* FindProductByIDInAllBrands(int ID) {
	if (!derco->brands) { return NULL; }
	
	struct BrandNode* traverse = derco->brands;
	struct ProductNode* product = NULL;

	do {
		product = FindProductByID(traverse->brand->stock, ID);
		if (product) { return product->product; }
		traverse = traverse->next;
	} while (traverse != derco->brands);
	
	return NULL;
}

void PrintProductIDsInAllBrands() {
	if (!derco->brands) { return; }

	struct BrandNode* traverse = derco->brands;
	
	printf("PRODUCTOS DE LAS MARCAS/PROVEEDORES:\n\n");

	do {
		printf("marca: \"%s\"\n\n", *traverse->brand->name);
		PrintProductIDs(traverse->brand->stock);
		traverse = traverse->next;
	} while (traverse != derco->brands);

	return;
}

int EnterInteger(char* text) {
	int output;
	printf("[ENTERO POSITIVO] ");
	printf(text);
	if (!scanf("%d", &output) || output < 0) {
		printf("\nValor invalido.\n");
		Sleep(ACTIONTIME);
		output = -1;
	}
	printf("\n");
	CLEARINPUTBUFFER;
	return output;
}

float EnterFloatingPoint(char* text) {
	float output;
	printf("[PUNTO FLOTANTE POSITIVO] ");
	printf(text);
	if (!scanf("%f", &output) || output < 0) {
		printf("\nValor invalido.\n");
		Sleep(ACTIONTIME);
		output = -1;
	}
	printf("\n");
	CLEARINPUTBUFFER;
	return output;
}

char* EnterText(char* text) {
	char* output;
	printf("[MAX %d CARACTERES] ", STRINGBUFFERSIZE);
	printf(text);
	output = (char*)malloc(sizeof(char) * STRINGBUFFERSIZE);
	if (!scanf("%[^\n]", output)) {
		printf("\nValor invalido.\n");
		Sleep(ACTIONTIME);
		output = NULL;
	}
	printf("\n");
	CLEARINPUTBUFFER;
	return output;
}	