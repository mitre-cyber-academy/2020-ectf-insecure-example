/*
 * eCTF Collegiate 2020 miPod Example Code
 * Linux-side DRM driver
 */


#include "miPod.h"

#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <linux/gpio.h>
#include <string.h>
//c++ includes
#include <iostream.h>
#include <bits/stdc++.h> 
#include <boost/algorithm/string.hpp>

//change headerfile so that all structs use std::string instead of char[] or char*
volatile cmd_channel *c;


//////////////////////// UTILITY FUNCTIONS ////////////////////////


// sends a command to the microblaze using the shared command channel and interrupt
void send_command(int cmd) {
    memcpy((void*)&c->cmd, &cmd, 1); //replace with memcpy_s

    //trigger gpio interrupt
    system("devmem 0x41200000 32 0"); //reconsider the use of the system command 
    system("devmem 0x41200000 32 1"); //reconsider the use of the system command
}


// parses the input of a command with up to two arguments
// any arguments not present will be set to NULL
//void parse_input(char *input, char **cmd, char **arg1, char **arg2) {
//    *cmd = strtok(input, " \r\n");
//    *arg1 = strtok(NULL, " \r\n");
//    *arg2 = strtok(NULL, " \r\n");
//}

void parse_input(std::string& input, std::string[]& cmd, std::string& arg1, std::string& arg2) {
    boost::split(cmd, input, boost::is_any_of(" \r\n")); //not sure if I need a vector
}


// prints the help message while not in playback
//void print_help() {
//    mp_printf("miPod options:\r\n");
//    mp_printf("  login <username> <pin>: log on to a miPod account (must be logged out)\r\n");
//    mp_printf("  logout: log off of a miPod account (must be logged in)\r\n");
//    mp_printf("  query <song.drm>: display information about the song\r\n");
//    mp_printf("  share <song.drm> <username>: share the song with the specified user\r\n");
//    mp_printf("  play <song.drm>: play the song\r\n");
//    mp_printf("  digital_out <song.drm>: play the song to digital out\r\n");
//    mp_printf("  exit: exit miPod\r\n");
//    mp_printf("  help: display this message\r\n");
//}

//done
void print_help() {
    //cout is more secure than a printf variant because cout doesn't have format specifiers
    std::cout << "miPod options:\r\n"; 
    std::cout << "  login<username> <pin>: log on to a miPod account (must be logged out)\r\n";
    std::cout << "  logout: log off a miPod account (must be logged in)\r\n";
    std::cout << "  query <song.drm>: display information about the song\r\n";
    std::cout << "  share <song.drm>: <username>: share the song with the specified user\r\n";
    std::cout << "  play <song.drm>: play the song\r\n");
    std::cout << "  exit: exit miPod\r\n");
    std::cout << "  help: display this message\r\n");
}


// prints the help message while in playback
//void print_playback_help() {
//    mp_printf("miPod playback options:\r\n");
//    mp_printf("  stop: stop playing the song\r\n");
//    mp_printf("  pause: pause the song\r\n");
//    mp_printf("  resume: resume the paused song\r\n");
//    mp_printf("  restart: restart the song\r\n");
//    mp_printf("  ff: fast forwards 5 seconds(unsupported)\r\n");
//    mp_printf("  rw: rewind 5 seconds (unsupported)\r\n");
//    mp_printf("  help: display this message\r\n");
//}

//done
void print_playback_help() {
    std::cout << "miPod playback options:\r\n");
    std::cout << "  stop: stop playing the song\r\n");
    std::cout << "  pause: pause the song\r\n");
    std::cout << "  resume: resume the paused song\r\n");
    std::cout << "  restart: restart the song\r\n");
    std::cout << "  ff: fast forwards 5 seconds(unsupported)\r\n"); // we probably shouldn't print this 
    std::cout << "  rw: rewind 5 seconds (unsupported)\r\n"); // we probably shoudln't print this
    std::cout << "  help: display this message\r\n");
}


//done
// loads a file into the song buffer with the associate
// returns the size of the file or 0 on error
size_t load_file(char *fname, char *song_buf) {
    int fd;
    struct stat sb;

    fd = open(fname, O_RDONLY); //consider using c++ file i/o with destructor
    if (fd == -1){
        std::cerr << "Failed to open file! Error = " << (errno) << "\r\n"; //c++ version of stderr
        return 0;
    }

    if (fstat(fd, &sb) == -1){
        std::cerr << "Failed to stat file! Error = " << (errno) << "\r\n";
        return 0;
    }

    read(fd, song_buf, sb.st_size); //uses a count so this is safe
    close(fd);

    std::cout << "Loaded file into shared buffer " << sb.st_size << "\r\n";
    return sb.st_size;
}


//////////////////////// COMMAND FUNCTIONS ////////////////////////


// attempts to log in for a user
//void login(char *username, char *pin) {
//    if (!username || !pin) {
//        mp_printf("Invalid user name/PIN\r\n");
//        print_help();
//        return;
//    }

    // drive DRM
