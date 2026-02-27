#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_PLAYERS 2

typedef struct {
    char player_name[128];
    int thrown_out;
    int rolled_out;
    int playing_board[24];
    int dice[2];
}players_t;

int readPlayers(players_t players[], const int max_players) {
    int player_num = 0;
    while (player_num < max_players) {
        printf("%d. Spieler*in:", player_num+1);
        fflush(stdout);
        fgets(players[player_num].player_name, 128, stdin);
        players[player_num].player_name
        [strcspn(players[player_num].player_name, "\n")] = '\0';
        int new_check = 1;
        //check if the names are identical
        while (strcmp(players[0].player_name, players[1].player_name) == 0
            && new_check == 1) {
            fprintf(stderr, "Bitte unterschiedliche Spieler*innennamen eingeben\n");
            printf("%d. Spieler*in:", player_num+1);
            fflush(stdout);
            fgets(players[player_num].player_name, 128, stdin);
            players[player_num].player_name
            [strcspn(players[player_num].player_name, "\n")] = '\0';
            //if names aren´t identical anymore, continue by setting condition to False (0)
            if (strcmp(players[0].player_name, players[player_num].player_name) != 0) {
                new_check = 0;
                strcpy(players[1].player_name, players[player_num].player_name);
            }
        }
        player_num++;
    }
    return player_num;
}

void init_playing_board(players_t players[]) {
    //initalize the playing field with zero
    for (int i = 0; i < MAX_PLAYERS; i++) {
        memset(&players[i].playing_board, 0,
            sizeof(players[i].playing_board));
    }
    //initalize the playing field with values
    for (int i = 0; i < MAX_PLAYERS; i++) {
        players[i].playing_board[0] = 2;
        players[i].playing_board[11] = 5;
        players[i].playing_board[16] = 3;
        players[i].playing_board[18] = 5;
    }
}

void print_playing_board(players_t players[], const int which) {
    //print the playing field
    printf("Dein Spielfeld, auf geht's %s, Richtung ->\n", players[which].player_name);
    for (int j = 0; j < 24; j++) {
        printf(" %d", players[which].playing_board[j]);
    }
    printf("\n");
    //print the enemy playing field
    int p = 0;
    if (which == 1) {
        p = which-1;
        for (int i = 23; i != -1; i--) {
            printf(" %d", players[p].playing_board[i]);
        }
    }
    else {
        p = which+1;
        for (int i = 23; i != -1; i--) {
            printf(" %d", players[p].playing_board[i]);
        }
    }
    printf("\n");
    printf("Das Spielfeld von %s, Richtung <-\n", players[p].player_name);
}

int rolling_dices(int dice_num) {
    //two random dice values between 1 to 6
    int dice = (rand()%6)+1;
    printf("%d. Wuerfelzahl: %d\n", dice_num+1, dice);
    fflush(stdout);
    //return the dice value to calling function
    return dice;
}

int char_to_int(char typed_ch_int[128]) {
    //change string to a digit
    char *endptr;
    int const int_num = strtol(typed_ch_int, &endptr, 10);
    //if input isn´t a digit
    if (endptr == typed_ch_int) {
        fprintf(stderr, "Eingabefehler: Bitte eine gültige Zahl eingeben\n");
        //returns digit to the calling function that isn´t valid
        return -1;
    }
    //returns valid digit
    return int_num;
}

int check_valid_stone(players_t players[], const int players_index, int board_index) {
    board_index -= 1;
    if (players[players_index].playing_board[board_index] > 0) {
        return 1;
    }
    return 0;
}

int check_valid_moves_possible(players_t players[], const int players_index, int stone_move) {
    stone_move -= 1;
    //opponent index
    int opponent = 0;
    if (players_index == 0) {
        opponent = 1;
    }
    int count = 0;
    for (int i = 0; i < 18; i++) {
        count += players[players_index].playing_board[i];
    }
    //checking if inside the playing field
    //checking if the field is free with the first dice value
    if (stone_move + players[players_index].dice[0] <= 23
        && players[opponent].playing_board[stone_move + players[players_index].dice[0]] < 2) {
        //valid move returns 1
        return 1;
    }
    //checking if inside the playing field
    //checking if the field is free with the second dice value
    if (stone_move + players[players_index].dice[1] <= 23
        && players[opponent].playing_board[stone_move + players[players_index].dice[1]] < 2) {
        //valid move returns 1
        return 1;
    }
    //checking if rolling out is possible
    if ((stone_move + players[players_index].dice[0] > 23
        || stone_move + players[players_index].dice[1] > 23)
        && count == 0) {
        //valid move returns 1
        return 1;
    }
    //invalid move returns 0
    return 0;
}

