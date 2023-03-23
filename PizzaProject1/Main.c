
// Tal Lovton 

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#define REGULAR_PRICE 70
#define OLIVE_PRICE 10
#define MUSHROOM_PRICE 12
#define TOMATOES_PRICE 9
#define PINEAPPLE_PRICE 14
#define VEGAN_DOUGH_PRICE 5
#define WHOLE_DOUGH_PRICE 3
#define GLUTENF_DOUGH_PRICE 4
#define MIN_LEN 10
#define MAX_LEN 40
#define DEF_LEN 40
#define DEF_WID 50
#define MIN_WID 10
#define MAX_WID 80
#define BASIC_SIZE (DEF_LEN * DEF_WID)


typedef struct pizza {
	int counter, width, length, oliveTop, mushroomTop, tomatoTop, pineTop, totalSize;
	double price, sizeTopping;
	char breadType;
}Pizza;

typedef struct order {
	int totalPrice;
	int numberOfPizzas;
	int id;
	int delivery;
	int payment;
	int change;
}Order;

int printGreetingAndId();
int checkID();
void printMenu();
Order createOrder();
Pizza createPizza();
char chooseBreadType();
int calculateBreadType(char breadType, int totalSize);
int printToppings(char* nameOfTopping);
int calculateToppings(int newTop, int totalSize, int extra);
double sizeToppings(int addToppings);
double exceedToppings(double sizeToppings);
Pizza addToppings(Pizza newPizza);
void summaryOfOrder(int numOfPizza, int PizzaWidth, int PizzaLength, double PizzaPrice);
int delivery();
int sumOfAllOrders(int idNum, int numOfPizzas, int TotalPrice);
void paymentAndChange(int Total);




void main() {
	int id;
	id = printGreetingAndId();
	checkID(id);
	printMenu();
	createOrder(id);



}

int printGreetingAndId() {
	int id;
	printf(" Welcome to MTA-Pizza!\n\n***** \n ***\n  *\n\n");
	printf("Please enter your ID number:\n\n");
	scanf("%d", &id);
	return id;
}

int checkID(int id) {
	int num0fTab;
	int i;
	for (i = 1; i <= id; i++) {
		id = id / 10;
	}
	printf("%d", i);
	while (i > 9) {
		printf("Invalid ID number! Try again.\n");
		for (i = 1; i <= id; i++) {
			id = id / 10;
		}
	}
	return 0;
}

void printMenu() {
	printf("Our menu:\n*********\n");
	printf("Basic pizza: 70.00 NIS for 40x50 size pizza\n\n"
		"Toppings for basic size pizza: \nOlives: 10 NIS\n"
		"Mushrooms: 12 NIS\nTomatoes: 9 NIS\nPineapple: 14 NIS\n\n");

	printf("Dough type for basic size pizza:\n"
		"Regular: 0 NIS\n"
		"Vegan: 5 NIS\n"
		"Whole wheat: 3 NIS\n"
		"Gluten free: 4 NIS\n");

}

Pizza createPizza() {
	Pizza newPizza;
	printf("Please enter your pizza's length (cm): \n");
	scanf("%d", &newPizza.length);

	while ((newPizza.length < MIN_LEN) || (newPizza.length > MAX_LEN) || (newPizza.length % 2 != 0)) {
		printf("Invalid length! Try again.\n");
		printf("Please enter your pizza's length (cm): \n");
		scanf("%d", &newPizza.length);
	}

	printf("Please enter your pizza's width (cm):\n");
	scanf("%d", &newPizza.width);

	while ((newPizza.width < MIN_LEN) || (newPizza.width > MAX_WID) || (newPizza.width % 2 != 0)) {
		printf("Invalid width! Try again.\n");
		printf("Please enter your pizza's width (cm):\n");
		scanf("%d", &newPizza.width);
	}

	newPizza.totalSize = (newPizza.length * newPizza.width);
	newPizza.price = (((double)newPizza.totalSize / BASIC_SIZE) * REGULAR_PRICE);

	newPizza.breadType = chooseBreadType();
	newPizza.price += calculateBreadType(newPizza.breadType, newPizza.totalSize);

	newPizza = addToppings(newPizza);



}

