# from btf_cantor import *
from btf_binius import *


# additive ntt class
# runs on top of a binary tower field (btf) class
# import the desired btf class at top of this file
# see chapter 3 of https://github.com/ingonyama-zk/papers/blob/main/ntt_201_book.pdf for more details
class antt():
    def __init__(self, N):
        self.N = N
        self.logN = math.log2(N)
        assert self.logN == int(self.logN)
        self.logN = int(self.logN)
        self.loglogN = math.ceil(math.log2(self.logN))  # nlvls
        self._init_spolys()

    def _init_spolys(self):
        # s_poly[:, 0] = coefficients, s_poly[:, 1] = exponents
        # init at x
        s_poly = np.atleast_2d(np.array([btf(1, nlvls=self.loglogN), 1]).astype(object))
        s_polys = [s_poly]
        for i in range(1, self.logN):
            # square previous
            tmp = self.poly_mult(s_poly, s_poly)
            # scalar mult previous
            scl = self.s_poly_eval(s_poly, btf(1 << (i - 1)))
            for j in range(s_poly.shape[0]):
                s_poly[j, 0] *= scl
            # add square and scalar mult
            s_poly = self.poly_add(s_poly, tmp)
            s_polys += [s_poly]
        self.s_polys = s_polys

    @staticmethod
    def poly_mult(a, b):
        c = np.atleast_2d(np.array([btf(0), 0]).astype(object))
        for i in range(a.shape[0]):
            for j in range(b.shape[0]):
                tmp = np.array([a[i, 0] * b[j, 0], a[i, 1] + b[j, 1]])
                idx = np.where(c[:, 1] == tmp[1])[0]
                if len(idx) == 0:
                    c = np.vstack((c, tmp))
                elif len(idx) == 1:
                    c[idx[0], 0] += tmp[0]
                else:
                    assert False
        c = c[1:, :]
        return c

    @staticmethod
    def poly_add(a, b):
        c = copy.copy(a)
        for j in range(b.shape[0]):
            tmp = b[j, :]
            idx = np.where(c[:, 1] == tmp[1])[0]
            if len(idx) == 0:
                c = np.vstack((c, tmp))
            elif len(idx) == 1:
                c[idx[0], 0] += tmp[0]
            else:
                assert False
        tmp = c[:, 0]
        return c[c[:, 0] != 0, :]

    def s_poly_eval(self, i, x):
        try:
            s_poly = self.s_polys[i]
        except (AttributeError, TypeError):
            s_poly = i
        x.extend_nlvls(self.loglogN)  # extend to prevent aliasing (modulo)
        tmp = btf(0)
        for s, e in s_poly:
            tmp += s * x ** e
        return tmp

    def fft(self, x, coset=btf(0)):
        # init
        N = len(x)
        assert N <= self.N
        logN = math.log2(N)
        assert logN == int(logN)
        logN = int(logN)
        if logN == 0:
            return x
        # split
        x_0 = x[:N // 2]
        x_1 = x[N // 2:]
        # twiddle
        s = self.s_poly_eval(logN - 1, coset)
        s *= self.s_poly_eval(logN - 1, btf(1 << (logN - 1))).inv()
        # butterflies
        q_0 = x_0 + s * x_1
        q_1 = q_0 + x_1
        # recurse
        X_0 = self.fft(q_0, coset)
        X_1 = self.fft(q_1, coset + btf(1 << (logN - 1)))
        return np.concatenate((X_0, X_1))

    def ifft(self, X, coset=btf(0)):
        # init
        N = len(X)
        assert N <= self.N
        logN = math.log2(N)
        assert logN == int(logN)
        logN = int(logN)
        if logN == 0:
            return X
        # split
        X_0 = X[:N // 2]
        X_1 = X[N // 2:]
        # recurse
        Q_0 = self.ifft(X_0, coset)
        Q_1 = self.ifft(X_1, coset + btf(1 << (logN - 1)))
        # twiddle
        s = self.s_poly_eval(logN - 1, coset)
        s *= self.s_poly_eval(logN - 1, btf(1 << (logN - 1))).inv()
        # butterflies
        x_1 = Q_0 + Q_1
        x_0 = Q_0 + s * x_1
        return np.concatenate((x_0, x_1))


def antt_matrix(logN = 3):
    N = 2 ** logN
    loglogN = int(math.ceil(math.log2(logN)))
    afft_ = antt(N)
    # x_poly[:, 0] = coefficients, x_poly[:, 1] = exponents
    # init at 1
    const = np.atleast_2d(np.array([btf(1, nlvls=loglogN), 0]).astype(object))
    x_polys = [const]
    for i in range(1, 2 ** logN):
        x_poly = const
        i_bin = bin(i)[2:].zfill(logN)[::-1]
        for j in range(logN):
            j_bin = i_bin[j]
            if j_bin == '1':
                x_poly = afft_.poly_mult(x_poly, afft_.s_polys[j])
        x_polys += [x_poly]
    #
    F = np.full((N, N), btf(0)).astype(object)
    for i in range(N):
        for j in range(N):
            F[i, j] = afft_.s_poly_eval(x_polys[j], btf(i, nlvls=loglogN))
    print(F)
    print()
    a = np.array([btf(np.random.randint(N)) for i in range(N)]).astype(object)
    print(F @ a)
    print(afft_.fft(a))
    print()
    print(np.matmul(F, F.T))


if __name__ == '__main__':
    # test matrix form
    antt_matrix()

    # random test
    logN = 8
    antt_ = antt(2 ** logN)
    a = np.array([btf(np.random.randint(2 ** logN)) for i in range(2 ** logN)]).astype(object)
    print(a)
    b = antt_.fft(a)
    print(b)
    c = antt_.ifft(b)
    print(c)
    assert np.array_equal(a, c)

    # print beta series
    logN = 16
    antt_ = antt(2 ** logN)
    for i in range(logN):
        print(f'{i},{antt_.s_poly_eval(i, btf(2 ** i))}')
