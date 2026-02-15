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
    int check = 1;
    while (player_num < max_players
        || check == 1) {
        printf("%d. Spieler*in:", player_num+1);
        fflush(stdout);
        fgets(players[player_num].player_name, 128, stdin);
        players[player_num].player_name
        [strcspn(players[player_num].player_name, "\n")] = '\0';
        player_num++;
        int new_check = 1;
        //prüfen ob namen gleich
        while (strcmp(players[0].player_name, players[1].player_name) == 0
            && new_check == 1) {
            fprintf(stderr, "Bitte unterschiedliche Spieler*innennamen eingeben\n");
            printf("%d. Spieler*in:", player_num);
            fflush(stdout);
            fgets(players[player_num].player_name, 128, stdin);
            players[player_num].player_name
            [strcspn(players[player_num].player_name, "\n")] = '\0';
            //wenn namen nicht mehr gleich geht es weiter
            if (strcmp(players[0].player_name, players[player_num].player_name) != 0) {
                new_check = 0;
                strcpy(players[1].player_name, players[player_num].player_name);
            }
        }
        check = 0;
    }
    /*if (player_num != 2) {
        fprintf(stderr, "Fehlende Spieler:innen\n");
        exit(2);
    }*/
    //prüfen ob verschiedene namen

    return player_num;
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
        fprintf(stderr, "Eingabefehler: Bitte eine gültige Zahl eingeben\n");
        //gibt zahl außerhalb des gültigkeitsbereich zurück
        return -1;
    }
    //gibt zahl zurück
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
    //index der gegner*in
    int opponent = 0;
    if (players_index == 0) {
        opponent = 1;
    }
    int count = 0;
    for (int i = 0; i < 18; i++) {
        count += players[players_index].playing_board[i];
    }
    //prüfen ob innerhalb des spielfelds und ob feld frei mit erster würfelzahl
    if (players[players_index].playing_board[stone_move] + players[players_index].dice[0] <= 23
        && players[opponent].playing_board[stone_move + players[players_index].dice[0]] < 2) {
        //gültiger zug gibt 1 zurück
        return 1;
    }
    //prüfen ob innerhalb des spielfelds und ob feld frei mit zweiter würfelzahl
    if (players[players_index].playing_board[stone_move] + players[players_index].dice[1] <= 23
        && players[opponent].playing_board[stone_move + players[players_index].dice[1]] < 2) {
        //gültiger zug gibt 1 zurück
        return 1;
    }
    //prüfen ob rauswürfeln möglich
    if (players[players_index].playing_board[stone_move] + players[players_index].dice[0] > 23
        || players[players_index].playing_board[stone_move] + players[players_index].dice[0] > 23
        && count == 0) {
        return 1;
    }
    return 0;
}

int check_valid_move(players_t players[], const int players_index, int stone_move, int const dice_num) {
    stone_move -= 1;
    //prüfen ob stein innerhalb des spielfelds und ob feld besetzt durch gegner*innen stein
    if (players[players_index].playing_board[stone_move] + dice_num <= 23) {
        //index der gegner*in
        int opponent = 0;
        if (players_index == 0) {
            opponent = 1;
        }
        //prüfen ob das feld zu dem gezogenen wird durch einen gegner*innen stein besetzt ist
        if (players[opponent].playing_board[stone_move + dice_num] == 1) {
            //dann wird am entsprechenden feld 1 stein der gegner*in abgezogen
            //und in rausgeschmissen hinzugefügt
            players[opponent].playing_board[stone_move + dice_num] -= 1;
            players[opponent].thrown_out += 1;
            //gültiger zug
            return 1;
        }
        //wenn das feld durch mehr als einen stein der gegner*in besetzt ist
        if (players[opponent].playing_board[stone_move + dice_num] > 1) {
            //ungültiger zug
            return 0;
        }
        //wenn feld frei
        //gültiger zug
        return 1;
    }
    int count = 0;
    for (int i = 0; i < 18; i++) {
        count += players[players_index].playing_board[i];
    }
    //wenn return 2 dann rauswürfeln möglich
    if (count == 0) {
        return 2;
    }
    //ungültiger zug dann 0
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
        //umwandlung des strings in einen int
        num = char_to_int(stone_to_move);
        //prüfen ob es sich um eine zahl im gültigkeitsberich handelt
        //und den führenden stein auf den index anpassen
        //prüfen ob es sich um eine zahl im gültigkeitsberich handelt
        if (num < 1 || num > 24
            || check_valid_stone(players, players_index, num) == 0
            || check_valid_moves_possible(players, players_index, num)) {
            fprintf(stderr, "Eingabefehler\n");
        }
        else check = 0;
    }
    //zu fahrenden stein auf den index anpassen
    num -= 1;
    return num;
}

int input_check_and_convert_used_dice_num(players_t players[], int const players_index, int *num2, int const moving_stone) {
    //welche würfelzahl
    char digit_number_move[128] = {0};
    int used_num = 0;
    int check = 1;
    while (check) {
        printf("Mit welcher Wuerfelzahl moechtest Du den Stein bewegen (1. oder 2.)\n");
        fflush(stdout);
        fgets(digit_number_move, 128, stdin);
        digit_number_move[strcspn(digit_number_move, "\n")] = '\0';
        //umwandlung des strings in eine zahl
        used_num = char_to_int(digit_number_move);
        //prüfen ob es sich um eine zahl im gültigkeitsberich handelt
        if (used_num < 1 || used_num > 2) {
            fprintf(stderr, "Eingabefehler: 1 oder 2 eingeben\n");
        }
        //wenn kein gültiger zug kommt 0 zurück
        if (check_valid_move(players, players_index, moving_stone, used_num) == 0) {
            fprintf(stderr, "Eingabefehler: ungültiger Zug");
        }
        else check = 0;
    }
    //used num auf den index anpassen
    used_num -= 1;
    //anpassen des ungenutzten würfels
    if (used_num == 0) *num2 = 1;
    return used_num;
}


void how_many_moves(players_t players[], int *moves, const int num) {
    //prüfen ob push
    if (players[num].dice[0] == players[num].dice[1]){
        *moves = 4;
    }
    else *moves = 2;
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
        while (moves_counter != moves) {
            //umwandlung des strings in einen int
            //prüfen ob es sich um eine zahl im gültigkeitsberich handelt
            //und den führenden stein auf den index anpassen
            //prüfen ob stein auf dem geüwnschten feld liegt
            int const moving_stone = input_check_and_convert_moving_stone(players, which);

            int unused_dice = 0;
            if (moves == 2
                && moves_counter == 0) {
                //welche würfelzahl
                //umwandlung des strings in eine zahl
                //prüfen ob im gültigkeitsbereich und anpassen an index und anpassen des ungenutzen würfels
                int const used_num = input_check_and_convert_used_dice_num(players, which, &unused_dice, moving_stone);
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
        }//2endwhile

        //spielerwechsel
        if (which == 0) {
            which = 1;
        }
        else {
            which = 0;
        }
        //check
    }//1endwhile
    return 0;
}