Order createOrder(int id) {
	Order order;

	order.delivery = 0;
	order.change = 0;
	order.totalPrice = 0;
	Pizza curPizza;
	printf("How many pizzas would you like to order?\n");
	scanf("%d", &order.numberOfPizzas);
	while (order.numberOfPizzas < 0) {
		printf("Invalid choice! try again.\n");
		printf("How many pizzas would you like to order?\n");
		scanf("%d", &order.numberOfPizzas);

	}

	if (order.numberOfPizzas == 0)
		printf("Thank you and goodbye.");

	else {
		for (int i = 1; i <= order.numberOfPizzas; i++) {
			printf("\n*************************************************\nPizza #%d:\n", i);
			curPizza = createPizza();
			summaryOfOrder(i, curPizza.length, curPizza.width, (double)curPizza.price);
		}
		curPizza.price += delivery();
		order.totalPrice += curPizza.price;
		order.totalPrice = sumOfAllOrders(id, order.numberOfPizzas, order.totalPrice);

		paymentAndChange(order.totalPrice);
	}

}


char chooseBreadType() {
	char breadType;
	printf("\nPlease enter the pizza's dough type:\n"
		"r - for regular\n"
		"v - for vegan\n"
		"w - for whole wheat\n"
		"f - for gluten-free\n");
	scanf(" %c", &breadType);
	while ((breadType != 'r') && (breadType != 'v') && (breadType != 'w') && (breadType != 'f')) {
		printf("Invalid choice! Try again.");
		printf("\nPlease enter the pizza's dough type:\n"
			"r - for regular\n"
			"v - for vegan\n"
			"w - for whole wheat\n"
			"f - for gluten-free\n");
		scanf(" %c", &breadType);
	}
	return breadType;
}

int calculateBreadType(char breadType, int totalSize) {
	int price = 0;
	switch (breadType)
	{
	case 'r':
		break;
	case 'v':
		price += (totalSize / BASIC_SIZE) * VEGAN_DOUGH_PRICE;
		break;
	case 'w':
		price += (totalSize / BASIC_SIZE) * WHOLE_DOUGH_PRICE;
		break;
	case 'f':
		price += (totalSize / BASIC_SIZE) * GLUTENF_DOUGH_PRICE;
		break;
	default:
		printf("Invalid choice! Regular dough was chosen as a default.\n");
	}
	return price;

}

int printToppings(char* nameOfTopping) {
	int sizeTop = 0;
	printf("%s", nameOfTopping);
	printf(" (choose 0 - 3) : \n"
		"0. None\n"
		"1. Whole pizza\n"
		"2. Half pizza\n"
		"3. Quarter pizza\n");
	scanf("%d", &sizeTop);
	return sizeTop;
}

double sizeToppings(int sizeTop) {
	double sumOfTopping = 0;
	switch (sizeTop)
	{
	case 0:
		break;
	case 1:
		sumOfTopping = 1;
		break;
	case 2:
		sumOfTopping = 0.5;
		break;
	case 3:
		sumOfTopping = 0.25;
		break;
	default:
		break;
	}
	return sumOfTopping;
}

int calculateToppings(int newTop, int totalSize, int extra) {
	int price = 0;
	switch (newTop)
	{
	case 0:
		break;
	case 1:
		price += (totalSize / BASIC_SIZE) * extra;
		break;
	case 2:
		price += ((totalSize * 0.5) / BASIC_SIZE) * extra;
		break;
	case 3:
		price += ((totalSize * 0.25) / BASIC_SIZE) * extra;
		break;
	default:
		printf("Invalid choice! Current topping not added.");
	}
	return price;
}


