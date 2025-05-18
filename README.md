## Laboratory work V

Данная лабораторная работа посвещена изучению фреймворков для тестирования на примере **GTest**

```sh
$ open https://github.com/google/googletest
```

## Tasks

- [ ] 1. Создать публичный репозиторий с названием **lab05** на сервисе **GitHub**
- [ ] 2. Выполнить инструкцию учебного материала
- [ ] 3. Ознакомиться со ссылками учебного материала
- [ ] 4. Составить отчет и отправить ссылку личным сообщением в **Slack**

## Tutorial

```sh
$ export GITHUB_USERNAME=<имя_пользователя>
$ alias gsed=sed # for *-nix system
```

```sh
$ cd ${GITHUB_USERNAME}/workspace
$ pushd .
$ source scripts/activate
```

```sh
$ git clone https://github.com/${GITHUB_USERNAME}/lab04 projects/lab05
$ cd projects/lab05
$ git remote remove origin
$ git remote add origin https://github.com/${GITHUB_USERNAME}/lab05
```

```sh
$ mkdir third-party
$ git submodule add https://github.com/google/googletest third-party/gtest
$ cd third-party/gtest && git checkout release-1.8.1 && cd ../..
$ git add third-party/gtest
$ git commit -m"added gtest framework"
```

```sh
$ gsed -i '/option(BUILD_EXAMPLES "Build examples" OFF)/a\
option(BUILD_TESTS "Build tests" OFF)
' CMakeLists.txt
$ cat >> CMakeLists.txt <<EOF

if(BUILD_TESTS)
  enable_testing()
  add_subdirectory(third-party/gtest)
  file(GLOB \${PROJECT_NAME}_TEST_SOURCES tests/*.cpp)
  add_executable(check \${\${PROJECT_NAME}_TEST_SOURCES})
  target_link_libraries(check \${PROJECT_NAME} gtest_main)
  add_test(NAME check COMMAND check)
endif()
EOF
```

```sh
$ mkdir tests
$ cat > tests/test1.cpp <<EOF
#include <print.hpp>

#include <gtest/gtest.h>

TEST(Print, InFileStream)
{
  std::string filepath = "file.txt";
  std::string text = "hello";
  std::ofstream out{filepath};

  print(text, out);
  out.close();

  std::string result;
  std::ifstream in{filepath};
  in >> result;

  EXPECT_EQ(result, text);
}
EOF
```

```sh
$ cmake -H. -B_build -DBUILD_TESTS=ON
$ cmake --build _build
$ cmake --build _build --target test
```

```sh
$ _build/check
$ cmake --build _build --target test -- ARGS=--verbose
```

```sh
$ gsed -i 's/lab04/lab05/g' README.md
$ gsed -i 's/\(DCMAKE_INSTALL_PREFIX=_install\)/\1 -DBUILD_TESTS=ON/' .travis.yml
$ gsed -i '/cmake --build _build --target install/a\
- cmake --build _build --target test -- ARGS=--verbose
' .travis.yml
```

```sh
$ travis lint
```

```sh
$ git add .travis.yml
$ git add tests
$ git add -p
$ git commit -m"added tests"
$ git push origin master
```

```sh
$ travis login --auto
$ travis enable
```

```sh
$ mkdir artifacts
$ sleep 20s && gnome-screenshot --file artifacts/screenshot.png
# for macOS: $ screencapture -T 20 artifacts/screenshot.png
# open https://github.com/${GITHUB_USERNAME}/lab05
```

## Report

```sh
$ popd
$ export LAB_NUMBER=05
$ git clone https://github.com/tp-labs/lab${LAB_NUMBER} tasks/lab${LAB_NUMBER}
$ mkdir reports/lab${LAB_NUMBER}
$ cp tasks/lab${LAB_NUMBER}/README.md reports/lab${LAB_NUMBER}/REPORT.md
$ cd reports/lab${LAB_NUMBER}
$ edit REPORT.md
$ gist REPORT.md
```

## Homework
Подготовим директорию для выполнения лабораторной работы №5
```
$ METEOPuT/workspace/tasks
$ git clone https://github.com/tp-labs/lab05
$ nano README.md
```
### Задание
1. Создайте `CMakeList.txt` для библиотеки *banking*.
```
$ cd banking
$ nano CMakeLists.txt
```
Содержимое файла:
```
cmake_minimum_required(VERSION 3.10)
project(banking)

add_library(banking STATIC Account.cpp Account.h Transaction.cpp Transaction.h)

target_include_directories(banking PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
```
2. Создайте модульные тесты на классы `Transaction` и `Account`.
    * Используйте mock-объекты.
    * Покрытие кода должно составлять 100%.
