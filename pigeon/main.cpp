#include <ncurses/ncurses.h>
#include <stdio.h>
#include <string>
#include <stdint.h>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <unistd.h>
#include <filesystem>
#include <vector>
#include <chrono>

void draw_ui(std::pair<std::string, std::vector<std::string>> prompt_with_options, int selection, bool refresh_prompt, int currprompt);
void draw_all_options(std::vector<std::string> options, int selection, int currprompt);
void draw_option(int x, int y, const char option[], bool selected);
void print_prompt(const char prompt[], bool delay);
void show_inventory();
void draw_logo();
void resolve_choice(int prompt_id, int choice_id);

std::vector<std::pair<std::string, std::vector<std::string>>> prompts = {
    {"At last! The experiment is ready. The serum bubbles in the vial, storm clouds spiral in the skies - the 4 years of biochemistry in the University of Glasgow were not in vain. Soon, you will become a superior being, a creature of ultimate power, rivaling none. You climb into a chamber, and buckle in. This is the moment you've been waiting for - but you are hesitant. Your hand hovers above the two buttons, 'ABORT' and 'BEGIN'. This choice will change your life, forever.", {"ABORT", "BEGIN"}}
};

bool gameover = false;
bool has_hairpin = false;
bool has_bread = false;
bool has_idcard = false;
bool pigeonlocked = false;
bool remain = false;

int main(int argc, char *argv[]){

    bool should_end = false;
    int ch;
    int selection = 0;
    int currprompt = 0;

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);

    attron(COLOR_PAIR(1));

    draw_ui(prompts[currprompt], selection, true, currprompt);

    while (!should_end) {
        ch = getch();
        switch (ch) {
            case KEY_LEFT:
            case KEY_UP:
                selection--;
                if (selection < 0) selection = prompts[currprompt].second.size()-1;
                clear();
                draw_ui(prompts[currprompt], selection, false, currprompt);
                break;
            case KEY_RIGHT:
            case KEY_DOWN:
                selection++;
                if (selection >= prompts[currprompt].second.size()) selection = 0;
                clear();
                draw_ui(prompts[currprompt], selection, false, currprompt);
                break;
            case 10:
                if (gameover) {should_end = true;}
                else {
                    resolve_choice(currprompt, selection);
                    currprompt++;
                    selection = 0;
                    clear();
                    draw_ui(prompts[currprompt], selection, true, currprompt);
                }
                break;
            case 113:
            case 81:
                should_end = true;
                break;
            default:
                break;
        }
    }
    
    attroff(COLOR_PAIR(1));
    
    endwin();
}

void draw_ui(std::pair<std::string, std::vector<std::string>> prompt_with_options, int selection, bool refresh_prompt, int currprompt){
    int len = 0;

    for (int i = 0; i < COLS; i++) {
        mvprintw(0, i, "=");
        mvprintw(LINES-1, i, "=");
    }

    show_inventory();

    draw_logo();

    print_prompt(prompt_with_options.first.c_str(), refresh_prompt);

    draw_all_options(prompt_with_options.second, selection, currprompt);
}

void draw_all_options(std::vector<std::string> options, int selection, int currprompt) {
    int len = options.size();
    int optionlen = 15;
    if (currprompt == 9) {optionlen = 25;}
    int height = LINES-5;

    int mid_crd = COLS/2;
    int mid_opt = len/2;

    if (len == 1) {
        draw_option(mid_crd, height, options[0].c_str(), true);
    }
    else {
        for (int i = 0; i < len; i++) {
            int curr_crd = i - mid_opt;

            std::string option = options[i];
            
            if (selection == i) {
                draw_option(curr_crd*optionlen + mid_crd + 5, height, option.c_str(), true);
            }
            else {
                draw_option(curr_crd*optionlen + mid_crd + 5, height, option.c_str(), false);
            }
        }
    }
}

void draw_option(int x, int y, const char option[], bool selected) {
    int len = strlen(option);

    int startx = x - (len/2);
    int starty = y;

    if (selected) {
        attron(A_REVERSE);
    }

    mvprintw(starty, startx, "[%s]", option);

    if (selected) {
        attroff(A_REVERSE);
    }
}

void print_prompt(const char prompt[], bool delay) {
    int len = strlen(prompt);
    int maxline = COLS - 30;
    int currsym = 0;
    int lineoffset = LINES-15;
    int linestart = 10;

    move(lineoffset, linestart);
    for (int i = 0; i < len; i++) {
        if (delay) {
            refresh();
            usleep(1000);
        }
        if (prompt[i] == ' ') {
            if (currsym > maxline) {
                lineoffset++;
                move(lineoffset, linestart);
                currsym = 0;
            }
            else {
                printw("%c", prompt[i]);
                currsym++;
            }
        }
        else {
            printw("%c", prompt[i]);
            currsym++;
        }
    }
}

