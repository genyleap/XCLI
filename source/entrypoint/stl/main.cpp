#include "xcli.hpp"

int main(int argc, char* argv[]) {

    // Replace these placeholders with your actual API credentials
    std::string consumerKey = "YourApiKey";
    std::string consumerSecret = "YourSecretKey";

    // Create an instance of the XCLI class
    XCLI cli(consumerKey, consumerSecret);

    // Run the CLI interface, passing command-line arguments
    cli.run(argc, argv);

    return 0;
}
