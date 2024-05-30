#pragma once

#include <ap_int.h>

class vision_mark_32 {
public:
    struct state_t {
        ap_uint<32> v[24];

        state_t(ap_uint<768> x) {
            for (int i = 0; i < 24; i++) {
                v[i] = x;
                x >>= 32;
            }
        }

        ap_uint<768> to_packed() {
            ap_uint<768> x = 0;
            for (int i = 0; i < 24; i++) {
                x |= v[23 - i];
                x <<= 32;
            }
            return x;
        }
    };

    static state_t update(state_t state);
};
