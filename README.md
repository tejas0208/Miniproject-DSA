## AES and RSA Encryption using C

DSA 2016-17

MIS:111508053

Name:Tejas Uday Nayak

### Features

## AES:

Symmetric Key Algorithm

FIPS PUB 197: Advanced Encryption Standard (AES)

Padding done using PKCS#7 Standard

Sample keys corresponding to each size(128 / 192 / 256) included for quick demo

Can encrypt even large files in quick time

Used matrices as the prime data structure for implementation

## RSA:

1024 bit RSA cryptosystem

Created new data type for multiple precision integers using linked lists

Used gmp library for quick modular exponentiation and prime no generation

Being a slow algorithm RSA is not primarily used for file encryption but for securing symmetric keys


###Usage

```
$ make
$ ./final [-d | -e | -h] <source> <destination> <key_file> <key_size>
```

## Bibliography:

Cryptography and Network Security - Behrouz A. Forouzan

Cryptography and Network Security Principles and Practices - William Stallings

https://en.wikipedia.org/wiki/RSA_(cryptosystem)

https://en.wikipedia.org/wiki/Advanced_Encryption_Standard
