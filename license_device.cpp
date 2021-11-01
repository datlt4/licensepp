//
// License++
//
//  Copyright © 2018-present Amrayn Web Services
//  https://amrayn.com
//
//  See https://github.com/amrayn/licensepp/blob/master/LICENSE
//
// Usage: ./license-manager-sample --license <license-file> [--signature <signature>]

/**
 * ./license-device \
 * --license ../license.lic \
 * --signature D712EAD67B95D09C8AF84E44ECCAA01D
 *
 * ./license-device
 * --license ../license.enc \
 * --signature D712EAD67B95D09C8AF84E44ECCAA01D \
 * --decrypt
 */

#include <cstring>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <iomanip>
#include "license-manager.h"
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <cctype>

// FIXME: Improve security!
#define ENC_KEY_LEN 32
#define ENC_IV_LEN 16
#define ENC_SALT_LEN 8
#define ENC_ITER 100012
#define ENC_PASS "JxJjd5A2MOTXRFeK"
#define ENC_PASS_SIZE 16
const size_t SALT_PREFIX_LEN = ENC_SALT_LEN ? 8 + ENC_SALT_LEN : 0;

std::string strip(const std::string &inpt)
{
    auto start_it = inpt.begin();
    auto end_it = inpt.rbegin();
    while (!(*start_it > 32 || *start_it < 127))
        ++start_it;
    while (!(*end_it > 32 || *end_it < 127))
        ++end_it;
    return std::string(start_it, end_it.base());
}

std::string decryptModelFile(std::string filepath, bool dectyptLicense = false, int pad = 0)
{
    FILE *file_in = fopen(filepath.c_str(), "rb");
    fseek(file_in, 0, SEEK_END);
    const size_t size_in = ftell(file_in);
    const size_t size_out = size_in - SALT_PREFIX_LEN - pad; // TODO: remove padding

    unsigned char *in = new unsigned char[size_in];
    unsigned char *out = new unsigned char[size_out];
    rewind(file_in);
    fread(in, 1, size_in, file_in);
    fclose(file_in);

    unsigned char keyiv[ENC_KEY_LEN + ENC_IV_LEN];
    PKCS5_PBKDF2_HMAC(ENC_PASS, ENC_PASS_SIZE, ENC_SALT_LEN ? in + 8 : nullptr, ENC_SALT_LEN, ENC_ITER, EVP_sha256(), ENC_KEY_LEN + ENC_IV_LEN, keyiv);
    AES_KEY dec_key;
    AES_set_decrypt_key(keyiv, ENC_KEY_LEN * 8, &dec_key);
    AES_cbc_encrypt(in + SALT_PREFIX_LEN, out, size_out, &dec_key, keyiv + ENC_KEY_LEN, AES_DECRYPT);
    delete[] in;
    std::string tmpname = tmpnam(nullptr);
    if (false /*process*/)
    {
    }
    else if (dectyptLicense)
    {
        std::string contentBase64 = std::string((char *)out, size_out);
        delete[] out;
        return strip(contentBase64);
    }
    else
    {
        std::string tmpname = tmpnam(nullptr);
        FILE *file_out = fopen("EMoi.txt", "wb");
        fwrite(out, 1, size_out, file_out);
        fclose(file_out);
        delete[] out;
    }
    return tmpname;
}

int main(int argc, char *argv[])
{
    std::string licenseFile;
    std::string signature;
    bool decryptLicenseFlag = false;
    for (int i = 0; i < argc; i++)
    {
        std::string arg(argv[i]);
        if (arg == "--license" && i < argc)
        {
            licenseFile = argv[++i];
        }
        else if (arg == "--signature" && i < argc)
        {
            signature = argv[++i];
        }

        else if (arg == "--decrypt")
        {
            decryptLicenseFlag = true;
        }
    }

    if (!licenseFile.empty())
    {
        License license;
        try
        {
            if (decryptLicenseFlag)
            {
                std::string decryptedModel = decryptModelFile(licenseFile, true);
                license.load(decryptedModel);
            }
            else
            {
                license.loadFromFile(licenseFile);
            }

            LicenseManager licenseManager;
            if (!licenseManager.validate(&license, true, signature))
            {
                std::cerr << "License is not valid" << std::endl;
            }
            else
            {
                std::string serialNumber;
                // std::ifstream serialNumberStream("/sys/firmware/devicetree/base/serial-number");
                std::ifstream serialNumberStream("/root/base/serial-number");

                if (!serialNumberStream.is_open())
                {
                    std::cerr << "Jetson is not valid" << std::endl;
                    return 0;
                }
                else
                {
                    std::getline(serialNumberStream, serialNumber);
                    serialNumberStream.close();
                }
                serialNumber = strip(serialNumber);
                if (serialNumber == license.additionalPayload())
                {
                    std::cout << std::left << std::setw(25) << "[ Serial-Number ]" << serialNumber << std::endl;
                    std::cout << std::left << std::setw(25) << "[ licensee ]" << license.licensee() << std::endl;
                    std::cout << std::left << std::setw(25) << "[ issuingAuthorityId ]" << license.issuingAuthorityId() << std::endl;
                    std::cout << std::left << std::setw(25) << "[ licenseeSignature ]" << license.licenseeSignature() << std::endl;
                    std::cout << std::left << std::setw(25) << "[ authoritySignature ]" << license.authoritySignature() << std::endl;
                    std::cout << std::left << std::setw(25) << "[ formattedExpiry ]" << license.formattedExpiry() << std::endl;
                    std::cout << std::left << std::setw(25) << "[ additionalPayload ]" << license.additionalPayload() << std::endl;
                }
                else
                {
                    std::cerr << "License not match Jetson" << std::endl;
                }
            }
        }
        catch (LicenseException &e)
        {
            std::cerr << "Exception thrown " << e.what() << std::endl;
            return 0;
        }
    }
    else
    {
        std::cout << "License file not provided" << std::endl;
        std::cout << "Usage: ./license-manager-sample --license <license-file> [--signature <signature>]" << std::endl;
    }
    return 0;
}
