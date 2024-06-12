#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include <string>

class ConfigFile {
public:
    ConfigFile() = delete;
    explicit ConfigFile(const std::string& filePath);

    std::string rpc_url = "127.0.0.1";
    std::string cookie_file_path;
    std::string auth;
    int rpc_port = 8332;
    int port = 2024;

private:
    void loadConfigFile(const std::string& filePath);
    std::string read_cookie_file(const std::string& filepath);
};

#endif // CONFIGFILE_H

