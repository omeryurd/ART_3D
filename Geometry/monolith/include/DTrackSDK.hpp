/* DTrackSDK: C++ header file, A.R.T. GmbH 03.05.07-09.07.10
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
 *
 * $Id: DTrackSDK.hpp,v 1.11 2010/11/12 09:44:45 sebastianz Exp $
 */

/*! \mainpage DTrackSDK Overview
 *
 * \section intro_sec Introduction
 *
 * The DTrackSDK provides an interface to control a DTrack/DTrack2 server and to receive tracking data.
 * Command and data exchange is done through an ASCII protocol.
 *
 * \section content_sec Contents
 *
 * This package consists of one main class: DTrackSDK. For new applications please use this class.
 *
 * Classes in folder 'Compatibility' provide legacy support for older SDK versions (DTrackLib, DTrack, DTrack2).
 *
 * Files in folder 'Lib' provide type definitions, basic UDP/TCP functionality and string parsing.
 */

#ifndef _ART_DTRACKSDK_HPP_
#define _ART_DTRACKSDK_HPP_

#include "DTrackDataTypes.h"
#include "DTrackNet.h"
#include "DTrackParse.hpp"

#include <string>
#include <vector>

//! Max message size
#define DTRACK_PROT_MAXLEN 200

/**
 * 	\brief DTrack SDK main class.
 */
class DTrackSDK
{
public:
	// Compatibility modes for older SDKs
	typedef enum {
		SYS_DTRACK_UNKNOWN = 0,
		SYS_DTRACK,
		SYS_DTRACK_2
	} RemoteSystemType;

	/**
	 * 	\brief	Constructor.
	 *
	 *	@param	server_host			hostname or IP address of ARTtrack Controller (empty if not used)
	 *	@param	server_port			port number of ARTtrack Controller (default is 50105)
	 *	@param	data_port			port number to receive tracking data from ARTtrack Controller (0 if to be chosen)
	 *	@param	remote_type			RemoteSystemType (default is SYS_DTRACK_UNKNOWN)
	 *	@param	data_bufsize		size of buffer for UDP packets (in bytes; default is 20000)
	 *	@param	data_timeout_us		timeout (receiving) in us (in micro second, default is 1s)
	 *	@param	server_timeout_us	timeout for access to ARTtrack Controller (in micro second; receiving and sending; default is 10s)
	 */
	DTrackSDK(const std::string& server_host,
			unsigned short server_port,
			unsigned short data_port,
			RemoteSystemType remote_type,
			int data_bufsize = 20000,
			int data_timeout_us = 1000000,
			int server_timeout_us = 10000000
	);

	/**
	 *	\brief	Constructor. Use for DTrack2.
	 *	@param	server_host			hostname or IP address of ARTtrack Controller (empty if not used)
	 */
	DTrackSDK(const std::string& server_host);

	/**
	 *	\brief	Constructor. Use for DTrack.
	 *	@param	server_host			hostname or IP address of ARTtrack Controller (empty if not used)
	 *	@param	server_port			port number of ARTtrack Controller (default is 50105)
	 *	@param	data_port			port number to receive tracking data from ARTtrack Controller (0 if to be chosen)
	 */
	DTrackSDK(const std::string& server_host, unsigned short server_port, unsigned short data_port = 0);

	/**
	 * 	\brief Destructor.
	 */
	~DTrackSDK();

	// Error codes
	typedef enum {
		ERR_NONE = 0,
		ERR_TIMEOUT,
		ERR_NET,
		ERR_PARSE,
	} Errors;

	/**
	 * 	\brief	Get current remote system type (e.g. DTrack, DTrack2).
	 */
	RemoteSystemType getRemoteSystemType();

	/**
	 * 	\brief	Get last error as error code (data).
	 * 	@return Error code (Success = Code 0).
	 */
	Errors getLastDataError();

	/**
	 * 	\brief	Get last error as error code (server).
	 * 	@return Error code (Success = Code 0).
	 */
	Errors getLastServerError();

	/**
	 * 	\brief	Get last DTrack error code.
	 * 	@return Error code.
	 */
	int getLastDTrackError();

	/**
	 * 	\brief	Get last DTrack error description.
	 * 	@return Error description.
	 */
	std::string getLastDTrackErrorDescription();

	/**
	 *	\brief Is UDP connection valid?
	 *
	 *	Replace valid() function in older SDKs.
	 *	@return	Valid?
	 */
	bool isUDPValid();

	/**
	 *	\brief Is TCP connection still valid?
	 *	On DTrack systems this function always returns false.
	 *	@return	Valid?
	 */
	bool isTCPValid();
	/**
	 *	\brief	Receive and process one DTrack data packet (UDP; ASCII protocol)
	 *
	 *	@return	receiving was successful
	 */
	bool receive();

	/**
	 *	\brief	Send DTrack command (UDP).
	 *
	 *	@param command command
	 *	@return command was successful (if not, a DTrack error is available)
	 */
	bool sendCommand(const std::string& command);

