#pragma once

#include <ap_int.h>

class vision_mark_32 {
public:
    struct state_t {
        ap_uint<32> v[24];
    };

    static state_t permute(state_t state);
};
