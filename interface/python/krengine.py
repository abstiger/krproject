#!/usr/bin/env python
#-*- coding:utf-8 -*-
# author : Tiger
#

import os
import _krengine

class KREngine(object):

    def __init__(self, dsn=os.environ.get('DBNAME'), user=os.environ.get('DBUSER'), passwd=os.environ.get('DBPASS'), logpath='.', logname='krengine', loglevel=5, shmkey=os.environ.get('KR_SHMKEY'), krdbname = 'KRDB', dbmodulename=' ', hdicachesize=0, threadcnt=0):
        self.engine=_krengine.startup(dsn, user, passwd, logpath, logname, loglevel, shmkey, krdbname, dbmodulename, hdicachesize, threadcnt)

    def run(self, oprcode, datasrc, msgbuf):
        return _krengine.run(self.engine, oprcode, datasrc, msgbuf)

    def shutdown(self):
        return _krengine.shutdown(self.engine)

    def info(self):
        return _krengine.info(self.engine)
   