	/**
	 * 	\brief Send command to DTrack and receive answer (TCP).
	 *
	 *	Answers like "dtrack2 ok" and "dtrack2 err .." are processed and no answer string is returned.
	 *	In this case getLastDTrackError() and getLastDTrackErrorString() will return more information.
	 *
	 * 	@param[in]	command	Command,50105,5000 string
	 * 	@param[out]	answer	Buffer for answer, at least 200 Bytes
	 * 	@return <0 if error occured, 0 if answer still has to be processed, >0 if answer is already processed
	 */
	int sendCommandReceive(const std::string& command, char* answer);

	/**setLastDTrackError
	 * 	\brief	Get frame counter.
	 *
	 *	Refers to last received frame.
	 *	@return	Frame counter.
	 */
	unsigned int getFrameCounter();

	/**
	 * 	\brief	Get timestamp.
	 *
	 *	Refers to last received frame.
	 *	@return	Timestamp (-1 if information not available).
	 */
	double getTimeStamp();

	/**
	 * 	\brief	Get number of calibrated standard bodies (as far as known).
	 *
	 *	Refers to last received frame.
	 *	@return	Number of calibrated standard bodies.
	 */
	int getNumBody();

	/**
	 * 	\brief	Get standard body data (id (i): standard body id 0..max-1).
	 *
	 *	Refers to last received
	} frame. Currently not tracked bodies are getting a quality of -1
	 *	@param id id
	 *	@return	id-th standard body data.
	 */
	DTrack_Body_Type_d* getBody(int id);

	/**
	 * 	\brief	Get number of calibrated Flysticks.
	 *
	 *	Refers to last received frame.
	 *	@return	Number of calibrate	sdk->setCompatibilityMode(DTrackSDK::COMP_MODE_DTRACK_2);d Flysticks.
	 */
	int getNumFlyStick();

	/**
	 * 	\brief	Get Flystick data (id (i): Flystick id 0..max-1).
	 *
	 *	Refers to last received frame. Currently not tracked bodies are getting a quality of -1
	 *	@param id id
	 *	@return	id-th Flystick data.
	 */
	DTrack_FlyStick_Type_d* getFlyStick(int id);

	/**
	 * 	\brief	Get number of calibrated measurement tools.
	 *
	 *	Refers to last received frame.
	 *	@return	Number of calibrated measurement tools.
	 */
	int getNumMeaTool();

	/**
	 * 	\brief	Get measurement tool data (id (i): tool id 0..max-1).
	 *
	 *	Refers to last received frame. Currently not tracked bodies are getting a quality of -1
	 *	@param id id
	 *	@return	id-th measurement tool data.
	 */
	DTrack_MeaTool_Type_d* getMeaTool(int id);

	/**
	 * 	\brief	Get number of calibrated Fingertracking hands (as far as known).
	 *
	 *	Refers to last received frame.
	 *	@return	Number of calibrated Fingertracking hands (as far as known).
	 */

	int getNumHand();

	/**
	 * 	\brief	Get Fingertracking hand data (id (i): hand id 0..max-1).
	 *
	 *	Refers to last received frame. Currently not tracked bodies are getting a quality of -1
	 *	@param id id
	 *	@return	id-th Fingertracking hand data.
	 */
	DTrack_Hand_Type_d* getHand(int id);

	/**
	 * 	\brief	Get number of tracked single markers.
	 *
	 *	Refers to last received frame.
	 *	@return	Number of tracked single markers.
	 */
	int getNumMarker();

	/**
	 * 	\brief	Get single marker data (id (i): index 0..max-1).
	 *
	 *	Refers to last received frame. Currently not tracked bodies are getting a quality of -1
	 *	@param index index
	 *	@return	id-th single marker data.
	 */
	DTrack_Marker_Type_d* getMarker(int index);

	/**
	 * 	\brief	Set DTrack parameter.
	 *	@param 	category	parameter category
	 *	@param 	name		parameter name
	 *	@param 	value		parameter value
	 *	@return setting was successful (if not, a DTrack error message is available)
	 */
	bool setParam(const std::string& category, const std::string& name, const std::string& value);

	/**
	 * 	\brief	Set DTrack parameter.
	 * 	@param	parameter	total parameter (category, name and value; without starting "dtrack set ")
	 *	@return setting was successful (if not, a DTrack error message is available)
	 */
	bool setParam(const std::string& parameter);

	/**
	}
	 * 	\brief	Get DTrack parameter.
	 *	@param[in] 	category	parameter category
	 *	@param[in] 	name		parameter name
	 *	@param[out]	value		parameter value
	 *	@return	getting was successful (if not, a DTrack error message is available)
	 */
	bool getParam(const std::string& category, const std::string& name, std::string& value);

