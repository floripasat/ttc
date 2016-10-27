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

def crc16(s):
    crcValue = 0x0000
    crc16tab = (0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280,
                0xC241, 0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481,
                0x0440, 0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81,
                0x0E40, 0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880,
                0xC841, 0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81,
                0x1A40, 0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80,
                0xDC41, 0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680,
                0xD641, 0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081,
                0x1040, 0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281,
                0x3240, 0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480,
                0xF441, 0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80,
                0xFE41, 0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881,
                0x3840, 0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80,
                0xEA41, 0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81,
                0x2C40, 0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681,
                0x2640, 0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080,
                0xE041, 0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281,
                0x6240, 0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480,
                0xA441, 0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80,
                0xAE41, 0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881,
                0x6840, 0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80,
                0xBA41, 0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81,
                0x7C40, 0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681,
                0x7640, 0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080,
                0xB041, 0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280,
                0x9241, 0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481,
                0x5440, 0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81,
                0x5E40, 0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880,
                0x9841, 0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81,
                0x4A40, 0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80,
                0x8C41, 0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680,
                0x8641, 0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081,
                0x4040)
    
    for ch in s:
        if type(s) is str:
            tmp = crcValue^(ord(ch))
        elif type(s) is list:
            tmp = crcValue^(ch)
        crcValue = (crcValue >> 8)^crc16tab[(tmp & 0xFF)]
        
    return crcValue


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
    if packet[0:8] == str(bin(sync_bytes[0])[2:].zfill(8)):
        if packet[8:16] == str(bin(sync_bytes[1])[2:].zfill(8)):
            if packet[16:24] == str(bin(sync_bytes[2])[2:].zfill(8)):
                if packet[24:32] == str(bin(sync_bytes[3])[2:].zfill(8)):
                    if packet[32:40] == str(bin(address)[2:].zfill(8)):
                        if packet[-16:] == crc:
                            return True
    return False
    
    
def main(args):
    path = str()
    if len(args) == 1:
        path = "/code/gnuradio/binData.bin"
    else:
        path = args[1]

#****************************************************
#-- INPUTS ------------------------------------------
#****************************************************
    preamble_byte       = 0xAA
    preamble_size       = 4
    sync_bytes          = [0x75,0x06,0x25,0x45]
    address             = 0x39
    message             = "FloripaSat"
    others_bytes        = [0x00]    # If there is an additional byte, write it here
    print_expected_pkt  = True
    print_bin_str       = False
    print_packets       = False
    print_pkt_bytes     = True
    print_statistics    = True
    save_results        = True
#****************************************************
    
    preamble        = preamble_size*str(bin(preamble_byte))[2:]
    sync_word_size  = len(sync_bytes)
    address_size    = 1
    others_size     = len(others_bytes)
    message_size    = len(message)
    crc16_data = [address]
    for c in message:
        crc16_data.append(ord(c))
    crc             = str(bin(crc16(crc16_data))[2:].zfill(16))
    crc_size        = len(crc)/8

    packet_size = preamble_size + sync_word_size + address_size + message_size + crc_size + others_size
    packet_size *= 8        # Bytes to bits

    if print_expected_pkt:
        print "###################################"
        print "-- Expected Packet ----------------"
        print "###################################"
        print "Preamble:\t" + preamble
        print "Sync. word:\t" + str(bin(sync_bytes[0])[2:].zfill(8)) + str(bin(sync_bytes[1])[2:].zfill(8)) + str(bin(sync_bytes[2])[2:].zfill(8)) + str(bin(sync_bytes[3])[2:].zfill(8))
        print "Address:\t" + str(bin(address)[2:].zfill(8))
        print "Others bytes:\t" + str(bin(others_bytes[0])[2:].zfill(8))
        print "Message:\t" + message
        print "CRC16:\t\t" + crc + " (" + str(crc16(crc16_data)) + ")"
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
