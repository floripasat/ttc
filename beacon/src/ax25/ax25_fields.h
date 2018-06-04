/*
 * ax25_fields.h
 * 
 * Copyright (C) 2017, Federal University of Santa Catarina.
 * 
 * This file is part of FloripaSat-Beacon.
 * 
 * FloripaSat-Beacon is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * FloripaSat-Beacon is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with FloripaSat-Beacon. If not, see <http://www.gnu.org/licenses/>.
 * 
 */
 
/**
 * \brief AX25 protocol header fields.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 08/09/2017
 * 
 * \defgroup ax25_fields Fields
 * \ingroup ax25
 * \{
 */

#ifndef AX25_FIELDS_H_
#define AX25_FIELDS_H_

/**
 * \brief Flag Field.
 * 
 * The flag field is one octet long. Because the flag delimits frames, it occurs at both the
 * beginning and end of each frame. Two frames may share one flag, which would denote
 * the end of the first frame and the start of the next frame. A flag consists of a zero followed
 * by six ones followed by another zero, or 01111110 (7E hex). As a result of bit stuffing,
 * this sequence is not allowed to occur anywhere else inside a complete frame.
 */
#define AX25_FLAG                                   0x7E

/*
 * \brief Address Field Enconding.
 * 
 * The address field identifies both the source of the frame and its destination. In addition,
 * the address field contains the command/response information and facilities for Layer 2
 * repeater operation.
 * 
 * The address field of all frames consists of a destination, source and (optionally) two
 * Layer 2 repeater subfields. Each subfield consists of an amateur call sign and a
 * Secondary Station Identifier (SSID). The call sign is made up of upper-case alpha and
 * numeric ASCII characters only. The SSID is a four-bit integer that uniquely identifies
 * multiple stations using the same amateur call sign.
 * 
 * The HDLC address field is extended beyond one octet by assigning the least-
 * significant bit of each octet to be an "extension bit". The extension bit of each octet is set
 * to "0" to indicate the next octet contains more address information, or to "1", to indicate
 * that this is the last octet of the HDLC address field. To make room for this extension bit,
 * the amateur radio call sign information is shifted one bit left.
 */
#define AX25_ADR_NON_REPEATER_SSID                  0x60    /**< If Layer 2 repeaters are not being used. */
#define AX25_ADR_REPEATER_SSID                      0x61    /**< If Layer 2 repeaters are being used. */
#define AX25_ADR_DST_COMMAND                        0x80    /**< Destination SSID C-Bit as command frame. */
#define AX25_ADR_DST_RESPONSE                       0x00    /**< Destination SSID C-Bit as response frame. */
#define AX25_ADR_SRC_COMMAND                        0x00    /**< Source SSID C-Bit as command frame. */
#define AX25_ADR_SRC_RESPONSE                       0x80    /**< Source SSID C-Bit as response frame. */

/*
 * \brief Control Fields.
 * 
 * The control field identifies the type of frame being sent.
 * The control fields in AX.25 are modeled after the ISO HDLC
 * balanced operation control fields.
 */

/*
 * \brief Control Fields Formats.
 * 
 * The three formats of control fields used in AX.25 are the:
 *      - Information frame (I frame)
 *      - Supervisory frame (S frame)
 *      - Unnumbered frame (U frame)
 *      .
 */
#define AX25_CTRL_INFORMATION                       0x00    /**< Information Frame (I). */
#define AX25_CTRL_SUPERVISORY                       0x01    /**< Supervisory Frame (S). */
#define AX25_CTRL_UNNUMBERED                        0x03    /**< Unnumbered Frame (U). */
#define AX25_CTRL_INFORMATION_128                   0x0000  /**< Information Frame modulo 128. */
#define AX25_CTRL_SUPERVISORY_128                   0x0001  /**< Supervisory Frame modulo 128. */

/*
 * \brief Supervisory Frame Control Field.
 */
