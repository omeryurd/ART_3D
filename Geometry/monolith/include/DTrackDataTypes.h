/* DTrackTypes: C header file, A.R.T. GmbH 3.5.07-07.09.10
 *
 * Type definitions used in DTrackSDK
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
 * $Id: DTrackDataTypes.h,v 1.2 2010/07/09 09:03:20 sebastianz Exp $
 */

#ifndef _ART_DTRACK_DATATYPES_H_
#define _ART_DTRACK_DATATYPES_H_

/**
 * 	\brief	Single marker data (3DOF, float)
 */
typedef struct{
	//! id number (starting with 1)
	int id;
	//! quality (0 <= qu <= 1)
	float quality;
	//! location (in mm)
	float loc[3];
} DTrack_Marker_Type_f;

/**
 * 	\brief	Single marker data (3DOF, double)
 */
typedef struct{
	//! id number (starting with 1)
	int id;
	//! quality (0 <= qu <= 1)
	double quality;
	//! location (in mm)
	double loc[3];
} DTrack_Marker_Type_d;

/**
 * 	\brief	DTrack_Marker_Type definition for older SDKs
 */
typedef DTrack_Marker_Type_f DTrack_Marker_Type;

// -----------------------------------------------------------------------------------------------------

/**
 * 	\brief	Standard body data (6DOF, float)
 *
 *	Currently not tracked bodies are getting a quality of -1.
 */
typedef struct{
	//! id number (starting with 0)
	int id;
	//! quality (0 <= qu <= 1, no tracking if -1)
	float quality;
	//! location (in mm)
	float loc[3];
	//! rotation matrix (column-wise)
	float rot[9];
} DTrack_Body_Type_f;

/**
 * 	\brief	Standard body data (6DOF, double)
 *
 *	Currently not tracked bodies are getting a quality of -1.
 */
typedef struct{
	//! id number (starting with 0)
	int id;
	//! quality (0 <= qu <= 1, no tracking if -1)
	double quality;
	//! location (in mm)
	double loc[3];
	//! rotation matrix (column-wise)
	double rot[9];
} DTrack_Body_Type_d;

/**
 * 	\brief	DTrack_Body_Type definition for older SDKs
 */
typedef DTrack_Body_Type_f DTrack_Body_Type;

// -----------------------------------------------------------------------------------------------------

#define DTRACK_FLYSTICK_MAX_BUTTON    16	//! FlyStick data: maximum number of buttons
#define DTRACK_FLYSTICK_MAX_JOYSTICK   8	//! FlyStick data: maximum number of joystick values

/**
 * 	\brief	A.R.T.Flystick data (6DOF + buttons, float)
 *
 * 	Currently not tracked bodies are getting a quality of -1.
 *	Note the maximum number of buttons and joystick values.
 */
typedef struct{
	//! id number (starting with 0)
	int id;
	//! quality (0 <= qu <= 1, no tracking if -1)
	float quality;
	//! number of buttons
	int num_button;
	//! button state (1 pressed, 0 not pressed); 0 front, 1..n-1 right to left
	int button[DTRACK_FLYSTICK_MAX_BUTTON];
	//! number of joystick values
	int num_joystick;
	//! joystick value (-1 <= joystick <= 1); 0 horizontal, 1 vertical
	float joystick[DTRACK_FLYSTICK_MAX_JOYSTICK];
	//! location (in mm)
	float loc[3];
	//! rotation matrix (column-wise)
	float rot[9];
} DTrack_FlyStick_Type_f;

/**
 * 	\brief	A.R.T.Flystick data (6DOF + buttons, double)
 *
 * 	Currently not tracked bodies are getting a quality of -1.
 *	Note the maximum number of buttons and joystick values.
 */
typedef struct{
	//! id number (starting with 0)
	int id;
	//! quality (0 <= qu <= 1, no tracking if -1)
	double quality;
	//! number of buttons
	int num_button;
	//! button state (1 pressed, 0 not pressed): 0 front, 1..n-1 right to left
	int button[DTRACK_FLYSTICK_MAX_BUTTON];
	//! number of joystick values
	int num_joystick;
	//! joystick value (-1 <= joystick <= 1); 0 horizontal, 1 vertical
	double joystick[DTRACK_FLYSTICK_MAX_JOYSTICK];
	//! location (in mm)
	double loc[3];
	//! rotation matrix (column-wise)
	double rot[9];
} DTrack_FlyStick_Type_d;

