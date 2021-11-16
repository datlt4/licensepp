#include "main.h"

int main(int argc, char **argv)
{
    int port = 6464;
    if (argc == 2)
        port = std::stoi(std::string(argv[1]));

    crow::SimpleApp app;

    CROW_ROUTE(app, "/license").methods("GET"_method)([](const crow::request &req)
                                                      {
                                                    try
                                                    {
                                                        std::string signature;
                                                        std::string licensee;
                                                        std::string secret;
                                                        std::string authority = "default";
                                                        std::string additionalPayload;
                                                        unsigned int period;


                                                        if (req.url_params.get("serial") == nullptr)
                                                            return crow::response(400);
                                                        additionalPayload = std::string(req.url_params.get("serial"));

                                                        if (req.url_params.get("period") == nullptr)
                                                            period = 86400;
                                                        else
                                                            period = static_cast<unsigned int>(std::stoi(std::string(req.url_params.get("period"))));

                                                        if (req.url_params.get("authority") == nullptr)
                                                            authority = "sample-license-authority";
                                                        else
                                                            authority = std::string(req.url_params.get("authority"));

                                                        if (req.url_params.get("signature") == nullptr)
                                                            signature = "D712EAD67B95D09C8AF84E44ECCAA01D";
                                                        else
                                                            signature = std::string(req.url_params.get("signature"));

                                                        if (req.url_params.get("licensee") == nullptr)
                                                            licensee = "Vizgard_ltd";
                                                        else
                                                            licensee = std::string(req.url_params.get("signature"));


                                                        const licensepp::IssuingAuthority *issuingAuthority = nullptr;
                                                        for (const auto &a : LicenseKeysRegister::LICENSE_ISSUING_AUTHORITIES)
                                                        {
                                                            if (a.id() == authority)
                                                            {
                                                                issuingAuthority = &(a);
                                                            }
                                                        }

                                                        LicenseManager licenseManager;
                                                        licensepp::License license = licenseManager.issue(licensee, period, issuingAuthority, secret, signature, additionalPayload);
                                                        std::ofstream outfile;
                                                        outfile.open(additionalPayload + ".lic");
                                                        outfile << license.toString() << std::endl;
                                                        outfile.close();
        
                                                        std::ostringstream cmd;
                                                        cmd << "openssl enc -aes-256-cbc -pbkdf2 -iter 100012 -k JxJjd5A2MOTXRFeK -in " << additionalPayload << ".lic -out " << additionalPayload << ".enc";
                                                        std::array<char, 128> buffer0;
                                                        std::string result;
                                                        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.str().c_str(), "r"), pclose);
                                                        if (!pipe) {
                                                            throw std::runtime_error("popen() failed!");
                                                        }
                                                        while (fgets(buffer0.data(), buffer0.size(), pipe.get()) != nullptr) {
                                                            result += buffer0.data();
                                                        }
                                               
                                                        std::ifstream infile(additionalPayload + ".enc");
                                                        //get length of file
                                                        infile.seekg(0, std::ios::end);
                                                        size_t length = infile.tellg();
                                                        infile.seekg(0, std::ios::beg);
                                                        //read file
                                                        char buffer[length];
                                                        infile.read(buffer, length);
                                                        std::string base64_encode_data = base64_encode(reinterpret_cast<unsigned char*>( buffer), length);
                                                        return crow::response(std::string(base64_encode_data));
                                                    }
                                                    catch (const std::exception &e)
                                                    {
                                                        std::cerr << "[ERROR] " << e.what() << std::endl;
                                                        return crow::response(500);
                                                    } });

    app.port(port).multithreaded().run();
    return 0;
}