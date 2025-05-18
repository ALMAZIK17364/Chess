#include <iostream>
#include <windows.h>
#include <vector>
using namespace std;

/* Памятка */
/* 
    O - Пустота - 0
    K - Король  - 1
    Q - Ферзь   - 2            Белые с w в начале обозначения wQ wP
    R - Ладья   - 3            Чёрные с b в начале обозначения bQ bP
    B - Слон    - 4            0 есть О :D
    N - Конь    - 5
    P - Пешка   - 6
*/


/* Дополнительные масивы */
char notice_colors[8] = { 'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b' };
string notice[8] = { "Enter the coordinates of the figure \nyou want to move (for example a2) \nthen a space and the coordinates \nof the place where the figure will move\n(for example a3).", "K - king\nQ - queen\nR - rook\nN - knight\nB - bishop\nP - pawn", "empty", "empty", "empty", "empty", "empty", "empty" };

/* Дополнительные переменные */
string board[8][8];
vector<char> w_killed;
vector<char> b_killed;
char winner = 'n';


/* Позиции фигур */
int k_plases[8][2] = {  // Король
       {1, 1},
       {0, 1},
       {1, 0},
       {-1, -1},
       {1, -1},
       {-1, 1},
       {-1, 0},
       {0, -1}
};

int q_plases[56][2] = { // Ферзь
    {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}, {7, 7},
    {-1, -1}, {-2, -2}, {-3, -3}, {-4, -4}, {-5, -5}, {-6, -6}, {-7, -7},
    {7, 1}, {6, 2}, {5, 3}, {4, 4}, {3, 5}, {2, 6}, {1, 7},
    {-1, 1}, {-2, 2}, {-3, 3}, {-4, 4}, {-5, 5}, {-6, 6}, {-7, 7},
    {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0},
    {-1, 0}, {-2, 0}, {-3, 0}, {-4, 0}, {-5, 0}, {-6, 0}, {-7, 0},
    {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7},
    {0, -1}, {0, -2}, {0, -3}, {0, -4}, {0, -5}, {0, -6}, {0, -7}
};

int r_plases[28][2] = { // Ладья
    {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}, {7, 7},
    {-1, -1}, {-2, -2}, {-3, -3}, {-4, -4}, {-5, -5}, {-6, -6}, {-7, -7},
    {7, 1}, {6, 2}, {5, 3}, {4, 4}, {3, 5}, {2, 6}, {1, 7},
    {-1, 1}, {-2, 2}, {-3, 3}, {-4, 4}, {-5, 5}, {-6, 6}, {-7, 7}
};

int b_plases[28][2] = { // Слон
    {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0},
    {-1, 0}, {-2, 0}, {-3, 0}, {-4, 0}, {-5, 0}, {-6, 0}, {-7, 0},
    {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7},
    {0, -1}, {0, -2}, {0, -3}, {0, -4}, {0, -5}, {0, -6}, {0, -7}
};

int n_plases[8][2] = { // Конь
    {2, 1}, {2, -1}, {1, 2}, {-1, 2}, {-2, 1}, {-2, -1}, {-1, -2}, {1, -2}
};

int p_plases[4][4] = { // Пешка
    {2, 0},{1, 0}, {1, 1}, {1, -1}
};


/* Вспомогательные функции */
vector<int> string_to_nums(const string& input) {
    vector<int> values;
    for (char c : input) {
        if (c >= 'a' && c <= 'z') {
            values.push_back(c - 'a');
        }
        else if (c >= '0' && c <= '9') {
            values.push_back(c - '0');
        }
    }
    return values;
}
void setColor(int textColor, int bgColor) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (bgColor << 4) | textColor);
}

int num_flip(int num)
{
    int result = 9 - num;
    cout << num << " " << result << endl;
    return result;
}

void add_notice(string text, char color)
{
    for (int i = 0; i < 8; i++)
    {
        if (notice[i] == "empty")
        {
            notice[i] = text;
            notice_colors[i] = color;
            break;
        }
    }
}

