mod_blacklist
-------------

A simple Apache 2 module to blacklist remote visitors, by IP address.


Links
-----

* Github
   * https://github.com/skx/mod_blacklist
* Mirror
   * https://git.steve.org.uk/skx/mod_blacklist


About
-----

This is a simple Apache module which will perform an ACL check on each incoming HTTP request.  If the remote visitor has been blacklisted each access will result in a `403` forbidden response.

Access is tested, trivially, via a lookup of their remote IP address in a particular directory, which defaults to `/etc/blacklist.d/`.

For example to blacklist the remote IP 1.2.3.4:

    touch /etc/blacklist.d/1.2.3.4

This is 100% dynamic, and changes will be reflected immediately.  Removing a previously blacklisted entry can be achieved via:

    rm /etc/blacklist.d/1.2.3.4


Performance
-----------

The module is pretty lightweight, a single extra `stat()` call for each visitor will be made to perform the access-test.

In a low-traffic server, which is not otherwise I/O bound, this overhead should be minimal.


Compilation
-----------

Assume you have the appropriate Apache-development package(s) installed upon your host it can be compiled `apxs`:

    apxs2 -c mod_blacklist.c

The `Makefile` does that for you.


Installation
------------

Once compiled copy the `.so` file from `.libs` to `/usr/lib/apache2/modules`, or your local module path.

To cause the module to be loaded by Apache create the file `/etc/apache2/mods-enabled/blacklist.load` with the following contents (adjusting your local path if different):

     LoadModule blacklist_module /usr/lib/apache2/modules/mod_blacklist.so

If you wish to change the prefix-directory in which blacklisted IP addresses are stored then you can use the `BlacklistPrefix` setting.  This is a global setting, which you could add to the file  `/etc/apache2/mods-enabled/blacklist.conf`:

     # Change the blacklist prefix.
     BlacklistPrefix /root/blacklist.d/

Steve
--
