# MotoCPP - A C++ a Motocycle racing online game.

## Table of Contents
1. [Introduction](#introduction)
2. [Features](#features)
3. [Requirements](#requirements)
4. [Installation](#installation)
5. [Configuration](#configuration)
6. [Usage](#usage)
7. [Development](#development)
8. [Testing](#testing)
9. [Contributing](#contributing)
10. [License](#license)
11. [Acknowledgements](#acknowledgements)

## Introduction
**Complexo**, is a C++ -based online racing game with controller support and 
## Features
- **nothing yet tbh**: smol progrem.


## Requirements
Before installing and running **Complexo**, ensure you have the following prerequisites:
- **Operating System**: macOS 10.10+, or a recent Linux distribution.
- **Compiler**: GCC or Clang for Linux/macOS.
- **Libraries**: Regex, SDL2, SDL2_Net
- **Development Tools**: Make or CMake for build automation.

## Installation
### Step-by-Step Guide
1. **Clone the Repository**
    ```bash
    git clone https://github.com/carteeeltheboss/MotoCP.git
    cd MotoCP
    ```

2. **Install gcc and Dependencies**
    - **Windows**: use Visual Sudio and import the project as a solution, use the .sln provided in the repo
    - **macOS**: You need to have brew installed first.
        ```bash
        brew install gcc make autoconf automake libtool
        ```
    - **Linux**: 
        ```bash
        sudo apt update
        sudo apt-get install build-essential
        ```

3. **Build the program**
    - **Using Make**
        ```bash
        make 
        ```

## Configuration
Not much to configure for the moment

## Usage

To start the program, navigate to the directory where the input file is located and run:

```bash
./server fir
./client
```
## Development
### Step-by-Step Guide
1. **Fork the Repository**
2. **Install GCC and Dependencies**

    ```bash
    git clone https://github.com/carteeeltheboss/MotoCP.git
    cd complexo
    ```

3. **Checkout to a new branch**
   
    ```bash
    git checkout -b your-new-branch-name
    ```

4. **Commit your changes**
   
    ```bash
    git commit -am 'Add new feature'
    ```

3. **Push to your fork**
   
    ```bash
    git push origin your-new-branch-name
    ```

## License

Copyright (c) 2024 HANFAOUI Karim and MotoCP

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

## Acknowledgements

I would like to express my sincere gratitude to the following individuals and organizations for their support and contributions to this project:

- **Open Source Contributors**: For the various open-source tools, libraries, and resources that were used in the development of this game.

Special thanks to the following resources and inspirations that played a significant role in the project's design and development:

- Regex: where if you had a problem and tried to use it, you have then 2 problems
- (not chatgpt at all): For helping in debugging and bug tracability.
- [Contributors]: For their valuable contributions, whether it be through code, suggestions, or testing.

