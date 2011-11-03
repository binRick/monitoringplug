/**
 * Monitoring Plugin - check_rhc
 **
 *
 * check_rhc - Check a RedHat Cluster Suite by snmp.
 * Copyright (C) 2010 Marius Rieder <marius.rieder@durchmesser.ch>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * $Id$
 */


const char *progname  = "check_rhc";
const char *progvers  = "0.1";
const char *progcopy  = "2010";
const char *progauth = "Marius Rieder <marius.rieder@durchmesser.ch>";
const char *progusage = "-H <HOST>";

/* MP Includes */
#include "mp_common.h"
#include "snmp_utils.h"
/* Default Includes */
#include <getopt.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/* Library Includes */
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>

/* Global Vars */
const char *hostname = NULL;
int port = 0;

int main (int argc, char **argv) {
    /* Local Vars */
    int status;
    char *clustername;
    int clusterstatus;
    char *clusterstatusdesc;
    int clustervotes;
    int clusterquorum;
    int clusternodes;
    netsnmp_session *ss;

    /* OIDs to query */
    struct mp_snmp_query_cmd snmpcmd[] = {
        {{1,3,6,1,4,1,2312,8,2,1,0}, 11, ASN_OCTET_STR, (void *)&clustername},
        {{1,3,6,1,4,1,2312,8,2,2,0}, 11, ASN_INTEGER, (void *)&clusterstatus},
        {{1,3,6,1,4,1,2312,8,2,3,0}, 11, ASN_OCTET_STR, (void *)&clusterstatusdesc},
        {{1,3,6,1,4,1,2312,8,2,5,0}, 11, ASN_INTEGER, (void *)&clustervotes},
        {{1,3,6,1,4,1,2312,8,2,4,0}, 11, ASN_INTEGER, (void *)&clusterquorum},
        {{1,3,6,1,4,1,2312,8,2,7,0}, 11, ASN_INTEGER, (void *)&clusternodes},
        {{0}, 0, 0, 0},
    };

    /* Set signal handling and alarm */
    if (signal(SIGALRM, timeout_alarm_handler) == SIG_ERR)
        critical("Setup SIGALRM trap faild!");

    /* Process check arguments */
    if (process_arguments(argc, argv) != OK)
        unknown("Parsing arguments faild!");

    /* Start plugin timeout */
    alarm(mp_timeout);

    /* Init Net-SNMP */
    ss = mp_snmp_init();

    /* Query target agent */
    status = mp_snmp_query(ss, snmpcmd);
    switch (status) {
        case STAT_ERROR:
            unknown("Error in libnetsnmp");
        case STAT_TIMEOUT:
            critical("Timeout in libnetsnmp");
    }

    /* Finish Net-SNMP */
    mp_snmp_deinit();

    /* Perfdata */
    if (mp_showperfdata)
        perfdata_int("votes", clustervotes, "", clusterquorum, clusterquorum-1, 0, clusternodes);

    /* Output and return */
    if (clusterstatus < 2)
        ok("%s [%s]", clustername, clusterstatusdesc);
    if (clusterstatus < 16)
        warning("%s [%s]", clustername, clusterstatusdesc);
    critical("%s [%s]", clustername, clusterstatusdesc);
}

int process_arguments (int argc, char **argv) {
    int c;
    int option = 0;

    static struct option longopts[] = {
            MP_LONGOPTS_DEFAULT,
            MP_LONGOPTS_HOST,
            MP_LONGOPTS_PORT,
            SNMP_LONGOPTS,
            MP_LONGOPTS_TIMEOUT,
            MP_LONGOPTS_END
    };

    if (argc < 3) {
       print_help();
       exit(STATE_OK);
    }


    while (1) {
        c = getopt_long (argc, argv, MP_OPTSTR_DEFAULT"t:H:p:"SNMP_OPTSTR, longopts, &option);

        if (c == -1 || c == EOF)
            break;

        getopt_snmp(c);

        switch (c) {
            /* Default opts */
            MP_GETOPTS_DEFAULT
            /* Hostname opt */
            case 'H':
                getopt_host(optarg, &hostname);
                break;
            /* Port opt */
            case 'P':
                getopt_port(optarg, &port);
                break;
            /* Timeout opt */
            case 't':
                getopt_timeout(optarg);
                break;
        }
    }

    return(OK);
}

void print_help (void) {
    print_revision();
    print_revision_snmp();
    print_copyright();

    printf("\n");

    printf("This plugin check a RedHat Cluster Suite by snmp..");

    printf("\n\n");

    print_usage();

    print_help_default();

    print_help_snmp();
}

