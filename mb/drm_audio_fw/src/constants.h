#ifndef SRC_CONSTANTS_H_
#define SRC_CONSTANTS_H_

#include "xil_printf.h"

// shared DDR address
#define SHARED_DDR_BASE (0x20000000 + 0x1CC00000)

// memory constants
#define CHUNK_SZ 16000
#define FIFO_CAP 4096*4

// number of seconds to record/playback
#define PREVIEW_TIME_SEC 30

// ADC/DAC sampling rate in Hz
#define AUDIO_SAMPLING_RATE 48000
#define BYTES_PER_SAMP 2
#define PREVIEW_SZ (PREVIEW_TIME_SEC * AUDIO_SAMPLING_RATE * BYTES_PER_SAMP)

// printing utility
#define MB_PROMPT "\r\nMB> "
#define mb_printf(...) xil_printf(MB_PROMPT __VA_ARGS__)

// protocol constants
#define MAX_REGIONS 64
#define REGION_NAME_SZ 64
#define MAX_USERS 64
#define USERNAME_SZ 64
#define MAX_PIN_SZ 64
#define MAX_SONG_SZ (1<<25)

#define CHUNK_TIME_SEC 1
#define AUDIO_SAMPLING_RATE 48000
#define BYTES_PER_SAMP 2
#define NONCE_SIZE 12
#define WAVE_HEADER_SZ 44
#define META_DATA_ALLOC 4
#define ENC_WAVE_HEADER_SZ WAVE_HEADER_SZ + META_DATA_ALLOC
#define MAC_SIZE 16
#define SONG_CHUNK_SZ 20480
#define SONG_CHUNK_BUFFER 1000
#define ENC_CHUNK_SZ SONG_CHUNK_SZ + MAC_SIZE

#define RID_SZ 8
#define UID_SZ 8

#define MAX_METADATA_SZ UID_SZ + (RID_SZ * MAX_REGIONS) + (MAX_USERS * UID_SZ)


// LED colors and controller
struct color {
    u32 r;
    u32 g;
    u32 b;
};


// struct to interpret shared buffer as a query
typedef struct {
    int num_regions;
    int num_users;
    char owner[USERNAME_SZ];
    char regions[MAX_REGIONS * REGION_NAME_SZ];
    char users[MAX_USERS * USERNAME_SZ];
} query;

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
    u32 file_size;
    char packing2[32];
    u32 wav_size;
    drm_md md;
} song;

typedef struct __attribute__ ((__packed__)) {
    char packing1[4];
    u32 file_size;
    char packing2[32];
    u32 wav_size;
} waveHeaderStruct;

typedef struct __attribute__ ((__packed__)) {
	waveHeaderStruct wave_header;
	u32 metadata_size;
} waveHeaderMetaStruct;

typedef struct __attribute__ ((__packed__)) {
	unsigned char nonce[NONCE_SIZE];
	waveHeaderMetaStruct wave_header_meta;
	unsigned char tag[MAC_SIZE];
} encryptedWaveheader;

typedef struct __attribute__ ((__packed__)) {
	unsigned char nonce[NONCE_SIZE];
	unsigned char tag[MAC_SIZE];
	unsigned char metadata[];
} encryptedMetadata;

#define get_metadata(m) ((unsigned char *)(&m.metadata))

typedef struct __attribute__ ((__packed__)) {
	unsigned char nonce[NONCE_SIZE];
	unsigned char tag[MAC_SIZE];
	unsigned char data[SONG_CHUNK_SZ];
} encryptedSongChunk;

#define get_chunk_data(c) ((unsigned char *)(&c.data))

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
    u32 metadata_size;
    u32 total_chunks;
    u32 chunk_size;
    u32 chunk_nums;
    u32 chunk_remainder;

    // shared buffer is either a drm song or a query
    union {
        song song;
        query query;
        encryptedWaveheader encWaveHeaderMeta;
        encryptedMetadata encMetadata;
        encryptedSongChunk encSongChunk;
    };
} cmd_channel;


// local store for drm metadata
typedef struct {
    u8 md_size;
    u8 owner_id;
    u8 num_regions;
    u8 rids[MAX_REGIONS];
    u8 num_users;
    u8 uids[MAX_USERS];
} song_md;


// store of internal state
typedef struct {
    char logged_in;             // whether or not a user is logged on
    u8 uid;                     // logged on user id
    char username[USERNAME_SZ]; // logged on username
    char pin[MAX_PIN_SZ];       // logged on pin
    song_md song_md;            // current song metadata
    char drm_state;				// drm state
} internal_state;


#endif /* SRC_CONSTANTS_H_ */
