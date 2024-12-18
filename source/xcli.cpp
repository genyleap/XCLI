#include "xcli.hpp"

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, Types::String* outBuffer) {
    auto totalSize = size * nmemb;
    outBuffer->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

XCLI::XCLI(Types::String key, Types::String secret)
    : consumerKey(std::move(key)), consumerSecret(std::move(secret)) {}

bool XCLI::loadTokens() {
    if (!std::filesystem::exists(tokenFile)) {
        Logger::formattedError("Error: Token file '{}' does not exist.", tokenFile);
        return false;
    }

    std::ifstream file(tokenFile.data());
    if (!file.is_open()) {
        Logger::formattedError("Error: Unable to open token file '{}'.", tokenFile);
        return false;
    }

    Json::Value tokens;
    Json::CharReaderBuilder readerBuilder;
    Types::String errors;

    if (!Json::parseFromStream(readerBuilder, file, &tokens, &errors)) {
        Logger::formattedError("Error: Unable to parse token file '{}'.\n{}", tokenFile, errors);
        return false;
    }

    bearerToken = tokens["bearer_token"].asString();
    accessToken = tokens["access_token"].asString();
    accessSecret = tokens["access_secret"].asString();

    return !bearerToken.empty();
}

void XCLI::saveTokens() {
    Json::Value tokens;
    tokens["bearer_token"] = bearerToken;
    tokens["access_token"] = accessToken;
    tokens["access_secret"] = accessSecret;

    std::ofstream file(tokenFile.data());
    if (!file.is_open()) {
        Logger::formattedError("Error: Unable to save tokens to file '{}'.", tokenFile);
        return;
    }

    Json::StreamWriterBuilder writerBuilder;
    file << Json::writeString(writerBuilder, tokens);

    Logger::formattedInfo("Tokens saved successfully to '{}'.", tokenFile);
}

Types::String XCLI::buildQueryString(const Types::MapString& params) {
    Types::String query;
    for (const auto& param : params) {
        query += std::format("{}={}&", param.first, param.second);
    }
    if (!query.empty()) query.pop_back(); // Remove trailing '&'
    return query;
}

CURL* XCLI::initializeCURL(Types::StringView url, Types::String& response, curl_slist* headers) {
    if (auto curl = curl_easy_init(); curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.data());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        if (headers) {
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        }
        return curl;
    }
    Logger::error("Failed to initialize CURL");
    return nullptr;
}

bool XCLI::authenticateOAuth2() {
    constexpr Types::StringView url = "https://api.twitter.com/oauth2/token";
    Types::String response;
    if (auto curl = initializeCURL(url, response); curl) {
        const Types::String credentials = consumerKey + ":" + consumerSecret;
        const Types::String authHeader = std::format("Authorization: Basic {}", base64Encode(credentials));

        curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, authHeader.c_str());
        headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded;charset=UTF-8");

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "grant_type=client_credentials");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);

        if (res != CURLE_OK) {
            Logger::formattedError("CURL error: {}", curl_easy_strerror(res));
            return false;
        }

        Json::Value jsonResponse;
        Json::CharReaderBuilder readerBuilder;
        Types::String errors;

        std::istringstream responseStream(response);
        if (!Json::parseFromStream(readerBuilder, responseStream, &jsonResponse, &errors)) {
            Logger::formattedError("Error parsing JSON response: {}", errors);
            return false;
        }

        if (!jsonResponse.isMember("access_token")) {
            Logger::error("Error: No access_token in response.");
            return false;
        }

        bearerToken = jsonResponse["access_token"].asString();
        if (bearerToken.empty()) {
            Logger::error("Error: Bearer token is empty.");
            return false;
        }

        saveTokens();
        return true;
    }
    return false;
}

Types::String XCLI::base64Encode(Types::StringView input) {
    constexpr auto encodeTable = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    Types::String encoded;
    int val = 0;
    int valb = -6;
    for (unsigned char c : input) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            encoded.push_back(encodeTable[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) encoded.push_back(encodeTable[((val << 8) >> (valb + 8)) & 0x3F]);
    while (encoded.size() % 4) encoded.push_back('=');
    return encoded;
}

void XCLI::getRequest(Types::StringView endpoint, const Types::MapString& params) {
    if (bearerToken.empty() && (accessToken.empty() || accessSecret.empty())) {
        if (!loadTokens()) {
            Logger::error("Authenticate first!");
            return;
        }
    }

    Types::String url = std::format("https://api.twitter.com{}?{}", endpoint, buildQueryString(params));

    Types::String response;
    if (auto curl = initializeCURL(url, response); curl) {
        curl_slist* headers = nullptr;
        if (!bearerToken.empty()) {
            const Types::String authHeader = std::format("Authorization: Bearer {}", bearerToken);
            headers = curl_slist_append(headers, authHeader.c_str());
        }

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);

        if (res != CURLE_OK) {
            Logger::formattedError("CURL error: {}", curl_easy_strerror(res));
            return;
        }

        Logger::formattedInfo("API Response:\n{}", response);
    }
}

void XCLI::run(int argc, char* argv[]) {
    if (argc < 2) {
        Logger::error("Usage: xcli <command> [options]");
        return;
    }

    Types::StringView command = argv[1];

    if (command == "auth") {
        if (authenticateOAuth2()) {
            Logger::info("Authentication successful.");
        } else {
            Logger::error("Authentication failed.");
        }
    } else if (command == "get") {
        if (argc < 3) {
            Logger::error("Usage: xcli get <endpoint> [key=value ...]");
            return;
        }

        Types::StringView endpoint = argv[2];
        Types::MapString params;
        for (int i = 3; i < argc; ++i) {
            Types::String arg = argv[i];
            if (auto pos = arg.find('='); pos != Types::String::npos) {
                params.emplace(arg.substr(0, pos), arg.substr(pos + 1));
            }
        }

        getRequest(endpoint, params);
    } else if (command == "space") {
        if (argc < 3) {
            Logger::error("Usage: xcli space <space_id>");
            return;
        }

        Types::String spaceId = argv[2];
        Types::String endpoint = "/2/spaces/" + spaceId;

               // Query specific fields for the Twitter Space
        Types::MapString params = {
            {"space.fields", "host_ids,created_at,title,participant_count"}
        };

        getRequest(endpoint, params);
    } else {
        Logger::formattedError("Unknown command: {}", command);
    }
}
