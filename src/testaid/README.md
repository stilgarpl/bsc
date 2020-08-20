# bsc::parser

## Purpose

bsc::testaid is a small, simple library with classes that help manage resources in unit tests, using C++17 std::filesystem.

## Features

* Reading files as strings

```cpp
auto content = readFile("/path/to/file");
```

* Writing string to file
```cpp
auto content = "text";
createFile("/path/to/file",content);
```

* RAII path cleanup (provided path will be deleted when going out of scope)
```cpp
{
Cleanup c("/path/to/cleanup");
//some other code that will make temporary things in that path that you want to delete after leaving this scope
}
```

* Automatic management of test folders

    * TestDir
       
       This class will create a random folder that you can put your files during test. After the test that folder will be deleted (using Cleanup)
        ```cpp
      const std::string filename    = "file.txt";
      const std::string fileContent = "file content";
      {
           TestDir testDir;
           testPath = testDir.getTestDirPath();
           createFile(testPath / filename, fileContent);
           auto readContent = readFile(testPath / filename);
      }
        ```
      
    * TestDirWithResources
        This class will do everything TestDir does and will also give you access to resource files (just like in Java). It's constructor takes one parameter - path to resource folder (which defaults to "resources", assuming working directory is the one with such folder). 
        It will copy all the resource will, so any changes to them will not affect the actual resource folder. If you only want access to resources, but folder is too big to copy, use constructor parameter to only copy one subfolder or use `Resources` class that will provide that access. 
        Assuming following folder structure:
        ```
        resources
        |- testFile1.txt
        |- testFile2.csv
        ```
        You will have access to those files like this:
        ```cpp
        {
        TestDirWithResources testDirWithResources;
        testPath = testDirWithResources.getResourcePath();
      
        fs::exists(testPath);
        fs::exists(testPath / "testFile1.txt");
        fs::exists(testPath / "testFile2.csv");
        }  
        ```