#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

#include <stdlib.h> // srand rand
#include <time.h>

#include "Game.hpp"

#define LOG_COMMENT(msg) std::cout << msg << "\n";

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int TILE_SIZE = 32;

Game game;
SDL_Event event;

enum direction {RIGHT, LEFT, UP, DOWN};

int tiles[15][20] = {0};

struct Tile
{
    int x=0,y=0;
};

class Square
{
public:
    Square(){set_tile(0,0);}
    Square(int x, int y){set_tile(x,y);}
    SDL_Rect rect = {0,0,TILE_SIZE,TILE_SIZE};

    void set_tile(int x, int y){
        if(x != tile.x || y != tile.y){
            prevTile = tile;
            tile.x = x;
            tile.y = y;
            rect.x = x*TILE_SIZE;
            rect.y = y*TILE_SIZE;
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
        SDL_SetRenderDrawColor( game.renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderFillRect(game.renderer, &rect);
    }

private:
    Tile tile, prevTile;
};

class Snake
{
public:
    Square sq;
    std::vector<Square> sqrs = {};
    Tile v;
    direction dir;

    void move_(){
        if(sq.get_Tile().x+v.x > 19){
            sq.set_tile(0, sq.get_Tile().y+v.y);
        }else if(sq.get_Tile().x+v.x < 0){
            sq.set_tile(19, sq.get_Tile().y+v.y);
        }else if(sq.get_Tile().y+v.y < 0){
            sq.set_tile(sq.get_Tile().x+v.x, 14);
        }else if(sq.get_Tile().y+v.y > 14){
            sq.set_tile(sq.get_Tile().x+v.x, 0);
        }else{
            sq.set_tile(sq.get_Tile().x+v.x, sq.get_Tile().y+v.y);
        }

        if(sqrs.size()>0){
            sqrs[0].set_tile(sq.get_prevTile().x, sq.get_prevTile().y);
        }

        for(unsigned int i=1; i < sqrs.size(); i++){
            sqrs[i].set_tile(sqrs[i-1].get_prevTile().x, sqrs[i-1].get_prevTile().y);
        }
    }

    bool checkCol(){
        if(sqrs.size()>1){
            for(unsigned int i=0; i < sqrs.size(); i++){
                if(sqrs[i].get_Tile().x == sq.get_Tile().x && sqrs[i].get_Tile().y == sq.get_Tile().y){
                    return true;
                }
            }
        }

        return false;
    }

    void reset(){
        sqrs.clear();
    }

    void draw(){
        sq.draw();
        if(sqrs.size()>0){
            for(unsigned int i=0; i < sqrs.size(); i++){
                sqrs[i].draw();
            }
        }
    }
};

class Food
{
public:
    Square sq;

    void changePos(){
        sq.set_tile(rand() % 20, rand() % 15);
    }

    void draw(){
        sq.draw();
    }
};

int main(int argc, char* args[])
{
    const int FPS = 15;
	const int frameDelay = 1000 / FPS;
    srand (time(NULL));

	Uint32 frameStart;
    int frameTime;

    game.init("Snake", SCREEN_WIDTH, SCREEN_HEIGHT);

    Snake sn;
    sn.v.x = 1;
    Food fd;

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
                switch(event.key.keysym.sym){
                    case SDLK_RIGHT:
                        if(sn.dir != LEFT){
                            sn.v.x = 1;
                            sn.v.y = 0;
                            sn.dir = RIGHT;
                        }

                    break;
                    case SDLK_LEFT:
                        if(sn.dir != RIGHT){
                            sn.v.x = -1;
                            sn.v.y = 0;
                            sn.dir = LEFT;
                        }

                    break;
                    case SDLK_UP:
                        if(sn.dir != DOWN){
                            sn.v.x = 0;
                            sn.v.y = -1;
                            sn.dir = UP;
                        }

                    break;
                    case SDLK_DOWN:
                        if(sn.dir != UP){
                            sn.v.x = 0;
                            sn.v.y = 1;
                            sn.dir = DOWN;
                        }
                    break;
                    case SDLK_f:
                        fd.changePos();
                    break;
                }

            case SDL_KEYUP:
                switch(event.key.keysym.sym){
                    case SDLK_RIGHT:


                    break;
                }
                break;
        }

        if(sn.sq.get_Tile().x == fd.sq.get_Tile().x && sn.sq.get_Tile().y == fd.sq.get_Tile().y){
            fd.changePos();
            Square s = Square(sn.sq.get_prevTile().x, sn.sq.get_prevTile().y);
            sn.sqrs.push_back(s);
        }
        sn.move_();

        if(sn.checkCol()){
            sn.reset();
        }

        sn.draw();
        fd.draw();

        SDL_SetRenderDrawColor( game.renderer, 0xff, 0xff, 0xff, 0xff);


        frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
        }

        SDL_RenderPresent(game.renderer);
    }

    game.clean();

    return 0;
}
