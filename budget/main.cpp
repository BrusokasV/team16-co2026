#include <stdio.h>
#include <iostream>
#include <chrono>
#include <thread>

int main(int argc, char *argv[]){

    int balance, tmp;
    char tmpchr;

    std::cout << "Please input GUTS current budget: " << std::endl;
    std::cin >> balance;

    std::cout << "Please input how much you wish to spend on 'DO YOU HAVE THE GUTS?': " << std::endl;
    std::cin >> tmp;
    balance -= tmp;

    std::cout << "Please input how much you wish to spend on 'Code Olympics': " << std::endl;
    std::cin >> tmp;
    balance -= tmp;

    std::cout << "Please input how much you wish to spend on Merch: " << std::endl;
    std::cin >> tmp;
    balance -= tmp;

    std::cout << "Do you think JP Morgan will sponsor the next event? [y/n]" << std::endl;
    std::cin >> tmpchr;
    if (tmpchr == 'y') {
        balance += 1000;
    }

    std::cout << "Do you think Morgan Stanley will sponsor the next event? [y/n]" << std::endl;
    std::cin >> tmpchr;
    if (tmpchr == 'y') {
        balance += 1000;
    }

    std::cout << "Do you think SAS will sponsor the next event? [y/n]" << std::endl;
    std::cin >> tmpchr;
    if (tmpchr == 'y') {
        balance += 1000;
    }

    std::cout << "Computing..." << std::endl;
    for (int i = 0; i < 20; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << i*5 << "%" << std::endl;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    std::cout << "Segmentation Fault (core dumped)" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    std::cout << "Kidding lol" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    if (balance > 0) {
        std::cout << "You are in the green zone, buy more ducks." << std::endl;
    }
    else {
        std::cout << "Funds ain't looking good, request SOCS funding ASAP." << std::endl;
    }
}