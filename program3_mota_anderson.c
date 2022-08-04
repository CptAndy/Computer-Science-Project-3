// Name:Anderson Mota
// Dr. Steinberg
// COP3502 Spring 2022
// Programming Assignment 3 Skeleton

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct card_s
{
	int rank;	// number
	char *type; // type of card
	struct card_s *nextptr;
} card_t;

// function prototypes
void rules();													 // display rules
int playRound();												 // simulate round
card_t *openCardDeck();											 // open the card deck and place into a linkedlist setup
card_t *insertBackSetup(card_t *node, char *name, int cardrank); // take card from orginial deck and place in back of linked list for setup of game
int empty(card_t *node);										 // check to see if linked list is empty
void cleanUp(card_t *head);										 // free memory to prevent memory leaks
int deckSize(card_t *head);										 // count number of nodes in the linked list
card_t *search(card_t *node, int spot);							 // search list for a specific spot in the card deck indexing is similar to array setup
card_t *copyCard(card_t *node);									 // make a deep copy of card
card_t *removeCard(card_t *node, int spot);						 // remove card from linkedlist
card_t *insertBackDeck(card_t *head, card_t *node);				 // place card at end of pile
int compareCard(card_t *cardp1, card_t *cardp2);				 // compare cards
card_t *moveCardBack(card_t *head);								 // place card at top of deck to the bottom of the deck
void displayR(card_t *head);									 // to display
int main()
{
	int seed;
	printf("Enter Seed: ");
	scanf("%d", &seed);

	srand(seed); // seed set
	rules();

	int player; // 1 or 2
	int result;

	printf("Would you like to be player 1 or 2?\n");
	printf("Enter 1 or 2: ");
	scanf("%d", &player); // choose player

	for (int game = 1; game <= 1; ++game) // simulate games
	{
		printf("Alright lets play game %d.\n", game);
		printf("Lets split the deck.\n");

		result = playRound(); // play game

		if ((result == 1 && player == 1) || (result == 2 && player == 2)) // determine who won
			printf("You won game %d.\n", game);
		else
			printf("I won game %d.\n", game);
	}

	return 0;
}
void rules()
{
	printf("Welcome to the card game war!\n");
	printf("Here are the rules.\n");
	printf("You have a pile of cards and I have a pile of cards.\n");
	printf("We will each pull the top card off of our respective deck and compare them.\n");
	printf("The card with the highest number will win the round and take both cards.\n");
	printf("However if there is a tie, then we have to we have to place one card faced down and the next one faced up and compare the results.\n");
	printf("Winner of the tie, will grab all the cards out. However if it's a tie again, then we repeat the same action.\n");
	printf("Ready? Here we go!\n");
}
int playRound()
{
	// There are going to be multiple decks
	// initialize main deck
	card_t *deck = openCardDeck();
	card_t *p1 = NULL;
	card_t *p2 = NULL;

	// variable we may need
	int cardsInDeck = deckSize(deck);
	int position = 0;

	printf("There are %d cards in the deck.\n", deckSize(deck));
	// while the deck is not empty
	while (!empty(deck))
	{
		// Distribute random cards in the deck to each player
		position = rand() % deckSize(deck);
		p1 = insertBackDeck(p1, copyCard(search(deck, position)));
		deck = removeCard(deck, position);
		
		position = rand() % deckSize(deck);
		p2 = insertBackDeck(p2, copyCard(search(deck, position)));
		deck = removeCard(deck, position);
		
	}
	// clean up the original deck
	cleanUp(deck);
	// Test to see if all functions worked
	//  printf("Player 1s Deck:\n");
	//  displayR(p1);
	//  p1 = moveCardBack(p1);
	//  printf("Player 1s Deck after we moved head to back:\n");
	//  displayR(p1);
	//  printf("Player 1s Deck after we removed the head:\n");
	//  p1 = removeCard(p1, 0);
	//  displayR(p1);
	//   printf("Player 1s Deck:\n");
	//   displayR(p1);
	//    printf("Player 2s Deck:\n");
	//   displayR(p2);
	//  END OF TEST ZONE

	// condition to determine what gets triggered
	int condition = 0;
	int head = 0;
	// while p1 is not empty or p2 is not empty
	while (!empty(p1) || !empty(p2))
	{
		// recieve a condition number
		condition = compareCard(p1, p2);
		printf("Player 1 pulled out %s. \t Player 2 pulled out %s.\n", p1->type, p2->type);

		if (condition == 1)
		{
			printf("Player 1 won that round.\n");
			// move the head to the back of deck
			p1 = moveCardBack(p1);
			// Take the head of player 2s card make a copy insert it into p1s deck
			p1 = insertBackDeck(p1, copyCard(p2));
			// remove the card from player 2
			p2 = removeCard(p2, head);
		}
		if (condition == 2)
		{
			printf("Player 2 won that round.\n");
			// move the head to the back of deck
			p2 = moveCardBack(p2);
			// Take the head of player 1s card make a copy insert it into p2s deck
			p2 = insertBackDeck(p2, copyCard(p1));
			// remove the card from player 1
			p1 = removeCard(p1, head);
		}
		// if we recieve a 0
		if (condition == 0)
		{
			printf("Ugh oh...We have a tie! W-A-R!\n");
			// Inside war
			// The winner of the war gets 3 initially
			int tie = 0;
			int warCondition = 0;
			card_t *profitDeck = NULL;
			while (tie == 0)
			{
				// Special condition
				if (deckSize(p1) <= 2)
				{

					// compare at second position
					warCondition = compareCard(search(p1, deckSize(p1)), search(p2, deckSize(p2)));
					if (warCondition == 1)
					{
						printf("In 1.\n");
						printf("Player 1 won that W-A-R!\n");
						// move p1
						p1 = moveCardBack(p1);

						// insertBack a card from 2
						p1 = insertBackDeck(p1, copyCard(p2));
						p2 = removeCard(p2, head);
						// move p1
						p1 = moveCardBack(p1);
						// insertBack a card from 2
						p1 = insertBackDeck(p1, copyCard(p2));
						p2 = removeCard(p2, head);
						tie = 1;
					}
					if (warCondition == 2)
					{
						printf("In 2.\n");
						// player 2 won
						printf("Player 2 won that W-A-R!\n");
						// insert card from p1 to p2
						p2 = insertBackDeck(p2, copyCard(p1));
						p1 = removeCard(p1, head);
						// move p2
						p2 = moveCardBack(p2);
						// insertBack a card from 1
						p2 = insertBackDeck(p2, copyCard(p1));
						p1 = removeCard(p1, head);
						// move p2
						p2 = moveCardBack(p2);
						tie = 1;
					}
				}
				// special condition
				if (deckSize(p2) <= 2)
				{
					// compare at second position
					warCondition = compareCard(search(p1, deckSize(p1)), search(p2, deckSize(p2)));
					if (warCondition == 1)
					{
						printf("Player 1 won that W-A-R!\n");
						// move p1
						p1 = moveCardBack(p1);

						// insertBack a card from 2
						p1 = insertBackDeck(p1, copyCard(p2));
						p2 = removeCard(p2, head);
						// move p1
						p1 = moveCardBack(p1);
						// insertBack a card from 2
						p1 = insertBackDeck(p1, copyCard(p2));
						p2 = removeCard(p2, head);
						tie = 1;
					}
					if (warCondition == 2)
					{
						// player 2 won
						printf("Player 2 won that W-A-R!\n");
						// insert card from p1 to p2
						p2 = insertBackDeck(p2, copyCard(p1));
						p1 = removeCard(p1, head);
						// move p2
						p2 = moveCardBack(p2);
						// insertBack a card from 1
						p2 = insertBackDeck(p2, copyCard(p1));
						p1 = removeCard(p1, head);
						// move p2
						p2 = moveCardBack(p2);
						tie = 1;
					}

				} // END OF SPECIAL

				warCondition = compareCard(p1, p2);

				if (warCondition == 1)
				{
					printf("Player 1 won that W-A-R!\n");
					while (!empty(profitDeck))
					{
						p1 = insertBackDeck(p1, copyCard(profitDeck));
						profitDeck = removeCard(profitDeck, head);
					}
					// move the head to the back of deck
					p1 = moveCardBack(p1);
					p1 = moveCardBack(p1);

					// Take the head of player 2s card make a copy insert it into p1s deck
					p1 = insertBackDeck(p1, copyCard(p2));
					p2 = removeCard(p2, head);
					// Take the head of player 2s card make a copy insert it into p1s deck
					p1 = insertBackDeck(p1, copyCard(p2));
					p2 = removeCard(p2, head);

					p1 = moveCardBack(p1);
					// Take the head of player 2s card make a copy insert it into p1s deck
					p1 = insertBackDeck(p1, copyCard(p2));
					p2 = removeCard(p2, head);

					tie = 1;
				}
				if (warCondition == 2)
				{
					printf("Player 2 won that W-A-R!\n");
					while (!empty(profitDeck))
					{
						p2 = insertBackDeck(p2, copyCard(profitDeck));
						profitDeck = removeCard(profitDeck, head);
					}
					// move the head to the back of deck
					p2 = moveCardBack(p2);
					p2 = moveCardBack(p2);

					// Take the head of player 2s card make a copy insert it into p1s deck
					p2 = insertBackDeck(p2, copyCard(p1));
					// remove the card from player 1
					p1 = removeCard(p1, head);
					// Take the head of player 1s card make a copy insert it into p1s deck
					p2 = insertBackDeck(p2, copyCard(p1));
					// remove the card from p1
					p1 = removeCard(p1, head);
					p2 = moveCardBack(p2);
					// Take the head of player 2s card make a copy insert it into p1s deck
					p2 = insertBackDeck(p2, copyCard(p1));
					// remove the card
					p1 = removeCard(p1, head);
					tie = 1;
				}
				// a tie
				if (warCondition == 0)
				{
					// insert 2 of p1
					profitDeck = insertBackDeck(profitDeck, copyCard(p1));
					p1 = removeCard(p1, head);
					profitDeck = insertBackDeck(profitDeck, copyCard(p1));
					p1 = removeCard(p1, head);
					// insert 2 of p2
					profitDeck = insertBackDeck(profitDeck, copyCard(p2));
					p2 = removeCard(p2, head);
					profitDeck = insertBackDeck(profitDeck, copyCard(p2));
					p2 = removeCard(p2, head);
				}
			} // END OF WAR WHILE LOOP
		}

		printf("Player 1 has %d cards.\n", deckSize(p1));
		printf("Player 2 has %d cards.\n", deckSize(p2));

	} // END OF WHILE LOOP FOR CARD COMPARISONS

	if (deckSize(p1) == 0)
	{
		cleanUp(p1);
		cleanUp(p2);

		return 2;
	}
	if (deckSize(p2) == 0)
	{
		cleanUp(p1);
		cleanUp(p2);

		return 1;
	}

} // End of play round

