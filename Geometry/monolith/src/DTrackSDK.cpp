/* DTrackSDK: C++ source file, A.R.T. GmbH 3.5.07-09.07.10
 *
 * DTrackSDK: functions to receive and process DTrack UDP packets (ASCII protocol), as
 * well as to exchange DTrack2 TCP command strings.
 * Copyright (C) 2007-2010, Advanced Realtime Tracking GmbH
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *
 * Version v2.0
 *
 * Purpose:
 *  - receives DTrack UDP packets (ASCII protocol) and converts them into easier to handle data
 *  - sends and receives DTrack2 commands (TCP)
 *  - DTrack2 network protocol due to: 'Technical Appendix DTrack v2.0'
 *  - for ARTtrack Controller versions v0.2 (and compatible versions)
 *  - tested under Linux (gcc) and MS Windows 2000/XP (MS Visual C++)
 *
 * $Id: DTrackSDK.cpp,v 1.13 2010/12/13 14:02:21 sebastianz Exp $
 */

#include "DTrackSDK.hpp"

#include <iostream>
#include <sstream>

// Constructor
DTrackSDK::DTrackSDK(const std::string& server_host,
		unsigned short server_port,
		unsigned short data_port,
		RemoteSystemType remote_type,
		int data_bufsize,
		int data_timeout_us,
		int server_timeout_us)
{
	init(remote_type, server_host, server_port, data_port, data_bufsize, data_timeout_us, server_timeout_us);
}

// Constructor (DTrack2)
DTrackSDK::DTrackSDK(const std::string& server_host)
{
	init(SYS_DTRACK_UNKNOWN, server_host);
}

// Constructor (DTrack)
DTrackSDK::DTrackSDK(const std::string& server_host, unsigned short server_port, unsigned short data_port)
{
	init(SYS_DTRACK_UNKNOWN, server_host, server_port, data_port);
}

//	Init function, called from constructor.
void DTrackSDK::init(RemoteSystemType sysType, const std::string& server_host, unsigned short server_port, unsigned short data_port,
		int data_bufsize, int data_timeout_us, int server_timeout_us
)
{
	rsType = sysType;
	int err;

	d_udpsock = NULL;
	d_tcpsock = NULL;
	d_udpbuf = NULL;

	lastDataError = ERR_NONE;
	lastServerError = ERR_NONE;
	setLastDTrackError();

	d_udptimeout_us = data_timeout_us;
	d_tcptimeout_us = server_timeout_us;
	d_remoteport = server_port;

	// parse remote address if available
	d_remote_ip = 0;
	if (!server_host.empty()) {
		d_remote_ip = ip_name2ip(server_host.c_str());
	}

	// create UDP socket:
	d_udpport = data_port;

	if ((d_remote_ip != 0) && (server_port == 0)) { // listen to multicast case
		err = udp_init(&d_udpsock, &d_udpport, d_remote_ip);
	} else { // normal case
		err = udp_init(&d_udpsock, &d_udpport);
	}
	if (err) {
		d_udpsock = NULL;
		d_udpport = 0;
		return;
	}

	// create UDP buffer:
	d_udpbufsize = data_bufsize;
	d_udpbuf = (char *)malloc(data_bufsize);
	if (!d_udpbuf) {
		udp_exit(d_udpsock);
		d_udpsock = NULL;
		d_udpport = 0;
		return;
	}

	if ((d_remote_ip != 0) && (server_port == 0)) { // multicast
		d_remoteport = 0;
	} else {
		if (rsType != SYS_DTRACK) {
			err = tcp_client_init(&d_tcpsock, d_remote_ip, server_port);
			if (err) {  // no connection to DTrack2 server
				// on error assuming DTrack 1 if system is unknown
				if (rsType == SYS_DTRACK_UNKNOWN)
				{
					rsType = SYS_DTRACK;
					// DTrack 1 controller will not listen to port 50105 -> adjust port to DTrack 1 default
					d_remoteport = 5001;
				}
			} else {
				// TCP connection up, should be DTrack2
				rsType = SYS_DTRACK_2;
			}
		}
	}
	// reset actual DTrack data:
	act_framecounter = 0;
	act_timestamp = -1;

	act_num_body = act_num_flystick = act_num_meatool = act_num_hand = 0;
	act_num_marker = 0;

	d_message_origin = "";
	d_message_status = "";
	d_message_framenr = 0;
	d_message_errorid = 0;
	d_message_msg = "";
}

