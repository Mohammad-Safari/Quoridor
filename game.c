#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include "getch.c"
//location of each player
const int home[2][2] = {{0, 4}, {8, 4}};
//player info->loaction (i, j), total used walls
int ply[2][3];
//used flag(1/0), orientation(1/0) and board location
int wall[2][10][4];
//board content
void initialize()
{
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            ply[i][j] = home[i][j];
}
int column_wall(int i, int j)
{
    for (int p = 0; p < 2; p++)
        for (int k = 0; k < 10; k++)
            if (wall[p][k][0] == 1 && wall[p][k][1] == 0)
            {
                if ((wall[p][k][2] == i && wall[p][k][3] == j) ||
                    (wall[p][k][2] == i - 1 && wall[p][k][3] == j))
                    // printf("\033[41m");
                    return 1;
            }
    // printf("  \033[0m");
    return 0;
}
int row_wall(int i, int j)
{
    for (int p = 0; p < 2; p++)
        for (int k = 0; k < 10; k++)
            for (int r = -1; r < 1; r++)
                if (wall[p][k][0] == 1)
                {
                    if (wall[p][k][1] == 1 && wall[p][k][2] == i && wall[p][k][3] == j + r)
                    {
                        // printf("\033[41m          ");
                        return 1;
                    }
                    // else if (wall[p][k][1] == 0 && wall[p][k][2] == i + r && wall[p][k][3] == j)
                    // {
                    //     // printf("\033[46m        \033[41m  \033[46m");
                    //     return -1;
                    // }
                }

    // printf("\033[46m          \033[0m");
    return 0;
}
void board_design()
{
    //Clear Terminal
    system("clear");
    //Control Size
    printf("\033[8;35;93t");
    //Title!
    printf("%42c\033[34m\033[41mQuoridor!\033[0m\n\n\n", '\0');

    //Column Numbers Row!
    printf("\033[46m  \033[0m");
    for (int j = 0; j < 9; j++)
    {
        printf("\033[46m   %d    \033[0m", j + 1);
        if (j != 8)
            printf("\033[41m%d \033[0m", j + 1);
    }
    printf("\033[46m  \033[0m");

    printf("\n");
    //Row Alphabets and Row Content(Players Space and Wall Space)
    for (int i = 0; i < 9; i++)
    {
        //Above Padding Space
        printf("\033[46m  \033[0m");
        for (int j = 0; j < 9; j++)
        {
            printf("        ");
            //Inter-Row Wall space!!!
            printf("\033[46m");
            if (column_wall(i, j))
                printf("\033[41m");
            printf("  \033[0m");
        }
        printf("\n");
        //Content
        printf("\033[46m%c \033[0m", i + 'A');
        for (int j = 0; j < 9; j++)
        {
            printf("  ");
            if (i == ply[0][0] && j == ply[0][1])
                printf("\033[43m");
            else if (i == ply[1][0] && j == ply[1][1])
                printf("\033[45m");
            printf("    \033[0m  ");
            //Inter-Row Wall space!!!
            printf("\033[46m");
            if (column_wall(i, j))
                printf("\033[41m");
            printf("  \033[0m");
        }
        printf("\n");
        /////
        printf("\033[46m  \033[0m");
        for (int j = 0; j < 9; j++)
        {
            printf("  ");
            if (i == ply[0][0] && j == ply[0][1])
                printf("\033[43m");
            else if (i == ply[1][0] && j == ply[1][1])
                printf("\033[45m");
            printf("    \033[0m  ");
            //Inter-Row Wall space!!!
            printf("\033[46m");
            if (column_wall(i, j))
                printf("\033[41m");
            printf("  \033[0m");
        }
        printf("\n");

        //Below Padding Space
        printf("\033[46m  \033[0m");
        for (int j = 0; j < 9; j++)
        {
            printf("        ");
            //Inter-Row Wall space!!!
            printf("\033[46m");
            if (column_wall(i, j))
                printf("\033[41m");
            printf("  \033[0m");
        }
        printf("\n");
        //Wall Row Space
        if (i != 8)
            printf("\033[41m%c \033[0m", i + 'a');
        for (int j = 0; j < 9; j++)
        {
            int sp = row_wall(i, j);
            switch (sp)
            {
            case 1:
                printf("\033[41m          ");
                break;
            case -1:
                printf("\033[46m        \033[41m  \033[46m");
                break;
            case 0:
                printf("\033[46m          \033[0m");
                break;
            }
        }
        if (i != 8)
            printf("\n");
    }
    printf("\033[46m  \033[0m\n\n");
}
int move(int who, int m)
{
    int i = ply[who][0];
    int j = ply[who][1];
    switch (m)
    {
    case 0:
    {
        // for (int r = -1; r < 1; r++)
        if (ply[who][0] == 0 || row_wall(i - 1, j /* + r */) == 1 || (i - 1 == ply[(who + 1) % 2][0] && j == ply[(who + 1) % 2][1]))
            return 0;
        ply[who][0] -= 1;
        return 1;
    }
    case 1:
    {
        // for (int r = -1; r < 1; r++)
        if (ply[who][0] == 8 || row_wall(i, j /* + r */) == 1 || (i + 1 == ply[(who + 1) % 2][0] && j == ply[(who + 1) % 2][1]))
            return 0;
        ply[who][0] += 1;
        return 1;
    }
    case 2:
    {
        // for (int r = -1; r < 1; r++)
        if (ply[who][1] == 0 || column_wall(i /* + r */, j - 1) == 1 || (j - 1 == ply[(who + 1) % 2][1] && i == ply[(who + 1) % 2][0]))
            return 0;
        ply[who][1] -= 1;
        return 1;
    }
    case 3:
    {
        // for (int r = -1; r < 1; r++)
        if (ply[who][1] == 8 || column_wall(i /* + r */, j) == 1 || j + 1 == (ply[(who + 1) % 2][1] && i == ply[(who + 1) % 2][0]))
            return 0;
        ply[who][1] += 1;
        return 1;
    }
    }
}
int place_wall(int who, int i, int j, int orn)
{
    if (ply[who][2] == 10) //if player has used all of his walls
        return -1;
    for (int p = 0; p < 2; p++)             //checking placed walls through each player
        for (int q = 0; q < ply[p][2]; q++) //checking placed walls one by one
            for (int r = -1; r <= 1; r++)   //investigating neigbor wall houses
                if ((orn == 0 && wall[p][q][2] == i + r && wall[p][q][3] == j) ||
                    (orn == 1 && wall[p][q][2] == i && wall[p][q][3] == j + r))
                    if (r == 0 || orn == wall[p][r][1]) // because we cannot merge or re-place wall!
                        return 0;
    wall[who][ply[who][2]][0] = 1;
    wall[who][ply[who][2]][1] = orn;
    wall[who][ply[who][2]][2] = i;
    wall[who][ply[who][2]++][3] = j;
    return 1;
}
int chk_board(int who)
{
    // if (ply[who][0] == ply[(who + 1) % 2][0] && ply[who][1] == ply[(who + 1) % 2][1])
    if (ply[who][0] == home[(who + 1) % 2][0])
        return 1;
    return 0;
}
int main()
{
    printf("%39c\033[34m\033[41mQuoridor!\033[0m\n\n\n", '\0');
    printf("%38c\033[33mBY\033[31m M\033[33m_\033[31mSAFARI\033[0m\n", '\0');
    sleep(2);
    system("clear");
    printf("%36c\033[33mPlayer1 TOP Yellow\033[0m\n\n", '\0');
    printf("%35c\033[35mPlayer2 Bottom Purple\033[0m\n", '\0');
    sleep(2);
    initialize();
    board_design();
    int turn = 0;
    while (80)
    {
        if (turn % 2)
            printf("\033[35m");
        else
            printf("\033[33m");
        printf("%37cPlayer %d 's %dth round\033[0m\n%42c1 - \033[32mMove\033[0m\n%40c2 - \033[31mPut Wall\n\033[0m", '\0', turn % 2 + 1, (int)((turn + 2) / 2), '\0', '\0');
        char in = getch();
        int e = 0;
        switch (in)
        {
        case '1':
        {
            while (80)
            {
                printf("%39c\033[36ma-UP    \033[34mb-DOWN\n%38c\033[33mc-LEFT    \033[35md-RIGHT\033[0m\n", '\0', '\0');
                int m = getch() - 'a';
                if (m > 3 || m < 0)
                    continue;
                int mc = move(turn % 2, m);
                if (!mc)
                {
                    e = -1;
                }
                break;
            }
            break;
        }
        case '2':
        {

            int wj;
            char wi, wo;

            printf("\033[31mEnter desired row for placing wall:\033[0m\n");
            scanf("%c", &wi);
            getch();
            if (wi > 'h' || wi < 'a')
            {
                e = -1;
                break;
            }

            printf("\033[31mEnter desired column for placing wall:\033[0m\n");
            scanf("%d", &wj);
            getch();
            if (wj > 8 || wj < 1)
            {
                e = -1;
                break;
            }

            printf("\033[31mEnter desired orientation for placing wall, h for horizontal & v for vertical:\033[0m\n");
            scanf("%c", &wo);
            getch();
            if (wo != 'v' && wo != 'h')
            {
                e = -1;
                break;
            }
            int o = (wo == 'h') ? 1 : 0;
            int out = place_wall(turn % 2, wi - 'a', wj - 1, o);
            if (out == 1)
                break;
            else if (out == 0)
            {
                e = -1;
                break;
            }
            else
            {
                e = -1;
                break;
            }
        }
        default:
            e = -1;
            break;
        }
        if (e == -1)
            continue;
        turn++;
        board_design();
        if (chk_board((turn - 1) % 2))
            break;
    }
    printf("Player %d won!! in %dth move!!\n", turn % 2 + 1, (int)(turn / 2));
    sleep(2);
}