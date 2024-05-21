#define AP_INT_MAX_W 4096
#include <ap_int.h>
#include <hls_vector.h>
#include <iostream>
#include <iomanip>

#define PAD_HEX(digits) std::setfill('0') << std::setw(digits) << std::hex

template <int ROWS, int COLS, int S>
struct state_matrix_t {
private:
    ap_uint<S> value[ROWS][COLS] = {0};
public:
    static constexpr bool ROW_ORDER = false;
    static constexpr bool COL_ORDER = true;

    state_matrix_t() = default;

    state_matrix_t(const state_matrix_t &other) {
#pragma HLS INLINE
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                this->value[i][j] = other.value[i][j];
            }
        }
    }

    state_matrix_t(const ap_uint<S> other[ROWS][COLS]) {
#pragma HLS INLINE
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                this->value[i][j] = other[i][j];
            }
        }
    }

    state_matrix_t(ap_uint<ROWS*COLS*S> raw, bool order) {
#pragma HLS INLINE
        int len = ROWS * COLS * S;
        if (order == ROW_ORDER) {
            for (int i = 0; i < ROWS; i++) {
                for (int j = 0; j < COLS; j++) {
                    this->value[i][j] = raw((len - 1) - (i * S * COLS + j * S), (len - S) - (i * S * COLS + j * S));
                }
            }
        } else {
            for (int i = 0; i < COLS; i++) {
                for (int j = 0; j < ROWS; j++) {
                    this->value[j][i] = raw((len - 1) - (i * S * ROWS + j * S), (len - S) - (i * S * ROWS + j * S));
                }
            }
        }
    }

    ap_uint<ROWS*COLS*S> to_uint(bool order) {
#pragma HLS INLINE
        int len = ROWS * COLS * S;
        ap_uint<ROWS * COLS * S> tmp;
        if (order == ROW_ORDER) {
            for (int i = 0; i < ROWS; i++) {
                for (int j = 0; j < COLS; j++) {
                    tmp((len - 1) - (i * S * COLS + j * S), (len - S) - (i * S * COLS + j * S)) = this->value[i][j];
                }
            }
        } else {
            for (int i = 0; i < COLS; i++) {
                for (int j = 0; j < ROWS; j++) {
                    tmp((len - 1) - (i * S * ROWS + j * S), (len - S) - (i * S * ROWS + j * S)) = this->value[j][i];
                }
            }
        }
        return tmp;
    }

    state_matrix_t<COLS, ROWS, S> transpose() {
#pragma HLS INLINE
        state_matrix_t<COLS, ROWS, S> tmp;
        for (int i = 0; i < COLS; i++) {
            for (int j = 0; j < ROWS; j++) {
                tmp[i][j] = this->value[j][i];
            }
        }
        return tmp;
    }

    void print() {
        for (int i = 0; i < ROWS; i++) {
            if (i == 0) {
                std::cout << "[[";
            } else {
                std::cout << " [";
            }
            for (int j = 0; j < COLS; j++) {
                std::cout << " " << std::setw(S/4 + 2) << std::hex << this->value[i][j];
            }
            if (i == ROWS-1) {
                std::cout << " ]]" << std::endl;
            } else {
                std::cout << " ]" << std::endl;
            }
        }
    }

    ap_uint<S> (&operator[](int i))[COLS] {
#pragma HLS INLINE
        return value[i];
    }

    hls::vector<ap_uint<S>, ROWS> get_col(int j) {
#pragma HLS INLINE
        hls::vector<ap_uint<S>, ROWS> tmp;
        for (int i = 0; i < ROWS; i++) {
            tmp[i] = this->value[i][j];
        }
        return tmp;
    }

    state_matrix_t operator^(state_matrix_t other) {
#pragma HLS INLINE
        state_matrix_t tmp;
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                tmp[i][j] = this->value[i][j] ^ other[i][j];
            }
        }
        return tmp;
    }

    state_matrix_t operator+(state_matrix_t other) {
#pragma HLS INLINE
        state_matrix_t tmp;
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                tmp[i][j] = this->value[i][j] + other[i][j];
            }
        }
        return tmp;
    }

    state_matrix_t rotr_row(int row, int n) {
#pragma HLS INLINE
        state_matrix_t tmp = *this;
        for (int i = 0; i < COLS; i++) {
            tmp[row][i] = this->value[row][(i + n) % COLS];
        }
        return tmp;
    }

    state_matrix_t rotl_row(int row, int n) {
#pragma HLS INLINE
        state_matrix_t tmp = *this;
        for (int i = 0; i < COLS; i++) {
            tmp[row][(i + n) % COLS] = this->value[row][i];
        }
        return tmp;
    }

    state_matrix_t rotr_rows(int n[ROWS]) {
#pragma HLS INLINE
        state_matrix_t tmp;
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                tmp[i][(j + n[i]) % COLS] = this->value[i][j];
            }
        }
        return tmp;
    }

    state_matrix_t rotl_rows(int n[ROWS]) {
#pragma HLS INLINE
        state_matrix_t tmp;
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                tmp[i][j] = this->value[i][(j + n[i]) % COLS];
            }
        }
        return tmp;
    }

    state_matrix_t rotd_cols(int n[COLS]) {
#pragma HLS INLINE
        state_matrix_t tmp;
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                tmp[(i + n[j]) % ROWS][j] = this->value[i][j];
            }
        }
        return tmp;
    }

    state_matrix_t rotu_cols(int n[COLS]) {
#pragma HLS INLINE
        state_matrix_t tmp;
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                tmp[i][j] = this->value[(i + n[j]) % ROWS][j];
            }
        }
        return tmp;
    }
};