/**
 * 	\brief	DTrack_FlyStick_Type definition for older SDKs
 */
typedef DTrack_FlyStick_Type_f DTrack_FlyStick_Type;

// -----------------------------------------------------------------------------------------------------

#define DTRACK_MEATOOL_MAX_BUTTON    1  //! Measurement tool data: maximum number of buttons

/**
 * 	\brief	Measurement tool data (6DOF + buttons, float)
 *
 * 	Currently not tracked bodies are getting a quality of -1.
 * 	Note the maximum number of buttons.
 */
typedef struct{
	//! id number (starting with 0)
	int id;
	//! quality (0 <= qu <= 1, no tracking if -1)
	float quality;
	//! number of buttons
	int num_button;
	//! button state (1 pressed, 0 not pressed)
	int button[DTRACK_MEATOOL_MAX_BUTTON];
	//! location (in mm)
	float loc[3];
	//! rotation matrix (column-wise)
	float rot[9];
} DTrack_MeaTool_Type_f;

/**
 * 	\brief	Measurement tool data (6DOF + buttons, double)
 *
 * 	Currently not tracked bodies are getting a quality of -1.
 * 	Note the maximum number of buttons.
 */
typedef struct{
	//! id number (starting with 0)
	int id;
	//! quality (0 <= qu <= 1, no tracking if -1)
	double quality;
	//! number of buttons
	int num_button;
	//! button state (1 pressed, 0 not pressed)
	int button[DTRACK_MEATOOL_MAX_BUTTON];
	//! location (in mm)
	double loc[3];
	//! rotation matrix (column-wise)
	double rot[9];
} DTrack_MeaTool_Type_d;

/**
 * 	\brief	DTrack_MeaTool_Type definition for older SDKs
 */
typedef DTrack_MeaTool_Type_f DTrack_MeaTool_Type;

// -----------------------------------------------------------------------------------------------------

#define DTRACK_HAND_MAX_FINGER    5  //! Fingertracking hand data: maximum number of fingers

/**
 *	\brief	A.R.T. Fingertracking hand data (6DOF + fingers, float)
 *
 *	Currently not tracked bodies are getting a quality of -1.
 */
typedef struct{
	//! id number (starting with 0)
	int id;
	//! quality (0 <= qu <= 1, no tracking if -1)
	float quality;
	//! left (0) or right (1) hand
	int lr;
	//! number of fingers (maximum 5)
	int nfinger;
	//! back of the hand: location (in mm)
	float loc[3];
	//! back of the hand: rotation matrix (column-wise)
	float rot[9];
	//! order: thumb, index finger, middle finger, ...
	struct{
		//! finger: location (in mm)
		float loc[3];
		//! finger: rotation matrix (column-wise)
		float rot[9];
		//! finger: radius of tip
		float radiustip;
		//! finger: length of phalanxes; order: outermost, middle, innermost
		float lengthphalanx[3];
		//! finger: angle between phalanxes
		float anglephalanx[2];
	} finger[DTRACK_HAND_MAX_FINGER];
} DTrack_Hand_Type_f;

/**
 *	\brief	A.R.T. Fingertracking hand data (6DOF + fingers, double)
 *
 *	Currently not tracked bodies are getting a quality of -1.
 */
typedef struct{
	// id number (starting with 0)
	int id;
	// quality (0 <= qu <= 1, no tracking if -1)
	double quality;
	// left (0) or right (1) hand
	int lr;
	// number of fingers (maximum 5)
	int nfinger;
	// back of the hand: location (in mm)
	double loc[3];
	// back of the hand: rotation matrix (column-wise)
	double rot[9];
	//! order: thumb, index finger, middle finger, ...
	struct{
		//! finger: location (in mm)
		double loc[3];
		//! finger: rotation matrix (column-wise)
		double rot[9];
		//! finger: radius of tip
		double radiustip;
		//! finger: length of phalanxes; order: outermost, middle, innermost
		double lengthphalanx[3];
		//! finger: angle between phalanxes
		double anglephalanx[2];
	} finger[DTRACK_HAND_MAX_FINGER];
} DTrack_Hand_Type_d;

/**
 * 	\brief	DTrack_Hand_Type definition for older SDKs
 */
typedef DTrack_Hand_Type_f DTrack_Hand_Type;


#endif /* ART_DTRACK_DATATYPES_H_ */
