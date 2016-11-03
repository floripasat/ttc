#!/usr/bin/env python
# -*- coding: utf-8 -*-

#
# bin2bin.py
#
# Copyright (C) 2016, Universidade Federal de Santa Catarina
# 
# This file is part of FloripaSat-TTC.
# 
# FloripaSat-TTC is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# FloripaSat-TTC is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with FloripaSat-TTC.  If not, see <http://www.gnu.org/licenses/>.
# 
# 

__author__      = "Gabriel Mariano Marcelino"
__copyright__   = "gabriel.mm8@gmail.com"
__credits__     = ["Gabriel Mariano Marcelino","Ruan Molgero"]
__license__     = "GPL3"
__version__     = "1.0-dev"
__maintaner__   = "Gabriel Mariano Marcelino"
__email__       = "gabriel.mm8@gmail.com"
__status__      = "Development"

import pickle


def crc16(poly, initial_value, data):
    table = list()
    for i in range(256):
        crc_t = 0
        c = i
        c = c << 8
        for j in range(8):
            if (crc_t ^ c) & 0x8000:
                crc_t = (crc_t << 1) ^ poly
            else:
                crc_t = crc_t << 1
            c = c << 1
        table.append(crc_t)
    
    crc = initial_value
    for c in data:
        if type(data) is str:
            c = ord(c)
        cc = 0xFF & c
        tmp = (crc >> 8) ^ cc
        crc = (crc << 8) ^ table[tmp & 0xFF]
        crc = crc & 0xFFFF
    return crc


def Bytes2String(f):
    bin_string = str()

    # Converts the binary file to a string of ones and zeros
    byte = f.read(1)
    while byte != "":
        if byte == '\x00':
            bin_string += '0'
        elif byte == '\x01':
            bin_string += '1'
        byte = f.read(1)
    return bin_string
    

def FindPackets(bin_stream, preamble, packet_size):
    # Find packets in the binary string (using the preamble as trigger)
    buf = str()
    packets = list()
    i = 0
    for b in bin_stream:
        buf += b
        if len(buf) == len(preamble):
            if buf == preamble:
                if (i+packet_size-len(preamble)) < len(bin_stream):
                    packets.append(bin_stream[i+1:(i+packet_size-len(preamble)+1)])
            buf = buf[1:]
        i += 1
    return packets
    

def CheckPacket(packet, sync_bytes, address, message, crc):
    i = 0
    for sb in sync_bytes:
        if packet[i:i+8] != str(bin(sb)[2:].zfill(8)):
            break
        i += 8
    if i == 8*len(sync_bytes):
        if packet[-16:] == crc:
            return True
    else:
        return False
    
    
def main(args):
    path = str()
    if len(args) == 1:
        path = "/code/gnuradio/bin_data.bin"
    else:
        path = args[1]

#****************************************************
#-- INPUTS ------------------------------------------
#****************************************************
    preamble_byte       = 0xAA
    preamble_size       = 4
    sync_bytes          = [0x04,0x08,0x0F,0x10]
    address             = 0x17
    message             = "FloripaSat"  # String or list of bytes
    crc_polynomial      = 0x8005        # x^16 + x^15 + x^2 + 1
    crc_initial_value   = 0xFFFF
    print_expected_pkt  = True
    print_bin_str       = False
    print_packets       = False
    print_pkt_bytes     = True
    print_statistics    = True
    save_results        = False
