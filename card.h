#include<stdio.h>

struct State {
    uint write;
    char move;
    uint next_card;
};

struct Card {
    uint id;
    struct State state[2];
    struct Card *next; 
};

struct Card *get_cards(char *file) {
    /*
    Collects the data for the instruction cards from the file and creates a
    singly linked list

    args:
        file - path to the file which has the data in the specified format
    
    ret:
        head - pointer to the first instruction card
    */

    FILE *fptr = fopen(file, "r");
    if(fptr == NULL) {
        printf("Error reading file\n");
        exit(0);
    }
    
    // creating the hault card
    struct Card *head = (struct Card*)malloc(sizeof(struct Card));
    head->id = 0;
    head->state[0].write = 0, head->state[0].move = 'S', head->state[0].next_card = 0;
    head->state[1].write = 0, head->state[1].move = 'S', head->state[1].next_card = 0;

    head->next = (struct Card*)malloc(sizeof(struct Card));
    struct Card *dummy = head->next, *prev;

    while(fscanf(fptr, "%d", &dummy->id) != EOF) {
        
        fscanf(fptr, "%d %c %d", &dummy->state[0].write, &dummy->state[0].move, &dummy->state[0].next_card);
        fscanf(fptr, "%d %c %d", &dummy->state[1].write, &dummy->state[1].move, &dummy->state[1].next_card);

        dummy->next = (struct Card*)malloc(sizeof(struct Card));
        prev = dummy;
        dummy = dummy->next;
    }

    prev->next = NULL;
    free(dummy);
    fclose(fptr);
    
    return head;
}

struct Card *search_card(struct Card* head, uint target) {
    /*
    Searches the singly linked list of cards to get the card with the target
    value as id

    args:
        head - pointer to the head of the singly linked list of cards
        target - id value of the card to find
    
    ret:
        if the card exists returns the pointer to the card
        else it returns 'NULL'
    */

    struct Card *dummy = head;

    while(dummy != NULL) {

        if(dummy->id == target)
            return dummy;
        dummy = dummy->next;
    }

    return NULL;
}

uint validate_cards(struct Card *head) {
    /*
    Validates the singly linked lists of cards

    args:
        head - the head of the singly linked lists of cards

    ret:
        if all cards are valid returns '1'
        else returns '0'
    */
    
    uint ret = 1;
    printf("\nValidating Cards...\n");

    struct Card *dummy = head->next;
    int count = 0;
    
    while(dummy != NULL) {

        uint is_valid_card = 1;
        count++;
        
        for(int i = 0; i < 2; ++i) {
            
            uint write = dummy->state[i].write == 0 || dummy->state[i].write == 1;
            uint move = dummy->state[i].move == 'L' || dummy->state[i].move == 'R' || dummy->state[i].move == 'S';
            struct Card *find = search_card(head, dummy->state[i].next_card);
            
            if(!write || !move || (!find && dummy->state[i].next_card != 0)) {
                is_valid_card = 0;
                break;
            }
        }

        if(is_valid_card) {
            printf("Card ID: %d is valid\n", dummy->id);
        
        } else {
            printf("Card ID: %d is not valid\n", dummy->id);
            ret = 0;
        }

        dummy = dummy->next;
    }

    printf("\n");

    if(count < 1) {
        printf("Invalid: No cards availabl\n");
        return 0;
    }
    return ret;
}

void print_cards(struct Card *head) {
    /*
    Prints all the cards in the singly linked lists

    args:
        head - pointer to the head of the card
    */

    printf("Printing cards...\n-----------------\n");

    struct Card *dummy = head->next;
    while(dummy != NULL) {

        printf("\nCard - %d\n", dummy->id);
        printf("S  W M N\n");
        printf("0  %d %c %d\n", dummy->state[0].write, dummy->state[0].move, dummy->state[0].next_card);
        printf("1  %d %c %d\n\n", dummy->state[1].write, dummy->state[1].move, dummy->state[1].next_card);
        
        dummy = dummy->next;
    }
    printf("-----------------\n");
}
