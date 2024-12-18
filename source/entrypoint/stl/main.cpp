#include "twittercli.hpp"

int main(int argc, char* argv[]) {

    // Replace these placeholders with your actual API credentials
    std::string consumerKey = "HRTuFcju8TpJJoJwhPTeh78y8";
    std::string consumerSecret = "ZxiB9Ss6fPYJ7pb1lIOgSVC2MeVVDjM0fJmaBjscoGSPxIwNyg";

    // Create an instance of the TwitterCLI class
    TwitterCLI cli(consumerKey, consumerSecret);

    // Run the CLI interface, passing command-line arguments
    cli.run(argc, argv);

    return 0;
}
