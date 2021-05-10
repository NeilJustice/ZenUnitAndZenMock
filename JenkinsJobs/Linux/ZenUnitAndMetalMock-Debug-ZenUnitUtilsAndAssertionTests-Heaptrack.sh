#!/bin/bash
set -ev

export PYTHONPATH=.
export CXX=/usr/bin/clang++
python ZenUnitDevOpsPython/BuildAndTestZenUnitAndMetalMock.py --cmake-generator=Ninja --cmake-build-type=Debug --cmake-definitions=""
heaptrack Debug/ZenUnitUtilsAndAssertionTests/ZenUnitUtilsAndAssertionTests --random-seed=0