Pizza addToppings(Pizza newPizza) {

	double currentToppingSize = 0;

	printf("\nPlease choose the toppings:\n\n");
	newPizza.oliveTop = printToppings("Olives");
	newPizza.sizeTopping = sizeToppings(newPizza.oliveTop);
	newPizza.price += calculateToppings(newPizza.oliveTop, newPizza.totalSize, OLIVE_PRICE);

	if (newPizza.sizeTopping < 1)
	{
		currentToppingSize = sizeToppings(printToppings("Mushrooms"));
		if (newPizza.sizeTopping + currentToppingSize > 1) {
			currentToppingSize = exceedToppings(newPizza.sizeTopping);
		}
		newPizza.sizeTopping += currentToppingSize;
		newPizza.mushroomTop = currentToppingSize;
		newPizza.price += calculateToppings(newPizza.mushroomTop, newPizza.totalSize, MUSHROOM_PRICE);
	}


	if (newPizza.sizeTopping < 1)
	{
		currentToppingSize = sizeToppings(printToppings("Tomatoes"));
		if (newPizza.sizeTopping + currentToppingSize > 1) {
			currentToppingSize = exceedToppings(newPizza.sizeTopping);
		}
		newPizza.sizeTopping += currentToppingSize;
		newPizza.tomatoTop = currentToppingSize;
		newPizza.price += calculateToppings(newPizza.tomatoTop, newPizza.totalSize, TOMATOES_PRICE);
	}


	if (newPizza.sizeTopping < 1) {
		currentToppingSize = sizeToppings(printToppings("Pineapple"));
		if (newPizza.sizeTopping + currentToppingSize > 1) {
			currentToppingSize = exceedToppings(newPizza.sizeTopping);
		}
		newPizza.sizeTopping += currentToppingSize;
		newPizza.pineTop = currentToppingSize;
		newPizza.price += calculateToppings(newPizza.pineTop, newPizza.totalSize, PINEAPPLE_PRICE);
	}

	return newPizza;
}

double exceedToppings(double currentTopSize) {
	double newChoose = 0;

	do {
		printf("You have exceede the maximum amount of toppings allowed on one pizza! Try again.\n");
		printf("0. None\n"
			"1. Whole pizza\n"
			"2. Half pizza\n"
			"3. Quarter pizza\n");
		scanf("%lf", &newChoose);
		newChoose = sizeToppings(newChoose);
	} while (currentTopSize + newChoose > 1);

	return newChoose;
}

void summaryOfOrder(int numOfPizza, int PizzaWidth, int PizzaLength, double PizzaPrice) {
	int TotalSum = 0;
	printf("\nPizza #%d details:\n*******************\n", numOfPizza);
	printf("Pizza size: %dx%d\n", PizzaLength, PizzaWidth);
	printf("Pizza price (without tax): %.2lf\n", PizzaPrice);
}

int delivery() {
	int delivery;
	int extraDelivery;
	printf("\n*************************************************\nDo you want delivery for the price of 15 NIS? Enter 1 for "
		"delivery or 0 for pick-up: ");
	scanf("%d", &delivery);

	switch (delivery)
	{
	case 0:
		break;
	case 1:
		extraDelivery = 15;
		break;
	default:
		printf("Invalid Choice! pick-up was chosen as a default.\n");
		break;
	}
	return extraDelivery;
}

int sumOfAllOrders(int idNum, int numOfPizzas, int TotalPrice)
{
	printf("\nYour order details:\n********************\n");
	printf("ID number: %09d", idNum);
	printf("\nNumber of pizzas: %d", numOfPizzas);
	printf("\nTotal price: %d", TotalPrice);
	TotalPrice += TotalPrice * 0.17;
	printf("\nTotal price with tax (rounded down): %d \n", TotalPrice);

	return (int)TotalPrice;
}

void paymentAndChange(int Total) {
	int coin;
	Order pay;
	printf("\nPlease enter your payment: \n");
	scanf("%d", &pay.payment);

	while (pay.payment < Total)
	{
		Total -= pay.payment;
		printf("Your remaining balance is: %d\n", Total);
		printf("Please enter your payment: \n");
		scanf("%d", &pay.payment);
	}
	pay.change = pay.payment - Total;
	if (pay.change != 0)
	{
		printf("Your change is %d NIS using:", pay.change);

		coin = pay.change / 10;
		if (coin != 0)
		{
			printf("\n%d coin(s) of ten", coin);
			pay.change -= (coin * 10);
		}
		coin = pay.change / 5;
		if (coin != 0)
		{
			printf("\n%d coin(s) of five", coin);
			pay.change -= (coin * 5);
		}
		coin = pay.change / 2;
		if (coin != 0) {
			printf("\n%d coin(s) of two", coin);
			pay.change -= (coin * 2);
		}
		if (pay.change != 0)
			printf("\n%d coin(s) of one", pay.change);

	}
	printf("\nThank you for your order!");
	return 0;
}