/* LibTomCrypt, modular cryptographic library -- Tom St Denis
 *
 * LibTomCrypt is a library that provides various cryptographic
 * algorithms in a highly modular and flexible manner.
 *
 * The library is free for all purposes without any express
 * guarantee it works.
 */
#include "tomcrypt.h"

/**
   @file dsa_make_key.c
   DSA implementation, generate a DSA key
*/

#ifdef LTC_MDSA

/**
  Create a DSA key
  @param prng          An active PRNG state
  @param wprng         The index of the PRNG desired
  @param key           [in/out] Where to store the created key
  @return CRYPT_OK if successful.
*/
int dsa_make_key_ex(prng_state *prng, int wprng, dsa_key *key)
{
  int err, qbits;

  LTC_ARGCHK(key         != NULL);
  LTC_ARGCHK(key->x      != NULL);
  LTC_ARGCHK(key->y      != NULL);
  LTC_ARGCHK(key->p      != NULL);
  LTC_ARGCHK(key->g      != NULL);
  LTC_ARGCHK(key->q      != NULL);
  LTC_ARGCHK(ltc_mp.name != NULL);

  /* so now we have our DH structure, generator g, order q, modulus p
     Now we need a random exponent [mod q] and it's power g^x mod p
   */
  qbits = mp_count_bits(key->q);
  do {
     if ((err = rand_bn_bits(key->x, qbits, prng, wprng)) != CRYPT_OK)                  { return err; }
     /* private key x should be from range: 1 <= x <= q-1 (see FIPS 186-4 B.1.2) */
  } while (mp_cmp_d(key->x, 0) != LTC_MP_GT || mp_cmp(key->x, key->q) != LTC_MP_LT);
  if ((err = mp_exptmod(key->g, key->x, key->p, key->y)) != CRYPT_OK)                   { return err; }
  key->type = PK_PRIVATE;

  return CRYPT_OK;
}

/**
  Old-style creation of a DSA key
  @param prng          An active PRNG state
  @param wprng         The index of the PRNG desired
  @param group_size    Size of the multiplicative group (octets)
  @param modulus_size  Size of the modulus (octets)
  @param key           [out] Where to store the created key
  @return CRYPT_OK if successful.
*/
int dsa_make_key(prng_state *prng, int wprng, int group_size, int modulus_size, dsa_key *key)
{
  int err;

  if ((err = dsa_generate_pqg(prng, wprng, group_size, modulus_size, key)) != CRYPT_OK) { return err; }
  if ((err = dsa_make_key_ex(prng, wprng, key)) != CRYPT_OK) { return err; }

  return CRYPT_OK;
}

#endif

/* ref:         $Format:%D$ */
/* git commit:  $Format:%H$ */
/* commit time: $Format:%ai$ */
