//Joseph Aschenbrenner
//Dr. Steinberg
//COP3502C Spring 2022
//Programming Assignment 3

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct card_s{
	int rank;    //number
	char * type; //type of card
	struct card_s * nextptr;
}card_t;

//function prototypes
void rules(); //display rules
int playRound(); //simulate round
card_t * openCardDeck(); //open the card deck and place into a linkedlist setup
card_t * insertBackSetup(card_t *node, char *name, int cardrank); //take card from orginial deck and place in back of linked list for setup of game
int empty(card_t * node); //check to see if linked list is empty
void cleanUp(card_t * head); //free memory to prevent memory leaks
int deckSize(card_t * head); //count number of nodes in the linked list
card_t * search(card_t * node, int spot); //search list for a specific spot in the card deck indexing is similar to array setup
card_t * copyCard(card_t * node); //make a deep copy of card
card_t * removeCard(card_t * node, int spot); //remove card from linkedlist
card_t * insertBackDeck(card_t *head, card_t *node); //place card at end of pile
int compareCard(card_t * cardp1, card_t * cardp2); //compare cards
card_t * moveCardBack(card_t *head); //place card at top of deck to the bottom of the deck

int main()
{
	int seed;
	printf("Enter Seed: ");
	scanf("%d", &seed);

	srand(seed); //seed set
	rules();

	int player; //1 or 2
	int result;

	printf("Would you like to be player 1 or 2?\n");
	printf("Enter 1 or 2: ");
	scanf("%d", &player); //choose player

	for(int game = 1; game <= 5; ++game) //simulate games
	{
		printf("Alright lets play game %d.\n", game);
		printf("Lets split the deck.\n");

		result = playRound(); //play game

		if((result == 1 && player == 1) || (result == 2 && player == 2)) //determine who won
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

card_t * openCardDeck()
{
	FILE *fptr = fopen("deck.txt", "r");

	char *name = (char *) malloc(sizeof(char) * 20);

	if (name == NULL)
	{
		printf("Error in malloc...\n");
		exit(1);
	}

	card_t * head = NULL;

	int cardrank = 13;
	int tracker = 1;
	while(fscanf(fptr, "%s", name) == 1)
	{
		strcat(name, " ");

		if(tracker % 5 == 1)
		{
			strcat(name, "of Clubs");
			head = insertBackSetup(head, name, cardrank);
		}
		else if(tracker % 5 == 2)
		{
			strcat(name, "of Diamonds");
			head = insertBackSetup(head, name, cardrank);
		}
		else if(tracker % 5 == 3)
		{
			strcat(name, "of Hearts");
			head = insertBackSetup(head, name, cardrank);
		}
		else if(tracker % 5 == 4)
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

card_t * insertBackSetup(card_t *node, char *name, int cardrank)
{

    if(empty(node)) //check to see if list is empty
    {
		node = (card_t *) malloc(sizeof(card_t));

		if(empty(node)) //check to see if malloc worked
		{
			printf("Did not allocate head successfully...");
			printf("Program Terminating...\n");
			exit(1);
		}
		else //otherwise populate data of new head node
		{
			node->type = (char *) malloc(sizeof(char) * 20);

			if(node->type == NULL)
			{
				printf("Error with malloc...\n");
				exit(1);
			}

			strcpy(node->type, name);
			node->rank = cardrank;
			node->nextptr = NULL; //must make new tail nextptr NULL!!!

			return node;  //terminate
		}

    }

	//here we know that the list has at least one node

	card_t *head = node; //keep pointer to head since we will need to return this address

	while(node->nextptr != NULL) //traverse to tail
		node = node->nextptr;

	node->nextptr = (card_t *) malloc(sizeof(card_t));

	if(node->nextptr == NULL) //see if new tail was allocated successfully
	{
		printf("Did not allocate node successfully...");
		return head; //terminate if tail didn't get created
	}

	//populate new node
	node->nextptr->type = (char *) malloc(sizeof(char) * 20);

	if(node->nextptr->type == NULL)
	{
		printf("Error with malloc...\n");
		exit(1);
	}

	strcpy(node->nextptr->type, name);
	node->nextptr->rank = cardrank;
	node->nextptr->nextptr = NULL; //very important

	return head; //return head node since we need our starting point of the linked list
}

int empty(card_t * node)
{
	return (node == NULL); //return condition result
}

card_t * search(card_t * node, int spot)
{
    if(spot == 0) //check to see if spot to remove is head
    {
        return node;
    }
    int count = 0;
    card_t *temp = NULL;
    temp = node;
    while(count<spot)
    {
        temp = temp->nextptr; //traverse linked list to get to spot
        count++;
    }
    return temp; //return head of list
}

card_t * copyCard(card_t * node)
{
    card_t *nodeCopy = NULL;
    nodeCopy = (card_t *) malloc(sizeof(card_t));

    if(empty(node)) //check to see if malloc worked
    {
        printf("Did not allocate head successfully...");
        printf("Program Terminating...\n");
        exit(1);
    }

    nodeCopy->rank = node->rank;
    nodeCopy->type = node->type;
    nodeCopy->nextptr = NULL; //isolate node
    return nodeCopy;
}

card_t * insertBackDeck(card_t *head, card_t *node)
{
    if(head == NULL) //if deck is empty, make node head
    {
        head = node;

        return head;
    }
    card_t *temp = head;
    while(temp->nextptr != NULL)
    {
        temp = temp->nextptr;
    }

    temp->nextptr = node; //assign node to temps nextptr at end of list
    return head;
}

card_t * removeCard(card_t * node, int spot)
{
    if(empty(node)) //check to see if list is empty
    {
        return NULL;
    }

    card_t *temp = search(node, spot);//get node to remove
    card_t *temp2 = node;

    if(temp == NULL) //check if resulting node is null
    {
        return node;
    }

    if(node == temp) //check if node is head
    {
        node = node->nextptr; //set head to next in list
        cleanUp(temp); //free old head
        return node;
    }

    while(temp2->nextptr != temp) //traverse to node before node to remove
    {
        temp2 = temp2->nextptr;
    }
    temp2->nextptr = temp->nextptr; //set nodes nextptr to node to remove's nextptr, essentially removing it

    cleanUp(temp); //free removed node
    return node;
}

int playRound()
{
    card_t * deckHead = openCardDeck();

    int size = deckSize(deckHead);
    printf("There are %d cards in the deck.\n", size);
    int spot;
    card_t * p1Head = NULL;
    card_t * p2Head = NULL;
    card_t * randCard = NULL;
    card_t * randCopy = NULL;
    while(!empty(deckHead))
    {
        spot = rand() % deckSize(deckHead); //get random spot within size of deck
        randCard = search(deckHead, spot);
        randCopy = copyCard(randCard); //get a copy of selected card
        p2Head = insertBackDeck(p2Head, randCopy);
        deckHead = removeCard(deckHead, spot); //remove card from original deck

        spot = rand() % deckSize(deckHead);//get different spot within deck
        randCard = search(deckHead, spot);
        randCopy = copyCard(randCard);
        p1Head = insertBackDeck(p1Head, randCopy);
        deckHead = removeCard(deckHead, spot);
    }

    int compareResult;
    while(!empty(p1Head) && !empty(p2Head))//only run when neither players' hands are empty
    {
        compareResult = compareCard(p1Head, p2Head);
        switch(compareResult)
        {
            case 1:
                printf("Player 1 won that round.\n");
                p1Head = moveCardBack(p1Head); //move players card to back of hand
                p1Head = insertBackDeck(p1Head, copyCard(p2Head)); //add won card to back of hand
                p2Head = removeCard(p2Head, 0);

                printf("Player 1 has %d cards.\n", deckSize(p1Head));
                printf("Player 2 has %d cards.\n", deckSize(p2Head));

                break;
            case 2:
                printf("Player 2 won that round.\n");

                p2Head = moveCardBack(p2Head);
                p2Head = insertBackDeck(p2Head, copyCard(p1Head));
                p1Head = removeCard(p1Head, 0);

                printf("Player 1 has %d cards.\n", deckSize(p1Head));
                printf("Player 2 has %d cards.\n", deckSize(p2Head));

                break;
            case 0:
                {
                    int wars = 0; //keeping track in case consecutive wars
                    card_t *temp = NULL;
                    card_t *temp2 = NULL;
                    if(p1Head->nextptr != NULL) //next few lines exist in case one player doesnt have enough cards
                    {
                        if(p1Head->nextptr->nextptr != NULL)
                        {
                            temp = p1Head->nextptr->nextptr;
                        }
                        else
                            temp = p1Head->nextptr;
                    }
                    else
                        temp = p1Head;

                    if(p2Head->nextptr != NULL)
                    {
                        if(p2Head->nextptr->nextptr != NULL)
                        {
                            temp2 = p2Head->nextptr->nextptr;
                        }
                        else
                            temp2 = p2Head->nextptr;
                    }
                    else
                        temp2 = p2Head;
                    while(compareResult == 0) //only run when we have a war
                    {
                        printf("Ugh oh...We have a tie! W-A-R!\n");
                        compareResult = compareCard(temp, temp2); //compare 'tiebraker' cards
                        if(compareResult == 0)
                        {
                            printf("Player 1 has %d cards.\n", deckSize(p1Head));
                            printf("Player 2 has %d cards.\n", deckSize(p2Head));
                        }
                        wars++;
                        if(temp->nextptr != NULL) //cases for if a player doesnt have enough cards for consecutive wars
                        {
                            if(temp->nextptr->nextptr != NULL)
                            {
                                temp = temp->nextptr->nextptr;
                            }
                            else
                                temp = temp->nextptr;
                        }
                        else
                            temp = temp;

                        if(temp2->nextptr != NULL)
                        {
                            if(temp2->nextptr->nextptr != NULL)
                            {
                                temp2 = temp2->nextptr->nextptr;
                            }
                            else
                                temp2 = temp2->nextptr;
                        }
                        else
                            temp2 = temp2;
                    }
                    if(compareResult == 1) //result of one means player 1 won
                    {
                        printf("Player 1 won that W-A-R!\n");

                        for(wars; wars > 0; wars--) //for however many consecutive wars there were, add cards as described
                        {
                            p1Head = moveCardBack(p1Head);
                            p1Head = moveCardBack(p1Head);
                            if(p2Head != NULL)
                            {
                                p1Head = insertBackDeck(p1Head, copyCard(p2Head));
                                p2Head = removeCard(p2Head, 0);
                            }
                            if(p2Head != NULL)
                            {
                                p1Head = insertBackDeck(p1Head, copyCard(p2Head));
                                p2Head = removeCard(p2Head, 0);
                            }
                        }


                        p1Head = moveCardBack(p1Head); //move last card to back

                        if(p2Head != NULL) //if card availible, move last card won to back
                        {
                            p1Head = insertBackDeck(p1Head, copyCard(p2Head));
                            p2Head = removeCard(p2Head, 0);
                        }

                    }
                    else if(compareResult == 2) //same process for player 2
                    {
                        printf("Player 2 won that W-A-R!\n");
                        for(wars; wars > 0; wars--)
                        {
                            if(p1Head != NULL)
                            {
                                p2Head = insertBackDeck(p2Head, copyCard(p1Head));
                                p1Head = removeCard(p1Head, 0);
                            }

                            if(p1Head != NULL)
                            {
                                p2Head = insertBackDeck(p2Head, copyCard(p1Head));
                                p1Head = removeCard(p1Head, 0);
                            }
                            p2Head = moveCardBack(p2Head);
                            p2Head = moveCardBack(p2Head);
                        }
                        if(p1Head != NULL)
                        {
                            p2Head = insertBackDeck(p2Head, copyCard(p1Head));
                            p1Head = removeCard(p1Head, 0);
                        }

                        p2Head = moveCardBack(p2Head);

                    }
                    printf("Player 1 has %d cards.\n", deckSize(p1Head));
                    printf("Player 2 has %d cards.\n", deckSize(p2Head));
                    break;
                }


        }


    }
    if(empty(p1Head)) //check if player 1 lost
    {
        return 2;
    }
    else if(empty(p2Head)) //check if player 2 lost
    {
        return 1;
    }

    return 0;
}

int compareCard(card_t * cardp1, card_t * cardp2)
{
    int card;
    card = cardp1->rank; //get card rank as int

    char * type1;
    type1 = cardp1->type; //get card type as string

    int card2;
    card2 = cardp2->rank;

    char * type2;
    type2 = cardp2->type;
    printf("Player 1 pulled out %s.      ", type1);
    printf("Player 2 pulled out %s.\n", type2);

    if(card > card2) //case if player 1 had higher card
        return 1;
    else if(card2 > card) //case if player 2 had higher card
        return 2;
    else //else its a tie/war
        return 0;
}

card_t * moveCardBack(card_t *head)
{
    card_t *temp;
    temp = head;

    while(temp->nextptr != NULL) //traverse to back of list
    {
        temp = temp->nextptr;
    }
    temp->nextptr = head; //head placed at back
    temp = head->nextptr; //make temp new head
    head->nextptr = NULL; //cut off circular list, making head last

    return temp;
}

int deckSize(card_t * head)
{
    int count = 0;
    while(head !=  NULL) //count until reach end of list
    {
        count++;
        //printf("%s\n", head->type);
        head = head->nextptr;
    }
    return count;
}

void cleanUp(card_t * head)
{
    free(head);
}
