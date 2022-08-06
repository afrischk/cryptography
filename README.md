# cryptography
A repository for crypto challenges.

## Craxor
A tool for xor encryption and decryption. Build by solving the cryptopals challenges from the base set 1.1-1.6.

## Usage
```
Usage: craxor [OPTION...] FILE
Craxor is a tool to crack xor encryption. It takes a base64 encoded encrypted
input and tries to decrpyt the cipher text.

  -a, --min_key_len=MINKEYLEN   Minimum assumend key length (default: 2).
  -b, --max_key_len=MAXKEYLEN   Maximum assumend key length (default: 40).
  -?, --help                 Give this help list
      --usage                Give a short usage message
  -V, --version              Print program version

Mandatory or optional arguments to long options are also mandatory or optional
for any corresponding short options.
```