card_t *openCardDeck()
{
	FILE *fptr = fopen("deck.txt", "r");

	char *name = (char *)malloc(sizeof(char) * 20);

	if (name == NULL)
	{
		printf("Error in malloc...\n");
		exit(1);
	}

	card_t *head = NULL;

	int cardrank = 13;
	int tracker = 1;
	while (fscanf(fptr, "%s", name) == 1)
	{
		strcat(name, " ");

		if (tracker % 5 == 1)
		{
			strcat(name, "of Clubs");
			head = insertBackSetup(head, name, cardrank);
		}
		else if (tracker % 5 == 2)
		{
			strcat(name, "of Diamonds");
			head = insertBackSetup(head, name, cardrank);
		}
		else if (tracker % 5 == 3)
		{
			strcat(name, "of Hearts");
			head = insertBackSetup(head, name, cardrank);
		}
		else if (tracker % 5 == 4)
		{
			strcat(name, "of Spades");
			head = insertBackSetup(head, name, cardrank);
			tracker = 0;
			--cardrank;
		}

		++tracker;
	}

	free(name);
	fclose(fptr);

	return head;
}

card_t *insertBackSetup(card_t *node, char *name, int cardrank)
{

	if (empty(node)) // check to see if list is empty
	{
		node = (card_t *)malloc(sizeof(card_t));

		if (empty(node)) // check to see if malloc worked
		{
			printf("Did not allocate head successfully...");
			printf("Program Terminating...\n");
			exit(1);
		}
		else // otherwise populate data of new head node
		{
			node->type = (char *)malloc(sizeof(char) * 20);

			if (node->type == NULL)
			{
				printf("Error with malloc...\n");
				exit(1);
			}

			strcpy(node->type, name);
			node->rank = cardrank;
			node->nextptr = NULL; // must make new tail nextptr NULL!!!

			return node; // terminate
		}
	}

	// here we know that the list has at least one node

	card_t *head = node; // keep pointer to head since we will need to return this address

	while (node->nextptr != NULL) // traverse to tail
		node = node->nextptr;

	node->nextptr = (card_t *)malloc(sizeof(card_t));

	if (node->nextptr == NULL) // see if new tail was allocated successfully
	{
		printf("Did not allocate node successfully...");
		return head; // terminate if tail didn't get created
	}

	// populate new node
	node->nextptr->type = (char *)malloc(sizeof(char) * 20);

	if (node->nextptr->type == NULL)
	{
		printf("Error with malloc...\n");
		exit(1);
	}

	strcpy(node->nextptr->type, name);
	node->nextptr->rank = cardrank;
	node->nextptr->nextptr = NULL; // very important

	return head; // return head node since we need our starting point of the linked list
}

