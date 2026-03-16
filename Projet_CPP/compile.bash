source env_robotique/bin/activate
rm ./run_tests

export PKG_CONFIG_PATH=$PWD/env_robotique/lib/python3.12/site-packages/cmeel.prefix/lib/pkgconfig:$PKG_CONFIG_PATH
g++ -std=c++17 test_joints.cpp -o run_tests $(pkg-config --cflags --libs eigen3 pinocchio) -lgtest -lgtest_main -pthread
export LD_LIBRARY_PATH=$PWD/env_robotique/lib/python3.12/site-packages/cmeel.prefix/lib:$LD_LIBRARY_PATH



g++ -std=c++17 test_bras.cpp -o run_tests_bras $(pkg-config --cflags --libs eigen3 pinocchio) -lgtest -lgtest_main -pthread
valgrind --leak-check=full --track-origins=yes ./run_tests_bras


g++ -std=c++17 simulateur.cpp -o simulateur $(pkg-config --cflags --libs eigen3 pinocchio)

./simulateur
