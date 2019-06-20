#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

#include <stdlib.h> // srand rand

#include "Game.hpp"

#define LOG_COMMENT(msg) std::cout << msg << "\n";

const int SCREEN_WIDTH = 320;
const int SCREEN_HEIGHT = 640;
const int TILE_SIZE = SCREEN_WIDTH/10;

Game game;
SDL_Event event;

enum direction {RIGHT, LEFT, UP, DOWN};
enum block_type {IBLOCK, OBLOCK, LBLOCK, INVLBLOCK, TBLOCK, NBLOCK, INVNBLOCK};
enum color {CYAN=1, RED, GREEN, PURPLE, ORANGE, BLUE, WHITE};

int tiles[SCREEN_HEIGHT/TILE_SIZE][SCREEN_WIDTH/TILE_SIZE] = {0};

void printTiles(){
    std::cout << "---------------------\n";
    for(int i = 0; i < SCREEN_HEIGHT/TILE_SIZE; i++){
         std::cout << i << "-";
        for(int j = 0; j < SCREEN_WIDTH/TILE_SIZE; j++){
            std::cout << tiles[i][j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "---------------------\n";
}

int sum_arr(int arrlen, int arr[]){
    int sum = 0;
    for(int i = 0; i < arrlen; i++){
        sum += arr[i];
    }
    return sum;
}
                //arr1 in arr
void arr_cpy(int arr[], int arr1[], int arrlen){
    for(int i = 0; i < arrlen; i++){
        arr[i] = arr1[i];
    }
}

bool allBiggerZero(int arr[]){
    for(int i = 0; i < SCREEN_WIDTH/TILE_SIZE; i++){
        if(arr[i] == 0){
            return false;
        }
    }
    return true;
}

void updateTiles(){
    int n = 0;
    for(int i = 0; i < SCREEN_HEIGHT/TILE_SIZE; i++){
        n = sum_arr(SCREEN_WIDTH/TILE_SIZE, tiles[i]);
        if(allBiggerZero(tiles[i])){
            for(int j = 0; j<SCREEN_WIDTH/TILE_SIZE; j++){
                tiles[i][j] = 0;
            }
        }
    }

    for(int i = SCREEN_HEIGHT/TILE_SIZE; i > 0; i--){
        n = sum_arr(SCREEN_WIDTH/TILE_SIZE, tiles[i]);
        if(n == 0){
            int zero_arr[SCREEN_WIDTH/TILE_SIZE] = {0};
            arr_cpy(tiles[i], tiles[i-1], SCREEN_WIDTH/TILE_SIZE);
            arr_cpy(tiles[i-1], zero_arr, SCREEN_WIDTH/TILE_SIZE);
        }
    }
}

struct Tile
{
    int x=0,y=0;
};

class Square
{
public:
    Square(){set_tile(0,0);}
    Square(int x, int y){set_tile(x,y);}
    Square(int x, int y, color c){set_tile(x,y);set_color(c);}
    SDL_Rect rect = {0,0,TILE_SIZE,TILE_SIZE};

    void set_color(color c){
        color_ = c;
    }

    color get_color(){
        return color_;
    }

    void set_tile(int x, int y){
        if(x != tile.x || y != tile.y){
            prevTile = tile;
            tile.x = x;
            tile.y = y;
            rect.x = x*TILE_SIZE;
            rect.y = y*TILE_SIZE;

            //tiles[y][x] = 1;
            //tiles[prevTile.y][prevTile.x] = 0;
        }
    }

    Tile get_Tile(){
        return tile;
    }

    Tile get_prevTile(){
        return prevTile;
    }

    void print_tile(){
        std::cout << "actual: x: " << tile.x << " y: " << tile.y << "\n";
        std::cout << "prev: x: " << prevTile.x << " y: " << prevTile.y << "\n";
    }

    void draw(){
        if(color_ == CYAN){
            SDL_SetRenderDrawColor( game.renderer, 0, 255, 255, 0);
        } else if(color_ == RED){
            SDL_SetRenderDrawColor( game.renderer, 255, 0, 0, 0);
        } else if(color_ == ORANGE){
            SDL_SetRenderDrawColor( game.renderer, 255, 140, 0, 0);
        } else if(color_ == GREEN){
            SDL_SetRenderDrawColor( game.renderer, 0, 255, 0, 0);
        } else if(color_ == BLUE){
            SDL_SetRenderDrawColor( game.renderer, 0, 0, 255, 0);
        } else if(color_ == PURPLE){
            SDL_SetRenderDrawColor( game.renderer, 130, 0, 125, 0);
        } else {
            SDL_SetRenderDrawColor( game.renderer, 255,255,255,255);
        }
        SDL_RenderFillRect(game.renderer, &rect);
    }


private:
    Tile tile, prevTile;
    color color_;
};

class Block
{
public:
    Block(block_type blockt){
        switch(blockt){
            case IBLOCK:
                //Iblock
                squares[0].set_tile(4,3);
                squares[1].set_tile(4,2);
                squares[2].set_tile(4,1);
                squares[3].set_tile(4,0);
                for(int i = 0; i<4; i++){
                    squares[i].set_color(RED);
                }
            break;

            case OBLOCK:
                //Oblock
                squares[0].set_tile(4,0);
                squares[1].set_tile(4,1);
                squares[2].set_tile(5,0);
                squares[3].set_tile(5,1);
                for(int i = 0; i<4; i++){
                    squares[i].set_color(BLUE);
                }
            break;

            case LBLOCK:
                // Lblock
                squares[0].set_tile(4,0);
                squares[1].set_tile(4,1);
                squares[2].set_tile(4,2);
                squares[3].set_tile(5,2);
                for(int i = 0; i<4; i++){
                    squares[i].set_color(PURPLE);
                }
            break;

            case INVLBLOCK:
                // INVLblock
                squares[0].set_tile(4,0);
                squares[1].set_tile(4,1);
                squares[2].set_tile(4,2);
                squares[3].set_tile(3,2);
                for(int i = 0; i<4; i++){
                    squares[i].set_color(WHITE);
                }
            break;

            case TBLOCK:
                // Tblock
                squares[0].set_tile(5,0);
                squares[1].set_tile(4,1);
                squares[2].set_tile(5,1);
                squares[3].set_tile(6,1);
                for(int i = 0; i<4; i++){
                    squares[i].set_color(ORANGE);
                }
            break;

            case NBLOCK:
                //Nblock
                squares[0].set_tile(3,0);
                squares[1].set_tile(4,0);
                squares[2].set_tile(4,1);
                squares[3].set_tile(5,1);
                for(int i = 0; i<4; i++){
                    squares[i].set_color(CYAN);
                }
            break;

            case INVNBLOCK:
                //INVNblock
                squares[0].set_tile(4,0);
                squares[1].set_tile(5,0);
                squares[2].set_tile(3,1);
                squares[3].set_tile(4,1);
                for(int i = 0; i<4; i++){
                    squares[i].set_color(GREEN);
                }
            break;
        }
        pos = 0;
        type = blockt;
    };
    Square squares[4];
    int pos;
    block_type type;

    bool fall(){
        for(int i = 0; i < 4; i++){
            if(tiles[squares[i].get_Tile().y+1][squares[i].get_Tile().x] >= 1){
                return true;
            }

        }

        for(int i = 0; i < 4; i++){
                if(squares[i].get_Tile().y+1 >= SCREEN_HEIGHT/TILE_SIZE){
                    return true;
                }

        }
        for(int i = 0; i < 4; i++){
            squares[i].set_tile(squares[i].get_Tile().x,squares[i].get_Tile().y+1);
        }

        return false;
    }

    void mov(direction d){
        if(d == RIGHT){
            for(int i = 0; i < 4; i++){
                if(squares[i].get_Tile().x+1 >= SCREEN_WIDTH/TILE_SIZE || tiles[squares[i].get_Tile().y][squares[i].get_Tile().x+1] >= 1){
                    return;
                }
            }
            for(int i = 0; i < 4; i++){
                squares[i].set_tile(squares[i].get_Tile().x+1,squares[i].get_Tile().y);
            }
        }
        if(d == LEFT){
            for(int i = 0; i < 4; i++){
                if(squares[i].get_Tile().x-1 < 0 || tiles[squares[i].get_Tile().y][squares[i].get_Tile().x-1] >= 1){
                    return;
                }
            }

            for(int i = 0; i < 4; i++){
                squares[i].set_tile(squares[i].get_Tile().x-1,squares[i].get_Tile().y);
            }
        }
    }

    void rotate(){
        if(type == IBLOCK){
            if(pos == 0){
                squares[0].set_tile(squares[2].get_Tile().x-1, squares[2].get_Tile().y);
                squares[1].set_tile(squares[2].get_Tile().x+1, squares[2].get_Tile().y);
                squares[3].set_tile(squares[2].get_Tile().x+2, squares[2].get_Tile().y);
                pos = 1;
            }else if(pos == 1){
                squares[0].set_tile(squares[2].get_Tile().x, squares[2].get_Tile().y-1);
                squares[1].set_tile(squares[2].get_Tile().x, squares[2].get_Tile().y+1);
                squares[3].set_tile(squares[2].get_Tile().x, squares[2].get_Tile().y+2);
                pos = 0;
            }
        }

        if(type == LBLOCK){
            if(pos == 0){
                squares[0].set_tile(squares[1].get_Tile().x-1, squares[1].get_Tile().y);
                squares[2].set_tile(squares[1].get_Tile().x+1, squares[1].get_Tile().y);
                squares[3].set_tile(squares[0].get_Tile().x, squares[1].get_Tile().y+1);
                pos = 1;
            } else if(pos == 1){
                squares[0].set_tile(squares[1].get_Tile().x, squares[1].get_Tile().y-1);
                squares[2].set_tile(squares[1].get_Tile().x, squares[1].get_Tile().y+1);
                squares[3].set_tile(squares[2].get_Tile().x-1, squares[0].get_Tile().y);
                pos = 2;
            } else if(pos == 2){
                squares[0].set_tile(squares[1].get_Tile().x+1, squares[1].get_Tile().y);
                squares[2].set_tile(squares[1].get_Tile().x-1, squares[1].get_Tile().y);
                squares[3].set_tile(squares[0].get_Tile().x, squares[1].get_Tile().y-1);
                pos = 3;
            } else if(pos == 3){
                squares[0].set_tile(squares[1].get_Tile().x, squares[1].get_Tile().y-1);
                squares[2].set_tile(squares[1].get_Tile().x, squares[1].get_Tile().y+1);
                squares[3].set_tile(squares[2].get_Tile().x+1, squares[2].get_Tile().y);
                pos = 0;
            }
        }

        if(type == INVLBLOCK){
            if(pos == 0){
                squares[0].set_tile(squares[1].get_Tile().x-1, squares[1].get_Tile().y);
                squares[2].set_tile(squares[1].get_Tile().x+1, squares[1].get_Tile().y);
                squares[3].set_tile(squares[0].get_Tile().x, squares[1].get_Tile().y-1);
                pos = 1;
            }else if(pos == 1){
                squares[0].set_tile(squares[1].get_Tile().x, squares[1].get_Tile().y-1);
                squares[2].set_tile(squares[1].get_Tile().x, squares[1].get_Tile().y+1);
                squares[3].set_tile(squares[0].get_Tile().x+1, squares[0].get_Tile().y);
                pos = 2;
            } else if(pos == 2){
                squares[0].set_tile(squares[1].get_Tile().x+1, squares[1].get_Tile().y);
                squares[2].set_tile(squares[1].get_Tile().x-1, squares[1].get_Tile().y);
                squares[3].set_tile(squares[0].get_Tile().x, squares[0].get_Tile().y+1);
                pos = 3;
            } else if(pos == 3){
                squares[0].set_tile(squares[1].get_Tile().x, squares[1].get_Tile().y-1);
                squares[2].set_tile(squares[1].get_Tile().x, squares[1].get_Tile().y+1);
                squares[3].set_tile(squares[2].get_Tile().x-1, squares[2].get_Tile().y);
                pos = 0;
            }
        }

        if(type == TBLOCK){
            if(pos == 0){
                squares[1].set_tile(squares[2].get_Tile().x, squares[2].get_Tile().y+1);
                pos = 1;
            }else if(pos == 1){
                squares[0].set_tile(squares[2].get_Tile().x-1, squares[2].get_Tile().y);
                pos = 2;
            } else if(pos == 2){
                squares[3].set_tile(squares[2].get_Tile().x, squares[2].get_Tile().y-1);
                pos = 3;
            } else if(pos == 3){
                squares[0].set_tile(squares[2].get_Tile().x, squares[2].get_Tile().y-1);
                squares[1].set_tile(squares[2].get_Tile().x-1, squares[2].get_Tile().y);
                squares[3].set_tile(squares[2].get_Tile().x+1, squares[2].get_Tile().y);
                pos = 0;
            }
        }

        if(type == NBLOCK){
            if(pos == 0){
                squares[0].set_tile(squares[1].get_Tile().x+1, squares[1].get_Tile().y);
                squares[3].set_tile(squares[0].get_Tile().x, squares[0].get_Tile().y-1);

                pos = 1;
            }else if(pos == 1){
                squares[0].set_tile(squares[1].get_Tile().x-1, squares[1].get_Tile().y);
                squares[3].set_tile(squares[2].get_Tile().x+1, squares[2].get_Tile().y);
                pos = 0;
            }

        }

        if(type == INVNBLOCK){
            if(pos == 0){
                squares[2].set_tile(squares[0].get_Tile().x-1, squares[0].get_Tile().y);
                squares[1].set_tile(squares[2].get_Tile().x, squares[2].get_Tile().y-1);

                pos = 1;
            }else if(pos == 1){
                squares[2].set_tile(squares[0].get_Tile().x+1, squares[0].get_Tile().y);
                squares[1].set_tile(squares[3].get_Tile().x-1, squares[3].get_Tile().y);
                pos = 0;
            }

        }
    }

    void draw(){
        for(int i = 0; i < 4; i++){
            squares[i].draw();
        }
    }

};


int main(int argc, char* args[])
{
    const int FPS = 10;
	const int frameDelay = 1000 / FPS;
    srand (SDL_GetTicks());

	Uint32 frameStart;
    int frameTime;

    game.init("Tetris", SCREEN_WIDTH, SCREEN_HEIGHT);

    Block bk = Block(LBLOCK);
    std::vector<Square> squares;

    int frames = 0;
    bool pushed = false;


    while(game.running){
        SDL_RenderClear(game.renderer);

        frameStart = SDL_GetTicks();
        SDL_PollEvent(&event);

        switch (event.type)
        {
            case SDL_QUIT :
                game.running = false;
                break;
            case SDL_KEYDOWN:

                if(!pushed){
                pushed = true;

                switch(event.key.keysym.sym){
                    case SDLK_RIGHT:
                        bk.mov(RIGHT);
                    break;
                    case SDLK_LEFT:
                        bk.mov(LEFT);
                    break;
                    case SDLK_UP:
                        bk.rotate();
                    break;
                    case SDLK_DOWN:

                    break;
                    case SDLK_f:

                    case SDLK_t:
                        printTiles();
                    break;

                    case SDLK_c:
                        for(int i = 0; i < SCREEN_HEIGHT/TILE_SIZE; i++){
                            for(int j = 0; j < SCREEN_WIDTH/TILE_SIZE; j++){
                               tiles[i][j] = 0;
                            }
                        }
                    break;
                }
                }
            break;

            case SDL_KEYUP:
                if(pushed){pushed = false;}
                //LOG_COMMENT(pushed);
                switch(event.key.keysym.sym){
                    case SDLK_RIGHT:

                    break;
                }
                break;

            case SDL_MOUSEBUTTONDOWN:

                std::cout << "(" << event.button.x/TILE_SIZE << ", " << event.button.y/TILE_SIZE << ")" <<"\n";
            break;


        }

        SDL_SetRenderDrawColor( game.renderer, 0x00, 0x00, 0x00, 0x00);

        bk.draw();


//        if(frames == 30 || frames == 15){
        if(frames % 9 == 0){
            if(bk.fall()){
                for(int i = 0; i<4; i++){
                    tiles[bk.squares[i].get_Tile().y][bk.squares[i].get_Tile().x] = (int)bk.squares[i].get_color();
                }
                bk = Block((block_type)(rand()%7));
            }
        }

        for(int i = 0; i < SCREEN_HEIGHT/TILE_SIZE; i++){
            for(int j = 0; j < SCREEN_WIDTH/TILE_SIZE; j++){
                if(tiles[i][j] >= 1){
                    squares.push_back(Square(j,i,(color)tiles[i][j]));
                }
            }
        }

        for(unsigned int i = 0; i<squares.size(); i++){
            squares[i].draw();
        }

        updateTiles();

        squares.clear();

        frames++;
        if(frames > 60){
            frames = 0;
        }


        SDL_SetRenderDrawColor( game.renderer, 0x00, 0x00, 0x00, 0x00);



        frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime){
			//SDL_Delay(frameDelay - frameTime);
        }

        SDL_RenderPresent(game.renderer);
    }

    game.clean();

    return 0;
}
