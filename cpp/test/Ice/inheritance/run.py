#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2001
# MutableRealms, Inc.
# Huntsville, AL, USA
#
# All Rights Reserved
#
# **********************************************************************

import os, sys

for toplevel in ["", "..", os.path.join("..", ".."), os.path.join("..", "..", "..")]:
    if os.path.exists(os.path.join(toplevel, "config", "TestUtil.py")):
        break
else:
    raise "can't find toplevel directory!"

sys.path.append(os.path.join(toplevel, "config"))
import TestUtil

name = os.path.join("Ice", "inheritance")
TestUtil.clientServerTest(toplevel, name)
TestUtil.collocatedTest(toplevel, name)
sys.exit(1)