```
$ cd ..
$ mkdir third-party
$ git submodule add https://github.com/google/googletest third-party/gtest
$ cd third-party/gtest
$ git checkout release-1.8.1
$ cd ../../
$ git add third-party/gtest
$ git commit -m "Добавлен Google Test submodule"
$ nano CMakeLists.txt 
```
Содержимое корневого CMakeLists.txt:
```
cmake_minimum_required(VERSION 3.10)
project(lab05)

option(BUILD_TESTS "Build tests" OFF)

add_subdirectory(banking)

if(BUILD_TESTS)
    enable_testing()

    set(gtest_disable_warnings ON CACHE BOOL "" FORCE)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=maybe-uninitialized")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-error=maybe-uninitialized")

    add_subdirectory(third-party/gtest)

    file(GLOB LAB05_TEST_SOURCES tests/*.cpp)

    add_executable(check ${LAB05_TEST_SOURCES})
    target_link_libraries(check banking gtest_main gmock)

    target_include_directories(check PRIVATE banking)

    add_test(NAME check COMMAND check)
endif()
```
```
$ mkdir tests
$ cd tests
$ nano AccountTests.cpp
```
Содержимое файла AccountTests.cpp:
```
#include <gtest/gtest.h>
#include "Account.h"

TEST(AccountTest, ChangeBalanceIncreasesBalance) {
    Account acc(1, 0);
    acc.Lock();
    acc.ChangeBalance(100);
    EXPECT_EQ(acc.GetBalance(), 100);
    acc.Unlock();
}

TEST(AccountTest, ChangeBalanceDecreasesBalance) {
    Account acc(1, 0);
    acc.Lock();
    acc.ChangeBalance(100);
    acc.ChangeBalance(-30);
    EXPECT_EQ(acc.GetBalance(), 70);
    acc.Unlock();

}
```
```
$ cd ..
$ mkdir build
$ cd build
$ cmake -DBUILD_TESTS=ON ..
$ cmake --build .
$ ctest --verbose
UpdateCTestConfiguration  from :/home/kali/METEOPuT/workspace/tasks/lab05/build/DartConfiguration.tcl
UpdateCTestConfiguration  from :/home/kali/METEOPuT/workspace/tasks/lab05/build/DartConfiguration.tcl
Test project /home/kali/METEOPuT/workspace/tasks/lab05/build
Constructing a list of tests
Done constructing a list of tests
Updating test list for fixtures
Added 0 tests to meet fixture requirements
Checking test dependency graph...
Checking test dependency graph end
test 1
    Start 1: check

1: Test command: /home/kali/METEOPuT/workspace/tasks/lab05/build/check
1: Working Directory: /home/kali/METEOPuT/workspace/tasks/lab05/build
1: Test timeout computed to be: 10000000
1: Running main() from /home/kali/METEOPuT/workspace/tasks/lab05/third-party/gtest/googletest/src/gtest_main.cc
1: [==========] Running 2 tests from 1 test case.
1: [----------] Global test environment set-up.
1: [----------] 2 tests from AccountTest
1: [ RUN      ] AccountTest.ChangeBalanceIncreasesBalance
1: [       OK ] AccountTest.ChangeBalanceIncreasesBalance (0 ms)
1: [ RUN      ] AccountTest.ChangeBalanceDecreasesBalance
1: [       OK ] AccountTest.ChangeBalanceDecreasesBalance (0 ms)
1: [----------] 2 tests from AccountTest (0 ms total)
1: 
1: [----------] Global test environment tear-down
1: [==========] 2 tests from 1 test case ran. (0 ms total)
1: [  PASSED  ] 2 tests.
1/1 Test #1: check ............................   Passed    0.00 sec

100% tests passed, 0 tests failed out of 1

Total Test time (real) =   0.01 sec
```
3. Настройте сборочную процедуру на **TravisCI**.
```
$ cd ..
$ nano .gitignore
$ mkdir .github
$ mkdir .github/workflows
$ cd .github/workflows
$ nano build-and-test.yml
```
Содержимое файла build-and-test.yml:
```
name: Build and Test

on:
  push:
    branches:
      - master
  pull_request:

jobs:
  build:
    runs-on: ubuntu-latest

    steps:
      - name: Checkout repository
        uses: actions/checkout@v3

      - name: Install dependencies
        run: |
          sudo apt-get update
          sudo apt-get install -y build-essential cmake

      - name: Configure project with CMake
        run: cmake -S . -B build -DBUILD_TESTS=ON

      - name: Build project
        run: cmake --build build -- -j$(nproc)

      - name: Run tests
        run: ctest --test-dir build --output-on-failure

```
```
$ cd ../../
```
Загружаем на GitHub
```
$ git remote add origin https://github.com/METEOPuT/lab05.git
$ git add .
$ git commit -m "Добавлены файлы"
$ git push origin master
```
4. Настройте [Coveralls.io](https://coveralls.io/).

## Links

- [C++ CI: Travis, CMake, GTest, Coveralls & Appveyor](http://david-grs.github.io/cpp-clang-travis-cmake-gtest-coveralls-appveyor/)
- [Boost.Tests](http://www.boost.org/doc/libs/1_63_0/libs/test/doc/html/)
- [Catch](https://github.com/catchorg/Catch2)

```
Copyright (c) 2015-2021 The ISC Authors
```