// Destructor
DTrackSDK::~DTrackSDK()
{
	// release buffer:
	if (d_udpbuf) {
		free(d_udpbuf);
	}
	// release sockets:
	if (d_udpsock) {
		if ((d_remote_ip != 0) && (d_remoteport == 0)) {
			udp_exit(d_udpsock, d_remote_ip);
		} else {
			udp_exit(d_udpsock);
		}
		d_udpsock = NULL;
	}
	if (d_tcpsock) {
		tcp_exit(d_tcpsock);
		d_tcpsock = NULL;
	}
}

// Get current remote system type (e.g. DTrack, DTrack2).
DTrackSDK::RemoteSystemType DTrackSDK::getRemoteSystemType()
{
	return rsType;
}

// Get last error as error code (data).
DTrackSDK::Errors DTrackSDK::getLastDataError()
{
	return lastDataError;
}

// Get last error as error code (server d).
DTrackSDK::Errors DTrackSDK::getLastServerError()
{
	return lastServerError;
}

// Set last DTrack error codestd::cout << "s1" << std::endl;.
void DTrackSDK::setLastDTrackError(int newError, std::string newErrorString)
{
	lastDTrackError = newError;
	lastDTrackErrorString = newErrorString;
}

// Get last DTrack error code.
int DTrackSDK::getLastDTrackError()
{
	return lastDTrackError;
}

//	Get last DTrack error description.
std::string DTrackSDK::getLastDTrackErrorDescription()
{
	return lastDTrackErrorString;
}

// Is UDP connection valid?
bool DTrackSDK::isUDPValid()
{
	return (d_udpsock != NULL);
}

// Is TCP connection still valid?

bool DTrackSDK::isTCPValid()
{
	return (d_tcpsock != NULL);
}

