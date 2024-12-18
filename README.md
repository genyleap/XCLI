Here’s a professional and comprehensive README for your GitHub project:

---

# XCLI

XCLI is a command-line interface (CLI) tool for interacting with the Twitter API. It allows developers to perform actions such as authenticating via OAuth 2.0, retrieving information from API endpoints, and working with Twitter Spaces. This project is designed with a focus on simplicity, modularity, and extensibility.

## Features

- **OAuth 2.0 Authentication**: Securely authenticate with Twitter using bearer tokens.
- **Custom API Requests**: Perform GET requests to specified Twitter API endpoints with query parameters.
- **Twitter Spaces**: Fetch detailed information about Twitter Spaces by providing a Space ID.
- **Extensive Logging**: Provides formatted error and info logs for better debugging and user feedback.

## Requirements

- **C++ Compiler**: A modern C++ compiler with support for C++20 or higher (e.g., GCC 10+, Clang 12+, or MSVC 2019+).
- **Dependencies**:
  - [cURL](https://curl.se/libcurl/) for HTTP requests.
  - [JSON for Modern C++](https://github.com/nlohmann/json) for parsing and generating JSON.
- **Build Tools**:
  - [CMake](https://cmake.org/) for building the project.

## Installation

### Clone the Repository

```bash
git clone https://github.com/genyleap/XCLI.git
cd XCLI
```

### Build the Project

1. Create a build directory:
   ```bash
   mkdir build && cd build
   ```

2. Run CMake to configure the build:
   ```bash
   cmake ..
   ```

3. Build the project:
   ```bash
   cmake --build .
   ```

## Usage

### Basic Command Structure

```bash
twitter_cli <command> [options]
```

### Available Commands

- **`auth`**: Authenticate with Twitter using OAuth 2.0.
  ```bash
  twitter_cli auth
  ```

- **`get`**: Perform a GET request to a specified API endpoint.
  ```bash
  twitter_cli get <endpoint> [key=value ...]
  ```

  Example:
  ```bash
  twitter_cli get /2/users/by/username/johndoe user.fields=id,name,username
  ```

- **`space`**: Fetch information about a Twitter Space using its Space ID.
  ```bash
  twitter_cli space <space_id>
  ```

  Example:
  ```bash
  twitter_cli space 1vOGwMNOZXYxB
  ```

### Example Output

```plaintext
Authentication successful.
Tokens saved successfully to "auth_tokens.json".

API Response:
{
    "data": {
        "id": "1vOGwMNOZXYxB",
        "title": "Tech Talks",
        "host_ids": ["123456789"],
        "participant_count": 150,
        "created_at": "2024-12-15T10:00:00Z"
    }
}
```

## Project Structure

```
XCLI/
├── source/               # Source code files
│   ├── XCLI.cpp    # Main implementation of XCLI
│   └── ...
├── include/              # Header files
│   └── XCLI.hpp    # Declaration of XCLI class
├── third-party/          # Third-party dependencies (e.g., JSON library)
├── CMakeLists.txt        # CMake configuration
└── README.md             # Project documentation
```

## Configuration

Modify the following constants in `XCLI.hpp` as needed:

- `tokenFile`: Path to the JSON file where authentication tokens are stored.

## Contributing

Contributions are welcome! If you have ideas or improvements, feel free to submit a pull request or open an issue.

### Steps to Contribute

1. Fork the repository.
2. Create a new branch:
   ```bash
   git checkout -b feature/your-feature
   ```
3. Commit your changes:
   ```bash
   git commit -m "Add your feature description"
   ```
4. Push to your fork:
   ```bash
   git push origin feature/your-feature
   ```
5. Submit a pull request.

## License

This project is licensed under the [MIT License](LICENSE).

## Acknowledgments

- [Twitter API](https://developer.twitter.com/en/docs) for providing access to social data.
- [cURL](https://curl.se/) for HTTP requests.
- [JSON for Modern C++](https://github.com/open-source-parsers/jsoncpp) for JSON parsing.

---

Feel free to customize the placeholders (e.g., `genyleap` in the repository URL) to match your project specifics.
