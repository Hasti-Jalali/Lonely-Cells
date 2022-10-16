#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

struct Cell {
    int x,y;
    char name[6]; //"empty" = there is no cell  here
    char feature; // Energy = '1', Mitosis = '2', Forbidden = '3', Normal = '4'
    int score;
    int energy_block;
    int which_player; //0 = empty,(in single_player game : 1 = there is a cell here),(in multi player game : 1 = Player number one, 2 = Player number two).
    struct Cell * next;
};

void print_list(struct Cell * list , int n){
    struct Cell * current = list;
    for(int i = 0 ; i < n ; i++){
        for(int j = 0; j < n; j++){
            printf("x = %d, y = %d, name = %s,feature = %c, score = %d, energy_block = %d, which_player = %d\n",current->x,current->y,current->name,current->feature,current->score,current->energy_block, current->which_player);
            current = current->next;
        }
    }
}

struct Cell * creat_cell(int x, int y, char name[6], char feature, int score,int energy_block, int which_player);
void add_end(struct Cell * list, struct Cell * new_cell);
void save(struct Cell * list, int n, char type_of_game[256]);
int load(struct Cell ** plist, char type_of_game[256]);
int check_of_empty_cell(struct Cell * list, int x, int y);
void print(int n, struct Cell * list);
void naming_cell(int x, int y, char name[6], struct Cell * list, int which_player, int score);
void print_cell_for_choose(struct Cell * list, int which_player);
void move(struct Cell * list, int which_player, int choose_cell);
void splite_a_cell(struct Cell * list, int which_player, int choose_cell);
int check(struct Cell * list, int x,int y);
void boost_energy(struct Cell * list,int which_player, int choose_cell);
void Exit(struct Cell * list);
int calculate_score(struct Cell * list, int which_player);

