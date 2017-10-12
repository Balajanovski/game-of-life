#include "Drawer.h"
#include "Coords.h"

#include <string>
#include <stdexcept>

Drawer::Drawer(int field_width, int field_height)
        : SdlHandle(SDL_INIT_EVERYTHING), width(field_width), height(field_height) {
    window.reset(SDL_CreateWindow("Conway's Game of Life", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, field_width * CELL_SIZE, field_height * CELL_SIZE, SDL_WINDOW_SHOWN));
    if (!window) {
        throwSdlError("unable to create window");
    }

    renderer.reset(SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED));
    if (!renderer) {
        throwSdlError("unable to create renderer");
    }
}

void Drawer::flush() {
    // Draw white background
    SDL_SetRenderDrawColor(renderer.get(), 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer.get());

    // Draw the cells from the buffer
    SDL_Rect draw_dst;
    draw_dst.w = draw_dst.h = CELL_SIZE;
    SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, SDL_ALPHA_OPAQUE);
    while (!empty()) {
        Coords coord = front();
        pop();

        draw_dst.x = coord.x * CELL_SIZE;
        draw_dst.y = coord.y * CELL_SIZE;

        SDL_RenderFillRect(renderer.get(), &draw_dst);
    }

    // Draw grid
    SDL_SetRenderDrawColor(renderer.get(), 125, 125, 125, SDL_ALPHA_OPAQUE);
    draw_dst.x = draw_dst.y = 0;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            draw_dst.x = x * CELL_SIZE;
            draw_dst.y = y * CELL_SIZE;

            SDL_RenderDrawRect(renderer.get(), &draw_dst);
        }
    }

    SDL_RenderPresent(renderer.get());

}
