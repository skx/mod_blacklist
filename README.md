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

This is a simple Apache module which will perform an ACL check on
each incoming HTTP request.  If the remote visitor has been blacklisted
each access will result in a `403` forbidden response.

Access is tested, trivially, via a lookup of their remote IP address
in a particular directory.  This will default to `/etc/blacklist.d/`.

For example to blacklist the remote IP 1.2.3.4:

    touch /etc/blacklist.d/1.2.3.4

This is 100% dynamic, and changes will be reflected immediately.

The overhead of this test is clear, a single `stat()` call for each
visitor.  In a low-traffic server which is not I/O bound this will
be harmless.


Compilation
-----------

Assume you have your Apache-development package(s) installed you
can compile via `apxs`:

    apxs2 -c mod_blacklist.c 

The `Makefile` does that for you.


Installation
------------

Create the file /etc/apache2/mods-enabled/blacklist.load:

     LoadModule blacklist_module /usr/lib/apache2/modules/mod_blacklist.so

If you wish to change the prefix create `blacklist.conf`:

     # Change the blacklist prefix.
     BlacklistPrefix /root/blacklist.d/


Steve
-- 



