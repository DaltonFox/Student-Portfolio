""" Dalton Fox Quadtrees """

import os
import sys
import math
import struct
from PIL import Image

# Node
class QuadNode(object):
    RGBA_mode = False
    
    # Initialization
    def __init__(self, parent, x, y, w, h):
        if QuadNode.RGBA_mode: self.color = [0, 0, 0, 0]
        else: self.color = [0, 0, 0]

        self.x = x; self.y = y
        self.w = w; self.h = h
        self.isLeaf = False
        
        self.parent = parent
        self.north_west = self.north_east = self.south_west = self.south_east = None        

    # Recursive routine for calculating quads
    def subdivide(self, depth):
        if depth == 0: return

        n_w = self.w // 2
        n_h = self.h // 2
        r_w = self.w % 2
        r_h = self.h % 2

        if n_w + r_w != 0 and n_h + r_h != 0:
            self.north_west = QuadNode(self, self.x, self.y, n_w + r_w, n_h + r_h)
        if n_w != 0 and n_h + r_h != 0:
            self.north_east = QuadNode(self, self.x + n_w + r_w, self.y, n_w, n_h + r_h)
        if n_w + r_w != 0 and n_h != 0:
            self.south_west = QuadNode(self, self.x, self.y + n_h + r_h, n_w + r_w, n_h)
        if n_w != 0 and n_h != 0:
            self.south_east = QuadNode(self, self.x + n_w + r_w, self.y + n_h + r_h, n_w, n_h)

        if self.north_west is not None: self.north_west.subdivide(depth - 1)
        if self.north_east is not None: self.north_east.subdivide(depth - 1)
        if self.south_west is not None: self.south_west.subdivide(depth - 1)
        if self.south_east is not None: self.south_east.subdivide(depth - 1)

    # Recursive routine for calculating compressed colors
    def average_colors(self, width, data):
        mp = {}
        if self.north_west is not None:
            self.north_west.average_colors(width, data)
            mp["nw"] = self.north_west.color
        if self.north_east is not None:
            self.north_east.average_colors(width, data)
            mp["ne"] = self.north_east.color
        if self.south_west is not None:
            self.south_west.average_colors(width, data)
            mp["sw"] = self.south_west.color
        if self.south_east is not None:
            self.south_east.average_colors(width, data)
            mp["se"] = self.south_east.color

        # Average color data
        if len(mp) == 0:
            if QuadNode.RGBA_mode: color_sum = [0, 0, 0, 0]
            else: color_sum = [0, 0, 0]

            count = 0
            for i in range(self.w):
                for j in range(self.h):
                    pixel = data[(i + self.x) + width * (j + self.y)]
                    if len(color_sum) == 4:
                        color_sum = [color_sum[0] + pixel[0], color_sum[1] + pixel[1], color_sum[2] + pixel[2], color_sum[3] + pixel[3]]
                    else:
                        color_sum =  [color_sum[0] + pixel[0], color_sum[1] + pixel[1], color_sum[2] + pixel[2]]
                    count += 1
            if len(color_sum) == 4:
                self.color = tuple([int(color_sum[0] / float(count)), int(color_sum[1] / float(count)), int(color_sum[2] / float(count)), int(color_sum[3] / float(count))])
            else:
                self.color = tuple([int(color_sum[0] / float(count)), int(color_sum[1] / float(count)), int(color_sum[2] / float(count))])
            self.isLeaf = True
        else:
            self.isLeaf = False

    # Write compressed pixel data to file
    def write(self, file_pointer):
        if self.isLeaf:
            if len(self.color) == 4:
                # Write X, Y, W, H, RGBA
                file_pointer.write(struct.pack("HHHHBBBB", self.x, self.y, self.w, self.h, self.color[0], self.color[1], self.color[2], self.color[3]))
            else:
                # Write X, Y, W, H, RGB
                file_pointer.write(struct.pack("HHHHBBB", self.x, self.y, self.w, self.h, self.color[0], self.color[1], self.color[2]))
        else:
            if self.north_west is not None: self.north_west.write(file_pointer)
            if self.north_east is not None: self.north_east.write(file_pointer)
            if self.south_west is not None: self.south_west.write(file_pointer)
            if self.south_east is not None: self.south_east.write(file_pointer)

# Root
class QuadTree(object):
    def __init__(self, image):
        self.image = image
        self.root = QuadNode(None, 0, 0, image.width, image.height)

    def subdivide(self, depth=0): 
        self.root.subdivide(depth)

    def average_colors(self): 
        self.root.average_colors(self.image.width, list(self.image.getdata()))

    def write(self, file_pointer): 
        self.root.write(file_pointer)


# Open file from command argument
with open(sys.argv[1], "rb") as fp:
    fp.seek(0, 2)
    file_size = fp.tell()

# Initialization
image = Image.open(sys.argv[1])
compression_amount = int(sys.argv[2])
extension = image.format

# Image mode to store data appropriately
if image.mode in ('RGBA', 'LA') or (image.mode == 'P' and 'transparency' in image.info):
    QuadNode.RGBA_mode = True
    image = image.convert("RGBA")
else:
    image = image.convert("RGB")

# Calculate compression depth for our Quadtree
closest_power = int(2 ** math.ceil(math.log(max([image.width, image.height])) / float(math.log(2))))
i = 0
while closest_power > 1:
    closest_power /= 2.0
    i += 1
percent = compression_amount / 100.0
target_depth = int(math.floor(i * percent))

# Calculate / Compression routine
tree = QuadTree(image)
tree.subdivide(target_depth)
tree.average_colors()
image.close()

# Output our result into a binary file
extension = os.path.splitext(sys.argv[1])[1]
with open(sys.argv[1].replace(extension, ".z"), 'wb') as fp:
    # Header
    fp.write(extension + "\n")
    if QuadNode.RGBA_mode: fp.write(chr(1))
    else: fp.write(chr(0))

    # Data
    fp.write(struct.pack("HH", tree.root.w, tree.root.h))
    tree.write(fp)

    # Calculation
    new_size = fp.tell()
