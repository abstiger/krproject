# Introduction

[![Build Status](https://travis-ci.org/abstiger/krproject.svg?branch=master)](https://travis-ci.org/abstiger/krproject)

krproject is a **rule-based real-time streaming data analysis system**. it is now consist of:

* __krshell__: command line processor for krserver
* __krweb__: web frontend editor for krserver
* __krserver__: tcp server for krengine
* __krtools__: useful tools for krengine
* __krengine__: the core process engine
* __krinput__: the input processor of krengine
* __kroutput__: the output processor of krengine
* __krdata__: dataitem definition and processing
* __krdb__: in-memory timeseries database
* __krparam__: parameter definition and processing
* __krcalc__: an expression calculator
* __krutils__: utilities of krengine

# Install

1. Make sure you have installed the dependencies:

   * `git`
   * `gcc` 4.7 or later
   * GNU `make` 3.81 or later
   * `unixodbc` and `unixodbc-dev` 2.2.14 or later

2. Clone the [source] with `git`:

   ```sh
   $ git clone https://github.com/abstiger/krproject.git
   $ cd krproject
   ```

[source]: https://github.com/abstiger/krproject

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
