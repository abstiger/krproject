#!/usr/bin/env python
#-*- coding:utf-8 -*-
# author : Tiger
#

import os
import sys

from distutils.core import setup, Extension

os.environ["CFLAGS"] = "-Wno-strict-prototypes -std=c99"

install_path='/home/tiger/krproject/'

ext_krengine = Extension('_krengine',
        include_dirs = [os.path.join(install_path,'include')],
        library_dirs = [os.path.join(install_path,'lib')],
        libraries = ['krutils','dbsbas','dbsdbs','krshm','krcalc','krdb','krrule','krengine'],
        sources = ['krengine_module.c'])

setup(name = 'krengine-python',
      version = '1.0.0',
      description = 'krengine python interface',
      py_modules = ['krengine'],
      author = 'Tiger',
      author_email = 'huzai9999999990@163.com',
      url = 'http://absoltuetiger.com/krproject',
      ext_modules = [ext_krengine])

