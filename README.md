# Installation

1. Export `licensepp` directory

```
export LICENSEPP {...}
```

2. Clone repositorise
```
cd ${LICENSEPP}
git clone https://github.com/amrayn/licensepp
git clone https://github.com/weidai11/cryptopp.git
git clone https://github.com/amraynweb/cryptopp-pem.git
git clone https://github.com/amrayn/ripe.git
cp cryptopp-pem/* cryptopp/
```

3. Install `cryptopp`

```
cd ${LICENSEPP}/cryptopp
make -j$(nproc)
sudo make install
```

4. Install `licensepp`

```
mkdir -p ${LICENSEPP}/licensepp/build
cd ${LICENSEPP}/licensepp/build
cmake ..
make -j$(nproc)
sudo make install
```

5. Install `ripe`

```
mkdir -p ${LICENSEPP}/ripe/build
cd ${LICENSEPP}/ripe/build
cmake ..
make -j$(nproc)
sudo make install
```

6. Export environment

```
echo "export LD_LIBRARY_PATH=/usr/local/lib:\$LD_LIBRARY_PATH" >> ~/.bashrc
source ~/bashrc
```

7. Install `OpenSSL`

```
sudo apt install libssl1* -y
```

# Generate new keys

1. [Generate New Authority Key](https://github.com/amrayn/licensepp#generate-new-authority-key)

```
ripe -g --rsa --length 2048 [--secret <secret>]
```

2. [Generate New Signature Key](https://github.com/amrayn/licensepp#generate-new-signature-key)

```
ripe -g --aes --length 128
```

# Build application that generate license

## Build

```
g++ main.cc -I/usr/local/lib -llicensepp -lcryptopp -std=c++2a -O3 -o license-manager
```

## Generate license

```
LICENSEE="Vizgard_ltd"
PERIOD="86400"
BOARD_ID="1421621043399"
OUTPUT_LICENSE="license.lic"
./license-manager --issue \
--licensee ${LICENSEE} \
--period ${PERIOD} \
--authority sample-license-authority \
--signature D712EAD67B95D09C8AF84E44ECCAA01D \
--additional-payload ${BOARD_ID} \
--output-license ${OUTPUT_LICENSE}
```

## Validate license

```
LICENSE_FILE="license.lic"
./license-manager \
--validate ${LICENSE_FILE} \
--signature D712EAD67B95D09C8AF84E44ECCAA01D
```

## Encrypt license

```
openssl enc -aes-256-cbc -pbkdf2 -iter 100012 -k JxJjd5A2MOTXRFeK -in license.lic -out license.enc
```

# Build application that decrypt license

## Encrypt license

```
LICENSE_FILE="license.lic"
ENCRYPT_FILE="license.enc"
openssl enc -aes-256-cbc -pbkdf2 -iter 100012 -k JxJjd5A2MOTXRFeK -in ${LICENSE_FILE} -out ${ENCRYPT_FILE}
```

## Decrypt license

```
mkdir -p build && cd build
cmake ..
make -j$(nproc)
```

```
./license-device \
--license ../license.lic \
--signature D712EAD67B95D09C8AF84E44ECCAA01D
```

```
./license-device \
--license ../license.enc \
--signature D712EAD67B95D09C8AF84E44ECCAA01D \
--decrypt
```
