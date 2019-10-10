# Ce projet lit les nanodacs dans toute la societe 
# 
# On importe les donnes dans la structure historique
# dans la table meca_datalog
#

Les fichiers executables sont posees dans /home/mecadmin/bin (make install)

En cas de modif :

cd /home/mecadmin/nanodac/
vi nanodac.c
make
tests en local : lancer ./nanodac
Si ok
git commit -am "ma nouvelle version"
git push
make install

# L'executable est lancee toutes les 10 minutes par la crontab 
# exemple ci-dessous
# variables d'environnement
PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
ORACLE_HOME=/usr/lib/oracle/12.2/client64
LD_LIBRARY_PATH=/usr/lib/oracle/12.2/client64/lib

# m h  dom mon dow   command
0,10,20,30,40,50 * * * * /home/mecadmin/bin/nanodac.sh > /dev/null 2>&1