void write_notices(int i = 0)
{
    for (i; i < 8; i++)
    {
        string text = notice[i];
        char color = notice_colors[i];
        if (notice[i] != "empty")
        {
            switch (color)
            {
            case 'b':
                setColor(0, 15);
                cout << endl << text << endl;
                setColor(15, 0);
                break;
            case 'r':
                setColor(0, 4);
                cout << endl << text << endl;
                setColor(15, 0);
                break;
            case 'y':
                setColor(0, 14);
                cout << endl << text << endl;
                setColor(15, 0);
                break;
            case 'g':
                setColor(0, 2);
                cout << endl << text << endl;
                setColor(15, 0);
                break;
            default:
                setColor(15, 0);
                cout << endl << text << endl;
                setColor(15, 0);
                break;

            }
            if (i > 0)
            {
                notice[i] = "empty";
                notice_colors[i] = 'g';
            }

        }
    }
}


/* Основные функции */
void clear_board() // Подготовка доски к новой игре
{
    w_killed.clear();
    b_killed.clear();

    w_killed.push_back(':');
    b_killed.push_back(':');
    

    //1 строка
    board[0][0] = "Bb";
    board[0][1] = "Nb";
    board[0][2] = "Rb";
    board[0][3] = "Kb";
    board[0][4] = "Qb";
    board[0][5] = "Rb";
    board[0][6] = "Nb";
    board[0][7] = "Bb";

    //2 строка
    board[1][0] = "Pb";
    board[1][1] = "Pb";
    board[1][2] = "Pb";
    board[1][3] = "Pb";
    board[1][4] = "Pb";
    board[1][5] = "Pb";
    board[1][6] = "Pb";
    board[1][7] = "Pb";

    //3 строка
    board[2][0] = "O";
    board[2][1] = "O";
    board[2][2] = "O";
    board[2][3] = "O";
    board[2][4] = "O";
    board[2][5] = "O";
    board[2][6] = "O";
    board[2][7] = "O";

    //4 строка
    board[3][0] = "O";
    board[3][1] = "O";
    board[3][2] = "O";
    board[3][3] = "O";
    board[3][4] = "O";
    board[3][5] = "O";
    board[3][6] = "O";
    board[3][7] = "O";

    //5 строка
    board[4][0] = "O";
    board[4][1] = "O";
    board[4][2] = "O";
    board[4][3] = "O";
    board[4][4] = "O";
    board[4][5] = "O";
    board[4][6] = "O";
    board[4][7] = "O";

    //6 строка
    board[5][0] = "O";
    board[5][1] = "O";
    board[5][2] = "O";
    board[5][3] = "O";
    board[5][4] = "O";
    board[5][5] = "O";
    board[5][6] = "O";
    board[5][7] = "O";

    //7 строка
    board[6][0] = "Pw";
    board[6][1] = "Pw";
    board[6][2] = "Pw";
    board[6][3] = "Pw";
    board[6][4] = "Pw";
    board[6][5] = "Pw";
    board[6][6] = "Pw";
    board[6][7] = "Pw";

    //8 строка
    board[7][0] = "Bw";
    board[7][1] = "Nw";
    board[7][2] = "Rw";
    board[7][3] = "Kw";
    board[7][4] = "Qw";
    board[7][5] = "Rw";
    board[7][6] = "Nw";
    board[7][7] = "Bw";
}

void write_board() // Вывод доски для кажого хода
{
    system("cls");
    setColor(7, 10);
    cout << "  a b c d e f g h  " << endl;
    cout << " |----------------|" << endl;
    int mn = 8;
    for (int line = 0; line < 8; line++)
    {
        setColor(7, 10);
        cout << mn << "|";
        mn--;
        for (int col = 0; col < 8; col++)
        {
            if (board[line][col][0] == 'O')
            {
                setColor(9, 10);
                cout << board[line][col][0] << " ";

            }
            else if (board[line][col][1] == 'b')
            {
                setColor(0, 10);
                cout << board[line][col][0] << " ";

            }
            else if (board[line][col][1] == 'w')
            {
                setColor(7, 10);
                cout << board[line][col][0] << " ";
            }
        }
        setColor(7, 10);
        cout << "|" << endl;
    }
    setColor(7, 10);
    cout << " |----------------|" << endl << endl;
    setColor(7, 0);

    cout << "Captured by the White:" << " ";
    for (int i = 0; i < w_killed.size(); i++)
    {
        cout << w_killed[i] << " ";
    }
    cout << endl;

    cout << "Captured by the Black:" << " ";
    for (int i = 0; i < b_killed.size(); i++)
    {
        cout << b_killed[i] << " ";
    }
    cout << endl;
}

