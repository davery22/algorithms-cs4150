#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    long x;
    long y;
    long d;
} ee_pack;

static inline long mod(long x, long N) {
    return (x < 0) ? N - (-x % N) : (x % N);
}

// Prints x^y mod N.
void mod_exp(long x, long y, long N) {
    long i, z = 1;
    for (i = 0x80000000; i > 0; i >>= 1) {
        z = (i & y) ? mod(x*mod(z*z, N), N) : mod(z*z, N);
    }
    printf("%ld\n", z);
}

// Returns x^y mod N.
long mod_exp2(long x, long y, long N) {
    long i, z = 1;
    for (i = 0x80000000; i > 0; i >>= 1) {
        z = (i & y) ? mod(x*mod(z*z, N), N) : mod(z*z, N);
    }
    return z;
}

// Returns [x, y, d] such that d = gcd(a, b) and ax + by = d (Extended Euclid's).
ee_pack ee(long a, long b) {
    if (!b) {
        ee_pack *pack = malloc(sizeof(ee_pack));
        pack->x = 1; pack->y = 0; pack->d = a;
        return *pack;
    }
    else {
        ee_pack pack = ee(b, mod(a, b));
        long tmp = pack.y;
        pack.y = pack.x - (a/b)*pack.y;
        pack.x = tmp;
        return pack;
    }
}

// Prints the inverse of a (mod N). Uses Extended Euclid's.
void inverse(long a, long N) {
    ee_pack pack = ee(a, N);
    if (pack.d == 1) {
        printf("%ld\n", mod(pack.x, N));
    }
    else {
        printf("none\n");
    }
}

// Returns the inverse of a (mod N). Uses Extended Euclid's.
long inverse2(long a, long N) {
    ee_pack pack = ee(a, N);
    return mod(pack.x, N);
}

// Prints the GCD of a and b (Euclid's algorithm).
void gcd(long a, long b) {
    while (b > 0) {
        long amb = mod(a, b);
        a = b;
        b = amb;
    }
    printf("%ld\n", a);
}

// Returns the GCD of a and b (Euclid's algorithm).
long gcd2(long a, long b) {
    while (b > 0) {
        long amb = mod(a, b);
        a = b;
        b = amb;
    }
    return a;
}

// Prints "yes" if p passes the Fermat test for a = 2,3,5; else prints "no".
void is_prime(long p) {
    if (mod_exp2(2, p-1, p) == 1 && mod_exp2(3, p-1, p) == 1 && mod_exp2(5, p-1, p) == 1) {
        printf("yes\n");
    }
    else {
        printf("no\n");
    }
}

// Prints the modulus, public exponent, and private exponent of the RSA key pair derived from p and q.
void rsa_key(long p, long q) {
    long N = p*q;
    long phi = (p-1)*(q-1);
    int i = 0;
    static int primes[] = {3,5,7,9,11,13,17,19,23,27,29,31};
    long e;
    for (e = primes[i]; gcd2(e, phi) != 1; e = primes[++i]) {}
    long d = inverse2(e, phi);
    printf("%ld %ld %ld\n", N, e, d);
}

long parse_num() {
    long n = 0;
    char r;
    while ((r = getc_unlocked(stdin)) >= '0') {
        n = n*10 + r - 48;
    }
    return n;
}

int main() {
    char *buf = malloc(9);

    while (fscanf(stdin, "%s ", buf) >= 0) {
        //printf("%s\n", buf);
        if (!strcmp(buf, "gcd")) {
            long a = parse_num();
            long b = parse_num();
            gcd(a, b);
        }
        else if (!strcmp(buf, "exp")) {
            long x = parse_num();
            long y = parse_num();
            long N = parse_num();
            mod_exp(x, y, N);
        }
        else if (!strcmp(buf, "inverse")) {
            long a = parse_num();
            long N = parse_num();
            inverse(a, N);
        }
        else if (!strcmp(buf, "isprime")) {
            long p = parse_num();
            is_prime(p);
        }
        else if (!strcmp(buf, "key")) {
            long p = parse_num();
            long q = parse_num();
            rsa_key(p, q);
        }
    }

    return 0;
}
