#include "krutils/kr_utils.h"
#include "dbs/dbs_basopr.h"
#include "dbs/dbs/set_cfg_ins.h"

int main(int argc,char *argv[])
{
    T_DbsEnv *dbsenv = dbsConnect(\
            getenv("DBNAME"), getenv("DBUSER"), getenv("DBPASS"));
    if (dbsenv == NULL) {
        fprintf(stderr, "dbsDbConnect Failed!");
        return -1;
    }

    int ret = 0;
    T_SetCfgIns stSetCfgIns = {0};
    stSetCfgIns.lInSetId = 1;
    strcpy(stSetCfgIns.caInElementValue, "haha");
    strcpy(stSetCfgIns.caInElementDesc, "hehe");
    ret = dbsSetCfgIns(dbsenv, &stSetCfgIns);
    if (ret != 0) {
        fprintf(stderr, "insert bad merchant[%s] failed[%d] [%d][%s][%s]\n",
                stSetCfgIns.caInElementValue, ret, 
                dbsenv->sqlcode, dbsenv->sqlstate, dbsenv->sqlerrmsg);
        dbsRollback(dbsenv);
    }
    dbsCommit(dbsenv);
    fprintf(stdout, "insert bad merchant[%s]\n", stSetCfgIns.caInElementValue);
    
    dbsDisconnect(dbsenv);
    return 0;
}


