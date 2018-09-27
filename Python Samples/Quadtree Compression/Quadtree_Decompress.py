""" Dalton Fox ~ Lab 8: Quadtrees """

import os
import sys
import struct
from PIL import Image

# Open file from command argument
fp = open(sys.argv[1], 'rb')

# Header Block
fp.seek(0, 2)
end = fp.tell() - 5
fp.seek(0, 0)
ext = fp.readline(100)
end -= len(ext)
ext = ext.strip()
RGBA_mode = struct.unpack("B", fp.read(1))[0]
image_size = struct.unpack("HH", fp.read(4))

# Set image color type
if RGBA_mode:
    image = Image.new("RGBA", image_size)
else:
    image = Image.new("RGB", image_size)

# Decompression routine
pos = 0
while pos < end:
    if RGBA_mode:
        x, y, w, h, r, g, b, a = struct.unpack("HHHHBBBB", fp.read(12))
        for i in range(w):
            for j in range(h):
                image.putpixel((i + x, j + y), (r, g, b, a))
        pos += 12
    else:
        x, y, w, h, r, g, b = struct.unpack("HHHHBBB", fp.read(11))
        for i in range(w):
            for j in range(h):
                image.putpixel((i + x, j + y), (r, g, b))
        pos += 11

# Finalize
image.save("n" + sys.argv[1].replace(".z", ext))
fp.close()
