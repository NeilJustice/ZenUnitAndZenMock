#!/bin/bash
set -e
CXX=/usr/bin/clang++ python3 ZenUnitPy/ZenUnitPy/BuildZenUnitOrZenMock.py --ZenUnitOrZenMock=ZenUnit --generator=Ninja --buildType=Release --definitions=""

