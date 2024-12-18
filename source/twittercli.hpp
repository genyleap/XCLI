#ifndef TWITTERCLI_HPP
#define TWITTERCLI_HPP

#include "common.hpp"
#include <curl/curl.h>
#include <json/json.h>

/**
 * @class Logger
 * @brief Utility class for logging messages to standard output and error streams.
 */
class Logger {
public:
    /**
     * @brief Logs an error message to the standard error stream.
     * @param message The error message to log.
     */
    static void error(const Types::String& message) {
        std::cerr << message << std::endl;
    }

    /**
     * @brief Logs an informational message to the standard output stream.
     * @param message The informational message to log.
     */
    static void info(const Types::String& message) {
        std::cout << message << std::endl;
    }

    /**
     * @brief Logs a formatted error message to the standard error stream.
     * @tparam Args Variadic template for the format arguments.
     * @param fmt The format string.
     * @param args The format arguments.
     */
    template<typename... Args>
    static void formattedError(const std::format_string<Args...>& fmt, Args&&... args) {
        error(std::format(fmt, std::forward<Args>(args)...));
    }

    /**
     * @brief Logs a formatted informational message to the standard output stream.
     * @tparam Args Variadic template for the format arguments.
     * @param fmt The format string.
     * @param args The format arguments.
     */
    template<typename... Args>
    static void formattedInfo(const std::format_string<Args...>& fmt, Args&&... args) {
        info(std::format(fmt, std::forward<Args>(args)...));
    }
};

/**
 * @class TwitterCLI
 * @brief A command-line interface for interacting with the Twitter API.
 */
class TwitterCLI {
private:
    Types::String consumerKey;   ///< Twitter API consumer key.
    Types::String consumerSecret; ///< Twitter API consumer secret.
    Types::String accessToken;   ///< Twitter API access token.
    Types::String accessSecret;  ///< Twitter API access secret.
    Types::String bearerToken;   ///< Twitter API bearer token.

    static constexpr Types::StringView tokenFile = "auth_tokens.json"; ///< File to store authentication tokens.

    /**
     * @brief Loads authentication tokens from a file.
     * @return True if tokens were loaded successfully, false otherwise.
     */
    bool loadTokens();

    /**
     * @brief Saves authentication tokens to a file.
     */
    void saveTokens();

    /**
     * @brief Initializes a CURL session with the given URL and headers.
     * @param url The URL for the request.
     * @param response Reference to a string to store the response data.
     * @param headers Optional CURL headers to attach to the request.
     * @return A pointer to the initialized CURL object.
     */
    CURL* initializeCURL(Types::StringView url, Types::String& response, curl_slist* headers = nullptr);

    /**
     * @brief Encodes a string in Base64 format.
     * @param input The input string to encode.
     * @return The Base64-encoded string.
     */
    Types::String base64Encode(Types::StringView input);

    /**
     * @brief Constructs a query string from a map of key-value pairs.
     * @param params The parameters to include in the query string.
     * @return A query string for use in HTTP requests.
     */
    Types::String buildQueryString(const Types::MapString& params);

public:
    /**
     * @brief Constructs a TwitterCLI object with the given API keys.
     * @param key The Twitter API consumer key.
     * @param secret The Twitter API consumer secret.
     */
    TwitterCLI(Types::String key, Types::String secret);

    /**
     * @brief Authenticates with Twitter using OAuth 2.0 and retrieves a bearer token.
     * @return True if authentication was successful, false otherwise.
     */
    bool authenticateOAuth2();

    /**
     * @brief Performs a GET request to the Twitter API.
     * @param endpoint The API endpoint to query.
     * @param params The query parameters for the request.
     */
    void getRequest(Types::StringView endpoint, const Types::MapString& params);

    /**
     * @brief Runs the command-line interface.
     * @param argc The argument count.
     * @param argv The argument vector.
     */
    void run(int argc, char* argv[]);
};

#endif // TWITTERCLI_HPP
