// Taki Project
// Tal Lovton 

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#define INIT_NUM_OF_CARDS 4
#define MAX_CHARS 20
#define MAX_CHARS_CARDS 6
#define MAX_CARDS 14
#define COLS 1
#define RED 2
#define GREEN 4
#define BLUE 3
#define YELLOW 1
#define NO_COLOR -1 
#define STOP 10
#define PLUS 11
#define CHANGE_DIR 12
#define TAKI 14
#define CHANGE_COL 13
#define ROWS 10
#define IS_UPPER true
#define NOT_UPPER false
// temporary special cards valuse only for the statistics chart.
#define TMP_STOP 9
#define TMP_PLUS 10
#define TMP_CHANGE_DIR 11
#define TMP_TAKI 13
#define TMP_CHANGE_COL 12

// Structs:

typedef struct statistiscs {
	char cardType[MAX_CHARS_CARDS];
	int counter;
}Stats;

typedef struct card {
	int cardNum;
	int cardColor;
}Card;

typedef struct player {
	char playerName[MAX_CHARS];
	Card* playerCards;
	int logicSize;
	int physicSize;
}Player;

typedef struct game {
	Player* players;
	int size;
	Card upperCard;
}Game;


// functions declartions:
void entryMessage();
void readCards(Stats arrCards[]);
int askNumberOfPlayersInGame();
void initilaisedGame(Game* gamePoint, Stats arrCards[]);
void getPalyersData(Player* playersArr, int numOfPlayers, Stats arrCards[]);
void askPlayersNames(Player* playersArr, int numOfPlayers);
void getPlayesCards(Player* playersArr, int numOfPlayers, Stats arrCards[]);
void getCardData(Card* card, bool isUpper, Stats arrCards[]);
void startGame(Game* gameStart, Card* card, Stats arrCards[]);
bool checkForWinner(Game* gameStart);
void printCards(Card card, bool isUpper);
char numToChar(Card card);
void specialCard(Card card, char* cardAbility);
int playerChoice(Player* playerTurn, Card* upperCard, Stats arrCards[]);
void cardFromDeck(Player* playerPack, Stats arrCards[]);
void cardToMiddle(Player* playerPack, Card* cardOnTable, int playerChoose, Stats arrCards[]);
void swapCards(Card arr[], int size, int playerChoose);
void changePackAfterTurn(Player* playerPack, Card* cardOnTable, int playerChoose);
void freeGame(Game* game);
void SortCardsChart(Stats arrCards[]);
void printCardsChart(Stats arrCards[]);




void main()
{
	Stats gameStats[MAX_CARDS];
	Game game;
	Card card;
	srand(time(NULL));
	entryMessage();
	readCards(gameStats);
	initilaisedGame(&game, gameStats);
	startGame(&game, &card, gameStats);
	freeGame(&game);
	SortCardsChart(gameStats);
}


// functions implemtations:

void entryMessage() {
	printf("************  Welcome to TAKI game !!! ***********\n");

int askNumberOfPlayersInGame()
{
	int numPlayers;
	printf("\nPlease enter the number of players: ");
	scanf("%d", &numPlayers);
	return numPlayers;
}

// inputing all the game details (num of players, cards data)

void initilaisedGame(Game* gamePoint, Stats arrCards[]) {
	gamePoint->size = askNumberOfPlayersInGame();   // input the number of players
	gamePoint->players = (Player*)malloc(sizeof(Player) * gamePoint->size); // dynamic array for players number.
	getPalyersData(gamePoint->players, gamePoint->size, arrCards);
	getCardData(&gamePoint->upperCard, true, arrCards);
}


void getPalyersData(Player* playersArr, int numOfPlayers, Stats arrCards[])
{
	askPlayersNames(playersArr, numOfPlayers);
	getPlayesCards(playersArr, numOfPlayers, arrCards);
}


// input for the players name.

void askPlayersNames(Player* playersArr, int numOfPlayers)
{
	int i;
	for (i = 0; i < numOfPlayers; i++) {
		printf("Enter the player name %d:\n\n", i + 1);
		scanf("%s", playersArr[i].playerName);
	}
}

// creating the first card array of the game, 4 cards for each player.

void getPlayesCards(Player* playersArr, int numOfPlayers, Stats arrCards[])
{
	int i, j;
	for (i = 0; i < numOfPlayers; i++) {
		playersArr[i].logicSize = playersArr[i].physicSize = INIT_NUM_OF_CARDS;   // passing for each player the first 4 cards
		playersArr[i].playerCards = (Card*)malloc(sizeof(Card) * INIT_NUM_OF_CARDS);
		for (j = 0; j < INIT_NUM_OF_CARDS; j++)
			getCardData(&playersArr[i].playerCards[j], false, arrCards);   // for each card from the array getting the number and color
	}


}

// getting for each card the card number and color(voth in integers).

void getCardData(Card* card, bool isUpper, Stats arrCards[])
{

	if (isUpper) {
		card->cardNum = rand() % 9 + 1;             // if its upper card
		arrCards[card->cardNum - 1].counter++;     // counter for frequency
	}
	else {
		card->cardNum = rand() % 14 + 1;           // regular card from pack
		arrCards[card->cardNum - 1].counter++;
	}


	if (card->cardNum == CHANGE_COL)
		card->cardColor = NO_COLOR;
	else
		card->cardColor = rand() % 4 + 1;        // each card color

}

// game platform, printing the first card and passing the turns.

void startGame(Game* gameStart, Card* card, Stats arrCards[]) {
	int i, j;
	int playerChoose;
	int activePlayer = 0;
	int direction = 1;

	printf("Upper Card:\n");
	printCards(gameStart->upperCard, IS_UPPER);
	while (!checkForWinner(gameStart)) {                 // checking if there is a winner
		printf("\n\n%s's turn:\n\n", gameStart->players[activePlayer].playerName);
		for (i = 0; i < gameStart->players[activePlayer].logicSize; i++) {
			printf("\nCard #%d\n", i + 1);                                         // printing the cards for each player
			printCards(gameStart->players[activePlayer].playerCards[i], NOT_UPPER);
		}
		playerChoose = playerChoice(&gameStart->players[activePlayer], &gameStart->upperCard, arrCards);      // player choose for witch card to play
		printf("Upper Card:\n");
		printCards(gameStart->upperCard, IS_UPPER);


		// the conditions for passing or not passing turns by the special cards
		if ((((gameStart->upperCard.cardNum == 11 || gameStart->upperCard.cardNum == 14) && playerChoose != 0) || (gameStart->upperCard.cardNum == 10 && gameStart->size <= 2)) || (gameStart->upperCard.cardNum == 12 && gameStart->size <= 2)) {
			activePlayer--;
			activePlayer = (activePlayer + direction) % gameStart->size;
		}
		else if ((gameStart->upperCard.cardNum == 10 && gameStart->size > 2) && playerChoose != 0) {
			activePlayer++;
			activePlayer = (activePlayer + direction) % gameStart->size;
		}
		else if (gameStart->upperCard.cardNum == 12 && playerChoose != 0) {
			activePlayer *= -1;
			activePlayer = (activePlayer + direction) % gameStart->size;
		}
		else {
			activePlayer = (activePlayer + direction) % gameStart->size;
		}

	}


}


// functions that check when a player finish his cards and declared as winner.

bool checkForWinner(Game* gameStart)
{
	int i;
	for (i = 0; i < gameStart->size; i++) {
		if (gameStart->players[i].logicSize == 0) {
			printf("The winner is...%s! Congratulations!\n\n", gameStart->players->playerName);
			return true;
		}
	}
	return false;
}

// print the cards with stars.

void printCards(Card card, bool isUpper) {
	int i, j, k = 0, z;
	char cardColor;
	char* cardPower = (char*)malloc(5);
	for (i = 1; i <= ROWS / 2 + 2; i++) {
		for (j = 1; j < ROWS; j++) {
			if (i == 1 || i == ROWS - 1 || j == 1 || j == ROWS - 1 || i == ROWS / 2 + 2)
				printf("*");
			else if (((i == ((ROWS / 2 + 2) / 2) && j == ROWS / 2))) {
				if (isUpper || card.cardNum < 10)
					if (card.cardNum == 13) {
						specialCard(card, cardPower);
						printf("%s ", cardPower);
						j += 5;
					}
					else if (card.cardNum == 12) {
						specialCard(card, cardPower);
						printf("%s  ", cardPower);
						j += 4;
					}
					else if ((card.cardNum == 10 || card.cardNum == 14)) {
						specialCard(card, cardPower);
						printf(" %s", cardPower);
						j += 4;
					}
					else if (card.cardNum == 11) {
						specialCard(card, cardPower);
						printf("%s", cardPower);
					}
					else
						printf("%d", card.cardNum);

				else if (card.cardNum == 11) {
					specialCard(card, cardPower);
					printf("%s", cardPower);
				}

			}
			else if ((i == ((ROWS / 2 + 2) / 2) && j == (ROWS / 2) - 2) && (card.cardNum == 13)) {
				specialCard(card, cardPower);
				printf("%s ", cardPower);
				j += 5;
			}
			else if ((i == ((ROWS / 2 + 2) / 2) && j == (ROWS / 2) - 1) && (card.cardNum == 12)) {
				specialCard(card, cardPower);
				printf("%s  ", cardPower);
				j += 4;
			}
			else if ((i == ((ROWS / 2 + 2) / 2) && j == (ROWS / 2) - 2) && (card.cardNum == 10 || card.cardNum == 14)) {
				specialCard(card, cardPower);
				printf(" %s", cardPower);
				j += 4;
			}

			else if (i == ((ROWS / 2 + 2) / 2 + 1) && j == ROWS / 2) {
				cardColor = numToChar(card);
				printf("%c", cardColor);

			}

			else
			{
				printf(" ");
			}
		}
		printf("\n");

	}

}


// changing the color num to chars of the first letter 

char numToChar(Card card) {
	char colorLetter;
	if (card.cardColor == -1) {
		colorLetter = ' ';
	}
	switch (card.cardColor)
	{
	case 1:
		if (card.cardColor == YELLOW)
			colorLetter = 'Y';
		break;
	case 2:
		if (card.cardColor == RED)
			colorLetter = 'R';
		break;
	case 3:
		if (card.cardColor == BLUE)
			colorLetter = 'B';
		break;
	case 4:
		if (card.cardColor == GREEN)
			colorLetter = 'G';
		break;
	default:
		break;
	}


	return colorLetter;
}


// if its special card, bigger than 9. printing his ability

void specialCard(Card card, char* cardAbility) {

	switch (card.cardNum)
	{
	case 10:
		if (card.cardNum == STOP);
		strcpy(cardAbility, "STOP");
		break;
	case 11:
		if (card.cardNum == PLUS);
		strcpy(cardAbility, "+");
		break;
	case 12:
		if (card.cardNum == CHANGE_DIR);
		strcpy(cardAbility, "<->");
		break;
	case 13:
		if (card.cardNum == CHANGE_COL);
		strcpy(cardAbility, "COLOR");
		break;
	case 14:
		if (card.cardNum == TAKI);
		strcpy(cardAbility, "TAKI");
		break;
	default:
		break;
	}


}


// free ak the dynamic array.

void freeGame(Game* game)
{
	int i, j;
	for (i = 0; i < game->size; i++)
	{
		free(game->players[i].playerCards);
	}
	free(game->players);
}


// the player choice for each turn. taking card or use one of his cards.

int playerChoice(Player* playerTurn, Card* upperCard, Stats arrCards[]) {
	int playerChoose;
	printf("\nPlease enter 0 if you want to take a card from the deck\n"
		"or 1-%d if you want to put one of your cards in the middle:\n", playerTurn->physicSize);
	scanf("%d", &playerChoose);

	while (playerChoose > playerTurn->physicSize) {
		printf("Invalid card! Try again.\n");
		printf("\nPlease enter 0 if you want to take a card from the deck\n"
			"or 1-%d if you want to put one of your cards in the middle:\n", playerTurn->physicSize);
		scanf("%d", &playerChoose);
	}

	if (playerChoose == 0) {                               // the player has no valid cards to use
		cardFromDeck(playerTurn, arrCards);
		playerTurn->physicSize++;
	}
	else if (playerChoose > 0) {                          // the player choose to play with one card from his pack
		playerTurn->physicSize--;
		cardToMiddle(playerTurn, upperCard, playerChoose - 1, arrCards);
	}

	return playerChoose;



}


// if the player choose to take card from deck.

void cardFromDeck(Player* playerPack, Stats arrCards[]) {
	int color, card;
	playerPack->logicSize += 1;                                      // increasing the cards pack
	getCardData(&playerPack->playerCards[playerPack->logicSize - 1], NOT_UPPER, arrCards);
}

// if the player use one of his cards.

void cardToMiddle(Player* playerPack, Card* cardOnTable, int playerChoose, Stats arrCards[]) {
	int i, newColor = 0;
	int addCard = 1;

	// the cards with regular numbers
	if (playerPack->playerCards[playerChoose].cardNum >= cardOnTable->cardNum && playerPack->playerCards[playerChoose].cardColor == cardOnTable->cardColor) {
		cardOnTable->cardNum = playerPack->playerCards[playerChoose].cardNum;
		cardOnTable->cardColor = playerPack->playerCards[playerChoose].cardColor;
		changePackAfterTurn(playerPack, cardOnTable, playerChoose);
	}

	// Plus card conditions
	else if (playerPack->playerCards[playerChoose].cardNum == 11 && playerPack->playerCards[playerChoose].cardColor == cardOnTable->cardColor) {
		cardOnTable->cardNum = playerPack->playerCards[playerChoose].cardNum;
		cardOnTable->cardColor = playerPack->playerCards[playerChoose].cardColor;
		changePackAfterTurn(playerPack, cardOnTable, playerChoose);
	}

	// Change color conditions
	else if (playerPack->playerCards[playerChoose].cardNum == 13) {
		cardOnTable->cardNum = playerPack->playerCards[playerChoose].cardNum;
		printf("Please enter your color choice:\n1 - Yellow\n2 - Red\n3 - Blue\n4 - Green\n");
		scanf("%d", &newColor);
		cardOnTable->cardColor = newColor;
		changePackAfterTurn(playerPack, cardOnTable, playerChoose);

	}

	// Taki card conditins
	else if (playerPack->playerCards[playerChoose].cardNum == 14 && playerPack->playerCards[playerChoose].cardColor == cardOnTable->cardColor) {
		cardOnTable->cardNum = playerPack->playerCards[playerChoose].cardNum;
		cardOnTable->cardColor = playerPack->playerCards[playerChoose].cardColor;
		changePackAfterTurn(playerPack, cardOnTable, playerChoose);
	}

	// changing direction card
	else if (playerPack->playerCards[playerChoose].cardNum == 12 && playerPack->playerCards[playerChoose].cardColor == cardOnTable->cardColor) {
		cardOnTable->cardNum = playerPack->playerCards[playerChoose].cardNum;
		cardOnTable->cardColor = playerPack->playerCards[playerChoose].cardColor;
		changePackAfterTurn(playerPack, cardOnTable, playerChoose);
	}

	// Stop card condtions
	else if (playerPack->playerCards[playerChoose].cardNum == 10 && playerPack->playerCards[playerChoose].cardColor == cardOnTable->cardColor) {
		cardOnTable->cardNum = playerPack->playerCards[playerChoose].cardNum;
		cardOnTable->cardColor = playerPack->playerCards[playerChoose].cardColor;
		changePackAfterTurn(playerPack, cardOnTable, playerChoose);
	}

	// for the second turns of player and when the upper card is special
	else if (cardOnTable->cardNum > 9) {
		if ((playerPack->playerCards[playerChoose].cardNum < cardOnTable->cardNum) && playerPack->playerCards[playerChoose].cardColor == cardOnTable->cardColor) {
			cardOnTable->cardNum = playerPack->playerCards[playerChoose].cardNum;
			cardOnTable->cardColor = playerPack->playerCards[playerChoose].cardColor;
			changePackAfterTurn(playerPack, cardOnTable, playerChoose);
		}
	}

	else {
		printf("Invalid card! Try again.");
		playerPack->physicSize++;
		playerChoice(playerPack, cardOnTable, arrCards);

	}


}

// changing the place in array of the players cards.

void swapCards(Card arr[], int size, int playerChoose)
{
	int i;
	int tmpNum;
	int tmpColor;
	for (i = 0; i <= size; i++) {
		if (i > playerChoose) {
			tmpNum = arr[playerChoose].cardNum;
			tmpColor = arr[playerChoose].cardColor;
			arr[playerChoose].cardNum = arr[i].cardNum;
			arr[playerChoose].cardColor = arr[i].cardColor;
			arr[i].cardNum = arr[playerChoose].cardNum;
			arr[i].cardColor = arr[playerChoose].cardColor;
			playerChoose++;
		}

	}
}


// remove from the arrays of cards the card that initilised.

void changePackAfterTurn(Player* playerPack, Card* cardOnTable, int playerChoose) {
	int i;
	swapCards(playerPack->playerCards, playerPack->logicSize, playerChoose);
	playerPack->logicSize -= 1;         // decreasing the array of cards.

}


// read the cards for the statistics chart

void readCards(Stats arrCards[]) {
	int i;

	for (i = 0; i < MAX_CARDS; i++) {
		arrCards[i].counter = 0;                    // zero all the counter for each card.
	}

	for (i = 0; i < MAX_CARDS; i++) {
		if (i < 9) {
			arrCards[i].cardType[0] = i + 1 + '0';
			arrCards[i].cardType[1] = '\0';                      // transfering the nums to chars
		}
		else {
			switch (i)
			{
			case TMP_STOP:
				strcpy(arrCards[i].cardType, "STOP");
				break;

			case TMP_PLUS:
				strcpy(arrCards[i].cardType, "+");
				break;

			case TMP_CHANGE_DIR:
				strcpy(arrCards[i].cardType, "<->");
				break;

			case TMP_CHANGE_COL:
				strcpy(arrCards[i].cardType, "COLOR");
				break;

			case TMP_TAKI:
				strcpy(arrCards[i].cardType, "TAKI");
				break;
			default:
				break;
			}
		}


	}



}


// sorting the cards by the frequency of them in the game.

void SortCardsChart(Stats arrCards[]) {
	int i, j, k;
	int tmpCounter;
	char* tmpCardType[MAX_CHARS_CARDS];

	for (j = 0; j < MAX_CARDS; j++) {
		for (i = 0; i < MAX_CARDS; i++) {                              // swaping the cards by their frequency
			if (arrCards[i].counter > arrCards[i + 1].counter) {
				tmpCounter = arrCards[i].counter;
				strcpy(tmpCardType, arrCards[i].cardType);
				arrCards[i].counter = arrCards[i + 1].counter;
				strcpy(arrCards[i].cardType, arrCards[i + 1].cardType);
				arrCards[i + 1].counter = tmpCounter;
				strcpy(arrCards[i + 1].cardType, tmpCardType);
			}
		}
	}

	printCardsChart(arrCards);



}


// print the cards chart of statistics.

void printCardsChart(Stats arrCards[]) {
	int i;
	printf("************ Game Statistics ************\n");
	printf("Card # | Frequency\n");
	printf("__________________\n");
	for (i = 0; i < MAX_CARDS; i++) {

		switch (strlen(arrCards[i].cardType)) {
		case 1:
		{
			printf("   %s   |    %d\n", arrCards[i].cardType, arrCards[i].counter);
			break;
		}
		case 3:
		{
			printf("  %s  |    %d\n", arrCards[i].cardType, arrCards[i].counter);
			break;
		}
		case 4:
		{
			printf(" %s  |    %d\n", arrCards[i].cardType, arrCards[i].counter);
			break;
		}
		case 5:
		{
			printf(" %s |    %d\n", arrCards[i].cardType, arrCards[i].counter);
			break;
		}

		}

	}
}




