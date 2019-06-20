#ifndef GAME_HPP
#define GAME_HPP

#include <SDL2/SDL.h>
#include <string>

class Game
{
    public:
        Game();
        ~Game();

        void init(std::string title, int screenw, int screenh);
        void clean();

        bool running = false;

        SDL_Renderer *renderer;

    private:
        SDL_Window *window;

};

#endif // GAME_HPP
