import struct
import os
import wave


protected_wave = wave.open(os.path.abspath("/home/tthomas/mitre/2020-ectf-insecure-example/sample-audio/demo_flag.drm", 'rb')

unprotected_wave = struct.unpack('<h', 
