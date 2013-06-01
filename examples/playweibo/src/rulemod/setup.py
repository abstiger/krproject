#!/usr/bin/env python
#-*- coding:utf-8 -*-
# author : Tiger
#

import os
import sys

from distutils.core import setup, Extension

os.environ["CFLAGS"] = "-Wno-strict-prototypes -std=c99"

ext_krengine = Extension('playweibo_rule_func',
        sources = ['playweibo_rule_func.c'])

setup(name = 'playweibo_rule_module',
      version = '1.0.0',
      description = 'playweibo rule module',
      author = 'Tiger',
      author_email = 'huzai9999999990@163.com',
      url = 'http://absoltuetiger.com/krproject',
      ext_modules = [ext_krengine])