int empty(card_t *node)
{
	return (node == NULL); // return condition result
}
void cleanUp(card_t *head)
{
	card_t *temp;
	// traverse the list
	while (head != NULL)
	{
		temp = head;
		head = head->nextptr;
		free(temp);
	}
} // free memory to prevent memory leaks

int deckSize(card_t *head)
{
	// count number of nodes in the linked list
	int numOfCards = 0;
	// Traverse the list of nodes
	while (head != NULL)
	{
		// point the head to the next pointer to traverse
		head = head->nextptr;
		// increment
		numOfCards++;
	}
	return numOfCards;
} // End of deckSize
card_t *search(card_t *node, int spot)
{

	// search list for a specific spot in the card deck indexing is similar to array setup
	int counter = 0; // keep track
	if (spot == 0)
		return node;
	// Traverse the list
	while (node != NULL)
	{
		if (counter == spot) // if the counter is matched to the position
		{

			// return the node at that position
			return node;
		}
		// if not keep going
		node = node->nextptr;
		// increment the counter by 1
		counter++;
	}

} // End of Search
card_t *copyCard(card_t *node)
{
	// make a deep copy of a card (a node)
	// create a node
	card_t *copy = (card_t *)malloc(sizeof(card_t));
	if (empty(copy))
	{
		printf("Unable to Allocate!");
		exit(1);
	}
	// Allocate space needed
	copy->type = (char *)malloc(sizeof(char) * 20);

	// a card has a rank, type, in this case an address
	copy->rank = node->rank;
	strcpy(copy->type, node->type);
	copy->nextptr = NULL;
	// return the copy of the card (node)
	return copy;
} // End of copyCard
card_t *removeCard(card_t *node, int spot)
{
	card_t *temp = node;
	if (spot == 0)
	{
		node = temp->nextptr;
		free(temp->type);
		free(temp);
		return node;
	}

	// search list for a specific spot to remove the node in the card deck indexing is similar to array setup
	int counter = 0; // keep track
	// Traverse the list
	while (node != NULL)
	{
		if (counter == spot) // if the counter is matched to the position
		{
			node = temp->nextptr;
			free(temp->type);
			free(temp);
			return node;
		}
		// if not keep going
		node = node->nextptr;
		// increment the counter by 1
		counter++;
	}

} // End of removeCard
card_t *insertBackDeck(card_t *head, card_t *node)
{
	card_t *temp = head;
	if (empty(head))
	{
		node->nextptr = NULL;
		head = node;
		return head;
	}
	// traverse the list
	// while the next address is not pointed to null
	while (temp->nextptr != NULL)
	{
		temp = temp->nextptr;
	}
	// when NULL is reached set node to be inserted to establish a link
	temp->nextptr = node;
	node->nextptr = NULL;
	// return with new node inserted
	return head;

} // End of insertBackDeck
int compareCard(card_t *cardp1, card_t *cardp2)
{
	// compare cards using the rankings
	if (cardp1->rank > cardp2->rank)
	{
		return 1;
	}
	else if (cardp1->rank < cardp2->rank)
	{

		return 2;
	}
	else
		return 0;
}
card_t *moveCardBack(card_t *head)
{
	card_t *cardToBeMoved = head; // be moved to back
	card_t *temp = head;		  // traversal
	head = temp->nextptr;		  // set head equal to next
	while (temp->nextptr != NULL)
	{
		temp = temp->nextptr;
	}
	temp->nextptr = cardToBeMoved;
	cardToBeMoved->nextptr = NULL; // cut off link

	return head;
}
void displayR(card_t *head)
{

	if (head != NULL) // base case
	{

		printf("Type: %s\tRank: %d\n", head->type, head->rank);

		displayR(head->nextptr);
	}
}