python.exe ZenUnitPy\ZenUnitPy\RunZenUnitPyTestsWithCoverageAndLinting.py
if ($LastExitCode -ne 0) { exit $LastExitCode }
python.exe ZenUnitPy\ZenUnitPy\BuildZenUnitOrZenMock.py --zenunit-or-zenmock=ZenUnit --cmake-generator="Visual Studio 15 2017 Win64" --cmake-build-type=Debug --cmake-definitions=""