int check_valid_move(players_t players[], const int players_index, int stone_move, int const dice_num) {
    stone_move -= 1;
    //checking if stone inside the playing field
    //checking if the field is occupied by an enemy stone
    if (players[players_index].playing_board[stone_move] + dice_num <= 23) {
        //enemy index
        int opponent = 0;
        if (players_index == 0) {
            opponent = 1;
        }
        //checking if the field where to move is occupied by an enemy stone
        if (players[opponent].playing_board[stone_move + dice_num] == 1) {
            //at the relevant field -1 opponent stone
            //opponent´s thrown out +1
            players[opponent].playing_board[stone_move + dice_num] -= 1;
            players[opponent].thrown_out += 1;
            //valid move returns 1
            return 1;
        }
        //if the field where to move is occupied by more than one enemy stone
        if (players[opponent].playing_board[stone_move + dice_num] > 1) {
            //invalid move returns 0
            return 0;
        }
        //if field free
        //valid move returns 1
        return 1;
    }
    int count = 0;
    for (int i = 0; i < 18; i++) {
        count += players[players_index].playing_board[i];
    }
    //if count 0, rolling out possible
    //returns 2 to calling function
    if (count == 0) {
        return 2;
    }
    //invalid move returns 0
    return 0;
}

int input_check_and_convert_moving_stone(players_t players[], int const players_index) {
    char stone_to_move[128] = {0};
    int num = 0;
    int check = 1;
    while (check) {
        printf("Welchen Stein moechtest Du bewegen (Index 1-24):");
        fflush(stdout);
        fgets(stone_to_move, 128, stdin);
        stone_to_move[strcspn(stone_to_move, "\n")] = '\0';
        //changing the string to an int
        num = char_to_int(stone_to_move);
        //checking if the digit is inside the valid area
        //correct the stone to move to the index
        if (num < 1 || num > 24
            || check_valid_stone(players, players_index, num) == 0
            || check_valid_moves_possible(players, players_index, num) == 0) {
            fprintf(stderr, "Eingabefehler 1\n");
        }
        else check = 0;
    }
    //correct the index of the stone to move
    num -= 1;
    return num;
}

int input_check_and_convert_used_dice_num(players_t players[], int const players_index, int *num2, int const moving_stone) {
    //which dice number to use
    char digit_number_move[128] = {0};
    int used_num = 0;
    int check = 1;
    while (check) {
        printf("Mit welcher Wuerfelzahl moechtest Du den Stein bewegen (1. oder 2.)\n");
        fflush(stdout);
        fgets(digit_number_move, 128, stdin);
        digit_number_move[strcspn(digit_number_move, "\n")] = '\0';
        //change string to int
        used_num = char_to_int(digit_number_move);
        //checking if the int could be valid
        if (used_num < 1 || used_num > 2) {
            fprintf(stderr, "Eingabefehler: 1 oder 2 eingeben\n");
        }
        //if no valid move, we get 0 in return
        else if (check_valid_move(players, players_index, moving_stone+1,
                players[players_index].dice[used_num-1]) == 0) {
            fprintf(stderr, "Eingabefehler: ungültiger Zug\n");
        }
        else check = 0;
    }
    //used num corrected index
    used_num -= 1;
    //adjust unused dice number
    if (used_num == 0) *num2 = 1;
    return used_num;
}


void how_many_moves(players_t players[], int *moves, const int num) {
    //check if push (e.g., 4 4 4 4)
    if (players[num].dice[0] == players[num].dice[1]){
        *moves = 4;
    }
    else *moves = 2;
}

void you_ve_x_moves_w_value_x(players_t players[], const int which, const int moves) {
    //how many playing moves and which dice numbers do you have?
    printf("Du hast %d Spielzuege mit den Wuerfelzahlen:", moves);
    if (moves == 2) {
        for (int t = 0; t < 2; t++) {
            printf(" %d", players[which].dice[t]);
        }
    }
    //if it´s a push
    else {
        for (int t = 0; t < 4; t++) {
            printf(" %d", players[which].dice[0]);
        }
    }
    printf("\n");
}

void get_stone_back_in(players_t players[], const int players_index, int *moves_counter) {
    //opponent index
    int opponent = 0;
    if (players_index == 0) {
        opponent = 1;
    }
    int const back_in_dice = rolling_dices(0);
    if (players[opponent].playing_board[0 + back_in_dice -1] < 2) {
        players[players_index].thrown_out--;
        players[players_index].playing_board[0 + back_in_dice -1]++;
        if (players[opponent].playing_board[0 + back_in_dice -1] == 1) {
            players[opponent].playing_board[0 + back_in_dice -1] = 0;
            players[opponent].thrown_out++;
        }
    }
    (*moves_counter)++;
}

int rolling_out_possible(players_t players[], const int index) {
    int count = 0;
    //checking if there is a stone in an anti-rolling-out area
    for (int i = 0; i < 18; i++) {
        count += players[index].playing_board[i];
    }
    if (players[index].thrown_out == 0
        && count == 0) {
        //rolling out valid, we return 1 to calling function
        return 1;
    }
    return 0;
}

