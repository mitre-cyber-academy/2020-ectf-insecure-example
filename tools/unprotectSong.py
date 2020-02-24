#!/usr/bin/env python3
import json
import struct
import os
import wave
from argparse import ArgumentParser
import numpy as np

# Used for encryption
import nacl.bindings as b
import nacl.exceptions as exc
import nacl.hash
import nacl.encoding

# calculating the chunk remainder
import math

# TODO: Move decryption processes to separate functions
# TODO: Add argument parsing
# TODO: Move nonces to encrypted chunk, keep first nonce public

def decrypt_song(keys_loc):
    # Configuration Variables
    sample_rate = 48000     # 48KHz
    chunk_time = 30         # 30s
    bytes_per_sample = 2    # 2 bytes sampled
    mac_size = 16           # poly1305 mac size
    hash_byte_size = 12     # Take 12 bytes of a 256 bit hash
    aad_size = 4            # Use 4 bytes to store aad size
    wave_header_size = 44
    metadata_size_allocation = 8
    encrypted_wave_header_size = wave_header_size + metadata_size_allocation + mac_size
    chunk_size = sample_rate * chunk_time * bytes_per_sample
    encrypted_chunk_size = chunk_size + mac_size
    encoder = nacl.encoding.RawEncoder

    encrypted_file_size = 0

    print("Setting chunksize to " + str(chunk_size) + " bytes")

    encrypted_song = open("song.blr", 'rb')
    decrypted_song = open("dec_song.wav", 'wb')

    keys_file = json.load(open(keys_loc, "r"))

    print("key " + keys_file["key"])
    print("iv " + keys_file["iv"])

    key = bytes.fromhex(keys_file["key"])
    iv = bytes.fromhex(keys_file["iv"])

    print("Starting decrypt song")

    nonce = encrypted_song.read(hash_byte_size)
    print("Wave Header Nonce: " + str(nonce))

    encrypted_wave_header = encrypted_song.read(encrypted_wave_header_size)

    aad = b"wave_header\0"

    # Encrypt Wav Header
    wav_header = b.crypto_aead_chacha20poly1305_ietf_decrypt(encrypted_wave_header, aad, nonce, key)

    metadata_size = int.from_bytes(wav_header[-metadata_size_allocation:], 'little')
    print("Metadata size: " + str(metadata_size))

    # Strip metadata size off wave_header
    wav_header = wav_header[:wave_header_size]

    decrypted_song.write(wav_header)

    song_info_size = int.from_bytes(wav_header[-4:], byteorder='little')
    print("Song size: " + str(song_info_size))

    # Calculate # of chunks to read
    chunk_to_read = math.floor(song_info_size / chunk_size)
    print("Chunks to read: " + str(chunk_to_read))

    # Calculate remainder
    chunk_remainder = song_info_size % chunk_size
    encrypted_chunk_remainder_size = chunk_remainder + mac_size
    print("Chunk remainder size: " + str(chunk_remainder))

    nonce = encrypted_song.read(hash_byte_size)
    print("Metadata nonce: " + str(nonce))

    aad = b"meta_data\0"

    encrypted_metadata_size = metadata_size + mac_size
    encrypted_metadata = encrypted_song.read(encrypted_metadata_size)

    metadata = b.crypto_aead_chacha20poly1305_ietf_decrypt(encrypted_metadata, aad, nonce, key)

    for i in range(1, chunk_to_read + 1):
        print("Read chunk: " + str(i))
        nonce = encrypted_song.read(hash_byte_size)
        print("Chunk " + str(i) + " Nonce: " + str(nonce))
        aad = int.to_bytes(i, aad_size, 'little')
        encrypted_chunk = encrypted_song.read(encrypted_chunk_size)

        song_chunk = b.crypto_aead_chacha20poly1305_ietf_decrypt(encrypted_chunk, aad, nonce, key)
        decrypted_song.write(song_chunk)

    nonce = encrypted_song.read(hash_byte_size)
    print("Remainder Nonce: " + str(nonce))
    aad = int.to_bytes(chunk_to_read + 1, aad_size, 'little')
    encrypted_chunk = encrypted_song.read(encrypted_chunk_remainder_size)

    song_chunk = b.crypto_aead_chacha20poly1305_ietf_decrypt(encrypted_chunk, aad, nonce, key)
    decrypted_song.write(song_chunk)
    encrypted_song.close()
    decrypted_song.close()

    print("Decryption Success")
    

def main():
    decrypt_song("keys.json")

if __name__ == '__main__':
    main()
