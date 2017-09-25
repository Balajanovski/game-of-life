#ifndef LIFE_NOCOPY_H
#define LIFE_NOCOPY_H

struct NoCopy {
    NoCopy() = default;
    NoCopy(NoCopy const &) = delete;
    NoCopy &operator = (NoCopy const &) = delete;
};

#endif //LIFE_NOCOPY_H
