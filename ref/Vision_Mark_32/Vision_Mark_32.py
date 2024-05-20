#this code is not tested yet
#run this on a jupyter notebook with sage installed


from sage.all import *

# The binmul function below is adapted from work by Vitalik Buterin

def binmul(v1, v2, length=None):
    if v1 < 256 and v2 < 256 and rawmulcache[v1][v2] is not None:
        return rawmulcache[v1][v2]
    if v1 < 2 or v2 < 2:
        return v1 * v2
    if length is None:
        length = 1 << (max(v1, v2).bit_length() - 1).bit_length()
    halflen = length // 2
    quarterlen = length // 4
    halfmask = (1 << halflen) - 1

    L1, R1 = v1 & halfmask, v1 >> halflen
    L2, R2 = v2 & halfmask, v2 >> halflen

    if (L1, R1) == (0, 1):
        outR = binmul(1 << quarterlen, R2, halflen) ^ L2
        return R2 ^ (outR << halflen)

    L1L2 = binmul(L1, L2, halflen)
    R1R2 = binmul(R1, R2, halflen)
    R1R2_high = binmul(1 << quarterlen, R1R2, halflen)
    Z3 = binmul(L1 ^ R1, L2 ^ R2, halflen)
    return (
        L1L2 ^
        R1R2 ^
        ((Z3 ^ L1L2 ^ R1R2 ^ R1R2_high) << halflen)
    )

