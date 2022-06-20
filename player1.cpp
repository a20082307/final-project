#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <array>
#include <set>

#define enemy 3 - player

enum SPOT_STATE {
    EMPTY = 0,
    BLACK = 1,
    WHITE = 2
};

int player;
const int SIZE = 15;
std::array<std::array<int, SIZE>, SIZE> board;

int getvalue(int aimx, int aimy) {
    int newval= 0;

    int atknum[3][3] = {0};  //左上、上、右上 | 左、無作用、右 | 左下、下、右下 
    for (int i = -1; i <= 1; i ++) {
        for (int j = -1; j <= 1; j ++) {
            for (int k = 1; k <= 4; k ++) {
                if (aimx + k * i < 0 || aimx + k * i >= SIZE || aimy + k * j < 0 || aimy + k * j >= SIZE)
                    break;

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
            newval += 1000000;
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
            newval += 1000000;
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

    int lu1_x = aimx - atknum[0][0] - 1, lu2_x = lu1_x - 1, lu3_x = lu2_x - 1;
    int lu1_y = aimy - atknum[0][0] - 1, lu2_y = lu1_y - 1, lu3_y = lu2_y - 1;  //左上
    int ru1_x = aimx + atknum[0][2] + 1, ru2_x = ru1_x + 1, ru3_x = ru2_x + 1;
    int ru1_y = aimy - atknum[0][2] - 1, ru2_y = ru1_y - 1, ru3_y = ru2_y - 1;  //右上
    int ld1_x = aimx - atknum[2][0] - 1, ld2_x = ld1_x - 1, ld3_x = ld2_x - 1;
    int ld1_y = aimy + atknum[2][0] + 1, ld2_y = ld1_y + 1, ld3_y = ld2_y + 1;  //左下
    int rd1_x = aimx + atknum[2][2] + 1, rd2_x = rd1_x + 1, rd3_x = rd2_x + 1;
    int rd1_y = aimy + atknum[2][2] + 1, rd2_y = rd2_y + 1, rd3_y = rd2_y + 1;  //右下

    switch (atknum[0][0] + atknum[2][2]) {
        case 8:
        case 7:
        case 6:
        case 5:
        case 4:
            newval += 1000000;
            break;
        
        case 3: {
            if (lu1_x < 0 || lu1_y < 0) {\
                if (rd1_x >= SIZE || rd1_y >= SIZE)
                    break;
                  // |XXXX|

                if (!board[rd1_x][rd1_y]) 
                    newval += 10000;
                  // |XXXX_
                break;
            }
            else if (rd1_x >= SIZE || rd1_y >= SIZE) {
                if (!board[lu1_x][lu1_y])
                    newval += 10000;
                  // _XXXX|
                break;
            }

            int lu1 = board[lu1_x][lu1_y], rd1 = board[rd1_x][rd1_y];
            if (lu1 + rd1 == enemy) {
                newval += 10000;
                break;
            }  // OXXXX_
            else if (!(lu1 + rd1)) {
                newval += 20000;
                break;
            }  // _XXXX_

            break;
        }

        case 2: {
            if (lu1_x < 0 || lu1_y < 0) {
                if (rd1_x >= SIZE || rd1_y >= SIZE)
                    break;
                  // |XXX|

                if (rd2_x >= SIZE || rd2_y >= SIZE)
                    break;
                  // |XXX_|

                if (!board[rd1_x][rd1_y] && !board[rd2_x][rd2_y]) {
                    newval += 5500;
                    break;
                }  // |XXX__
                else if (!board[rd1_x][rd1_y] && board[rd2_x][rd2_y] == player) {
                    newval += 7000;
                    break;
                }  // |XXX_X
                break;

            }
            else if (rd1_x >= SIZE || rd1_y >= SIZE) {
                if (lu2_x < 0 || lu2_y < 0)
                    break;
                  // |_XXX|

                if (!board[lu1_x][lu1_y] && !board[lu2_x][lu2_y]) {
                    newval += 5500;
                    break;
                }  // __XXX|
                else if (!board[lu1_x][lu1_y] && board[lu2_x][lu2_y] == player) {
                    newval += 7000;
                    break;
                }  // X_XXX|
                break;
                
            }

            int lu1 = board[lu1_x][lu1_y], rd1 = board[rd1_x][rd1_y];
            if (!lu1 && !rd1) {

                if (lu2_x < 0 || lu2_y) {
                    if (rd2_x >= SIZE || rd2_y >= SIZE) {
                        newval += 5500;
                        break;
                    }  // |_XXX_|
  
                    int rd2 = board[rd2_x][rd2_y];
                    if (!rd2)
                        newval += 6200;  // |_XXX__
                    else if (rd2 == player)
                        newval += 7000;  // |_XXX_X
                    else 
                        newval += 5500;  // |_XXX_O
                    break;
                }  // |_XXX_?
                else if (rd2_x >= SIZE || rd2_y >= SIZE) {

                    int lu2 = board[lu2_x][lu2_y];
                    if (!lu2) 
                        newval += 6200;  // __XXX_|
                    else if (lu2 == player)
                        newval += 7000;  // X_XXX_|
                    else 
                        newval += 5500;  // O_XXX_|
                    break;
                }  // ?_XXX_|

                int lu2 = board[lu2_x][lu2_y], rd2 = board[rd2_x][rd2_y];
                if (lu2 == player && rd2 == player) {
                    newval += 20000;
                    break;
                }  // X_XXX_X
                else if (lu2 == enemy && rd2 == enemy) {
                    newval += 5500;
                    break;
                }  // O_XXX_O
 
                int sum2 = lu2 + rd2;
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
            else if (lu1 == enemy && !rd1) {
                if (rd2_x >= SIZE || rd2_y >= SIZE)
                    break;
                  // OXXX_|

                int rd2 = board[rd2_x][rd2_y];
                if (!rd2)
                    newval += 5500;  // OXXX__
                else if (rd2 == player)
                    newval += 7000;  // OXXX_X
                break;

            }  // OXXX_?
            else if (rd1 == enemy && !lu1) {
                if (lu2_x < 0 || lu2_y < 0)
                    break;
                  // |_XXXO

                int lu2 = board[lu2_x][lu2_y];
                if (!lu2)
                    newval += 5500;  // __XXXO
                else if (lu2 == player)
                    newval += 7000;  // X_XXXO
                break;

            }  // ?_XXXO
           
            break;
        }

        case 1: {
            if (lu1_x < 0 || lu1_y < 0) {
                if (rd1_x >= SIZE || rd1_y >= SIZE)
                    break;
                  // |XX|

                if (!board[rd1_x][rd1_y]) {
                    if (rd2_x >= SIZE || rd2_y >= SIZE)
                        break;
                      // |XX_|

                    if (!board[rd2_x][rd2_y]) {
                        if (rd3_x >= SIZE || rd3_y >= SIZE)
                            break;
                          // |XX__|

                        if (!board[rd3_x][rd3_y]) {
                            newval += 2000;
                            break;
                        }  // |XX___
                        else if (board[rd3_x][rd3_y] == player) {
                            newval += 5500;
                            break;
                        }  // |XX__X

                    }  // |XX__?
                    else if (board[rd2_x][rd2_y] == player) {
                        if (rd3_x >= SIZE || rd3_y >= SIZE)
                            break;
                          // |XX_X|

                        if (!board[rd3_x][rd3_y]) {
                            newval += 5500;
                            break;
                        }  // |XX_X_

                    } 

                }
                break;
            }  // |XX_??
            else if (rd1_x >= SIZE || rd1_y >= SIZE) {

                if (!board[lu1_x][lu1_y]) {
                    if (rd2_x >= SIZE || rd2_y >= SIZE)
                        break;
                      // |_XX|

                    if (!board[lu2_x][lu2_y]) {
                        if (rd3_x >= SIZE || rd3_y >= SIZE)
                            break;
                          // |__XX|

                        if (!board[lu3_x][lu3_y]) {
                            newval += 2000;
                            break;
                        }  // ___XX|
                        else if (board[lu3_x][lu3_y] == player) {
                            newval += 5500;
                            break;
                        }  // X__XX|

                    }
                    else if (board[lu2_x][lu2_y] == player) {
                        if (lu3_x < 0 || lu3_y < 0)
                            break;
                          // |X_XX|

                        if (!board[lu3_x][lu3_y]) {
                            newval += 5500;
                            break;
                        }  // _X_XX|
                    }

                }
                break;
            }
        
            int lu1 = board[lu1_x][lu1_y], rd1 = board[rd1_x][rd1_y];
            if (!lu1 && !rd1) {

                if (lu2_x < 0 || lu2_y < 0) {
                    if (rd2_x >= SIZE || rd2_y >= SIZE)
                        break;
                      // |_XX_|
                    
                    int rd2 = board[rd2_x][rd2_y], rd3 = board[rd3_x][rd3_y];
                    if (!rd2) {
                        if (rd3_x >= SIZE || rd3_y >= SIZE) {
                            newval += 2000;
                            break;
                        }  // |_XX__|

                        if (!rd3) 
                            newval += 3000;  // |_XX___
                        else if (rd3 == player)
                            newval += 5500;  // |_XX__X
                        else 
                            newval += 2000;  // |_XX__O
                        break;

                    }
                    else if (rd2 == player) {
                        if (rd3_x >= SIZE || rd3_y >= SIZE) {
                            newval += 5500;
                            break;
                        }  // |_XX_X|

                        if (!rd3)
                            newval += 6000;  // |_XX_X_
                        else if (rd3 == enemy)
                            newval += 5500;  // |_XX_XO
                        break;

                    }
                    
                    break;
                }  // |_XX_??
                else if (rd2_x >= SIZE || rd2_y >= SIZE) {
                    
                    int lu2 = board[lu2_x][lu2_y], lu3 = board[lu3_x][lu3_y];
                    if (!lu2) {
                        if (lu3_x < 0 || lu3_y < 0) {
                            newval += 2000;
                            break;
                        }  // |__XX_|
                        
                        if (!lu3)
                            newval += 3000;  // ___XX_|
                        else if (lu3 == player)
                            newval += 5500;  // X__XX_|
                        else 
                            newval += 2000;  // O__XX_|
                        break;

                    }
                    else if (lu2 == player) {
                        if (rd3_x >= SIZE || rd3_y >= SIZE) {
                            newval += 5500;
                            break;
                        }  // |X_XX_|

                        if (!lu3)
                            newval += 6000;  // _X_XX_|
                        else if (lu3 == enemy)
                            newval += 5500;  // OX_XX_|
                        break;

                    }
                    
                    break;
                }

                int lu2 = board[lu2_x][lu2_y], rd2 = board[rd2_x][rd2_y], sum2 = lu2 + rd2;
                if (!lu2 && !rd2) {
                    if (lu3_x < 0 || lu3_y < 0) {
                        if (rd3_x >= SIZE || rd3_y >= SIZE) {
                            newval += 3500;
                            break;
                        }  // |__XX__|

                        int rd3 = board[rd3_x][rd3_y];
                        if (!rd3)
                            newval += 4000;  // |__XX___
                        else if (rd3 == player)
                            newval += 5700;  // |__XX__X
                        else 
                            newval += 3500;  // |__XX__O
                        break;
                        // |__XX__?
                    }
                    else if (rd3_x >= SIZE || rd3_y >= SIZE) {

                        int lu3 = board[lu3_x][lu3_y];
                        if (!lu3)
                            newval += 4000;  // ___XX__|
                        else if (lu3 == player) 
                            newval += 5700;  // X__XX__|
                        else 
                            newval += 3500;  // O__XX__|
                        break;
                        // ?__XX__|
                    }

                    int lu3 = board[lu3_x][lu3_y], rd3 = board[rd3_x][rd3_y], sum3 = lu3 + rd3;
                    if (lu3 == enemy && rd3 == enemy) 
                        newval += 3500;  // O__XX__O
                    else if (!sum3)
                        newval += 5000;  // ___XX___
                    else if (sum3 == player)
                        newval += 6100;  // X__XX___  or  ___XX__X
                    else if (sum3 == enemy)
                        newval += 4000;  // O__XX___  or  ___XX__O
                    else if (sum3 == 3)
                        newval += 5700;  // O__XX__X  or  X__XX__O

                    break;
                }  // ?__XX__?
                else if (lu2 == player && rd2 == player) {
                    if (lu3_x < 0 || lu3_y < 0) {
                        newval += 6500;
                        break;
                    }  // |X_XX_X_
                    if (rd3_x >= SIZE || rd3_y >= SIZE) {
                        newval += 6500;
                        break;
                    }  // _X_XX_X|

                    if (board[lu3_x][lu3_y] + board[rd3_x][rd3_y] == enemy)
                        newval += 6500;  // OX_XX_X_  or  _X_XX_XO
                    break;
                }
                else if (sum2 == player) {
                    if (lu3_x < 0 || lu3_y < 0) {
                        if (rd3_x >= SIZE || rd3_y >= SIZE) {
                            newval += 5500;
                            break;
                        }  // |X_XX__|  or  |__XX_X|

                        int lu3 = board[lu3_x][lu3_y], rd3 = board[rd3_x][rd3_y];
                        if (lu2 + lu3 == player || rd2 + rd3 == player) {
                            newval += 6300;
                            break;
                        }  // |__XX_X_  or  _X_XX__|
                    }
                    int sum3 = board[lu3_x][lu3_y] + board[rd3_x][rd3_y];
                    if (sum3 == 2 * enemy) {
                        newval += 5500;
                        break;
                    }  // OX_XX__O  or  O__XX_XO
                    else if ((lu2 == player && board[rd3_x][rd3_y] == enemy && !board[lu3_x][lu3_y]) || (rd2 == player && board[lu3_x][lu3_y] && !board[rd3_x][rd3_y])) {
                        newval += 6300;
                        break;
                    }  // _X_XX__O  or  O__XX_X_

                    int lu3 = board[lu3_x][lu3_y];
                    if (!sum3)
                        newval += 6300;  // _X_XX___  or  ___XX_X_
                    else if (sum3 == player && lu3 + lu2 == player)
                        newval += 6400;  // _X_XX__X  or  X__XX_X_
                    else if (sum3 == enemy && lu2 + lu3 == 3)
                        newval += 5500;  // OX_XX___  or  ___XX_XO
                    else if (sum3 == enemy && lu2 + lu3 != 3)
                        newval += 6300;  // O__XX_X_  or  _X_XX__O
                    else if (sum3 == 3 && lu2 + lu3 == 3)
                        newval += 6500;  // OX_XX__X  or  X__XX_XO  
                    break;

                }
                else if (sum2 == enemy) {
                    int lu3 = board[lu3_x][lu3_y], sum3 = board[lu3_x][lu3_y] + board[rd3_x][rd3_y];
                    if (!sum3)
                        newval += 3000;  // _O_XX___  or  ___XX_O_
                    else if (sum3 == player && lu2 + lu3 != 3)
                        newval += 5500;  // _O_XX__X  or  X__XX_O_
                    else if (sum3 == enemy && lu2 + lu3 == enemy)
                        newval += 2000;  // _O_XX__O  or  O__XX_O_
                    break;

                }
                else if (sum2 == 3) {
                    int lu3 = board[lu3_x][lu3_y], sum3 = board[lu3_x][lu3_y] + board[rd3_x][rd3_y];
                    if (!sum3)
                        newval += 6000;  // _O_XX_X_  or  _X_XX_O_
                    else if (sum3 == enemy && lu2 + lu3 != player)
                        newval += 5500;  // _O_XX_XO  or  OX_XX_O_
                    break;

                }

                break;
            }  // ??_XX_|
            else if (lu1 == enemy && !rd1) {
                if (rd2_x >= SIZE || rd2_y >= SIZE)
                    break;
                  // OXX_|

                int sum2 = board[rd2_x][rd2_y] + board[rd3_x][rd3_y];
                if (sum2 == player)
                    newval += 5500;  // OXX_X_  or  OXX__X
                else if (!sum2)
                    newval += 2000;  // OXX___ 
                break;

            }  //OXX_??
            else if (rd1 == enemy && !lu1) {
                int sum2 = board[lu2_x][lu2_y] + board[lu3_x][lu3_y];
                if (sum2 == player)
                    newval += 5500;  // _X_XXO  or  X__XXO
                else if (!sum2)
                    newval += 2000;  // ___XXO
                break;
            }
        
            break;
        }
    }  
    //左上到右下的斜線

    switch (atknum[2][0] + atknum[0][2]) {
        case 8:
        case 7:
        case 6:
        case 5:
        case 4:
            newval += 1000000;
            break;
        
        case 3: {
            if (ld1_x < 0 || ld1_y < 0) {\
                if (ru1_x >= SIZE || ru1_y >= SIZE)
                    break;
                  // |XXXX|

                if (!board[ru1_x][ru1_y]) 
                    newval += 10000;
                  // |XXXX_
                break;
            }
            else if (ru1_x >= SIZE || ru1_y >= SIZE) {
                if (!board[ld1_x][ld1_y])
                    newval += 10000;
                  // _XXXX|
                break;
            }

            int ld1 = board[ld1_x][ld1_y], ru1 = board[ru1_x][ru1_y];
            if (ld1 + ru1 == enemy) {
                newval += 10000;
                break;
            }  // OXXXX_
            else if (!(ld1 + ru1)) {
                newval += 20000;
                break;
            }  // _XXXXO

            break;
        }

        case 2: {
            if (ld1_x < 0 || ld1_y < 0) {
                if (ru1_x >= SIZE || ru1_y >= SIZE)
                    break;
                  // |XXX|

                if (ru2_x >= SIZE || ru2_y >= SIZE)
                    break;
                  // |XXX_|

                if (!board[ru1_x][ru1_y] && !board[ru2_x][ru2_y]) {
                    newval += 5500;
                    break;
                }  // |XXX__
                else if (!board[ru1_x][ru1_y] && board[ru2_x][ru2_y] == player) {
                    newval += 7000;
                    break;
                }  // |XXX_X
                break;

            }
            else if (ru1_x >= SIZE || ru1_y >= SIZE) {
                if (ld2_x < 0 || ld2_y < 0)
                    break;
                  // |_XXX|

                if (!board[ld1_x][ld1_y] && !board[ld2_x][ld2_y]) {
                    newval += 5500;
                    break;
                }  // __XXX|
                else if (!board[ld1_x][ld1_y] && board[ld2_x][ld2_y] == player) {
                    newval += 7000;
                    break;
                }  // X_XXX|
                break;
                
            }

            int ld1 = board[ld1_x][ld1_y], ru1 = board[ru1_x][ru1_y];
            if (!ld1 && !ru1) {

                if (ld2_x < 0 || ld2_y) {
                    if (ru2_x >= SIZE || ru2_y >= SIZE) {
                        newval += 5500;
                        break;
                    }  // |_XXX_|
  
                    int ru2 = board[ru2_x][ru2_y];
                    if (!ru2)
                        newval += 6200;  // |_XXX__
                    else if (ru2 == player)
                        newval += 7000;  // |_XXX_X
                    else 
                        newval += 5500;  // |_XXX_O
                    break;
                }  // |_XXX_?
                else if (ru2_x >= SIZE || ru2_y >= SIZE) {

                    int ld2 = board[ld2_x][ld2_y];
                    if (!ld2) 
                        newval += 6200;  // __XXX_|
                    else if (ld2 == player)
                        newval += 7000;  // X_XXX_|
                    else 
                        newval += 5500;  // O_XXX_|
                    break;
                }  // ?_XXX_|

                int ld2 = board[ld2_x][ld2_y], ru2 = board[ru2_x][ru2_y];
                if (ld2 == player && ru2 == player) {
                    newval += 20000;
                    break;
                }  // X_XXX_X
                else if (ld2 == enemy && ru2 == enemy) {
                    newval += 5500;
                    break;
                }  // O_XXX_O
 
                int sum2 = ld2 + ru2;
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
            else if (ld1 == enemy && !ru1) {
                if (ru2_x >= SIZE || ru2_y >= SIZE)
                    break;
                  // OXXX_|

                int ru2 = board[ru2_x][ru2_y];
                if (!ru2)
                    newval += 5500;  // OXXX__
                else if (ru2 == player)
                    newval += 7000;  // OXXX_X
                break;

            }  // OXXX_?
            else if (ru1 == enemy && !ld1) {
                if (ld2_x < 0 || ld2_y < 0)
                    break;
                  // |_XXXO

                int ld2 = board[ld2_x][ld2_y];
                if (!ld2)
                    newval += 5500;  // __XXXO
                else if (ld2 == player)
                    newval += 7000;  // X_XXXO
                break;

            }  // ?_XXXO
           
            break;
        }

        case 1: {
            if (ld1_x < 0 || ld1_y < 0) {
                if (ru1_x >= SIZE || ru1_y >= SIZE)
                    break;
                  // |XX|

                if (!board[ru1_x][ru1_y]) {
                    if (ru2_x >= SIZE || ru2_y >= SIZE)
                        break;
                      // |XX_|

                    if (!board[ru2_x][ru2_y]) {
                        if (ru3_x >= SIZE || ru3_y >= SIZE)
                            break;
                          // |XX__|

                        if (!board[ru3_x][ru3_y]) {
                            newval += 2000;
                            break;
                        }  // |XX___
                        else if (board[ru3_x][ru3_y] == player) {
                            newval += 5500;
                            break;
                        }  // |XX__X

                    }  // |XX__?
                    else if (board[ru2_x][ru2_y] == player) {
                        if (ru3_x >= SIZE || ru3_y >= SIZE)
                            break;
                          // |XX_X|

                        if (!board[ru3_x][ru3_y]) {
                            newval += 5500;
                            break;
                        }  // |XX_X_

                    } 

                }
                break;
            }  // |XX_??
            else if (ru1_x >= SIZE || ru1_y >= SIZE) {

                if (!board[ld1_x][ld1_y]) {
                    if (ru2_x >= SIZE || ru2_y >= SIZE)
                        break;
                      // |_XX|

                    if (!board[ld2_x][ld2_y]) {
                        if (ru3_x >= SIZE || ru3_y >= SIZE)
                            break;
                          // |__XX|

                        if (!board[ld3_x][ld3_y]) {
                            newval += 2000;
                            break;
                        }  // ___XX|
                        else if (board[ld3_x][ld3_y] == player) {
                            newval += 5500;
                            break;
                        }  // X__XX|

                    }
                    else if (board[ld2_x][ld2_y] == player) {
                        if (ld3_x < 0 || ld3_y < 0)
                            break;
                          // |X_XX|

                        if (!board[ld3_x][ld3_y]) {
                            newval += 5500;
                            break;
                        }  // _X_XX|
                    }

                }
                break;
            }
        
            int ld1 = board[ld1_x][ld1_y], ru1 = board[ru1_x][ru1_y];
            if (!ld1 && !ru1) {

                if (ld2_x < 0 || ld2_y < 0) {
                    if (ru2_x >= SIZE || ru2_y >= SIZE)
                        break;
                      // |_XX_|
                    
                    int ru2 = board[ru2_x][ru2_y], ru3 = board[ru3_x][ru3_y];
                    if (!ru2) {
                        if (ru3_x >= SIZE || ru3_y >= SIZE) {
                            newval += 2000;
                            break;
                        }  // |_XX__|

                        if (!ru3) 
                            newval += 3000;  // |_XX___
                        else if (ru3 == player)
                            newval += 5500;  // |_XX__X
                        else 
                            newval += 2000;  // |_XX__O
                        break;

                    }
                    else if (ru2 == player) {
                        if (ru3_x >= SIZE || ru3_y >= SIZE) {
                            newval += 5500;
                            break;
                        }  // |_XX_X|

                        if (!ru3)
                            newval += 6000;  // |_XX_X_
                        else if (ru3 == enemy)
                            newval += 5500;  // |_XX_XO
                        break;

                    }
                    
                    break;
                }  // |_XX_??
                else if (ru2_x >= SIZE || ru2_y >= SIZE) {
                    
                    int ld2 = board[ld2_x][ld2_y], ld3 = board[ld3_x][ld3_y];
                    if (!ld2) {
                        if (ld3_x < 0 || ld3_y < 0) {
                            newval += 2000;
                            break;
                        }  // |__XX_|
                        
                        if (!ld3)
                            newval += 3000;  // ___XX_|
                        else if (ld3 == player)
                            newval += 5500;  // X__XX_|
                        else 
                            newval += 2000;  // O__XX_|
                        break;

                    }
                    else if (ld2 == player) {
                        if (ru3_x >= SIZE || ru3_y >= SIZE) {
                            newval += 5500;
                            break;
                        }  // |X_XX_|

                        if (!ld3)
                            newval += 6000;  // _X_XX_|
                        else if (ld3 == enemy)
                            newval += 5500;  // OX_XX_|
                        break;

                    }
                    
                    break;
                }

                int ld2 = board[ld2_x][ld2_y], ru2 = board[ru2_x][ru2_y], sum2 = ld2 + ru2;
                if (!ld2 && !ru2) {
                    if (ld3_x < 0 || ld3_y < 0) {
                        if (ru3_x >= SIZE || ru3_y >= SIZE) {
                            newval += 3500;
                            break;
                        }  // |__XX__|

                        int ru3 = board[ru3_x][ru3_y];
                        if (!ru3)
                            newval += 4000;  // |__XX___
                        else if (ru3 == player)
                            newval += 5700;  // |__XX__X
                        else 
                            newval += 3500;  // |__XX__O
                        break;
                        // |__XX__?
                    }
                    else if (ru3_x >= SIZE || ru3_y >= SIZE) {

                        int ld3 = board[ld3_x][ld3_y];
                        if (!ld3)
                            newval += 4000;  // ___XX__|
                        else if (ld3 == player) 
                            newval += 5700;  // X__XX__|
                        else 
                            newval += 3500;  // O__XX__|
                        break;
                        // ?__XX__|
                    }

                    int ld3 = board[ld3_x][ld3_y], ru3 = board[ru3_x][ru3_y], sum3 = ld3 + ru3;
                    if (ld3 == enemy && ru3 == enemy) 
                        newval += 3500;  // O__XX__O
                    else if (!sum3)
                        newval += 5000;  // ___XX___
                    else if (sum3 == player)
                        newval += 6100;  // X__XX___  or  ___XX__X
                    else if (sum3 == enemy)
                        newval += 4000;  // O__XX___  or  ___XX__O
                    else if (sum3 == 3)
                        newval += 5700;  // O__XX__X  or  X__XX__O

                    break;
                }  // ?__XX__?
                else if (ld2 == player && ru2 == player) {
                    if (ld3_x < 0 || ld3_y < 0) {
                        newval += 6500;
                        break;
                    }  // |X_XX_X_
                    if (ru3_x >= SIZE || ru3_y >= SIZE) {
                        newval += 6500;
                        break;
                    }  // _X_XX_X|

                    if (board[ld3_x][ld3_y] + board[ru3_x][ru3_y] == enemy)
                        newval += 6500;  // OX_XX_X_  or  _X_XX_XO
                    break;
                }
                else if (sum2 == player) {
                    if (ld3_x < 0 || ld3_y < 0) {
                        if (ru3_x >= SIZE || ru3_y >= SIZE) {
                            newval += 5500;
                            break;
                        }  // |X_XX__|  or  |__XX_X|

                        int ld3 = board[ld3_x][ld3_y], ru3 = board[ru3_x][ru3_y];
                        if (ld2 + ld3 == player || ru2 + ru3 == player) {
                            newval += 6300;
                            break;
                        }  // |__XX_X_  or  _X_XX__|
                    }
                    int sum3 = board[ld3_x][ld3_y] + board[ru3_x][ru3_y];
                    if (sum3 == 2 * enemy) {
                        newval += 5500;
                        break;
                    }  // OX_XX__O  or  O__XX_XO
                    else if ((ld2 == player && board[ru3_x][ru3_y] == enemy && !board[ld3_x][ld3_y]) || (ru2 == player && board[ld3_x][ld3_y] && !board[ru3_x][ru3_y])) {
                        newval += 6300;
                        break;
                    }  // _X_XX__O  or  O__XX_X_

                    int ld3 = board[ld3_x][ld3_y];
                    if (!sum3)
                        newval += 6300;  // _X_XX___  or  ___XX_X_
                    else if (sum3 == player && ld3 + ld2 == player)
                        newval += 6400;  // _X_XX__X  or  X__XX_X_
                    else if (sum3 == enemy && ld2 + ld3 == 3)
                        newval += 5500;  // OX_XX___  or  ___XX_XO
                    else if (sum3 == enemy && ld2 + ld3 != 3)
                        newval += 6300;  // O__XX_X_  or  _X_XX__O
                    else if (sum3 == 3 && ld2 + ld3 == 3)
                        newval += 6500;  // OX_XX__X  or  X__XX_XO  
                    break;

                }
                else if (sum2 == enemy) {
                    int ld3 = board[ld3_x][ld3_y], sum3 = board[ld3_x][ld3_y] + board[ru3_x][ru3_y];
                    if (!sum3)
                        newval += 3000;  // _O_XX___  or  ___XX_O_
                    else if (sum3 == player && ld2 + ld3 != 3)
                        newval += 5500;  // _O_XX__X  or  X__XX_O_
                    else if (sum3 == enemy && ld2 + ld3 == enemy)
                        newval += 2000;  // _O_XX__O  or  O__XX_O_
                    break;

                }
                else if (sum2 == 3) {
                    int ld3 = board[ld3_x][ld3_y], sum3 = board[ld3_x][ld3_y] + board[ru3_x][ru3_y];
                    if (!sum3)
                        newval += 6000;  // _O_XX_X_  or  _X_XX_O_
                    else if (sum3 == enemy && ld2 + ld3 != player)
                        newval += 5500;  // _O_XX_XO  or  OX_XX_O_
                    break;

                }

                break;
            }  // ??_XX_|
            else if (ld1 == enemy && !ru1) {
                if (ru2_x >= SIZE || ru2_y >= SIZE)
                    break;
                  // OXX_|

                int sum2 = board[ru2_x][ru2_y] + board[ru3_x][ru3_y];
                if (sum2 == player)
                    newval += 5500;  // OXX_X_  or  OXX__X
                else if (!sum2)
                    newval += 2000;  // OXX___ 
                break;

            }  //OXX_??
            else if (ru1 == enemy && !ld1) {
                int sum2 = board[ld2_x][ld2_y] + board[ld3_x][ld3_y];
                if (sum2 == player)
                    newval += 5500;  // _X_XXO  or  X__XXO
                else if (!sum2)
                    newval += 2000;  // ___XXO
                break;
            }
        
            break;
        }
    }  
    //左下到右上的斜線

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

    switch(defnum[1][0] + defnum[1][2]) {
        case 8:
        case 7:
        case 6:
        case 5:
        case 4:
            newval += 200000;
            break;
        case 3:
            newval += 100000;
            break;
        case 2:
        case 1:
            newval += 500;
            break;
    }
    //水平線

    switch(defnum[0][1] + defnum[2][1]) {
        case 8:
        case 7:
        case 6:
        case 5:
        case 4:
            newval += 200000;
            break;
        case 3:
            newval += 100000;
            break;
        case 2:
        case 1:
            newval += 500;
            break;
    }
    //鉛直線

    switch(defnum[0][0] + defnum[2][2]) {
        case 8:
        case 7:
        case 6:
        case 5:
        case 4:
            newval += 200000;
            break;
        case 3:
            newval += 100000;
            break;
        case 2:
        case 1:
            newval += 500;
            break;
    }
    //左上到右下的斜線

    switch(defnum[0][2] + defnum[2][0]) {
        case 8:
        case 7:
        case 6:
        case 5:
        case 4:
            newval += 200000;
            break;
        case 3:
            newval += 100000;
            break;
        case 2:
        case 1:
            newval += 500;
            break;
    }
    //左下到右上的斜線

    return newval;
}
// 算完後放二維陣列，不然每次都要重算

class state {
    int x, y;
    int value;

    std::set<state *> child;

    state(int xx, int yy, int val = 0) : x(xx), y(yy), value(val) {}

    bool operator< (const state &a) const {
        if (x == a.x)
            return x > a.x;
        
        return y > a.y;
    }
};

void read_board(std::ifstream& fin) {
    fin >> player;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fin >> board[i][j];
        }
    }
}

void write_valid_spot(std::ofstream& fout) {
    
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
