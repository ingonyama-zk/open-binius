import math
import copy
import numpy as np
import galois
import random


# binary galois field class
# note that there is no verification that poly is irreducible
# note that due to multiplier byte level optimization, this implementation:
# 1. is limited to ring polynomials that fit in a byte, i.e. x^nbits + b7*x^7 + b6*x^6 + ... + b1*x + b0
# 2. is limited to nbits > 15 since _mul_table is 15 bits wide
# this limitation can be lifted but would require more lu memory
class bgf():
    def __init__(self, val, nbits=128, poly=0b10000111):
        self._set_nbits(nbits)
        self._set_poly(poly)
        self._val = int(val) % self._p
        assert self._val >= 0

    def _set_nbits(self, nbits):
        assert nbits > 15
        self.nbits = nbits
        self._p = 2 ** self.nbits

    def _set_poly(self, poly):
        assert 0 < poly < 256
        self.poly = poly
        self._mul_table = [0]
        for i in range(1, 256):
            ii = bin(i)[2:]
            tmp = 0
            for idx, b in enumerate(ii[::-1]):
                tmp ^= int(b) * poly << idx
            self._mul_table += [tmp]

    def _bits(self, x):
        return [int(b) for b in bin(x)[2:].zfill(self.nbits)]

    @staticmethod
    def _int(bits):
        return int("".join(str(b) for b in bits), 2)
    
    def _conv(self, x, y):
        # convert x and y to binary lists
        x_bits = self._bits(x)
        y_bits = self._bits(y)
        # convolve mod 2
        return self._int(np.convolve(x_bits, y_bits, mode='full') % 2)

    def __str__(self):
        return f'{self._val}'

    __repr__ = __str__

    def zero(self):
        self._val = 0

    def one(self):
        self._val = 1

    def val(self):
        return self._val

    def bin(self):
        return bin(self._val)[2:].zfill(self.nbits)

    def __eq__(self, other):
        if isinstance(other, int):
            return self._val == other
        elif isinstance(other, bgf):
            return self._val == other.val()
        else:
            return False

    def __add__(self, other):
        # assert same type
        if isinstance(other, int):
            other = bgf(other, self.nbits, self.poly)
        assert self.nbits == other.nbits and self.poly == other.poly
        this = copy.copy(self)
        this._val = this._val ^ other.val()
        return this

    __sub__ = __add__

    def __neg__(self):
        return copy.copy(self)

    def __mul__(self, other):
        # support ndarray commutativity for scalar-vector product
        if isinstance(other, np.ndarray):
            return other * self
        # assert same type
        if isinstance(other, int):
            other = bgf(other, self.nbits, self.poly)
        assert self.nbits == other.nbits and self.poly == other.poly
        this = copy.copy(self)
        # optimizations in case of 0 or 1
        if self._val == 0 or other.val() == 1:
            return this
        if self._val == 1 or other.val() == 0:
            return copy.copy(other)
        # else
        tmp = self._conv(this._val, other.val())
        # reduce
        hi, tmp = tmp // self._p, tmp % self._p
        n = self.nbits // 8
        hi_bytes = list(hi.to_bytes(n, 'little'))
        for i in reversed(range(n)):
            tmp ^= self._mul_table[hi_bytes[i]] << (8 * i)
        # final reduce
        hi, tmp = tmp // self._p, tmp % self._p
        tmp ^= self._mul_table[hi]
        return bgf(tmp, self.nbits, self.poly)

    def __pow__(self, power):
        tmp = copy.copy(self)
        # special case (not modulo)
        if power == 0:
            tmp.one()
            return tmp
        # standard cases
        if tmp == 0 or tmp == 1:
            return tmp
        power %= (self._p - 1)
        if power == 0:
            tmp.one()
            return tmp
        elif power == 1:
            return tmp
        elif power == 2:
            return tmp * tmp
        else:
            return tmp.__pow__(power % 2) * tmp.__pow__(power // 2) ** 2

    def inv(self):
        tmp = copy.copy(self)
        return tmp ** (self._p - 2)


if __name__ == '__main__':
    GF = galois.GF(2 ** 128, irreducible_poly="x^128 + x^7 + x^2 + x + 1")
    for _ in range(1000):
        a_ = random.randint(0, 2**128)
        b_ = random.randint(0, 2**128)
        tst = bgf(a_) * bgf(b_)
        ref = GF(a_) * GF(b_)
        assert tst.val() == ref