#****************************************************

    preamble        = preamble_size*str(bin(preamble_byte))[2:]
    sync_word_size  = len(sync_bytes)
    address_size    = 1
    message_size    = len(message)
    
    crc16_data = [address]
    for c in message:
        if type(message) is str:
            crc16_data.append(ord(c))   # Message as a string
        elif type(message) is list:
            crc16_data.append(c)        # Message as a list of bytes
    crc             = str(bin(crc16(crc_polynomial, crc_initial_value, crc16_data))[2:].zfill(16))
    crc_size        = len(crc)/8

    packet_size = preamble_size + sync_word_size + address_size + message_size + crc_size
    packet_size *= 8        # Bytes to bits

    if print_expected_pkt:
        print "###################################"
        print "-- Expected Packet ----------------"
        print "###################################"
        print "Preamble:\t" + preamble
        print "Sync. word:\t" + str(bin(sync_bytes[0])[2:].zfill(8)) + str(bin(sync_bytes[1])[2:].zfill(8)) + str(bin(sync_bytes[2])[2:].zfill(8)) + str(bin(sync_bytes[3])[2:].zfill(8))
        print "Address:\t" + str(bin(address)[2:].zfill(8))
        msg = ""
        if type(message) is str:
            msg = message
        elif type(message) is list:
            msg = str(message)
        print "Message:\t" + msg
        print "CRC16:\t\t" + crc + " (" + str(crc16(crc_polynomial, crc_initial_value, crc16_data)) + ")"
        print "Total bytes:\t" + str(packet_size/8)
        print "Total bits:\t" + str(packet_size)
        print "###################################"

    f = open(path, "rb")

    bin_stream = Bytes2String(f)

    f.close()

    # Prints the binary string
    if print_bin_str:
        print "\n###################################"
        print "-- Output -------------------------"
        print "###################################"
        print bin_stream
        print "###################################"
        if save_results:
            bin_file = open("bin_stream.txt", "w")
            bin_file.write(bin_stream)
            bin_file.close()

    packets = FindPackets(bin_stream, preamble, packet_size)

    # Prints the binary packets
    if print_packets:
        print "\n###################################"
        print "-- Packets -------------------------"
        print "###################################"
        print packets
        print "###################################"
        if save_results:
            bin_pkts_file = open("bin_pkts.txt", "w")
            pickle.dump(packets, bin_pkts_file)
            bin_pkts_file.close()

    # Split packets into bytes
    valid_packets = list()
    packet_counter = 0
    if print_pkt_bytes:
        print "\n###################################"
        print "-- Packets (Bytes) ----------------"
        print "###################################"
        for p in packets:
            print "\n----------------------------------"
            print " Packet " + str(packet_counter) + ":"
            print "----------------------------------"
            print "Binary\t\tHex.\tASCII"
            print "----------------------------------"
            i = 0
            j = 0
            byte = str()
            while j < len(p):
                byte += p[j]
                i += 1
                if i == 8:
                    hex_byte = hex(int(byte, 2))
                    ascii_char = str()
                    try:
                        ascii_char = str(hex(int(byte,2)))[2:].decode("hex")
                    except:
                        ascii_char = "Non char"
                    print byte + "\t" + str(hex_byte) + "\t" + ascii_char
                    byte = ""
                    i = 0
                j += 1
            print "----------------------------------"
            if CheckPacket(p, sync_bytes, address, message, crc):
                print "Result: Valid"
                valid_packets.append(packet_counter)
            else:
                print "Result: Invalid"
            
            packet_counter += 1
        print "###################################"
    
    if print_statistics:
        if not print_pkt_bytes:
            for p in packets:
                if CheckPacket(p, sync_bytes, address, message, crc):
                    valid_packets.append(packet_counter)
                packet_counter += 1
            
        print "\n###################################"
        print "-- Statistics ---------------------"
        print "###################################"
        print "Number of packets:\t\t" + str(packet_counter)
        print "Valid packets:\t\t\t" + str(valid_packets)
        print "Number of valid packets:\t" + str(len(valid_packets))
        print "Number of lost packets:\t\t" + str(packet_counter-len(valid_packets))
        print "Lost percentage:\t\t" + str(100.0 - 100.0*len(valid_packets)/packet_counter) if packet_counter > 0 else "0"
        print "###################################"
    
    return 0

if __name__ == '__main__':
    import sys
    sys.exit(main(sys.argv))