// Receive and process one DTrack data packet (UDP; ASCII protocol)
bool DTrackSDK::receive()
{
	char* s;
	int i, j, k, l, n, len, id;
	char sfmt[20];
	int iarr[3];
	double d, darr[6];
	int loc_num_bodycal, loc_num_handcal, loc_num_flystick1, loc_num_meatool;

	lastDataError = ERR_NONE;
	lastServerError = ERR_NONE;

	if (!isUDPValid()) {
		lastDataError = ERR_NET;
        printf("d_udpsock is null\n");
		return false;
	}
	// defaults:
	act_framecounter = 0;
	act_timestamp = -1;   // i.e. not available
	loc_num_bodycal = loc_num_handcal = -1;  // i.e. not available
	loc_num_flystick1 = loc_num_meatool = 0;

	// receive UDP packet:
	len = udp_receive(d_udpsock, d_udpbuf, d_udpbufsize-1, d_udptimeout_us);
	if (len == -1) {
		lastDataError = ERR_TIMEOUT;
		return false;
	}

	if (len <= 0) {
		lastDataError = ERR_NET;
        printf("udp_recieve: %i\n", len);
		return false;
	}

	s = d_udpbuf;
	s[len] = '\0';

	// process lines:
	lastDataError = ERR_PARSE;

	do {
		// line for frame counter:
		if (!strncmp(s, "fr ", 3)) {
			s += 3;
			if (!(s = string_get_ui(s, &act_framecounter))) {
				act_framecounter = 0;
				return false;
			}
			continue;
		}
		// line for timestamp:
		if (!strncmp(s, "ts ", 3)) {
			s += 3;
			if (!(s = string_get_d(s, &act_timestamp)))	{
				act_timestamp = -1;
				return false;
			}
			continue;
		}
		// line for additional information about number of calibrated bodies:
		if (!strncmp(s, "6dcal ", 6)) {
			s += 6;
			if (!(s = string_get_i(s, &loc_num_bodycal))) {
				return false;
			}
			continue;
		}
		// line for standard body data:
		if (!strncmp(s, "6d ", 3)) {
			s += 3;
			// disable all existing dataargc != 2
			for (i=0; i<act_num_body; i++) {
				memset(&act_body[i], 0, sizeof(DTrack_Body_Type));
				act_body[i].id = i;
				act_body[i].quality = -1;
			}
			// get number of standard bodies (in line)
			if (!(s = string_get_i(s, &n))) {
				return false;
			}
			// get data of standard bodies
			for (i=0; i<n; i++) {
				if (!(s = string_get_block(s, "id", &id, NULL, &d))) {
					return false;
				}
				// adjust length of vector
				if (id >= act_num_body) {
					act_body.resize(id + 1);
					for (j = act_num_body; j<=id; j++) {
						memset(&act_body[j], 0, sizeof(DTrack_Body_Type));
						act_body[j].id = j;
						act_body[j].quality = -1;
					}
					act_num_body = id + 1;
				}
				act_body[id].id = id;
				act_body[id].quality = d;
				if (!(s = string_get_block(s, "ddd", NULL, NULL, act_body[id].loc))) {
					return false;
				}
				if (!(s = string_get_block(s, "ddddddddd", NULL, NULL, act_body[id].rot))) {
					return false;
				}
			}
			continue;
		}

		// line for Flystick data (older format):
		if (!strncmp(s, "6df ", 4)) {
			s += 4;
			// get number of calibrated Flysticks
			if (!(s = string_get_i(s, &n))) {
				return false;
			}
			loc_num_flystick1 = n;
			// adjust length of vector
			if (n != act_num_flystick) {
				act_flystick.resize(n);
				act_num_flystick = n;
			}
			// get data of Flysticks
			for (i=0; i<n; i++) {
				if (!(s = string_get_block(s, "idi", iarr, NULL, &d))) {
					return false;
				}
				if (iarr[0] != i) {	// not expected
					return false;
				}
				act_flystick[i].id = iarr[0];
				act_flystick[i].quality = d;
				act_flystick[i].num_button = 8;
				k = iarr[1];
				for (j=0; j<8; j++) {
					act_flystick[i].button[j] = k & 0x01;
					k >>= 1;
				}
				act_flystick[i].num_joystick = 2;  // additionally to buttons 5-8
				if (iarr[1] & 0x20) {
					act_flystick[i].joystick[0] = -1;
				} else
				if (iarr[1] & 0x80) {
					act_flystick[i].joystick[0] = 1;
				} else {
					act_flystick[i].joystick[0] = 0;
				}
				if(iarr[1] & 0x10){
					act_flystick[i].joystick[1] = -1;
				}else if(iarr[1] & 0x40){
					act_flystick[i].joystick[1] = 1;
				}else{
					act_flystick[i].joystick[1] = 0;
				}
				if (!(s = string_get_block(s, "ddd", NULL, NULL, act_flystick[i].loc))) {
					return false;
				}
				if (!(s = string_get_block(s, "ddddddddd", NULL, NULL, act_flystick[i].rot))) {
					return false;
				}
			}
			continue;
		}

		// line for Flystick data (newer format):
		if (!strncmp(s, "6df2 ", 5)) {
			s += 5;
			// get number of calibrated Flysticks
			if (!(s = string_get_i(s, &n))) {
				return false;
			}
			// adjust length of vector
			if (n != act_num_flystick) {
				act_flystick.resize(n);
				act_num_flystick = n;
			}
			// get number of Flysticks
			if (!(s = string_get_i(s, &n))) {
				return false;
			}
			// get data of Flysticks
			for (i=0; i<n; i++) {
				if (!(s = string_get_block(s, "idii", iarr, NULL, &d))) {
					return false;
				}
				if (iarr[0] != i) {  // not expected
					return false;
				}
				act_flystick[i].id = iarr[0];
				act_flystick[i].quality = d;
				if ((iarr[1] > DTRACK_FLYSTICK_MAX_BUTTON)||(iarr[1] > DTRACK_FLYSTICK_MAX_JOYSTICK)) {
					return false;
				}
				act_flystick[i].num_button = iarr[1];
				act_flystick[i].num_joystick = iarr[2];
				if (!(s = string_get_block(s, "ddd", NULL, NULL, act_flystick[i].loc))){
					return false;
				}
				if (!(s = string_get_block(s, "ddddddddd", NULL, NULL, act_flystick[i].rot))){
					return false;
				}
				strcpy(sfmt, "");
				j = 0;
				while (j < act_flystick[i].num_button) {
					strcat(sfmt, "i");
					j += 32;
				}
				j = 0;
				while (j < act_flystick[i].num_joystick) {
					strcat(sfmt, "d");
					j++;
				}
				if (!(s = string_get_block(s, sfmt, iarr, NULL, act_flystick[i].joystick))) {
					return false;
				}
				k = l = 0;
				for (j=0; j<act_flystick[i].num_button
				; j++) {
					act_flystick[i].button[j] = iarr[k] & 0x01;
					iarr[k] >>= 1;
					l++;
					if (l == 32) {
						k++;
						l = 0;
					}
				}
			}
			continue;
		}

		// line for measurement tool data:
		if (!strncmp(s, "6dmt ", 5)) {
			s += 5;
			// get number of calibrated measurement tools
			if (!(s = string_get_i(s, &n))) {
				return false;
			}
			loc_num_meatool = n;
			// adjust length of vector
			if (n != act_num_meatool) {
				act_meatool.resize(n);
				act_num_meatool = n;
			}
			// get data of measurement tools
			for (i=0; i<n; i++) {
				if (!(s = string_get_block(s, "idi", iarr, NULL, &d))) {
					return false;
				}
				if (iarr[0] != i) {  // not expected
					return false;
				}
				act_meatool[i].id = iarr[0];
				act_meatool[i].quality = d;
				act_meatool[i].num_button = 1;
				act_meatool[i].button[0] = iarr[1] & 0x01;
				if (!(s = string_get_block(s, "ddd", NULL, NULL, act_meatool[i].loc))) {
					return false;
				}
				if (!(s = string_get_block(s, "ddddddddd", NULL, NULL, act_meatool[i].rot))) {
					return false;
				}
			}
			continue;
		}

		// line for additional information about number of calibrated Fingertracking hands:
		if (!strncmp(s, "glcal ", 6)) {
			s += 6;
			if (!(s = string_get_i(s, &loc_num_handcal))) {	// get number of calibrated hands
				return false;
			}
		}

		// line for A.R.T. Fingertracking hand data:
		if (!strncmp(s, "gl ", 3)) {
			s += 3;
			// disable all existing data
			for (i=0; i<act_num_hand; i++) {
				memset(&act_hand[i], 0, sizeof(DTrack_Hand_Type));
				act_hand[i].id = i;
				act_hand[i].quality = -1;
			}
			// get number of hands (in line)
			if (!(s = string_get_i(s, &n))) {
				return false;
			}
			// get data of hands
			for (i=0; i<n; i++) {
				if (!(s = string_get_block(s, "idii", iarr, NULL, &d))){
					return false;
				}
				id = iarr[0];
				if (id >= act_num_hand) {  // adjust length of vector
					act_hand.resize(id + 1);
					for (j=act_num_hand; j<=id; j++) {
						memset(&act_hand[j], 0, sizeof(DTrack_Hand_Type));
						act_hand[j].id = j;
						act_hand[j].quality = -1;
					}
					act_num_hand = id + 1;
				}
				act_hand[id].id = iarr[0];
				act_hand[id].lr = iarr[1];
				act_hand[id].quality = d;
				if (iarr[2] > DTRACK_HAND_MAX_FINGER) {
					return false;
				}
				act_hand[id].nfinger = iarr[2];
				if (!(s = string_get_block(s, "ddd", NULL, NULL, act_hand[id].loc))) {
					return false;

				}
				if (!(s = string_get_block(s, "ddddddddd", NULL, NULL, act_hand[id].rot))){
					return false;
				}
				// get data of fingers
				for (j = 0; j < act_hand[id].nfinger; j++) {
					if (!(s = string_get_block(s, "ddd", NULL, NULL, act_hand[id].finger[j].loc))) {
						return false;
					}
					if (!(s = string_get_block(s, "ddddddddd", NULL, NULL, act_hand[id].finger[j].rot))){
						return false;
					}
					if (!(s = string_get_block(s, "dddddd", NULL, NULL, darr))){
						return false;
					}
					act_hand[id].finger[j].radiustip = darr[0];
					act_hand[id].finger[j].lengthphalanx[0] = darr[1];
					act_hand[id].finger[j].anglephalanx[0] = darr[2];
					act_hand[id].finger[j].lengthphalanx[1] = darr[3];
					act_hand[id].finger[j].anglephalanx[1] = darr[4];
					act_hand[id].finger[j].lengthphalanx[2] = darr[5];
				}
			}
			continue;
		}

		// line for single marker data:
		if (!strncmp(s, "3d ", 3)) {
			s += 3;
			// get number of markers
			if (!(s = string_get_i(s, &act_num_marker))) {
				act_num_marker = 0;
				return false;
			}
			if (act_num_marker > (int )act_marker.size()) {
				act_marker.resize(act_num_marker);
			}
			// get data of single markers
			for (i=0; i<act_num_marker; i++) {
				if (!(s = string_get_block(s, "id", &act_marker[i].id, NULL, &act_marker[i].quality))) {
					return false;
				}
				if (!(s = string_get_block(s, "ddd", NULL, NULL, act_marker[i].loc))) {
					return false;
				}
			}
			continue;
		}

		// ignore unknown line identifiers (could be valid in future DTracks)
	} while((s = string_nextline(d_udpbuf, s, d_udpbufsize)));

	// set number of calibrated standard bodies, if necessary:
	if (loc_num_bodycal >= 0) {	// '6dcal' information was available
		n = loc_num_bodycal - loc_num_flystick1 - loc_num_meatool;
		if (n > act_num_body) {  // adjust length of vector
			act_body.resize(n);
			for (j=act_num_body; j<n; j++) {
				memset(&act_body[j], 0, sizeof(DTrack_Body_Type));
				act_body[j].id = j;
				act_body[j].quality = -1;
			}
		}
		act_num_body = n;
	}

	// set number of calibrated Fingertracking hands, if necessary:
	if (loc_num_handcal >= 0) {  // 'glcal' information was available
		if (loc_num_handcal > act_num_hand) {  // adjust length of vector
			act_hand.resize(loc_num_handcal);
			for (j=act_num_hand; j<loc_num_handcal; j++) {
				memset(&act_hand[j], 0, sizeof(DTrack_Hand_Type));
				act_hand[j].id = j;
				act_hand[j].quality = -1;
			}
		}
		act_num_hand = loc_num_handcal;
	}

	lastDataError = ERR_NONE;
	return true;
}

