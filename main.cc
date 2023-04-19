//
// License++
//
// Copyright © 2018-present Amrayn Web Services
// https://amrayn.com
//
// See https://github.com/amrayn/licensepp/blob/master/LICENSE
//

/**
 * ./license-manager --issue \
 * --licensee dragziv_ltd \
 * --period 86400 \
 * --authority sample-license-authority \
 * --signature D712EAD67B95D09C8AF84E44ECCAA01D \
 * --additional-payload "1421621043399" \
 * --output-license "license.lic"
 */

/**
 * ./license-manager \
 * --validate license.m \
 * --signature D712EAD67B95D09C8AF84E44ECCAA01D
 */

#include <cstring>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <iomanip>
#include "license-manager.h"

void displayUsage()
{
    std::cout << "USAGE: license-manager [--validate <file> --signature <signature>] [--issue --licensee <licensee> --signature <licensee_signature> --period <validation_period> --authority <issuing_authority> --passphrase <passphrase_for_issuing_authority> [--additional-payload <additional data>]]" << std::endl;
}

void displayVersion()
{
    std::cout << "License Manager v1.0.0" << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc > 1 && strcmp(argv[1], "--version") == 0)
    {
        displayVersion();
        return 0;
    }

    if (argc > 1 && strcmp(argv[1], "--help") == 0)
    {
        displayUsage();
        return 0;
    }

    std::string licenseFile;
    std::string signature;
    std::string licensee;
    std::string secret;
    std::string authority = "default";
    std::string additionalPayload;
    std::string output_license;
    unsigned int period = 0U;
    bool doIssue = false;
    bool doValidate = false;

    for (int i = 0; i < argc; i++)
    {
        std::string arg(argv[i]);
        if (arg == "--validate" && i < argc)
        {
            licenseFile = argv[++i];
            doValidate = true;
        }
        else if (arg == "--signature" && i < argc)
        {
            signature = argv[++i];
        }
        else if (arg == "--issue" && i < argc)
        {
            doIssue = true;
        }
        else if (arg == "--licensee" && i < argc)
        {
            licensee = argv[++i];
        }
        else if (arg == "--period" && i < argc)
        {
            period = static_cast<unsigned int>(atoi(argv[++i]));
        }
        else if (arg == "--authority" && i < argc)
        {
            authority = argv[++i];
        }
        else if (arg == "--passphrase" && i < argc)
        {
            secret = argv[++i];
        }
        else if (arg == "--additional-payload" && i < argc)
        {
            additionalPayload = argv[++i];
        }
        else if (arg == "--output-license" && i < argc)
        {
            output_license = argv[++i];
        }
    }

    LicenseManager licenseManager;
    if (doValidate && !licenseFile.empty())
    {
        License license;
        try
        {
            std::cout << "[ doValidate ]" << std::endl;
            if (license.loadFromFile(licenseFile))
            {
                if (!licenseManager.validate(&license, true, signature))
                {
                    std::cout << "License is not valid" << std::endl;
                }
                else
                {
                    // std::cout << "Licensed to " << license.licensee() << std::endl;
                    // std::cout << "Subscription is active until " << license.formattedExpiry() << std::endl << std::endl;
                    std::cout << std::endl;
                    std::cout << std::left << std::setw(25) << "[ licensee ]" << license.licensee() << std::endl;
                    std::cout << std::left << std::setw(25) << "[ issuingAuthorityId ]" << license.issuingAuthorityId() << std::endl;
                    std::cout << std::left << std::setw(25) << "[ licenseeSignature ]" << license.licenseeSignature() << std::endl;
                    std::cout << std::left << std::setw(25) << "[ authoritySignature ]" << license.authoritySignature() << std::endl;
                    std::cout << std::left << std::setw(25) << "[ formattedExpiry ]" << license.formattedExpiry() << std::endl;
                    std::cout << std::left << std::setw(25) << "[ additionalPayload ]" << license.additionalPayload() << std::endl;
                }
            }
        }
        catch (LicenseException &e)
        {
            std::cerr << "Exception thrown " << e.what() << std::endl;
        }
    }
    else if (doIssue)
    {
        const licensepp::IssuingAuthority *issuingAuthority = nullptr;
        for (const auto &a : LicenseKeysRegister::LICENSE_ISSUING_AUTHORITIES)
        {
            if (a.id() == authority)
            {
                issuingAuthority = &(a);
            }
        }
        if (issuingAuthority == nullptr)
        {
            std::cout << "Invalid issuing authority." << std::endl;
            return 1;
        }
        licensepp::License license = licenseManager.issue(licensee, period, issuingAuthority, secret, signature, additionalPayload);
        std::cout << license.toString() << std::endl;
        std::ofstream outfile;
        outfile.open(output_license);
        outfile << license.toString() << std::endl;
        outfile.close();
        // std::cout << "Licensed to " << license.licensee() << std::endl;
        // std::cout << "Subscription is active until " << license.formattedExpiry() << std::endl << std::endl;
        std::cout << std::endl;
        std::cout << std::left << std::setw(25) << "[ licensee ]" << license.licensee() << std::endl;
        std::cout << std::left << std::setw(25) << "[ issuingAuthorityId ]" << license.issuingAuthorityId() << std::endl;
        std::cout << std::left << std::setw(25) << "[ licenseeSignature ]" << license.licenseeSignature() << std::endl;
        std::cout << std::left << std::setw(25) << "[ authoritySignature ]" << license.authoritySignature() << std::endl;
        std::cout << std::left << std::setw(25) << "[ formattedExpiry ]" << license.formattedExpiry() << std::endl;
        std::cout << std::left << std::setw(25) << "[ additionalPayload ]" << license.additionalPayload() << std::endl;
    }
    else
    {
        displayUsage();
    }
    return 0;
}
