#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <array>

#define enemy 3 - player

enum SPOT_STATE {
    EMPTY = 0,
    BLACK = 1,
    WHITE = 2
};

int player;
const int SIZE = 15;
std::array<std::array<int, SIZE>, SIZE> board;

class state {
public :
    int x, y;
    int alpha, beta;

    state(int xx = 0, int yy = 0) : x(xx), y(yy), alpha((int)-1e15), beta((int)1e15) {}
};

void getvalue(int aimx, int aimy) {
    int newval= 0;

    int atknum[3][3] = {0};  //左上、上、右上 | 左、無作用、右 | 左下、下、右下 
    for (int i = -1; i <= 1; i ++) {
        for (int j = -1; j <= 1; j ++) {
            for (int k = 1; k <= 4; k ++) {
                if (aimx + k * i < 0 || aimx + k * i >= SIZE || aimy + k * j < 0 || aimy + k * j >= SIZE)
                    continue;

                if (board[aimx + k * i][aimy + k * j] == player)
                    atknum[aimx + 1][aimy + 1] ++;
                else    
                    break;    
            }
        }
    }

    int up1_idx = aimx - atknum[0][1] - 1, up2_idx = up1_idx - 1, up3_idx = up2_idx - 1;
    int down1_idx = aimx + atknum[2][1] + 1, down2_idx = down1_idx - 1, down3_idx = down2_idx - 1;
    int left1_idx = aimy - atknum[1][0] - 1, left2_idx = left1_idx - 1, left3_idx = left2_idx - 1;
    int right1_idx = aimy + atknum[1][2] + 1, right2_idx = right1_idx - 1, right3_idx = right2_idx - 1;

    
    switch (atknum[1][0] + atknum[1][2]) {
        case 8:
        case 7:
        case 6:
        case 5:
        case 4:
            newval += 100000;
            break;
        
        case 3: {
            if (left1_idx < 0) {
                if (!board[aimx][right1_idx]) 
                    newval += 10000;
                break;
            }
            else if (right1_idx >= SIZE) {
                if (!board[aimx][left1_idx])
                    newval += 10000;
                break;
            }

            int left1 = board[aimx][left1_idx], right1 = board[aimx][right1_idx];
            if (left1 + right1 == enemy) {
                newval += 10000;
                break;
            }
            else if (!(left1 + right1)) {
                newval += 20000;
                break;
            }

            break;
        }

        case 2: {
            if (left1_idx < 0) {
                if (!board[aimx][right1_idx] && !board[aimx][right2_idx]) {
                    newval += 5500;
                    break;
                }  // |XXX__
                else if (!board[aimx][right1_idx] && board[aimx][right2_idx] == player) {
                    newval += 7000;
                    break;
                }  // |XXX_X
                break;

            }
            else if (right1_idx >= SIZE) {
                if (!board[aimx][left1_idx] && !board[aimx][left2_idx]) {
                    newval += 5500;
                    break;
                }  // __XXX|
                else if (!board[aimx][left1_idx] && board[aimx][left2_idx] == player) {
                    newval += 7000;
                    break;
                }  // X_XXX|
                break;

            }

            int left1 = board[aimx][left1_idx], right1 = board[aimx][right1_idx];
            if (!left1 && !right1) {

                if (left2_idx < 0) {
                    int right2 = board[aimx][right2_idx];
                    if (!right2)
                        newval += 6200;
                    else if (right2 == player)
                        newval += 7000;
                    else 
                        newval += 5500;
                    break;
                }  // |_XXX_?
                else if (right2_idx >= SIZE) {
                    int left2 = board[aimx][left2_idx];
                    if (!left2) 
                        newval += 6200;
                    else if (left2 == player)
                        newval += 7000;
                    else 
                        newval += 5500;
                    break;
                }  // ?_XXX_|

                int left2 = board[aimx][left2_idx], right2 = board[aimx][right2_idx];
                if (left2 == player && right2 == player) {
                    newval += 20000;
                    break;
                }  // X_XXX_X
                else if (left2 == enemy && right2 == enemy) {
                    newval += 5500;
                    break;
                }  // O_XXX_O
 
                int sum2 = left2 + right2;
                if (!sum2)
                    newval += 6500;
                else if (sum2 == player)
                    newval += 7100;
                else if (sum2 == enemy)
                    newval += 6200;
                else if (sum2 == 2 * enemy)
                    newval += 7000;
                break;
                // ?_XXX_?
            
            }
            else if (left1 == enemy && !right1) {
                int right2 = board[aimx][right2_idx];
                if (!right2)
                    newval += 5500;
                else if (right2 == player)
                    newval += 7000;
                break;

            }
            else if (right1 == enemy && !left1) {
                int left2 = board[aimx][left2_idx];
                if (!left2)
                    newval += 5500;
                else if (left2 == player)
                    newval += 7000;
                break;

            }
           
            break;
        }

        case 1: {
            if (left1_idx < 0) {
                if (!board[aimx][right1_idx]) {
                    if (!board[aimx][right2_idx]) {
                        if (!board[aimx][right3_idx]) {
                            newval += 2000;
                            break;
                        }  // |XX___
                        else if (board[aimx][right3_idx] == player) {
                            newval += 5500;
                            break;
                        }  // |XX__X

                    }  // |XX__?
                    else if (board[aimx][right2_idx] == player) {
                        if (!board[aimx][right3_idx]) {
                            newval += 5500;
                            break;
                        }  // |XX_X_

                    } 

                }
                break;
            }  // |XX_??
            else if (right1_idx >= SIZE) {
                if (!board[aimx][left1_idx]) {
                    if (!board[aimx][left2_idx]) {
                        if (!board[aimx][left3_idx]) {
                            newval += 2000;
                            break;
                        }
                        else if (board[aimx][left3_idx] == player) {
                            newval += 5500;
                            break;
                        }

                    }
                    else if (board[aimx][left2_idx] == player) {
                        if (!board[aimx][left3_idx]) {
                            newval += 5500;
                            break;
                        }
                    }

                }
                break;
            }
        
            int left1 = board[aimx][left1_idx], right1 = board[aimx][right1_idx];
            if (!left1 && !right1) {

                if (left2_idx < 0) {
                    
                    int right2 = board[aimx][right2_idx], right3 = board[aimx][right3_idx];
                    if (!right2) {
                        if (!right3) 
                            newval += 3000;
                        else if (right3 == player)
                            newval += 5500;
                        else 
                            newval += 2000;
                        break;

                    }
                    else if (right2 == player) {
                        if (!right3)
                            newval += 6000;
                        else if (right3 == enemy)
                            newval += 5500;
                        break;

                    }
                    
                    break;
                }  // |_XX_??
                else if (right2_idx >= SIZE) {
                    
                    int left2 = board[aimx][left2_idx], left3 = board[aimx][left3_idx];
                    if (!left2) {
                        if (!left3)
                            newval += 3000;
                        else if (left3 == player)
                            newval += 5500;
                        else 
                            newval += 2000;
                        break;

                    }
                    else if (left2 == player) {
                        if (!left3)
                            newval += 6000;
                        else if (left3 == player)
                            newval += 5000;
                        break;

                    }
                    
                    break;
                }

                int left2 = board[aimx][left2_idx], right2 = board[aimx][right2_idx], sum2 = left2 + right2;
                if (!left2 && !right2) {
                    if (left3_idx < 0) {

                        int right3 = board[aimx][right3_idx];
                        if (!right3)
                            newval += 4000;
                        else if (right3 == player)
                            newval += 5700;
                        else 
                            newval += 3500;
                        break;
                        // |__XX__?
                    }
                    else if (right3_idx >= SIZE) {

                        int left3 = board[aimx][left3_idx];
                        if (!left3)
                            newval += 4000;
                        else if (left3 == player)
                            newval += 5700;
                        else 
                            newval += 3500;
                        break;
                        // ?_XX__|
                    }

                    int left3 = board[aimx][left3_idx], right3 = board[aimx][right3_idx], sum3 = left3 + right3;
                    if (left3 == enemy && right3 == enemy) 
                        newval += 3500;
                    else if (!sum3)
                        newval += 5000;
                    else if (sum3 == player)
                        newval += 6100;
                    else if (sum3 == enemy)
                        newval += 4000;
                    else if (sum3 == 3)
                        newval += 5700;

                    break;
                }  //?__XX__?
                else if (left2 == player && right2 == player) {
                    if (board[aimx][left3_idx] + board[aimx][right3_idx] == enemy)
                        newval += 6500;
                    break;
                }
                else if (sum2 == player) {
                    int sum3 = board[aimx][left3_idx] + board[aimx][right3_idx];
                    if (sum3 == 2 * enemy) {
                        newval += 5500;
                        break;
                    }
                    else if ((left2 == player && board[aimx][right3_idx] == enemy) || (right2 == player && board[aimx][left3_idx])) {
                        newval += 6300;
                        break;
                    }


                    if (!sum3)
                        newval += 6300;
                    else if (sum3 == player)
                        newval += 6400; 
                    else if (sum3 == enemy)
                        newval += 5500;
                    else if (sum3 == 3)
                        newval += 6500;
                    break;

                }
                else if (sum2 == enemy) {
                    int sum3 = board[aimx][left3_idx] + board[aimx][right3_idx];
                    if (!sum3)
                        newval += 3000; 
                    else if (sum3 == player)
                        newval += 5500;
                    else if (sum3 == enemy)
                        newval += 2000;
                    break;

                }
                else if (sum2 == 3) {
                    int sum3 = board[aimx][left3_idx] + board[aimx][right3_idx];
                    if (!sum3)
                        newval += 6000;
                    else if (sum3 == enemy)
                        newval += 5500;
                    break;

                }

                break;
            }  // ??_XX_|
            else if (left1 == enemy && !right1) {

                int sum = board[aimx][right2_idx] + board[aimx][right3_idx];
                if (sum == player)
                    newval += 5500;
                else if (!sum)
                    newval += 2000;
                break;

            }  //OXX_??
            else if (right1 == enemy && !left1) {
                int sum = board[aimx][left2_idx] + board[aimx][left3_idx];
                if (sum == player)
                    newval += 5500;
                else if (!sum)
                    newval += 2000;
                break;
            }
        
            break;
        }
    }
    //水平線

    switch (atknum[0][1] + atknum[2][1]) {
        case 8:
        case 7:
        case 6:
        case 5:
        case 4:
            newval += 100000;
            break;
        
        case 3: {
            if (up1_idx < 0) {
                if (!board[down1_idx][aimy]) 
                    newval += 10000;
                break;
            }
            else if (down1_idx >= SIZE) {
                if (!board[up1_idx][aimy])
                    newval += 10000;
                break;
            }

            int up1 = board[up1_idx][aimy], down1 = board[down1_idx][aimy];
            if (up1 + down1 == enemy) {
                newval += 10000;
                break;
            }
            else if (!(up1 + down1)) {
                newval += 20000;
                break;
            }

            break;
        }

        case 2: {
            if (up1_idx < 0) {
                if (!board[down1_idx][aimy] && !board[down2_idx][aimy]) {
                    newval += 5500;
                    break;
                }  // |XXX__
                else if (!board[down1_idx][aimy] && board[down2_idx][aimy] == player) {
                    newval += 7000;
                    break;
                }  // |XXX_X
                break;

            }
            else if (down1_idx >= SIZE) {
                if (!board[up1_idx][aimy] && !board[up2_idx][aimy]) {
                    newval += 5500;
                    break;
                }  // __XXX|
                else if (!board[up1_idx][aimy] && board[up2_idx][aimy] == player) {
                    newval += 7000;
                    break;
                }  // X_XXX|
                break;
                
            }

            int up1 = board[up1_idx][aimy], down1 = board[down1_idx][aimy];
            if (!up1 && !down1) {

                if (up2_idx < 0) {
                    int down2 = board[down2_idx][aimy];
                    if (!down2)
                        newval += 6200;
                    else if (down2 == player)
                        newval += 7000;
                    else 
                        newval += 5500;
                    break;
                }  // |_XXX_?
                else if (down2_idx >= SIZE) {
                    int up2 = board[up2_idx][aimy];
                    if (!up2) 
                        newval += 6200;
                    else if (up2 == player)
                        newval += 7000;
                    else 
                        newval += 5500;
                    break;
                }  // ?_XXX_|

                int up2 = board[up2_idx][aimy], down2 = board[down2_idx][aimy];
                if (up2 == player && down2 == player) {
                    newval += 20000;
                    break;
                }  // X_XXX_X
                else if (up2 == enemy && down2 == enemy) {
                    newval += 5500;
                    break;
                }  // O_XXX_O
 
                int sum2 = up2 + down2;
                if (!sum2)
                    newval += 6500;
                else if (sum2 == player)
                    newval += 7100;
                else if (sum2 == enemy)
                    newval += 6200;
                else if (sum2 == 2 * enemy)
                    newval += 7000;
                break;
                // ?_XXX_?
            
            }
            else if (up1 == enemy && !down1) {
                int down2 = board[down2_idx][aimy];
                if (!down2)
                    newval += 5500;
                else if (down2 == player)
                    newval += 7000;
                break;

            }
            else if (down1 == enemy && !up1) {
                int up2 = board[up2_idx][aimy];
                if (!up2)
                    newval += 5500;
                else if (up2 == player)
                    newval += 7000;
                break;

            }
           
            break;
        }

        case 1: {
            if (up1_idx < 0) {
                if (!board[down1_idx][aimy]) {
                    if (!board[down2_idx][aimy]) {
                        if (!board[down3_idx][aimy]) {
                            newval += 2000;
                            break;
                        }  // |XX___
                        else if (board[down3_idx][aimy] == player) {
                            newval += 5500;
                            break;
                        }  // |XX__X

                    }  // |XX__?
                    else if (board[down2_idx][aimy] == player) {
                        if (!board[down3_idx][aimy]) {
                            newval += 5500;
                            break;
                        }  // |XX_X_

                    } 

                }
                break;
            }  // |XX_??
            else if (down1_idx >= SIZE) {
                if (!board[up1_idx][aimy]) {
                    if (!board[up2_idx][aimy]) {
                        if (!board[up3_idx][aimy]) {
                            newval += 2000;
                            break;
                        }
                        else if (board[up3_idx][aimy] == player) {
                            newval += 5500;
                            break;
                        }

                    }
                    else if (board[up2_idx][aimy] == player) {
                        if (!board[up3_idx][aimy]) {
                            newval += 5500;
                            break;
                        }
                    }

                }
                break;
            }
        
            int up1 = board[up1_idx][aimy], down1 = board[down1_idx][aimy];
            if (!up1 && !down1) {

                if (up2_idx < 0) {
                    
                    int down2 = board[down2_idx][aimy], down3 = board[down3_idx][aimy];
                    if (!down2) {
                        if (!down3) 
                            newval += 3000;
                        else if (down3 == player)
                            newval += 5500;
                        else 
                            newval += 2000;
                        break;

                    }
                    else if (down2 == player) {
                        if (!down3)
                            newval += 6000;
                        else if (down3 == enemy)
                            newval += 5500;
                        break;

                    }
                    
                    break;
                }  // |_XX_??
                else if (down2_idx >= SIZE) {
                    
                    int up2 = board[up2_idx][aimy], up3 = board[up3_idx][aimy];
                    if (!up2) {
                        if (!up3)
                            newval += 3000;
                        else if (up3 == player)
                            newval += 5500;
                        else 
                            newval += 2000;
                        break;

                    }
                    else if (up2 == player) {
                        if (!up3)
                            newval += 6000;
                        else if (up3 == player)
                            newval += 5000;
                        break;

                    }
                    
                    break;
                }

                int up2 = board[up2_idx][aimy], down2 = board[down2_idx][aimy], sum2 = up2 + down2;
                if (!up2 && !down2) {
                    if (up3_idx < 0) {

                        int down3 = board[down3_idx][aimy];
                        if (!down3)
                            newval += 4000;
                        else if (down3 == player)
                            newval += 5700;
                        else 
                            newval += 3500;
                        break;
                        // |__XX__?
                    }
                    else if (down3_idx >= SIZE) {

                        int up3 = board[up3_idx][aimy];
                        if (!up3)
                            newval += 4000;
                        else if (up3 == player)
                            newval += 5700;
                        else 
                            newval += 3500;
                        break;
                        // ?_XX__|
                    }

                    int up3 = board[up3_idx][aimy], down3 = board[down3_idx][aimy], sum3 = up3 + down3;
                    if (up3 == enemy && down3 == enemy) 
                        newval += 3500;
                    else if (!sum3)
                        newval += 5000;
                    else if (sum3 == player)
                        newval += 6100;
                    else if (sum3 == enemy)
                        newval += 4000;
                    else if (sum3 == 3)
                        newval += 5700;

                    break;
                }  //?__XX__?
                else if (up2 == player && down2 == player) {
                    if (board[up3_idx][aimy] + board[down3_idx][aimy] == enemy)
                        newval += 6500;
                    break;
                }
                else if (sum2 == player) {
                    int sum3 = board[up3_idx][aimy] + board[down3_idx][aimy];
                    if (sum3 == 2 * enemy) {
                        newval += 5500;
                        break;
                    }
                    else if ((up2 == player && board[down3_idx][aimy] == enemy) || (down2 == player && board[up3_idx][aimy])) {
                        newval += 6300;
                        break;
                    }


                    if (!sum3)
                        newval += 6300;
                    else if (sum3 == player)
                        newval += 6400; 
                    else if (sum3 == enemy)
                        newval += 5500;
                    else if (sum3 == 3)
                        newval += 6500;
                    break;

                }
                else if (sum2 == enemy) {
                    int sum3 = board[up3_idx][aimy] + board[down3_idx][aimy];
                    if (!sum3)
                        newval += 3000; 
                    else if (sum3 == player)
                        newval += 5500;
                    else if (sum3 == enemy)
                        newval += 2000;
                    break;

                }
                else if (sum2 == 3) {
                    int sum3 = board[up3_idx][aimy] + board[down3_idx][aimy];
                    if (!sum3)
                        newval += 6000;
                    else if (sum3 == enemy)
                        newval += 5500;
                    break;

                }

                break;
            }  // ??_XX_|
            else if (up1 == enemy && !down1) {

                int sum = board[down2_idx][aimy] + board[down3_idx][aimy];
                if (sum == player)
                    newval += 5500;
                else if (!sum)
                    newval += 2000;
                break;

            }  //OXX_??
            else if (down1 == enemy && !up1) {
                int sum = board[up2_idx][aimy] + board[up3_idx][aimy];
                if (sum == player)
                    newval += 5500;
                else if (!sum)
                    newval += 2000;
                break;
            }
        
            break;
        }
    }
    //垂直線


    int defnum[3][3] = {0};  //左上、上、右上 | 左、無作用、右 | 左下、下、右下 
    for (int i = -1; i <= 1; i ++) {
        for (int j = -1; j <= 1; j ++) {
            for (int k = 1; k <= 4; k ++) {
                if (aimx + k * i < 0 || aimx + k * i >= SIZE || aimy + k * j < 0 || aimy + k * j >= SIZE)
                    continue;

                if (board[aimx + k * i][aimy + k * j] == enemy)
                    defnum[aimx + 1][aimy + 1] ++;
                else    
                    break;    
            }
        }
    }

}
// 算完後放二維陣列，不然每次都要重算



void read_board(std::ifstream& fin) {
    fin >> player;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fin >> board[i][j];
        }
    }
}

void write_valid_spot(std::ofstream& fout) {
    srand(time(NULL));
    int x, y;          
    
    while(true) {
        // Choose a random spot.
        int x = (rand() % SIZE);
        int y = (rand() % SIZE);

        
        if (board[x][y] == EMPTY) {
            fout << x << " " << y << std::endl;
            // Remember to flush the output to ensure the last action is written to file.
            fout.flush();
        }
    }
    // Keep updating the output until getting killed. 6
}
// Remember to flush the output to ensure the last action is written to file.
// fout.flush();

int main(int, char** argv) {
    std::ifstream fin(argv[1]);
    std::ofstream fout(argv[2]);
    read_board(fin);
    write_valid_spot(fout);
    fin.close();
    fout.close();
    return 0;
}
