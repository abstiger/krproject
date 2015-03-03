# Introduction

krproject is a **real-time streaming data analysis** system.

This project is designed to be a powerful tool of bigdata's computation,
which now consist of:

* __krengine__: the core process engine of this project
* __krserver__: the tcp server, an wrapper of krengine
* __krcoordi__: the coordinator, associated with krserver
* __krshell__: the command line processor of krserver

Special thanks to Jennifer who named this, :)  

Have fun with it~ :P

# Build and Install

1. Make sure you have installed the dependencies:

   * `git`
   * `gcc` 4.7 or later
   * `python` 2.7 or later (but not 3.x)
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
    > to `configure`. Various other options are also supported – pass
    > `--help` for more information on them.

## License

krproject is primarily distributed under the terms of the MIT license.

see LICENSE for details.
