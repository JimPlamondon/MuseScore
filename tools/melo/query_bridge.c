/* SPDX-License-Identifier: GPL-3.0-only */
/* Command-line adapter for fixture generators; musical facts stay in Rust. */
#include <stdio.h>
#include "melo_musescore_bridge.h"

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: query_bridge <JSON request>\n");
        return 2;
    }
    char *response = melo_musescore_bridge_request(argv[1]);
    puts(response);
    melo_musescore_bridge_free(response);
    return 0;
}