class BinaryFieldElement:
    def __init__(self, value):
        if isinstance(value, BinaryFieldElement):
            value = value.value
        self.value = value

    def __add__(self, other):
        othervalue = other if isinstance(other, int) else other.value
        if self.value < 256 and othervalue < 256:
            return addcache[self.value][othervalue]
        return BinaryFieldElement(self.value ^ othervalue)

    __sub__ = __add__

    def __neg__(self):
        return self

    def __mul__(self, other):
        othervalue = other if isinstance(other, int) else other.value
        if self.value < 256 and othervalue < 256:
            return mulcache[self.value][othervalue]
        return BinaryFieldElement(binmul(self.value, othervalue))

    def __pow__(self, other):
        if other == 0:
            return BinaryFieldElement(1)
        elif other == 1:
            return self
        elif other == 2:
            return self * self
        else:
            return self.__pow__(other % 2) * self.__pow__(other // 2) ** 2

    def inv(self):
        L = 1 << (self.value.bit_length() - 1).bit_length()
        return self ** (2 ** L - 2)

    def __truediv__(self, other):
        if isinstance(other, int):
            other = BinaryFieldElement(other)
        return BinaryFieldElement(binmul(self.value, other.inv().value))

    def __eq__(self, other):
        othervalue = other if isinstance(other, int) else other.value
        return self.value == othervalue

    def __repr__(self):
        return '<' + str(self.value) + '>'

    def bit_length(self):
        return 1 << (self.value.bit_length() - 1).bit_length()

    def to_bytes(self, length, byteorder):
        assert length >= (self.bit_length() + 7) // 8
        return self.value.to_bytes(length, byteorder)

    @classmethod
    def from_bytes(cls, b, byteorder):
        return cls(int.from_bytes(b, byteorder))

rawmulcache = [[None for _ in range(256)] for _ in range(256)]
addcache = [[None for _ in range(256)] for _ in range(256)]
mulcache = [[None for _ in range(256)] for _ in range(256)]

for i in range(256):
    for j in range(256):
        rawmulcache[i][j] = binmul(i, j)
        addcache[i][j] = BinaryFieldElement(i ^ j)
        mulcache[i][j] = BinaryFieldElement(binmul(i, j))

# Define the binary field and its extension
n = 32
F = GF(2 ** n, 'a')

# m refers to the state size or another parameter mentioned for matrix dimensions
m = 24

# Initialize U using the method described in Code Listing 2
U = [[F.fetch_int(2**j) for j in range(ceil(log(m, 2)) + 1)]]
for i in range(1, ceil(log(m, 2))):
    U.append([U[i - 1][j] * (U[i - 1][j] + U[i - 1][i - 1]) for j in range(ceil(log(m, 2)) + 1)])

for i in range(ceil(log(m, 2))):
    normalization_constant = U[i][i].inverse()
    U[i] = [u * normalization_constant for u in U[i]]

# Expand U horizontally to create W using Code Listing 3
W = []
for i in range(ceil(log(m, 2))):
    W_i = [F(0)]
    for j in range(ceil(log(m, 2)) + 1):
        W_i += [W_i[k] + U[i][j] for k in range(1 << j)]
    W.append(W_i[:2 * m])

# Expand W vertically to create X using Code Listing 4
X = []
for j in range(2 * m):
    X_j = [F(1)]
    for i in range(ceil(log(m, 2))):
        X_j += [X_j[k] * W[i][j] for k in range(1 << i)]
    X.append(X_j[:m])

# Convert to a matrix for further use in cryptographic operations
M = matrix(F, X[:m])

# Define parameters for Vision Mark-32
rounds = 8  # Number of rounds
state_size = 24  # Size of the state array

# Initialize the state with random elements from F
state = vector(F, [F.random_element() for _ in range(state_size)])

# Define round constants, initializing 16 constants for 8 rounds
Cr = [vector(F, [F.random_element() for _ in range(state_size)]) for _ in range(2 * rounds)]

# Define coefficients for the linearized affine transformation B(x) and its inverse B^{-1}(x)
coefficients_B = [F.random_element() for _ in range(32)]
coefficients_B_inv = [F.random_element() for _ in range(32)]
beta_n = F.random_element()

def linearized_affine(x, coefficients):
    result = beta_n
    for k, beta_k in enumerate(coefficients):
        result += beta_k * x ** (2 ** k)
    return result

def apply_inverse(state):
    def invert_element(alpha):
        coeffs = alpha.polynomial().coefficients()
        if len(coeffs) == 0:  # Handle zero element
            return alpha
        a = coeffs[0]
        b = coeffs[1] if len(coeffs) > 1 else F(0)
        x3 = F.random_element() ** 3
        delta = a * (a + b * x3) + b * b
        delta_inv = delta.inverse()
        alpha_inv = b * delta_inv + delta_inv * (a + b * x3)
        return alpha_inv
    return vector(F, [invert_element(x) for x in state])

def apply_linear(state, inverse=False):
    coefficients = coefficients_B_inv if inverse else coefficients_B
    return vector(F, [linearized_affine(x, coefficients) for x in state])

def vision_mark_32_round(state, r):
    state = apply_inverse(state)
    state = apply_linear(state, inverse=True)
    state = M * state + Cr[2 * r]
    state = apply_inverse(state)
    state = apply_linear(state)
    state = M * state + Cr[2 * r + 1]
    return state

# Vision Mark-32 permutation
def vision_mark_32_permute(state):
    for r in range(rounds):
        state = vision_mark_32_round(state, r)
    return state

# Sponge construction functions
def vision_mark_32_sponge(message, digest_length=8):
    rate = 16
    capacity = 8
    state_size = rate + capacity

    # Initialize state
    state = vector(F, [F(0) for _ in range(rate)] + [F(0) for _ in range(capacity)])
    message_length = int(len(message)).to_bytes(8, byteorder='little')
    state[rate] = F(int.from_bytes(message_length[:4], byteorder='little'))
    state[rate+1] = F(int.from_bytes(message_length[4:], byteorder='little'))
    state[rate+2:] = [F(0) for _ in range(capacity-2)]

    # Padding message
    padded_message = message + [0] * ((rate - len(message) % rate) % rate)

    # Absorb message
    for i in range(0, len(padded_message), rate):
        block = padded_message[i:i+rate]
        state[:rate] = [state[j] + F(block[j]) for j in range(rate)]
        state = vision_mark_32_permute(state)

    # Squeeze digest
    digest = []
    while len(digest) < digest_length:
        digest.extend(state[:rate])
        state = vision_mark_32_permute(state)
    return digest[:digest_length]

# Format digest elements as hexadecimal strings
def format_digest(digest):
    def element_to_hex(elem):
        bits = ''.join(str(int(coef)) for coef in elem.polynomial().coefficients(sparse=False))
        return format(int(bits, 2), 'x')
    return [element_to_hex(elem) for elem in digest]

# Example usage
message = [1, 2, 3, 4, 5]  # Example message
digest = vision_mark_32_sponge(message)
formatted_digest = format_digest(digest)

# Print the formatted digest
for i, elem in enumerate(formatted_digest):
    print(f"Digest element {i}: {elem}")