// Get number of calibrated standard bodies (as far as known)
int DTrackSDK::getNumBody()
{
	return act_num_body;
}

// Get standard body data (id (i): standard body id 0..max-1)
DTrack_Body_Type_d* DTrackSDK::getBody(int id)
{
	if ((id >= 0) && (id < act_num_body))
		return &act_body.at(id);
	return NULL;
}

// Get number of calibrated Flysticks
int DTrackSDK::getNumFlyStick()
{
	return act_num_flystick;
}

// Get Flystick data (id (i): Flystick id 0..max-1)
DTrack_FlyStick_Type_d* DTrackSDK::getFlyStick(int id)
{
	if ((id >= 0) && (id < act_num_flystick))
		return &act_flystick.at(id);
	return NULL;
}

// Get number of calibrated measurement tools
int DTrackSDK::getNumMeaTool()
{
	return act_num_meatool;
}

// Get measurement tool data (id (i): tool id 0..max-1)
DTrack_MeaTool_Type_d* DTrackSDK::getMeaTool(int id)
{
	if ((id >= 0) && (id < act_num_meatool))
		return &act_meatool.at(id);
	return NULL;
}

// Get number of calibrated Fingertracking hands (as far as known)
int DTrackSDK::getNumHand()
{
	return act_num_hand;
}

