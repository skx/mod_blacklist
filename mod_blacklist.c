/**
 * mod_blacklist.c - Easily blacklist remote IP addresses.
 *
 * For every individaul IP address you wish to blacklist just
 * create a file. The overhead is one stat() call for each visitor.
 *
 * https://github.com/skx/mod_blacklist
 * https://git.steve.org.uk/skx/mod_blacklist
 *
 */

#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <syslog.h>
#include <errno.h>

#include "httpd.h"
#include "http_core.h"
#include "http_config.h"
#include "http_log.h"
#include "http_request.h"


/*
 * Default directory for our lookup-test.
 */
#define PREFIX_DIR "/etc/blacklist.d/"
static char *prefix_dir = NULL;


/*
 * Declare the module for Apache 2.x
 */
module AP_MODULE_DECLARE_DATA blacklist_module;


/*
 * Check the incoming request against our blacklist directory.
 */
static int access_checker(request_rec *r)
{
    int ret = OK;
    char filename[1024];
    struct stat s;

    /*
     * If we have enough details
     */
    if (r && r->useragent_ip)
    {
        /*
         * Build up a filename to test.
         */
        snprintf(filename, sizeof(filename) - 1, "%s/%s",
                 prefix_dir != NULL ? prefix_dir : PREFIX_DIR, r->useragent_ip);

        /*
         * If the file exists then the user will be denied.
         */
        if (stat(filename, &s) != -1)
        {
            ret = HTTP_FORBIDDEN;
        }
    }

    /*
     * However this might be changed.
     */
    if (ret == HTTP_FORBIDDEN
            && (ap_satisfies(r) != SATISFY_ANY || !ap_some_auth_required(r)))
    {
        ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, "mod_blacklist: %s exists - denying request %s", filename, r->filename);
    }

    return ret;
}


/*
 * Helper to allow the prefix to be changed in the Apache configuration
 */
static const char *
get_blacklist_prefix(cmd_parms *cmd, void *dconfig, const char *value)
{
    if (value != NULL && value[0] != 0)
    {
        if (prefix_dir != NULL)
            free(prefix_dir);

        prefix_dir = strdup(value);
    }

    return NULL;
}


/*
 * Register the configuration-options we support.
 */
static const command_rec access_cmds[] =
{
    AP_INIT_TAKE1("BlacklistPrefix", get_blacklist_prefix, NULL, RSRC_CONF,
    "Set prefix directory"),
    { NULL }
};

/*
 * Register our access-check.
 */
static void register_hooks(apr_pool_t *p)
{
    ap_hook_access_checker(access_checker, NULL, NULL, APR_HOOK_MIDDLE);
}

/*
 * Finally populate our module
 */
module AP_MODULE_DECLARE_DATA blacklist_module =
{
    STANDARD20_MODULE_STUFF,
    NULL,
    NULL,
    NULL,
    NULL,
    access_cmds,
    register_hooks
};
