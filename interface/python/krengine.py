#!/usr/bin/env python
#-*- coding:utf-8 -*-
# author : Tiger
#

import _krengine

class KREngine(object):

    def __init__(self, shmkey, dbname = 'KRDB', hdicachesize=0, threadcnt=0):
        _krengine.external_startup('.','krengine',5)
        self.engine=_krengine.startup(shmkey, dbname, hdicachesize, threadcnt)

    def run(self, oprcode, datasrc, msgbuf):
        return _krengine.run(self.engine, oprcode, datasrc, msgbuf)

    def shutdown(self):
        return _krengine.shutdown(self.engine)

    def info(self):
        return _krengine.info(self.engine)
   