bool is_place_valid(int fx, int fy, int sx, int sy, string cur_fig, bool is_end_chesk)
{
    /*
    cout << cur_fig;
    int a;
    cin >> a;
    */
    if (cur_fig[0] == 'K')
    {
        if (cur_fig[1] == 'w')
        {
            for (int i = 0; i < sizeof(k_plases) / sizeof(k_plases[0]); i++)
            {
                if (fy - k_plases[i][0] == sy && fx - k_plases[i][1] == sx)
                {
                    return true;
                }
            }
        }
        else
        {
            for (int i = 0; i < sizeof(k_plases) / sizeof(k_plases[0]); i++)
            {
                if (fy + k_plases[i][0] == sy && fx + k_plases[i][1] == sx)
                {
                    return true;
                }
            }
        }
        return false;
    }
    else if (cur_fig[0] == 'Q')
    {
        if (cur_fig[1] == 'w')
        {
            for (int i = 0; i < sizeof(q_plases) / sizeof(q_plases[0]); i++)
            {
                if (fy - q_plases[i][0] == sy && fx - q_plases[i][1] == sx)
                {
                    return true;
                }
            }
        }
        else
        {
            for (int i = 0; i < sizeof(q_plases) / sizeof(q_plases[0]); i++)
            {
                if (fy + q_plases[i][0] == sy && fx + q_plases[i][1] == sx)
                {
                    return true;
                }
            }
        }
        return false;
    }
    else if (cur_fig[0] == 'R')
    {
        for (int i = 0; i < sizeof(r_plases) / sizeof(r_plases[0]); i++)
        {
            if (fy + r_plases[i][0] == sy && fx + r_plases[i][1] == sx)
            {
                return true;
            }
        }
        return false;
    }
    else if (cur_fig[0] == 'B')
    {
        for (int i = 0; i < sizeof(b_plases) / sizeof(b_plases[0]); i++)
        {
            if (fy + b_plases[i][0] == sy && fx + b_plases[i][1] == sx)
            {
                return true;
            }
        }
        return false;
    }
    else if (cur_fig[0] == 'N')
    {
        for (int i = 0; i < sizeof(n_plases) / sizeof(n_plases[0]); i++)
        {
            if (fy + n_plases[i][0] == sy && fx + n_plases[i][1] == sx)
            {
                return true;
            }
        }
        return false;
    }
    else if (cur_fig[0] == 'P')
    {
        int start_for = 1;
        int end_for = 2;
        vector<int> include;
        //int qew;
        //cout << endl << fy << " " << fx << " " << sy << " " << sx << endl;
        //cin >> qew;
        if (cur_fig[1] == 'w')
        {
            if (fy == 6)
            {
                start_for = 0; //{2, 0},{1, 0}, {1, 1}, {1, -1};
                include.push_back(0);
                //add_notice("P can move to 2", 'g');
            }
            include.push_back(1);
            if (board[fy - 1][fx - 1][0] != 'O')
            {
                end_for++;
                include.push_back(2);
                //add_notice("P can move to +1 +1", 'g');

            }
            if (board[fy - 1][fx + 1][0] != 'O')
            {
                end_for++;
                include.push_back(3);
                //add_notice("P can move to +1 -1", 'g');
            }
        }
        else
        {
            if (fy == 1)
            {
                start_for = 0; //{2, 0},{1, 0}, {1, 1}, {1, -1};
                include.push_back(0);
                //add_notice("P can move to 2", 'g');
            }
            include.push_back(1);
            if (board[fy + 1][fx + 1][0] != 'O')
            {
                
                end_for++;
                include.push_back(2);
                if (!is_end_chesk)
                {
                    string notice = "P can move on ";
                    notice += board[fy + 1][fx + 1][0];
                    notice += " - ";
                    int a = fy + 1;
                    int b = fx + 1;
                    notice += a;
                    notice += ", ";
                    notice += b;
                    add_notice(notice, 'g');
                }
            }
            if (board[fy + 1][fx - 1][0] != 'O')
            {
                end_for++;
                include.push_back(3);
                if (!is_end_chesk)
                {
                    string notice = "P can move on ";
                    notice += board[fy + 1][fx - 1][0];
                    notice += " - ";
                    int a = fy + 1;
                    int b = fx - 1;
                    notice += a;
                    notice += ", ";
                    notice += b;
                    add_notice(notice, 'g');
                }
            }
        }
        
        if (board[fy - 1][fx][0] != 'O' && cur_fig[1] == 'w')
        {   
            if (!is_end_chesk)
            {
                add_notice("This way is blocked, try to move another figure!", 'r');
            }
            return false;
        }
        if (board[fy + 1][fx][0] != 'O' && cur_fig[1] == 'b')
        {
            if (!is_end_chesk)
            {
                add_notice("This way is blocked, try to move another figure!", 'r');
            }
            return false;
        }
        
        for (int i = 0; i < include.size(); i++)
        {
            //cout<<fy <<" "<< p_plases[i][0] <<" "<<sy<<" "<<fx<<" "<<p_plases[i][1]<<" "<<sx<<endl;
            //cin>>qew;
            //string notice = "now i = ";
            //notice+=to_string(fy);
            //add_notice(notice, 'y');
            if (cur_fig[1] == 'w')
            {
                
                if (fy - p_plases[include[i]][0] == sy && fx - p_plases[include[i]][1] == sx)
                {
                    return true;
                }
            }
            else
            {
                
                if (fy + p_plases[include[i]][0] == sy && fx + p_plases[include[i]][1] == sx)
                {
                    return true;
                }
            }
            if (fy + p_plases[include[i]][0] == sy && fx + p_plases[include[i]][1] == sx)
            {
                return true;
            }
        }
        if (!is_end_chesk)
        {
            add_notice("ERROR: Wrong P logic or idk", 'r');
        }
        return false;
    }
    else
    {
        return false;
    }
}