int is_there_a_valid_move(players_t players[], const int players_index) {
    for (int i = 0; i < 24; i++) {
        if (players[players_index].playing_board[i] > 0) {
            if (check_valid_moves_possible(players, players_index, i+1) == 1) {
                return 1;
            }
        }
    }
    return 0;
}

int main(){
    //otherwise the random digit would always be the same
    srand(time(NULL));
    //initializing the players list
    players_t players[MAX_PLAYERS] = {0};
    //getting the player names by calling the function readPlayers
    readPlayers(players, MAX_PLAYERS);
    //initializing the playing field with zeros
    //initializing the playing filed with values
    init_playing_board(players);
    //which player to move - zero or one (one or zero)
    int which = 0;
    while (players[0].rolled_out != 15
        && players[1].rolled_out != 15) {
        //rolling the dices twice
        int dice_num = 0;
        while (dice_num < 2) {
            players[which].dice[dice_num] = rolling_dices(dice_num);
            dice_num++;
        }
        int moves = 0;
        //push or not, number of moves (2 or 4)
        how_many_moves(players, &moves, which);
        //print the playing field
        //print the opponents playing field
        print_playing_board(players, which);
        //how many moves and which digit numbers do you have?
        you_ve_x_moves_w_value_x(players, which, moves);
        //which stone would you like to move?
        int moves_counter = 0;
        while (moves_counter != moves) {
            if (players[which].thrown_out > 0) {
                get_stone_back_in(players, which, &moves_counter);
                continue;
            }
            //checking if there even exists a valid move
            if (is_there_a_valid_move(players, which) == 0) {
                fprintf(stderr,"Kein gültiger Zug möglich\n");
                break;
            }
            //changing string to int
            //checking if number inside valid area
            //adjusting the stone to the correct index
            //checking if there even is a stone on the chosen field
            int const moving_stone = input_check_and_convert_moving_stone(players, which);

            int unused_dice = 0;
            if (moves == 2
                && moves_counter == 0) {
                //which dice digit
                //changing string to int
                //checking if inside the valid area
                //adjusting to correct index
                //adjusting the unused dice
                int const used_num = input_check_and_convert_used_dice_num(players, which, &unused_dice, moving_stone);

                //checking if rolling out possible
                if (check_valid_move(players, which, moving_stone+1, players[which].dice[used_num])
                    == 2
                    && rolling_out_possible(players, which) == 1) {
                    //-1 stone at the requested field
                    players[which].playing_board[moving_stone] -= 1;
                    //+1 stone in rolled out (our fundamental while condition)
                    players[which].rolled_out++;
                }
                else {
                    //-1 stone at the requested field
                    players[which].playing_board[moving_stone] -= 1;
                    //+1 stone at the (taken position + the to move number)
                    players[which].playing_board[moving_stone + players[which].dice[used_num]] += 1;
                }
            }
            //if moved once and no push
            //automatic use of the other dice digit
            if (moves == 2
                && moves_counter == 1) {
                //checking if rolling out possible
                if (check_valid_move(players, which, moving_stone+1, players[which].dice[unused_dice]) == 2
                    && rolling_out_possible(players, which) == 1) {
                    //-1 stone at the requested field
                    players[which].playing_board[moving_stone] -= 1;
                    //+1 stone in rolled out (our fundamental while condition)
                    players[which].rolled_out++;
                    }
                else {
                    //-1 stone at the requested field
                    players[which].playing_board[moving_stone] -= 1;
                    //+1 stone at the (taken position + the to move number)
                    players[which].playing_board[moving_stone + players[which].dice[unused_dice]] += 1;
                }
            }
            //if push only moving as wished, as the digit number is always the same
            if (moves == 4) {
                //checking if rolling out possible
                if (check_valid_move(players, which, moving_stone+1, players[which].dice[0]) == 2
                    && rolling_out_possible(players, which) == 1) {
                    //-1 stone at the requested field
                    players[which].playing_board[moving_stone] -= 1;
                    //+1 stone in rolled out (our fundamental while condition)
                    players[which].rolled_out++;
                    }
                else {
                    players[which].playing_board[moving_stone] -= 1;
                    //+1 stone at the (taken position + the to move number)
                    //push, therefore, all the dice values are the same
                    players[which].playing_board[moving_stone + players[which].dice[0]] += 1;
                }
            }
            moves_counter++;
        }//2endwhile

        //change if players
        if (which == 0) {
            which = 1;
        }
        else {
            which = 0;
        }
    }//1endwhile
    //who is the winner?
    if (players[0].rolled_out == 15) {
        printf("%s, Du hast gewonnen\n%s, Du hast verloren\n",
            players[0].player_name, players[1].player_name);
    }
    else {
        printf("%s, Du hast gewonnen\n%s, Du hast verloren\n",
            players[1].player_name, players[0].player_name);
    }

    return 0;
}
