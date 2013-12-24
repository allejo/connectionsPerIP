Connections Per IP
==================

A BZFlag plugin that will allow you to limit the number of connections from a certain IP address. A typical use for this plugin is to only allow two connections from an IP to prevent bots or multiple clients from joining to spam a server.

Table of Contents
-----------------

-   [Author](#author)

-   [Compiling](#compiling)

    -   [Requirements](#requirements)

    -   [How to Compile](#how-to-compile)

-   [Server Details](#server-details)

-   [License](#license)

Author
------

Vladimir “allejo” Jimenez

Compiling
---------

### Requirements

-   BZFlag 2.4.0+

### How To Compile

1.  Check out the BZFlag source code.  
    ```git clone -b v2_4_x https://github.com/BZFlag-Dev/bzflag-import-3.git bzflag```

2.  Got into the newly checked out source code and then the plugins directory.  
    ```cd plugins```

3.  Create a plugin using the `newplug.sh` script.  
    ```sh newplug.sh connectionsPerIP```

4.  Delete the newly create connectionsPerIP directory.  
    ```rm -rf connectionsPerIP```

5.  Run a git clone of this repository from within the plugins directory. This should have created a new connectionsPerIP directory withint the plugins directory.  
    ```git clone https://github.com/allejo/connectionsPerIP.git```

6.  Instruct the build system to generate a Makefile and then compile and install the plugin.  
    ```cd ..; ./autogen.sh; ./configure; make; make install;```

Server Details
--------------

To use this plugin after it has been compiled, simply load the plugin via the configuration file.

```-loadplugin /path/to/connectionsPerIP.so```

This plugin takes an optional command line parameter, the number of connections allowed. The default number of connections is 2. To use the optional command line parameter, load the plugin using a comma and a number after it. In this example, the maximum number of connections allowed would be 3.

```-loadplugin /path/to/connectionsPerIP.so,3```

License
-------

GPLv3