// Get Fingertracking hand data (id (i): hand id 0..max-1)
DTrack_Hand_Type_d* DTrackSDK::getHand(int id)
{
	if ((id >= 0) && (id < act_num_hand))
		return &act_hand.at(id);
	return NULL;
}

// Get number of tracked single markers
int DTrackSDK::getNumMarker()
{
	return act_num_marker;
}

// Get single marker data
DTrack_Marker_Type_d* DTrackSDK::getMarker(int index)
{
	if ((index >= 0) && (index < act_num_marker))
		return &act_marker.at(index);
	return NULL;
}

// Get frame counter
unsigned int DTrackSDK::getFrameCounter()
{
	return act_framecounter;
}

// Get timestamp (-1 if information not available)
double DTrackSDK::getTimeStamp()
{
	return act_timestamp;
}

// Send DTrack command (UDP)
bool DTrackSDK::sendCommand(const std::string& command)
{
	if (!isUDPValid())
		return false;
	// dest is dtrack2
	if (rsType == SYS_DTRACK_2)	{
		// command style is dtrack?
		if (0 == strncmp(command.c_str(), "dtrack ", 7)) {
			std::string c = command.substr(7);
			// start measurement
			if (	(0 == strncmp(c.c_str(), "10 1",4))
				||	(0 == strncmp(c.c_str(), "10 3",4))
				||	(0 == strncmp(c.c_str(), "31",2)))
			{
				startMeasurement();
			}
			// stop measurement
			if (	(0 == strncmp(c.c_str(), "10 0",4))
				||	(0 == strncmp(c.c_str(), "32",2)))
			{
				stopMeasurement();
			}
		}
	}
	if (udp_send(d_udpsock, (void*)command.c_str(), (unsigned int)command.length() + 1, d_remote_ip, d_remoteport, d_udptimeout_us))
	{
		lastDataError = ERR_NET;
		return false;
	}
	lastDataError = ERR_NONE;
	return true;
}