int main(){

    int choice;
    FILE * fp;
    fp = fopen("c://map.bin", "rb");
    if(fp == NULL){
        printf("Cannot open file\n");
        return -1;
    }
    int n;
    fread(&n, sizeof(int), 1, fp);
    char arr[n][n];
    for(int j = 0; j < n; j++){
        for(int i = 0 ; i < n; i++){
            fread(&(arr[i][j]),sizeof(char),1,fp);
        }
    }
    fclose(fp);
    struct Cell * list, * new_cell;
    int check = 0;
    for(int j = 0 ; j < n; j++){
        for(int i = 0 ; i < n ; i++){
            if(check == 0){
                if(arr[0][0] == '1')
                    list = creat_cell(0,0,"empty",arr[0][0],0,100,0);
                else
                    list = creat_cell(0,0,"empty",arr[0][0],0,0,0);
                check = 1;
            }
            else{
                if(arr[i][j] == '1')
                    new_cell = creat_cell(i,j,"empty",arr[i][j],0,100,0);
                else
                    new_cell = creat_cell(i,j,"empty",arr[i][j],0,0,0);
                add_end(list, new_cell);
            }
        }
    }

    while(1){
        printf("[ 1 ] Load \n[ 2 ] New Single Player Game\n[ 3 ] New Multi Player Game\n[ 4 ] Exit\n");
        scanf("%d", &choice);
        system("cls");
        if(choice == 1){
            printf("[ 1 ] load single player game \n[ 2 ] load multi player game\n");
            int choice_load;
            scanf("%d", &choice_load);
            if(choice_load == 1)
                n = load(&list,"single_player");
            else if(choice_load == 2)
                n = load(&list, "multiplayer");

            print(n,list);
            if(choice_load == 1){
                while(1){
                    print_cell_for_choose(list,1);
                        int choose_cell;
                        scanf("%d", &choose_cell);
                        system("cls");
                        printf("[ 1 ] Move \n[ 2 ] Split a cell\n[ 3 ] Boost energy \n[ 4 ] Save \n[ 5 ] Exit\n");
                        int choice_2;
                        scanf("%d",&choice_2);
                        system("cls");
                        if(choice_2 == 1){
                            move(list,1,choose_cell);
                            print(n,list);
                        }
                        if(choice_2 == 2){
                            splite_a_cell(list, 1,choose_cell);
                            print(n,list);
                        }
                        if(choice_2 == 3){
                            boost_energy(list,1,choose_cell);
                            print(n,list);
                        }
                        if(choice_2 == 4){
                            save(list,n,"single_player");
                            print(n,list);
                        }
                        if(choice_2 == 5){
                            system("cls");
                            int result = calculate_score(list,1);
                            printf("Your scrore is %d.\n",result);
                            Exit(list);
                            break;
                        }
                }
            }
            else if(choice_load == 2){
                while(1){
                    system("cls");
                    print(n,list);
                    printf("First Player : \n");
                    print_cell_for_choose(list,1);
                    int choose_cell;
                    scanf("%d", &choose_cell);
                    printf("[ 1 ] Move \n[ 2 ] Split a cell\n[ 3 ] Boost energy \n[ 4 ] Save \n[ 5 ] Exit\n");
                    int choice_2;
                    scanf("%d",&choice_2);
                    system("cls");
                    if(choice_2 == 1){
                        move(list,1,choose_cell);
                        print(n,list);
                    }
                    if(choice_2 == 2){
                        splite_a_cell(list, 1,choose_cell);
                        print(n,list);
                    }
                    if(choice_2 == 3){
                        boost_energy(list,1,choose_cell);
                        print(n,list);
                    }
                    if(choice_2 == 4){
                        save(list,n,"multiplayer");
                    }
                    if(choice_2 == 5){
                        system("cls");
                        int result_1 = calculate_score(list,1);
                        int result_2 = calculate_score(list,2);
                        printf("First Player's Score is %d.\n",result_1);
                        printf("Second Player's Score is %d.\n",result_2);
                        Exit(list);
                        break;
                    }
                    system("cls");
                    print(n,list);
                    printf("Second Player : \n");
                    print_cell_for_choose(list,2);
                    scanf("%d", &choose_cell);
                    printf("[ 1 ] Move \n[ 2 ] Split a cell\n[ 3 ] Boost energy \n[ 4 ] Save \n[ 5 ] Exit\n");
                    scanf("%d",&choice_2);
                    if(choice_2 == 1){
                        move(list,2,choose_cell);
                        print(n,list);
                    }
                    if(choice_2 == 2){
                        splite_a_cell(list, 2,choose_cell);
                        print(n,list);
                    }
                    if(choice_2 == 3){
                        boost_energy(list,2,choose_cell);
                        print(n,list);
                    }
                    if(choice_2 == 4){
                        save(list,n,"multiplayer");
                    }
                    if(choice_2 == 5){
                        int result_1 = calculate_score(list,1);
                        int result_2 = calculate_score(list,2);
                        printf("First Player's Score is %d.\n",result_1);
                        printf("Second Player's Score is %d.\n",result_2);
                        Exit(list);
                        break;
                    }
                }
            }

        }
        else if(choice == 2){
            print(n,list);
            printf("Please Enter Number of Cell\n");
            int number_of_cell;
            scanf("%d", &number_of_cell);
            system("cls");
            for(int i = 0; i < number_of_cell ; i++){
                printf("Where do you want to put your Cell number %d?\n", i+1);
                int result = 0;
                while(result != 1){
                    printf("x = ");
                    int x,y;
                    scanf("%d", &x);
                    printf("y = ");
                    scanf("%d", &y);
                    result = check_of_empty_cell(list,x,y);
                    if(result == 1){
                        printf("Please Enter the Name of Cell Number %d : ", i+1);
                        char name[6];
                        scanf("%s",name);
                        naming_cell(x,y,name,list,1,0);
                    }
                }
            }

            print(n,list);
            while(1){
                print_cell_for_choose(list,1);
                int choose_cell;
                scanf("%d", &choose_cell);
                printf("[ 1 ] Move \n[ 2 ] Split a cell\n[ 3 ] Boost energy \n[ 4 ] Save \n[ 5 ] Exit\n");
                int choice_2;
                scanf("%d",&choice_2);
                system("cls");
                if(choice_2 == 1){
                    move(list,1,choose_cell);
                    print(n,list);
                }
                if(choice_2 == 2){
                    splite_a_cell(list, 1,choose_cell);
                    print(n,list);
                }
                if(choice_2 == 3){
                    boost_energy(list,1,choose_cell);
                    print(n,list);
                }
                if(choice_2 == 4){
                    save(list,n,"single_player");
                }
                if(choice_2 == 5){
                    int result = calculate_score(list,1);
                    printf("Your score is %d.\n",result);
                    Exit(list);
                    break;
                }

            }
        }
        else if(choice == 3){
            int number_cell_1, number_cell_2;
            printf("Number of First Player's Cell : ");
            scanf("%d", &number_cell_1);
            for(int i = 0; i < number_cell_1 ; i++){
                printf("Where do you want to put your Cell number %d?\n", i+1);
                int result = 0;
                while(result != 1){
                    printf("x = ");
                    int x,y;
                    scanf("%d", &x);
                    printf("y = ");
                    scanf("%d", &y);
                    result = check_of_empty_cell(list,x,y);
                    if(result == 1){
                        printf("Please Enter the Name of Cell Number %d : ", i+1);
                        char name[6];
                        scanf("%s",name);
                        naming_cell(x,y,name,list,1,0);
                    }
                }
            }
            print(n,list);
            printf("Number of Second Player's Cell : ");
            scanf("%d", &number_cell_2);
            for(int i = 0; i < number_cell_2 ; i++){
                printf("Where do you want to put your Cell number %d?\n", i+1);
                int result = 0;
                while(result != 1){
                    printf("x = ");
                    int x,y;
                    scanf("%d", &x);
                    printf("y = ");
                    scanf("%d", &y);
                    result = check_of_empty_cell(list,x,y);
                    if(result == 1){
                        printf("Please Enter the Name of Cell Number %d : ", i+1);
                        char name[6];
                        scanf("%s",name);
                        naming_cell(x,y,name,list,2,0);
                    }
                }
            }
            //system("cls");
            print(n,list);
            while(1){
                printf("First Player : \n");
                print_cell_for_choose(list,1);
                int choose_cell;
                scanf("%d", &choose_cell);
                printf("[ 1 ] Move \n[ 2 ] Split a cell\n[ 3 ] Boost energy \n[ 4 ] Save \n[ 5 ] Exit\n");
                int choice_2;
                scanf("%d",&choice_2);
                //system("cls");
                if(choice_2 == 1){
                    move(list,1,choose_cell);
                    print(n,list);
                }
                if(choice_2 == 2){
                    splite_a_cell(list, 1,choose_cell);
                    print(n,list);
                }
                if(choice_2 == 3){
                    boost_energy(list,1,choose_cell);
                    print(n,list);
                }
                if(choice_2 == 4){
                    save(list,n,"multiplayer");
                }
                if(choice_2 == 5){
                    int result_1 = calculate_score(list,1);
                    int result_2 = calculate_score(list,2);
                    printf("First Player's Score is %d.\n",result_1);
                    printf("Second Player's Score is %d.\n",result_2);
                    Exit(list);
                    break;
                }
                printf("Second Player : \n");
                print_cell_for_choose(list,2);
                scanf("%d", &choose_cell);
                printf("[ 1 ] Move \n[ 2 ] Split a cell\n[ 3 ] Boost energy \n[ 4 ] Save \n[ 5 ] Exit\n");
                scanf("%d",&choice_2);
                if(choice_2 == 1){
                    move(list,2,choose_cell);
                    print(n,list);
                }
                if(choice_2 == 2){
                    splite_a_cell(list, 2,choose_cell);
                    print(n,list);
                }
                if(choice_2 == 3){
                    boost_energy(list,2,choose_cell);
                    print(n,list);
                }
                if(choice_2 == 4){
                    save(list,n,"multiplayer");
                }
                if(choice_2 == 5){
                    int result_1 = calculate_score(list,1);
                    int result_2 = calculate_score(list,2);
                    printf("First Player's Score is %d.\n",result_1);
                    printf("Second Player's Score is %d.\n",result_2);
                            Exit(list);
                    break;
                }
            }
        }
        else if(choice == 4){
            printf("Goodbye :)\n");
            return 1;
        }
        else{
            printf("There is No Such Option, Please choose another one\n");
        }

    }

    return 0;
}
struct Cell * creat_cell(int x, int y, char name[6], char feature, int score,int energy_block,int which_player){
    struct Cell * new_cell;
    new_cell = (struct Cell *) malloc(sizeof(struct Cell));
    if(new_cell == NULL){
        return NULL;
    }
    new_cell->x = x;
    new_cell->y = y;
    strcpy(new_cell->name, name);
    new_cell->feature = feature;
    new_cell->score = score;
    new_cell->energy_block = energy_block;
    new_cell->which_player = which_player;
    new_cell->next = NULL;
    return new_cell;
}
void add_end(struct Cell * list, struct Cell * new_cell){
    struct Cell * current = list;
    for(current = list; current->next != NULL; current = current->next);

    current->next = new_cell;
    new_cell->next = NULL;

}
void save(struct Cell * list, int n, char type_of_game[256]){

    FILE * fpout;
    if(strcmp(type_of_game, "single_player") == 0){
        fpout = fopen("previous_single_player.bin","wb");
    }
    else if(strcmp(type_of_game, "multiplayer") == 0){
        fpout = fopen("previous_multiplayer.bin", "wb");
    }
    if(fpout == NULL){
        printf("Cannot save the game\n");
        return NULL;
    }
    fwrite(&n,sizeof(int), 1, fpout);
    struct Cell * current = list;
    int x,y;
    char name[6];
    char feature;
    int score;
    int energy_block;
    int which_player;
    while(current != NULL){
        x = current->x;
        y = current->y;
        strcpy(name,current->name);
        feature = current->feature;
        score = current->score;
        energy_block = current->energy_block;
        which_player = current->which_player;
        fwrite(&x, sizeof(int), 1, fpout);
        fwrite(&y, sizeof(int), 1, fpout);
        fwrite(name, sizeof(char), 6, fpout);
        fwrite(&feature, sizeof(char), 1, fpout);
        fwrite(&score, sizeof(int), 1, fpout);
        fwrite(&energy_block, sizeof(int),1,fpout);
        fwrite(&which_player, sizeof(int), 1, fpout);
        current = current->next;
    }

    fclose(fpout);
}
int load(struct Cell ** plist, char type_of_game[256]){
    FILE * fpin;
    if(strcmp(type_of_game, "single_player") == 0){
        fpin = fopen("previous_single_player.bin","rb");
    }
    else if(strcmp(type_of_game, "multiplayer") == 0){
        fpin = fopen("previous_multiplayer.bin", "rb");
    }
    if(fpin == NULL){
        printf("Cannot load the game\n");
        return NULL;
    }
    int n;
    int check = 0;
    int x,y;
    char name[6];
    char feature;
    int score;
    int energy_block;
    int which_player;
    fread(&n, sizeof(int), 1, fpin);
    int num = n;
    for(int i = 0; i < num * num ; i++){
        fread(&x, sizeof(int), 1, fpin);
        fread(&y, sizeof(int), 1, fpin);
        fread(name, sizeof(char), 6, fpin);
        fread(&feature, sizeof(char), 1, fpin);
        fread(&score, sizeof(int), 1, fpin);
        fread(&energy_block, sizeof(int),1,fpin);
        fread(&which_player, sizeof(int), 1, fpin);
        if(check == 0){
            * plist = creat_cell(x, y, name, feature, score, energy_block,which_player);
            check = 1;
        }
        else{
            struct Cell * new_cell = creat_cell(x,y,name,feature, score,energy_block, which_player);
            add_end(*plist, new_cell);

        }
    }
    fclose(fpin);
    return n;
}
int check_of_empty_cell(struct Cell * list, int x, int y){
    struct Cell * current = list;
    while(current != NULL){
        if(current->x == x && current->y == y){
            break;
        }
        else
            current = current->next;
    }
    if(current == NULL){
        printf("---\nThere is no such option, please choose another one.\n---\n");
        return 0;
    }
    if(current->feature == '3'){//it means here is forbidden.
        printf("---\nIt's Forbidden, choose another one\n---\n");
        return 0;
    }
    if(strcmp(current->name, "empty") == 0){
        return 1;//it means here is fine.
    }
    printf("---\nIt's Full, choose another one\n---\n");
    return 0;//it means here is full.
}
void print(int n, struct Cell * list){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    WORD saved_attributes;
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    saved_attributes = consoleInfo.wAttributes;
    struct Cell * current_1 = list;
    struct Cell * current_2 = list->next;
    if(n % 2 == 0){
        for(int j = 0; j < 2 * n + 2 ; j++){
            if(j % 2 == 0){
                for(int i = 0; i < 4 * n + 1; i++){
                    if((i % 2 == 0) && (i % 8 != 6)){
                        if(j == 0 && i == 4 * n){
                            printf(" ");
                        }
                        else{
                            SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
                            printf("*");
                            SetConsoleTextAttribute(hConsole, saved_attributes);
                        }
                    }
                    else if(i % 2 == 1 || j == 0)
                        printf(" ");
                    else {
                        //printf("-");
                        if(current_2->feature == '1'){//it means energy's background is red
                            SetConsoleTextAttribute(hConsole, BACKGROUND_RED);
                        }
                        else if(current_2->feature == '2'){//it means mitosis's background is green
                            SetConsoleTextAttribute(hConsole, BACKGROUND_GREEN);

                        }
                        else if(current_2->feature == '3'){
                            SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
                            printf("!");
                        }
                       if(current_2->feature != '3'){
                           if(current_2->which_player == 1){
                                    printf("X");
                            }
                            else if(current_2->which_player == 2){
                                printf("Y");
                            }
                            else{
                                printf(" ");
                            }
                            SetConsoleTextAttribute(hConsole, saved_attributes);
                            if(current_2->next != NULL && current_2->next->next != NULL)
                                current_2 = current_2->next->next;
                        }
                    }
                }
            }
            else{
                for(int i = 0; i < 4 * n + 1; i++){
                    if((i % 2 == 0) && (i % 8 != 2)){
                        if(j == 2 * n + 1 && i == 0){
                            printf(" ");
                        }
                        else{
                            SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
                            printf("*");
                            SetConsoleTextAttribute(hConsole, saved_attributes);
                        }
                    }
                    else if(i % 2 == 1 || j == 2 * n + 1)
                        printf(" ");
                    else{
                        //printf("+");
                        if(current_1->feature == '1'){//it means energy's background is red
                            SetConsoleTextAttribute(hConsole, BACKGROUND_RED);
                        }
                        else if(current_1->feature == '2'){//it means mitosis's background is green
                            SetConsoleTextAttribute(hConsole, BACKGROUND_GREEN);

                        }
                        else if(current_1->feature == '3'){
                            SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
                            printf("!");
                        }
                        if(current_1->feature != '3'){
                           if(current_1->which_player == 1){
                                    printf("X");
                            }
                            else if(current_1->which_player == 2){
                                printf("Y");
                            }
                            else{
                                printf(" ");
                            }
                            SetConsoleTextAttribute(hConsole, saved_attributes);
                            if(current_1->next != NULL && current_1->next->next != NULL)
                                current_1 = current_1->next->next;
                        }
                    }
                }
            }
            printf("\n");
        }
    }
    else{
        for(int j = 0; j < 2 * n + 2 ; j++){
            if(j % 2 == 0){
                for(int i = 0; i < 4 * n + 1; i++){
                    if((i % 2 == 0) && (i % 8 != 6)){
                        SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
                        printf("*");
                        SetConsoleTextAttribute(hConsole, saved_attributes);
                    }
                    else if(i % 2 == 1 || j == 0)
                        printf(" ");
                    else{
                        if(current_2->feature == '1'){//it means energy's background is red
                            SetConsoleTextAttribute(hConsole, BACKGROUND_RED);
                        }
                        else if(current_2->feature == '2'){//it means mitosis's background is green
                            SetConsoleTextAttribute(hConsole, BACKGROUND_GREEN);

                        }
                        else if(current_2->feature == '3'){
                            SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
                            printf("!");
                        }
                        if(current_2->feature != '3'){
                           if(current_2->which_player == 1){
                                    printf("X");
                            }
                            else if(current_2->which_player == 2){
                                printf("Y");
                            }
                            else{
                                printf(" ");
                            }
                            SetConsoleTextAttribute(hConsole, saved_attributes);
                            if(current_2->next != NULL && current_2->next->next != NULL)
                                current_2 = current_2->next->next;
                        }
                    }

                }
            }
            else{
                for(int i = 0; i < 4 * n + 1; i++){
                    if((i % 2 == 0) && (i % 8 != 2)){
                        if(j == 2 * n + 1 &&(i == 4 * n || i == 0)){
                            printf(" ");
                        }
                        else{
                            SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
                            printf("*");
                            SetConsoleTextAttribute(hConsole, saved_attributes);
                        }
                    }
                    else if(i % 2 == 1 || j == 2 * n + 1)
                        printf(" ");
                    else{
                        if(current_1->feature == '1'){//it means energy's background is red
                            SetConsoleTextAttribute(hConsole, BACKGROUND_RED);
                        }
                        else if(current_1->feature == '2'){//it means mitosis's background is green
                            SetConsoleTextAttribute(hConsole, BACKGROUND_GREEN);

                        }
                        else if(current_1->feature == '3'){
                            SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
                            printf("!");
                        }
                        if(current_1->feature != '3'){
                           if(current_1->which_player == 1){
                                    printf("X");
                            }
                            else if(current_1->which_player == 2){
                                printf("Y");
                            }
                            else{
                                printf(" ");
                            }
                            SetConsoleTextAttribute(hConsole, saved_attributes);
                            if(current_1->next != NULL && current_1->next->next != NULL)
                                current_1 = current_1->next->next;
                        }
                    }
                }
            }
            printf("\n");
        }
    }
}

