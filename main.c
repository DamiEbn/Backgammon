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

int readPlayers(players_t players[], int max_players) {
    int player_num = 0;
    while (player_num < max_players) {
        printf("%d. Spieler*in:", player_num+1);
        fflush(stdout);
        fgets(players[player_num].player_name, 128, stdin);
        players[player_num].player_name
        [strcspn(players[player_num].player_name, "\n")] = '\0';
        player_num++;
    }
    if (player_num != 2) {
        fprintf(stderr, "Fehlende Spieler:innen");
        exit(2);
    }
    //prüfen ob verschiedene namen
    if (strcmp(players[0].player_name, players[1].player_name) == 0) {
        fprintf(stderr, "Bitte unterschiedliche Spieler*innennamen eingeben");
        exit(1);
    }
    return player_num;
}

int rolling_dices(int dice_num) {
    //zwei zufällige würfelzahlen von 1-6
    int dice = (rand()%6)+1;
    printf("%d. Wuerfelzahl: %d\n", dice_num+1, dice);
    fflush(stdout);
    //zurückgeben der würfelzahlen
    return dice;
}

int char_to_int(char typed_ch_int[128]) {
    //umwandlung des strings in eine zahl
    char *endptr;
    int int_num = strtol(typed_ch_int, &endptr, 10);
    //wenn es sich bei der Eingabe um keine Zahl handelt
    if (endptr == typed_ch_int) {
        fprintf(stderr, "Eingabefehler\n");
        exit(3);
    }

    return int_num;
}

void check_and_convert_moving_stone(int *num) {
    //prüfen ob es sich um eine zahl im gültigkeitsberich handelt
    if (*num < 1 || *num > 24) {
        fprintf(stderr, "Eingabefehler\n");
        exit(4);
    }
    //zu fahrenden stein auf den index anpassen
    *num -= 1;
}

void check_and_convert_used_num(int *num) {
    //prüfen ob es sich um eine zahl im gültigkeitsberich handelt
    if (*num < 1 || *num > 2) {
        fprintf(stderr, "Eingabefehler\n");
        exit(6);
    }
    //used num auf den index anpassen
    *num -= 1;
}

void init_playing_board(players_t players[]) {
    //initalisierung des spielfelds mit 0
    for (int i = 0; i < MAX_PLAYERS; i++) {
        memset(&players[i].playing_board, 0,
            sizeof(players[i].playing_board));
    }
    //initalisierung des spielfelds mit werten
    for (int i = 0; i < MAX_PLAYERS; i++) {
        players[i].playing_board[0] = 2;
        players[i].playing_board[11] = 5;
        players[i].playing_board[16] = 3;
        players[i].playing_board[18] = 5;
    }
}

void how_many_moves(players_t players[], int *moves, const int num) {
    //prüfen ob push
    if (players[num].dice[0] == players[num].dice[1]){
        *moves = 4;
    }
    else *moves = 2;
}

