#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

#define ROW 28
#define COL 92
#define TOTAL_CARDS 44
#define TOTAL_PLAYERS 4
#define DECK_ROW 2
#define DECK_COL 11
#define TYPE 2

/*...................
Author : Mahdy Mokhtari
file : CA2
Date : 1401 / 9 / 12
....................*/

void shuffle_card(int* card);
void game_display(char board[ROW][COL]);
void print_game_display(char board[ROW][COL]);
char card_passing(int card[TOTAL_CARDS], char player_cards[TOTAL_PLAYERS][DECK_ROW][DECK_COL], char board[ROW][COL]);
void card_throwing(int *turn, char board[ROW][COL], char *bakhshname, char player_cards[TOTAL_PLAYERS][DECK_ROW][DECK_COL], int ground[TOTAL_PLAYERS][TYPE]);
void press_enter(void);
void status_checking(int sets_point[TYPE], int *turn, char board[ROW][COL], int ground[TOTAL_PLAYERS][TYPE], char *bakhshname, int team_point[TYPE]);
void winner_check(int *set, int sets_point[TYPE], char board[ROW][COL], int team_point[TYPE]);


int main() {
    char board[ROW][COL];
    int team_point[TYPE] = {0};
    int sets_point[TYPE] = {0};
    int set = 1;
    game_display(board);
    print_game_display(board);
    printf("Click enter to start the game...\n");
    press_enter();
    for(set = 1; set <= 5; set++) {
        char player_cards[TOTAL_PLAYERS][DECK_ROW][DECK_COL]; /*[which player][what kind of card][card number]*/
        int card[TOTAL_CARDS] ={0};
        int ground[TOTAL_PLAYERS][TYPE] = {0};     /*first row: first card that was thrown second row: second ...
        first col:type of the card    second col: number of the card (all in ascii code)*/
        int turn = 0;
        shuffle_card(card);
        char bakhshname = card_passing(card, player_cards, board);
        while((team_point[0] < 6) && (team_point[1] < 6)) {
            card_throwing(&turn, board, &bakhshname, player_cards, ground);
            status_checking(sets_point ,&turn, board, ground, &bakhshname, team_point);
        }
        winner_check(&set, sets_point, board, team_point);
    }
}


void press_enter(void) {
    fflush(stdin);
    getc(stdin);
    fflush(stdin);
}

void shuffle_card(int* card) {
    int i, r, temp;
    for (temp = 0, i = 0; temp < 44; i++, temp++)
        card[temp] = i;
    srand(time(NULL));
    for (i = 43; i > 0; i--) {
        r = rand() % i;
        temp = card[i];
        card[i] = card[r];
        card[r] = temp;
    }
}

void game_display(char board[ROW][COL]) { /*designing the the field that we are going to play in */
    for(int i = 0; i < ROW; i++) {
        for(int j = 0; j < COL; j++) {
            if((i == 1 || i == 19) && (8 <= j && j <= 60)) {
                board[i][j] = '-';
            }
            else if((2 <= i && i <= 18) && (j == 8 || j == 60)) {
                board[i][j] = '|';
            }
            else if((i == 21 || i == 23 || i == 25) && (10 <= j && j <= 54)) {
                board[i][j] = '-';
            }
            else if((i == 22 || i == 24) && (10 <= j && j <= 54) && (j % 4 == 2)) {
                board[i][j] = '|';
            }
            else if((i == 26) && (12 <= j && j <= 48) && (j % 4 == 0)) {
                board[i][j] = ((j / 4) - 3) + 48;
            }
            else {
                board[i][j] = ' ';
            }
        }
    }
    board[0][34] = 'P' ; board[0][35] = '3';
    board[10][6] = 'P' ; board[10][7] = '2';
    board[10][61] = 'P'; board[10][62] = '4';
    board[20][34] = 'P'; board[20][35] = '1';
    board[26][52] = '1'; board[26][53] = '0';
    strncpy(&board[1][65], "team 1(current point): ", 24);
    strncpy(&board[19][65], "team 2(current point): ", 24);
    strncpy(&board[9][70], "team 1(sets won): ", 19);
    strncpy(&board[11][70], "team 2(sets won): ", 19);
}

