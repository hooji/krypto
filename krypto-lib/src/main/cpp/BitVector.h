//
//  BitVector.h
//  krypto
//
//  Created by Matthew Tamayo on 12/11/14.
//  Copyright (c) 2014 Kryptnostic. All rights reserved.
//

#ifndef krypto_BitVector_h
#define krypto_BitVector_h
#include <iostream>

static FILE * urandom = std::fopen("/dev/urandom", "rb" );
//N is number of 64 bit longs in the bitvector
template<unsigned int N>
class BitVector {
public:
	BitVector() {
        zero();
	}
    
    BitVector( const BitVector & v ) {
        for (unsigned int i = 0; i < N; ++i) {
            _bits[i] = v._bits[i];
        }
    }
    
    BitVector( BitVector && v ) {
        for (unsigned int i = 0; i < N; ++i) {
            _bits[i] = v._bits[i];
        }
    }

    
	unsigned long long * elements() {
		return _bits;
	}
    
	int length() {
		return N << 6;
	}

	bool get(unsigned int n) const {
		return (_bits[n >> 6] & (1ul << (n & 63ul))) != 0;
	}

	BitVector & set(unsigned int n) {
		_bits[n >> 6] |= (1ul << (n & 63ul));
		return *this;
	}

	void clear(unsigned int n) {
		_bits[n >> 6ul] &= ~(1ul << (n & 63ul));
	}
    
	BitVector<N> operator&(const BitVector<N> & rhs) {
		BitVector<N> result;
		for (unsigned int i = 0; i < N; ++i) {
			result._bits[i] = _bits[i] & rhs._bits[i];
		}
		return result;
	}

	BitVector<N> operator|(const BitVector<N> & rhs) {
		BitVector<N> result;
		for (unsigned int i = 0; i < N; ++i) {
			result._bits[i] = _bits[i] | rhs._bits[i];
		}
		return result;
	}

	BitVector<N> operator^(const BitVector<N> & rhs) {
		BitVector<N> result;
		for (unsigned int i = 0; i < N; ++i) {
			result._bits[i] = _bits[i] ^ rhs._bits[i];
		}
		return result;
	}

    const BitVector<N> & operator=(const BitVector<N> & rhs) {
        for (unsigned int i = 0; i < N; ++i) {
            _bits[i] = rhs._bits[i];
        }
        return *this;
    }
    
	BitVector<N> & operator^=(const BitVector<N> & rhs) {
		for (unsigned int i = 0; i < N; ++i) {
			_bits[i] ^= rhs._bits[i];
		}
		return *this;
	}

    BitVector<N> & operator&=(const BitVector<N> & rhs) {
        for (unsigned int i = 0; i < N; ++i) {
            _bits[i] &= rhs._bits[i];
        }
        return *this;
    }

	const bool isZero() const {
		for (unsigned int i = 0; i < N; ++i) {
			if (_bits[i] != 0) {
				return false;
			}
		}
		return true;
	}
	const bool parity() const {
		unsigned long long accumulator;
		for (unsigned int i = 0; i < N; ++i) {
			accumulator ^= _bits[i];
		}
		return __builtin_parityll(accumulator);
	}

	void zero() {
		for (unsigned int i = 0; i < N; ++i) {
			_bits[i] &= 0;
		}
	}

	template<unsigned int M>
	const bool operator==(const BitVector<M> & rhs) const {
		if (N != M) {
			return false;
		}
		for (unsigned int i = 0; i < N; ++i) {
			if (_bits[i] != rhs._bits[i]) {
				return false;
			}
		}
		return true;
	}
    
    template<unsigned int M>
    const bool operator!=(const BitVector<M> & rhs) {
        if (N != M) {
            return true;
        }
        for (unsigned int i = 0; i < N; ++i) {
            if (_bits[i] != rhs._bits[i]) {
                return true;
            }
        }
        return false;
    }

	void print() const {
		const unsigned int numBits = (N << 6) - 1;
		std::cout << "[ ";
		for (int i = 0; i < numBits; ++i) {
			std::cout << get(i) << ", ";
		}
		std::cout << get(numBits) << "]" << std::endl;
	}

    static const BitVector<N> & zeroVector() {
        static BitVector<N> v;
        return v;
    }
    
    //Generates a random vector, shoudl be improved to not just multiply two random longs together.
    static const BitVector<N> randomVector() {
        BitVector<N> result;
        
        while( result.isZero() ) {
            std::fread(&result._bits, sizeof( unsigned long long ), N, urandom );
        }
        return result;
    }
private:
	unsigned long long _bits[N];
};


#endif
