import os
import platform
import unittest
from unittest.mock import patch
from ZenUnitPy import CMake, Process, UnitTester

testNames = [
'generate_CreatesAndCdsToDirectory_RunsCMakeWithGeneratorAndBuildType_test' ]

class CMakeTests(unittest.TestCase):

   def generate_CreatesAndCdsToDirectory_RunsCMakeWithGeneratorAndBuildType_test(self):
      @patch('os.makedirs', spec_set=True)
      @patch('os.chdir', spec_set=True)
      @patch('platform.system', spec_set=True)
      @patch('builtins.print', spec_set=True)
      @patch('ZenUnitPy.Process.run', spec_set=True)
      def testcase(
         platformSystem, cmakeDefinitions, expectedCMakeCommand,
         _1, _2, _3, _4, _5):
         platform.system.return_value = platformSystem
         FolderPath = 'FolderPath'
         Generator = 'Generator'
         BuildType = 'BuildType'
         CMakeListsFolderPath = 'CMakeListsFolderPath'
         #
         CMake.generate(
            FolderPath, Generator, BuildType, cmakeDefinitions, CMakeListsFolderPath)
         #
         os.makedirs.assert_called_once_with(FolderPath, exist_ok=True)
         os.chdir.assert_called_once_with(FolderPath)
         print.assert_called_once_with('Generating CMake in folder', FolderPath)
         Process.run.assert_called_once_with(expectedCMakeCommand)
      testcase('Linux', '',
         'cmake -G"Generator" -DCMAKE_BUILD_TYPE=BuildType  CMakeListsFolderPath')
      testcase('Linux', '-DAddressSanitizerMode=ON',
         'cmake -G"Generator" -DCMAKE_BUILD_TYPE=BuildType -DAddressSanitizerMode=ON CMakeListsFolderPath')
      testcase('linux', '',
         'cmake -G"Generator"  CMakeListsFolderPath')
      testcase('Windows', '',
         'cmake -G"Generator"  CMakeListsFolderPath')
      testcase('Windows', '-DCMAKE_INSTALL_PREFIX=C:/install',
         'cmake -G"Generator" -DCMAKE_INSTALL_PREFIX=C:/install CMakeListsFolderPath')

if __name__ == '__main__': # pragma nocover
   UnitTester.run_tests(CMakeTests, testNames)
