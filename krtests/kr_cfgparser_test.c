#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include "krutils/kr_cfgparser.h"

#define CONFIG_FILE "krproject.cfg"

int main()
{
    int ret = 0;
    
    FILE *fp;
    
    fp = fopen(CONFIG_FILE, "w");
    assert(fp != NULL);
    fprintf(fp, "[PEOPLE]\nNAME=Tiger\nAGE=26\nHEIGHT=173\nBIRTHDAY=19860830\n[EXAMPLE]\nSCORE=99.97\n");
    fclose(fp);
    
    ret = kr_config_setfile("krproject.cfg");
    assert(ret == SUCCESS);
    
    char caName[20];
    ret = kr_config_getstring("PEOPLE", "NAME", caName);
    assert(ret == SUCCESS);
    short hAge;
    ret = kr_config_getshort("PEOPLE", "AGE", &hAge);
    assert(ret == SUCCESS);
    int iHeight;
    ret = kr_config_getint("PEOPLE", "HEIGHT", &iHeight);
    assert(ret == SUCCESS);
    long lBirthday;
    ret = kr_config_getlong("PEOPLE", "BIRTHDAY", &lBirthday);
    assert(ret == SUCCESS);

    double dScore;
    ret = kr_config_getdouble("EXAMPLE", "SCORE", &dScore);
    assert(ret == SUCCESS);

    printf("caName=[%s], hAge[%d], iHeight[%d], lBirthday[%ld], dScore[%lf]\n",\
            caName, hAge, iHeight, lBirthday, dScore);
    
    remove(CONFIG_FILE);
    
    return 0;
}