void show_inventory() {
    mvprintw(5, 5, "%s", "Inventory: ");
    if (has_bread) {
        printw("%s", "A piece of Bread");
    }
    if (has_hairpin) {
        printw("%s", "A Hairpin");
    }
    if (has_idcard) {
        printw("%s", ", An ID Card");
    }
}

void draw_logo() {
    mvprintw(LINES-5, COLS-7, "%s", "  />");
    mvprintw(LINES-4, COLS-7, "%s", "_/D ");
    mvprintw(LINES-3, COLS-7, "%s", " \"  ");
}

void resolve_choice(int prompt_id, int choice_id) {
    switch (prompt_id) {
        case 0:
            if (choice_id == 0) {
                prompts.push_back({"This is too much. It's stupid anyways - no way the experiment would have succeeded. You climb out of the chamber, and shut the machine down for good. The end.", {"END GAME"}});
                gameover = true;
            }
            else {
                prompts.push_back({"There is no time for hesitation. You slam the button, and the chamber closes. With a sonic crack, lightning strikes the antenna on top of the Boyd Orr, powering your device. The serum is vaporised in thin, pink, sweet mist, which begins filling the chamber. You can feel your body changing, rapidly - and irreversibly. Another lightning strike - this one you did not account for. The machine begins overheating, the mist now changing colour and taste, becoming thicker, bitter, and blue. The explosion is imminent, as you reach for the ABORT button - but it is too late.", {"CONTINUE"}});
            }
            break;
        case 1:
            prompts.push_back({"You open your eyes. You are lying on the ground, in Kelvingrove Park. The explosion must have flung you very far! You look around - the world is different. Or is it? No, YOU are different. You can see more, your field of vision - almost 360 degrees. You feel light, almost as if you can fly. Truly - the experiment was a success! You hear a low rumbling coming from behind you, and as you turn around - you see a large dog, running towards you.", {"COMMAND", "FLEE"}});
            break;
        case 2:
            if (choice_id == 0) {
                prompts.push_back({"Your newly found superior powers - it's time to test them out. You extend your hand forward, and speak a command word - 'HALT THY FOUL BEAST'. But... your voice sounds small, and as you try to speak those words, the only thing that comes out is 'coo'. Hold on, you are not a superior being - you are a pigeon! Alas, this realisation comes too late, and the massive canine, three times the size of you, pounces at you with its maw open. The end.", {"END GAME"}});
                gameover = true;
            }
            else {
                prompts.push_back({"Perhaps, this dog is an opponent you are not yet ready to take on, so you turn your back, and begin running, faster and faster, flailing your arms- no, your wings, as you take into the sky. The barking grows quieter, and soon you land on a tree. The view from here is stunning. Green grass, vibrant trees, blue sky, a pigeon staring at you from the reflection in the pond... Hold on, you are not a superior being - you are a pigeon! The experiment... that second lighting must have overloaded the parameters. This was not the plan! You think you can reverse the process... but it won't be easy.", {"CONTINUE"}});
            }
            break;
        case 3:
            prompts.push_back({"You hear a duck call out to you from the pond: 'Hey you, pigeon! It's my birthday today, and I am feeling generous, so I will grant you a gift of your choosing. Now choose: a hairpin, or a piece of bread?'", {"HAIRPIN", "BREAD"}});
            break;
        case 4:
            if (choice_id == 0) {
                prompts.push_back({"You have obtained a hairpin! Perhaps, it will help you further down the line...", {"CONTINUE"}});
                has_hairpin = true;
            }
            else {
                prompts.push_back({"You have obtained a piece of bread! Perhaps, it will help you further down the line...", {"CONTINUE"}});
                has_bread = true;
            }
            break;
        case 5:
            prompts.push_back({"Now, to revert the process, first you must get access back to your laboratory, so you need an ID card. Luckily, you see that some professors and students are taking a stroll in the park right now. Perhaps, you can get it from them... You see four individuals: Individual number one, is a tall Computing Science professor, in a white sweater. He looks joyful. Individual number two, is a Computing Science student. He looks oblivious to his surroundings. Individual number three, is a security guard. He looks tough, and vigilant. Individual number four, is another pigeon. Who do you approach?", {"INDIVIDUAL 1", "INDIVIDUAL 2", "INDIVIDUAL 3", "INDIVIDUAL 4"}});
            break;
        case 6:
            choice_id;
            switch (choice_id) {
                case 0:
                    prompts.push_back({"You approach the professor. He looks down: 'Hello little pigeon! How can I help you?' You awkwardly ask him for his staff card, and it seems like he understands you! He hands you his card, which reads 'Professor Jeremy Singer'. He asks you to bring it back later, as he has already lost a card last month. You have obtained an ID card!", {"CONTINUE"}});
                    has_idcard = true;
                    break;
                case 1:
                    prompts.push_back({"You approach the student from behind. He doesn't seem to notice you! With finesse, you pull out his student card from his back pocket. You have obtained an ID card!", {"CONTINUE"}});
                    has_idcard = true;
                    break;
                case 2:
                    prompts.push_back({"You approach the security guard, stealthily. His staff card is right there! You reach for it, grabbing it with your beak. As you start pulling back, you notice that the card is on a string, and the guard notices that someone is trying to steal his staff card! He shoos you away. You have failed to obtain an ID card!", {"CONTINUE"}});
                    has_idcard = false;
                    break;
                case 3:
                    if (has_bread) {
                        prompts.push_back({"You approach the pigeon. The pigeon hears your story, doesn't believe you - but is willing to help, for a price, of course. He asks if you have any bread. You share some of the bread given to you by the duck with the pigeon, and he gives you a staff card he stole off of a professor last month. You have obtained an ID card!", {"CONTINUE"}});
                        has_idcard = true;
                    }
                    else {
                        prompts.push_back({"You approach the pigeon. The pigeon hears your story, doesn't believe you - but is willing to help, for a price, of course. He asks if you have any bread. You have nothing to trade! You have failed to obtain an ID card!", {"CONTINUE"}});
                        has_idcard = false;
                    }
                    break;
            }
            break;
        case 7:
            if (has_idcard) {
                prompts.push_back({"The time is running short: if the janitors find your machine before you can get back to it, and shut it down - you might never be able to turn back again. You fly, fast as the wind, and reach Boyd Orr building - a stone obelisk, rising way too high for such a small pigeon like you. With an ID Card you stroll right inside: the students are amused, but you have an ID Card, surely you are allowed in then. You take the elevator to the 8th floor, and enter your laboratory. Your machine is almost out of power, but it has enough charge to undo your transformation.", {"CONTINUE"}});
            }
            else if (has_hairpin) {
                prompts.push_back({"The time is running short: if the janitors find your machine before you can get back to it, and shut it down - you might never be able to turn back again. You fly, fast as the wind, and reach Boyd Orr building - a stone obelisk, rising way too high for such a small pigeon like you. You have no ID Card, so you cannot go inside: you must go through the window. You take off, flying higher and higher, fourth floor, sixth floor, seventh floor, and with the last power that you have - you land on the windowsill of the eighth floor, just outside of your lab. The window is locked. But you remember the hairpin given to you by the duck! You masterfully pick the lock on the window and fly inside. Your machine is almost out of power, but it has enough charge to undo your transformation.", {"CONTINUE"}});
            }
            else {
                prompts.push_back({"The time is running short: if the janitors find your machine before you can get back to it, and shut it down - you might never be able to turn back again. You fly, fast as the wind, and reach Boyd Orr building - a stone obelisk, rising way too high for such a small pigeon like you. You have no ID Card, so you cannot go inside: you must go through the window. You take off, flying higher and higher, fourth floor, sixth floor, seventh floor, and with the last power that you have - you land on the windowsill of the eighth floor, just outside of your lab. The window is locked. You are powerless - your beak not strong enough to break the glass, your talons not sturdy enough to pick the lock. If only you had a hairpin or a staff card...", {"CONTINUE"}});
                pigeonlocked = true;
            }
            break;
        case 8:
            if (!pigeonlocked) {
                prompts.push_back({"Once more you climb into a chamber, and buckle in. This is the moment you've been waiting and fighting for - but you are hesitant. Your beak hovers above the two buttons, 'ABORT' and 'BEGIN'. You've only been a pigeon for a day, but you felt free - free to fly anywhere, free to do anything. And you made a duck friend too! If you choose to go back, you will never feel like that ever again...", {"REMAIN A PIGEON", "REVERT THE EXPERIMENT"}});
            }
            else {
                prompts.push_back({"Maybe you are stuck as a pigeon forever - but maybe it's not so bad? You've only been a pigeon for a day, but you felt free - free to fly anywhere, free to do anything. And you made a duck friend too! You return to the Kelvingrove Park, and remain in the pond with your duck friend! The end.", {"END GAME"}});
                gameover = true;
            }
            break;
        case 9:
            if (choice_id == 0) {
                prompts.push_back({"Being a pigeon has its perks! Perhaps, you have become a superior being you dreamed of becoming! You climb out of the chamber, and fly back out, to your duck friend, to hang out near the pond in the Kelvingrove Park! The End.", {"END GAME"}});
                gameover = true;
            }
            else {
                prompts.push_back({"No, this avian life is not for you. You peck the button, and once more you are engulfed by a pink mist. You climb out of the chamber, your limbs no longer that of a bird, your words no longer coos. You are free! The university learns about your experimentation and pulls all the funding, so you can never repeat the experiment again. Sometimes you have this vivid dream of you, flying over the Kelvingrove Park - but it is only a dream. The End.", {"END GAME"}});
                gameover = true;
            }
            break;
    }
}