#define AX25_CTRL_SUPERVISORY_RR                    0x01    /**< Receive Ready (RR) - System Ready To Receive. */
#define AX25_CTRL_SUPERVISORY_RNR                   0x05    /**< Receive Not Ready (RNR) - TNC Buffer Full. */
#define AX25_CTRL_SUPERVISORY_REJ                   0x09    /**< Reject (REJ) - Out of Sequence or Duplicate. */
#define AX25_CTRL_SUPERVISORY_SREJ                  0x0D    /**< Selective Reject (SREJ) - Request single frame repeat. */
#define AX25_CTRL_SUPERVISORY_RR_128                0x0001  /**< Receive Ready modulo 128. */
#define AX25_CTRL_SUPERVISORY_RNR_128               0x0005  /**< Receive Not Ready modulo 128. */
#define AX25_CTRL_SUPERVISORY_REJ_128               0x0009  /**< Reject modulo 128. */
#define AX25_CTRL_SUPERVISORY_SREJ_128              0x000D  /**< Selective Reject modulo 128. */

/*
 * \brief Unnumbered Frame Control Fields.
 * 
 * Unnumbered frame control fields are either commands or responses.
 */
#define AX25_CTRL_UNNUMBERED_SABME                  0x6F    /**< Set Asynchronous Balanced Mode Extended (SABME). */
#define AX25_CTRL_UNNUMBERED_SABM                   0x2F    /**< Set Asynchronous Balanced Mode (SABM). */
#define AX25_CTRL_UNNUMBERED_DISC                   0x43    /**< Disconnect (DISC). */
#define AX25_CTRL_UNNUMBERED_DM                     0x0F    /**< Disconnect Mode (DM). */
#define AX25_CTRL_UNNUMBERED_UA                     0x63    /**< Unnumbered Acknowledge (UA). */
#define AX25_CTRL_UNNUMBERED_FRMR                   0x87    /**< Frame Reject (FRMR). */
#define AX25_CTRL_UNNUMBERED_UI                     0x03    /**< Unnumered Information (UI). */
#define AX25_CTRL_UNNUMBERED_XID                    0xAF    /**< Exchange Identification (XID). */
#define AX25_CTRL_UNNUMBERED_TEST                   0xE3    /**< Test (TEST). */

/*
 * \brief Poll/Final (P/F) Bit Options.
 */
#define AX25_CTRL_PF_TRUE                           0x10    /**< Poll/Final bit true. */
#define AX25_CTRL_PF_FALSE                          0x00    /**< Poll/Final bit false. */
#define AX25_CTRL_PF_DISABLE                        0x00    /**< Poll/Final disabled. */

/*
 * \brief Protocol Identifier Field.
 * 
 * The Protocol Identifier (PID) field appears in information frames (I and UI) only.
 * It identifies which kind of Layer 3 protocol, if any, is in use.
 */
#define AX25_PID_AX25_LAYER_3_IMPLEMENTED           0x10    /**< AX.25 Layer Implemented. */
#define AX25_PID_ISO_8208_CCITT_X25_PLP             0x01    /**< ISO 8208/CCITT X.25 PLP. */
#define AX25_PID_COMPRESSED_TCP_IP_PKT_V_JACOBSON   0x06    /**< Compressed TCP/IP Packet Van Jacobson (RFC 1144). */
#define AX25_PID_UNCOMPRESSED_TCP_IP_PKT_V_JACOBSON 0x07    /**< Uncompressed TCP/IP Packet Van Jacobson (RFC 1144). */
#define AX25_PID_SEGMENTATION_FRAGMENT              0x08    /**< Segmentation Fragment. */
#define AX25_PID_TEXNET_DATAGRAM_PROTOCOL           0xC3    /**< TEXNET Datagram Protocol. */
#define AX25_PID_LINK_QUALITY_PROTOCOL              0xC4    /**< Link Quality Protocol. */
#define AX25_PID_APPLETALK                          0xCA    /**< Appletalk. */
#define AX25_PID_APPLETALK_ARP                      0xCB    /**< Appletalk ARP. */
#define AX25_PID_ARPA_INTERNET_PROTOCOL             0xCC    /**< ARPA Internet Protocol. */
#define AX25_PID_ARPA_ADR_RESOLUTION_PROTOCOL       0xCD    /**< ARPA Address Resolution Protocol. */
#define AX25_PID_FLEXNET                            0xCE    /**< FlexNet. */
#define AX25_PID_NET_ROM                            0xCF    /**< NET/ROM. */
#define AX25_PID_NO_LAYER_3                         0xF0    /**< No Layer 3 Protocol */
#define AX25_PID_ESCAPE_CHARACTER                   0xFF    /**< Escape Character - Next octet contains more Layer 3 protocol information. */

#endif // AX25_FIELDS_H_

//! \} End of ax25 group