void naming_cell(int x, int y, char name[6], struct Cell * list,int which_player, int score){
    struct Cell * current = list;

    while(current != NULL){
        if(current->x == x && current->y == y){
            strcpy(current->name, name);
            current->which_player = which_player;
            current->score = score;
            return;
        }
        current = current->next;
    }
}
void print_cell_for_choose(struct Cell * list, int which_player){
    struct Cell * current = list;
    int i = 1;
    printf("Choose one of Your Cells\n");
    while(current != NULL){
        if(current->which_player == which_player){
            printf("[ %d ] %s ( %d , %d )\n",i,current->name,current->x,current->y);
            i++;
        }
        current = current->next;
    }
}
void move(struct Cell * list, int which_player, int choose_cell){
    int i = 1;
    struct Cell * current = list;
    int check = 0;
    while(current != NULL){
        if(current->which_player == which_player){
            if(i == choose_cell){
                check = 1;
                break;
            }
            else
                i++;
        }
        if(check == 1)
            break;
        else
            current = current->next;
    }
    while(1){
        int next_move;
        printf("[ 1 ] North \n[ 2 ] South \n[ 3 ] North-east \n[ 4 ] North-west \n[ 5 ] South-east \n[ 6 ] Southwest\n");
        scanf("%d", &next_move);
        int new_x, new_y;
        new_x = (next_move == 1 || next_move == 2) ? (current->x) : (next_move == 3 || next_move == 5) ? (current->x ) + 1 : /*(next_move == 4 || next_move == 6) ? */(current->x) - 1;
        if(current->x % 2 == 1){
            new_y = (next_move == 1) ? (current->y) - 1 : (next_move == 2 || next_move == 5 || next_move == 6) ? (current->y) + 1 : /*(next_move == 3 || next_move == 4) ? */current->y ;
        }
        else{
            new_y = (next_move == 1 || next_move == 3 || next_move == 4) ? (current->y) - 1 : (next_move == 5 || next_move == 6) ? (current->y) : /*(next_move == 2) ? */(current->y) + 1;
        }
        if(check_of_empty_cell(list,new_x,new_y)){
            naming_cell(new_x,new_y,current->name,list,current->which_player,current->score);
            naming_cell(current->x,current->y,"empty",list,0,0);
            break;
        }
    }
}
void splite_a_cell(struct Cell * list, int which_player, int choose_cell){
    int i = 1;
    struct Cell * current = list;
    int check_ = 0;
    while(current != NULL){
        if(current->which_player == which_player){
            if(i == choose_cell){
                check_ = 1;
                break;
            }
            else
                i++;
        }
        if(check_ == 1)
            break;
        else
            current = current->next;
    }
        if(current->feature == '2' && current->score >= 80){
            int result = 0;
            if(current->x % 2 == 0){
                if(check(list,current->x,current->y - 1) == 1)
                    result++;
                if(check(list,current->x, current->y + 1) == 1)
                    result++;
                if(check(list,current->x + 1, current->y - 1) ==1)
                    result++;
                if(check(list, current->x - 1, current->y - 1) == 1)
                    result++;
                if(check(list,current->x -1, current->y) == 1)
                    result++;
                if(check(list, current->x + 1, current->y) == 1)
                    result++;
            }
            else {
                if(check(list,current->x,current->y - 1) == 1)
                    result++;
                if(check(list,current->x, current->y + 1) == 1)
                    result++;
                if(check(list,current->x + 1, current->y + 1) ==1)
                    result++;
                if(check(list, current->x - 1, current->y + 1) == 1)
                    result++;
                if(check(list,current->x -1, current->y) == 1)
                    result++;
                if(check(list, current->x + 1, current->y) == 1)
                    result++;
            }
            if(result >= 2){
                printf("Where do you want to put your first new cell ?\n");
                while(1){
                    int next_move;
                    printf("[ 1 ] North \n[ 2 ] South \n[ 3 ] North-east \n[ 4 ] North-west \n[ 5 ] South-east \n[ 6 ] Southwest\n");
                    scanf("%d", &next_move);
                    int new_x, new_y;
                    new_x = (next_move == 1 || next_move == 2) ? (current->x) : (next_move == 3 || next_move == 5) ? (current->x ) + 1 : /*(next_move == 4 || next_move == 6) ? */(current->x) - 1;
                    if(current->x % 2 == 1){
                        new_y = (next_move == 1) ? (current->y) - 1 : (next_move == 2 || next_move == 5 || next_move == 6) ? (current->y) + 1 : /*(next_move == 3 || next_move == 4) ? */current->y ;
                    }
                    else{
                        new_y = (next_move == 1 || next_move == 3 || next_move == 4) ? (current->y) - 1 : (next_move == 5 || next_move == 6) ? (current->y) : /*(next_move == 2) ? */(current->y) + 1;
                    }
                    if(check_of_empty_cell(list,new_x,new_y)){
                        printf("Please Enter your first new cell's name ?\n");
                        char name[6];
                        scanf("%s",name);
                        naming_cell(new_x,new_y,name,list,current->which_player,40);
                        //naming_cell(current->x,current->y,"empty",list,0,0);
                        break;
                    }
                }
                printf("Where do you want to put your second new cell ?\n");
                while(1){
                    int next_move;
                    printf("[ 1 ] North \n[ 2 ] South \n[ 3 ] North-east \n[ 4 ] North-west \n[ 5 ] South-east \n[ 6 ] Southwest\n");
                    scanf("%d", &next_move);
                    int new_x, new_y;
                    new_x = (next_move == 1 || next_move == 2) ? (current->x) : (next_move == 3 || next_move == 5) ? (current->x ) + 1 : /*(next_move == 4 || next_move == 6) ? */(current->x) - 1;
                    if(current->x % 2 == 1){
                        new_y = (next_move == 1) ? (current->y) - 1 : (next_move == 2 || next_move == 5 || next_move == 6) ? (current->y) + 1 : /*(next_move == 3 || next_move == 4) ? */current->y ;
                    }
                    else{
                        new_y = (next_move == 1 || next_move == 3 || next_move == 4) ? (current->y) - 1 : (next_move == 5 || next_move == 6) ? (current->y) : /*(next_move == 2) ? */(current->y) + 1;
                    }
                    if(check_of_empty_cell(list,new_x,new_y)){
                        printf("Please Enter your first new cell's name ?\n");
                        char name[6];
                        scanf("%s",name);
                        naming_cell(new_x,new_y,name,list,current->which_player,40);
                        naming_cell(current->x,current->y,"empty",list,0,0);
                        break;
                    }
                }

            }
            else{
                printf("There is no enough block to splite a cell.\n");
            }
        }
        else if(current->feature == '2' && current->score < 80){
            printf("You don't have enough energy for splite a cell.\n");
        }
        else if(current->feature != '2')
            printf("Here is not mitosis's Block.\n");

}

