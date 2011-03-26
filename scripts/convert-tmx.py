#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
converts tmx maps into our simplistic map format
"""
import sys
from xml.etree import ElementTree as et


class Map(object):

    def __init__(self, width, height, tile_width, tile_height, tileset):
        self.width = width
        self.height = height
        self.tile_width = tile_width
        self.tile_height = tile_height
        self.tileset = tileset
        self.layers = []


def relative_resource_path(path):
    marker = '/resources/'
    pos = path.find(marker)
    if pos >= 0:
        return path[pos + len(marker):]
    return path


def load_layer(data):
    result = []
    for i in xrange(0, len(data), 4):
        gid = ord(data[i]) | ord(data[i + 1]) << 8 | \
              ord(data[i + 2]) << 16 | ord(data[i + 3]) << 24
        result.append(gid)
    return result


def prepare_layer(root, name):
    for layer in root.findall('layer'):
        if layer.attrib['name'] != name:
            continue
        data = layer.find('data')
        assert data.attrib['encoding'] == 'base64'
        val = data.text.decode('base64')
        if data.attrib.get('compression'):
            val = val.decode(data.attrib['compression'])
        return load_layer(val)
    raise TypeError('Could not find layer ' + name)


def read_tmx_file(filename):
    with open(filename) as f:
        doc = et.parse(f)
    root = doc.getroot()
    tileset = root.find('tileset')

    map = Map(int(root.attrib['width']),
              int(root.attrib['height']),
              int(root.attrib['tilewidth']),
              int(root.attrib['tileheight']),
              relative_resource_path(tileset.getchildren()[0].attrib['source']))

    map.layers.append(prepare_layer(root, 'Background'))
    map.layers.append(prepare_layer(root, 'Foreground'))
    return map


def write_simple_format(m, filename):
    with open(filename, 'wb') as f:
        f.write(m.tileset + '\n')
        f.write('%d\n%d\n%d\n%d\n' % (m.width, m.height, m.tile_width, m.tile_height))
        for layer in m.layers:
            f.write(''.join(map(chr, layer)))


def main():
    if len(sys.argv) != 3:
        print >> sys.stderr, 'usage: convert-tmx.py [input] [output]'
        sys.exit(1)
    input_file, output_file = sys.argv[1:]

    m = read_tmx_file(input_file)
    write_simple_format(m, output_file)


if __name__ == '__main__':
    main()
