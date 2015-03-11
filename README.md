# Introduction

krproject is a **real-time streaming data analysis** system.

This project is designed to be a powerful tool of bigdata's computation,
it is consist of:

* __krengine__: the core process engine of this project
* __kriface__: the interface generator, helper for IO processing
* __krserver__: the tcp server, an wrapper of krengine
* __krcoordi__: the coordinator, associated with krserver
* __krshell__: the command line processor of krserver
* __krweb__: the web frontend, editor for kengine


# Installation

1. Make sure you have installed the dependencies:

   * `git`
   * `gcc` 4.7 or later
   * `make` 3.81 or later
   * `unixodbc` and `unixodbc-dev` 2.2.14 or later

2. Clone the [source] with `git`:

   ```sh
   $ git clone https://github.com/AbsoluteTiger/krproject.git
   $ cd krproject
   ```

[source]: https://github.com/AbsoluteTiger/krproject

3. Build and install:

    ```sh
    $ ./configure
    $ make && make install
    ```

    > ***Note:*** You may need to use `sudo make install` if you do not
    > normally have permission to modify the destination directory. The
    > install locations can be adjusted by passing a `--prefix` argument
    > to `configure`. Various other options are also supported. Pass 
    > `--help` for more information on them.

# License

krproject is distributed under the terms of the MIT license.

see LICENSE for details.

# Thanks

Special thanks to Jennifer who named it, :)  

Have fun with it~ :P