// Send command to DTrack and receive answer (TCP)
int DTrackSDK::sendCommandReceive(const std::string& command, char* answer)
{
	int len, i;
	char ans[DTRACK_PROT_MAXLEN + 1];
	setLastDTrackError();
	// Commands over TCP are not supported in DTrack 1
	if ((rsType == SYS_DTRACK)||(rsType == SYS_DTRACK_UNKNOWN))
		return -2;
	// command too large?
	if (command.length() > DTRACK_PROT_MAXLEN) {
		lastServerError = ERR_NET;
		return -3;
	}
	if (!isTCPValid()) {
		lastServerError = ERR_NET;
		return -10;
	}
	// send TCP command string:
	if ((len = tcp_send(d_tcpsock, command.c_str(), command.length()+1, d_tcptimeout_us))) {
		lastServerError = ERR_NET;
		return -11;
	}
	// receive TCP response string:
	if ((len = tcp_receive(d_tcpsock, ans, DTRACK_PROT_MAXLEN, d_tcptimeout_us)) < 0) {
		if (len == -1)
		{
			lastServerError = ERR_TIMEOUT;
		} else if(len == -9) {	// broken connection
			tcp_exit(d_tcpsock);
			d_tcpsock = NULL;
		} else {	// network error
			lastServerError = ERR_NET;
		}
		if (answer)
			strcpy(answer, "");
		return len;
	}
	// got ok msg?
	if (0 == strcmp(ans, "dtrack2 ok")) {	// no error
		setLastDTrackError();
		return 1;
	}
	// got error msg?
	if (0 == strncmp(ans, "dtrack2 err ", 12)) {
		char *s = ans + 12;
		if (!(s = string_get_i((char *)s, &i))) {	// get 'error code'
			setLastDTrackError(-1100, "SDK error -1100");
			lastServerError = ERR_PARSE;
			return -1100;
		}
		lastDTrackError = i;
		if (!(s = string_get_quoted_text((char *)s, lastDTrackErrorString))) {	// get 'error string'
			setLastDTrackError(-1100, "SDK error -1100");
			lastServerError = ERR_PARSE;
			return -1101;
		}
		return 2;
	}
	// return msg
	if (answer)
	{
		strncpy(answer, ans, len);
		answer[len] = '\0';  // for safety
	}
	lastServerError = ERR_NONE;
	return 0;
}

// Set DTrack parameter.
bool DTrackSDK::setParam(const std::string& category, const std::string& name, const std::string& value)
{
	return sendCommand("set " + category + " " + name + " " + value);
}

// Set DTrack parameter.
bool DTrackSDK::setParam(const std::string& parameter)
{
	// Params over TCP are not supported in DTrack 1
	if ((rsType == SYS_DTRACK)||(rsType == SYS_DTRACK_UNKNOWN))
		return false;
	char res[DTRACK_PROT_MAXLEN + 1];
	std::string cmd = "dtrack2 set " + parameter;
	setLastDTrackError();
	if (0 > sendCommandReceive(cmd.c_str(), res))
		return false;
	if (0 != strcmp(res, cmd.c_str())) {
		lastServerError = ERR_PARSE;
		return false;
	}
	return true;
}

// Get DTrack2 parameter.
bool DTrackSDK::getParam(const std::string& category, const std::string& name, std::string& value)
{
	return getParam(category + " " + name, value);
}