char move_unit(int fx, int fy, int sx, int sy, char turn) // Двигать фигуру
{
    /*string a = "It's first place ";
    a += board[fy][fx][0];
    add_notice(a, 'y');*/
    if (board[fy][fx][0] == '0')
    {


        add_notice("This is not your figure!", 'r');
        add_notice(board[sy][sx], 'y');
        return turn;


    }
    else if (board[fy][fx][1] != turn)
    {
        add_notice("Figure not found!\n" + board[fy][fx] + " not a " + turn + " figure", 'r');
        return turn;

    }
    else if (is_place_valid(fx, fy, sx, sy, board[fy][fx], false))
    {
        if (board[sy][sx][0] != 'O')
        {
            switch (board[sy][sx][1])
            {
            case 'w':
                if (board[sy][sx][1] == turn)
                {
                    add_notice("You can't eat your figure\n" + board[sy][sx] + " is your", 'r');
                    return turn;
                }
                else
                {
                    w_killed.push_back(board[sy][sx][0]);
                    board[sy][sx] = board[fy][fx];
                    board[fy][fx] = 'O';
                    //add_notice(board[sy][sx], 'y');
                    turn = 'w';
                    return turn;
                }
                break;

            case 'b':
                if (board[sy][sx][1] == turn)
                {
                    int i = 0;
                    add_notice("You can't eat your figure\n" + board[sy][sx] + " is your", 'r');
                    return turn;
                }
                else
                {
                    b_killed.push_back(board[sy][sx][0]);
                    board[sy][sx] = board[fy][fx];
                    board[fy][fx] = 'O';
                    //add_notice(board[sy][sx], 'y');
                    turn = 'b';
                    return turn;
                }
                break;

            default:
                string notice = "ERROR: Wrong figure id ";
                notice += board[sy][sx][1];
                add_notice(notice, 'r');
                break;
            }
        }
        else
        {
            board[sy][sx] = board[fy][fx];
            board[fy][fx] = 'O';
            add_notice(board[sy][sx], 'y');
            if (turn == 'w')
            {
                turn = 'b';
                return turn;
            }
            else
            {
                turn = 'w';
                return turn;
            }
        }
    }
    else
    {
        add_notice("Invalid place", 'r');
        return turn;
    }
    add_notice("move_unit is bad", 'r');
    return turn;


}



