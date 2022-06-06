#! /bin/bash

SCRIPT_PATH="`dirname \"$0\"`"
SCRIPT_PATH="`( cd \"$SCRIPT_PATH\" && pwd )`"

REPOSITORY=https://github.com/ARMmbed/mbedtls.git
REPOSITORY_NAME="`echo ${REPOSITORY} | rev | cut -d'/' -f1 | rev | cut -d'.' -f1`"
#version 2.18.0, tag 2.18.1 :
VERSION=ca933c7e0c9e84738b168b6b0feb89af4183a60a

VERSION_2_24_0=523f0554b6cdc7ace5d360885c3f5bbcc73ec0e8

git clone  ${REPOSITORY}
echo ${REPOSITORY_NAME}
echo ${SCRIPT_PATH}/${REPOSITORY_NAME}
cd ${SCRIPT_PATH}/${REPOSITORY_NAME}
git checkout ${VERSION_2_24_0}


echo "Configuring Mbed TLS to suppress unused functionnalities, with switch"
cd ..

./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_HAVE_ASM
#./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_CIPHER_MODE_CBC
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_CIPHER_MODE_CFB
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_CIPHER_MODE_CTR
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_CIPHER_MODE_OFB
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_CIPHER_MODE_XTS
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_ECP_DP_SECP192R1_ENABLED
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_ECP_DP_SECP224R1_ENABLED
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_ECP_DP_SECP256R1_ENABLED
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_ECP_DP_SECP384R1_ENABLED
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_ECP_DP_SECP521R1_ENABLED
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_ECP_DP_SECP192K1_ENABLED
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_ECP_DP_SECP224K1_ENABLED
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_ECP_DP_SECP256K1_ENABLED
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_ECP_DP_BP256R1_ENABLED
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_ECP_DP_BP384R1_ENABLED
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_ECP_DP_BP512R1_ENABLED
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_ECP_DP_CURVE25519_ENABLED
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_ECP_DP_CURVE448_ENABLED
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_ECP_NIST_OPTIM
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_ECDH_LEGACY_CONTEXT
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_ECDSA_DETERMINISTIC
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_KEY_EXCHANGE_DHE_PSK_ENABLED
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_KEY_EXCHANGE_ECDHE_PSK_ENABLED
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_KEY_EXCHANGE_DHE_RSA_ENABLED
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_KEY_EXCHANGE_ECDHE_RSA_ENABLED
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_KEY_EXCHANGE_ECDHE_ECDSA_ENABLED
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_KEY_EXCHANGE_ECDH_ECDSA_ENABLED
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_KEY_EXCHANGE_ECDH_RSA_ENABLED
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_PK_PARSE_EC_EXTENDED
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  set MBEDTLS_ENTROPY_FORCE_SHA256
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_PK_RSA_ALT_SUPPORT
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_SELF_TEST
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_SSL_ALL_ALERT_MESSAGES
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_SSL_ENCRYPT_THEN_MAC
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_SSL_EXTENDED_MASTER_SECRET
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_SSL_FALLBACK_SCSV
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_SSL_KEEP_PEER_CERTIFICATE
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_SSL_CBC_RECORD_SPLITTING
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_SSL_PROTO_TLS1
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_SSL_PROTO_TLS1_1
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  set MBEDTLS_SSL_PROTO_DTLS
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  set MBEDTLS_SSL_DTLS_ANTI_REPLAY
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  set MBEDTLS_SSL_DTLS_HELLO_VERIFY
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  set MBEDTLS_SSL_DTLS_CLIENT_PORT_REUSE
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  set MBEDTLS_SSL_DTLS_BADMAC_LIMIT
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_SSL_SESSION_TICKETS
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_SSL_EXPORT_KEYS
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_SSL_SERVER_NAME_INDICATION
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_SSL_TRUNCATED_HMAC
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  set MBEDTLS_THREADING_PTHREAD
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_X509_CHECK_KEY_USAGE
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_X509_CHECK_EXTENDED_KEY_USAGE
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_X509_RSASSA_PSS_SUPPORT
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_AESNI_C
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_ARC4_C
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_BLOWFISH_C
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_CAMELLIA_C
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_CCM_C
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_CHACHA20_C
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_CHACHAPOLY_C
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_DEBUG_C
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_DES_C
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_DHM_C
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_ECDH_C
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_ECDSA_C
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_ECP_C
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_GCM_C
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_HKDF_C
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_HMAC_DRBG_C
#./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_MD5_C
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_PADLOCK_C
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_PKCS5_C
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_POLY1305_C
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_PSA_CRYPTO_C
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_PSA_CRYPTO_STORAGE_C
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_RIPEMD160_C
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_SHA1_C
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_SHA512_C
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  set MBEDTLS_SSL_COOKIE_C
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_SSL_TICKET_C
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  set MBEDTLS_THREADING_C
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  set MBEDTLS_TIMING_C
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_X509_CRL_PARSE_C
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_XTEA_C
./mbedtls/scripts/config.pl -f mbedtls/include/mbedtls/config.h  unset MBEDTLS_TLS_DEFAULT_ALLOW_SHA1_IN_KEY_EXCHANGE


echo "Building .."
cd ${SCRIPT_PATH}/${REPOSITORY_NAME}

export SHARED=1
make -j4 no_test
make -C programs
make -C library

echo "Done!"