	/**
	 * 	\brief	Get DTrack parameter.
	 *	@param[in] 	parameter	total parameter (category and name; without starting "dtrack get ")
	 *	@param[out]	value		parameter value
	 *	@return	getting was successful (if not, a DTrack error message is available)
	 */
	bool getParam(const std::string& parameter, std::string& value);

	/**
	 *	\brief	Get DTrack message.
	 *	@return Message was available
	 */
	bool getMessage();

	/**
	 *	\brief Get origin of last DTrack message.
	 *	@return origin
	 */
	std::string getMessageOrigin();

	/**
	 *	\brief Get status of last DTrack message.
	 *	@return status
	 */
	std::string getMessageStatus();

	/**
	 *	\brief Get frame counter of last DTrack message.
	 *	@return frame counter
	 */
	unsigned int getMessageFrameNr();

	/**
	 *	\brief Get error id of last DTrack message.
	 *	@return error id
	 */
	unsigned int getMessageErrorId();

	/**
	 *	\brief Get message string of last DTrack message.
	 *	@return mesage string
	 */
	std::string getMessageMsg();

	/**
	 *	\brief Get data port where tracking data is received.
	 *	@return Data port.
	 */
	unsigned short getDataPort();

	/**
	 * 	\brief Start measurement.
	 * 	@param channel Value 1 - 5, DTrack2 output channel. Value 0 means current setting is not overwritten. Default is 1. Not used in DTrack.
	 *	@return Command successful?
	 */
	bool startMeasurement(unsigned int channel = 1);

	/**
	 * 	\brief Stop measurement.
	 * 	@return Command successful?
	 */
	bool stopMeasurement();
private:
	RemoteSystemType rsType;
	Errors lastDataError, lastServerError;

	unsigned int channelBackup;
	int lastDTrackError;			// last DTrack error: as code
	std::string lastDTrackErrorString;	// last DTrack error: as string

	void* d_tcpsock;                // socket number for TCP
	int d_tcptimeout_us;            // timeout for receiving and sending TCP data

	void* d_udpsock;                // socket number for UDP
	unsigned int d_remote_ip;       // IP address for remote access
	unsigned short d_udpport;		// port number for UDP
	unsigned short d_remoteport;	// port number for UDP (remote) / TCP
	int d_udptimeout_us;        	// timeout for receiving UDP data

	int d_udpbufsize;               // size of UDP buffer
	char* d_udpbuf;                 // UDP buffer

	unsigned int act_framecounter;                   // frame counter
	double act_timestamp;                            // timestamp (-1, if information not available)
	int act_num_body;                                // number of calibrated standard bodies (as far as known)
	std::vector<DTrack_Body_Type_d> act_body;         // array containing standard body data
	int act_num_flystick;                            // number of calibrated Flysticks
	std::vector<DTrack_FlyStick_Type_d> act_flystick; // array containing Flystick data
	int act_num_meatool;                             // number of calibrated measurement tools
	std::vector<DTrack_MeaTool_Type_d> act_meatool;   // array containing measurement tool data
	int act_num_hand;                                // number of calibrated Fingertracking hands (as far as known)
	std::vector<DTrack_Hand_Type_d> act_hand;         // array containing Fingertracking hands data
	int act_num_marker;                              // number of tracked single markers
	std::vector<DTrack_Marker_Type_d> act_marker;     // array containing single marker data

	std::string d_message_origin;     // last DTrack message: origin of message
	std::string d_message_status;     // last DTrack message: status of message
	unsigned int d_message_framenr;   // last DTrack message: frame counter
	unsigned int d_message_errorid;   // last DTrack message: error id
	std::string d_message_msg;        // last DTrack message: message string

	/**
	 * 	\brief Set last dtrack error.
	 *
	 * 	@param	newError		New error code for last operation (default is 0).
	 * 	@param	newErrorString	Corresponding error string if exists (optional).
	 */
	void setLastDTrackError(int newError = 0, std::string newErrorString = "");

	/**
	 *	\brief	Init function, called from constructor.
	 *
	 *	@param	remote_type			RemoteSystemType
	 *	@param	server_host			hostname or IP address of ARTtrack Controller (empty if not used)
	 *	@param	server_port			port number of ARTtrack Controller (default is 50105)
	 *	@param	data_port			port number to receive tracking data from ARTtrack Controller (0 if to be chosen)
	 *	@param	data_bufsize		size of buffer for UDP packets (in bytes; default is 20000)
	 *	@param	data_timeout_us		timeout (receiving) in us (in micro second, default is 1s)
	 *	@param	server_timeout_us	timeout for access to ARTtrack Controller (in micro second; receiving and sending; default is 10s)
	 */
	void init(RemoteSystemType sysType = SYS_DTRACK_UNKNOWN,
			const std::string& server_host = "",
			unsigned short server_port = 50105,
			unsigned short data_port = 0,
			int data_bufsize = 20000,
			int data_timeout_us = 1000000,
			int server_timeout_us = 10000000
	);
};


#endif /* _ART_DTRACKSDK_HPP_ */
