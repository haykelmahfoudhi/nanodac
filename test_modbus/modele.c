/*
 * Copyright © 2008-2010 Stéphane Raimbault <stephane.raimbault@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <modbus.h>

#include "unit-test.h"

enum {
    TCP,
    TCP_PI,
    RTU
};

int main(int argc, char *argv[])
{
    uint8_t *tab_rp_bits;
    uint16_t *tab_rp_registers;
    uint16_t *tab_rp_registers_bad;
    modbus_t *ctx;
    int i;
    uint8_t value;
    int nb_points;
    int rc;
    int addr_debut=0;
/*
    float real;
    struct timeval old_response_timeout;
    struct timeval response_timeout;
    int use_backend;  

    if (argc > 1) {
        if (strcmp(argv[1], "tcp") == 0) {
            use_backend = TCP;
	} else if (strcmp(argv[1], "tcppi") == 0) {
            use_backend = TCP_PI;
        } else if (strcmp(argv[1], "rtu") == 0) {
            use_backend = RTU;
        } else {
            printf("Usage:\n  %s [tcp|tcppi|rtu] - Modbus client for unit testing\n\n", argv[0]);
            exit(1);
        }
    } else {
        // By default 
        use_backend = TCP;
    }
*/
  //  if (use_backend == TCP) {
        //ctx = modbus_new_tcp("127.0.0.1", 1502);
        ctx = modbus_new_tcp("172.16.1.231", 502);
        //ctx = modbus_new_tcp("192.168.1.6", 502);
          modbus_set_slave(ctx, 1);
  //  } else if (use_backend == TCP_PI) {
  //      ctx = modbus_new_tcp_pi("::1", "1502");
//    } else {
  //      ctx = modbus_new_rtu("/dev/ttyUSB1", 115200, 'N', 8, 1);
  //  }
    if (ctx == NULL) {
        fprintf(stderr, "Unable to allocate libmodbus context\n");
        return -1;
    }
    modbus_set_debug(ctx, TRUE);
    modbus_set_error_recovery(ctx,
                              MODBUS_ERROR_RECOVERY_LINK |
                              MODBUS_ERROR_RECOVERY_PROTOCOL);
/*
    if (use_backend == RTU) {
          modbus_set_slave(ctx, SERVER_ID);
    }
*/
    if (modbus_connect(ctx) == -1) {
        fprintf(stderr, "Connection failed: %s\n",
                modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }

    /* Allocate and initialize the memory to store the bits */
//    nb_points = (UT_BITS_NB > UT_INPUT_BITS_NB) ? UT_BITS_NB : UT_INPUT_BITS_NB;
nb_points=100;
addr_debut=0;
//    tab_rp_bits = (uint8_t *) malloc(nb_points * sizeof(uint8_t));
//   memset(tab_rp_bits, 0, nb_points * sizeof(uint8_t));

    /* Allocate and initialize the memory to store the registers */
/*    nb_points = (UT_REGISTERS_NB > UT_INPUT_REGISTERS_NB) ?
        UT_REGISTERS_NB : UT_INPUT_REGISTERS_NB; */
nb_points=120;
addr_debut=0;
// LECTURE MOTS OK Didier
    tab_rp_registers = (uint16_t *) malloc(nb_points * sizeof(uint16_t));
    memset(tab_rp_registers, 0, nb_points * sizeof(uint16_t));

    printf("** UNIT TESTING **\n");

    printf("\nTEST WRITE/READ:\n");
    rc = modbus_read_registers(ctx,addr_debut ,
                               nb_points,tab_rp_registers);
    printf(" modbus_read_registers: ");
    if (rc != nb_points) {
        printf("FAILED (nb points %d)\n", rc);
    }
printf("-----------\n");
    for (i=0;i<nb_points;i++) {
        printf("MW%d : hex (%0X) = %d)\n",
               i+addr_debut,
               tab_rp_registers[i],
               tab_rp_registers[i]);
    }
    printf("OK\n");
printf("FIN Lecture Mots : \n");
    /* Free the memory */
    free(tab_rp_bits);
    free(tab_rp_registers);

    /* Close the connection */
    modbus_close(ctx);
    modbus_free(ctx);

    return 0;
}