void print_game_display(char board[ROW][COL]) { /*printing the designed bored that the palyers can see it and play*/
    system("cls");
    printf("\n");
    for(int i = 0; i < ROW; i++) {
        for(int j = 0; j < COL; j++) {
            printf("%c", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

char card_passing(int card[44], char player_cards[TOTAL_PLAYERS][DECK_ROW][DECK_COL], char board[ROW][COL]) { /*passing the cards to the players*/
    int sign = 0, value = 0, counter = 0;
    char bakhshname;
    while(counter < 44) {  /*P1: player_cards[0][][]  P2: player_cards[1][][]  P3: player_cards[2][][]  P4: player_cards[3][][]*/
        if(counter <= 4) {
            for(int i = 0; i < 5; i++) {
                sign = card[counter] / 11;
                player_cards[0][0][i] = sign + 65;
                board[22][(i * 4) + 12] = sign + 65;
                value = card[counter] % 11;
                player_cards[0][1][i] = value + 49;
                if(value + 49 == 58) {
                    board[24][(i * 4) + 12] = '1';
                    board[24][(i * 4) + 13] = '0';
                }
                else if(value + 49 == 59) {       /*the 10 and the 11 that we dont have ascii code for it*/
                    board[24][(i * 4) + 12] = '1';
                    board[24][(i * 4) + 13] = '1';
                }
                else {
                    board[24][(i * 4) + 12] = value + 49;
                }
                counter++;
            }
            print_game_display(board);
            printf("choose your bakhshname by knowing your first 5 cards(write A or B or C or D):\n");
            scanf("%c", &bakhshname);
            while((bakhshname != player_cards[0][0][0]) && (bakhshname != player_cards[0][0][1]) && (bakhshname != player_cards[0][0][2]) && (bakhshname != player_cards[0][0][3]) && (bakhshname != player_cards[0][0][4])) {
                fflush(stdin);
                printf("Not a valid input.\n");
                printf("choose your bakhshname by knowing your first 5 cards(write A or B or C or D):\n");
                scanf("%c", &bakhshname);
            }
            printf("bakhshname has been picked!\n");
            strncpy(&board[23][58], "bakhshname: ", 13);
            board[23][70] = bakhshname;
        }
        else if((counter <= 19)&& (counter >= 5)) {
            for (int i = 1; i < 4; i++) {
                for(int j = 0; j < 5; j++) {
                    sign = card[counter] / 11;
                    player_cards[i][0][j] = sign + 65; /*65 is for ascii table purposes*/
                    value = card[counter] % 11;
                    player_cards[i][1][j] = value + 49; /*49 is for ascii table purposes*/
                    counter++;
                }
            }
        }
        else if ((counter <= 35) && (counter >= 20)) {
            for (int i = 0; i < 4; i++) {
                for(int j = 5; j < 9; j++) {
                    sign = card[counter] / 11;
                    player_cards[i][0][j] = sign + 65; /*65 is for ascii table purposes*/
                    value = card[counter] % 11;
                    player_cards[i][1][j] = value + 49; /*49 is for ascii table purposes*/
                    if(i == 0) {
                        board[22][(j * 4) + 12] = sign + 65;
                        if(value + 49 == 58) {
                            board[24][(j * 4) + 12] = '1';
                            board[24][(j * 4) + 13] = '0';
                        }
                        else if(value + 49 == 59) {       /*the 10 and the 11 that we dont have ascii code for it*/
                            board[24][(j * 4) + 12] = '1';
                            board[24][(j * 4) + 13] = '1';
                        }
                        else {
                            board[24][(j * 4) + 12] = value + 49;
                        }
                    }
                    counter++;
                }
            }
        }
        else if ((counter <= 43) && (counter >= 36)) {
            for (int i = 0; i < 4; i++) {
                for(int j = 9; j < 11; j++) {
                    sign = card[counter] / 11;
                    player_cards[i][0][j] = sign + 65; /*65 is for ascii table purposes*/
                    value = card[counter] % 11;
                    player_cards[i][1][j] = value + 49; /*49 is for ascii table purposes*/
                    if(i == 0) {
                        board[22][(j * 4) + 12] = sign + 65;
                        if(value + 49 == 58) {
                            board[24][(j * 4) + 12] = '1';
                            board[24][(j * 4) + 13] = '0';
                        }
                        else if(value + 49 == 59) {       /*the 10 and the 11 that we dont have ascii code for it*/
                            board[24][(j * 4) + 12] = '1';
                            board[24][(j * 4) + 13] = '1';
                        }
                        else {
                            board[24][(j * 4) + 12] = value + 49;
                        }
                    }
                    counter++;
                }
            }
        }
    }
    return bakhshname;
    for(int i = 0; i < 44; i++) {
        printf("%d  ", card[i]);
    }
    /*printf("\n");
    for(int i=0;i<4;i++){
        for(int j=0;j<2;j++){                        /////
            for(int k=0;k<11;k++){
                printf("%c ", player_cards[i][j][k]);
            }
            printf("\n");
        }
        printf("\n\n");
    }*/
}

void card_throwing(int *turn, char board[ROW][COL], char *bakhshname, char player_cards[TOTAL_PLAYERS][DECK_ROW][DECK_COL], int ground[TOTAL_PLAYERS][TYPE]) {
    int counter = 0;
    int first_turn = *turn;
    
    do {                               /*B, F, H have been updated in emtiazi 2*/
        int max_nums_index = -1;
        int flag_sign = 0;
        int temp_value1 = -1, temp_value2 = 0;
        int flag_emtiazi;
        if(counter == 0) {
            if(*turn == 0) { /*A*/
                print_game_display(board);
                printf("Which card do you want to play from your deck?\n(write 0 or 1 or 2 ...or 10 and then press enter)\n");
                int card_taken = -1, flag = 0;
                flag = scanf("%d", &card_taken);

                /*strengethening the code to not accept wrong input*/
                while(flag != 1 || card_taken < 0 || card_taken > 11 || board[22][(card_taken * 4) + 12] == '-') {
                    if(flag != 1 || card_taken < 0 || card_taken > 11) {
                        printf("\nNot a valid input.\n");
                    }
                    else {
                        printf("\nYou dont have that card anymore!\n");
                    }
                    printf("Which card do you want to play from your deck?\n(write 0 or 1 or 2 ...or 10 and then press enter)\n");
                    fflush(stdin);
                    flag = scanf("%d", &card_taken);
                }
                int ind = (card_taken * 4) + 12;

                /*adjusting the board, the ground and the player_cards*/
                board[18][34] = board[22][ind];
                ground[counter][0] = player_cards[*turn][0][card_taken];
                board[22][ind] = '-';
                player_cards[*turn][0][card_taken] = '-';

                board[18][35] = board[24][ind];
                ground[counter][1] = player_cards[*turn][1][card_taken];
                board[24][ind] = '0';  
                player_cards[*turn][1][card_taken] = '0';

                board[18][36] = board[24][ind + 1];              
                board[24][ind + 1] = ' ';
                print_game_display(board);
                printf("Press enter to continue...\n");
                press_enter();
            }
            else { /*B*/
                temp_value1 = -1;
                temp_value2 = 0;
                max_nums_index = -1;
                for(int i = 0; i < 11; i++) {  /*throwing the maximum number in their cards(not bakhshname)*/
                    if((player_cards[*turn][0][i] != *bakhshname) && (player_cards[*turn][0][i] != '-')) {
                        temp_value2 = player_cards[*turn][1][i] - 48;
                        flag_sign = 1;
                        if(temp_value2 > temp_value1) {
                            temp_value1 = temp_value2;
                            max_nums_index = i;
                        }
                    }
                }
                if((flag_sign == 1) && (player_cards[*turn][1][max_nums_index] - 48 < 10)) {
                    flag_sign = 0;
                    temp_value1 = 12;
                    temp_value2 = 0;
                    max_nums_index = -1;
                    for(int i = 0; i < 11; i++) {  /*throwing the minimum number in their cards(not bakhshname)*/
                        if((player_cards[*turn][0][i] != *bakhshname) && (player_cards[*turn][0][i] != '-')) {
                            temp_value2 = player_cards[*turn][1][i] - 48;
                            flag_sign = 1;
                            if(temp_value2 < temp_value1) {
                                temp_value1 = temp_value2;
                                max_nums_index = i;
                            }
                        }
                    }
                }
                if(flag_sign == 0) {  /*throwing the maximum bakhshname*/
                    for(int i = 0; i < 11; i++) {
                        if(player_cards[*turn][0][i] == *bakhshname) {
                            temp_value2 = player_cards[*turn][1][i] - 48;
                            flag_sign = 1;
                            if(temp_value2 > temp_value1) {
                                temp_value1 = temp_value2;
                                max_nums_index = i;
                            }                            
                        }
                    }
                }

                /*adjusting the board, the ground and the player_cards*/
                ground[counter][0] = player_cards[*turn][0][max_nums_index];
                ground[counter][1] = player_cards[*turn][1][max_nums_index];
                if(*turn == 1) {
                    board[10][10] = player_cards[*turn][0][max_nums_index];
                    if(player_cards[*turn][1][max_nums_index] == 58) {
                        board[10][11] = '1';
                        board[10][12] = '0';
                    }
                    else if(player_cards[*turn][1][max_nums_index] == 59) {
                        board[10][11] = '1';
                        board[10][12] = '1';
                    }
                    else {
                        board[10][11] = player_cards[*turn][1][max_nums_index];
                    }
                }
                else if(*turn == 2) {
                    board[2][34] = player_cards[*turn][0][max_nums_index];
                    if(player_cards[*turn][1][max_nums_index] == 58) {
                        board[2][35] = '1';
                        board[2][36] = '0';
                    }
                    else if(player_cards[*turn][1][max_nums_index] == 59) {
                        board[2][35] = '1';
                        board[2][36] = '1';
                    }
                    else {
                        board[2][35] = player_cards[*turn][1][max_nums_index];
                    }
                }
                else if(*turn == 3) {
                    board[10][57] = player_cards[*turn][0][max_nums_index];
                    if(player_cards[*turn][1][max_nums_index] == 58) {
                        board[10][58] = '1';
                        board[10][59] = '0';
                    }
                    else if(player_cards[*turn][1][max_nums_index] == 59) {
                        board[10][58] = '1';
                        board[10][59] = '1';
                    }
                    else {
                        board[10][58] = player_cards[*turn][1][max_nums_index];
                    }
                }
                player_cards[*turn][0][max_nums_index] = '-';
                player_cards[*turn][1][max_nums_index] = '0';

            }
        }
        else if(counter == 1) {
            if(*turn == 0) { /*C*/
                print_game_display(board);
                int flag_ground = 0, flag_while;
                printf("Which card do you want to play from your deck?\n(write 0 or 1 or 2 ...or 10 and then press enter)\n");
                int card_taken = -1, flag = 0;
                flag = scanf("%d", &card_taken);

                /*strengethening the code to not accept wrong input*/
                while(flag != 1 || card_taken < 0 || card_taken > 11 || board[22][(card_taken * 4) + 12] == '-' || board[22][(card_taken * 4) + 12] != ground[0][0]) {
                    if(flag != 1 || card_taken < 0 || card_taken > 11) {
                        printf("\nNot a valid input.\n");
                    }
                    else if(board[22][(card_taken * 4) + 12] == '-'){
                        printf("\nYou dont have that card anymore!\n");
                    }
                    else {
                        for(int i = 0; i < 11; i++) {
                            if(player_cards[*turn][0][i] == ground[0][0]) {
                                flag_ground = 1;
                                break;
                            }
                        }
                        if(flag_ground == 0) {
                            break;
                        }
                        else {
                            printf("\nYou cant play that card!(not the same type as the first card thrown)\n");
                        }
                    }
                    printf("Which card do you want to play from your deck?\n(write 0 or 1 or 2 ...or 10 and then press enter)\n");
                    fflush(stdin);
                    flag = scanf("%d", &card_taken);
                }
                int ind = (card_taken * 4) + 12;

                /*adjusting the board, the ground and the player_cards*/
                board[18][34] = board[22][ind];
                ground[counter][0] = player_cards[*turn][0][card_taken];
                board[22][ind] = '-';
                player_cards[*turn][0][card_taken] = '-';

                board[18][35] = board[24][ind];
                ground[counter][1] = player_cards[*turn][1][card_taken];
                board[24][ind] = '0';  
                player_cards[*turn][1][card_taken] = '0';

                board[18][36] = board[24][ind + 1];              
                board[24][ind + 1] = ' ';
                print_game_display(board);
                printf("Press enter to continue...\n");
                press_enter();
            }
            else { /*D*/
                temp_value1 = -1;
                temp_value2 = 0;
                max_nums_index = -1;
                for(int i = 0; i < 11; i++) {  /*throwing the greatest similar card*/
                    if(player_cards[*turn][0][i] == ground[0][0]) {
                        temp_value2 = player_cards[*turn][1][i] - 48;
                        flag_sign = 1;
                        if(temp_value2 > temp_value1) {
                            temp_value1 = temp_value2;
                            max_nums_index = i;
                        }
                    }
                }
                if(flag_sign == 0) {  /*throwing the greatest bakhshname*/
                    for(int i = 0; i < 11; i++) {
                        if(player_cards[*turn][0][i] == *bakhshname) {
                            temp_value2 = player_cards[*turn][1][i] - 48;
                            flag_sign = 1;
                            if(temp_value2 > temp_value1) {
                                temp_value1 = temp_value2;
                                max_nums_index = i;
                            }                            
                        }
                    }
                }
                if(flag_sign == 0) {  /*throwing the least other type card(not bakhshname)*/
                    temp_value1 = 12;
                    for(int i = 0; i < 11; i++) {
                        if(player_cards[*turn][0][i] != '-') {
                            temp_value2 = player_cards[*turn][1][i] - 48;
                            flag_sign = 1;
                            if(temp_value2 < temp_value1) {
                                temp_value1 = temp_value2;
                                max_nums_index = i; /*in reality minimum number in their deck*/
                            }                            
                        }
                    }
                }

                /*adjusting the board, the ground and the player_cards*/
                ground[counter][0] = player_cards[*turn][0][max_nums_index];
                ground[counter][1] = player_cards[*turn][1][max_nums_index];
                if(*turn == 1) {
                    board[10][10] = player_cards[*turn][0][max_nums_index];
                    if(player_cards[*turn][1][max_nums_index] == 58) {
                        board[10][11] = '1';
                        board[10][12] = '0';
                    }
                    else if(player_cards[*turn][1][max_nums_index] == 59) {
                        board[10][11] = '1';
                        board[10][12] = '1';
                    }
                    else {
                        board[10][11] = player_cards[*turn][1][max_nums_index];
                    }
                }
                else if(*turn == 2) {
                    board[2][34] = player_cards[*turn][0][max_nums_index];
                    if(player_cards[*turn][1][max_nums_index] == 58) {
                        board[2][35] = '1';
                        board[2][36] = '0';
                    }
                    else if(player_cards[*turn][1][max_nums_index] == 59) {
                        board[2][35] = '1';
                        board[2][36] = '1';
                    }
                    else {
                        board[2][35] = player_cards[*turn][1][max_nums_index];
                    }
                }
                else if(*turn == 3) {
                    board[10][57] = player_cards[*turn][0][max_nums_index];
                    if(player_cards[*turn][1][max_nums_index] == 58) {
                        board[10][58] = '1';
                        board[10][59] = '0';
                    }
                    else if(player_cards[*turn][1][max_nums_index] == 59) {
                        board[10][58] = '1';
                        board[10][59] = '1';
                    }
                    else {
                        board[10][58] = player_cards[*turn][1][max_nums_index];
                    }
                }
                player_cards[*turn][0][max_nums_index] = '-';
                player_cards[*turn][1][max_nums_index] = '0';
            }
        }
        else if(counter == 2) {
            if(*turn == 0) { /*E*/
                print_game_display(board);
                int flag_ground = 0, flag_while;
                printf("Which card do you want to play from your deck?\n(write 0 or 1 or 2 ...or 10 and then press enter)\n");
                int card_taken = -1, flag = 0;
                flag = scanf("%d", &card_taken);

                /*strengethening the code to not accept wrong input*/
                while(flag != 1 || card_taken < 0 || card_taken > 11 || board[22][(card_taken * 4) + 12] == '-' || board[22][(card_taken * 4) + 12] != ground[0][0]) {
                    if(flag != 1 || card_taken < 0 || card_taken > 11) {
                        printf("\nNot a valid input.\n");
                    }
                    else if(board[22][(card_taken * 4) + 12] == '-'){
                        printf("\nYou dont have that card anymore!\n");
                    }
                    else {
                        for(int i = 0; i < 11; i++) {
                            if(player_cards[*turn][0][i] == ground[0][0]) {
                                flag_ground = 1;
                                break;
                            }
                        }
                        if(flag_ground == 0) {
                            break;
                        }
                        else {
                            printf("\nYou cant play that card!(not the same type as the first card thrown)\n");
                        }
                    }
                    printf("Which card do you want to play from your deck?\n(write 0 or 1 or 2 ...or 10 and then press enter)\n");
                    fflush(stdin);
                    flag = scanf("%d", &card_taken);
                }
                int ind = (card_taken * 4) + 12;

                /*adjusting the board, the ground and the player_cards*/
                board[18][34] = board[22][ind];
                ground[counter][0] = player_cards[*turn][0][card_taken];
                board[22][ind] = '-';
                player_cards[*turn][0][card_taken] = '-';

                board[18][35] = board[24][ind];
                ground[counter][1] = player_cards[*turn][1][card_taken];
                board[24][ind] = '0';  
                player_cards[*turn][1][card_taken] = '0';

                board[18][36] = board[24][ind + 1];              
                board[24][ind + 1] = ' ';
                print_game_display(board);
                printf("Press enter to continue...\n");
                press_enter();
            }
            else { /*F*/
                temp_value1 = -1;
                temp_value2 = 0;
                max_nums_index = -1;
                flag_emtiazi = 0;
                if((ground[0][0] == *bakhshname) && (ground[1][0] != *bakhshname)) {
                    flag_emtiazi = 1;
                }
                if((ground[0][0] == ground[1][0]) && (ground[1][1] < ground[0][1])) {
                    flag_emtiazi = 1;
                }
                if((ground[0][0] != ground[1][0]) && (ground[1][0] != *bakhshname)) {
                    flag_emtiazi = 1;
                }
                if(flag_emtiazi == 0) {
                    for(int i = 0; i < 11; i++) {  /*throwing the greatest similar card*/
                        if(player_cards[*turn][0][i] == ground[0][0]) {
                            temp_value2 = player_cards[*turn][1][i] - 48;
                            flag_sign = 1;
                            if(temp_value2 > temp_value1) {
                                temp_value1 = temp_value2;
                                max_nums_index = i;
                            }
                        }
                    }
                    if(flag_sign == 0) {  /*throwing the greatest bakhshname*/
                        for(int i = 0; i < 11; i++) {
                            if(player_cards[*turn][0][i] == *bakhshname) {
                                temp_value2 = player_cards[*turn][1][i] - 48;
                                flag_sign = 1;
                                if(temp_value2 > temp_value1) {
                                    temp_value1 = temp_value2;
                                    max_nums_index = i;
                                }                            
                            }
                        }
                    }
                    if(flag_sign == 0) {  /*throwing the least other type card(not bakhshname)*/
                        temp_value1 = 12;
                        for(int i = 0; i < 11; i++) {
                            if(player_cards[*turn][0][i] != '-') {
                                temp_value2 = player_cards[*turn][1][i] - 48;
                                flag_sign = 1;
                                if(temp_value2 < temp_value1) {
                                    temp_value1 = temp_value2;
                                    max_nums_index = i; /*in reality minimum number in their deck*/
                                }                            
                            }
                        }
                    }
                }
                else if(flag_emtiazi == 1) {
                    temp_value1 = 12;
                    for(int i = 0; i < 11; i++) {  /*throwing the least similar card*/
                        if(player_cards[*turn][0][i] == ground[0][0]) {
                            temp_value2 = player_cards[*turn][1][i] - 48;
                            flag_sign = 1;
                            if(temp_value2 < temp_value1) {
                                temp_value1 = temp_value2;
                                max_nums_index = i; /*in reality minimum number in their deck*/
                            }
                        }
                    }
                    if(flag_sign == 0) {  /*throwing the least other type card(not bakhshname)*/
                        temp_value1 = 12;
                        for(int i = 0; i < 11; i++) { 
                            if((player_cards[*turn][0][i] != '-') && (player_cards[*turn][0][i] != *bakhshname)) {
                                temp_value2 = player_cards[*turn][1][i] - 48;
                                flag_sign = 1;
                                if(temp_value2 < temp_value1) {
                                    temp_value1 = temp_value2;
                                    max_nums_index = i; /*in reality minimum number in their deck*/
                                }                            
                            }
                        }
                    }   
                    if(flag_sign == 0) {  /*throwing the greatest bakhshname*/
                        for(int i = 0; i < 11; i++) {  
                            if(player_cards[*turn][0][i] == *bakhshname) {
                                temp_value2 = player_cards[*turn][1][i] - 48;
                                flag_sign = 1;
                                if(temp_value2 < temp_value1) {
                                    temp_value1 = temp_value2;
                                    max_nums_index = i; /*in reality minimum number in their deck*/
                                }                            
                            }
                        }
                    }
                }

                /*adjusting the board, the ground and the player_cards*/
                ground[counter][0] = player_cards[*turn][0][max_nums_index];
                ground[counter][1] = player_cards[*turn][1][max_nums_index];
                if(*turn == 1) {
                    board[10][10] = player_cards[*turn][0][max_nums_index];
                    if(player_cards[*turn][1][max_nums_index] == 58) {
                        board[10][11] = '1';
                        board[10][12] = '0';
                    }
                    else if(player_cards[*turn][1][max_nums_index] == 59) {
                        board[10][11] = '1';
                        board[10][12] = '1';
                    }
                    else {
                        board[10][11] = player_cards[*turn][1][max_nums_index];
                    }
                }
                else if(*turn == 2) {
                    board[2][34] = player_cards[*turn][0][max_nums_index];
                    if(player_cards[*turn][1][max_nums_index] == 58) {
                        board[2][35] = '1';
                        board[2][36] = '0';
                    }
                    else if(player_cards[*turn][1][max_nums_index] == 59) {
                        board[2][35] = '1';
                        board[2][36] = '1';
                    }
                    else {
                        board[2][35] = player_cards[*turn][1][max_nums_index];
                    }
                }
                else if(*turn == 3) {
                    board[10][57] = player_cards[*turn][0][max_nums_index];
                    if(player_cards[*turn][1][max_nums_index] == 58) {
                        board[10][58] = '1';
                        board[10][59] = '0';
                    }
                    else if(player_cards[*turn][1][max_nums_index] == 59) {
                        board[10][58] = '1';
                        board[10][59] = '1';
                    }
                    else {
                        board[10][58] = player_cards[*turn][1][max_nums_index];
                    }
                }
                player_cards[*turn][0][max_nums_index] = '-';
                player_cards[*turn][1][max_nums_index] = '0';
            }
        }
        else if(counter == 3) {
            if(*turn == 0) { /*G*/
                print_game_display(board);
                int flag_ground = 0, flag_while;
                printf("Which card do you want to play from your deck?\n(write 0 or 1 or 2 ...or 10 and then press enter)\n");
                int card_taken = -1, flag = 0;
                flag = scanf("%d", &card_taken);

                /*strengethening the code to not accept wrong input*/
                while(flag != 1 || card_taken < 0 || card_taken > 11 || board[22][(card_taken * 4) + 12] == '-' || board[22][(card_taken * 4) + 12] != ground[0][0]) {
                    if(flag != 1 || card_taken < 0 || card_taken > 11) {
                        printf("\nNot a valid input.\n");
                    }
                    else if(board[22][(card_taken * 4) + 12] == '-'){
                        printf("\nYou dont have that card anymore!\n");
                    }
                    else {
                        for(int i = 0; i < 11; i++) {
                            if(player_cards[*turn][0][i] == ground[0][0]) {
                                flag_ground = 1;
                                break;
                            }
                        }
                        if(flag_ground == 0) {
                            break;
                        }
                        else {
                            printf("\nYou cant play that card!(not the same type as the first card thrown)\n");
                        }
                    }
                    printf("Which card do you want to play from your deck?\n(write 0 or 1 or 2 ...or 10 and then press enter)\n");
                    fflush(stdin);
                    flag = scanf("%d", &card_taken);
                }
                int ind = (card_taken * 4) + 12;

                /*adjusting the board, the ground and the player_cards*/
                board[18][34] = board[22][ind];
                ground[counter][0] = player_cards[*turn][0][card_taken];
                board[22][ind] = '-';
                player_cards[*turn][0][card_taken] = '-';

                board[18][35] = board[24][ind];
                ground[counter][1] = player_cards[*turn][1][card_taken];
                board[24][ind] = '0';  
                player_cards[*turn][1][card_taken] = '0';

                board[18][36] = board[24][ind + 1];              
                board[24][ind + 1] = ' ';
                print_game_display(board);
                printf("Press enter to continue...\n");
                press_enter();
            }
            else { /*H*/
                temp_value1 = -1;
                temp_value2 = 0;
                max_nums_index = -1;
                flag_emtiazi = 0;
                if((ground[0][0] != *bakhshname) && (ground[1][0] == *bakhshname) && (ground[2][0] != *bakhshname)) {
                    flag_emtiazi = 1;
                }
                if((ground[0][0] == *bakhshname) && (ground[1][0] == *bakhshname) && (ground[2][0] != *bakhshname) && (ground[1][1] > ground[0][1])) {
                    flag_emtiazi = 1;
                }
                if((ground[0][0] != *bakhshname) && (ground[1][0] == *bakhshname) && (ground[2][0] == *bakhshname) && (ground[1][1] > ground[2][1])) {
                    flag_emtiazi = 1;
                }
                if((ground[0][0] == ground[1][0] == ground[2][0]) && (ground[1][1] > ground[0][1]) && (ground[1][1] > ground[2][1])) {
                    flag_emtiazi = 1;
                }
                if((ground[0][0] == ground[1][0]) && (ground[2][0] != *bakhshname) && (ground[1][1] > ground[0][1])) {
                    flag_emtiazi = 1;
                }
                if(flag_emtiazi == 0) {
                    for(int i = 0; i < 11; i++) {  /*throwing the greatest similar card*/
                        if(player_cards[*turn][0][i] == ground[0][0]) {
                            temp_value2 = player_cards[*turn][1][i] - 48;
                            flag_sign = 1;
                            if(temp_value2 > temp_value1) {
                                temp_value1 = temp_value2;
                                max_nums_index = i;
                            }
                        }
                    }
                    if(flag_sign == 0) {  /*throwing the greatest bakhshname*/
                        for(int i = 0; i < 11; i++) {
                            if(player_cards[*turn][0][i] == *bakhshname) {
                                temp_value2 = player_cards[*turn][1][i] - 48;
                                flag_sign = 1;
                                if(temp_value2 > temp_value1) {
                                    temp_value1 = temp_value2;
                                    max_nums_index = i;
                                }                            
                            }
                        }
                    }
                    if(flag_sign == 0) {  /*throwing the least other type card(not bakhshname)*/
                        temp_value1 = 12;
                        for(int i = 0; i < 11; i++) {
                            if(player_cards[*turn][0][i] != '-') {
                                temp_value2 = player_cards[*turn][1][i] - 48;
                                flag_sign = 1;
                                if(temp_value2 < temp_value1) {
                                    temp_value1 = temp_value2;
                                    max_nums_index = i; /*in reality minimum number in their deck*/
                                }                            
                            }
                        }
                    }
                }
                else if(flag_emtiazi == 1) {
                    temp_value1 = 12;
                    for(int i = 0; i < 11; i++) {  /*throwing the similar card*/
                        if(player_cards[*turn][0][i] == ground[0][0]) {
                            temp_value2 = player_cards[*turn][1][i] - 48;
                            flag_sign = 1;
                            if(temp_value2 < temp_value1) {
                                temp_value1 = temp_value2;
                                max_nums_index = i; /*in reality minimum number in their deck*/
                            }
                        }
                    }
                    if(flag_sign == 0) {  /*throwing the other type card(not bakhshname)*/
                        temp_value1 = 12;
                        for(int i = 0; i < 11; i++) { 
                            if((player_cards[*turn][0][i] != '-') && (player_cards[*turn][0][i] != *bakhshname)) {
                                temp_value2 = player_cards[*turn][1][i] - 48;
                                flag_sign = 1;
                                if(temp_value2 < temp_value1) {
                                    temp_value1 = temp_value2;
                                    max_nums_index = i; /*in reality minimum number in their deck*/
                                }                            
                            }
                        }
                    }   
                    if(flag_sign == 0) {  /*throwing the bakhshname*/
                        for(int i = 0; i < 11; i++) {  
                            if(player_cards[*turn][0][i] == *bakhshname) {
                                temp_value2 = player_cards[*turn][1][i] - 48;
                                flag_sign = 1;
                                if(temp_value2 < temp_value1) {
                                    temp_value1 = temp_value2;
                                    max_nums_index = i; /*in reality minimum number in their deck*/
                                }                            
                            }
                        }
                    }
                }

                /*adjusting the board, the ground and the player_cards*/
                ground[counter][0] = player_cards[*turn][0][max_nums_index];
                ground[counter][1] = player_cards[*turn][1][max_nums_index];
                if(*turn == 1) {
                    board[10][10] = player_cards[*turn][0][max_nums_index];
                    if(player_cards[*turn][1][max_nums_index] == 58) {
                        board[10][11] = '1';
                        board[10][12] = '0';
                    }
                    else if(player_cards[*turn][1][max_nums_index] == 59) {
                        board[10][11] = '1';
                        board[10][12] = '1';
                    }
                    else {
                        board[10][11] = player_cards[*turn][1][max_nums_index];
                    }
                }
                else if(*turn == 2) {
                    board[2][34] = player_cards[*turn][0][max_nums_index];
                    if(player_cards[*turn][1][max_nums_index] == 58) {
                        board[2][35] = '1';
                        board[2][36] = '0';
                    }
                    else if(player_cards[*turn][1][max_nums_index] == 59) {
                        board[2][35] = '1';
                        board[2][36] = '1';
                    }
                    else {
                        board[2][35] = player_cards[*turn][1][max_nums_index];
                    }
                }
                else if(*turn == 3) {
                    board[10][57] = player_cards[*turn][0][max_nums_index];
                    if(player_cards[*turn][1][max_nums_index] == 58) {
                        board[10][58] = '1';
                        board[10][59] = '0';
                    }
                    else if(player_cards[*turn][1][max_nums_index] == 59) {
                        board[10][58] = '1';
                        board[10][59] = '1';
                    }
                    else {
                        board[10][58] = player_cards[*turn][1][max_nums_index];
                    }
                }
                player_cards[*turn][0][max_nums_index] = '-';
                player_cards[*turn][1][max_nums_index] = '0';
            }
        }
        if(*turn == 3) {
            *turn = 0;
        }
        else {
            (*turn)++;
        }
        counter++;
    }while(*turn != first_turn);
}

void status_checking(int sets_point[TYPE], int *turn, char board[ROW][COL], int ground[TOTAL_PLAYERS][TYPE], char *bakhshname, int team_point[TYPE]) {
    int flag = 0, temp_value1 = -1, temp_value2 = 0, max_nums_index = -1;
    print_game_display(board);
    printf("Press enter to calculate the score...\n");
    press_enter();

    /*checking to see which player won the round*/
    for(int i = 0; i < 4; i++) {
        if(ground[i][0] == *bakhshname) {
            flag = 1;
            temp_value2 = ground[i][1] - 48;
            if(temp_value2 > temp_value1) {
                temp_value1 = temp_value2;
                max_nums_index = i;
            }
        }
    }
    if(flag == 0) {
        for(int i = 0; i < 4; i++) {
            if(ground[i][0] == ground[0][0]) {
                flag = 1;
                temp_value2 = ground[i][1] - 48;
                if(temp_value2 > temp_value1) {
                    temp_value1 = temp_value2;
                    max_nums_index = i;
                }
            }
        }
    }

    /*updating the *turn value*/
    *turn = (max_nums_index + (*turn)) % 4;
    if((*turn) % 2 == 0) {
        team_point[0]++;
    }
    else {
        team_point[1]++;
    }

    /*adjusting the board, the ground and the player_cards*/
    board[18][34] = ' '; board[18][35] = ' '; board[18][36] = ' '; 
    board[10][10] = ' '; board[10][11] = ' '; board[10][12] = ' ';
    board[2][34] = ' ' ; board[2][35] = ' ' ; board[2][36] = ' ' ;
    board[10][57] = ' '; board[10][58] = ' '; board[10][59] = ' ';
    board[1][88] = team_point[0] + 48;
    board[19][88] = team_point[1] + 48;

    print_game_display(board);

    /*showing the winner and ending the game*/
    if(team_point[0] == 6 || team_point[1] == 6) {
        if(team_point[0] == 6) {
            printf("Congratulations! team 1 has won the set.\n\n");
            if(team_point[1] == 0) {
                sets_point[0] += 2;
            }
            else {
                sets_point[0]++;
            }
        }
        else {
            printf("Congratulations! team 2 has won the set.\n\n");
            if(team_point[0] == 0) {
                sets_point[1] += 2;
            }
            else {
                sets_point[1]++;
            }
        }
        return;
    }

    printf("Press enter to go to the next round...\n");
    press_enter();
}

void winner_check(int *set, int sets_point[TYPE], char board[ROW][COL], int team_point[TYPE]) {
    team_point[0] = 0; team_point[1] = 0;
    board[9][88] = sets_point[0] + 48;
    board[11][88] = sets_point[1] + 48;
    board[1][88] = '0'; board[19][88] = '0';
    board[23][70] = ' ';
    if(*set < 5) {
        for(int j = 22; j <= 24; j+=2) {
            for(int i = 11; i < 54; i++) {
                if(i % 4 != 2) {
                    board[j][i] = ' ';
                }
            }
        }
        printf("Press enter to go to the next set...\n");
        press_enter();
    }
    else{
        if (sets_point[0] > sets_point[1]) {
            printf("Congrats! Team 1 has won the game.\n\n");
        }
        else if(sets_point[0] < sets_point[1]) {
            printf("Congrats! Team 2 has won the game.\n\n");
        }
        else {
            printf("Great game! It's a tie\n\n");
        }
    }
}