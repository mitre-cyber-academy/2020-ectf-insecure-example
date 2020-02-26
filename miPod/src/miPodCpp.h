/*
 * miPod.h
 *
 *  Created on: Jan 9, 2020
 *      Author: ectf
 */

#ifndef SRC_MIPOD_H_
#define SRC_MIPOD_H_


// miPod constants
#define USR_CMD_SZ 64

// protocol constants
#define MAX_REGIONS 64
#define REGION_NAME_SZ 64
#define MAX_USERS 64
#define USERNAME_SZ 64
#define MAX_PIN_SZ 64
#define MAX_SONG_SZ (1<<25)

// printing utility
#define MP_PROMPT "MP> "
#define mp_printf(...) printf(MP_PROMPT __VA_ARGS__)

#define USER_PROMPT "miPod %s# "
#define print_prompt() printf(USER_PROMPT, "")
#define print_prompt_msg(...) printf(USER_PROMPT, __VA_ARGS__)

#define RID_SZ 8
#define UID_SZ 8

#define MAX_METADATA_SZ UID_SZ + (RID_SZ * MAX_REGIONS) + (MAX_USERS * UID_SZ)

#define CHUNK_TIME_SEC 5
#define AUDIO_SAMPLING_RATE 48000
#define BYTES_PER_SAMP 2
#define NONCE_SIZE 12
#define WAVE_HEADER_SZ 44
#define META_DATA_ALLOC 4
#define SONG_CHUNK_SZ 20480
#define SONG_CHUNK_RAM 1000
#define ENC_WAVE_HEADER_SZ WAVE_HEADER_SZ + META_DATA_ALLOC
#define MAC_SIZE 16

// struct to interpret shared buffer as a query
typedef struct {
    int num_regions;
    int num_users;
    char owner[USERNAME_SZ];
    char regions[MAX_REGIONS * REGION_NAME_SZ];
    char users[MAX_USERS * USERNAME_SZ];
} queryStruct;

// simulate array of 64B names without pointer indirection
#define q_region_lookup(q, i) (q.regions + (i * REGION_NAME_SZ))
#define q_user_lookup(q, i) (q.users + (i * USERNAME_SZ))


// struct to interpret drm metadata
typedef struct __attribute__((__packed__)) {
    char md_size;
    char owner_id;
    char num_regions;
    char num_users;
    char buf[];
} drm_md;


// struct to interpret shared buffer as a drm song file
// packing values skip over non-relevant WAV metadata
typedef struct __attribute__((__packed__)) {
    char packing1[4];
    int file_size;
    char packing2[32];
    int wav_size;
    drm_md md;
} songStruct;

typedef struct __attribute__ ((__packed__)) {
	unsigned char wav_header[WAVE_HEADER_SZ];
	unsigned int metadata_size;
} waveHeaderStruct;

typedef struct __attribute__ ((__packed__)) {
	unsigned char nonce[NONCE_SIZE];
	waveHeaderStruct wave_header_struct;
	unsigned char tag[MAC_SIZE];
} encryptedWaveheader;

typedef struct __attribute__ ((__packed__)) {
	unsigned char nonce[NONCE_SIZE];
	unsigned char tag[MAC_SIZE];
	unsigned char metadata[];
} encryptedMetadata;

#define get_metadata(m) ((char *)(&m.metadata) + NONCE_SIZE + MAC_SIZE)

typedef struct __attribute__ ((__packed__)) {
	unsigned char nonce[NONCE_SIZE];
	unsigned char tag[MAC_SIZE];
	unsigned char data[SONG_CHUNK_SZ];
} encryptedSongChunk;

#define get_chunk_data(c) ((char *)(&c.data))

// accessors for variable-length metadata fields
#define get_drm_rids(d) (d.md.buf)
#define get_drm_uids(d) (d.md.buf + d.md.num_regions)
#define get_drm_song(d) ((char *)(&d.md) + d.md.md_size)


// shared buffer values
enum commands { QUERY_PLAYER, QUERY_SONG, LOGIN, LOGOUT, SHARE, PLAY, STOP, DIGITAL_OUT, PAUSE, RESTART, FF, RW, READ_HEADER, READ_METADATA, READ_CHUNK };
enum states   { STOPPED, WORKING, PLAYING, PAUSED, WAITING_METADATA, WAITING_CHUNK, READING_CHUNK };


// struct to interpret shared command channel
typedef volatile struct __attribute__((__packed__)) {
    char cmd;                   // from commands enum
    char drm_state;             // from states enum
    char login_status;          // 0 = logged off, 1 = logged on
    char padding;               // not used
    char username[USERNAME_SZ]; // stores logged in or attempted username
    char pin[MAX_PIN_SZ];       // stores logged in or attempted pin
    int metadata_size;
    int total_chunks;
    int chunk_size;
    int chunk_nums;
    int chunk_remainder;

    // shared buffer is either a drm song or a query
    union {
        songStruct song;
        queryStruct query;
        encryptedWaveheader encWaveHeader;
        encryptedMetadata encMetadata;
        encryptedSongChunk encSongChunk[1000];
        char buf[MAX_SONG_SZ]; // sets correct size of cmd_channel for allocation
    };
} cmd_channel;

#endif /* SRC_MIPOD_H_ */
