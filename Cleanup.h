#ifndef LIFE_CLEANUP_H_H
#define LIFE_CLEANUP_H_H

#include "NoCopy.h"

#include <SDL2/SDL.h>
#include <stdexcept>
#include <memory>
#include <sstream>

struct SdlHandle : NoCopy {
    SdlHandle(Uint32 flags) {
        if(SDL_Init(flags))
            throw std::runtime_error("Unable to initialize SDL");
    }

    ~SdlHandle() { SDL_Quit(); }
};

template<typename... Args>
void cleanup(Args *... args){
    using ex = int[];
    (void)ex{(cleanup(args), 0)..., 0};
}
/*
 * These specializations serve to free the passed argument and also provide the
 * base cases for the recursive call above, eg. when args is only a single item
 * one of the specializations below will be called by
 * cleanup(std::forward<Args>(args)...), ending the recursion
 * We also make it safe to pass nullptrs to handle situations where we
 * don't want to bother finding out which values failed to load (and thus are null)
 * but rather just want to clean everything up and let cleanup sort it out
 */
inline void cleanup(SDL_Window *win){
    if (!win){
        return;
    }
    SDL_DestroyWindow(win);
}

inline void cleanup(SDL_Renderer *ren){
    if (!ren){
        return;
    }
    SDL_DestroyRenderer(ren);
}

inline void cleanup(SDL_Texture *tex){
    if (!tex){
        return;
    }
    SDL_DestroyTexture(tex);
}

inline void cleanup(SDL_Surface *surf){
    if (!surf){
        return;
    }
    SDL_FreeSurface(surf);
}

inline void cleanup(SDL_GLContext *context) {
    if(!context)
        return;

    SDL_GL_DeleteContext(context);
}

struct Cleaner {
    template <class T>
    void operator() (T *p) const {
        cleanup(p);
    }
};

using SdlWinPtr = std::unique_ptr<SDL_Window, Cleaner>;
using SdlRenPtr = std::unique_ptr<SDL_Renderer, Cleaner>;
using SdlTexPtr = std::unique_ptr<SDL_Texture, Cleaner>;
using SdlSurfPtr = std::unique_ptr<SDL_Surface, Cleaner>;

[[noreturn]] inline void throwSdlError(const std::string &msg) {
    std::stringstream ss;
    ss << msg << " error: " << SDL_GetError();
    throw std::runtime_error(ss.str());
}

#endif //LIFE_CLEANUP_H_H
