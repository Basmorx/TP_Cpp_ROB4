rm ./run_tests
g++ -std=c++17 test_joints.cpp -o run_tests $(pkg-config --cflags --libs eigen3 pinocchio) -lgtest -lgtest_main -pthread