void print_playing_board(players_t players[], const int which) {
    //drucken deines spielfelds
    printf("Dein Spielfeld, auf geht's %s, Richtung ->\n", players[which].player_name);
    for (int j = 0; j < 24; j++) {
        printf(" %d", players[which].playing_board[j]);
    }
    printf("\n");
    //drucken des gegnerischen Spielfelds
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

void you_ve_x_moves_w_value_x(players_t players[], const int which, const int moves) {
    //wieviele spielzüge und welche würfelzahl hast du?
    printf("Du hast %d Spielzuege mit den Wuerfelzahlen:", moves);
    if (moves == 2) {
        for (int t = 0; t < 2; t++) {
            printf(" %d", players[which].dice[t]);
        }
    }
    //im Falle eines Push
    else {
        for (int t = 0; t < 4; t++) {
            printf(" %d", players[which].dice[0]);
        }
    }
    printf("\n");
}

int main(){
    int thrown_out = 0;
    int rolled_out = 0;
    //sonst gibt er mir die gleiche random zahl
    srand(time(NULL));
    //deklarierung der players-liste
    players_t players[MAX_PLAYERS];
    //einlesen der spieler durch funktionsaufruf readPlayers
    readPlayers(players, MAX_PLAYERS);
    //initalisierung des spielfelds mit 0
    //initalisierung des spielfelds mit werten
    init_playing_board(players);
    //welche spieler*in ist am zug 0 oder 1 (1 oder 2)
    int which = 0;
    while (players[0].rolled_out != 15
        && players[1].rolled_out != 15) {
        //zweimal würfeln
        int dice_num = 0;
        while (dice_num < 2) {
            players[which].dice[dice_num] = rolling_dices(dice_num);
            dice_num++;
        }
        int moves = 0;
        //push oder nicht push, anzahl der züge, 2 oder 4
        how_many_moves(players, &moves, which);
        //drucken deines spielfelds
        //drucken des gegnerischen Spielfelds
        print_playing_board(players, which);
        //wieviele spielzüge und welche würfelzahl hast du?
        you_ve_x_moves_w_value_x(players, which, moves);
        //welchen stein möchtest Du bewegen?
        int moves_counter = 0;
        char stone_to_move[128] = {0};
        while (moves_counter != moves) {
            printf("Welchen Stein moechtest Du bewegen (Index 1-24):");
            fflush(stdout);
            fgets(stone_to_move, 128, stdin);
            stone_to_move[strcspn(stone_to_move, "\n")] = '\0';
            //umwandlung des strings in einen int
            int moving_stone = char_to_int(stone_to_move);
            //prüfen ob es sich um eine zahl im gültigkeitsberich handelt
            //und den führenden stein auf den index anpassen
            check_and_convert_moving_stone(&moving_stone);

            char digit_number_move[128] = {0};
            int unused_dice = 0;
            if (moves == 2
                && moves_counter == 0) {
                //welche würfelzahl
                printf("Mit welcher Wuerfelzahl moechtest Du den Stein bewegen (1. oder 2.)\n");
                fflush(stdout);
                fgets(digit_number_move, 128, stdin);
                digit_number_move[strcspn(digit_number_move, "\n")] = '\0';
                //umwandlung des strings in eine zahl
                int used_num = char_to_int(digit_number_move);
                //prüfen ob im gültigkeitsbereich und anpassen an index
                check_and_convert_used_num(&used_num);
                //-1 stein an der gewünschten stelle
                players[which].playing_board[moving_stone] -= 1;
                //+1 stein an der genommenen pos + die zu fahrende nummer
                players[which].playing_board[moving_stone + players[which].dice[used_num]] += 1;
            }
            //wenn schon eins gefahren und kein push
            //automatisch andere würfelzahl
            if (moves == 2
                && moves_counter == 1) {
                //-1 stein an der gewünschten stelle
                players[which].playing_board[moving_stone] -= 1;
                //+1 stein an der genommenen pos + die zu fahrende nummer
                players[which].playing_board[moving_stone + players[which].dice[unused_dice]] += 1;
            }
            //bei push nur wie gewünscht ziehen, da gleiche würfelzahl
            if (moves == 4) {
                players[which].playing_board[moving_stone] -= 1;
                //+1 stein an der genommenen pos + die zu fahrende nummer
                //push also alle dice gleich
                players[which].playing_board[moving_stone + players[which].dice[0]] += 1;
            }
            moves_counter++;
        }



        //spielerwechsel
        if (which == 0) {
            which = 1;
        }
        else {
            which = 0;
        }
        //check
    }

    //initialisieren des spielfelds
    /*int playing_board1 [28] = {0};
    int playing_board2 [28] = {0};
    //befüllen des ersten spielfelds mit werten
    playing_board1[2] = 2;
    playing_board1[13] = 5;
    playing_board1[18] = 3;
    playing_board1[20] = 5;

    for (int i = 0; i != 28; i++) {
        printf("%d", playing_board1[i]);
    }
    printf("\n");
    for (int i = 0; i != 28; i++) {
        printf("%d", playing_board2[i]);
    }
    printf("\n");
    return 0;*/
    return 0;
}