// Get DTrack2 parameter.
bool DTrackSDK::getParam(const std::string& parameter, std::string& value)
{
	// Params over TCP are not supported in DTrack 1
	if ((rsType == SYS_DTRACK)||(rsType == SYS_DTRACK_UNKNOWN))
		return false;
	char res[DTRACK_PROT_MAXLEN];
	std::string cmd = "dtrack2 get " + parameter;
	setLastDTrackError();
	if (0 > sendCommandReceive(cmd.c_str(), res)) {
		return false;
	}
	if (0 == strncmp(res, "dtrack2 set ", 12)) {
		char* s = res + 12;
		if (!(s = string_cmp_parameter(s, parameter.c_str()))) {  // check 'parameter'
			lastServerError = ERR_PARSE;
			return false;
		}
		value = s;
		return true;
	}
	return false;
}

// Get DTrack message
bool DTrackSDK::getMessage()
{
	// Messages over TCP are not supported in DTrack 1
	if ((rsType == SYS_DTRACK)||(rsType == SYS_DTRACK_UNKNOWN))
		return false;
	char res[DTRACK_PROT_MAXLEN];
	const char* s;
	unsigned int ui;

	setLastDTrackError();
	if (0 > sendCommandReceive("dtrack2 getmsg", res))
		return false;

	// we got an answer, we know nothing about
	if (0 != strncmp(res, "dtrack2 msg ", 12))
		return false;

	// message
	s = res + 12;
	if (!(s = string_get_word((char *)s, d_message_origin)))	// get 'origin'
		return false;

	if (!(s = string_get_word((char *)s, d_message_status)))	// get 'status'
			return false;

	if(!(s = string_get_ui((char *)s, &ui)))	// get 'frame counter'
		return false;
	d_message_framenr = ui;

	if(!(s = string_get_ui((char *)s, &ui)))	// get 'error id'
		return false;
	d_message_errorid = ui;

	if(!(s = string_get_quoted_text((char *)s, d_message_msg)))	// get 'message'
		return false;

	return true;
}

// Get data port where tracking data is received.
unsigned short DTrackSDK::getDataPort()
{
	return d_udpport;
}

// Get origin of last DTrack message.
std::string DTrackSDK::getMessageOrigin()
{
	return d_message_origin;
}

// Get status of last DTrack message.
std::string DTrackSDK::getMessageStatus()
{
	return d_message_status;
}

// Get frame counter of last DTrack message.
unsigned int DTrackSDK::getMessageFrameNr()
{
	return d_message_framenr;
}

// Get error id of last DTrack message.
unsigned int DTrackSDK::getMessageErrorId()
{
	return d_message_errorid;
}

// Get message string of last DTrack message.
std::string DTrackSDK::getMessageMsg()
{
	return d_message_msg;
}

// Start measurement.
bool DTrackSDK::startMeasurement(unsigned int channel)
{
	std::stringstream command;
	if (rsType == SYS_DTRACK)	{
		return (sendCommand("dtrack 10 3")) && (sendCommand("dtrack 31"));
	} else {	//DTrack2
		// Measurement already started, but not stopped
		if (channelBackup == 0)
		{
			channelBackup = channel;
			if (channelBackup > 5)
				channelBackup = 0;
			command.str("");
			command << "dtrack2 set output net ch0" << channelBackup << " udp my_ip " << getDataPort();  // send tracking data to this application
			if (channelBackup > 0)
			{
				if (0 > sendCommandReceive(command.str(), NULL) || (lastDTrackError != 0))
				{
					std::cout << lastDTrackErrorString << std::endl;
					return false;
				}
			}
		}
		if (0 > sendCommandReceive("dtrack2 tracking start", NULL))
		{
			std::cout << lastDTrackErrorString << std::endl;
			return false;
		}
	}
	return true;
}

// Stop measurement.
bool DTrackSDK::stopMeasurement()
{
	std::stringstream command;
	if (rsType == SYS_DTRACK)	{
		return (sendCommand("dtrack 32")) && (sendCommand("dtrack 10 0"));
	} else {	//DTrack2
		if (channelBackup != 0)
		{
			command.str("");
			command << "dtrack2 set output net ch0" << channelBackup << " none";  // send tracking data to this application
			channelBackup = 0;
			if ((0 > sendCommandReceive(command.str(), NULL)) || (lastDTrackError != 0)) {
				return false;
			}
		}
		return (0 <= sendCommandReceive("dtrack2 tracking stop", NULL)) && (lastDTrackError == 0);
	}
}
