#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <modbus.h>

#include "unit-test.h"
//#define TCP_PORT_MODBUS 1502
#define TCP_PORT_MODBUS 502
/*------------------------------------------------------------------
printf des mots lus
-----------------------------------------------------------------*/
lit_mots_automate ( char * szIp, int start , int count)
{
modbus_t *ctx;
  uint16_t tab_reg[512];
  int rc;
  int i;

 uint16_t *tab_rp_registers;
   int nb_points;
    int addr_debut=0;

 
  memset (tab_reg,0,sizeof(tab_reg));
  ctx = modbus_new_tcp(szIp, TCP_PORT_MODBUS);
    if (ctx == NULL) {
        fprintf(stderr, "Unable to allocate libmodbus context\n");
        return -1;
    }
    modbus_set_debug(ctx, TRUE);
    modbus_set_error_recovery(ctx,
                              MODBUS_ERROR_RECOVERY_LINK |
                              MODBUS_ERROR_RECOVERY_PROTOCOL);


  if (modbus_connect(ctx) == -1) {
    fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
    modbus_free(ctx);
    return -1;
  }
  /* Read count registers from the address start */
  rc = modbus_read_registers(ctx, start, count, tab_reg);
  if (rc == -1 && errno == EMBXILADD) {
        printf("OK\n");
    } else {
        printf("FAILED\n");
   }

  if ( rc < count) {
      printf("FAILED %s (nb points %d)\n",szIp , rc);
      fprintf(stderr, "%d %s\n", errno, modbus_strerror(errno));
  } 


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


  modbus_close(ctx);
  modbus_free(ctx);
  

 for (i=0;i<count;i++) {
     printf("%s[%d]=%d\n","tab_reg",start+i,tab_reg[i]);
 } 
}

main ()
{
lit_mots_automate ("172.16.1.28",0,30);
lit_mots_automate ("172.16.1.231",0,30);
}
