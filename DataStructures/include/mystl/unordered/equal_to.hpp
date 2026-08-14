#pragma once

template <typename T = void>
struct equal_to {

    bool operator()(const T& l, const T& r) {
        return l == r;
    }

};