//    strcpy((void*)c->username, username);
//    strcpy((void*)c->pin, pin);
//    send_command(LOGIN);
//}

//done
void login(std::string& username, std::string& pin) {
    if(username.size() == 0 || pin.size == 0 || username.size() > 8 || pin.size() > 5) {
        std::cout "Invalid user name/PIN\r\n";
        print_help();
        return;
    }
    if (username.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos || 
        pin.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos) {
        std::cerr << "Error username/pin not valid\n";
        exit(1); // change this as needed
    }
    //drive DRM
    //instead of strcpy use '=' operator
    c -> username = username; 
    c -> pin = pin;
    send_command(LOGIN);
}


//done
// logs out for a user
void logout() {
    // drive DRM
    send_command(LOGOUT);
}


//done
// queries the DRM about the player
// DRM will fill shared buffer with query content
void query_player() {
    // drive DRM
    send_command(QUERY_PLAYER);
    while (c->drm_state == STOPPED) continue; // wait for DRM to start working
    while (c->drm_state == WORKING) continue; // wait for DRM to dump file

    // print query results
    std::cout "Regions: " << q_region_lookup(c->query, 0);
    for (int i = 1; i < c->query.num_regions; i++) {
        std::cout", " << q_region_lookup(c->query, i);
    }
    std::cout << "\r\n";

    std::cout << "Authorized users: ";
    if (c->query.num_users) {
        std::cout << q_user_lookup(c->query, 0);
        for (int i = 1; i < c->query.num_users; i++) {
            std::cout", " << q_user_lookup(c->query, i);
        }
    }
    std::cout << "\r\n";
}

//done
// queries the DRM about a song
void query_song(char *song_name) {
    // load the song into the shared buffer
    if (!load_file(song_name, (void*)&c->song)) {
        std::cerr << "Failed to load song!\r\n";
        return;
    }

    // drive DRM
    send_command(QUERY_SONG);
    while (c->drm_state == STOPPED) continue; // wait for DRM to start working
    while (c->drm_state == WORKING) continue; // wait for DRM to finish

    // print query results

    std::cout "Regions: " << q_region_lookup(c->query, 0);
    for (int i = 1; i < c->query.num_regions; i++) {
        std::cout", " << q_region_lookup(c->query, i);
    }
    std::cout"\r\n";

    std::cout"Owner: " << c->query.owner;
    std::cout"\r\n";

    std::cout << "Authorized users: ";
    if (c->query.num_users) {
        std::cout << q_user_lookup(c->query, 0);
        for (int i = 1; i < c->query.num_users; i++) {
            std::cout", " << q_user_lookup(c->query, i);
        }
    }
    std::cout << "\r\n";
}

//done
// attempts to share a song with a user
void share_song(char *song_name, std::string& username) {
    int fd;
    unsigned int length;
    ssize_t wrote, written = 0;

    if (!username) {
        std::cout << "Need song name and username\r\n";
        print_help();
    }

    // load the song into the shared buffer
    if (!load_file(song_name, (void*)&c->song)) {
        std::cerr << "Failed to load song!\r\n";
        return;
    }

    //strcpy((char *)c->username, username);
    //use '=' operator instead of strcpy
    c -> username = username;

    // drive DRM
    send_command(SHARE);
    while (c->drm_state == STOPPED) continue; // wait for DRM to start working
    while (c->drm_state == WORKING) continue; // wait for DRM to share song

    // request was rejected if WAV length is 0
    length = c->song.wav_size;
    if (length == 0) {
        std::err << "Share rejected\r\n";
        return;
    }

    // open output file
    fd = open(song_name, O_WRONLY);
    if (fd == -1){
        std::cerr << "Failed to open file! Error = " << (errno) << "\r\n";
        return;
    }

    // write song dump to file
    std::cout << "Writing song to file " << song_name << " " << length << "\r\n";
    while (written < length) {
        wrote = write(fd, (char *)&c->song + written, length - written);
        if (wrote == -1) {
            std::cerr << "Error in writing file! Error = " << (errno) << "\r\n";
            return;
        }
        written += wrote;
    }
    close(fd);
    std::cout << "Finished writing file\r\n";
}


