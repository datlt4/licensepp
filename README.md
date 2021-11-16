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

# Build server for query license

## Build API

```
mkdir -p build2
cd build2
cmake ..
make -j$(nproc)
```

## Run API

```
./licensepp-generator-server 6464
```

## Query with `curl`

```bash
BOARD_ID=0981245263
curl 0.0.0.0:6464/license?serial=0981245263 | base64 -d > license.enc
```

For more params

```bash
BOARD_ID=0981245263
PERIOD=86400
AUTHORITY=sample-license-authority
SIGNATURE=D712EAD67B95D09C8AF84E44ECCAA01D
LICENSEE=Vizgard_ltd
curl 0.0.0.0:6464/license?serial=0981245263&period=86400&authority=sample-license-authority&signature=D712EAD67B95D09C8AF84E44ECCAA01D&licensee=Vizgard_ltd | base64 -d > license.enc
```
