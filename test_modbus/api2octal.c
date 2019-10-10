#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <modbus.h>

//#define TCP_PORT_MODBUS 1502
#define TCP_PORT_MODBUS 502
#define DEBUG_MODBUS FALSE

// definit le format d'affichage des mots
#define STRMOTS "MW%d=%d\n"
#define MAX_MOTS_LUS 123

typedef struct mots {
    char *txt;
    char *unit;
} t_mots ;
t_mots amots[256];


/*------------------------------------------------------------------
  * printf des mots lus
  * retourne le nombre de mots lus ou -1 si erreur
  * parametres : ip automate, mot de départ , nombre de mots a lire
-----------------------------------------------------------------*/
int lit_mots_automate ( char * szIp, short int start , short int count)
{
  uint16_t tab_reg[MAX_MOTS_LUS];
  int rc;
  int i;
  modbus_t *ctx;
  time_t t;
  char szt[20];
  float fval;

   memset (tab_reg,0,sizeof(tab_reg));
   ctx = modbus_new_tcp(szIp, TCP_PORT_MODBUS);
   if (ctx == NULL) {
       fprintf(stderr, "Unable to allocate libmodbus context\n");
       return -1;
   }
   modbus_set_slave(ctx, 1);
   modbus_set_debug(ctx, DEBUG_MODBUS);
   modbus_set_error_recovery(ctx,
                              MODBUS_ERROR_RECOVERY_LINK |
                              MODBUS_ERROR_RECOVERY_PROTOCOL);


  if (modbus_connect(ctx) == -1) {
       fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
       modbus_free(ctx);
       return -1;
  }
  if (count > MAX_MOTS_LUS) {
      count = MAX_MOTS_LUS;
  }
   
  /* Read count registers from the address start */
  rc = modbus_read_registers(ctx, start, count, tab_reg);
  if (rc == -1 && errno == EMBXILADD) {
        fprintf(stderr,"FAILED %d %d %s \n",rc,errno,modbus_strerror(errno));
        return -2;
   } 
 
   t = time (NULL); 
   strftime (szt,20,"%Y-%m-%d %H:%M:00",localtime(&t));
   for (i=0;i < rc;i++) {
       // printf(STRMOTS,start+i,tab_reg[i]);
       if (amots[i].txt) {
           if (! strcmp (amots[i]. unit , "°C") ) {
               fval = tab_reg[i]/10.0; 
           } else {
               fval = tab_reg[i];
           }
 
           printf ("%s;;%s;%5.2f;\n",szt,amots[i].txt,fval);

       }
   }   

  modbus_close(ctx);
  modbus_free(ctx);
  
  return rc;
}

/*------------------------------------------------------------------
------------------------------------------------------------------*/
init_aciers ()
{
int i=0;

memset( amots,0,sizeof(amots));
i=9;

amots[i].txt = "M168_PALAN";
amots[i].unit = "";
i++;
amots[i].txt = "M168_D1";
amots[i].unit = "°C";
i++;
amots[i].txt = "M168_D2";
amots[i].unit = "°C";
i++;
amots[i].txt = "M168_D2_5";
amots[i].unit = "°C";
i++;
amots[i].txt = "M168_D3";
amots[i].unit = "°C";
i++;
amots[i].txt = "M168_D7";
amots[i].unit = "°C";
i++;
amots[i].txt = "M168_D8";
amots[i].unit = "°C";
i++;
amots[i].txt = "M168_D10";
amots[i].unit = "°C";
i++;
amots[i].txt = "M168_D11";
amots[i].unit = "°C";
i++;
amots[i].txt = "M168_D12";
amots[i].unit = "°C";
i++;
amots[i].txt = "M168_D18";
amots[i].unit = "°C";
i++;
amots[i].txt = "M168_D30";
amots[i].unit = "°C";
i++;
amots[i].txt = "M168_D34";
amots[i].unit = "°C";
i++;
amots[i].txt = "M168_D35";
amots[i].unit = "°C";
i++;
amots[i].txt = "M168_D36";
amots[i].unit = "°C";
i++;
amots[i].txt = "M168_D37";
amots[i].unit = "°C";
i++;

i+=3;
amots[i].txt = "M168_D17";
amots[i].unit = "°C";
i++;

}


/*------------------------------------------------------------------
------------------------------------------------------------------*/
init_oac ()
{
int i=0;

memset( amots,0,sizeof(amots));
i=10;

amots[i].txt = "M158_E101";
amots[i].unit = "°C";
i++;
amots[i].txt = "M158_E102";
amots[i].unit = "°C";
i++;
amots[i].txt = "M158_E105";
amots[i].unit = "°C";
i++;
amots[i].txt = "M158_E108";
amots[i].unit = "°C";
i++;
amots[i].txt = "M158_E109";
amots[i].unit = "°C";
i++;
amots[i].txt = "M158_E110";
amots[i].unit = "°C";
i++;
amots[i].txt = "M158_E111";
amots[i].unit = "°C";
i++;
amots[i].txt = "M158_E117";
amots[i].unit = "°C";
i++;
amots[i].txt = "M158_E118";
amots[i].unit = "°C";
i++;
amots[i].txt = "M158_E119";
amots[i].unit = "°C";
i++;
}

/*------------------------------------------------------------------
  * printf des mots lus
  * retourne le nombre de mots lus ou -1 si erreur
  * parametres : ip automate, mot de départ , nombre de mots a lire
  ------------------------------------------------------------------*/

main ()
{
init_aciers ();
lit_mots_automate ("172.16.1.28",0,124);
init_oac ();
lit_mots_automate ("172.16.1.231",0,124);

}
