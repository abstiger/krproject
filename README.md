# Introduction

[![Build Status](https://travis-ci.org/abstiger/krproject.svg?branch=master)](https://travis-ci.org/abstiger/krproject)

krproject is a real-time streaming data analysis system based on rules.

it is now consist of:

* __krserver__: the tcp server, an wrapper of krengine
* __krshell__: the command line processor of krserver
* __krweb__: the web frontend, editor for kengine
* __krengine__: the core process engine of this project


# Install

1. Make sure you have installed the dependencies:

   * `git`
   * `gcc` 4.7 or later
   * GNU `make` 3.81 or later
   * `unixodbc` and `unixodbc-dev` 2.2.14 or later

2. Clone the [source] with `git`:

   ```sh
   git clone https://github.com/abstiger/krproject.git
   cd krproject
   ```

[source]: https://github.com/abstiger/krproject

3. Build and install:

    ```sh
    ./configure
    make && make install
    ```

    > ***Note:*** You may need to use `sudo make install` if you do not
    > normally have permission to modify the destination directory. The
    > install locations can be adjusted by passing a `--prefix` argument
    > to `configure`. Various other options are also supported. Pass 
    > `--help` for more information on them.

# Development
1. flex and bison
    ```sh
    sudo apt install flex bison swig 
    ```
 
4. Enable bindings: 

    To enable bindings, you need install [swig] first. then you can add 
    `--enable-python-binding` `--enable-java-binding` arguments to 
    `configure`.

[swig]:http://www.swig.org/

# License

krproject is distributed under the terms of the MIT license.

see [LICENSE](LICENSE) for details.

# Thanks

Special thanks to Jennifer who named it, :)  

Have fun with it~ :P