// plays a song and enters the playback command loop
int play_song(char *song_name) {
    //char usr_cmd[USR_CMD_SZ + 1], *cmd = NULL, *arg1 = NULL, *arg2 = NULL; 
    std::string usr_cmd = "";
    std::string cmd[];
    std::string arg1 = "";
    std::string arg2 = "";

    // load song into shared buffer
    if (!load_file(song_name, (void*)&c->song)) {
        std::cerr << "Failed to load song!\r\n";
        return 0;
    }

    // drive the DRM
    send_command(PLAY);
    while (c->drm_state == STOPPED) continue; // wait for DRM to start playing

    // play loop
    while(1) {
        // get a valid command
        do {
            print_prompt_msg(song_name);
            //fgets(usr_cmd, USR_CMD_SZ, stdin); 
            std::getline(std::cin, usr_cmd);

            // exit playback loop if DRM has finished song
            if (c->drm_state == STOPPED) {
                std::cout << "Song finished\r\n";
                return 0;
            }
        } while (usr_cmd.length() < 2); //chars are one byte so this is fine
        
        // parse and handle command
        parse_input(usr_cmd, cmd, arg1, arg2);
        if (cmd) {
            continue;
        } else if (cmd[0] == "help")) {
            print_playback_help();
        } else if (!strcmp(cmd[0] == "resume")) {
            send_command(PLAY);
            usleep(200000); // wait for DRM to print
        } else if (cmd[0] == "pause")) {
            send_command(PAUSE);
            usleep(200000); // wait for DRM to print
        } else if (cmd[0] == "stop")) {
            send_command(STOP);
            usleep(200000); // wait for DRM to print
            break;
        } else if (cmd[0] == "restart")) {
            send_command(RESTART);
        } else if (cmd[0] == "exit")) {
            std::cout << "Exiting...\r\n";
            send_command(STOP);
            return -1;
        } else if (cmd[0] == "rw")) {
            mp_printf("Unsupported feature.\r\n\r\n");
            print_playback_help();
        } else if (cmd[0] == "ff")) {
            std::cout << "Unsupported feature.\r\n\r\n";
            print_playback_help();
        } else if (cmd[0] == "lyrics")) {
            std::cout << "Unsupported feature.\r\n\r\n";
            print_playback_help();
        } else {
            std::cout << "Unrecognized command.\r\n\r\n";
            print_playback_help();
        }
    }

    return 0;
}

// turns DRM song into original WAV for digital output
void digital_out(char *song_name) {
    
    char fname[64];
    //not sure about converting this code to C++

    // load file into shared buffer
    if (!load_file(song_name, (void*)&c->song)) {
        std::cout << "Failed to load song!\r\n";
        return;
    }

    // drive DRM
    send_command(DIGITAL_OUT);
    while (c->drm_state == STOPPED) continue; // wait for DRM to start working
    while (c->drm_state == WORKING) continue; // wait for DRM to dump file

    // open digital output file
    int written = 0, wrote, length = c->song.file_size + 8;
    int fd = open(fname, O_WRONLY | O_CREAT | O_TRUNC);
    if (fd == -1){
        std::cerr << "Failed to open file! Error = " << (errno) << "\r\n";
        return;
    }

    // write song dump to file
    std::cout << "Writing song to file " << fname << " " << length;
    while (written < length) {
        wrote = write(fd, (char *)&c->song + written, length - written);
        if (wrote == -1) {
            std::cerr << "Error in writing file! Error = " << (errno) << "\r\n";
            return;
        }
        written += wrote;
    }
    close(fd);
    std::cout << "Finished writing file\r\n";
}


//////////////////////// MAIN ////////////////////////


int main(int argc, char** argv)
{
    int mem;
   // char usr_cmd[USR_CMD_SZ + 1], *cmd = NULL, *arg1 = NULL, *arg2 = NULL;
   // memset(usr_cmd, 0, USR_CMD_SZ + 1);
    std::string usr_cmd = "";
    std::string cmd[];
    std::string arg1 = "";
    std::string arg2 = "";

    // open command channel
    mem = open("/dev/uio0", O_RDWR);
    c = mmap(NULL, sizeof(cmd_channel), PROT_READ | PROT_WRITE,
             MAP_SHARED, mem, 0);
    if (c == MAP_FAILED){
        std::cerr << "MMAP Failed! Error = " << (errno);
        return -1;
    }
    std::cout << "Command channel open at " << c << " " << sizeof(cmd_channel) // do we really need to print this?;

    // dump player information before command loop
    query_player();

    // go into command loop until exit is requested
    while (1) {
        // get command
        print_prompt();
        //fgets(usr_cmd, USR_CMD_SZ, stdin);
        std::getline(std::cin, usr_cmd);

        // parse and handle command
        parse_input(usr_cmd, &cmd, &arg1, &arg2);
        if (!cmd) {
            continue;
        } else if (cmd[0] == "help")) {
            print_help();
        } else if (cmd[0] == "login")) {
            login(arg1, arg2);
        } else if (cmd[0] == "logout")) {
            logout();
        } else if (cmd[0] == "query")) {
            query_song(arg1);
        } else if (cmd[0] == "play")) {
            // break if exit was commanded in play loop
            if (play_song(arg1) < 0) {
                break;
            }
        } else if (cmd[0] == "digital_out")) {
            digital_out(arg1);
        } else if (cmd[0] == "share")) {
            share_song(arg1, arg2);
        } else if (cmd[0] == "exit")) {
            std::cout << "Exiting...\r\n";
            break;
        } else {
            std::cout << "Unrecognized command.\r\n\r\n";
            print_help();
        }
    }

    // unmap the command channel
    munmap((void*)c, sizeof(cmd_channel));

    return 0;
}