int check(struct Cell * list, int x,int y){
    struct Cell * current = list;
    while(current != NULL){
        if(current->x == x && current->y == y && (strcmp(current->name,"empty") == 0) && current->feature != '3'){
            return 1;
        }
        current = current->next;
    }
    return 0;
}
void boost_energy(struct Cell * list,int which_player, int choose_cell){
    int i = 1;
    struct Cell * current = list;
    int check = 0;
    while(current != NULL){
        if(current->which_player == which_player){
            if(i == choose_cell){
                check = 1;
                break;
            }
            else
                i++;
        }
        if(check == 1)
            break;
        else
            current = current->next;
    }
    printf("You had %d energy in this cell, ",current->score);
    if(current->feature == '1'){
        if(current->energy_block >= 15){
            current->score += 15;
            current->energy_block -= 15;
        }
        else{
            current->score += current->energy_block;
            current->energy_block = 0;
        }
    }
    printf("Now you have %d energy.\n",current->score);
}
int calculate_score(struct Cell * list, int which_player){
    struct Cell * current = list;
    int result = 0;
    while(current != NULL){
        if(current->which_player == which_player){
            result += current->score;
        }
        current = current->next;
    }
    return result;
}
void Exit(struct Cell * list){
    struct Cell * current = list;
    while(current != NULL){
        strcpy(current->name,"empty");
        if(current->feature == '1'){
            current->energy_block = 100;
        }
        current->score = 0;
        current->which_player = 0;
        current = current->next;
    }
}
