#include "Drawer.h"
#include "CellManager.h"
#include "Coords.h"

#include <memory>
#include <SDL2/SDL.h>

constexpr static int MS_PER_UPDATE = 40;
constexpr static int HEIGHT = 50;
constexpr static int WIDTH = 100;

enum GameState {
    PAUSED = 0,
    RUNNING = 1,
    STOPPED = 2,
};

void check_keyboard_events(GameState& state) {
    SDL_Event ev;

    while(SDL_PollEvent(&ev)) {
        if (ev.type == SDL_KEYDOWN) {
            switch(ev.key.keysym.sym) {
                case SDLK_ESCAPE: state = STOPPED; break;
                case SDLK_SPACE:
                {
                    switch(state) {
                        case PAUSED:
                            state = RUNNING;
                            break;
                        case RUNNING:
                            state = PAUSED;
                            break;
                    }
                }
                break;
            }
        }
        else if (ev.type == SDL_QUIT)
            state = STOPPED;

    }

}

void check_mouse_events(CellManager& manager) {
    SDL_PumpEvents();
    int x, y;
    auto mouse_event = SDL_GetMouseState(&x, &y);

    if (mouse_event & SDL_BUTTON_LMASK) {
        x = x / CELL_SIZE;
        y = y / CELL_SIZE;

        manager.make_cell(x, y);
    }

    else if (mouse_event & SDL_BUTTON_RMASK) {
        x = x / CELL_SIZE;
        y = y / CELL_SIZE;

        manager.remove_cell(x, y);
    }
}

int main() {
    GameState game_state = PAUSED;


    std::shared_ptr<Drawer> drawer = std::make_shared<Drawer>(WIDTH, HEIGHT);
    CellManager manager(WIDTH, HEIGHT, drawer);

    // Game loop
    int last_ticks = 0,
               lag = 0;
    while (game_state != STOPPED) {
        check_keyboard_events(game_state);
        check_mouse_events(manager);

        int current_ticks = SDL_GetTicks();
        int elapsed = current_ticks - last_ticks;
        last_ticks = current_ticks;
        lag += elapsed;

        while (lag >= MS_PER_UPDATE) {
            if (game_state == RUNNING) {
                manager.apply_rules();
            }
            lag -= MS_PER_UPDATE;
        }

        manager.iterate();
        drawer->flush();
    }

    return 0;
}