char enter_act(char turn = 'w') // Ты закончил на том, чтобы создать функцию для ввода данных для хода чтобы не рекурсировать game
{
    if (turn == 'w') // Ход белых
    {
        cout << endl << "White turn:" << endl;

        string ent_fc;
        string ent_sc;
        cin >> ent_fc >> ent_sc;

        vector<int> fc = string_to_nums(ent_fc);
        vector<int> sc = string_to_nums(ent_sc);
        int fy = 8 - fc[1];
        int sy = 8 - sc[1];

        int fx = fc[0];
        int sx = sc[0];
        add_notice(board[fy][fx] + " enter_act alright " + board[sy][sx] + " ", 'y');
        /*
        int AS=0;
        cout<<turn<<"!!!"<<endl;
        cin>>AS;

        cout<<next_turn<<"!!!"<<endl;

        cin>>AS;
        */
        char next_turn = move_unit(fx, fy, sx, sy, turn);
        string notice;
        notice += next_turn;
        add_notice(notice, 'g');
        turn = next_turn;

    }
    else            // Ход чёрных
    {
        cout << endl << "Black turn:" << endl;
        string ent_fc;
        string ent_sc;
        cin >> ent_fc >> ent_sc;

        vector<int> fc = string_to_nums(ent_fc);
        vector<int> sc = string_to_nums(ent_sc);

        int fy = 8 - fc[1];
        int sy = 8 - sc[1];

        int fx = fc[0];
        int sx = sc[0];

        add_notice(board[fy][fx] + " enter_act alright " + board[sy][sx] + " ", 'y');
        //int a;
        //cout << board[fy][fx] << "(" << fy << " " << fx << ")" << " enter_act alright " << board[sy][sx] << "(" << sy << " " << sx << ")";
        //cin >> a;

        char next_turn = move_unit(fx, fy, sx, sy, turn);
        turn = next_turn;
    }
    return turn;
}

string find_king(char turn)
{
    string king_coords = "";
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            if (board[y][x][0] == 'K' && board[y][x][1] == turn)
            {
                king_coords += y;
                king_coords += x;
                return king_coords;
            }
        }
    }
}

bool is_end(char turn)
{
    
    if (turn == 'w')
    {
        string king_coords = find_king('b'); //Определяем Шах и Мат
        int king_y = king_coords[0] - 48;
        int king_x = king_coords[1] - 48;
        for (int fy = 0; fy < 8; fy++)
        {
            for (int fx = 0; fx < 8; fx++)
            {
                if (is_place_valid(fy, fx, king_y, king_x, board[fy][fx], true))
                {
                    add_notice("It's CHECKMATE!\nGame over, White player win's!", 'g');
                    return true;
                }
            }
        }

        king_coords = find_king('w');        //Определяем Шах
        king_y = king_coords[0] - 48;
        king_x = king_coords[1] - 48;
        for (int fy = 0; fy < 8; fy++)
        {
            for (int fx = 0; fx < 8; fx++)
            {
                if (is_place_valid(fy, fx, king_y, king_x, board[fy][fx], true))
                {
                    add_notice("Be carefull! It's CHECK!\nWhite, save your king!", 'g');
                    return false;
                }
            }
        }

    }
    else
    {
        string king_coords = find_king('w');
        int king_y = king_coords[0] - 48;
        int king_x = king_coords[1] - 48;
        for (int fy = 0; fy < 8; fy++)
        {
            for (int fx = 0; fx < 8; fx++)
            {
                if (is_place_valid(fy, fx, king_y, king_x, board[fy][fx], true))
                {
                    add_notice("It's CHECKMATE!\nGame over, Black player win's!", 'g');
                    return true;
                }
            }
        }

        king_coords = find_king('b'); //Определяем Шах и Мат
        king_y = king_coords[0] - 48;
        king_x = king_coords[1] - 48;
        for (int fy = 0; fy < 8; fy++)
        {
            for (int fx = 0; fx < 8; fx++)
            {
                if (is_place_valid(fy, fx, king_y, king_x, board[fy][fx], true))
                {
                    add_notice("Be carefull!It's CHECK!\n Black, save your king!", 'g');
                    return true;
                }
            }
        }
    }
}

void game(char turn = 'w') // Запуск игры
{
    clear_board();
    write_board();
    int f = 0;
    bool game_over = false;
    while (!game_over)
    {
        write_board();
        write_notices();
        f++;
        turn = enter_act(turn);
        is_end(turn);
    }

}



int main() // Это база
{
    game();
}
