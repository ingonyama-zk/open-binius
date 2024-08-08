import math
import copy
import numpy as np


class btf():
    def __init__(self, val, nlvls=None):
        if nlvls is None:
            try:
                nlvls = int(math.log2(math.log2(val))) + 1
            except ValueError:
                nlvls = 1
        self._set_nlvls(nlvls)
        self._val = int(val) % self._p
        assert self._val >= 0

    def _set_nlvls(self, nlvls):
        assert nlvls > 0
        self.nlvls = nlvls
        self.nbits = 2 ** self.nlvls
        self.nhalfbits = 2 ** (self.nlvls - 1)
        self._p = 2 ** self.nbits

    def __str__(self):
        return f'{self._val}'

    __repr__ = __str__

    def zero(self):
        self._val = 0

    def one(self):
        self._val = 1

    def extend_nlvls(self, nlvls):
        assert self.nlvls <= nlvls
        self._set_nlvls(nlvls)

    def val(self):
        return self._val

    def bin(self):
        return bin(self._val)[2:].zfill(self.nbits)

    def split(self):
        hi = btf(int(self.bin()[:self.nhalfbits], 2), nlvls=self.nlvls - 1)
        lo = btf(int(self.bin()[self.nhalfbits:], 2), nlvls=self.nlvls - 1)
        return hi, lo

    def __eq__(self, other):
        if isinstance(other, int):
            return self._val == other
        elif isinstance(other, btf):
            return self._val == other.val()
        else:
            return False

    def __add__(self, other):
        # align nlvls to max
        this = copy.copy(self)
        if this.nlvls > other.nlvls:
            other.extend_nlvls(this.nlvls)
        if other.nlvls > this.nlvls:
            this.extend_nlvls(other.nlvls)
        this._val = this._val ^ other.val()
        return this

    __sub__ = __add__

    def __neg__(self):
        return copy.copy(self)

    def __mul__(self, other):
        # support ndarray commutativity for scalar-vector product
        if isinstance(other, np.ndarray):
            return other * self
        # align nlvls to max - a little suboptimal but all padded zeros are terminated by 0/1 optimizations
        this = copy.copy(self)
        if this.nlvls > other.nlvls:
            other.extend_nlvls(this.nlvls)
        if other.nlvls > this.nlvls:
            this.extend_nlvls(other.nlvls)
        # optimizations in case of 0 or 1
        if self._val == 0 or other.val() == 1:
            return this
        if self._val == 1 or other.val() == 0:
            return copy.copy(other)
        # else
        if max(this.nlvls, other.nlvls) > 1:
            # 4b mult or higher
            a_hi, a_lo = this.split()
            b_hi, b_lo = other.split()
            a_sum = a_hi + a_lo
            b_sum = b_hi + b_lo

            tmp = self._mul_abstract(a_hi, a_lo, a_sum, b_hi, b_lo, b_sum)
        else:
            # 2b mult
            # mod: x_0^2 = x_0 + 1
            # (a_hi * x_0 + a_lo)(b_hi * x_0 + b_lo) = (a_hi * b_hi + a_hi * b_lo + a_lo * b_hi) * x_0 +
            #                                          (a_hi * b_hi + a_lo * b_lo)
            a_hi = int(this.bin()[0], 2)
            a_lo = int(this.bin()[1], 2)
            b_hi = int(other.bin()[0], 2)
            b_lo = int(other.bin()[1], 2)
            a_sum = a_hi ^ a_lo
            b_sum = b_hi ^ b_lo
            lo = a_lo * b_lo
            hi = a_sum * b_sum ^ lo
            lo = a_hi * b_hi ^ lo
            tmp = btf(2 * hi + lo, nlvls=1)
        return tmp

    @staticmethod
    def _mul_abstract(a_hi, a_lo, a_sum, b_hi, b_lo, b_sum):
        # mod: x_i^2 = x_i * x_{i-1} + 1
        # (a_hi * x_i + a_lo)(b_hi * x_i + b_lo) = (a_hi * b_hi + a_lo * b_lo) +
        #                                          [ (a_hi * b_lo + a_lo * b_hi) + (a_hi * b_hi) * x_{i-1} ] * x_i
        # define: c === (a_hi * b_hi) <-- this is the main reason for the 0/1 optimization
        # => (a_hi * b_hi) * x_{i-1} = c * x_{i-1}
        #                            = (c_hi * x_{i-1} + c_lo) * x_{i-1}
        #                            = c_hi + [ c_lo + c_hi * x_{i-2} ] * x_{i-1}
        mx = a_hi * b_hi
        lo = a_lo * b_lo
        lo += mx
        mx = mx * btf(1 << mx.nhalfbits, nlvls=mx.nlvls)
        hi = a_sum * b_sum
        hi += (lo + mx)
        # concatenate
        tmp = btf((hi.val() << hi.nbits) + lo.val(), nlvls=hi.nlvls + 1)
        return tmp

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
    nlvls = 2
    for i in range(1, 2 ** (2 ** nlvls)):
        a = btf(i, nlvls=nlvls)
        for j in range(1, 2 ** (2 ** nlvls)):
            b = btf(j, nlvls=nlvls)
            c = b * a
            print(f'{a} * {b} = {c}')
