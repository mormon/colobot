// * This file is part of the COLOBOT source code
// * Copyright (C) 2001-2008, Daniel ROUX & EPSITEC SA, www.epsitec.ch
// *
// * This program is free software: you can redistribute it and/or modify
// * it under the terms of the GNU General Public License as published by
// * the Free Software Foundation, either version 3 of the License, or
// * (at your option) any later version.
// *
// * This program is distributed in the hope that it will be useful,
// * but WITHOUT ANY WARRANTY; without even the implied warranty of
// * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// * GNU General Public License for more details.
// *
// * You should have received a copy of the GNU General Public License
// * along with this program. If not, see  http://www.gnu.org/licenses/.

// restext.cpp

#define STRICT

#include <stdio.h>
#include <string.h>
//#include "struct.h"
//#include "D3DEngine.h"
#include "language.h"
//#include "misc.h"
//#include "event.h"
//#include "object.h"
//#include "cbot\resource.h"
#include "restext.h"



//** -> text to translate!



#ifdef WINDOWS
// Gives the pointer to the engine.

void SetEngine(CD3DEngine *engine)
{
	g_engine = engine;
}

// Give the player's name.

void SetGlobalGamerName(char *name)
{
	strcpy(g_gamerName, name);
}



typedef struct
{
	KeyRank		key;
	char		name[20];
}
KeyDesc;

static KeyDesc keyTable[22] =
{
	{ KEYRANK_LEFT,		"left;"    },
	{ KEYRANK_RIGHT,	"right;"   },
	{ KEYRANK_UP,		"up;"      },
	{ KEYRANK_DOWN,		"down;"    },
	{ KEYRANK_GUP,		"gup;"     },
	{ KEYRANK_GDOWN,	"gdown;"   },
	{ KEYRANK_CAMERA,	"camera;"  },
	{ KEYRANK_DESEL,	"desel;"   },
	{ KEYRANK_ACTION,	"action;"  },
	{ KEYRANK_NEAR,		"near;"    },
	{ KEYRANK_AWAY,		"away;"    },
	{ KEYRANK_NEXT,		"next;"    },
	{ KEYRANK_HUMAN,	"human;"   },
	{ KEYRANK_QUIT,		"quit;"    },
	{ KEYRANK_HELP,		"help;"    },
	{ KEYRANK_PROG,		"prog;"    },
	{ KEYRANK_CBOT,		"cbot;"    },
	{ KEYRANK_VISIT,	"visit;"   },
	{ KEYRANK_SPEED10,	"speed10;" },
	{ KEYRANK_SPEED15,	"speed15;" },
	{ KEYRANK_SPEED20,	"speed20;" },
	{ KEYRANK_SPEED30,	"speed30;" },
};

// Seeks a key.

BOOL SearchKey(char *cmd, KeyRank &key)
{
	int		i;

	for ( i=0 ; i<22 ; i++ )
	{
		if ( strstr(cmd, keyTable[i].name) == cmd )
		{
			key = keyTable[i].key;
			return TRUE;
		}
	}
	return FALSE;
}

// Replaces the commands "\key name;" in a text.

void PutKeyName(char* dst, char* src)
{
	KeyRank	key;
	char	name[50];
	int		s, d, n, res;

	s = d = 0;
	while ( src[s] != 0 )
	{
		if ( src[s+0] == '\\' &&
			 src[s+1] == 'k'  &&
			 src[s+2] == 'e'  &&
			 src[s+3] == 'y'  &&
			 src[s+4] == ' '  )
		{
			if ( SearchKey(src+s+5, key) )
			{
				res = g_engine->RetKey(key, 0);
				if ( res != 0 )
				{
					if ( GetResource(RES_KEY, res, name) )
					{
						n = 0;
						while ( name[n] != 0 )
						{
							dst[d++] = name[n++];
						}
						while ( src[s++] != ';' );
						continue;
					}
				}
			}
		}

		dst[d++] = src[s++];
	}
	dst[d++] = 0;
}


// Returns the text of a resource.

BOOL GetResource(ResType type, int num, char* text)
{
	char	buffer[100];

	if ( !GetResourceBase(type, num, buffer) )
	{
		text[0] = 0;
		return FALSE;
	}

	PutKeyName(text, buffer);
	return TRUE;
}


#endif
// Returns the text of a resource.

bool GetResourceBase(ResType type, int num, char* text)
{
	text[0] = 0;

#if _ENGLISH
	if ( type == RES_TEXT )
	{
		#if _FULL
		if ( num == RT_VERSION_ID          )  strcpy(text, "1.18 /e");
		#endif
		#if _NET
		if ( num == RT_VERSION_ID          )  strcpy(text, "CeeBot-A 1.18");
		#endif
		#if _SCHOOL & _EDU
		#if _TEEN
		if ( num == RT_VERSION_ID          )  strcpy(text, "CeeBot-Teen EDU 1.18");
		#else
		if ( num == RT_VERSION_ID          )  strcpy(text, "CeeBot-A EDU 1.18");
		#endif
		#endif
		#if _SCHOOL & _PERSO
		#if _TEEN
		if ( num == RT_VERSION_ID          )  strcpy(text, "CeeBot-Teen PERSO 1.18");
		#else
		if ( num == RT_VERSION_ID          )  strcpy(text, "CeeBot-A PERSO 1.18");
		#endif
		#endif
		#if _SCHOOL & _CEEBOTDEMO
		#if _TEEN
		if ( num == RT_VERSION_ID          )  strcpy(text, "CeeBot-Teen DEMO 1.18");
		#else
		if ( num == RT_VERSION_ID          )  strcpy(text, "CeeBot-A DEMO 1.18");
		#endif
		#endif
		#if _DEMO
		if ( num == RT_VERSION_ID          )  strcpy(text, "Demo 1.18 /e");
		#endif
		if ( num == RT_DISINFO_TITLE       )  strcpy(text, "SatCom");
		if ( num == RT_WINDOW_MAXIMIZED    )  strcpy(text, "Maximize");
		if ( num == RT_WINDOW_MINIMIZED    )  strcpy(text, "Minimize");
		if ( num == RT_WINDOW_STANDARD     )  strcpy(text, "Normal size");
		if ( num == RT_WINDOW_CLOSE        )  strcpy(text, "Close");

		if ( num == RT_STUDIO_TITLE        )  strcpy(text, "Program editor");
		if ( num == RT_SCRIPT_NEW          )  strcpy(text, "New");
		if ( num == RT_NAME_DEFAULT        )  strcpy(text, "Player");
		if ( num == RT_IO_NEW              )  strcpy(text, "New ...");
		if ( num == RT_KEY_OR              )  strcpy(text, " or ");

#if _NEWLOOK
		if ( num == RT_TITLE_BASE          )  strcpy(text, "CeeBot");
		if ( num == RT_TITLE_INIT          )  strcpy(text, "CeeBot");
#else
		if ( num == RT_TITLE_BASE          )  strcpy(text, "COLOBOT");
		if ( num == RT_TITLE_INIT          )  strcpy(text, "COLOBOT");
#endif
		if ( num == RT_TITLE_TRAINER       )  strcpy(text, "Programming exercises");
		if ( num == RT_TITLE_DEFI          )  strcpy(text, "Challenges");
		if ( num == RT_TITLE_MISSION       )  strcpy(text, "Missions");
		if ( num == RT_TITLE_FREE          )  strcpy(text, "Free game");
		if ( num == RT_TITLE_TEEN          )  strcpy(text, "Free game");
		if ( num == RT_TITLE_USER          )  strcpy(text, "User levels");
		if ( num == RT_TITLE_PROTO         )  strcpy(text, "Prototypes");
		if ( num == RT_TITLE_SETUP         )  strcpy(text, "Options");
		if ( num == RT_TITLE_NAME          )  strcpy(text, "Player's name");
		if ( num == RT_TITLE_PERSO         )  strcpy(text, "Customize your appearance");
		if ( num == RT_TITLE_WRITE         )  strcpy(text, "Save the current mission");
		if ( num == RT_TITLE_READ          )  strcpy(text, "Load a saved mission");

		if ( num == RT_PLAY_CHAPt          )  strcpy(text, " Chapters:");
		if ( num == RT_PLAY_CHAPd          )  strcpy(text, " Chapters:");
		if ( num == RT_PLAY_CHAPm          )  strcpy(text, " Planets:");
		if ( num == RT_PLAY_CHAPf          )  strcpy(text, " Planets:");
		if ( num == RT_PLAY_CHAPu          )  strcpy(text, " User levels:");
		if ( num == RT_PLAY_CHAPp          )  strcpy(text, " Planets:");
		if ( num == RT_PLAY_CHAPte         )  strcpy(text, " Chapters:");
		if ( num == RT_PLAY_LISTt          )  strcpy(text, " Exercises in the chapter:");
		if ( num == RT_PLAY_LISTd          )  strcpy(text, " Challenges in the chapter:");
		if ( num == RT_PLAY_LISTm          )  strcpy(text, " Missions on this planet:");
		if ( num == RT_PLAY_LISTf          )  strcpy(text, " Free game on this planet:");
		if ( num == RT_PLAY_LISTu          )  strcpy(text, " Missions on this level:");
		if ( num == RT_PLAY_LISTp          )  strcpy(text, " Prototypes on this planet:");
		if ( num == RT_PLAY_LISTk          )  strcpy(text, " Free game on this chapter:");
		if ( num == RT_PLAY_RESUME         )  strcpy(text, " Summary:");

		if ( num == RT_SETUP_DEVICE        )  strcpy(text, " Drivers:");
		if ( num == RT_SETUP_MODE          )  strcpy(text, " Resolution:");
		if ( num == RT_SETUP_KEY1          )  strcpy(text, "1) First click on the key you want to redefine.");
		if ( num == RT_SETUP_KEY2          )  strcpy(text, "2) Then press the key you want to use instead.");

		if ( num == RT_PERSO_FACE          )  strcpy(text, "Face type:");
		if ( num == RT_PERSO_GLASSES       )  strcpy(text, "Eyeglasses:");
		if ( num == RT_PERSO_HAIR          )  strcpy(text, "Hair color:");
		if ( num == RT_PERSO_COMBI         )  strcpy(text, "Suit color:");
		if ( num == RT_PERSO_BAND          )  strcpy(text, "Strip color:");

#if _NEWLOOK
		if ( num == RT_DIALOG_QUIT         )  strcpy(text, "Do you want to quit CeeBot ?");
		if ( num == RT_DIALOG_TITLE        )  strcpy(text, "CeeBot");
		if ( num == RT_DIALOG_YESQUIT      )  strcpy(text, "Quit\\Quit CeeBot");
#else
		if ( num == RT_DIALOG_QUIT         )  strcpy(text, "Do you want to quit COLOBOT ?");
		if ( num == RT_DIALOG_TITLE        )  strcpy(text, "COLOBOT");
		if ( num == RT_DIALOG_YESQUIT      )  strcpy(text, "Quit\\Quit COLOBOT");
#endif
		if ( num == RT_DIALOG_ABORT        )  strcpy(text, "Quit the mission?");
		if ( num == RT_DIALOG_YES          )  strcpy(text, "Abort\\Abort the current mission");
		if ( num == RT_DIALOG_NO           )  strcpy(text, "Continue\\Continue the current mission");
		if ( num == RT_DIALOG_NOQUIT       )  strcpy(text, "Continue\\Continue the game");
		if ( num == RT_DIALOG_DELOBJ       )  strcpy(text, "Do you really want to destroy the selected building?");
		if ( num == RT_DIALOG_DELGAME      )  strcpy(text, "Do you want to delete %s's saved games? ");
		if ( num == RT_DIALOG_YESDEL       )  strcpy(text, "Delete");
		if ( num == RT_DIALOG_NODEL        )  strcpy(text, "Cancel");
		if ( num == RT_DIALOG_LOADING      )  strcpy(text, "LOADING");

		if ( num == RT_STUDIO_LISTTT       )  strcpy(text, "Keyword help(\\key cbot;)");
		if ( num == RT_STUDIO_COMPOK       )  strcpy(text, "Compilation ok (0 errors)");
		if ( num == RT_STUDIO_PROGSTOP     )  strcpy(text, "Program finished");

		if ( num == RT_SATCOM_LIST         )  strcpy(text, "\\b;List of objects\n");
		if ( num == RT_SATCOM_BOT          )  strcpy(text, "\\b;Robots\n");
		if ( num == RT_SATCOM_BUILDING     )  strcpy(text, "\\b;Buildings\n");
		if ( num == RT_SATCOM_FRET         )  strcpy(text, "\\b;Moveable objects\n");
		if ( num == RT_SATCOM_ALIEN        )  strcpy(text, "\\b;Aliens\n");
		if ( num == RT_SATCOM_NULL         )  strcpy(text, "\\c; (none)\\n;\n");
		if ( num == RT_SATCOM_ERROR1       )  strcpy(text, "\\b;Error\n");
		if ( num == RT_SATCOM_ERROR2       )  strcpy(text, "The list is only available if a \\l;radar station\\u object\\radar; is working.\n");

		if ( num == RT_IO_OPEN             )  strcpy(text, "Open");
		if ( num == RT_IO_SAVE             )  strcpy(text, "Save");
		if ( num == RT_IO_LIST             )  strcpy(text, "Folder: %s");
		if ( num == RT_IO_NAME             )  strcpy(text, "Name:");
		if ( num == RT_IO_DIR              )  strcpy(text, "Folder:");
		if ( num == RT_IO_PRIVATE          )  strcpy(text, "Private\\Private folder");
		if ( num == RT_IO_PUBLIC           )  strcpy(text, "Public\\Common folder");

		if ( num == RT_GENERIC_DEV1        )  strcpy(text, "Developed by :");
		if ( num == RT_GENERIC_DEV2        )  strcpy(text, "www.epsitec.com");
//?		if ( num == RT_GENERIC_EDIT1       )  strcpy(text, "English version published by:");
//?		if ( num == RT_GENERIC_EDIT2       )  strcpy(text, "www.?.com");
		if ( num == RT_GENERIC_EDIT1       )  strcpy(text, " ");
		if ( num == RT_GENERIC_EDIT2       )  strcpy(text, " ");

		if ( num == RT_INTERFACE_REC       )  strcpy(text, "Recorder");
	}

	if ( type == RES_EVENT )
	{
		if ( num == EVENT_BUTTON_OK        )  strcpy(text, "OK");
		if ( num == EVENT_BUTTON_CANCEL    )  strcpy(text, "Cancel");
		if ( num == EVENT_BUTTON_NEXT      )  strcpy(text, "Next");
		if ( num == EVENT_BUTTON_PREV      )  strcpy(text, "Previous");
		if ( num == EVENT_BUTTON_QUIT      )  strcpy(text, "Menu (\\key quit;)");

		if ( num == EVENT_DIALOG_OK        )  strcpy(text, "OK");
		if ( num == EVENT_DIALOG_CANCEL    )  strcpy(text, "Cancel");

		if ( num == EVENT_INTERFACE_TRAINER)  strcpy(text, "Exercises\\Programming exercises");
		if ( num == EVENT_INTERFACE_DEFI   )  strcpy(text, "Challenges\\Programming challenges");
		if ( num == EVENT_INTERFACE_MISSION)  strcpy(text, "Missions\\Select mission");
		if ( num == EVENT_INTERFACE_FREE   )  strcpy(text, "Free game\\Free game without a specific goal");
		if ( num == EVENT_INTERFACE_TEEN   )  strcpy(text, "Free game\\Free game without a specific goal");
		if ( num == EVENT_INTERFACE_USER   )  strcpy(text, "User\\User levels");
		if ( num == EVENT_INTERFACE_PROTO  )  strcpy(text, "Proto\\Prototypes under development");
		if ( num == EVENT_INTERFACE_NAME   )  strcpy(text, "New player\\Choose player's name");
		if ( num == EVENT_INTERFACE_SETUP  )  strcpy(text, "Options\\Preferences");
		if ( num == EVENT_INTERFACE_AGAIN  )  strcpy(text, "Restart\\Restart the mission from the beginning");
		if ( num == EVENT_INTERFACE_WRITE  )  strcpy(text, "Save\\Save the current mission ");
		if ( num == EVENT_INTERFACE_READ   )  strcpy(text, "Load\\Load a saved mission");
#if _NEWLOOK
		if ( num == EVENT_INTERFACE_ABORT  )  strcpy(text, "\\Return to CeeBot");
		if ( num == EVENT_INTERFACE_QUIT   )  strcpy(text, "Quit\\Quit CeeBot");
#else
		if ( num == EVENT_INTERFACE_ABORT  )  strcpy(text, "\\Return to COLOBOT");
		if ( num == EVENT_INTERFACE_QUIT   )  strcpy(text, "Quit\\Quit COLOBOT");
#endif
		if ( num == EVENT_INTERFACE_BACK   )  strcpy(text, "<<  Back  \\Back to the previous screen");
		if ( num == EVENT_INTERFACE_PLAY   )  strcpy(text, "Play\\Start mission!");
		if ( num == EVENT_INTERFACE_SETUPd )  strcpy(text, "Device\\Driver and resolution settings");
		if ( num == EVENT_INTERFACE_SETUPg )  strcpy(text, "Graphics\\Graphics settings");
		if ( num == EVENT_INTERFACE_SETUPp )  strcpy(text, "Game\\Game settings");
		if ( num == EVENT_INTERFACE_SETUPc )  strcpy(text, "Controls\\Keyboard, joystick and mouse settings");
		if ( num == EVENT_INTERFACE_SETUPs )  strcpy(text, "Sound\\Music and game sound volume");
		if ( num == EVENT_INTERFACE_DEVICE )  strcpy(text, "Unit");
		if ( num == EVENT_INTERFACE_RESOL  )  strcpy(text, "Resolution");
		if ( num == EVENT_INTERFACE_FULL   )  strcpy(text, "Full screen\\Full screen or window mode");
		if ( num == EVENT_INTERFACE_APPLY  )  strcpy(text, "Apply changes\\Activates the changed settings");

		if ( num == EVENT_INTERFACE_TOTO   )  strcpy(text, "Robbie\\Your assistant");
		if ( num == EVENT_INTERFACE_SHADOW )  strcpy(text, "Shadows\\Shadows on the ground");
		if ( num == EVENT_INTERFACE_GROUND )  strcpy(text, "Marks on the ground\\Marks on the ground");
		if ( num == EVENT_INTERFACE_DIRTY  )  strcpy(text, "Dust\\Dust and dirt on bots and buildings");
		if ( num == EVENT_INTERFACE_FOG    )  strcpy(text, "Fog\\Fog");
		if ( num == EVENT_INTERFACE_LENS   )  strcpy(text, "Sunbeams\\Sunbeams in the sky");
		if ( num == EVENT_INTERFACE_SKY    )  strcpy(text, "Sky\\Clouds and nebulae");
		if ( num == EVENT_INTERFACE_PLANET )  strcpy(text, "Planets and stars\\Astronomical objects in the sky");
		if ( num == EVENT_INTERFACE_LIGHT  )  strcpy(text, "Dynamic lighting\\Mobile light sources");
		if ( num == EVENT_INTERFACE_PARTI  )  strcpy(text, "Number of particles\\Explosions, dust, reflections, etc.");
		if ( num == EVENT_INTERFACE_CLIP   )  strcpy(text, "Depth of field\\Maximum visibility");
		if ( num == EVENT_INTERFACE_DETAIL )  strcpy(text, "Details\\Visual quality of 3D objects");
		if ( num == EVENT_INTERFACE_TEXTURE)  strcpy(text, "Textures\\Quality of textures ");
		if ( num == EVENT_INTERFACE_GADGET )  strcpy(text, "Num of decorative objects\\Number of purely ornamental objects");
		if ( num == EVENT_INTERFACE_RAIN   )  strcpy(text, "Particles in the interface\\Steam clouds and sparks in the interface");
		if ( num == EVENT_INTERFACE_GLINT  )  strcpy(text, "Reflections on the buttons \\Shiny buttons");
		if ( num == EVENT_INTERFACE_TOOLTIP)  strcpy(text, "Help balloons\\Explain the function of the buttons");
		if ( num == EVENT_INTERFACE_MOVIES )  strcpy(text, "Film sequences\\Films before and after the missions");
		if ( num == EVENT_INTERFACE_NICERST)  strcpy(text, "Exit film\\Film at the exit of exercises");
		if ( num == EVENT_INTERFACE_HIMSELF)  strcpy(text, "Friendly fire\\Your shooting can damage your own objects ");
		if ( num == EVENT_INTERFACE_SCROLL )  strcpy(text, "Scrolling\\Scrolling when the mouse touches right or left border");
		if ( num == EVENT_INTERFACE_INVERTX)  strcpy(text, "Mouse inversion X\\Inversion of the scrolling direction on the X axis");
		if ( num == EVENT_INTERFACE_INVERTY)  strcpy(text, "Mouse inversion Y\\Inversion of the scrolling direction on the Y axis");
		if ( num == EVENT_INTERFACE_EFFECT )  strcpy(text, "Quake at explosions\\The screen shakes at explosions");
		if ( num == EVENT_INTERFACE_MOUSE  )  strcpy(text, "Mouse shadow\\Gives the mouse a shadow");
		if ( num == EVENT_INTERFACE_EDITMODE) strcpy(text, "Automatic indent\\When program editing");
		if ( num == EVENT_INTERFACE_EDITVALUE)strcpy(text, "Big indent\\Indent 2 or 4 spaces per level defined by braces");
		if ( num == EVENT_INTERFACE_SOLUCE4)  strcpy(text, "Access to solutions\\Show program \"4: Solution\" in the exercises");  //**

		if ( num == EVENT_INTERFACE_KDEF   )  strcpy(text, "Standard controls\\Standard key functions");
		if ( num == EVENT_INTERFACE_KLEFT  )  strcpy(text, "Turn left\\turns the bot to the left");
		if ( num == EVENT_INTERFACE_KRIGHT )  strcpy(text, "Turn right\\turns the bot to the right");
		if ( num == EVENT_INTERFACE_KUP    )  strcpy(text, "Forward\\Moves forward");
		if ( num == EVENT_INTERFACE_KDOWN  )  strcpy(text, "Backward\\Moves backward");
		if ( num == EVENT_INTERFACE_KGUP   )  strcpy(text, "Climb\\Increases the power of the jet");
		if ( num == EVENT_INTERFACE_KGDOWN )  strcpy(text, "Descend\\Reduces the power of the jet");
		if ( num == EVENT_INTERFACE_KCAMERA)  strcpy(text, "Change camera\\Switches between onboard camera and following camera");
		if ( num == EVENT_INTERFACE_KDESEL )  strcpy(text, "Previous object\\Selects the previous object");
		if ( num == EVENT_INTERFACE_KACTION)  strcpy(text, "Standard action\\Standard action of the bot (take/grab, shoot, sniff, etc)");
		if ( num == EVENT_INTERFACE_KNEAR  )  strcpy(text, "Camera closer\\Moves the camera forward");
		if ( num == EVENT_INTERFACE_KAWAY  )  strcpy(text, "Camera back\\Moves the camera backward");
		if ( num == EVENT_INTERFACE_KNEXT  )  strcpy(text, "Next object\\Selects the next object");
		if ( num == EVENT_INTERFACE_KHUMAN )  strcpy(text, "Select the astronaut\\Selects the astronaut");
		if ( num == EVENT_INTERFACE_KQUIT  )  strcpy(text, "Quit\\Quit the current mission or exercise");
		if ( num == EVENT_INTERFACE_KHELP  )  strcpy(text, "Instructions\\Shows the instructions for the current mission");
		if ( num == EVENT_INTERFACE_KPROG  )  strcpy(text, "Programming help\\Gives more detailed help with programming");
		if ( num == EVENT_INTERFACE_KCBOT  )  strcpy(text, "Key word help\\More detailed help about key words");
		if ( num == EVENT_INTERFACE_KVISIT )  strcpy(text, "Origin of last message\\Shows where the last message was sent from");
		if ( num == EVENT_INTERFACE_KSPEED10) strcpy(text, "Speed 1.0x\\Normal speed");
		if ( num == EVENT_INTERFACE_KSPEED15) strcpy(text, "Speed 1.5x\\1.5 times faster");
		if ( num == EVENT_INTERFACE_KSPEED20) strcpy(text, "Speed 2.0x\\Double speed");
		if ( num == EVENT_INTERFACE_KSPEED30) strcpy(text, "Speed 3.0x\\Three times faster");

		if ( num == EVENT_INTERFACE_VOLSOUND) strcpy(text, "Sound effects:\\Volume of engines, voice, shooting, etc.");
		if ( num == EVENT_INTERFACE_VOLMUSIC) strcpy(text, "Background sound :\\Volume of audio tracks on the CD");
		if ( num == EVENT_INTERFACE_SOUND3D)  strcpy(text, "3D sound\\3D positioning of the sound");

		if ( num == EVENT_INTERFACE_MIN    )  strcpy(text, "Lowest\\Minimum graphic quality (highest frame rate)");
		if ( num == EVENT_INTERFACE_NORM   )  strcpy(text, "Normal\\Normal graphic quality");
		if ( num == EVENT_INTERFACE_MAX    )  strcpy(text, "Highest\\Highest graphic quality (lowest frame rate)");

		if ( num == EVENT_INTERFACE_SILENT )  strcpy(text, "Mute\\No sound");
		if ( num == EVENT_INTERFACE_NOISY  )  strcpy(text, "Normal\\Normal sound volume");

		if ( num == EVENT_INTERFACE_JOYSTICK) strcpy(text, "Use a joystick\\Joystick or keyboard");
		if ( num == EVENT_INTERFACE_SOLUCE )  strcpy(text, "Access to solution\\Shows the solution (detailed instructions for missions)");

		if ( num == EVENT_INTERFACE_NEDIT  )  strcpy(text, "\\New player name");
		if ( num == EVENT_INTERFACE_NOK    )  strcpy(text, "OK\\Choose the selected player");
		if ( num == EVENT_INTERFACE_NCANCEL)  strcpy(text, "Cancel\\Keep current player name");
		if ( num == EVENT_INTERFACE_NDELETE)  strcpy(text, "Delete player\\Deletes the player from the list");
		if ( num == EVENT_INTERFACE_NLABEL )  strcpy(text, "Player name");

		if ( num == EVENT_INTERFACE_IOWRITE)  strcpy(text, "Save\\Saves the current mission");
		if ( num == EVENT_INTERFACE_IOREAD )  strcpy(text, "Load\\Loads the selected mission");
		if ( num == EVENT_INTERFACE_IOLIST )  strcpy(text, "List of saved missions");
		if ( num == EVENT_INTERFACE_IOLABEL)  strcpy(text, "Filename:");
		if ( num == EVENT_INTERFACE_IONAME )  strcpy(text, "Mission name");
		if ( num == EVENT_INTERFACE_IOIMAGE)  strcpy(text, "Photography");
		if ( num == EVENT_INTERFACE_IODELETE) strcpy(text, "Delete\\Deletes the selected file");

		if ( num == EVENT_INTERFACE_PERSO  )  strcpy(text, "Appearance\\Choose your appearance");
		if ( num == EVENT_INTERFACE_POK    )  strcpy(text, "OK");
		if ( num == EVENT_INTERFACE_PCANCEL)  strcpy(text, "Cancel");
		if ( num == EVENT_INTERFACE_PDEF   )  strcpy(text, "Standard\\Standard appearance settings");
		if ( num == EVENT_INTERFACE_PHEAD  )  strcpy(text, "Head\\Face and hair");
		if ( num == EVENT_INTERFACE_PBODY  )  strcpy(text, "Suit\\Astronaut suit");
		if ( num == EVENT_INTERFACE_PLROT  )  strcpy(text, "\\Turn left");
		if ( num == EVENT_INTERFACE_PRROT  )  strcpy(text, "\\Turn right");
		if ( num == EVENT_INTERFACE_PCRa   )  strcpy(text, "Red");
		if ( num == EVENT_INTERFACE_PCGa   )  strcpy(text, "Green");
		if ( num == EVENT_INTERFACE_PCBa   )  strcpy(text, "Blue");
		if ( num == EVENT_INTERFACE_PCRb   )  strcpy(text, "Red");
		if ( num == EVENT_INTERFACE_PCGb   )  strcpy(text, "Green");
		if ( num == EVENT_INTERFACE_PCBb   )  strcpy(text, "Blue");
		if ( num == EVENT_INTERFACE_PFACE1 )  strcpy(text, "\\Face 1");
		if ( num == EVENT_INTERFACE_PFACE2 )  strcpy(text, "\\Face 4");
		if ( num == EVENT_INTERFACE_PFACE3 )  strcpy(text, "\\Face 3");
		if ( num == EVENT_INTERFACE_PFACE4 )  strcpy(text, "\\Face 2");
		if ( num == EVENT_INTERFACE_PGLASS0)  strcpy(text, "\\No eyeglasses");
		if ( num == EVENT_INTERFACE_PGLASS1)  strcpy(text, "\\Eyeglasses 1");
		if ( num == EVENT_INTERFACE_PGLASS2)  strcpy(text, "\\Eyeglasses 2");
		if ( num == EVENT_INTERFACE_PGLASS3)  strcpy(text, "\\Eyeglasses 3");
		if ( num == EVENT_INTERFACE_PGLASS4)  strcpy(text, "\\Eyeglasses 4");
		if ( num == EVENT_INTERFACE_PGLASS5)  strcpy(text, "\\Eyeglasses 5");

		if ( num == EVENT_OBJECT_DESELECT  )  strcpy(text, "Previous selection (\\key desel;)");
		if ( num == EVENT_OBJECT_LEFT      )  strcpy(text, "Turn left (\\key left;)");
		if ( num == EVENT_OBJECT_RIGHT     )  strcpy(text, "Turn right (\\key right;)");
		if ( num == EVENT_OBJECT_UP        )  strcpy(text, "Forward (\\key up;)");
		if ( num == EVENT_OBJECT_DOWN      )  strcpy(text, "Backward (\\key down;)");
		if ( num == EVENT_OBJECT_GASUP     )  strcpy(text, "Up (\\key gup;)");
		if ( num == EVENT_OBJECT_GASDOWN   )  strcpy(text, "Down (\\key gdown;)");
		if ( num == EVENT_OBJECT_HTAKE     )  strcpy(text, "Grab or drop (\\key action;)");
		if ( num == EVENT_OBJECT_MTAKE     )  strcpy(text, "Grab or drop (\\key action;)");
		if ( num == EVENT_OBJECT_MFRONT    )  strcpy(text, "..in front");
		if ( num == EVENT_OBJECT_MBACK     )  strcpy(text, "..behind");
		if ( num == EVENT_OBJECT_MPOWER    )  strcpy(text, "..power cell");
		if ( num == EVENT_OBJECT_BHELP     )  strcpy(text, "Instructions for the mission (\\key help;)");
		if ( num == EVENT_OBJECT_BTAKEOFF  )  strcpy(text, "Take off to finish the mission");
		if ( num == EVENT_OBJECT_BDERRICK  )  strcpy(text, "Build a derrick");
		if ( num == EVENT_OBJECT_BSTATION  )  strcpy(text, "Build a power station");
		if ( num == EVENT_OBJECT_BFACTORY  )  strcpy(text, "Build a bot factory");
		if ( num == EVENT_OBJECT_BREPAIR   )  strcpy(text, "Build a repair center");
		if ( num == EVENT_OBJECT_BCONVERT  )  strcpy(text, "Build a converter");
		if ( num == EVENT_OBJECT_BTOWER    )  strcpy(text, "Build a defense tower");
		if ( num == EVENT_OBJECT_BRESEARCH )  strcpy(text, "Build a research center");
		if ( num == EVENT_OBJECT_BRADAR    )  strcpy(text, "Build a radar station");
		if ( num == EVENT_OBJECT_BENERGY   )  strcpy(text, "Build a power cell factory");
		if ( num == EVENT_OBJECT_BLABO     )  strcpy(text, "Build an autolab");
		if ( num == EVENT_OBJECT_BNUCLEAR  )  strcpy(text, "Build a nuclear power plant");
		if ( num == EVENT_OBJECT_BPARA     )  strcpy(text, "Build a lightning conductor");
		if ( num == EVENT_OBJECT_BINFO     )  strcpy(text, "Build a exchange post");
		if ( num == EVENT_OBJECT_GFLAT     )  strcpy(text, "Show if the ground is flat");
		if ( num == EVENT_OBJECT_FCREATE   )  strcpy(text, "Plant a flag");
		if ( num == EVENT_OBJECT_FDELETE   )  strcpy(text, "Remove a flag");
		if ( num == EVENT_OBJECT_FCOLORb   )  strcpy(text, "\\Blue flags");
		if ( num == EVENT_OBJECT_FCOLORr   )  strcpy(text, "\\Red flags");
		if ( num == EVENT_OBJECT_FCOLORg   )  strcpy(text, "\\Green flags");
		if ( num == EVENT_OBJECT_FCOLORy   )  strcpy(text, "\\Yellow flags");
		if ( num == EVENT_OBJECT_FCOLORv   )  strcpy(text, "\\Violet flags");
		if ( num == EVENT_OBJECT_FACTORYfa )  strcpy(text, "Build a winged grabber");
		if ( num == EVENT_OBJECT_FACTORYta )  strcpy(text, "Build a tracked grabber");
		if ( num == EVENT_OBJECT_FACTORYwa )  strcpy(text, "Build a wheeled grabber");
		if ( num == EVENT_OBJECT_FACTORYia )  strcpy(text, "Build a legged grabber");
		if ( num == EVENT_OBJECT_FACTORYfc )  strcpy(text, "Build a winged shooter");
		if ( num == EVENT_OBJECT_FACTORYtc )  strcpy(text, "Build a tracked shooter");
		if ( num == EVENT_OBJECT_FACTORYwc )  strcpy(text, "Build a wheeled shooter");
		if ( num == EVENT_OBJECT_FACTORYic )  strcpy(text, "Build a legged shooter");
		if ( num == EVENT_OBJECT_FACTORYfi )  strcpy(text, "Build a winged orga shooter");
		if ( num == EVENT_OBJECT_FACTORYti )  strcpy(text, "Build a tracked orga shooter");
		if ( num == EVENT_OBJECT_FACTORYwi )  strcpy(text, "Build a wheeled orga shooter");
		if ( num == EVENT_OBJECT_FACTORYii )  strcpy(text, "Build a legged orga shooter");
		if ( num == EVENT_OBJECT_FACTORYfs )  strcpy(text, "Build a winged sniffer");
		if ( num == EVENT_OBJECT_FACTORYts )  strcpy(text, "Build a tracked sniffer");
		if ( num == EVENT_OBJECT_FACTORYws )  strcpy(text, "Build a wheeled sniffer");
		if ( num == EVENT_OBJECT_FACTORYis )  strcpy(text, "Build a legged sniffer");
		if ( num == EVENT_OBJECT_FACTORYrt )  strcpy(text, "Build a thumper");
		if ( num == EVENT_OBJECT_FACTORYrc )  strcpy(text, "Build a phazer shooter");
		if ( num == EVENT_OBJECT_FACTORYrr )  strcpy(text, "Build a recycler");
		if ( num == EVENT_OBJECT_FACTORYrs )  strcpy(text, "Build a shielder");
		if ( num == EVENT_OBJECT_FACTORYsa )  strcpy(text, "Build a subber");
		if ( num == EVENT_OBJECT_RTANK     )  strcpy(text, "Run research program for tracked bots");
		if ( num == EVENT_OBJECT_RFLY      )  strcpy(text, "Run research program for winged bots");
		if ( num == EVENT_OBJECT_RTHUMP    )  strcpy(text, "Run research program for thumper");
		if ( num == EVENT_OBJECT_RCANON    )  strcpy(text, "Run research program for shooter");
		if ( num == EVENT_OBJECT_RTOWER    )  strcpy(text, "Run research program for defense tower");
		if ( num == EVENT_OBJECT_RPHAZER   )  strcpy(text, "Run research program for phazer shooter");
		if ( num == EVENT_OBJECT_RSHIELD   )  strcpy(text, "Run research program for shielder");
		if ( num == EVENT_OBJECT_RATOMIC   )  strcpy(text, "Run research program for nuclear power");
		if ( num == EVENT_OBJECT_RiPAW     )  strcpy(text, "Run research program for legged bots");
		if ( num == EVENT_OBJECT_RiGUN     )  strcpy(text, "Run research program for orga shooter");
		if ( num == EVENT_OBJECT_RESET     )  strcpy(text, "Return to start");
		if ( num == EVENT_OBJECT_SEARCH    )  strcpy(text, "Sniff (\\key action;)");
		if ( num == EVENT_OBJECT_TERRAFORM )  strcpy(text, "Thump (\\key action;)");
		if ( num == EVENT_OBJECT_FIRE      )  strcpy(text, "Shoot (\\key action;)");
		if ( num == EVENT_OBJECT_RECOVER   )  strcpy(text, "Recycle (\\key action;)");
		if ( num == EVENT_OBJECT_BEGSHIELD )  strcpy(text, "Extend shield (\\key action;)");
		if ( num == EVENT_OBJECT_ENDSHIELD )  strcpy(text, "Withdraw shield (\\key action;)");
		if ( num == EVENT_OBJECT_DIMSHIELD )  strcpy(text, "Shield radius");
		if ( num == EVENT_OBJECT_PROGRUN   )  strcpy(text, "Execute the selected program");
		if ( num == EVENT_OBJECT_PROGEDIT  )  strcpy(text, "Edit the selected program");
		if ( num == EVENT_OBJECT_INFOOK    )  strcpy(text, "\\SatCom on standby");
		if ( num == EVENT_OBJECT_DELETE    )  strcpy(text, "Destroy the building");
		if ( num == EVENT_OBJECT_GENERGY   )  strcpy(text, "Energy level");
		if ( num == EVENT_OBJECT_GSHIELD   )  strcpy(text, "Shield level");
		if ( num == EVENT_OBJECT_GRANGE    )  strcpy(text, "Jet temperature");
		if ( num == EVENT_OBJECT_GPROGRESS )  strcpy(text, "Still working ...");
		if ( num == EVENT_OBJECT_GRADAR    )  strcpy(text, "Number of insects detected");
		if ( num == EVENT_OBJECT_GINFO     )  strcpy(text, "Transmitted information");
		if ( num == EVENT_OBJECT_COMPASS   )  strcpy(text, "Compass");
//?		if ( num == EVENT_OBJECT_MAP       )  strcpy(text, "Mini-map");
		if ( num == EVENT_OBJECT_MAPZOOM   )  strcpy(text, "Zoom mini-map");
		if ( num == EVENT_OBJECT_CAMERA    )  strcpy(text, "Camera (\\key camera;)");
		if ( num == EVENT_OBJECT_CAMERAleft)  strcpy(text, "Camera to left");
		if ( num == EVENT_OBJECT_CAMERAright) strcpy(text, "Camera to right");
		if ( num == EVENT_OBJECT_CAMERAnear)  strcpy(text, "Camera nearest");
		if ( num == EVENT_OBJECT_CAMERAaway)  strcpy(text, "Camera awayest");
		if ( num == EVENT_OBJECT_HELP      )  strcpy(text, "Help about selected object");
		if ( num == EVENT_OBJECT_SOLUCE    )  strcpy(text, "Show the solution");
		if ( num == EVENT_OBJECT_SHORTCUT00)  strcpy(text, "Switch bots <-> buildings");
		if ( num == EVENT_OBJECT_LIMIT     )  strcpy(text, "Show the range");
		if ( num == EVENT_OBJECT_PEN0      )  strcpy(text, "\\Raise the pencil");
		if ( num == EVENT_OBJECT_PEN1      )  strcpy(text, "\\Use the black pencil");
		if ( num == EVENT_OBJECT_PEN2      )  strcpy(text, "\\Use the yellow pencil");
		if ( num == EVENT_OBJECT_PEN3      )  strcpy(text, "\\Use the orange pencil");
		if ( num == EVENT_OBJECT_PEN4      )  strcpy(text, "\\Use the red pencil");
		if ( num == EVENT_OBJECT_PEN5      )  strcpy(text, "\\Use the purple pencil");
		if ( num == EVENT_OBJECT_PEN6      )  strcpy(text, "\\Use the blue pencil");
		if ( num == EVENT_OBJECT_PEN7      )  strcpy(text, "\\Use the green pencil");
		if ( num == EVENT_OBJECT_PEN8      )  strcpy(text, "\\Use the brown pencil");
		if ( num == EVENT_OBJECT_REC       )  strcpy(text, "\\Start recording");
		if ( num == EVENT_OBJECT_STOP      )  strcpy(text, "\\Stop recording");
		if ( num == EVENT_DT_VISIT0        ||
			 num == EVENT_DT_VISIT1        ||
			 num == EVENT_DT_VISIT2        ||
			 num == EVENT_DT_VISIT3        ||
			 num == EVENT_DT_VISIT4        )  strcpy(text, "Show the place");
		if ( num == EVENT_DT_END           )  strcpy(text, "Continue");
		if ( num == EVENT_CMD              )  strcpy(text, "Command line");
		if ( num == EVENT_SPEED            )  strcpy(text, "Game speed");

		if ( num == EVENT_HYPER_PREV       )  strcpy(text, "Back");
		if ( num == EVENT_HYPER_NEXT       )  strcpy(text, "Forward");
		if ( num == EVENT_HYPER_HOME       )  strcpy(text, "Home");
		if ( num == EVENT_HYPER_COPY       )  strcpy(text, "Copy");
		if ( num == EVENT_HYPER_SIZE1      )  strcpy(text, "Size 1");
		if ( num == EVENT_HYPER_SIZE2      )  strcpy(text, "Size 2");
		if ( num == EVENT_HYPER_SIZE3      )  strcpy(text, "Size 3");
		if ( num == EVENT_HYPER_SIZE4      )  strcpy(text, "Size 4");
		if ( num == EVENT_HYPER_SIZE5      )  strcpy(text, "Size 5");
		if ( num == EVENT_SATCOM_HUSTON    )  strcpy(text, "Instructions from Houston");
#if _TEEN
		if ( num == EVENT_SATCOM_SAT       )  strcpy(text, "Dictionnary");
#else
		if ( num == EVENT_SATCOM_SAT       )  strcpy(text, "Satellite report");
#endif
		if ( num == EVENT_SATCOM_LOADING   )  strcpy(text, "Programs dispatched by Houston");
		if ( num == EVENT_SATCOM_OBJECT    )  strcpy(text, "List of objects");
		if ( num == EVENT_SATCOM_PROG      )  strcpy(text, "Programming help");
		if ( num == EVENT_SATCOM_SOLUCE    )  strcpy(text, "Solution");

		if ( num == EVENT_STUDIO_OK        )  strcpy(text, "OK\\Close program editor and return to game");
		if ( num == EVENT_STUDIO_CANCEL    )  strcpy(text, "Cancel\\Cancel all changes");
		if ( num == EVENT_STUDIO_NEW       )  strcpy(text, "New");
		if ( num == EVENT_STUDIO_OPEN      )  strcpy(text, "Open (Ctrl+o)");
		if ( num == EVENT_STUDIO_SAVE      )  strcpy(text, "Save (Ctrl+s)");
		if ( num == EVENT_STUDIO_UNDO      )  strcpy(text, "Undo (Ctrl+z)");
		if ( num == EVENT_STUDIO_CUT       )  strcpy(text, "Cut (Ctrl+x)");
		if ( num == EVENT_STUDIO_COPY      )  strcpy(text, "Copy (Ctrl+c)");
		if ( num == EVENT_STUDIO_PASTE     )  strcpy(text, "Paste (Ctrl+v)");
		if ( num == EVENT_STUDIO_SIZE      )  strcpy(text, "Font size");
		if ( num == EVENT_STUDIO_TOOL      )  strcpy(text, "Instructions (\\key help;)");
		if ( num == EVENT_STUDIO_HELP      )  strcpy(text, "Programming help  (\\key prog;)");
		if ( num == EVENT_STUDIO_COMPILE   )  strcpy(text, "Compile");
		if ( num == EVENT_STUDIO_RUN       )  strcpy(text, "Execute/stop");
		if ( num == EVENT_STUDIO_REALTIME  )  strcpy(text, "Pause/continue");
		if ( num == EVENT_STUDIO_STEP      )  strcpy(text, "One step");
	}

	if ( type == RES_OBJECT )
	{
		if ( num == OBJECT_PORTICO      )  strcpy(text, "Gantry crane");
		if ( num == OBJECT_BASE         )  strcpy(text, "Spaceship");
		if ( num == OBJECT_DERRICK      )  strcpy(text, "Derrick");
		if ( num == OBJECT_FACTORY      )  strcpy(text, "Bot factory");
		if ( num == OBJECT_REPAIR       )  strcpy(text, "Repair center");
		if ( num == OBJECT_DESTROYER    )  strcpy(text, "Destroyer");
		if ( num == OBJECT_STATION      )  strcpy(text, "Power station");
		if ( num == OBJECT_CONVERT      )  strcpy(text, "Converts ore to titanium");
		if ( num == OBJECT_TOWER        )  strcpy(text, "Defense tower");
		if ( num == OBJECT_NEST         )  strcpy(text, "Nest");
		if ( num == OBJECT_RESEARCH     )  strcpy(text, "Research center");
		if ( num == OBJECT_RADAR        )  strcpy(text, "Radar station");
		if ( num == OBJECT_INFO         )  strcpy(text, "Information exchange post");
#if _TEEN
		if ( num == OBJECT_ENERGY       )  strcpy(text, "Power cell factory");
#else
		if ( num == OBJECT_ENERGY       )  strcpy(text, "Power cell factory");
#endif
		if ( num == OBJECT_LABO         )  strcpy(text, "Autolab");
		if ( num == OBJECT_NUCLEAR      )  strcpy(text, "Nuclear power station");
		if ( num == OBJECT_PARA         )  strcpy(text, "Lightning conductor");
		if ( num == OBJECT_SAFE         )  strcpy(text, "Vault");
		if ( num == OBJECT_HUSTON       )  strcpy(text, "Houston Mission Control");
		if ( num == OBJECT_TARGET1      )  strcpy(text, "Target");
		if ( num == OBJECT_TARGET2      )  strcpy(text, "Target");
		if ( num == OBJECT_START        )  strcpy(text, "Start");
		if ( num == OBJECT_END          )  strcpy(text, "Finish");
		if ( num == OBJECT_STONE        )  strcpy(text, "Titanium ore");
		if ( num == OBJECT_URANIUM      )  strcpy(text, "Uranium ore");
		if ( num == OBJECT_BULLET       )  strcpy(text, "Organic matter");
		if ( num == OBJECT_METAL        )  strcpy(text, "Titanium");
		if ( num == OBJECT_POWER        )  strcpy(text, "Power cell");
		if ( num == OBJECT_ATOMIC       )  strcpy(text, "Nuclear power cell");
		if ( num == OBJECT_BBOX         )  strcpy(text, "Black box");
		if ( num == OBJECT_KEYa         )  strcpy(text, "Key A");
		if ( num == OBJECT_KEYb         )  strcpy(text, "Key B");
		if ( num == OBJECT_KEYc         )  strcpy(text, "Key C");
		if ( num == OBJECT_KEYd         )  strcpy(text, "Key D");
		if ( num == OBJECT_TNT          )  strcpy(text, "Explosive");
		if ( num == OBJECT_BOMB         )  strcpy(text, "Fixed mine");
		if ( num == OBJECT_BAG          )  strcpy(text, "Survival kit");
		if ( num == OBJECT_WAYPOINT     )  strcpy(text, "Checkpoint");
		if ( num == OBJECT_FLAGb        )  strcpy(text, "Blue flag");
		if ( num == OBJECT_FLAGr        )  strcpy(text, "Red flag");
		if ( num == OBJECT_FLAGg        )  strcpy(text, "Green flag");
		if ( num == OBJECT_FLAGy        )  strcpy(text, "Yellow flag");
		if ( num == OBJECT_FLAGv        )  strcpy(text, "Violet flag");
		if ( num == OBJECT_MARKPOWER    )  strcpy(text, "Energy deposit (site for power station)");
		if ( num == OBJECT_MARKURANIUM  )  strcpy(text, "Uranium deposit (site for derrick)");
		if ( num == OBJECT_MARKKEYa     )  strcpy(text, "Found key A (site for derrick)");
		if ( num == OBJECT_MARKKEYb     )  strcpy(text, "Found key B (site for derrick)");
		if ( num == OBJECT_MARKKEYc     )  strcpy(text, "Found key C (site for derrick)");
		if ( num == OBJECT_MARKKEYd     )  strcpy(text, "Found key D (site for derrick)");
		if ( num == OBJECT_MARKSTONE    )  strcpy(text, "Titanium deposit (site for derrick)");
		if ( num == OBJECT_MOBILEft     )  strcpy(text, "Practice bot");
		if ( num == OBJECT_MOBILEtt     )  strcpy(text, "Practice bot");
		if ( num == OBJECT_MOBILEwt     )  strcpy(text, "Practice bot");
		if ( num == OBJECT_MOBILEit     )  strcpy(text, "Practice bot");
		if ( num == OBJECT_MOBILEfa     )  strcpy(text, "Winged grabber");
		if ( num == OBJECT_MOBILEta     )  strcpy(text, "Tracked grabber");
		if ( num == OBJECT_MOBILEwa     )  strcpy(text, "Wheeled grabber");
		if ( num == OBJECT_MOBILEia     )  strcpy(text, "Legged grabber");
		if ( num == OBJECT_MOBILEfc     )  strcpy(text, "Winged shooter");
		if ( num == OBJECT_MOBILEtc     )  strcpy(text, "Tracked shooter");
		if ( num == OBJECT_MOBILEwc     )  strcpy(text, "Wheeled shooter");
		if ( num == OBJECT_MOBILEic     )  strcpy(text, "Legged shooter");
		if ( num == OBJECT_MOBILEfi     )  strcpy(text, "Winged orga shooter");
		if ( num == OBJECT_MOBILEti     )  strcpy(text, "Tracked orga shooter");
		if ( num == OBJECT_MOBILEwi     )  strcpy(text, "Wheeled orga shooter");
		if ( num == OBJECT_MOBILEii     )  strcpy(text, "Legged orga shooter");
		if ( num == OBJECT_MOBILEfs     )  strcpy(text, "Winged sniffer");
		if ( num == OBJECT_MOBILEts     )  strcpy(text, "Tracked sniffer");
		if ( num == OBJECT_MOBILEws     )  strcpy(text, "Wheeled sniffer");
		if ( num == OBJECT_MOBILEis     )  strcpy(text, "Legged sniffer");
		if ( num == OBJECT_MOBILErt     )  strcpy(text, "Thumper");
		if ( num == OBJECT_MOBILErc     )  strcpy(text, "Phazer shooter");
		if ( num == OBJECT_MOBILErr     )  strcpy(text, "Recycler");
		if ( num == OBJECT_MOBILErs     )  strcpy(text, "Shielder");
		if ( num == OBJECT_MOBILEsa     )  strcpy(text, "Subber");
		if ( num == OBJECT_MOBILEtg     )  strcpy(text, "Target bot");
		if ( num == OBJECT_MOBILEdr     )  strcpy(text, "Drawer bot");
		if ( num == OBJECT_HUMAN        )  strcpy(text, g_gamerName);
		if ( num == OBJECT_TECH         )  strcpy(text, "Engineer");
		if ( num == OBJECT_TOTO         )  strcpy(text, "Robbie");
		if ( num == OBJECT_MOTHER       )  strcpy(text, "Alien Queen");
		if ( num == OBJECT_ANT          )  strcpy(text, "Ant");
		if ( num == OBJECT_SPIDER       )  strcpy(text, "Spider");
		if ( num == OBJECT_BEE          )  strcpy(text, "Wasp");
		if ( num == OBJECT_WORM         )  strcpy(text, "Worm");
		if ( num == OBJECT_EGG          )  strcpy(text, "Egg");
		if ( num == OBJECT_RUINmobilew1 )  strcpy(text, "Wreckage");
		if ( num == OBJECT_RUINmobilew2 )  strcpy(text, "Wreckage");
		if ( num == OBJECT_RUINmobilet1 )  strcpy(text, "Wreckage");
		if ( num == OBJECT_RUINmobilet2 )  strcpy(text, "Wreckage");
		if ( num == OBJECT_RUINmobiler1 )  strcpy(text, "Wreckage");
		if ( num == OBJECT_RUINmobiler2 )  strcpy(text, "Wreckage");
		if ( num == OBJECT_RUINfactory  )  strcpy(text, "Ruin");
		if ( num == OBJECT_RUINdoor     )  strcpy(text, "Ruin");
		if ( num == OBJECT_RUINsupport  )  strcpy(text, "Waste");
		if ( num == OBJECT_RUINradar    )  strcpy(text, "Ruin");
		if ( num == OBJECT_RUINconvert  )  strcpy(text, "Ruin");
		if ( num == OBJECT_RUINbase     )  strcpy(text, "Spaceship ruin");
		if ( num == OBJECT_RUINhead     )  strcpy(text, "Spaceship ruin");
		if ( num == OBJECT_APOLLO1      ||
			 num == OBJECT_APOLLO3      ||
			 num == OBJECT_APOLLO4      ||
			 num == OBJECT_APOLLO5      )  strcpy(text, "Remains of Apollo mission");
		if ( num == OBJECT_APOLLO2      )  strcpy(text, "Lunar Roving Vehicle");
	}

	if ( type == RES_ERR )
	{
		strcpy(text, "Error");
		if ( num == ERR_CMD             )  strcpy(text, "Unknown command");
#if _NEWLOOK
		if ( num == ERR_INSTALL         )  strcpy(text, "CeeBot not installed.");
		if ( num == ERR_NOCD            )  strcpy(text, "Please insert the CeeBot CD\nand re-run the game.");
#else
		if ( num == ERR_INSTALL         )  strcpy(text, "COLOBOT not installed.");
		if ( num == ERR_NOCD            )  strcpy(text, "Please insert the COLOBOT CD\nand re-run the game.");
#endif
		if ( num == ERR_MANIP_VEH       )  strcpy(text, "Inappropriate bot");
		if ( num == ERR_MANIP_FLY       )  strcpy(text, "Impossible when flying");
		if ( num == ERR_MANIP_BUSY      )  strcpy(text, "Already carrying something");
		if ( num == ERR_MANIP_NIL       )  strcpy(text, "Nothing to grab");
		if ( num == ERR_MANIP_MOTOR     )  strcpy(text, "Impossible when moving");
		if ( num == ERR_MANIP_OCC       )  strcpy(text, "Place occupied");
		if ( num == ERR_MANIP_FRIEND    )  strcpy(text, "No other robot");
		if ( num == ERR_MANIP_RADIO     )  strcpy(text, "You can not carry a radioactive object");
		if ( num == ERR_MANIP_WATER     )  strcpy(text, "You can not carry an object under water");
		if ( num == ERR_MANIP_EMPTY     )  strcpy(text, "Nothing to drop");
		if ( num == ERR_BUILD_FLY       )  strcpy(text, "Impossible when flying");
		if ( num == ERR_BUILD_WATER     )  strcpy(text, "Impossible under water");
		if ( num == ERR_BUILD_ENERGY    )  strcpy(text, "Not enough energy");
		if ( num == ERR_BUILD_METALAWAY )  strcpy(text, "Titanium too far away");
		if ( num == ERR_BUILD_METALNEAR )  strcpy(text, "Titanium too close");
		if ( num == ERR_BUILD_METALINEX )  strcpy(text, "No titanium around");
		if ( num == ERR_BUILD_FLAT      )  strcpy(text, "Ground not flat enough");
		if ( num == ERR_BUILD_FLATLIT   )  strcpy(text, "Flat ground not large enough");
		if ( num == ERR_BUILD_BUSY      )  strcpy(text, "Place occupied");
		if ( num == ERR_BUILD_BASE      )  strcpy(text, "Too close to space ship");
		if ( num == ERR_BUILD_NARROW    )  strcpy(text, "Too close to a building");
		if ( num == ERR_BUILD_MOTOR     )  strcpy(text, "Impossible when moving");
		if ( num == ERR_SEARCH_FLY      )  strcpy(text, "Impossible when flying");
		if ( num == ERR_SEARCH_VEH      )  strcpy(text, "Inappropriate bot");
		if ( num == ERR_SEARCH_MOTOR    )  strcpy(text, "Impossible when moving");
		if ( num == ERR_TERRA_VEH       )  strcpy(text, "Inappropriate bot");
		if ( num == ERR_TERRA_ENERGY    )  strcpy(text, "Not enough energy");
		if ( num == ERR_TERRA_FLOOR     )  strcpy(text, "Ground inappropriate");
		if ( num == ERR_TERRA_BUILDING  )  strcpy(text, "Building too close");
		if ( num == ERR_TERRA_OBJECT    )  strcpy(text, "Object too close");
		if ( num == ERR_RECOVER_VEH     )  strcpy(text, "Inappropriate bot");
		if ( num == ERR_RECOVER_ENERGY  )  strcpy(text, "Not enough energy");
		if ( num == ERR_RECOVER_NULL    )  strcpy(text, "Nothing to recycle");
		if ( num == ERR_SHIELD_VEH      )  strcpy(text, "Inappropriate bot");
		if ( num == ERR_SHIELD_ENERGY   )  strcpy(text, "No more energy");
		if ( num == ERR_MOVE_IMPOSSIBLE )  strcpy(text, "Error in instruction move");
		if ( num == ERR_FIND_IMPOSSIBLE )  strcpy(text, "Object not found");
		if ( num == ERR_GOTO_IMPOSSIBLE )  strcpy(text, "Goto: inaccessible destination");
		if ( num == ERR_GOTO_ITER       )  strcpy(text, "Goto: inaccessible destination");
		if ( num == ERR_GOTO_BUSY       )  strcpy(text, "Goto: destination occupied");
		if ( num == ERR_FIRE_VEH        )  strcpy(text, "Inappropriate bot");
		if ( num == ERR_FIRE_ENERGY     )  strcpy(text, "Not enough energy");
		if ( num == ERR_FIRE_FLY        )  strcpy(text, "Impossible when flying");
		if ( num == ERR_CONVERT_EMPTY   )  strcpy(text, "No titanium ore to convert");
		if ( num == ERR_DERRICK_NULL    )  strcpy(text, "No ore in the subsoil");
		if ( num == ERR_STATION_NULL    )  strcpy(text, "No energy in the subsoil");
		if ( num == ERR_TOWER_POWER     )  strcpy(text, "No power cell");
		if ( num == ERR_TOWER_ENERGY    )  strcpy(text, "No more energy");
		if ( num == ERR_RESEARCH_POWER  )  strcpy(text, "No power cell");
		if ( num == ERR_RESEARCH_ENERGY )  strcpy(text, "Not enough energy");
		if ( num == ERR_RESEARCH_TYPE   )  strcpy(text, "Inappropriate cell type");
		if ( num == ERR_RESEARCH_ALREADY)  strcpy(text, "Research program already performed");
		if ( num == ERR_ENERGY_NULL     )  strcpy(text, "No energy in the subsoil");
		if ( num == ERR_ENERGY_LOW      )  strcpy(text, "Not enough energy yet");
		if ( num == ERR_ENERGY_EMPTY    )  strcpy(text, "No titanium to transform");
		if ( num == ERR_ENERGY_BAD      )  strcpy(text, "Transforms only titanium");
		if ( num == ERR_BASE_DLOCK      )  strcpy(text, "Doors blocked by a robot or another object ");
		if ( num == ERR_BASE_DHUMAN     )  strcpy(text, "You must get on the spaceship to take off ");
		if ( num == ERR_LABO_NULL       )  strcpy(text, "Nothing to analyze");
		if ( num == ERR_LABO_BAD        )  strcpy(text, "Analyzes only organic matter");
		if ( num == ERR_LABO_ALREADY    )  strcpy(text, "Analysis already performed");
		if ( num == ERR_NUCLEAR_NULL    )  strcpy(text, "No energy in the subsoil");
		if ( num == ERR_NUCLEAR_LOW     )  strcpy(text, "Not yet enough energy");
		if ( num == ERR_NUCLEAR_EMPTY   )  strcpy(text, "No uranium to transform");
		if ( num == ERR_NUCLEAR_BAD     )  strcpy(text, "Transforms only uranium");
		if ( num == ERR_FACTORY_NULL    )  strcpy(text, "No titanium");
		if ( num == ERR_FACTORY_NEAR    )  strcpy(text, "Object too close");
		if ( num == ERR_RESET_NEAR      )  strcpy(text, "Place occupied");
		if ( num == ERR_INFO_NULL       )  strcpy(text, "No information exchange post within range");
		if ( num == ERR_VEH_VIRUS       )  strcpy(text, "Program infected by a virus");
		if ( num == ERR_BAT_VIRUS       )  strcpy(text, "Infected by a virus, temporarily out of order");
		if ( num == ERR_VEH_POWER       )  strcpy(text, "No power cell");
		if ( num == ERR_VEH_ENERGY      )  strcpy(text, "No more energy");
		if ( num == ERR_FLAG_FLY        )  strcpy(text, "Impossible when flying");
		if ( num == ERR_FLAG_WATER      )  strcpy(text, "Impossible when swimming");
		if ( num == ERR_FLAG_MOTOR      )  strcpy(text, "Impossible when moving");
		if ( num == ERR_FLAG_BUSY       )  strcpy(text, "Impossible when carrying an object");
		if ( num == ERR_FLAG_CREATE     )  strcpy(text, "Too many flags of this color (maximum 5)");
		if ( num == ERR_FLAG_PROXY      )  strcpy(text, "Too close to an existing flag");
		if ( num == ERR_FLAG_DELETE     )  strcpy(text, "No flag nearby");
		if ( num == ERR_MISSION_NOTERM  )  strcpy(text, "The mission is not accomplished yet (press \\key help; for more details)");
		if ( num == ERR_DELETEMOBILE    )  strcpy(text, "Bot destroyed");
		if ( num == ERR_DELETEBUILDING  )  strcpy(text, "Building destroyed");
		if ( num == ERR_TOOMANY         )  strcpy(text, "Can not create this, there are too many objects");
		if ( num == ERR_OBLIGATORYTOKEN )  strcpy(text, "\"%s\" missing in this exercise");  //**
		if ( num == ERR_PROHIBITEDTOKEN )  strcpy(text, "Do not use in this exercise");  //**

		if ( num == INFO_BUILD          )  strcpy(text, "Building completed");
		if ( num == INFO_CONVERT        )  strcpy(text, "Titanium available");
		if ( num == INFO_RESEARCH       )  strcpy(text, "Research program completed");
		if ( num == INFO_RESEARCHTANK   )  strcpy(text, "Plans for tracked robots available ");
		if ( num == INFO_RESEARCHFLY    )  strcpy(text, "You can fly with the keys (\\key gup;) and (\\key gdown;)");
		if ( num == INFO_RESEARCHTHUMP  )  strcpy(text, "Plans for thumper available");
		if ( num == INFO_RESEARCHCANON  )  strcpy(text, "Plans for shooter available");
		if ( num == INFO_RESEARCHTOWER  )  strcpy(text, "Plans for defense tower available");
		if ( num == INFO_RESEARCHPHAZER )  strcpy(text, "Plans for phazer shooter available");
		if ( num == INFO_RESEARCHSHIELD )  strcpy(text, "Plans for shielder available");
		if ( num == INFO_RESEARCHATOMIC )  strcpy(text, "Plans for nuclear power plant available");
		if ( num == INFO_FACTORY        )  strcpy(text, "New bot available");
		if ( num == INFO_LABO           )  strcpy(text, "Analysis performed");
		if ( num == INFO_ENERGY         )  strcpy(text, "Power cell available");
		if ( num == INFO_NUCLEAR        )  strcpy(text, "Nuclear power cell available");
		if ( num == INFO_FINDING        )  strcpy(text, "You found a usable object");
		if ( num == INFO_MARKPOWER      )  strcpy(text, "Found a site for power station");
		if ( num == INFO_MARKURANIUM    )  strcpy(text, "Found a site for a derrick");
		if ( num == INFO_MARKSTONE      )  strcpy(text, "Found a site for a derrick");
		if ( num == INFO_MARKKEYa       )  strcpy(text, "Found a site for a derrick");
		if ( num == INFO_MARKKEYb       )  strcpy(text, "Found a site for a derrick");
		if ( num == INFO_MARKKEYc       )  strcpy(text, "Found a site for a derrick");
		if ( num == INFO_MARKKEYd       )  strcpy(text, "Found a site for a derrick");
		if ( num == INFO_WIN            )  strcpy(text, "<<< Well done, mission accomplished >>>");
		if ( num == INFO_LOST           )  strcpy(text, "<<< Sorry, mission failed >>>");
		if ( num == INFO_LOSTq          )  strcpy(text, "<<< Sorry, mission failed >>>");
		if ( num == INFO_WRITEOK        )  strcpy(text, "Current mission saved");
		if ( num == INFO_DELETEPATH     )  strcpy(text, "Checkpoint crossed");
		if ( num == INFO_DELETEMOTHER   )  strcpy(text, "Alien Queen killed");
		if ( num == INFO_DELETEANT      )  strcpy(text, "Ant fatally wounded");
		if ( num == INFO_DELETEBEE      )  strcpy(text, "Wasp fatally wounded");
		if ( num == INFO_DELETEWORM     )  strcpy(text, "Worm fatally wounded");
		if ( num == INFO_DELETESPIDER   )  strcpy(text, "Spider fatally wounded");
		if ( num == INFO_BEGINSATCOM    )  strcpy(text, "Press \\key help; to read instructions on your SatCom");
	}

	if ( type == RES_CBOT )
	{
		strcpy(text, "Error");
		if ( num == TX_OPENPAR       ) strcpy(text, "Opening bracket missing");
		if ( num == TX_CLOSEPAR      ) strcpy(text, "Closing bracket missing ");
		if ( num == TX_NOTBOOL       ) strcpy(text, "The expression must return a boolean value");
		if ( num == TX_UNDEFVAR      ) strcpy(text, "Variable not declared");
		if ( num == TX_BADLEFT       ) strcpy(text, "Assignment impossible");
		if ( num == TX_ENDOF         ) strcpy(text, "Semicolon terminator missing");
		if ( num == TX_OUTCASE       ) strcpy(text, "Instruction ""case"" outside a block ""switch""");
		if ( num == TX_NOTERM        ) strcpy(text, "Instructions after the final closing brace");
		if ( num == TX_CLOSEBLK      ) strcpy(text, "End of block missing");
		if ( num == TX_ELSEWITHOUTIF ) strcpy(text, "Instruction ""else"" without corresponding ""if"" ");
		if ( num == TX_OPENBLK       ) strcpy(text, "Opening brace missing ");// beginning of a block expected?
		if ( num == TX_BADTYPE       ) strcpy(text, "Wrong type for the assignment");
		if ( num == TX_REDEFVAR      ) strcpy(text, "A variable can not be declared twice");
		if ( num == TX_BAD2TYPE      ) strcpy(text, "The types of the two operands are incompatible ");
		if ( num == TX_UNDEFCALL     ) strcpy(text, "Unknown function");
		if ( num == TX_MISDOTS       ) strcpy(text, "Sign "" : "" missing");
		if ( num == TX_WHILE         ) strcpy(text, "Keyword ""while"" missing");
		if ( num == TX_BREAK         ) strcpy(text, "Instruction ""break"" outside a loop");
		if ( num == TX_LABEL         ) strcpy(text, "A label must be followed by ""for"", ""while"", ""do"" or ""switch""");
		if ( num == TX_NOLABEL       ) strcpy(text, "This label does not exist");// This label doesn't exist
		if ( num == TX_NOCASE        ) strcpy(text, "Instruction ""case"" missing");
		if ( num == TX_BADNUM        ) strcpy(text, "Number missing");
		if ( num == TX_VOID          ) strcpy(text, "Void parameter");
		if ( num == TX_NOTYP         ) strcpy(text, "Type declaration missing");
		if ( num == TX_NOVAR         ) strcpy(text, "Variable name missing");
		if ( num == TX_NOFONC        ) strcpy(text, "Function name missing");
		if ( num == TX_OVERPARAM     ) strcpy(text, "Too many parameters");
		if ( num == TX_REDEF         ) strcpy(text, "Function already exists");
		if ( num == TX_LOWPARAM      ) strcpy(text, "Parameters missing ");
		if ( num == TX_BADPARAM      ) strcpy(text, "No function with this name accepts this kind of parameter");
		if ( num == TX_NUMPARAM      ) strcpy(text, "No function with this name accepts this number of parameters");
		if ( num == TX_NOITEM        ) strcpy(text, "This is not a member of this class");
		if ( num == TX_DOT           ) strcpy(text, "This object is not a member of a class");
		if ( num == TX_NOCONST       ) strcpy(text, "Appropriate constructor missing");
		if ( num == TX_REDEFCLASS    ) strcpy(text, "This class already exists");
		if ( num == TX_CLBRK         ) strcpy(text, """ ] "" missing");
		if ( num == TX_RESERVED      ) strcpy(text, "Reserved keyword of CBOT language");
		if ( num == TX_BADNEW        ) strcpy(text, "Bad argument for ""new""");
		if ( num == TX_OPBRK         ) strcpy(text, """ [ "" expected");
		if ( num == TX_BADSTRING     ) strcpy(text, "String missing");
		if ( num == TX_BADINDEX      ) strcpy(text, "Incorrect index type");
		if ( num == TX_PRIVATE       ) strcpy(text, "Private element");
		if ( num == TX_NOPUBLIC      ) strcpy(text, "Public required");
		if ( num == TX_DIVZERO       ) strcpy(text, "Dividing by zero");
		if ( num == TX_NOTINIT       ) strcpy(text, "Variable not initialized");
		if ( num == TX_BADTHROW      ) strcpy(text, "Negative value rejected by ""throw""");//What is this?
		if ( num == TX_NORETVAL      ) strcpy(text, "The function returned no value ");
		if ( num == TX_NORUN         ) strcpy(text, "No function running");
		if ( num == TX_NOCALL        ) strcpy(text, "Calling an unknown function");
		if ( num == TX_NOCLASS       ) strcpy(text, "This class does not exist");
		if ( num == TX_NULLPT        ) strcpy(text, "Unknown Object");
		if ( num == TX_OPNAN         ) strcpy(text, "Operation impossible with value ""nan""");
		if ( num == TX_OUTARRAY      ) strcpy(text, "Access beyond array limit");
		if ( num == TX_STACKOVER     ) strcpy(text, "Stack overflow");
		if ( num == TX_DELETEDPT     ) strcpy(text, "Illegal object");
		if ( num == TX_FILEOPEN      ) strcpy(text, "Can't open file");
		if ( num == TX_NOTOPEN       ) strcpy(text, "File not open");
		if ( num == TX_ERRREAD       ) strcpy(text, "Read error");
		if ( num == TX_ERRWRITE      ) strcpy(text, "Write error");
	}

	if ( type == RES_KEY )
	{
		if ( num == 0                   )  strcpy(text, "< none >");
		if ( num == VK_LEFT             )  strcpy(text, "Arrow left");
		if ( num == VK_RIGHT            )  strcpy(text, "Arrow right");
		if ( num == VK_UP               )  strcpy(text, "Arrow up");
		if ( num == VK_DOWN             )  strcpy(text, "Arrow down");
		if ( num == VK_CANCEL           )  strcpy(text, "Control-break");
		if ( num == VK_BACK             )  strcpy(text, "<--");
		if ( num == VK_TAB              )  strcpy(text, "Tab");
		if ( num == VK_CLEAR            )  strcpy(text, "Clear");
		if ( num == VK_RETURN           )  strcpy(text, "Enter");
		if ( num == VK_SHIFT            )  strcpy(text, "Shift");
		if ( num == VK_CONTROL          )  strcpy(text, "Ctrl");
		if ( num == VK_MENU             )  strcpy(text, "Alt");
		if ( num == VK_PAUSE            )  strcpy(text, "Pause");
		if ( num == VK_CAPITAL          )  strcpy(text, "Caps Lock");
		if ( num == VK_ESCAPE           )  strcpy(text, "Esc");
		if ( num == VK_SPACE            )  strcpy(text, "Space");
		if ( num == VK_PRIOR            )  strcpy(text, "Page Up");
		if ( num == VK_NEXT             )  strcpy(text, "Page Down");
		if ( num == VK_END              )  strcpy(text, "End");
		if ( num == VK_HOME             )  strcpy(text, "Home");
		if ( num == VK_SELECT           )  strcpy(text, "Select");
		if ( num == VK_EXECUTE          )  strcpy(text, "Execute");
		if ( num == VK_SNAPSHOT         )  strcpy(text, "Print Scrn");
		if ( num == VK_INSERT           )  strcpy(text, "Insert");
		if ( num == VK_DELETE           )  strcpy(text, "Delete");
		if ( num == VK_HELP             )  strcpy(text, "Help");
		if ( num == VK_LWIN             )  strcpy(text, "Left Windows");
		if ( num == VK_RWIN             )  strcpy(text, "Right Windows");
		if ( num == VK_APPS             )  strcpy(text, "Application key");
		if ( num == VK_NUMPAD0          )  strcpy(text, "NumPad 0");
		if ( num == VK_NUMPAD1          )  strcpy(text, "NumPad 1");
		if ( num == VK_NUMPAD2          )  strcpy(text, "NumPad 2");
		if ( num == VK_NUMPAD3          )  strcpy(text, "NumPad 3");
		if ( num == VK_NUMPAD4          )  strcpy(text, "NumPad 4");
		if ( num == VK_NUMPAD5          )  strcpy(text, "NumPad 5");
		if ( num == VK_NUMPAD6          )  strcpy(text, "NumPad 6");
		if ( num == VK_NUMPAD7          )  strcpy(text, "NumPad 7");
		if ( num == VK_NUMPAD8          )  strcpy(text, "NumPad 8");
		if ( num == VK_NUMPAD9          )  strcpy(text, "NumPad 9");
		if ( num == VK_MULTIPLY         )  strcpy(text, "NumPad *");
		if ( num == VK_ADD              )  strcpy(text, "NumPad +");
		if ( num == VK_SEPARATOR        )  strcpy(text, "NumPad sep");
		if ( num == VK_SUBTRACT         )  strcpy(text, "NumPad -");
		if ( num == VK_DECIMAL          )  strcpy(text, "NumPad .");
		if ( num == VK_DIVIDE           )  strcpy(text, "NumPad /");
		if ( num == VK_F1               )  strcpy(text, "F1");
		if ( num == VK_F2               )  strcpy(text, "F2");
		if ( num == VK_F3               )  strcpy(text, "F3");
		if ( num == VK_F4               )  strcpy(text, "F4");
		if ( num == VK_F5               )  strcpy(text, "F5");
		if ( num == VK_F6               )  strcpy(text, "F6");
		if ( num == VK_F7               )  strcpy(text, "F7");
		if ( num == VK_F8               )  strcpy(text, "F8");
		if ( num == VK_F9               )  strcpy(text, "F9");
		if ( num == VK_F10              )  strcpy(text, "F10");
		if ( num == VK_F11              )  strcpy(text, "F11");
		if ( num == VK_F12              )  strcpy(text, "F12");
		if ( num == VK_F13              )  strcpy(text, "F13");
		if ( num == VK_F14              )  strcpy(text, "F14");
		if ( num == VK_F15              )  strcpy(text, "F15");
		if ( num == VK_F16              )  strcpy(text, "F16");
		if ( num == VK_F17              )  strcpy(text, "F17");
		if ( num == VK_F18              )  strcpy(text, "F18");
		if ( num == VK_F19              )  strcpy(text, "F19");
		if ( num == VK_F20              )  strcpy(text, "F20");
		if ( num == VK_NUMLOCK          )  strcpy(text, "Num Lock");
		if ( num == VK_SCROLL           )  strcpy(text, "Scroll");
		if ( num == VK_ATTN             )  strcpy(text, "Attn");
		if ( num == VK_CRSEL            )  strcpy(text, "CrSel");
		if ( num == VK_EXSEL            )  strcpy(text, "ExSel");
		if ( num == VK_EREOF            )  strcpy(text, "Erase EOF");
		if ( num == VK_PLAY             )  strcpy(text, "Play");
		if ( num == VK_ZOOM             )  strcpy(text, "Zoom");
		if ( num == VK_PA1              )  strcpy(text, "PA1");
		if ( num == VK_OEM_CLEAR        )  strcpy(text, "Clear");
		if ( num == VK_BUTTON1          )  strcpy(text, "Button 1");
		if ( num == VK_BUTTON2          )  strcpy(text, "Button 2");
		if ( num == VK_BUTTON3          )  strcpy(text, "Button 3");
		if ( num == VK_BUTTON4          )  strcpy(text, "Button 4");
		if ( num == VK_BUTTON5          )  strcpy(text, "Button 5");
		if ( num == VK_BUTTON6          )  strcpy(text, "Button 6");
		if ( num == VK_BUTTON7          )  strcpy(text, "Button 7");
		if ( num == VK_BUTTON8          )  strcpy(text, "Button 8");
		if ( num == VK_BUTTON9          )  strcpy(text, "Button 9");
		if ( num == VK_BUTTON10         )  strcpy(text, "Button 10");
		if ( num == VK_BUTTON11         )  strcpy(text, "Button 11");
		if ( num == VK_BUTTON12         )  strcpy(text, "Button 12");
		if ( num == VK_BUTTON13         )  strcpy(text, "Button 13");
		if ( num == VK_BUTTON14         )  strcpy(text, "Button 14");
		if ( num == VK_BUTTON15         )  strcpy(text, "Button 15");
		if ( num == VK_BUTTON16         )  strcpy(text, "Button 16");
		if ( num == VK_BUTTON17         )  strcpy(text, "Button 17");
		if ( num == VK_BUTTON18         )  strcpy(text, "Button 18");
		if ( num == VK_BUTTON19         )  strcpy(text, "Button 19");
		if ( num == VK_BUTTON20         )  strcpy(text, "Button 20");
		if ( num == VK_BUTTON21         )  strcpy(text, "Button 21");
		if ( num == VK_BUTTON22         )  strcpy(text, "Button 22");
		if ( num == VK_BUTTON23         )  strcpy(text, "Button 23");
		if ( num == VK_BUTTON24         )  strcpy(text, "Button 24");
		if ( num == VK_BUTTON25         )  strcpy(text, "Button 25");
		if ( num == VK_BUTTON26         )  strcpy(text, "Button 26");
		if ( num == VK_BUTTON27         )  strcpy(text, "Button 27");
		if ( num == VK_BUTTON28         )  strcpy(text, "Button 28");
		if ( num == VK_BUTTON29         )  strcpy(text, "Button 29");
		if ( num == VK_BUTTON30         )  strcpy(text, "Button 30");
		if ( num == VK_BUTTON31         )  strcpy(text, "Button 31");
		if ( num == VK_BUTTON32         )  strcpy(text, "Button 32");
		if ( num == VK_WHEELUP          )  strcpy(text, "Wheel up");
		if ( num == VK_WHEELDOWN        )  strcpy(text, "Wheel down");
	}
#endif

#if _FRENCH
	if ( type == RES_TEXT )
	{
		#if _FULL
		if ( num == RT_VERSION_ID          )  strcpy(text, "1.18 /f");
		#endif
		#if _NET
		if ( num == RT_VERSION_ID          )  strcpy(text, "CeeBot-A 1.18");
		#endif
		#if _SCHOOL & _EDU
		#if _TEEN
		if ( num == RT_VERSION_ID          )  strcpy(text, "CeeBot-Teen EDU 1.18");
		#else
		if ( num == RT_VERSION_ID          )  strcpy(text, "CeeBot-A EDU 1.18");
		#endif
		#endif
		#if _SCHOOL & _PERSO
		#if _TEEN
		if ( num == RT_VERSION_ID          )  strcpy(text, "CeeBot-Teen PERSO 1.18");
		#else
		if ( num == RT_VERSION_ID          )  strcpy(text, "CeeBot-A PERSO 1.18");
		#endif
		#endif
		#if _SCHOOL & _CEEBOTDEMO
		#if _TEEN
		if ( num == RT_VERSION_ID          )  strcpy(text, "CeeBot-Teen DEMO 1.18");
		#else
		if ( num == RT_VERSION_ID          )  strcpy(text, "CeeBot-A DEMO 1.18");
		#endif
		#endif
		#if _DEMO
		if ( num == RT_VERSION_ID          )  strcpy(text, "Demo 1.18 /f");
		#endif
		if ( num == RT_DISINFO_TITLE       )  strcpy(text, "SatCom");
		if ( num == RT_WINDOW_MAXIMIZED    )  strcpy(text, "Taille maximale");
		if ( num == RT_WINDOW_MINIMIZED    )  strcpy(text, "Taille réduite");
		if ( num == RT_WINDOW_STANDARD     )  strcpy(text, "Taille normale");
		if ( num == RT_WINDOW_CLOSE        )  strcpy(text, "Fermer");

		if ( num == RT_STUDIO_TITLE        )  strcpy(text, "Edition du programme");
		if ( num == RT_SCRIPT_NEW          )  strcpy(text, "Nouveau");
		if ( num == RT_NAME_DEFAULT        )  strcpy(text, "Joueur");
		if ( num == RT_IO_NEW              )  strcpy(text, "Nouveau ...");
		if ( num == RT_KEY_OR              )  strcpy(text, " ou ");

#if _NEWLOOK
		if ( num == RT_TITLE_BASE          )  strcpy(text, "CeeBot");
		if ( num == RT_TITLE_INIT          )  strcpy(text, "CeeBot");
#else
		if ( num == RT_TITLE_BASE          )  strcpy(text, "COLOBOT");
		if ( num == RT_TITLE_INIT          )  strcpy(text, "COLOBOT");
#endif
		if ( num == RT_TITLE_TRAINER       )  strcpy(text, "Programmation");
		if ( num == RT_TITLE_DEFI          )  strcpy(text, "Défis");
		if ( num == RT_TITLE_MISSION       )  strcpy(text, "Missions");
		if ( num == RT_TITLE_FREE          )  strcpy(text, "Jeu libre");
		if ( num == RT_TITLE_TEEN          )  strcpy(text, "Jeu libre");
		if ( num == RT_TITLE_USER          )  strcpy(text, "Niveaux supplémentaires");
		if ( num == RT_TITLE_PROTO         )  strcpy(text, "Prototypes");
		if ( num == RT_TITLE_SETUP         )  strcpy(text, "Options");
		if ( num == RT_TITLE_NAME          )  strcpy(text, "Nom du joueur");
		if ( num == RT_TITLE_PERSO         )  strcpy(text, "Personnalisation de votre apparence");
		if ( num == RT_TITLE_WRITE         )  strcpy(text, "Enregistrement de la mission en cours");
		if ( num == RT_TITLE_READ          )  strcpy(text, "Chargement d'une mission enregistrée");

		if ( num == RT_PLAY_CHAPt          )  strcpy(text, " Liste des chapitres :");
		if ( num == RT_PLAY_CHAPd          )  strcpy(text, " Liste des chapitres :");
		if ( num == RT_PLAY_CHAPm          )  strcpy(text, " Liste des plančtes :");
		if ( num == RT_PLAY_CHAPf          )  strcpy(text, " Liste des plančtes :");
		if ( num == RT_PLAY_CHAPu          )  strcpy(text, " Niveaux supplémentaires :");
		if ( num == RT_PLAY_CHAPp          )  strcpy(text, " Liste des plančtes :");
		if ( num == RT_PLAY_CHAPte         )  strcpy(text, " Liste des chapitres :");
		if ( num == RT_PLAY_LISTt          )  strcpy(text, " Liste des exercices du chapitre :");
		if ( num == RT_PLAY_LISTd          )  strcpy(text, " Liste des défis du chapitre :");
		if ( num == RT_PLAY_LISTm          )  strcpy(text, " Liste des missions du chapitre :");
		if ( num == RT_PLAY_LISTf          )  strcpy(text, " Liste des jeux libres du chapitre :");
		if ( num == RT_PLAY_LISTu          )  strcpy(text, " Missions du niveau :");
		if ( num == RT_PLAY_LISTp          )  strcpy(text, " Liste des prototypes du chapitre :");
		if ( num == RT_PLAY_LISTk          )  strcpy(text, " Liste des jeux libres du chapitre :");
		if ( num == RT_PLAY_RESUME         )  strcpy(text, " Résumé :");

		if ( num == RT_SETUP_DEVICE        )  strcpy(text, " Pilotes :");
		if ( num == RT_SETUP_MODE          )  strcpy(text, " Résolutions :");
		if ( num == RT_SETUP_KEY1          )  strcpy(text, "1) Cliquez d'abord sur la touche ŕ redéfinir.");
		if ( num == RT_SETUP_KEY2          )  strcpy(text, "2) Appuyez ensuite sur la nouvelle touche souhaitée.");

		if ( num == RT_PERSO_FACE          )  strcpy(text, "Type de visage :");
		if ( num == RT_PERSO_GLASSES       )  strcpy(text, "Lunettes :");
		if ( num == RT_PERSO_HAIR          )  strcpy(text, "Couleur des cheveux :");
		if ( num == RT_PERSO_COMBI         )  strcpy(text, "Couleur de la combinaison :");
		if ( num == RT_PERSO_BAND          )  strcpy(text, "Couleur des bandes :");

#if _NEWLOOK
		if ( num == RT_DIALOG_TITLE        )  strcpy(text, "CeeBot");
		if ( num == RT_DIALOG_QUIT         )  strcpy(text, "Voulez-vous quitter CeeBot ?");
		if ( num == RT_DIALOG_YESQUIT      )  strcpy(text, "Quitter\\Quitter CeeBot");
#else
		if ( num == RT_DIALOG_TITLE        )  strcpy(text, "COLOBOT");
		if ( num == RT_DIALOG_QUIT         )  strcpy(text, "Voulez-vous quitter COLOBOT ?");
		if ( num == RT_DIALOG_YESQUIT      )  strcpy(text, "Quitter\\Quitter COLOBOT");
#endif
		if ( num == RT_DIALOG_ABORT        )  strcpy(text, "Quitter la mission ?");
		if ( num == RT_DIALOG_YES          )  strcpy(text, "Abandonner\\Abandonner la mission en cours");
		if ( num == RT_DIALOG_NO           )  strcpy(text, "Continuer\\Continuer la mission en cours");
		if ( num == RT_DIALOG_NOQUIT       )  strcpy(text, "Continuer\\Continuer de jouer");
		if ( num == RT_DIALOG_DELOBJ       )  strcpy(text, "Voulez-vous vraiment détruire le bâtiment sélectionné ?");
		if ( num == RT_DIALOG_DELGAME      )  strcpy(text, "Voulez-vous détruire les sauvegardes de %s ?");
		if ( num == RT_DIALOG_YESDEL       )  strcpy(text, "Détruire");
		if ( num == RT_DIALOG_NODEL        )  strcpy(text, "Annuler");
		if ( num == RT_DIALOG_LOADING      )  strcpy(text, "CHARGEMENT");

		if ( num == RT_STUDIO_LISTTT       )  strcpy(text, "Aide sur le mot-clé (\\key cbot;)");
		if ( num == RT_STUDIO_COMPOK       )  strcpy(text, "Compilation ok (0 erreur)");
		if ( num == RT_STUDIO_PROGSTOP     )  strcpy(text, "Programme terminé");

		if ( num == RT_SATCOM_LIST         )  strcpy(text, "\\b;Listes des objets\n");
		if ( num == RT_SATCOM_BOT          )  strcpy(text, "\\b;Listes des robots\n");
		if ( num == RT_SATCOM_BUILDING     )  strcpy(text, "\\b;Listes des bâtiments\n");
		if ( num == RT_SATCOM_FRET         )  strcpy(text, "\\b;Listes des objets transportables\n");
		if ( num == RT_SATCOM_ALIEN        )  strcpy(text, "\\b;Listes des ennemis\n");
		if ( num == RT_SATCOM_NULL         )  strcpy(text, "\\c; (aucun)\\n;\n");
		if ( num == RT_SATCOM_ERROR1       )  strcpy(text, "\\b;Erreur\n");
		if ( num == RT_SATCOM_ERROR2       )  strcpy(text, "Liste non disponible sans \\l;radar\\u object\\radar; !\n");

		if ( num == RT_IO_OPEN             )  strcpy(text, "Ouvrir");
		if ( num == RT_IO_SAVE             )  strcpy(text, "Enregistrer");
		if ( num == RT_IO_LIST             )  strcpy(text, "Dossier: %s");
		if ( num == RT_IO_NAME             )  strcpy(text, "Nom:");
		if ( num == RT_IO_DIR              )  strcpy(text, "Dans:");
		if ( num == RT_IO_PRIVATE          )  strcpy(text, "Privé\\Dossier privé");
		if ( num == RT_IO_PUBLIC           )  strcpy(text, "Public\\Dossier commun ŕ tous les joueurs");

		if ( num == RT_GENERIC_DEV1        )  strcpy(text, "Développé par :");
		if ( num == RT_GENERIC_DEV2        )  strcpy(text, "www.epsitec.com");
#if _SCHOOL
		if ( num == RT_GENERIC_EDIT1       )  strcpy(text, " ");
		if ( num == RT_GENERIC_EDIT2       )  strcpy(text, " ");
#else
		//?if ( num == RT_GENERIC_EDIT1       )  strcpy(text, "Version française éditée par :");
		//?if ( num == RT_GENERIC_EDIT2       )  strcpy(text, "www.alsyd.com");
		if ( num == RT_GENERIC_EDIT1       )  strcpy(text, " ");
		if ( num == RT_GENERIC_EDIT2       )  strcpy(text, " ");
#endif

		if ( num == RT_INTERFACE_REC       )  strcpy(text, "Enregistreur");
	}

	if ( type == RES_EVENT )
	{
		if ( num == EVENT_BUTTON_OK        )  strcpy(text, "D'accord");
		if ( num == EVENT_BUTTON_CANCEL    )  strcpy(text, "Annuler");
		if ( num == EVENT_BUTTON_NEXT      )  strcpy(text, "Suivant");
		if ( num == EVENT_BUTTON_PREV      )  strcpy(text, "Précédent");
		if ( num == EVENT_BUTTON_QUIT      )  strcpy(text, "Menu (\\key quit;)");

		if ( num == EVENT_DIALOG_OK        )  strcpy(text, "D'accord");
		if ( num == EVENT_DIALOG_CANCEL    )  strcpy(text, "Annuler");

		if ( num == EVENT_INTERFACE_TRAINER)  strcpy(text, "Programmation\\Exercices de programmation");
		if ( num == EVENT_INTERFACE_DEFI   )  strcpy(text, "Défis\\Défis de programmation");
		if ( num == EVENT_INTERFACE_MISSION)  strcpy(text, "Missions\\La grande aventure");
		if ( num == EVENT_INTERFACE_FREE   )  strcpy(text, "Jeu libre\\Jeu libre sans but précis");
		if ( num == EVENT_INTERFACE_TEEN   )  strcpy(text, "Jeu libre\\Jeu libre sans but précis");
		if ( num == EVENT_INTERFACE_USER   )  strcpy(text, "Suppl.\\Niveaux supplémentaires");
		if ( num == EVENT_INTERFACE_PROTO  )  strcpy(text, "Proto\\Prototypes en cours d'élaboration");
		if ( num == EVENT_INTERFACE_NAME   )  strcpy(text, "Autre joueur\\Choix du nom du joueur");
		if ( num == EVENT_INTERFACE_SETUP  )  strcpy(text, "Options\\Réglages");
		if ( num == EVENT_INTERFACE_AGAIN  )  strcpy(text, "Recommencer\\Recommencer la mission au début");
		if ( num == EVENT_INTERFACE_WRITE  )  strcpy(text, "Enregistrer\\Enregistrer la mission en cours");
		if ( num == EVENT_INTERFACE_READ   )  strcpy(text, "Charger\\Charger une mission enregistrée");
#if _NEWLOOK
		if ( num == EVENT_INTERFACE_ABORT  )  strcpy(text, "\\Retourner dans CeeBot");
		if ( num == EVENT_INTERFACE_QUIT   )  strcpy(text, "Quitter\\Quitter CeeBot");
#else
		if ( num == EVENT_INTERFACE_ABORT  )  strcpy(text, "\\Retourner dans COLOBOT");
		if ( num == EVENT_INTERFACE_QUIT   )  strcpy(text, "Quitter\\Quitter COLOBOT");
#endif
		if ( num == EVENT_INTERFACE_BACK   )  strcpy(text, "<<  Retour  \\Retour au niveau précédent");
		if ( num == EVENT_INTERFACE_PLAY   )  strcpy(text, "Jouer ...\\Démarrer l'action");
		if ( num == EVENT_INTERFACE_SETUPd )  strcpy(text, "Affichage\\Pilote et résolution d'affichage");
		if ( num == EVENT_INTERFACE_SETUPg )  strcpy(text, "Graphique\\Options graphiques");
		if ( num == EVENT_INTERFACE_SETUPp )  strcpy(text, "Jeu\\Options de jouabilité");
		if ( num == EVENT_INTERFACE_SETUPc )  strcpy(text, "Commandes\\Touches du clavier");
		if ( num == EVENT_INTERFACE_SETUPs )  strcpy(text, "Son\\Volumes bruitages & musiques");
		if ( num == EVENT_INTERFACE_DEVICE )  strcpy(text, "Unité");
		if ( num == EVENT_INTERFACE_RESOL  )  strcpy(text, "Résolution");
		if ( num == EVENT_INTERFACE_FULL   )  strcpy(text, "Plein écran\\Plein écran ou fenętré");
		if ( num == EVENT_INTERFACE_APPLY  )  strcpy(text, "Appliquer les changements\\Active les changements effectués");

		if ( num == EVENT_INTERFACE_TOTO   )  strcpy(text, "Robbie\\Votre assistant");
		if ( num == EVENT_INTERFACE_SHADOW )  strcpy(text, "Ombres\\Ombres projetées au sol");
		if ( num == EVENT_INTERFACE_GROUND )  strcpy(text, "Marques sur le sol\\Marques dessinées sur le sol");
		if ( num == EVENT_INTERFACE_DIRTY  )  strcpy(text, "Salissures\\Salissures des robots et bâtiments");
		if ( num == EVENT_INTERFACE_FOG    )  strcpy(text, "Brouillard\\Nappes de brouillard");
		if ( num == EVENT_INTERFACE_LENS   )  strcpy(text, "Rayons du soleil\\Rayons selon l'orientation");
		if ( num == EVENT_INTERFACE_SKY    )  strcpy(text, "Ciel\\Ciel et nuages");
		if ( num == EVENT_INTERFACE_PLANET )  strcpy(text, "Plančtes et étoiles\\Motifs mobiles dans le ciel");
		if ( num == EVENT_INTERFACE_LIGHT  )  strcpy(text, "Lumičres dynamiques\\Eclairages mobiles");
		if ( num == EVENT_INTERFACE_PARTI  )  strcpy(text, "Quantité de particules\\Explosions, poussičres, reflets, etc.");
		if ( num == EVENT_INTERFACE_CLIP   )  strcpy(text, "Profondeur de champ\\Distance de vue maximale");
		if ( num == EVENT_INTERFACE_DETAIL )  strcpy(text, "Détails des objets\\Qualité des objets en 3D");
		if ( num == EVENT_INTERFACE_TEXTURE)  strcpy(text, "Qualité des textures\\Qualité des images");
		if ( num == EVENT_INTERFACE_GADGET )  strcpy(text, "Nb d'objets décoratifs\\Qualité d'objets non indispensables");
		if ( num == EVENT_INTERFACE_RAIN   )  strcpy(text, "Particules dans l'interface\\Pluie de particules");
		if ( num == EVENT_INTERFACE_GLINT  )  strcpy(text, "Reflets sur les boutons\\Boutons brillants");
		if ( num == EVENT_INTERFACE_TOOLTIP)  strcpy(text, "Bulles d'aide\\Bulles explicatives");
		if ( num == EVENT_INTERFACE_MOVIES )  strcpy(text, "Séquences cinématiques\\Films avant ou aprčs une mission");
		if ( num == EVENT_INTERFACE_NICERST)  strcpy(text, "Retour animé\\Retour animé dans les exercices");
		if ( num == EVENT_INTERFACE_HIMSELF)  strcpy(text, "Dégâts ŕ soi-męme\\Vos tirs infligent des dommages ŕ vos unités");
		if ( num == EVENT_INTERFACE_SCROLL )  strcpy(text, "Défilement dans les bords\\Défilement lorsque la souris touches les bords gauche ou droite");
		if ( num == EVENT_INTERFACE_INVERTX)  strcpy(text, "Inversion souris X\\Inversion de la rotation lorsque la souris touche un bord");
		if ( num == EVENT_INTERFACE_INVERTY)  strcpy(text, "Inversion souris Y\\Inversion de la rotation lorsque la souris touche un bord");
		if ( num == EVENT_INTERFACE_EFFECT )  strcpy(text, "Secousses lors d'explosions\\L'écran vibre lors d'une explosion");
		if ( num == EVENT_INTERFACE_MOUSE  )  strcpy(text, "Souris ombrée\\Jolie souris avec une ombre");
		if ( num == EVENT_INTERFACE_EDITMODE) strcpy(text, "Indentation automatique\\Pendant l'édition d'un programme");
		if ( num == EVENT_INTERFACE_EDITVALUE)strcpy(text, "Grande indentation\\Indente avec 2 ou 4 espaces");
		if ( num == EVENT_INTERFACE_SOLUCE4)  strcpy(text, "Accčs aux solutions\\Programme \"4: Solution\" dans les exercices");

		if ( num == EVENT_INTERFACE_KDEF   )  strcpy(text, "Tout réinitialiser\\Remet toutes les touches standards");
		if ( num == EVENT_INTERFACE_KLEFT  )  strcpy(text, "Tourner ŕ gauche\\Moteur ŕ gauche");
		if ( num == EVENT_INTERFACE_KRIGHT )  strcpy(text, "Tourner ŕ droite\\Moteur ŕ droite");
		if ( num == EVENT_INTERFACE_KUP    )  strcpy(text, "Avancer\\Moteur en avant");
		if ( num == EVENT_INTERFACE_KDOWN  )  strcpy(text, "Reculer\\Moteur en arričre");
		if ( num == EVENT_INTERFACE_KGUP   )  strcpy(text, "Monter\\Augmenter la puissance du réacteur");
		if ( num == EVENT_INTERFACE_KGDOWN )  strcpy(text, "Descendre\\Diminuer la puissance du réacteur");
		if ( num == EVENT_INTERFACE_KCAMERA)  strcpy(text, "Changement de caméra\\Autre de point de vue");
		if ( num == EVENT_INTERFACE_KDESEL )  strcpy(text, "Sélection précédente\\Sélectionne l'objet précédent");
		if ( num == EVENT_INTERFACE_KACTION)  strcpy(text, "Action standard\\Action du bouton avec le cadre rouge");
		if ( num == EVENT_INTERFACE_KNEAR  )  strcpy(text, "Caméra plus proche\\Avance la caméra");
		if ( num == EVENT_INTERFACE_KAWAY  )  strcpy(text, "Caméra plus loin\\Recule la caméra");
		if ( num == EVENT_INTERFACE_KNEXT  )  strcpy(text, "Sélectionner l'objet suivant\\Sélectionner l'objet suivant");
		if ( num == EVENT_INTERFACE_KHUMAN )  strcpy(text, "Sélectionner le cosmonaute\\Sélectionner le cosmonaute");
		if ( num == EVENT_INTERFACE_KQUIT  )  strcpy(text, "Quitter la mission en cours\\Terminer un exercice ou une mssion");
		if ( num == EVENT_INTERFACE_KHELP  )  strcpy(text, "Instructions mission\\Marche ŕ suivre");
		if ( num == EVENT_INTERFACE_KPROG  )  strcpy(text, "Instructions programmation\\Explication sur la programmation");
		if ( num == EVENT_INTERFACE_KCBOT  )  strcpy(text, "Instructions mot-clé\\Explication sur le mot-clé");
		if ( num == EVENT_INTERFACE_KVISIT )  strcpy(text, "Montrer le lieu d'un message\\Montrer le lieu du dernier message");
		if ( num == EVENT_INTERFACE_KSPEED10) strcpy(text, "Vitesse 1.0x\\Vitesse normale");
		if ( num == EVENT_INTERFACE_KSPEED15) strcpy(text, "Vitesse 1.5x\\Une fois et demi plus rapide");
		if ( num == EVENT_INTERFACE_KSPEED20) strcpy(text, "Vitesse 2.0x\\Deux fois plus rapide");
		if ( num == EVENT_INTERFACE_KSPEED30) strcpy(text, "Vitesse 3.0x\\Trois fois plus rapide");

		if ( num == EVENT_INTERFACE_VOLSOUND) strcpy(text, "Bruitages :\\Volume des moteurs, voix, etc.");
		if ( num == EVENT_INTERFACE_VOLMUSIC) strcpy(text, "Fond sonore :\\Volume des pistes audio du CD");
		if ( num == EVENT_INTERFACE_SOUND3D)  strcpy(text, "Bruitages 3D\\Positionnement sonore dans l'espace");

		if ( num == EVENT_INTERFACE_MIN    )  strcpy(text, "Mini\\Qualité minimale (+ rapide)");
		if ( num == EVENT_INTERFACE_NORM   )  strcpy(text, "Normal\\Qualité standard");
		if ( num == EVENT_INTERFACE_MAX    )  strcpy(text, "Maxi\\Haute qualité (+ lent)");

		if ( num == EVENT_INTERFACE_SILENT )  strcpy(text, "Silencieux\\Totalement silencieux");
		if ( num == EVENT_INTERFACE_NOISY  )  strcpy(text, "Normal\\Niveaux normaux");

		if ( num == EVENT_INTERFACE_JOYSTICK) strcpy(text, "Utilise un joystick\\Joystick ou clavier");
		if ( num == EVENT_INTERFACE_SOLUCE )  strcpy(text, "Accčs ŕ la solution\\Donne la solution");

		if ( num == EVENT_INTERFACE_NEDIT  )  strcpy(text, "\\Nom du joueur ŕ créer");
		if ( num == EVENT_INTERFACE_NOK    )  strcpy(text, "D'accord\\Choisir le joueur");
		if ( num == EVENT_INTERFACE_NCANCEL)  strcpy(text, "Annuler\\Conserver le joueur actuel");
		if ( num == EVENT_INTERFACE_NDELETE)  strcpy(text, "Supprimer le joueur\\Supprimer le joueur de la liste");
		if ( num == EVENT_INTERFACE_NLABEL )  strcpy(text, "Nom du joueur");

		if ( num == EVENT_INTERFACE_IOWRITE)  strcpy(text, "Enregistrer\\Enregistrer la mission en cours");
		if ( num == EVENT_INTERFACE_IOREAD )  strcpy(text, "Charger\\Charger la mission sélectionnée");
		if ( num == EVENT_INTERFACE_IOLIST )  strcpy(text, "Liste des missions enregistrées");
		if ( num == EVENT_INTERFACE_IOLABEL)  strcpy(text, "Nom du fichier :");
		if ( num == EVENT_INTERFACE_IONAME )  strcpy(text, "Nom de la mission");
		if ( num == EVENT_INTERFACE_IOIMAGE)  strcpy(text, "Vue de la mission");
		if ( num == EVENT_INTERFACE_IODELETE) strcpy(text, "Supprimer\\Supprime l'enregistrement sélectionné");

		if ( num == EVENT_INTERFACE_PERSO  )  strcpy(text, "Aspect\\Choisir votre aspect");
		if ( num == EVENT_INTERFACE_POK    )  strcpy(text, "D'accord");
		if ( num == EVENT_INTERFACE_PCANCEL)  strcpy(text, "Annuler");
		if ( num == EVENT_INTERFACE_PDEF   )  strcpy(text, "Standard\\Remet les couleurs standards");
		if ( num == EVENT_INTERFACE_PHEAD  )  strcpy(text, "Tęte\\Visage et cheveux");
		if ( num == EVENT_INTERFACE_PBODY  )  strcpy(text, "Corps\\Combinaison");
		if ( num == EVENT_INTERFACE_PLROT  )  strcpy(text, "\\Rotation ŕ gauche");
		if ( num == EVENT_INTERFACE_PRROT  )  strcpy(text, "\\Rotation ŕ droite");
		if ( num == EVENT_INTERFACE_PCRa   )  strcpy(text, "Rouge");
		if ( num == EVENT_INTERFACE_PCGa   )  strcpy(text, "Vert");
		if ( num == EVENT_INTERFACE_PCBa   )  strcpy(text, "Bleu");
		if ( num == EVENT_INTERFACE_PCRb   )  strcpy(text, "Rouge");
		if ( num == EVENT_INTERFACE_PCGb   )  strcpy(text, "Vert");
		if ( num == EVENT_INTERFACE_PCBb   )  strcpy(text, "Bleu");
		if ( num == EVENT_INTERFACE_PFACE1 )  strcpy(text, "\\Visage 1");
		if ( num == EVENT_INTERFACE_PFACE2 )  strcpy(text, "\\Visage 4");
		if ( num == EVENT_INTERFACE_PFACE3 )  strcpy(text, "\\Visage 3");
		if ( num == EVENT_INTERFACE_PFACE4 )  strcpy(text, "\\Visage 2");
		if ( num == EVENT_INTERFACE_PGLASS0)  strcpy(text, "\\Pas de lunettes");
		if ( num == EVENT_INTERFACE_PGLASS1)  strcpy(text, "\\Lunettes 1");
		if ( num == EVENT_INTERFACE_PGLASS2)  strcpy(text, "\\Lunettes 2");
		if ( num == EVENT_INTERFACE_PGLASS3)  strcpy(text, "\\Lunettes 3");
		if ( num == EVENT_INTERFACE_PGLASS4)  strcpy(text, "\\Lunettes 4");
		if ( num == EVENT_INTERFACE_PGLASS5)  strcpy(text, "\\Lunettes 5");

		if ( num == EVENT_OBJECT_DESELECT  )  strcpy(text, "Sélection précédente (\\key desel;)");
		if ( num == EVENT_OBJECT_LEFT      )  strcpy(text, "Tourne ŕ gauche (\\key left;)");
		if ( num == EVENT_OBJECT_RIGHT     )  strcpy(text, "Tourne ŕ droite (\\key right;)");
		if ( num == EVENT_OBJECT_UP        )  strcpy(text, "Avance (\\key up;)");
		if ( num == EVENT_OBJECT_DOWN      )  strcpy(text, "Recule (\\key down;)");
		if ( num == EVENT_OBJECT_GASUP     )  strcpy(text, "Monte (\\key gup;)");
		if ( num == EVENT_OBJECT_GASDOWN   )  strcpy(text, "Descend (\\key gdown;)");
		if ( num == EVENT_OBJECT_HTAKE     )  strcpy(text, "Prend ou dépose  (\\key action;)");
		if ( num == EVENT_OBJECT_MTAKE     )  strcpy(text, "Prend ou dépose  (\\key action;)");
		if ( num == EVENT_OBJECT_MFRONT    )  strcpy(text, "..devant");
		if ( num == EVENT_OBJECT_MBACK     )  strcpy(text, "..derričre");
		if ( num == EVENT_OBJECT_MPOWER    )  strcpy(text, "..pile");
		if ( num == EVENT_OBJECT_BHELP     )  strcpy(text, "Instructions sur la mission (\\key help;)");
		if ( num == EVENT_OBJECT_BTAKEOFF  )  strcpy(text, "Décolle pour terminer la mission");
		if ( num == EVENT_OBJECT_BDERRICK  )  strcpy(text, "Construit un derrick");
		if ( num == EVENT_OBJECT_BSTATION  )  strcpy(text, "Construit une station");
		if ( num == EVENT_OBJECT_BFACTORY  )  strcpy(text, "Construit une fabrique de robots");
		if ( num == EVENT_OBJECT_BREPAIR   )  strcpy(text, "Construit un centre de réparation");
		if ( num == EVENT_OBJECT_BCONVERT  )  strcpy(text, "Construit un convertisseur");
		if ( num == EVENT_OBJECT_BTOWER    )  strcpy(text, "Construit une tour");
		if ( num == EVENT_OBJECT_BRESEARCH )  strcpy(text, "Construit un centre de recherches");
		if ( num == EVENT_OBJECT_BRADAR    )  strcpy(text, "Construit un radar");
		if ( num == EVENT_OBJECT_BENERGY   )  strcpy(text, "Construit une fabrique de piles");
		if ( num == EVENT_OBJECT_BLABO     )  strcpy(text, "Construit un laboratoire");
		if ( num == EVENT_OBJECT_BNUCLEAR  )  strcpy(text, "Construit une centrale nucléaire");
		if ( num == EVENT_OBJECT_BPARA     )  strcpy(text, "Construit un paratonnerre");
		if ( num == EVENT_OBJECT_BINFO     )  strcpy(text, "Construit une borne d'information");
		if ( num == EVENT_OBJECT_GFLAT     )  strcpy(text, "Montre si le sol est plat");
		if ( num == EVENT_OBJECT_FCREATE   )  strcpy(text, "Pose un drapeau de couleur");
		if ( num == EVENT_OBJECT_FDELETE   )  strcpy(text, "Enlčve un drapeau");
		if ( num == EVENT_OBJECT_FCOLORb   )  strcpy(text, "\\Drapeaux bleus");
		if ( num == EVENT_OBJECT_FCOLORr   )  strcpy(text, "\\Drapeaux rouges");
		if ( num == EVENT_OBJECT_FCOLORg   )  strcpy(text, "\\Drapeaux verts");
		if ( num == EVENT_OBJECT_FCOLORy   )  strcpy(text, "\\Drapeaux jaunes");
		if ( num == EVENT_OBJECT_FCOLORv   )  strcpy(text, "\\Drapeaux violets");
		if ( num == EVENT_OBJECT_FACTORYfa )  strcpy(text, "Fabrique un déménageur volant");
		if ( num == EVENT_OBJECT_FACTORYta )  strcpy(text, "Fabrique un déménageur ŕ chenilles");
		if ( num == EVENT_OBJECT_FACTORYwa )  strcpy(text, "Fabrique un déménageur ŕ roues");
		if ( num == EVENT_OBJECT_FACTORYia )  strcpy(text, "Fabrique un déménageur ŕ pattes");
		if ( num == EVENT_OBJECT_FACTORYfc )  strcpy(text, "Fabrique un shooter volant");
		if ( num == EVENT_OBJECT_FACTORYtc )  strcpy(text, "Fabrique un shooter ŕ chenilles");
		if ( num == EVENT_OBJECT_FACTORYwc )  strcpy(text, "Fabrique un shooter ŕ roues");
		if ( num == EVENT_OBJECT_FACTORYic )  strcpy(text, "Fabrique un shooter ŕ pattes");
		if ( num == EVENT_OBJECT_FACTORYfi )  strcpy(text, "Fabrique un orgaShooter volant");
		if ( num == EVENT_OBJECT_FACTORYti )  strcpy(text, "Fabrique un orgaShooter ŕ chenilles");
		if ( num == EVENT_OBJECT_FACTORYwi )  strcpy(text, "Fabrique un orgaShooter ŕ roues");
		if ( num == EVENT_OBJECT_FACTORYii )  strcpy(text, "Fabrique un orgaShooter ŕ pattes");
		if ( num == EVENT_OBJECT_FACTORYfs )  strcpy(text, "Fabrique un renifleur volant");
		if ( num == EVENT_OBJECT_FACTORYts )  strcpy(text, "Fabrique un renifleur ŕ chenilles");
		if ( num == EVENT_OBJECT_FACTORYws )  strcpy(text, "Fabrique un renifleur ŕ roues");
		if ( num == EVENT_OBJECT_FACTORYis )  strcpy(text, "Fabrique un renifleur ŕ pattes");
		if ( num == EVENT_OBJECT_FACTORYrt )  strcpy(text, "Fabrique un robot secoueur");
		if ( num == EVENT_OBJECT_FACTORYrc )  strcpy(text, "Fabrique un robot phazer");
		if ( num == EVENT_OBJECT_FACTORYrr )  strcpy(text, "Fabrique un robot recycleur");
		if ( num == EVENT_OBJECT_FACTORYrs )  strcpy(text, "Fabrique un robot bouclier");
		if ( num == EVENT_OBJECT_FACTORYsa )  strcpy(text, "Fabrique un robot sous-marin");
		if ( num == EVENT_OBJECT_RTANK     )  strcpy(text, "Recherche les chenilles");
		if ( num == EVENT_OBJECT_RFLY      )  strcpy(text, "Recherche les robots volants");
		if ( num == EVENT_OBJECT_RTHUMP    )  strcpy(text, "Recherche le secoueur");
		if ( num == EVENT_OBJECT_RCANON    )  strcpy(text, "Recherche le canon shooter");
		if ( num == EVENT_OBJECT_RTOWER    )  strcpy(text, "Recherche la tour de défense");
		if ( num == EVENT_OBJECT_RPHAZER   )  strcpy(text, "Recherche le canon phazer");
		if ( num == EVENT_OBJECT_RSHIELD   )  strcpy(text, "Recherche le bouclier");
		if ( num == EVENT_OBJECT_RATOMIC   )  strcpy(text, "Recherche le nucléaire");
		if ( num == EVENT_OBJECT_RiPAW     )  strcpy(text, "Recherche les pattes");
		if ( num == EVENT_OBJECT_RiGUN     )  strcpy(text, "Recherche le canon orgaShooter");
		if ( num == EVENT_OBJECT_RESET     )  strcpy(text, "Remet au départ");
		if ( num == EVENT_OBJECT_SEARCH    )  strcpy(text, "Cherche (\\key action;)");
		if ( num == EVENT_OBJECT_TERRAFORM )  strcpy(text, "Secoue (\\key action;)");
		if ( num == EVENT_OBJECT_FIRE      )  strcpy(text, "Tir (\\key action;)");
		if ( num == EVENT_OBJECT_RECOVER   )  strcpy(text, "Recycle (\\key action;)");
		if ( num == EVENT_OBJECT_BEGSHIELD )  strcpy(text, "Déploie le bouclier (\\key action;)");
		if ( num == EVENT_OBJECT_ENDSHIELD )  strcpy(text, "Stoppe le bouclier (\\key action;)");
		if ( num == EVENT_OBJECT_DIMSHIELD )  strcpy(text, "Rayon du bouclier");
		if ( num == EVENT_OBJECT_PROGRUN   )  strcpy(text, "Exécute le programme sélectionné");
		if ( num == EVENT_OBJECT_PROGEDIT  )  strcpy(text, "Edite le programme sélectionné");
		if ( num == EVENT_OBJECT_INFOOK    )  strcpy(text, "\\Mettre le SatCom en veille");
		if ( num == EVENT_OBJECT_DELETE    )  strcpy(text, "Démolit le bâtiment");
		if ( num == EVENT_OBJECT_GENERGY   )  strcpy(text, "Niveau d'énergie");
		if ( num == EVENT_OBJECT_GSHIELD   )  strcpy(text, "Niveau du bouclier");
		if ( num == EVENT_OBJECT_GRANGE    )  strcpy(text, "Température du réacteur");
		if ( num == EVENT_OBJECT_GPROGRESS )  strcpy(text, "Travail en cours ...");
		if ( num == EVENT_OBJECT_GRADAR    )  strcpy(text, "Nombre d'insectes détectés");
		if ( num == EVENT_OBJECT_GINFO     )  strcpy(text, "Informations diffusées");
		if ( num == EVENT_OBJECT_COMPASS   )  strcpy(text, "Boussole");
//?		if ( num == EVENT_OBJECT_MAP       )  strcpy(text, "Mini-carte");
		if ( num == EVENT_OBJECT_MAPZOOM   )  strcpy(text, "Zoom mini-carte");
		if ( num == EVENT_OBJECT_CAMERA    )  strcpy(text, "Caméra (\\key camera;)");
		if ( num == EVENT_OBJECT_CAMERAleft)  strcpy(text, "Caméra ŕ gauche");
		if ( num == EVENT_OBJECT_CAMERAright) strcpy(text, "Caméra ŕ droite");
		if ( num == EVENT_OBJECT_CAMERAnear)  strcpy(text, "Caméra plus proche");
		if ( num == EVENT_OBJECT_CAMERAaway)  strcpy(text, "Caméra plus loin");
		if ( num == EVENT_OBJECT_HELP      )  strcpy(text, "Instructions sur la sélection");
		if ( num == EVENT_OBJECT_SOLUCE    )  strcpy(text, "Donne la solution");
		if ( num == EVENT_OBJECT_SHORTCUT00)  strcpy(text, "Permute robots <-> bâtiments");
		if ( num == EVENT_OBJECT_LIMIT     )  strcpy(text, "Montre le rayon d'action");
		if ( num == EVENT_OBJECT_PEN0      )  strcpy(text, "\\Relčve le crayon");
		if ( num == EVENT_OBJECT_PEN1      )  strcpy(text, "\\Abaisse le crayon noir");
		if ( num == EVENT_OBJECT_PEN2      )  strcpy(text, "\\Abaisse le crayon jaune");
		if ( num == EVENT_OBJECT_PEN3      )  strcpy(text, "\\Abaisse le crayon orange");
		if ( num == EVENT_OBJECT_PEN4      )  strcpy(text, "\\Abaisse le crayon rouge");
		if ( num == EVENT_OBJECT_PEN5      )  strcpy(text, "\\Abaisse le crayon violet");
		if ( num == EVENT_OBJECT_PEN6      )  strcpy(text, "\\Abaisse le crayon bleu");
		if ( num == EVENT_OBJECT_PEN7      )  strcpy(text, "\\Abaisse le crayon vert");
		if ( num == EVENT_OBJECT_PEN8      )  strcpy(text, "\\Abaisse le crayon brun");
		if ( num == EVENT_OBJECT_REC       )  strcpy(text, "\\Démarre l'enregistrement");
		if ( num == EVENT_OBJECT_STOP      )  strcpy(text, "\\Stoppe l'enregistrement");
		if ( num == EVENT_DT_VISIT0        ||
			 num == EVENT_DT_VISIT1        ||
			 num == EVENT_DT_VISIT2        ||
			 num == EVENT_DT_VISIT3        ||
			 num == EVENT_DT_VISIT4        )  strcpy(text, "Montre l'endroit");
		if ( num == EVENT_DT_END           )  strcpy(text, "Continuer");
		if ( num == EVENT_CMD              )  strcpy(text, "Console de commande");
		if ( num == EVENT_SPEED            )  strcpy(text, "Vitesse du jeu");

		if ( num == EVENT_HYPER_PREV       )  strcpy(text, "Page précédente");
		if ( num == EVENT_HYPER_NEXT       )  strcpy(text, "Page suivante");
		if ( num == EVENT_HYPER_HOME       )  strcpy(text, "Page initiale");
		if ( num == EVENT_HYPER_COPY       )  strcpy(text, "Copier");
		if ( num == EVENT_HYPER_SIZE1      )  strcpy(text, "Taille 1");
		if ( num == EVENT_HYPER_SIZE2      )  strcpy(text, "Taille 2");
		if ( num == EVENT_HYPER_SIZE3      )  strcpy(text, "Taille 3");
		if ( num == EVENT_HYPER_SIZE4      )  strcpy(text, "Taille 4");
		if ( num == EVENT_HYPER_SIZE5      )  strcpy(text, "Taille 5");
		if ( num == EVENT_SATCOM_HUSTON    )  strcpy(text, "Instructions de Houston");
#if _TEEN
		if ( num == EVENT_SATCOM_SAT       )  strcpy(text, "Dictionnaire anglais-français");
#else
		if ( num == EVENT_SATCOM_SAT       )  strcpy(text, "Rapport du satellite");
#endif
		if ( num == EVENT_SATCOM_LOADING   )  strcpy(text, "Programmes envoyés par Houston");
		if ( num == EVENT_SATCOM_OBJECT    )  strcpy(text, "Liste des objets");
		if ( num == EVENT_SATCOM_PROG      )  strcpy(text, "Aide ŕ la programmation");
		if ( num == EVENT_SATCOM_SOLUCE    )  strcpy(text, "Solution");

		if ( num == EVENT_STUDIO_OK        )  strcpy(text, "D'accord\\Compiler le programme");
		if ( num == EVENT_STUDIO_CANCEL    )  strcpy(text, "Annuler\\Annuler toutes les modifications");
		if ( num == EVENT_STUDIO_NEW       )  strcpy(text, "Nouveau");
		if ( num == EVENT_STUDIO_OPEN      )  strcpy(text, "Ouvrir (Ctrl+o)");
		if ( num == EVENT_STUDIO_SAVE      )  strcpy(text, "Enregistrer (Ctrl+s)");
		if ( num == EVENT_STUDIO_UNDO      )  strcpy(text, "Annuler (Ctrl+z)");
		if ( num == EVENT_STUDIO_CUT       )  strcpy(text, "Couper (Ctrl+x)");
		if ( num == EVENT_STUDIO_COPY      )  strcpy(text, "Copier (Ctrl+c)");
		if ( num == EVENT_STUDIO_PASTE     )  strcpy(text, "Coller (Ctrl+v)");
		if ( num == EVENT_STUDIO_SIZE      )  strcpy(text, "Taille des caractčres");
		if ( num == EVENT_STUDIO_TOOL      )  strcpy(text, "Instructions (\\key help;)");
		if ( num == EVENT_STUDIO_HELP      )  strcpy(text, "Aide ŕ la programmation (\\key prog;)");
		if ( num == EVENT_STUDIO_COMPILE   )  strcpy(text, "Compiler");
		if ( num == EVENT_STUDIO_RUN       )  strcpy(text, "Démarrer/stopper");
		if ( num == EVENT_STUDIO_REALTIME  )  strcpy(text, "Pause/continuer");
		if ( num == EVENT_STUDIO_STEP      )  strcpy(text, "Un pas");
	}

	if ( type == RES_OBJECT )
	{
		if ( num == OBJECT_PORTICO      )  strcpy(text, "Portique");
		if ( num == OBJECT_BASE         )  strcpy(text, "Vaisseau spatial");
		if ( num == OBJECT_DERRICK      )  strcpy(text, "Derrick");
		if ( num == OBJECT_FACTORY      )  strcpy(text, "Fabrique de robots");
		if ( num == OBJECT_REPAIR       )  strcpy(text, "Centre de réparation");
		if ( num == OBJECT_DESTROYER    )  strcpy(text, "Destructeur");
		if ( num == OBJECT_STATION      )  strcpy(text, "Station de recharge");
		if ( num == OBJECT_CONVERT      )  strcpy(text, "Conversion minerai en titanium");
		if ( num == OBJECT_TOWER        )  strcpy(text, "Tour de défense");
		if ( num == OBJECT_NEST         )  strcpy(text, "Nid");
		if ( num == OBJECT_RESEARCH     )  strcpy(text, "Centre de recherches");
		if ( num == OBJECT_RADAR        )  strcpy(text, "Radar");
		if ( num == OBJECT_INFO         )  strcpy(text, "Borne d'information");
#if _TEEN
		if ( num == OBJECT_ENERGY       )  strcpy(text, "Désintégrateur");
#else
		if ( num == OBJECT_ENERGY       )  strcpy(text, "Fabrique de piles");
#endif
		if ( num == OBJECT_LABO         )  strcpy(text, "Laboratoire de matičres organiques");
		if ( num == OBJECT_NUCLEAR      )  strcpy(text, "Centrale nucléaire");
		if ( num == OBJECT_PARA         )  strcpy(text, "Paratonnerre");
		if ( num == OBJECT_SAFE         )  strcpy(text, "Coffre-fort");
		if ( num == OBJECT_HUSTON       )  strcpy(text, "Centre de contrôle");
		if ( num == OBJECT_TARGET1      )  strcpy(text, "Cible");
		if ( num == OBJECT_TARGET2      )  strcpy(text, "Cible");
		if ( num == OBJECT_START        )  strcpy(text, "Départ");
		if ( num == OBJECT_END          )  strcpy(text, "But");
		if ( num == OBJECT_STONE        )  strcpy(text, "Minerai de titanium");
		if ( num == OBJECT_URANIUM      )  strcpy(text, "Minerai d'uranium");
		if ( num == OBJECT_BULLET       )  strcpy(text, "Matičre organique");
		if ( num == OBJECT_METAL        )  strcpy(text, "Titanium");
		if ( num == OBJECT_POWER        )  strcpy(text, "Pile normale");
		if ( num == OBJECT_ATOMIC       )  strcpy(text, "Pile nucléaire");
		if ( num == OBJECT_BBOX         )  strcpy(text, "Boîte noire");
		if ( num == OBJECT_KEYa         )  strcpy(text, "Clé A");
		if ( num == OBJECT_KEYb         )  strcpy(text, "Clé B");
		if ( num == OBJECT_KEYc         )  strcpy(text, "Clé C");
		if ( num == OBJECT_KEYd         )  strcpy(text, "Clé D");
		if ( num == OBJECT_TNT          )  strcpy(text, "Explosif");
		if ( num == OBJECT_BOMB         )  strcpy(text, "Mine fixe");
		if ( num == OBJECT_BAG          )  strcpy(text, "Sac de survie");
		if ( num == OBJECT_WAYPOINT     )  strcpy(text, "Indicateur");
		if ( num == OBJECT_FLAGb        )  strcpy(text, "Drapeau bleu");
		if ( num == OBJECT_FLAGr        )  strcpy(text, "Drapeau rouge");
		if ( num == OBJECT_FLAGg        )  strcpy(text, "Drapeau vert");
		if ( num == OBJECT_FLAGy        )  strcpy(text, "Drapeau jaune");
		if ( num == OBJECT_FLAGv        )  strcpy(text, "Drapeau violet");
		if ( num == OBJECT_MARKPOWER    )  strcpy(text, "Emplacement pour station");
		if ( num == OBJECT_MARKURANIUM  )  strcpy(text, "Emplacement pour derrick (uranium)");
		if ( num == OBJECT_MARKKEYa     )  strcpy(text, "Emplacement pour derrick (clé A)");
		if ( num == OBJECT_MARKKEYb     )  strcpy(text, "Emplacement pour derrick (clé B)");
		if ( num == OBJECT_MARKKEYc     )  strcpy(text, "Emplacement pour derrick (clé C)");
		if ( num == OBJECT_MARKKEYd     )  strcpy(text, "Emplacement pour derrick (clé D)");
		if ( num == OBJECT_MARKSTONE    )  strcpy(text, "Emplacement pour derrick (titanium)");
		if ( num == OBJECT_MOBILEft     )  strcpy(text, "Robot d'entraînement");
		if ( num == OBJECT_MOBILEtt     )  strcpy(text, "Robot d'entraînement");
		if ( num == OBJECT_MOBILEwt     )  strcpy(text, "Robot d'entraînement");
		if ( num == OBJECT_MOBILEit     )  strcpy(text, "Robot d'entraînement");
		if ( num == OBJECT_MOBILEfa     )  strcpy(text, "Robot déménageur");
		if ( num == OBJECT_MOBILEta     )  strcpy(text, "Robot déménageur");
		if ( num == OBJECT_MOBILEwa     )  strcpy(text, "Robot déménageur");
		if ( num == OBJECT_MOBILEia     )  strcpy(text, "Robot déménageur");
		if ( num == OBJECT_MOBILEfc     )  strcpy(text, "Robot shooter");
		if ( num == OBJECT_MOBILEtc     )  strcpy(text, "Robot shooter");
		if ( num == OBJECT_MOBILEwc     )  strcpy(text, "Robot shooter");
		if ( num == OBJECT_MOBILEic     )  strcpy(text, "Robot shooter");
		if ( num == OBJECT_MOBILEfi     )  strcpy(text, "Robot orgaShooter");
		if ( num == OBJECT_MOBILEti     )  strcpy(text, "Robot orgaShooter");
		if ( num == OBJECT_MOBILEwi     )  strcpy(text, "Robot orgaShooter");
		if ( num == OBJECT_MOBILEii     )  strcpy(text, "Robot orgaShooter");
		if ( num == OBJECT_MOBILEfs     )  strcpy(text, "Robot renifleur");
		if ( num == OBJECT_MOBILEts     )  strcpy(text, "Robot renifleur");
		if ( num == OBJECT_MOBILEws     )  strcpy(text, "Robot renifleur");
		if ( num == OBJECT_MOBILEis     )  strcpy(text, "Robot renifleur");
		if ( num == OBJECT_MOBILErt     )  strcpy(text, "Robot secoueur");
		if ( num == OBJECT_MOBILErc     )  strcpy(text, "Robot phazer");
		if ( num == OBJECT_MOBILErr     )  strcpy(text, "Robot recycleur");
		if ( num == OBJECT_MOBILErs     )  strcpy(text, "Robot bouclier");
		if ( num == OBJECT_MOBILEsa     )  strcpy(text, "Robot sous-marin");
		if ( num == OBJECT_MOBILEtg     )  strcpy(text, "Cible d'entraînement");
		if ( num == OBJECT_MOBILEdr     )  strcpy(text, "Robot dessinateur");
		if ( num == OBJECT_HUMAN        )  strcpy(text, g_gamerName);
		if ( num == OBJECT_TECH         )  strcpy(text, "Technicien");
		if ( num == OBJECT_TOTO         )  strcpy(text, "Robbie");
		if ( num == OBJECT_MOTHER       )  strcpy(text, "Pondeuse");
		if ( num == OBJECT_ANT          )  strcpy(text, "Fourmi");
		if ( num == OBJECT_SPIDER       )  strcpy(text, "Araignée");
		if ( num == OBJECT_BEE          )  strcpy(text, "Guępe");
		if ( num == OBJECT_WORM         )  strcpy(text, "Ver");
		if ( num == OBJECT_EGG          )  strcpy(text, "Oeuf");
		if ( num == OBJECT_RUINmobilew1 )  strcpy(text, "Epave de robot");
		if ( num == OBJECT_RUINmobilew2 )  strcpy(text, "Epave de robot");
		if ( num == OBJECT_RUINmobilet1 )  strcpy(text, "Epave de robot");
		if ( num == OBJECT_RUINmobilet2 )  strcpy(text, "Epave de robot");
		if ( num == OBJECT_RUINmobiler1 )  strcpy(text, "Epave de robot");
		if ( num == OBJECT_RUINmobiler2 )  strcpy(text, "Epave de robot");
		if ( num == OBJECT_RUINfactory  )  strcpy(text, "Bâtiment en ruine");
		if ( num == OBJECT_RUINdoor     )  strcpy(text, "Bâtiment en ruine");
		if ( num == OBJECT_RUINsupport  )  strcpy(text, "Déchet");
		if ( num == OBJECT_RUINradar    )  strcpy(text, "Bâtiment en ruine");
		if ( num == OBJECT_RUINconvert  )  strcpy(text, "Bâtiment en ruine");
		if ( num == OBJECT_RUINbase     )  strcpy(text, "Epave de vaisseau spatial");
		if ( num == OBJECT_RUINhead     )  strcpy(text, "Epave de vaisseau spatial");
		if ( num == OBJECT_APOLLO1      ||
			 num == OBJECT_APOLLO3      ||
			 num == OBJECT_APOLLO4      ||
			 num == OBJECT_APOLLO5      )  strcpy(text, "Vestige d'une mission Apollo");
		if ( num == OBJECT_APOLLO2      )  strcpy(text, "Lunar Roving Vehicle");
	}

	if ( type == RES_ERR )
	{
		strcpy(text, "Erreur");
		if ( num == ERR_CMD             )  strcpy(text, "Commande inconnue");
#if _NEWLOOK
		if ( num == ERR_INSTALL         )  strcpy(text, "CeeBot n'est pas installé.");
		if ( num == ERR_NOCD            )  strcpy(text, "Veuillez mettre le CD de CeeBot\net relancer le jeu.");
#else
		if ( num == ERR_INSTALL         )  strcpy(text, "COLOBOT n'est pas installé.");
		if ( num == ERR_NOCD            )  strcpy(text, "Veuillez mettre le CD de COLOBOT\net relancer le jeu.");
#endif
		if ( num == ERR_MANIP_VEH       )  strcpy(text, "Robot inadapté");
		if ( num == ERR_MANIP_FLY       )  strcpy(text, "Impossible en vol");
		if ( num == ERR_MANIP_BUSY      )  strcpy(text, "Porte déjŕ quelque chose");
		if ( num == ERR_MANIP_NIL       )  strcpy(text, "Rien ŕ prendre");
		if ( num == ERR_MANIP_MOTOR     )  strcpy(text, "Impossible en mouvement");
		if ( num == ERR_MANIP_OCC       )  strcpy(text, "Emplacement occupé");
		if ( num == ERR_MANIP_FRIEND    )  strcpy(text, "Pas d'autre robot");
		if ( num == ERR_MANIP_RADIO     )  strcpy(text, "Vous ne pouvez pas transporter un objet radioactif");
		if ( num == ERR_MANIP_WATER     )  strcpy(text, "Vous ne pouvez pas transporter un objet sous l'eau");
		if ( num == ERR_MANIP_EMPTY     )  strcpy(text, "Rien ŕ déposer");
		if ( num == ERR_BUILD_FLY       )  strcpy(text, "Impossible en vol");
		if ( num == ERR_BUILD_WATER     )  strcpy(text, "Impossible sous l'eau");
		if ( num == ERR_BUILD_ENERGY    )  strcpy(text, "Pas assez d'énergie");
		if ( num == ERR_BUILD_METALAWAY )  strcpy(text, "Titanium trop loin");
		if ( num == ERR_BUILD_METALNEAR )  strcpy(text, "Titanium trop proche");
		if ( num == ERR_BUILD_METALINEX )  strcpy(text, "Titanium inexistant");
		if ( num == ERR_BUILD_FLAT      )  strcpy(text, "Sol pas assez plat");
		if ( num == ERR_BUILD_FLATLIT   )  strcpy(text, "Sol plat pas assez grand");
		if ( num == ERR_BUILD_BUSY      )  strcpy(text, "Emplacement occupé");
		if ( num == ERR_BUILD_BASE      )  strcpy(text, "Trop proche du vaisseau spatial");
		if ( num == ERR_BUILD_NARROW    )  strcpy(text, "Trop proche d'un bâtiment");
		if ( num == ERR_BUILD_MOTOR     )  strcpy(text, "Impossible en mouvement");
		if ( num == ERR_SEARCH_FLY      )  strcpy(text, "Impossible en vol");
		if ( num == ERR_SEARCH_VEH      )  strcpy(text, "Robot inadapté");
		if ( num == ERR_SEARCH_MOTOR    )  strcpy(text, "Impossible en mouvement");
		if ( num == ERR_TERRA_VEH       )  strcpy(text, "Robot inadapté");
		if ( num == ERR_TERRA_ENERGY    )  strcpy(text, "Pas assez d'énergie");
		if ( num == ERR_TERRA_FLOOR     )  strcpy(text, "Terrain inadapté");
		if ( num == ERR_TERRA_BUILDING  )  strcpy(text, "Bâtiment trop proche");
		if ( num == ERR_TERRA_OBJECT    )  strcpy(text, "Objet trop proche");
		if ( num == ERR_RECOVER_VEH     )  strcpy(text, "Robot inadapté");
		if ( num == ERR_RECOVER_ENERGY  )  strcpy(text, "Pas assez d'énergie");
		if ( num == ERR_RECOVER_NULL    )  strcpy(text, "Rien ŕ recycler");
		if ( num == ERR_SHIELD_VEH      )  strcpy(text, "Robot inadapté");
		if ( num == ERR_SHIELD_ENERGY   )  strcpy(text, "Plus d'énergie");
		if ( num == ERR_MOVE_IMPOSSIBLE )  strcpy(text, "Déplacement impossible");
		if ( num == ERR_FIND_IMPOSSIBLE )  strcpy(text, "Objet n'existe pas");
		if ( num == ERR_GOTO_IMPOSSIBLE )  strcpy(text, "Chemin introuvable");
		if ( num == ERR_GOTO_ITER       )  strcpy(text, "Position inaccessible");
		if ( num == ERR_GOTO_BUSY       )  strcpy(text, "Destination occupée");
		if ( num == ERR_FIRE_VEH        )  strcpy(text, "Robot inadapté");
		if ( num == ERR_FIRE_ENERGY     )  strcpy(text, "Pas assez d'énergie");
		if ( num == ERR_FIRE_FLY        )  strcpy(text, "Impossible en vol");
		if ( num == ERR_CONVERT_EMPTY   )  strcpy(text, "Pas de minerai de titanium ŕ convertir");
		if ( num == ERR_DERRICK_NULL    )  strcpy(text, "Pas de minerai en sous-sol");
		if ( num == ERR_STATION_NULL    )  strcpy(text, "Pas d'énergie en sous-sol");
		if ( num == ERR_TOWER_POWER     )  strcpy(text, "Pas de pile");
		if ( num == ERR_TOWER_ENERGY    )  strcpy(text, "Plus d'énergie");
		if ( num == ERR_RESEARCH_POWER  )  strcpy(text, "Pas de pile");
		if ( num == ERR_RESEARCH_ENERGY )  strcpy(text, "Plus assez d'énergie");
		if ( num == ERR_RESEARCH_TYPE   )  strcpy(text, "Pas le bon type de pile");
		if ( num == ERR_RESEARCH_ALREADY)  strcpy(text, "Recherche déjŕ effectuée");
		if ( num == ERR_ENERGY_NULL     )  strcpy(text, "Pas d'énergie en sous-sol");
		if ( num == ERR_ENERGY_LOW      )  strcpy(text, "Pas encore assez d'énergie");
		if ( num == ERR_ENERGY_EMPTY    )  strcpy(text, "Pas de titanium ŕ transformer");
		if ( num == ERR_ENERGY_BAD      )  strcpy(text, "Ne transforme que le titanium");
		if ( num == ERR_BASE_DLOCK      )  strcpy(text, "Portes bloquées par un robot ou un objet");
		if ( num == ERR_BASE_DHUMAN     )  strcpy(text, "Vous devez embarquer pour pouvoir décoller");
		if ( num == ERR_LABO_NULL       )  strcpy(text, "Rien ŕ analyser");
		if ( num == ERR_LABO_BAD        )  strcpy(text, "N'analyse que la matičre organique");
		if ( num == ERR_LABO_ALREADY    )  strcpy(text, "Analyse déjŕ effectuée");
		if ( num == ERR_NUCLEAR_NULL    )  strcpy(text, "Pas d'énergie en sous-sol");
		if ( num == ERR_NUCLEAR_LOW     )  strcpy(text, "Pas encore assez d'énergie");
		if ( num == ERR_NUCLEAR_EMPTY   )  strcpy(text, "Pas d'uranium ŕ transformer");
		if ( num == ERR_NUCLEAR_BAD     )  strcpy(text, "Ne transforme que l'uranium");
		if ( num == ERR_FACTORY_NULL    )  strcpy(text, "Pas de titanium");
		if ( num == ERR_FACTORY_NEAR    )  strcpy(text, "Quelque chose est trop proche");
		if ( num == ERR_RESET_NEAR      )  strcpy(text, "Emplacement occupé");
		if ( num == ERR_INFO_NULL       )  strcpy(text, "Pas trouvé de borne d'information");
		if ( num == ERR_VEH_VIRUS       )  strcpy(text, "Un programme est infecté par un virus");
		if ( num == ERR_BAT_VIRUS       )  strcpy(text, "Infecté par un virus, ne fonctionne plus temporairement");
		if ( num == ERR_VEH_POWER       )  strcpy(text, "Pas de pile");
		if ( num == ERR_VEH_ENERGY      )  strcpy(text, "Plus d'énergie");
		if ( num == ERR_FLAG_FLY        )  strcpy(text, "Impossible en vol");
		if ( num == ERR_FLAG_WATER      )  strcpy(text, "Impossible en nageant");
		if ( num == ERR_FLAG_MOTOR      )  strcpy(text, "Impossible en mouvement");
		if ( num == ERR_FLAG_BUSY       )  strcpy(text, "Impossible en portant un objet");
		if ( num == ERR_FLAG_CREATE     )  strcpy(text, "Trop de drapeaux de cette couleur (maximum 5)");
		if ( num == ERR_FLAG_PROXY      )  strcpy(text, "Trop proche d'un drapeau existant");
		if ( num == ERR_FLAG_DELETE     )  strcpy(text, "Aucun drapeau ŕ proximité");
		if ( num == ERR_MISSION_NOTERM  )  strcpy(text, "La misssion n'est pas terminée (appuyez sur \\key help; pour plus de détails)");
		if ( num == ERR_DELETEMOBILE    )  strcpy(text, "Robot détruit");
		if ( num == ERR_DELETEBUILDING  )  strcpy(text, "Bâtiment détruit");
		if ( num == ERR_TOOMANY         )  strcpy(text, "Création impossible, il y a trop d'objets");
		if ( num == ERR_OBLIGATORYTOKEN )  strcpy(text, "Il manque \"%s\" dans le programme");
		if ( num == ERR_PROHIBITEDTOKEN )  strcpy(text, "Interdit dans cet exercice");

		if ( num == INFO_BUILD          )  strcpy(text, "Bâtiment terminé");
		if ( num == INFO_CONVERT        )  strcpy(text, "Titanium disponible");
		if ( num == INFO_RESEARCH       )  strcpy(text, "Recherche terminée");
		if ( num == INFO_RESEARCHTANK   )  strcpy(text, "Fabrication d'un robot ŕ chenilles possible");
		if ( num == INFO_RESEARCHFLY    )  strcpy(text, "Il est possible de voler avec les touches (\\key gup;) et (\\key gdown;)");
		if ( num == INFO_RESEARCHTHUMP  )  strcpy(text, "Fabrication d'un robot secoueur possible");
		if ( num == INFO_RESEARCHCANON  )  strcpy(text, "Fabrication de robots shooter possible");
		if ( num == INFO_RESEARCHTOWER  )  strcpy(text, "Construction d'une tour de défense possible");
		if ( num == INFO_RESEARCHPHAZER )  strcpy(text, "Fabrication d'un robot phazer possible");
		if ( num == INFO_RESEARCHSHIELD )  strcpy(text, "Fabrication d'un robot bouclier possible");
		if ( num == INFO_RESEARCHATOMIC )  strcpy(text, "Construction d'une centrale nucléaire possible");
		if ( num == INFO_FACTORY        )  strcpy(text, "Nouveau robot disponible");
		if ( num == INFO_LABO           )  strcpy(text, "Analyse terminée");
		if ( num == INFO_ENERGY         )  strcpy(text, "Pile disponible");
		if ( num == INFO_NUCLEAR        )  strcpy(text, "Pile nucléaire disponible");
		if ( num == INFO_FINDING        )  strcpy(text, "Vous avez trouvé un objet utilisable");
		if ( num == INFO_MARKPOWER      )  strcpy(text, "Emplacement pour station trouvé");
		if ( num == INFO_MARKURANIUM    )  strcpy(text, "Emplacement pour derrick trouvé");
		if ( num == INFO_MARKSTONE      )  strcpy(text, "Emplacement pour derrick trouvé");
		if ( num == INFO_MARKKEYa       )  strcpy(text, "Emplacement pour derrick trouvé");
		if ( num == INFO_MARKKEYb       )  strcpy(text, "Emplacement pour derrick trouvé");
		if ( num == INFO_MARKKEYc       )  strcpy(text, "Emplacement pour derrick trouvé");
		if ( num == INFO_MARKKEYd       )  strcpy(text, "Emplacement pour derrick trouvé");
		if ( num == INFO_WIN            )  strcpy(text, "<<< Bravo, mission terminée >>>");
		if ( num == INFO_LOST           )  strcpy(text, "<<< Désolé, mission échouée >>>");
		if ( num == INFO_LOSTq          )  strcpy(text, "<<< Désolé, mission échouée >>>");
		if ( num == INFO_WRITEOK        )  strcpy(text, "Enregistrement effectué");
		if ( num == INFO_DELETEPATH     )  strcpy(text, "Indicateur atteint");
		if ( num == INFO_DELETEMOTHER   )  strcpy(text, "Pondeuse mortellement touchée");
		if ( num == INFO_DELETEANT      )  strcpy(text, "Fourmi mortellement touchée");
		if ( num == INFO_DELETEBEE      )  strcpy(text, "Guępe mortellement touchée");
		if ( num == INFO_DELETEWORM     )  strcpy(text, "Ver mortellement touché");
		if ( num == INFO_DELETESPIDER   )  strcpy(text, "Araignée mortellement touchée");
		if ( num == INFO_BEGINSATCOM    )  strcpy(text, "Consultez votre SatCom en appuyant sur \\key help;");
	}

	if ( type == RES_CBOT )
	{
		strcpy(text, "Erreur");
		if ( num == TX_OPENPAR       ) strcpy(text, "Il manque une parenthčse ouvrante");
		if ( num == TX_CLOSEPAR      ) strcpy(text, "Il manque une parenthčse fermante");
		if ( num == TX_NOTBOOL       ) strcpy(text, "L'expression doit ętre un boolean");
		if ( num == TX_UNDEFVAR      ) strcpy(text, "Variable non déclarée");
		if ( num == TX_BADLEFT       ) strcpy(text, "Assignation impossible");
		if ( num == TX_ENDOF         ) strcpy(text, "Terminateur point-virgule non trouvé");
		if ( num == TX_OUTCASE       ) strcpy(text, "Instruction ""case"" hors d'un bloc ""switch""");
		if ( num == TX_NOTERM        ) strcpy(text, "Instructions aprčs la fin");
		if ( num == TX_CLOSEBLK      ) strcpy(text, "Il manque la fin du bloc");
		if ( num == TX_ELSEWITHOUTIF ) strcpy(text, "Instruction ""else"" sans ""if"" correspondant");
		if ( num == TX_OPENBLK       ) strcpy(text, "Début d'un bloc attendu");
		if ( num == TX_BADTYPE       ) strcpy(text, "Mauvais type de résultat pour l'assignation");
		if ( num == TX_REDEFVAR      ) strcpy(text, "Redéfinition d'une variable");
		if ( num == TX_BAD2TYPE      ) strcpy(text, "Les deux opérandes ne sont pas de types compatibles");
		if ( num == TX_UNDEFCALL     ) strcpy(text, "Routine inconnue");
		if ( num == TX_MISDOTS       ) strcpy(text, "Séparateur "" : "" attendu");
		if ( num == TX_WHILE         ) strcpy(text, "Manque le mot ""while""");
		if ( num == TX_BREAK         ) strcpy(text, "Instruction ""break"" en dehors d'une boucle");
		if ( num == TX_LABEL         ) strcpy(text, "Un label ne peut se placer que devant un ""for"", un ""while"", un ""do"" ou un ""switch""");
		if ( num == TX_NOLABEL       ) strcpy(text, "Cette étiquette n'existe pas");
		if ( num == TX_NOCASE        ) strcpy(text, "Manque une instruction ""case""");
		if ( num == TX_BADNUM        ) strcpy(text, "Un nombre est attendu");
		if ( num == TX_VOID          ) strcpy(text, "Paramčtre void");
		if ( num == TX_NOTYP         ) strcpy(text, "Déclaration de type attendu");
		if ( num == TX_NOVAR         ) strcpy(text, "Nom d'une variable attendu");
		if ( num == TX_NOFONC        ) strcpy(text, "Nom de la fonction attendu");
		if ( num == TX_OVERPARAM     ) strcpy(text, "Trop de paramčtres");
		if ( num == TX_REDEF         ) strcpy(text, "Cette fonction existe déjŕ");
		if ( num == TX_LOWPARAM      ) strcpy(text, "Pas assez de paramčtres");
		if ( num == TX_BADPARAM      ) strcpy(text, "Aucune fonction de ce nom n'accepte ce(s) type(s) de paramčtre(s)");
		if ( num == TX_NUMPARAM      ) strcpy(text, "Aucune fonction de ce nom n'accepte ce nombre de paramčtres");
		if ( num == TX_NOITEM        ) strcpy(text, "Cet élément n'existe pas dans cette classe");
		if ( num == TX_DOT           ) strcpy(text, "L'objet n'est pas une instance d'une classe");
		if ( num == TX_NOCONST       ) strcpy(text, "Il n'y a pas de constructeur approprié");
		if ( num == TX_REDEFCLASS    ) strcpy(text, "Cette classe existe déjŕ");
		if ( num == TX_CLBRK         ) strcpy(text, """ ] "" attendu");
		if ( num == TX_RESERVED      ) strcpy(text, "Ce mot est réservé");
		if ( num == TX_BADNEW        ) strcpy(text, "Mauvais argument pour ""new""");
		if ( num == TX_OPBRK         ) strcpy(text, """ [ "" attendu");
		if ( num == TX_BADSTRING     ) strcpy(text, "Une chaîne de caractčre est attendue");
		if ( num == TX_BADINDEX      ) strcpy(text, "Mauvais type d'index");
		if ( num == TX_PRIVATE       ) strcpy(text, "Elément protégé");
		if ( num == TX_NOPUBLIC      ) strcpy(text, "Public requis");
		if ( num == TX_DIVZERO       ) strcpy(text, "Division par zéro");
		if ( num == TX_NOTINIT       ) strcpy(text, "Variable non initialisée");
		if ( num == TX_BADTHROW      ) strcpy(text, "Valeur négative refusée pour ""throw""");
		if ( num == TX_NORETVAL      ) strcpy(text, "La fonction n'a pas retourné de résultat");
		if ( num == TX_NORUN         ) strcpy(text, "Pas de fonction en exécution");
		if ( num == TX_NOCALL        ) strcpy(text, "Appel d'une fonction inexistante");
		if ( num == TX_NOCLASS       ) strcpy(text, "Cette classe n'existe pas");
		if ( num == TX_NULLPT        ) strcpy(text, "Objet n'existe pas");
		if ( num == TX_OPNAN         ) strcpy(text, "Opération sur un ""nan""");
		if ( num == TX_OUTARRAY      ) strcpy(text, "Accčs hors du tableau");
		if ( num == TX_STACKOVER     ) strcpy(text, "Débordement de la pile");
		if ( num == TX_DELETEDPT     ) strcpy(text, "Objet inaccessible");
		if ( num == TX_FILEOPEN      ) strcpy(text, "Ouverture du fichier impossible");
		if ( num == TX_NOTOPEN       ) strcpy(text, "Le fichier n'est pas ouvert");
		if ( num == TX_ERRREAD       ) strcpy(text, "Erreur ŕ la lecture");
		if ( num == TX_ERRWRITE      ) strcpy(text, "Erreur ŕ l'écriture");
	}

	if ( type == RES_KEY )
	{
		if ( num == 0                   )  strcpy(text, "< aucune >");
		if ( num == VK_LEFT             )  strcpy(text, "Flčche Gauche");
		if ( num == VK_RIGHT            )  strcpy(text, "Flčche Droite");
		if ( num == VK_UP               )  strcpy(text, "Flčche Haut");
		if ( num == VK_DOWN             )  strcpy(text, "Flčche Bas");
		if ( num == VK_CANCEL           )  strcpy(text, "Control-break");
		if ( num == VK_BACK             )  strcpy(text, "<--");
		if ( num == VK_TAB              )  strcpy(text, "Tab");
		if ( num == VK_CLEAR            )  strcpy(text, "Clear");
		if ( num == VK_RETURN           )  strcpy(text, "Entrée");
		if ( num == VK_SHIFT            )  strcpy(text, "Shift");
		if ( num == VK_CONTROL          )  strcpy(text, "Ctrl");
		if ( num == VK_MENU             )  strcpy(text, "Alt");
		if ( num == VK_PAUSE            )  strcpy(text, "Pause");
		if ( num == VK_CAPITAL          )  strcpy(text, "Caps Lock");
		if ( num == VK_ESCAPE           )  strcpy(text, "Esc");
		if ( num == VK_SPACE            )  strcpy(text, "Espace");
		if ( num == VK_PRIOR            )  strcpy(text, "Page Up");
		if ( num == VK_NEXT             )  strcpy(text, "Page Down");
		if ( num == VK_END              )  strcpy(text, "End");
		if ( num == VK_HOME             )  strcpy(text, "Home");
		if ( num == VK_SELECT           )  strcpy(text, "Select");
		if ( num == VK_EXECUTE          )  strcpy(text, "Execute");
		if ( num == VK_SNAPSHOT         )  strcpy(text, "Print Scrn");
		if ( num == VK_INSERT           )  strcpy(text, "Insert");
		if ( num == VK_DELETE           )  strcpy(text, "Delete");
		if ( num == VK_HELP             )  strcpy(text, "Help");
		if ( num == VK_LWIN             )  strcpy(text, "Left Windows");
		if ( num == VK_RWIN             )  strcpy(text, "Right Windows");
		if ( num == VK_APPS             )  strcpy(text, "Application key");
		if ( num == VK_NUMPAD0          )  strcpy(text, "NumPad 0");
		if ( num == VK_NUMPAD1          )  strcpy(text, "NumPad 1");
		if ( num == VK_NUMPAD2          )  strcpy(text, "NumPad 2");
		if ( num == VK_NUMPAD3          )  strcpy(text, "NumPad 3");
		if ( num == VK_NUMPAD4          )  strcpy(text, "NumPad 4");
		if ( num == VK_NUMPAD5          )  strcpy(text, "NumPad 5");
		if ( num == VK_NUMPAD6          )  strcpy(text, "NumPad 6");
		if ( num == VK_NUMPAD7          )  strcpy(text, "NumPad 7");
		if ( num == VK_NUMPAD8          )  strcpy(text, "NumPad 8");
		if ( num == VK_NUMPAD9          )  strcpy(text, "NumPad 9");
		if ( num == VK_MULTIPLY         )  strcpy(text, "NumPad *");
		if ( num == VK_ADD              )  strcpy(text, "NumPad +");
		if ( num == VK_SEPARATOR        )  strcpy(text, "NumPad sep");
		if ( num == VK_SUBTRACT         )  strcpy(text, "NumPad -");
		if ( num == VK_DECIMAL          )  strcpy(text, "NumPad .");
		if ( num == VK_DIVIDE           )  strcpy(text, "NumPad /");
		if ( num == VK_F1               )  strcpy(text, "F1");
		if ( num == VK_F2               )  strcpy(text, "F2");
		if ( num == VK_F3               )  strcpy(text, "F3");
		if ( num == VK_F4               )  strcpy(text, "F4");
		if ( num == VK_F5               )  strcpy(text, "F5");
		if ( num == VK_F6               )  strcpy(text, "F6");
		if ( num == VK_F7               )  strcpy(text, "F7");
		if ( num == VK_F8               )  strcpy(text, "F8");
		if ( num == VK_F9               )  strcpy(text, "F9");
		if ( num == VK_F10              )  strcpy(text, "F10");
		if ( num == VK_F11              )  strcpy(text, "F11");
		if ( num == VK_F12              )  strcpy(text, "F12");
		if ( num == VK_F13              )  strcpy(text, "F13");
		if ( num == VK_F14              )  strcpy(text, "F14");
		if ( num == VK_F15              )  strcpy(text, "F15");
		if ( num == VK_F16              )  strcpy(text, "F16");
		if ( num == VK_F17              )  strcpy(text, "F17");
		if ( num == VK_F18              )  strcpy(text, "F18");
		if ( num == VK_F19              )  strcpy(text, "F19");
		if ( num == VK_F20              )  strcpy(text, "F20");
		if ( num == VK_NUMLOCK          )  strcpy(text, "Num Lock");
		if ( num == VK_SCROLL           )  strcpy(text, "Scroll");
		if ( num == VK_ATTN             )  strcpy(text, "Attn");
		if ( num == VK_CRSEL            )  strcpy(text, "CrSel");
		if ( num == VK_EXSEL            )  strcpy(text, "ExSel");
		if ( num == VK_EREOF            )  strcpy(text, "Erase EOF");
		if ( num == VK_PLAY             )  strcpy(text, "Play");
		if ( num == VK_ZOOM             )  strcpy(text, "Zoom");
		if ( num == VK_PA1              )  strcpy(text, "PA1");
		if ( num == VK_OEM_CLEAR        )  strcpy(text, "Clear");
		if ( num == VK_BUTTON1          )  strcpy(text, "Bouton 1");
		if ( num == VK_BUTTON2          )  strcpy(text, "Bouton 2");
		if ( num == VK_BUTTON3          )  strcpy(text, "Bouton 3");
		if ( num == VK_BUTTON4          )  strcpy(text, "Bouton 4");
		if ( num == VK_BUTTON5          )  strcpy(text, "Bouton 5");
		if ( num == VK_BUTTON6          )  strcpy(text, "Bouton 6");
		if ( num == VK_BUTTON7          )  strcpy(text, "Bouton 7");
		if ( num == VK_BUTTON8          )  strcpy(text, "Bouton 8");
		if ( num == VK_BUTTON9          )  strcpy(text, "Bouton 9");
		if ( num == VK_BUTTON10         )  strcpy(text, "Bouton 10");
		if ( num == VK_BUTTON11         )  strcpy(text, "Bouton 11");
		if ( num == VK_BUTTON12         )  strcpy(text, "Bouton 12");
		if ( num == VK_BUTTON13         )  strcpy(text, "Bouton 13");
		if ( num == VK_BUTTON14         )  strcpy(text, "Bouton 14");
		if ( num == VK_BUTTON15         )  strcpy(text, "Bouton 15");
		if ( num == VK_BUTTON16         )  strcpy(text, "Bouton 16");
		if ( num == VK_BUTTON17         )  strcpy(text, "Bouton 17");
		if ( num == VK_BUTTON18         )  strcpy(text, "Bouton 18");
		if ( num == VK_BUTTON19         )  strcpy(text, "Bouton 19");
		if ( num == VK_BUTTON20         )  strcpy(text, "Bouton 20");
		if ( num == VK_BUTTON21         )  strcpy(text, "Bouton 21");
		if ( num == VK_BUTTON22         )  strcpy(text, "Bouton 22");
		if ( num == VK_BUTTON23         )  strcpy(text, "Bouton 23");
		if ( num == VK_BUTTON24         )  strcpy(text, "Bouton 24");
		if ( num == VK_BUTTON25         )  strcpy(text, "Bouton 25");
		if ( num == VK_BUTTON26         )  strcpy(text, "Bouton 26");
		if ( num == VK_BUTTON27         )  strcpy(text, "Bouton 27");
		if ( num == VK_BUTTON28         )  strcpy(text, "Bouton 28");
		if ( num == VK_BUTTON29         )  strcpy(text, "Bouton 29");
		if ( num == VK_BUTTON30         )  strcpy(text, "Bouton 30");
		if ( num == VK_BUTTON31         )  strcpy(text, "Bouton 31");
		if ( num == VK_BUTTON32         )  strcpy(text, "Bouton 32");
		if ( num == VK_WHEELUP          )  strcpy(text, "Molette haut");
		if ( num == VK_WHEELDOWN        )  strcpy(text, "Molette bas");
	}
#endif

#if _GERMAN | _WG
	if ( type == RES_TEXT )
	{
		#if _FULL
		if ( num == RT_VERSION_ID          )  strcpy(text, "1.18 /d");
		#endif
		#if _NET
		if ( num == RT_VERSION_ID          )  strcpy(text, "CeeBot-A 1.18");
		#endif
		#if _SCHOOL & _EDU
		#if _TEEN
		if ( num == RT_VERSION_ID          )  strcpy(text, "CeeBot-Teen EDU 1.18");
		#else
		if ( num == RT_VERSION_ID          )  strcpy(text, "CeeBot-A EDU 1.18");
		#endif
		#endif
		#if _SCHOOL & _PERSO
		#if _TEEN
		if ( num == RT_VERSION_ID          )  strcpy(text, "CeeBot-Teen PERSO 1.18");
		#else
		if ( num == RT_VERSION_ID          )  strcpy(text, "CeeBot-A PERSO 1.18");
		#endif
		#endif
		#if _SCHOOL & _CEEBOTDEMO
		#if _TEEN
		if ( num == RT_VERSION_ID          )  strcpy(text, "CeeBot-Teen DEMO 1.18");
		#else
		if ( num == RT_VERSION_ID          )  strcpy(text, "CeeBot-A DEMO 1.18");
		#endif
		#endif
		#if _DEMO
		if ( num == RT_VERSION_ID          )  strcpy(text, "Demo 1.18 /d");
		#endif
		if ( num == RT_DISINFO_TITLE       )  strcpy(text, "SatCom");
		if ( num == RT_WINDOW_MAXIMIZED    )  strcpy(text, "Großes Fenster");
		if ( num == RT_WINDOW_MINIMIZED    )  strcpy(text, "Reduzieren");
		if ( num == RT_WINDOW_STANDARD     )  strcpy(text, "Normale Größe");
		if ( num == RT_WINDOW_CLOSE        )  strcpy(text, "Schließen");

		if ( num == RT_STUDIO_TITLE        )  strcpy(text, "Programmeditor");
		if ( num == RT_SCRIPT_NEW          )  strcpy(text, "Neu");
		if ( num == RT_NAME_DEFAULT        )  strcpy(text, "Spieler");
		if ( num == RT_IO_NEW              )  strcpy(text, "Neu ...");
		if ( num == RT_KEY_OR              )  strcpy(text, " oder ");

#if _NEWLOOK
		if ( num == RT_TITLE_BASE          )  strcpy(text, "CeeBot");
		if ( num == RT_TITLE_INIT          )  strcpy(text, "CeeBot");
#else
		if ( num == RT_TITLE_BASE          )  strcpy(text, "COLOBOT");
		if ( num == RT_TITLE_INIT          )  strcpy(text, "COLOBOT");
#endif
#if _SCHOOL
		if ( num == RT_TITLE_TRAINER       )  strcpy(text, "Übungen");
#else
		if ( num == RT_TITLE_TRAINER       )  strcpy(text, "Programmieren");
#endif
		if ( num == RT_TITLE_DEFI          )  strcpy(text, "Challenges");
		if ( num == RT_TITLE_MISSION       )  strcpy(text, "Missionen");
		if ( num == RT_TITLE_FREE          )  strcpy(text, "Freestyle");
		if ( num == RT_TITLE_TEEN          )  strcpy(text, "Freestyle");
		if ( num == RT_TITLE_USER          )  strcpy(text, "Userlevels");
		if ( num == RT_TITLE_PROTO         )  strcpy(text, "Prototypen");
		if ( num == RT_TITLE_SETUP         )  strcpy(text, "Einstellungen");
		if ( num == RT_TITLE_NAME          )  strcpy(text, "Name ");
		if ( num == RT_TITLE_PERSO         )  strcpy(text, "Aussehen einstellen");
		if ( num == RT_TITLE_WRITE         )  strcpy(text, "Aktuelle Mission speichern");
		if ( num == RT_TITLE_READ          )  strcpy(text, "Gespeicherte Mission laden");

		if ( num == RT_PLAY_CHAPt          )  strcpy(text, " Liste der Kapitel:");
		if ( num == RT_PLAY_CHAPd          )  strcpy(text, " Liste der Kapitel:");
		if ( num == RT_PLAY_CHAPm          )  strcpy(text, " Liste der Planeten:");
		if ( num == RT_PLAY_CHAPf          )  strcpy(text, " Liste der Planeten:");
		if ( num == RT_PLAY_CHAPu          )  strcpy(text, " Userlevels:");
		if ( num == RT_PLAY_CHAPp          )  strcpy(text, " Liste der Planeten:");
		if ( num == RT_PLAY_CHAPte         )  strcpy(text, " Liste der Kapitel:");
		if ( num == RT_PLAY_LISTt          )  strcpy(text, " Liste der Übungen des Kapitels:");
		if ( num == RT_PLAY_LISTd          )  strcpy(text, " Liste der Challenges des Kapitels:");
		if ( num == RT_PLAY_LISTm          )  strcpy(text, " Liste der Missionen des Planeten:");
		if ( num == RT_PLAY_LISTf          )  strcpy(text, " Liste der freien Levels des Planeten:");
		if ( num == RT_PLAY_LISTu          )  strcpy(text, " Missionen des Userlevels:");
		if ( num == RT_PLAY_LISTp          )  strcpy(text, " Liste der Prototypen des Planeten:");
		if ( num == RT_PLAY_LISTk          )  strcpy(text, " Liste der freien Levels des Kapitel:");
		if ( num == RT_PLAY_RESUME         )  strcpy(text, " Zusammenfassung:");

		if ( num == RT_SETUP_DEVICE        )  strcpy(text, " Driver:");
		if ( num == RT_SETUP_MODE          )  strcpy(text, " Auflösung:");
		if ( num == RT_SETUP_KEY1          )  strcpy(text, "1) Klicken Sie auf die neu zu definierende Taste.");
		if ( num == RT_SETUP_KEY2          )  strcpy(text, "2) Drücken Sie auf die neue Taste.");

		if ( num == RT_PERSO_FACE          )  strcpy(text, "Kopf:");
		if ( num == RT_PERSO_GLASSES       )  strcpy(text, "Brille:");
		if ( num == RT_PERSO_HAIR          )  strcpy(text, "Haarfarbe:");
		if ( num == RT_PERSO_COMBI         )  strcpy(text, "Farbe des Anzugs:");
		if ( num == RT_PERSO_BAND          )  strcpy(text, "Farbe der Streifen:");

#if _NEWLOOK
		if ( num == RT_DIALOG_TITLE        )  strcpy(text, "CeeBot");
		if ( num == RT_DIALOG_QUIT         )  strcpy(text, "Wollen Sie CeeBot schließen ?");
		if ( num == RT_DIALOG_YESQUIT      )  strcpy(text, "Schließen\\CeeBot schließen");
#else
		if ( num == RT_DIALOG_TITLE        )  strcpy(text, "COLOBOT");
		if ( num == RT_DIALOG_QUIT         )  strcpy(text, "Wollen Sie COLOBOT schließen ?");
		if ( num == RT_DIALOG_YESQUIT      )  strcpy(text, "Schließen\\COLOBOT schließen");
#endif
		if ( num == RT_DIALOG_ABORT        )  strcpy(text, "Mission abbrechen ?");
		if ( num == RT_DIALOG_YES          )  strcpy(text, "Abbrechen\\Mission abbrechen");
		if ( num == RT_DIALOG_NO           )  strcpy(text, "Weitermachen\\Mission weitermachen");
		if ( num == RT_DIALOG_NOQUIT       )  strcpy(text, "Weitermachen\\Weitermachen");
		if ( num == RT_DIALOG_DELOBJ       )  strcpy(text, "Wollen Sie das angewählte Gebäude wirklich zerstören ?");
		if ( num == RT_DIALOG_DELGAME      )  strcpy(text, "Wollen Sie die gespeicherten Missionen von %s löschen ?");
		if ( num == RT_DIALOG_YESDEL       )  strcpy(text, "Zerstören");
		if ( num == RT_DIALOG_NODEL        )  strcpy(text, "Abbrechen");
		if ( num == RT_DIALOG_LOADING      )  strcpy(text, "Laden");

		if ( num == RT_STUDIO_LISTTT       )  strcpy(text, "Hilfe über den Begriff (\\key cbot;)");
		if ( num == RT_STUDIO_COMPOK       )  strcpy(text, "Kompilieren OK (0 Fehler)");
		if ( num == RT_STUDIO_PROGSTOP     )  strcpy(text, "Programm beendet");

		if ( num == RT_SATCOM_LIST         )  strcpy(text, "\\b;Liste der Objekte\n");
		if ( num == RT_SATCOM_BOT          )  strcpy(text, "\\b;Liste der Roboter\n");
		if ( num == RT_SATCOM_BUILDING     )  strcpy(text, "\\b;Listes der Gebäude\n");
		if ( num == RT_SATCOM_FRET         )  strcpy(text, "\\b;Listes der tragbaren Gegenstände\n");
		if ( num == RT_SATCOM_ALIEN        )  strcpy(text, "\\b;Listes der Feinde\n");
		if ( num == RT_SATCOM_NULL         )  strcpy(text, "\\c; (keine)\\n;\n");
		if ( num == RT_SATCOM_ERROR1       )  strcpy(text, "\\b;Fehler\n");
		if ( num == RT_SATCOM_ERROR2       )  strcpy(text, "Die Liste ist ohne \\l;Radar\\u object\\radar; nicht verfügbar !\n");

		if ( num == RT_IO_OPEN             )  strcpy(text, "Öffnen");
		if ( num == RT_IO_SAVE             )  strcpy(text, "Speichern");
		if ( num == RT_IO_LIST             )  strcpy(text, "Ordner: %s");
		if ( num == RT_IO_NAME             )  strcpy(text, "Name:");
		if ( num == RT_IO_DIR              )  strcpy(text, "In:");
		if ( num == RT_IO_PRIVATE          )  strcpy(text, "Privat\\Privater Ordner");
		if ( num == RT_IO_PUBLIC           )  strcpy(text, "Öffentlich\\Gemeinsamer Ordner für alle Spieler");

		if ( num == RT_GENERIC_DEV1        )  strcpy(text, "Entwickelt von:");
		if ( num == RT_GENERIC_DEV2        )  strcpy(text, "www.epsitec.com");
#if _WG
		if ( num == RT_GENERIC_EDIT1       )  strcpy(text, "Herausgegeben von:");
		if ( num == RT_GENERIC_EDIT2       )  strcpy(text, "www.wg-verlag.ch");
#else
		if ( num == RT_GENERIC_EDIT1       )  strcpy(text, " ");
		if ( num == RT_GENERIC_EDIT2       )  strcpy(text, " ");
#endif

		if ( num == RT_INTERFACE_REC       )  strcpy(text, "Recorder");
	}

	if ( type == RES_EVENT )
	{
		if ( num == EVENT_BUTTON_OK        )  strcpy(text, "OK");
		if ( num == EVENT_BUTTON_CANCEL    )  strcpy(text, "Abbrechen");
		if ( num == EVENT_BUTTON_NEXT      )  strcpy(text, "Nächster");
		if ( num == EVENT_BUTTON_PREV      )  strcpy(text, "Vorherg.");
		if ( num == EVENT_BUTTON_QUIT      )  strcpy(text, "Menü (\\key quit;)");

		if ( num == EVENT_DIALOG_OK        )  strcpy(text, "OK");
		if ( num == EVENT_DIALOG_CANCEL    )  strcpy(text, "Abbrechen");

#if _SCHOOL
		if ( num == EVENT_INTERFACE_TRAINER)  strcpy(text, "Übungen\\Programmierübungen");
#else
		if ( num == EVENT_INTERFACE_TRAINER)  strcpy(text, "Programmieren\\Programmierübungen");
#endif
		if ( num == EVENT_INTERFACE_DEFI   )  strcpy(text, "Challenges\\Herausforderungen");
		if ( num == EVENT_INTERFACE_MISSION)  strcpy(text, "Missionen\\Aufbruch ins Weltall");
		if ( num == EVENT_INTERFACE_FREE   )  strcpy(text, "Freestyle\\Freies Spielen ohne vorgegebenes Ziel");
		if ( num == EVENT_INTERFACE_TEEN   )  strcpy(text, "Freestyle\\Freies Spielen ohne vorgegebenes Ziel");
		if ( num == EVENT_INTERFACE_USER   )  strcpy(text, "User\\Userlevels");
		if ( num == EVENT_INTERFACE_PROTO  )  strcpy(text, "Proto\\In Entwicklung befindliche Prototypen");
		if ( num == EVENT_INTERFACE_NAME   )  strcpy(text, "Anderer Spieler\\Spielername ändern");
		if ( num == EVENT_INTERFACE_SETUP  )  strcpy(text, "Einstellungen\\Einstellungen");
		if ( num == EVENT_INTERFACE_AGAIN  )  strcpy(text, "Neu anfangen\\Die Mission von vorne anfangen");
		if ( num == EVENT_INTERFACE_WRITE  )  strcpy(text, "Speichern\\Aktuelle Mission speichern");
		if ( num == EVENT_INTERFACE_READ   )  strcpy(text, "Laden\\Eine gespeicherte Mission öffnen");
#if _NEWLOOK
		if ( num == EVENT_INTERFACE_ABORT  )  strcpy(text, "\\Zurück zu CeeBot");
		if ( num == EVENT_INTERFACE_QUIT   )  strcpy(text, "Schließen\\CeeBot schließen");
#else
		if ( num == EVENT_INTERFACE_ABORT  )  strcpy(text, "\\Zurück zu COLOBOT");
		if ( num == EVENT_INTERFACE_QUIT   )  strcpy(text, "Schließen\\COLOBOT schließen");
#endif
		if ( num == EVENT_INTERFACE_BACK   )  strcpy(text, "<<  Zurück  \\Zurück zum Hauptmenü");
		if ( num == EVENT_INTERFACE_PLAY   )  strcpy(text, "Spielen ...\\Los geht's");
		if ( num == EVENT_INTERFACE_SETUPd )  strcpy(text, "Bildschirm\\Driver und Bildschirmauflösung");
		if ( num == EVENT_INTERFACE_SETUPg )  strcpy(text, "Grafik\\Grafische Einstellungen");
		if ( num == EVENT_INTERFACE_SETUPp )  strcpy(text, "Spiel\\Gameplay Einstellungen");
		if ( num == EVENT_INTERFACE_SETUPc )  strcpy(text, "Steuerung\\Auswahl der Tasten");
		if ( num == EVENT_INTERFACE_SETUPs )  strcpy(text, "Geräusche\\Lautstärke Geräusche und Musik");
		if ( num == EVENT_INTERFACE_DEVICE )  strcpy(text, "Einheit");
		if ( num == EVENT_INTERFACE_RESOL  )  strcpy(text, "Auflösung");
		if ( num == EVENT_INTERFACE_FULL   )  strcpy(text, "Vollbildschirm\\Vollbildschirm oder Fenster");
		if ( num == EVENT_INTERFACE_APPLY  )  strcpy(text, "Änderungen ausführen\\Getätigte Einstellungen ausführen");

		if ( num == EVENT_INTERFACE_TOTO   )  strcpy(text, "Robby\\Ihr Assistent");
		if ( num == EVENT_INTERFACE_SHADOW )  strcpy(text, "Schatten\\Schlagschatten auf dem Boden");
		if ( num == EVENT_INTERFACE_GROUND )  strcpy(text, "Markierungen\\Markierungen auf dem Boden");
		if ( num == EVENT_INTERFACE_DIRTY  )  strcpy(text, "Schmutz\\Schmutz auf Robotern und Bauten");
		if ( num == EVENT_INTERFACE_FOG    )  strcpy(text, "Nebel\\Nebelschwaden");
		if ( num == EVENT_INTERFACE_LENS   )  strcpy(text, "Sonnenstrahlen\\Sonnenstrahlen");
		if ( num == EVENT_INTERFACE_SKY    )  strcpy(text, "Himmel\\Himmel und Wolken");
		if ( num == EVENT_INTERFACE_PLANET )  strcpy(text, "Planeten und Sterne\\Kreisende Planeten und Sterne");
		if ( num == EVENT_INTERFACE_LIGHT  )  strcpy(text, "Dynamische Beleuchtung\\Dynamische Beleuchtung");
		if ( num == EVENT_INTERFACE_PARTI  )  strcpy(text, "Anzahl Partikel\\Explosionen, Staub, usw.");
		if ( num == EVENT_INTERFACE_CLIP   )  strcpy(text, "Sichtweite\\Maximale Sichtweite");
		if ( num == EVENT_INTERFACE_DETAIL )  strcpy(text, "Details\\Detailliertheit der Objekte in 3D");
		if ( num == EVENT_INTERFACE_TEXTURE)  strcpy(text, "Qualität der Texturen\\Qualität der Anzeige");
		if ( num == EVENT_INTERFACE_GADGET )  strcpy(text, "Anzahl Ziergegenstände\\Anzahl Gegenstände ohne Funktion");
		if ( num == EVENT_INTERFACE_RAIN   )  strcpy(text, "Partikel in den Menüs\\Funken und Sterne in den Menüs");
		if ( num == EVENT_INTERFACE_GLINT  )  strcpy(text, "Glänzende Tasten\\Glänzende Tasten in den Menüs");
		if ( num == EVENT_INTERFACE_TOOLTIP)  strcpy(text, "Hilfsblasen\\Hilfsblasen");
		if ( num == EVENT_INTERFACE_MOVIES )  strcpy(text, "Filme\\Filme vor und nach den Missionen");
		if ( num == EVENT_INTERFACE_NICERST)  strcpy(text, "Zurücksetzen \\Kleine Show beim Zurücksetzen in den Übungen");
		if ( num == EVENT_INTERFACE_HIMSELF)  strcpy(text, "Eigenbeschuss\\Ihre Einheiten werden von Ihren Waffen beschädigt.");
		if ( num == EVENT_INTERFACE_SCROLL )  strcpy(text, "Kameradrehung mit der Maus\\Die Kamera dreht wenn die Maus den Rand erreicht");
		if ( num == EVENT_INTERFACE_INVERTX)  strcpy(text, "Umkehr X\\Umkehr der Kameradrehung X-Achse");
		if ( num == EVENT_INTERFACE_INVERTY)  strcpy(text, "Umkehr Y\\Umkehr der Kameradrehung Y-Achse");
		if ( num == EVENT_INTERFACE_EFFECT )  strcpy(text, "Beben bei Explosionen\\Die Kamera bebt bei Explosionen");
		if ( num == EVENT_INTERFACE_MOUSE  )  strcpy(text, "Schatten unter der Maus\\Ein Schatten erscheint unter der Maus");
		if ( num == EVENT_INTERFACE_EDITMODE) strcpy(text, "Automatisches Einrücken\\Beim Bearbeiten der Programme");
		if ( num == EVENT_INTERFACE_EDITVALUE)strcpy(text, "Einrücken mit 4 Leerstellen\\Einrücken mit 2 oder 4 Leerstellen");
		if ( num == EVENT_INTERFACE_SOLUCE4)  strcpy(text, "Lösung zugänglich\\Die Lösung ist im Programmslot \"4: Lösung\" zugänglich");

		if ( num == EVENT_INTERFACE_KDEF   )  strcpy(text, "Alles zurücksetzen\\Standarddefinition aller Tasten");
		if ( num == EVENT_INTERFACE_KLEFT  )  strcpy(text, "Drehung nach links\\Steuer links");
		if ( num == EVENT_INTERFACE_KRIGHT )  strcpy(text, "Drehung nach rechts\\Steuer rechts");
		if ( num == EVENT_INTERFACE_KUP    )  strcpy(text, "Vorwärts\\Bewegung nach vorne");
		if ( num == EVENT_INTERFACE_KDOWN  )  strcpy(text, "Rückwärts\\Bewegung nach hinten");
		if ( num == EVENT_INTERFACE_KGUP   )  strcpy(text, "Steigen\\Leistung des Triebwerks steigern");
		if ( num == EVENT_INTERFACE_KGDOWN )  strcpy(text, "Sinken\\Leistung des Triebwerks drosseln");
		if ( num == EVENT_INTERFACE_KCAMERA)  strcpy(text, "Andere Kamera\\Sichtpunkt einstellen");
		if ( num == EVENT_INTERFACE_KDESEL )  strcpy(text, "Vorherg. Auswahl\\Das vorhergehende Objekt auswählen");
		if ( num == EVENT_INTERFACE_KACTION)  strcpy(text, "Standardhandlung\\Führt die Standardhandlung des Roboters aus.");
		if ( num == EVENT_INTERFACE_KNEAR  )  strcpy(text, "Kamera näher\\Bewegung der Kamera vorwärts");
		if ( num == EVENT_INTERFACE_KAWAY  )  strcpy(text, "Kamera weiter\\Bewegung der Kamera rückwärts");
		if ( num == EVENT_INTERFACE_KNEXT  )  strcpy(text, "Nächstes auswählen\\Nächstes Objekt auswählen");
		if ( num == EVENT_INTERFACE_KHUMAN )  strcpy(text, "Astronauten auswählen\\Astronauten auswählen");
		if ( num == EVENT_INTERFACE_KQUIT  )  strcpy(text, "Mission verlassen\\Eine Mission oder Übung verlassen");
		if ( num == EVENT_INTERFACE_KHELP  )  strcpy(text, "Anweisungen\\Anweisungen für die Mission oder Übung");
		if ( num == EVENT_INTERFACE_KPROG  )  strcpy(text, "Hilfe CBOT-Sprache\\Hilfe über die Programmiersprache CBOT");
		if ( num == EVENT_INTERFACE_KCBOT  )  strcpy(text, "Hilfe über Begriff\\Hilfe über einen Begriff");
		if ( num == EVENT_INTERFACE_KVISIT )  strcpy(text, "Ort der Meldung\\Zeigt den Ort, von dem die letzte Meldung stammt");
		if ( num == EVENT_INTERFACE_KSPEED10) strcpy(text, "Geschwindigkeit 1.0x\\Normale Spielgeschwindigkeit");
		if ( num == EVENT_INTERFACE_KSPEED15) strcpy(text, "Geschwindigkeit 1.5x\\Spielgeschwindigkeit anderthalb Mal schneller");
		if ( num == EVENT_INTERFACE_KSPEED20) strcpy(text, "Geschwindigkeit 2.0x\\Spielgeschwindigkeit doppelt so schnell");
		if ( num == EVENT_INTERFACE_KSPEED30) strcpy(text, "Geschwindigkeit 3.0x\\Spielgeschwindigkeit drei Mal schneller");

		if ( num == EVENT_INTERFACE_VOLSOUND) strcpy(text, "Geräusche:\\Lautstärke Motoren, Stimmen, usw.");
		if ( num == EVENT_INTERFACE_VOLMUSIC) strcpy(text, "Geräuschkulisse:\\Lautstärke der Soundtracks der CD");
		if ( num == EVENT_INTERFACE_SOUND3D)  strcpy(text, "3D-Geräusche\\Orten der Geräusche im Raum");

		if ( num == EVENT_INTERFACE_MIN    )  strcpy(text, "Min.\\Minimale Qualität (großes Framerate)");
		if ( num == EVENT_INTERFACE_NORM   )  strcpy(text, "Normal\\Standardqualität");
		if ( num == EVENT_INTERFACE_MAX    )  strcpy(text, "Max.\\Beste Qualität (niedriges Framerate)");

		if ( num == EVENT_INTERFACE_SILENT )  strcpy(text, "Kein Ton\\Keine Geräusche und Geräuschkulisse");
		if ( num == EVENT_INTERFACE_NOISY  )  strcpy(text, "Normal\\Normale Lautstärke");

		if ( num == EVENT_INTERFACE_JOYSTICK) strcpy(text, "Joystick\\Joystick oder Tastatur");
		if ( num == EVENT_INTERFACE_SOLUCE )  strcpy(text, "Zeigt die Lösung\\Zeigt nach 3mal Scheitern die Lösung");

		if ( num == EVENT_INTERFACE_NEDIT  )  strcpy(text, "\\Name des Spielers");
		if ( num == EVENT_INTERFACE_NOK    )  strcpy(text, "OK\\Spieler auswählen");
		if ( num == EVENT_INTERFACE_NCANCEL)  strcpy(text, "Abbrechen\\Behält den bisherigen Spieler bei");
		if ( num == EVENT_INTERFACE_NDELETE)  strcpy(text, "Spieler löschen\\Löscht den Spieler aus der Liste");
		if ( num == EVENT_INTERFACE_NLABEL )  strcpy(text, "Name ");

		if ( num == EVENT_INTERFACE_IOWRITE)  strcpy(text, "Speichern\\Speichert die Mission");
		if ( num == EVENT_INTERFACE_IOREAD )  strcpy(text, "Laden\\Öffnet eine gespeicherte Mission");
		if ( num == EVENT_INTERFACE_IOLIST )  strcpy(text, "Liste der gespeicherten Missionen");
		if ( num == EVENT_INTERFACE_IOLABEL)  strcpy(text, "Dateiname:");
		if ( num == EVENT_INTERFACE_IONAME )  strcpy(text, "Name der Mission");
		if ( num == EVENT_INTERFACE_IOIMAGE)  strcpy(text, "Ansicht der Mission");
		if ( num == EVENT_INTERFACE_IODELETE) strcpy(text, "Löschen\\Löscht die gespeicherte Mission");

		if ( num == EVENT_INTERFACE_PERSO  )  strcpy(text, "Aussehen\\Erscheinungsbild des Astronauten einstellen");
		if ( num == EVENT_INTERFACE_POK    )  strcpy(text, "OK");
		if ( num == EVENT_INTERFACE_PCANCEL)  strcpy(text, "Abbrechen");
		if ( num == EVENT_INTERFACE_PDEF   )  strcpy(text, "Standard\\Standardfarben einsetzen");
		if ( num == EVENT_INTERFACE_PHEAD  )  strcpy(text, "Kopf\\Gesicht und Haare");
		if ( num == EVENT_INTERFACE_PBODY  )  strcpy(text, "Anzug\\Raumfahrtanzug");
		if ( num == EVENT_INTERFACE_PLROT  )  strcpy(text, "\\Drehung links");
		if ( num == EVENT_INTERFACE_PRROT  )  strcpy(text, "\\Drehung rechts");
		if ( num == EVENT_INTERFACE_PCRa   )  strcpy(text, "Rot");
		if ( num == EVENT_INTERFACE_PCGa   )  strcpy(text, "Grün");
		if ( num == EVENT_INTERFACE_PCBa   )  strcpy(text, "Blau");
		if ( num == EVENT_INTERFACE_PCRb   )  strcpy(text, "Rot");
		if ( num == EVENT_INTERFACE_PCGb   )  strcpy(text, "Grün");
		if ( num == EVENT_INTERFACE_PCBb   )  strcpy(text, "Blau");
		if ( num == EVENT_INTERFACE_PFACE1 )  strcpy(text, "\\Kopf 1");
		if ( num == EVENT_INTERFACE_PFACE2 )  strcpy(text, "\\Kopf 4");
		if ( num == EVENT_INTERFACE_PFACE3 )  strcpy(text, "\\Kopf 3");
		if ( num == EVENT_INTERFACE_PFACE4 )  strcpy(text, "\\Kopf 2");
		if ( num == EVENT_INTERFACE_PGLASS0)  strcpy(text, "\\Keine Brille");
		if ( num == EVENT_INTERFACE_PGLASS1)  strcpy(text, "\\Brille 1");
		if ( num == EVENT_INTERFACE_PGLASS2)  strcpy(text, "\\Brille 2");
		if ( num == EVENT_INTERFACE_PGLASS3)  strcpy(text, "\\Brille 3");
		if ( num == EVENT_INTERFACE_PGLASS4)  strcpy(text, "\\Brille 4");
		if ( num == EVENT_INTERFACE_PGLASS5)  strcpy(text, "\\Brille 5");

		if ( num == EVENT_OBJECT_DESELECT  )  strcpy(text, "Vorherg. Auwahl (\\key desel;)");
		if ( num == EVENT_OBJECT_LEFT      )  strcpy(text, "Drehung links (\\key left;)");
		if ( num == EVENT_OBJECT_RIGHT     )  strcpy(text, "Drehung rechts (\\key right;)");
		if ( num == EVENT_OBJECT_UP        )  strcpy(text, "Vorwärts (\\key up;)");
		if ( num == EVENT_OBJECT_DOWN      )  strcpy(text, "Rückwärts (\\key down;)");
		if ( num == EVENT_OBJECT_GASUP     )  strcpy(text, "Steigt (\\key gup;)");
		if ( num == EVENT_OBJECT_GASDOWN   )  strcpy(text, "Sinkt (\\key gdown;)");
		if ( num == EVENT_OBJECT_HTAKE     )  strcpy(text, "Nehmen oder hinlegen  (\\key action;)");
		if ( num == EVENT_OBJECT_MTAKE     )  strcpy(text, "Nehmen oder hinlegen  (\\key action;)");
		if ( num == EVENT_OBJECT_MFRONT    )  strcpy(text, "..vorne");
		if ( num == EVENT_OBJECT_MBACK     )  strcpy(text, "..hinten");
		if ( num == EVENT_OBJECT_MPOWER    )  strcpy(text, "..Batterie");
		if ( num == EVENT_OBJECT_BHELP     )  strcpy(text, "Anweisungen über die Mission(\\key help;)");
		if ( num == EVENT_OBJECT_BTAKEOFF  )  strcpy(text, "Abheben nach vollbrachter Mission");
		if ( num == EVENT_OBJECT_BDERRICK  )  strcpy(text, "Baut einen Bohrturm");
		if ( num == EVENT_OBJECT_BSTATION  )  strcpy(text, "Baut ein Kraftwerk");
		if ( num == EVENT_OBJECT_BFACTORY  )  strcpy(text, "Baut eine Roboterfabrik");
		if ( num == EVENT_OBJECT_BREPAIR   )  strcpy(text, "Baut ein Reparaturzentrum");
		if ( num == EVENT_OBJECT_BCONVERT  )  strcpy(text, "Baut einen Konverter");
		if ( num == EVENT_OBJECT_BTOWER    )  strcpy(text, "Baut einen Geschützturm");
		if ( num == EVENT_OBJECT_BRESEARCH )  strcpy(text, "Baut ein Forschungszentrum");
		if ( num == EVENT_OBJECT_BRADAR    )  strcpy(text, "Baut ein Radar");
		if ( num == EVENT_OBJECT_BENERGY   )  strcpy(text, "Baut eine Batteriefabrik");
		if ( num == EVENT_OBJECT_BLABO     )  strcpy(text, "Baut ein automatisches Labor");
		if ( num == EVENT_OBJECT_BNUCLEAR  )  strcpy(text, "Baut eine Brennstoffzellenfabrik");
		if ( num == EVENT_OBJECT_BPARA     )  strcpy(text, "Baut einen Blitzableiter");
		if ( num == EVENT_OBJECT_BINFO     )  strcpy(text, "Baut einen Infoserver");
		if ( num == EVENT_OBJECT_GFLAT     )  strcpy(text, "Zeigt ob der Boden eben ist");
		if ( num == EVENT_OBJECT_FCREATE   )  strcpy(text, "Setzt eine Fahne");
		if ( num == EVENT_OBJECT_FDELETE   )  strcpy(text, "Sammelt die Fahne ein");
		if ( num == EVENT_OBJECT_FCOLORb   )  strcpy(text, "\\Blaue Fahne");
		if ( num == EVENT_OBJECT_FCOLORr   )  strcpy(text, "\\Rote Fahne");
		if ( num == EVENT_OBJECT_FCOLORg   )  strcpy(text, "\\Grüne Fahne");
		if ( num == EVENT_OBJECT_FCOLORy   )  strcpy(text, "\\Gelbe Fahne");
		if ( num == EVENT_OBJECT_FCOLORv   )  strcpy(text, "\\Violette Fahne");
		if ( num == EVENT_OBJECT_FACTORYfa )  strcpy(text, "Baut einen Jettransporter");
		if ( num == EVENT_OBJECT_FACTORYta )  strcpy(text, "Baut einen Kettentransporter");
		if ( num == EVENT_OBJECT_FACTORYwa )  strcpy(text, "Baut einen Radtransporter");
		if ( num == EVENT_OBJECT_FACTORYia )  strcpy(text, "Baut einen Krabbeltransporter");
		if ( num == EVENT_OBJECT_FACTORYfc )  strcpy(text, "Baut einen Jetshooter");
		if ( num == EVENT_OBJECT_FACTORYtc )  strcpy(text, "Baut einen Kettenshooter");
		if ( num == EVENT_OBJECT_FACTORYwc )  strcpy(text, "Baut einen Radshooter");
		if ( num == EVENT_OBJECT_FACTORYic )  strcpy(text, "Baut einen Krabbelshooter");
		if ( num == EVENT_OBJECT_FACTORYfi )  strcpy(text, "Baut einen Jetorgashooter");
		if ( num == EVENT_OBJECT_FACTORYti )  strcpy(text, "Baut einen Kettenorgashooter");
		if ( num == EVENT_OBJECT_FACTORYwi )  strcpy(text, "Baut einen Radorgashooter");
		if ( num == EVENT_OBJECT_FACTORYii )  strcpy(text, "Baut einen Krabbelorgashooter");
		if ( num == EVENT_OBJECT_FACTORYfs )  strcpy(text, "Baut einen Jetschnüffler");
		if ( num == EVENT_OBJECT_FACTORYts )  strcpy(text, "Baut einen Kettenschnüffler");
		if ( num == EVENT_OBJECT_FACTORYws )  strcpy(text, "Baut einen Radschnüffler");
		if ( num == EVENT_OBJECT_FACTORYis )  strcpy(text, "Baut einen Krabbelschnüffler");
		if ( num == EVENT_OBJECT_FACTORYrt )  strcpy(text, "Baut einen Stampfer");
		if ( num == EVENT_OBJECT_FACTORYrc )  strcpy(text, "Baut einen Phazershooter");
		if ( num == EVENT_OBJECT_FACTORYrr )  strcpy(text, "Baut einen Recycler");
		if ( num == EVENT_OBJECT_FACTORYrs )  strcpy(text, "Baut einen Schutzschild");
		if ( num == EVENT_OBJECT_FACTORYsa )  strcpy(text, "Baut einen Kettentaucher");
		if ( num == EVENT_OBJECT_RTANK     )  strcpy(text, "Forschungsprogramm Kettenantrieb");
		if ( num == EVENT_OBJECT_RFLY      )  strcpy(text, "Forschungsprogramm Jetantrieb");
		if ( num == EVENT_OBJECT_RTHUMP    )  strcpy(text, "Forschungsprogramm Stampfer");
		if ( num == EVENT_OBJECT_RCANON    )  strcpy(text, "Forschungsprogramm Shooterkanone");
		if ( num == EVENT_OBJECT_RTOWER    )  strcpy(text, "Forschungsprogramm Geschützturm");
		if ( num == EVENT_OBJECT_RPHAZER   )  strcpy(text, "Forschungsprogramm Phazerkanone");
		if ( num == EVENT_OBJECT_RSHIELD   )  strcpy(text, "Forschungsprogramm Schutzschild");
		if ( num == EVENT_OBJECT_RATOMIC   )  strcpy(text, "Forschungsprogramm Brennstoffzelle");
		if ( num == EVENT_OBJECT_RiPAW     )  strcpy(text, "Forschungsprogramm Krabbelantrieb");
		if ( num == EVENT_OBJECT_RiGUN     )  strcpy(text, "Forschungsprogramm Orgashooterkanone");
		if ( num == EVENT_OBJECT_RESET     )  strcpy(text, "Alles zurücksetzen");
		if ( num == EVENT_OBJECT_SEARCH    )  strcpy(text, "Schnüffeln (\\key action;)");
		if ( num == EVENT_OBJECT_TERRAFORM )  strcpy(text, "Stampfen (\\key action;)");
		if ( num == EVENT_OBJECT_FIRE      )  strcpy(text, "Feuer (\\key action;)");
		if ( num == EVENT_OBJECT_RECOVER   )  strcpy(text, "Recyceln (\\key action;)");
		if ( num == EVENT_OBJECT_BEGSHIELD )  strcpy(text, "Schutzschild ausfahren (\\key action;)");
		if ( num == EVENT_OBJECT_ENDSHIELD )  strcpy(text, "Schutzschild einholen (\\key action;)");
		if ( num == EVENT_OBJECT_DIMSHIELD )  strcpy(text, "Reichweite Schutzschild");
		if ( num == EVENT_OBJECT_PROGRUN   )  strcpy(text, "Gewähltes Programm ausführen");
		if ( num == EVENT_OBJECT_PROGEDIT  )  strcpy(text, "Gewähltes Programm bearbeiten");
		if ( num == EVENT_OBJECT_INFOOK    )  strcpy(text, "\\SatCom in Standby");
		if ( num == EVENT_OBJECT_DELETE    )  strcpy(text, "Gebäude sprengen");
		if ( num == EVENT_OBJECT_GENERGY   )  strcpy(text, "Energievorrat");
		if ( num == EVENT_OBJECT_GSHIELD   )  strcpy(text, "Schäden");
		if ( num == EVENT_OBJECT_GRANGE    )  strcpy(text, "Triebwerktemperatur");
		if ( num == EVENT_OBJECT_GPROGRESS )  strcpy(text, "Prozess im Gang ...");
		if ( num == EVENT_OBJECT_GRADAR    )  strcpy(text, "Anzahl erfasster Insekten");
		if ( num == EVENT_OBJECT_GINFO     )  strcpy(text, "Gesendete Informationen");
		if ( num == EVENT_OBJECT_COMPASS   )  strcpy(text, "Kompass");
//?		if ( num == EVENT_OBJECT_MAP       )  strcpy(text, "Minikarte");
		if ( num == EVENT_OBJECT_MAPZOOM   )  strcpy(text, "Zoom Minikarte");
		if ( num == EVENT_OBJECT_CAMERA    )  strcpy(text, "Kamera (\\key camera;)");
		if ( num == EVENT_OBJECT_CAMERAleft)  strcpy(text, "Kamera links");
		if ( num == EVENT_OBJECT_CAMERAright) strcpy(text, "Kamera rechts");
		if ( num == EVENT_OBJECT_CAMERAnear)  strcpy(text, "Kamera näher");
		if ( num == EVENT_OBJECT_CAMERAaway)  strcpy(text, "Kamera weiter weg");
		if ( num == EVENT_OBJECT_HELP      )  strcpy(text, "Anweisungen über das ausgewählte Objekt");
		if ( num == EVENT_OBJECT_SOLUCE    )  strcpy(text, "Zeigt die Lösung");
		if ( num == EVENT_OBJECT_SHORTCUT00)  strcpy(text, "Anzeige Roboter <-> Bauten");
		if ( num == EVENT_OBJECT_LIMIT     )  strcpy(text, "Zeigt die Reichweite");
		if ( num == EVENT_OBJECT_PEN0      )  strcpy(text, "\\Bleistift abheben");
		if ( num == EVENT_OBJECT_PEN1      )  strcpy(text, "\\Schwarzen Bleistift hinunterlassen");
		if ( num == EVENT_OBJECT_PEN2      )  strcpy(text, "\\Gelben Bleistift hinunterlassen");
		if ( num == EVENT_OBJECT_PEN3      )  strcpy(text, "\\Orangefarbenen Bleistift hinunterlassen");
		if ( num == EVENT_OBJECT_PEN4      )  strcpy(text, "\\Roten Bleistift hinunterlassen");
		if ( num == EVENT_OBJECT_PEN5      )  strcpy(text, "\\Violetten Bleistift hinunterlassen");
		if ( num == EVENT_OBJECT_PEN6      )  strcpy(text, "\\Blauen Bleistift hinunterlassen");
		if ( num == EVENT_OBJECT_PEN7      )  strcpy(text, "\\Grünen Bleistift hinunterlassen");
		if ( num == EVENT_OBJECT_PEN8      )  strcpy(text, "\\Braunen Bleistift hinunterlassen");
		if ( num == EVENT_OBJECT_REC       )  strcpy(text, "\\Aufnahme starten");
		if ( num == EVENT_OBJECT_STOP      )  strcpy(text, "\\Aufnahme stoppen");
		if ( num == EVENT_DT_VISIT0        ||
			 num == EVENT_DT_VISIT1        ||
			 num == EVENT_DT_VISIT2        ||
			 num == EVENT_DT_VISIT3        ||
			 num == EVENT_DT_VISIT4        )  strcpy(text, "Zeigt den Ort");
		if ( num == EVENT_DT_END           )  strcpy(text, "Weitermachen");
		if ( num == EVENT_CMD              )  strcpy(text, "Befehleingabe");
		if ( num == EVENT_SPEED            )  strcpy(text, "Spielgeschwindigkeit");

		if ( num == EVENT_HYPER_PREV       )  strcpy(text, "Vorherg. Seite");
		if ( num == EVENT_HYPER_NEXT       )  strcpy(text, "Nächste Seite");
		if ( num == EVENT_HYPER_HOME       )  strcpy(text, "Home");
		if ( num == EVENT_HYPER_COPY       )  strcpy(text, "Kopieren");
		if ( num == EVENT_HYPER_SIZE1      )  strcpy(text, "Größe 1");
		if ( num == EVENT_HYPER_SIZE2      )  strcpy(text, "Größe 2");
		if ( num == EVENT_HYPER_SIZE3      )  strcpy(text, "Größe 3");
		if ( num == EVENT_HYPER_SIZE4      )  strcpy(text, "Größe 4");
		if ( num == EVENT_HYPER_SIZE5      )  strcpy(text, "Größe 5");
		if ( num == EVENT_SATCOM_HUSTON    )  strcpy(text, "Anweisungen von Houston");
#if _TEEN
		if ( num == EVENT_SATCOM_SAT       )  strcpy(text, "Wörterbuch Englisch-Deutsch");
#else
		if ( num == EVENT_SATCOM_SAT       )  strcpy(text, "Satellitenbericht");
#endif
		if ( num == EVENT_SATCOM_LOADING   )  strcpy(text, "Von Houston übermittelte Programme");
		if ( num == EVENT_SATCOM_OBJECT    )  strcpy(text, "Liste der Objekte");
		if ( num == EVENT_SATCOM_PROG      )  strcpy(text, "Hilfe über Programmieren");
		if ( num == EVENT_SATCOM_SOLUCE    )  strcpy(text, "Lösung");

		if ( num == EVENT_STUDIO_OK        )  strcpy(text, "OK\\Programm kompilieren");
		if ( num == EVENT_STUDIO_CANCEL    )  strcpy(text, "Abbrechen\\Editor schließen");
		if ( num == EVENT_STUDIO_NEW       )  strcpy(text, "Neu");
		if ( num == EVENT_STUDIO_OPEN      )  strcpy(text, "Öffnen (Ctrl+o)");
		if ( num == EVENT_STUDIO_SAVE      )  strcpy(text, "Speichern (Ctrl+s)");
		if ( num == EVENT_STUDIO_UNDO      )  strcpy(text, "Widerrufen (Ctrl+z)");
		if ( num == EVENT_STUDIO_CUT       )  strcpy(text, "Ausschneiden (Ctrl+x)");
		if ( num == EVENT_STUDIO_COPY      )  strcpy(text, "Kopieren (Ctrl+c)");
		if ( num == EVENT_STUDIO_PASTE     )  strcpy(text, "Einfügen (Ctrl+v)");
		if ( num == EVENT_STUDIO_SIZE      )  strcpy(text, "Zeichengröße");
		if ( num == EVENT_STUDIO_TOOL      )  strcpy(text, "Anweisungen (\\key help;)");
		if ( num == EVENT_STUDIO_HELP      )  strcpy(text, "Hilfe über Programmieren (\\key prog;)");
		if ( num == EVENT_STUDIO_COMPILE   )  strcpy(text, "Kompilieren");
		if ( num == EVENT_STUDIO_RUN       )  strcpy(text, "Start/Stop");
		if ( num == EVENT_STUDIO_REALTIME  )  strcpy(text, "Pause/Weitermachen");
		if ( num == EVENT_STUDIO_STEP      )  strcpy(text, "Ein Schritt");
	}

	if ( type == RES_OBJECT )
	{
		if ( num == OBJECT_PORTICO      )  strcpy(text, "Träger");
		if ( num == OBJECT_BASE         )  strcpy(text, "Raumschiff");
		if ( num == OBJECT_DERRICK      )  strcpy(text, "Bohrturm");
		if ( num == OBJECT_FACTORY      )  strcpy(text, "Roboterfabrik");
		if ( num == OBJECT_REPAIR       )  strcpy(text, "Reparaturzentrum");
		if ( num == OBJECT_DESTROYER    )  strcpy(text, "Einstampfer");
		if ( num == OBJECT_STATION      )  strcpy(text, "Kraftwerk");
		if ( num == OBJECT_CONVERT      )  strcpy(text, "Konverter Erz-Titan");
		if ( num == OBJECT_TOWER        )  strcpy(text, "Geschützturm");
		if ( num == OBJECT_NEST         )  strcpy(text, "Orgastoffquelle");
		if ( num == OBJECT_RESEARCH     )  strcpy(text, "Forschungszentrum");
		if ( num == OBJECT_RADAR        )  strcpy(text, "Radar");
		if ( num == OBJECT_INFO         )  strcpy(text, "Infoserver");
#if _TEEN
		if ( num == OBJECT_ENERGY       )  strcpy(text, "Auflöser");
#else
		if ( num == OBJECT_ENERGY       )  strcpy(text, "Batteriefabrik");
#endif
		if ( num == OBJECT_LABO         )  strcpy(text, "Automatisches Labor");
		if ( num == OBJECT_NUCLEAR      )  strcpy(text, "Brennstoffzellenfabrik");
		if ( num == OBJECT_PARA         )  strcpy(text, "Blitzableiter");
		if ( num == OBJECT_SAFE         )  strcpy(text, "Bunker");
		if ( num == OBJECT_HUSTON       )  strcpy(text, "Kontrollzentrum");
		if ( num == OBJECT_TARGET1      )  strcpy(text, "Zielscheibe");
		if ( num == OBJECT_TARGET2      )  strcpy(text, "Zielscheibe");
		if ( num == OBJECT_START        )  strcpy(text, "Startfläche");
		if ( num == OBJECT_END          )  strcpy(text, "Zielfläche");
		if ( num == OBJECT_STONE        )  strcpy(text, "Titanerz");
		if ( num == OBJECT_URANIUM      )  strcpy(text, "Platinerz");
		if ( num == OBJECT_BULLET       )  strcpy(text, "Orgastoff");
		if ( num == OBJECT_METAL        )  strcpy(text, "Titan");
		if ( num == OBJECT_POWER        )  strcpy(text, "Elektrolytische Batterie");
		if ( num == OBJECT_ATOMIC       )  strcpy(text, "Brennstoffzelle");
		if ( num == OBJECT_BBOX         )  strcpy(text, "Flugschreiber");
		if ( num == OBJECT_KEYa         )  strcpy(text, "Schlüssel A");
		if ( num == OBJECT_KEYb         )  strcpy(text, "Schlüssel B");
		if ( num == OBJECT_KEYc         )  strcpy(text, "Schlüssel C");
		if ( num == OBJECT_KEYd         )  strcpy(text, "Schlüssel D");
		if ( num == OBJECT_TNT          )  strcpy(text, "Sprengstoff");
		if ( num == OBJECT_BOMB         )  strcpy(text, "Landmine");
		if ( num == OBJECT_BAG          )  strcpy(text, "Überlebenskit");
		if ( num == OBJECT_WAYPOINT     )  strcpy(text, "Checkpoint");
		if ( num == OBJECT_FLAGb        )  strcpy(text, "Blaue Fahne");
		if ( num == OBJECT_FLAGr        )  strcpy(text, "Rote Fahne");
		if ( num == OBJECT_FLAGg        )  strcpy(text, "Grüne Fahne");
		if ( num == OBJECT_FLAGy        )  strcpy(text, "Gelbe Fahne");
		if ( num == OBJECT_FLAGv        )  strcpy(text, "Violette Fahne");
		if ( num == OBJECT_MARKPOWER    )  strcpy(text, "Markierung für unterirdische Energiequelle");
		if ( num == OBJECT_MARKURANIUM  )  strcpy(text, "Markierung für unterirdisches Platinvorkommen");
		if ( num == OBJECT_MARKKEYa     )  strcpy(text, "Markierung für vergrabenen Schlüssel A");
		if ( num == OBJECT_MARKKEYb     )  strcpy(text, "Markierung für vergrabenen Schlüssel B");
		if ( num == OBJECT_MARKKEYc     )  strcpy(text, "Markierung für vergrabenen Schlüssel C");
		if ( num == OBJECT_MARKKEYd     )  strcpy(text, "Markierung für vergrabenen Schlüssel D");
		if ( num == OBJECT_MARKSTONE    )  strcpy(text, "Markierung für unterirdisches Titanvorkommen");
		if ( num == OBJECT_MOBILEft     )  strcpy(text, "Übungsroboter");
		if ( num == OBJECT_MOBILEtt     )  strcpy(text, "Übungsroboter");
		if ( num == OBJECT_MOBILEwt     )  strcpy(text, "Übungsroboter");
		if ( num == OBJECT_MOBILEit     )  strcpy(text, "Übungsroboter");
		if ( num == OBJECT_MOBILEfa     )  strcpy(text, "Transporter");
		if ( num == OBJECT_MOBILEta     )  strcpy(text, "Transporter");
		if ( num == OBJECT_MOBILEwa     )  strcpy(text, "Transporter");
		if ( num == OBJECT_MOBILEia     )  strcpy(text, "Transporter");
		if ( num == OBJECT_MOBILEfc     )  strcpy(text, "Shooter");
		if ( num == OBJECT_MOBILEtc     )  strcpy(text, "Shooter");
		if ( num == OBJECT_MOBILEwc     )  strcpy(text, "Shooter");
		if ( num == OBJECT_MOBILEic     )  strcpy(text, "Shooter");
		if ( num == OBJECT_MOBILEfi     )  strcpy(text, "OrgaShooter");
		if ( num == OBJECT_MOBILEti     )  strcpy(text, "OrgaShooter");
		if ( num == OBJECT_MOBILEwi     )  strcpy(text, "OrgaShooter");
		if ( num == OBJECT_MOBILEii     )  strcpy(text, "OrgaShooter");
		if ( num == OBJECT_MOBILEfs     )  strcpy(text, "Schnüffler");
		if ( num == OBJECT_MOBILEts     )  strcpy(text, "Schnüffler");
		if ( num == OBJECT_MOBILEws     )  strcpy(text, "Schnüffler");
		if ( num == OBJECT_MOBILEis     )  strcpy(text, "Schnüffler");
		if ( num == OBJECT_MOBILErt     )  strcpy(text, "Stampfer");
		if ( num == OBJECT_MOBILErc     )  strcpy(text, "Phazershooter");
		if ( num == OBJECT_MOBILErr     )  strcpy(text, "Recycler");
		if ( num == OBJECT_MOBILErs     )  strcpy(text, "Schutzschild");
		if ( num == OBJECT_MOBILEsa     )  strcpy(text, "Kettentaucher");
		if ( num == OBJECT_MOBILEtg     )  strcpy(text, "Mobile Zielscheibe");
		if ( num == OBJECT_MOBILEdr     )  strcpy(text, "Zeichner");
		if ( num == OBJECT_HUMAN        )  strcpy(text, g_gamerName);
		if ( num == OBJECT_TECH         )  strcpy(text, "Techniker");
		if ( num == OBJECT_TOTO         )  strcpy(text, "Robby");
		if ( num == OBJECT_MOTHER       )  strcpy(text, "Insektenkönigin");
		if ( num == OBJECT_ANT          )  strcpy(text, "Ameise");
		if ( num == OBJECT_SPIDER       )  strcpy(text, "Spinne");
		if ( num == OBJECT_BEE          )  strcpy(text, "Wespe");
		if ( num == OBJECT_WORM         )  strcpy(text, "Wurm");
		if ( num == OBJECT_EGG          )  strcpy(text, "Ei");
		if ( num == OBJECT_RUINmobilew1 )  strcpy(text, "Roboterwrack");
		if ( num == OBJECT_RUINmobilew2 )  strcpy(text, "Roboterwrack");
		if ( num == OBJECT_RUINmobilet1 )  strcpy(text, "Roboterwrack");
		if ( num == OBJECT_RUINmobilet2 )  strcpy(text, "Roboterwrack");
		if ( num == OBJECT_RUINmobiler1 )  strcpy(text, "Roboterwrack");
		if ( num == OBJECT_RUINmobiler2 )  strcpy(text, "Roboterwrack");
		if ( num == OBJECT_RUINfactory  )  strcpy(text, "Gebäuderuine");
		if ( num == OBJECT_RUINdoor     )  strcpy(text, "Gebäuderuine");
		if ( num == OBJECT_RUINsupport  )  strcpy(text, "Abfall");
		if ( num == OBJECT_RUINradar    )  strcpy(text, "Gebäuderuine");
		if ( num == OBJECT_RUINconvert  )  strcpy(text, "Gebäuderuine");
		if ( num == OBJECT_RUINbase     )  strcpy(text, "Raumschiffruine");
		if ( num == OBJECT_RUINhead     )  strcpy(text, "Raumschiffruine");
		if ( num == OBJECT_APOLLO1      ||
			 num == OBJECT_APOLLO3      ||
			 num == OBJECT_APOLLO4      ||
			 num == OBJECT_APOLLO5      )  strcpy(text, "Überreste einer Apollo-Mission");
		if ( num == OBJECT_APOLLO2      )  strcpy(text, "Lunar Roving Vehicle");
	}

	if ( type == RES_ERR )
	{
		strcpy(text, "Fehler");
		if ( num == ERR_CMD             )  strcpy(text, "Befehl unbekannt");
#if _NEWLOOK
		if ( num == ERR_INSTALL         )  strcpy(text, "CeeBot wurde nicht installiert.");
		if ( num == ERR_NOCD            )  strcpy(text, "Legen Sie die CeeBot-CD ein\nund starten Sie das Spiel neu.");
#else
		if ( num == ERR_INSTALL         )  strcpy(text, "COLOBOT wurde nicht installiert.");
		if ( num == ERR_NOCD            )  strcpy(text, "Legen Sie die COLOBOT-CD ein\nund starten Sie das Spiel neu.");
#endif
		if ( num == ERR_MANIP_VEH       )  strcpy(text, "Roboter ungeeignet");
		if ( num == ERR_MANIP_FLY       )  strcpy(text, "Im Flug unmöglich");
		if ( num == ERR_MANIP_BUSY      )  strcpy(text, "Trägt schon etwas");
		if ( num == ERR_MANIP_NIL       )  strcpy(text, "Nichts zu ergreifen");
		if ( num == ERR_MANIP_MOTOR     )  strcpy(text, "In Fahrt unmöglich");
		if ( num == ERR_MANIP_OCC       )  strcpy(text, "Stelle schon besetzt");
		if ( num == ERR_MANIP_FRIEND    )  strcpy(text, "Kein anderer Roboter");
		if ( num == ERR_MANIP_RADIO     )  strcpy(text, "Sie können keinen radioaktiven Gegenstand tragen");
		if ( num == ERR_MANIP_WATER     )  strcpy(text, "Sie können unter Wasser nichts tragen");
		if ( num == ERR_MANIP_EMPTY     )  strcpy(text, "Nichts abzulegen");
		if ( num == ERR_BUILD_FLY       )  strcpy(text, "Im Flug unmöglich");
		if ( num == ERR_BUILD_WATER     )  strcpy(text, "Unter Wasser unmöglich");
		if ( num == ERR_BUILD_ENERGY    )  strcpy(text, "Nicht genug Energie");
		if ( num == ERR_BUILD_METALAWAY )  strcpy(text, "Titan zu weit weg");
		if ( num == ERR_BUILD_METALNEAR )  strcpy(text, "Titan zu nahe");
		if ( num == ERR_BUILD_METALINEX )  strcpy(text, "Kein Titan vorhanden");
		if ( num == ERR_BUILD_FLAT      )  strcpy(text, "Boden nicht eben genug");
		if ( num == ERR_BUILD_FLATLIT   )  strcpy(text, "Ebener Boden nicht groß genug");
		if ( num == ERR_BUILD_BUSY      )  strcpy(text, "Stelle schon besetzt");
		if ( num == ERR_BUILD_BASE      )  strcpy(text, "Zu nahe am Raumschiff");
		if ( num == ERR_BUILD_NARROW    )  strcpy(text, "Zu nahe an einem Gebäude");
		if ( num == ERR_BUILD_MOTOR     )  strcpy(text, "In Fahrt unmöglich");
		if ( num == ERR_SEARCH_FLY      )  strcpy(text, "Im Flug unmöglich");
		if ( num == ERR_SEARCH_VEH      )  strcpy(text, "Roboter ungeeignet");
		if ( num == ERR_SEARCH_MOTOR    )  strcpy(text, "In Fahrt unmöglich");
		if ( num == ERR_TERRA_VEH       )  strcpy(text, "Roboter ungeeignet");
		if ( num == ERR_TERRA_ENERGY    )  strcpy(text, "Nicht genug Energie");
		if ( num == ERR_TERRA_FLOOR     )  strcpy(text, "Boden ungeeignet");
		if ( num == ERR_TERRA_BUILDING  )  strcpy(text, "Gebäude zu nahe");
		if ( num == ERR_TERRA_OBJECT    )  strcpy(text, "Gegenstand zu nahe");
		if ( num == ERR_RECOVER_VEH     )  strcpy(text, "Roboter ungeeignet");
		if ( num == ERR_RECOVER_ENERGY  )  strcpy(text, "Nicht genug Energie");
		if ( num == ERR_RECOVER_NULL    )  strcpy(text, "Nichts zu recyceln");
		if ( num == ERR_SHIELD_VEH      )  strcpy(text, "Roboter ungeeignet");
		if ( num == ERR_SHIELD_ENERGY   )  strcpy(text, "Keine Energie mehr");
//?		if ( num == ERR_COM             )  strcpy(text, "Kommunikationsproblem mit dem Roboter");
		if ( num == ERR_MOVE_IMPOSSIBLE )  strcpy(text, "Ziel kann nicht erreicht werden");
		if ( num == ERR_FIND_IMPOSSIBLE )  strcpy(text, "Das Objekt existiert nicht");
		if ( num == ERR_GOTO_IMPOSSIBLE )  strcpy(text, "Ziel kann nicht erreicht werden");
		if ( num == ERR_GOTO_ITER       )  strcpy(text, "Ziel kann nicht erreicht werden");
		if ( num == ERR_GOTO_BUSY       )  strcpy(text, "Ziel ist schon besetzt");
		if ( num == ERR_FIRE_VEH        )  strcpy(text, "Roboter ungeeignet");
		if ( num == ERR_FIRE_ENERGY     )  strcpy(text, "Nicht genug Energie");
		if ( num == ERR_FIRE_FLY        )  strcpy(text, "Im Flug unmöglich");
		if ( num == ERR_CONVERT_EMPTY   )  strcpy(text, "Kein konvertierbares Titanerz vorhanden");
		if ( num == ERR_DERRICK_NULL    )  strcpy(text, "Keine unterirdische Erzlagerstätte");
		if ( num == ERR_STATION_NULL    )  strcpy(text, "Kein unterirdisches Energievorkommen");
		if ( num == ERR_TOWER_POWER     )  strcpy(text, "Keine Batterie");
		if ( num == ERR_TOWER_ENERGY    )  strcpy(text, "Keine Energie mehr");
		if ( num == ERR_RESEARCH_POWER  )  strcpy(text, "Keine Batterie");
		if ( num == ERR_RESEARCH_ENERGY )  strcpy(text, "Nicht mehr genug Energie");
		if ( num == ERR_RESEARCH_TYPE   )  strcpy(text, "Falscher Batterietyp");
		if ( num == ERR_RESEARCH_ALREADY)  strcpy(text, "Forschungsprogramm schon ausgeführt");
		if ( num == ERR_ENERGY_NULL     )  strcpy(text, "Kein unterirdisches Energievorkommen");
		if ( num == ERR_ENERGY_LOW      )  strcpy(text, "Noch nicht genug Energie");
		if ( num == ERR_ENERGY_EMPTY    )  strcpy(text, "Kein konvertierbares Titanerz vorhanden");
		if ( num == ERR_ENERGY_BAD      )  strcpy(text, "Wandelt nur Titanerz um");
		if ( num == ERR_BASE_DLOCK      )  strcpy(text, "Die Türen werden von einem Gegenstand blockiert");
		if ( num == ERR_BASE_DHUMAN     )  strcpy(text, "Gehen Sie an Bord, bevor Sie abheben");
		if ( num == ERR_LABO_NULL       )  strcpy(text, "Nichts zu analysieren");
		if ( num == ERR_LABO_BAD        )  strcpy(text, "Analysiert nur Orgastoff");
		if ( num == ERR_LABO_ALREADY    )  strcpy(text, "Analyse schon durchgeführt");
		if ( num == ERR_NUCLEAR_NULL    )  strcpy(text, "Kein unterirdisches Energievorkommen");
		if ( num == ERR_NUCLEAR_LOW     )  strcpy(text, "Noch nicht genug Energie");
		if ( num == ERR_NUCLEAR_EMPTY   )  strcpy(text, "Kein konvertierbares Platin");
		if ( num == ERR_NUCLEAR_BAD     )  strcpy(text, "Wandelt nur Platin um");
		if ( num == ERR_FACTORY_NULL    )  strcpy(text, "Kein Titan vorhanden");
		if ( num == ERR_FACTORY_NEAR    )  strcpy(text, "Ein Gegenstand ist zu nahe");
		if ( num == ERR_RESET_NEAR      )  strcpy(text, "Stelle schon besetzt");
		if ( num == ERR_INFO_NULL       )  strcpy(text, "Kein Infoserver in Reichweite");
		if ( num == ERR_VEH_VIRUS       )  strcpy(text, "Ein Programm wurde von einem Virus infiziert");
		if ( num == ERR_BAT_VIRUS       )  strcpy(text, "Von Virus infiziert, zeitweise außer Betrieb");
		if ( num == ERR_VEH_POWER       )  strcpy(text, "Keine Batterie");
		if ( num == ERR_VEH_ENERGY      )  strcpy(text, "Keine Energie mehr");
		if ( num == ERR_FLAG_FLY        )  strcpy(text, "Im Flug unmöglich");
		if ( num == ERR_FLAG_WATER      )  strcpy(text, "Im Wasser unmöglich");
		if ( num == ERR_FLAG_MOTOR      )  strcpy(text, "Beim Gehen unmöglich");
		if ( num == ERR_FLAG_BUSY       )  strcpy(text, "Unmöglich wenn Sie etwas tragen");
		if ( num == ERR_FLAG_CREATE     )  strcpy(text, "Zu viele Fahnen dieser Farbe (Maximum 5)");
		if ( num == ERR_FLAG_PROXY      )  strcpy(text, "Zu nahe an einer anderen Fahne");
		if ( num == ERR_FLAG_DELETE     )  strcpy(text, "Keine Fahne in Reichweite");
		if ( num == ERR_MISSION_NOTERM  )  strcpy(text, "Mission noch nicht beendet (Drücken Sie auf \\key help; für weitere Informationen)");
		if ( num == ERR_DELETEMOBILE    )  strcpy(text, "Roboter zerstört");
		if ( num == ERR_DELETEBUILDING  )  strcpy(text, "Gebäude zerstört");
		if ( num == ERR_TOOMANY         )  strcpy(text, "Kein neues Objekt kann erstellt werden (zu viele vorhanden)");
		if ( num == ERR_OBLIGATORYTOKEN )  strcpy(text, "Es fehlt \"%s\" in Ihrem Programm");
		if ( num == ERR_PROHIBITEDTOKEN )  strcpy(text, "In dieser Übung verboten");

		if ( num == INFO_BUILD          )  strcpy(text, "Gebäude fertiggestellt");
		if ( num == INFO_CONVERT        )  strcpy(text, "Titan verfügbar");
		if ( num == INFO_RESEARCH       )  strcpy(text, "Forschungsprogramm abgeschlossen");
		if ( num == INFO_RESEARCHTANK   )  strcpy(text, "Herstellung eines Roboters mit Kettenantrieb möglich");
		if ( num == INFO_RESEARCHFLY    )  strcpy(text, "Sie können jetzt mit den Tasten \\key gup; und \\key gdown; fliegen");
		if ( num == INFO_RESEARCHTHUMP  )  strcpy(text, "Herstellung eines Stampfers möglich");
		if ( num == INFO_RESEARCHCANON  )  strcpy(text, "Herstellung eines Shooters möglich");
		if ( num == INFO_RESEARCHTOWER  )  strcpy(text, "Errichtung eines Geschützturms möglich");
		if ( num == INFO_RESEARCHPHAZER )  strcpy(text, "Herstellung eines Phazershooters möglich");
		if ( num == INFO_RESEARCHSHIELD )  strcpy(text, "Herstellung eines Schutzschildes möglich");
		if ( num == INFO_RESEARCHATOMIC )  strcpy(text, "Errichtung einer Brennstoffzellenfabrik möglich");
		if ( num == INFO_FACTORY        )  strcpy(text, "Neuer Roboter verfügbar");
		if ( num == INFO_LABO           )  strcpy(text, "Analyse vollendet");
		if ( num == INFO_ENERGY         )  strcpy(text, "Batterie verfügbar");
		if ( num == INFO_NUCLEAR        )  strcpy(text, "Brennstoffzelle verfügbar");
		if ( num == INFO_FINDING        )  strcpy(text, "Sie haben ein brauchbares Objekt gefunden");
		if ( num == INFO_MARKPOWER      )  strcpy(text, "Geeignete Stelle für Kraftwerk gefunden");
		if ( num == INFO_MARKURANIUM    )  strcpy(text, "Geeignete Stelle für Bohrturm gefunden");
		if ( num == INFO_MARKSTONE      )  strcpy(text, "Geeignete Stelle für Bohrturm gefunden");
		if ( num == INFO_MARKKEYa       )  strcpy(text, "Geeignete Stelle für Bohrturm gefunden");
		if ( num == INFO_MARKKEYb       )  strcpy(text, "Geeignete Stelle für Bohrturm gefunden");
		if ( num == INFO_MARKKEYc       )  strcpy(text, "Geeignete Stelle für Bohrturm gefunden");
		if ( num == INFO_MARKKEYd       )  strcpy(text, "Geeignete Stelle für Bohrturm gefunden");
		if ( num == INFO_WIN            )  strcpy(text, "<<< Bravo, Mission vollendet >>>");
		if ( num == INFO_LOST           )  strcpy(text, "<<< Mission gescheitert >>>");
		if ( num == INFO_LOSTq          )  strcpy(text, "<<< Mission gescheitert >>>");
		if ( num == INFO_WRITEOK        )  strcpy(text, "Mission gespeichert");
		if ( num == INFO_DELETEPATH     )  strcpy(text, "Checkpoint erreicht");
		if ( num == INFO_DELETEMOTHER   )  strcpy(text, "Insektenkönigin tödlich verwundet");
		if ( num == INFO_DELETEANT      )  strcpy(text, "Ameise tödlich verwundet");
		if ( num == INFO_DELETEBEE      )  strcpy(text, "Wespe tödlich verwundet");
		if ( num == INFO_DELETEWORM     )  strcpy(text, "Wurm tödlich verwundet");
		if ( num == INFO_DELETESPIDER   )  strcpy(text, "Spinne tödlich verwundet");
		if ( num == INFO_BEGINSATCOM    )  strcpy(text, "Beziehen Sie sich auf Ihren SatCom, indem Sie auf \\key help; drücken");
	}

	if ( type == RES_CBOT )
	{
		strcpy(text, "Fehler");
		if ( num == TX_OPENPAR       ) strcpy(text, "Es fehlt eine offene Klammer ""(""");
		if ( num == TX_CLOSEPAR      ) strcpy(text, "Es fehlt eine geschlossene Klammer "")""");
		if ( num == TX_NOTBOOL       ) strcpy(text, "Der Ausdruck muss einen boolschen Wert ergeben");
		if ( num == TX_UNDEFVAR      ) strcpy(text, "Variable nicht deklariert");
		if ( num == TX_BADLEFT       ) strcpy(text, "Zuweisung unmöglich");
		if ( num == TX_ENDOF         ) strcpy(text, "Es fehlt ein Strichpunkt "";"" am Ende der Anweisung");
		if ( num == TX_OUTCASE       ) strcpy(text, "Anweisung ""case"" ohne vorhergehende Anweisung ""switch""");
		if ( num == TX_NOTERM        ) strcpy(text, "Hier ist eine Anweisung nach dem Ende des Programms");
		if ( num == TX_CLOSEBLK      ) strcpy(text, "Es fehlt eine geschlossene geschweifte Klammer ""}"" (Ende des Blocks)");
		if ( num == TX_ELSEWITHOUTIF ) strcpy(text, "Anweisung ""else"" ohne vorhergehende Anweisung ""if""");
		if ( num == TX_OPENBLK       ) strcpy(text, "Es fehlt eine offene geschweifte Klammer""{""");
		if ( num == TX_BADTYPE       ) strcpy(text, "Der Ausdruck ergibt einen falschen Typ für die Zuweisung");
		if ( num == TX_REDEFVAR      ) strcpy(text, "Eine Variable wird zum zweiten Mal deklariert");
		if ( num == TX_BAD2TYPE      ) strcpy(text, "Die zwei Operanden sind nicht kompatibel");
		if ( num == TX_UNDEFCALL     ) strcpy(text, "Unbekannte Funktion");
		if ( num == TX_MISDOTS       ) strcpy(text, "Es fehlt ein Doppelpunkt "" : """);
		if ( num == TX_WHILE         ) strcpy(text, "Es fehlt das Wort ""while""");
		if ( num == TX_BREAK         ) strcpy(text, "Anweisung ""break"" außerhalb einer Schleife");
		if ( num == TX_LABEL         ) strcpy(text, "Ein Label kann nur vor den Anweisungen ""for"", ""while"", ""do"" oder ""switch"" vorkommen");
		if ( num == TX_NOLABEL       ) strcpy(text, "Dieses Label existiert nicht");
		if ( num == TX_NOCASE        ) strcpy(text, "Es fehlt eine Anweisung ""case""");
		if ( num == TX_BADNUM        ) strcpy(text, "Es fehlt eine Zahl");
		if ( num == TX_VOID          ) strcpy(text, "Parameter void");
		if ( num == TX_NOTYP         ) strcpy(text, "Hier muss ein Variablentyp stehen");
		if ( num == TX_NOVAR         ) strcpy(text, "Es fehlt der Name einer Variable");
		if ( num == TX_NOFONC        ) strcpy(text, "Hier muss der Name der Funktion stehen");
		if ( num == TX_OVERPARAM     ) strcpy(text, "Zu viele Parameter");
		if ( num == TX_REDEF         ) strcpy(text, "Diese Funktion gibt es schon");
		if ( num == TX_LOWPARAM      ) strcpy(text, "Nicht genug Parameter");
		if ( num == TX_BADPARAM      ) strcpy(text, "Keine Funktion mit diesem Namen verträgt Parameter diesen Typs");
		if ( num == TX_NUMPARAM      ) strcpy(text, "Keine Funktion mit diesem Namen verträgt diese Anzahl Parameter");
		if ( num == TX_NOITEM        ) strcpy(text, "Dieses Element gibt es nicht in dieser Klasse");
		if ( num == TX_DOT           ) strcpy(text, "Das Objekt ist nicht eine Instanz einer Klasse");
		if ( num == TX_NOCONST       ) strcpy(text, "Es gibt keinen geeigneten Konstruktor");
		if ( num == TX_REDEFCLASS    ) strcpy(text, "Diese Klasse gibt es schon");
		if ( num == TX_CLBRK         ) strcpy(text, "Es fehlt eine geschlossene eckige Klammer "" ] """);
		if ( num == TX_RESERVED      ) strcpy(text, "Dieses Wort ist reserviert");
		if ( num == TX_BADNEW        ) strcpy(text, "Falsche Argumente für ""new""");
		if ( num == TX_OPBRK         ) strcpy(text, "Es fehlt eine offene eckige Klammer "" [ """);
		if ( num == TX_BADSTRING     ) strcpy(text, "Hier wird eine Zeichenkette erwartet");
		if ( num == TX_BADINDEX      ) strcpy(text, "Falscher Typ für einen Index");
		if ( num == TX_PRIVATE       ) strcpy(text, "Geschütztes Element (private)");
		if ( num == TX_NOPUBLIC      ) strcpy(text, "Hier muss das Wort ""public"" stehen");
		if ( num == TX_DIVZERO       ) strcpy(text, "Teilung durch Null");
		if ( num == TX_NOTINIT       ) strcpy(text, "Der Wert dieser Variable wurde nicht definiert");
		if ( num == TX_BADTHROW      ) strcpy(text, "Negativer Wert ungeeignet für Anweisung ""throw""");
		if ( num == TX_NORETVAL      ) strcpy(text, "Die Funktion hat kein Ergebnis zurückgegeben");
		if ( num == TX_NORUN         ) strcpy(text, "Keine Funktion wird ausgeführt");
		if ( num == TX_NOCALL        ) strcpy(text, "Die aufgerufene Funktion existiert nicht");
		if ( num == TX_NOCLASS       ) strcpy(text, "Diese Klasse existiert nicht");
		if ( num == TX_NULLPT        ) strcpy(text, "Das Objekt existiert nicht");
		if ( num == TX_OPNAN         ) strcpy(text, "Operation mit dem Wert ""nan""");
		if ( num == TX_OUTARRAY      ) strcpy(text, "Zugriff im Array außerhalb der Grenzen");
		if ( num == TX_STACKOVER     ) strcpy(text, "Stack overflow");
		if ( num == TX_DELETEDPT     ) strcpy(text, "Objekt nicht verfügbar");
		if ( num == TX_FILEOPEN      ) strcpy(text, "Die Datei kann nicht geöffnet werden");
		if ( num == TX_NOTOPEN       ) strcpy(text, "Die Datei wurde nicht geöffnet");
		if ( num == TX_ERRREAD       ) strcpy(text, "Fehler beim Lesezugriff");
		if ( num == TX_ERRWRITE      ) strcpy(text, "Fehler beim Schreibzugriff");
	}

	if ( type == RES_KEY )
	{
		if ( num == 0                   )  strcpy(text, "< keine >");
		if ( num == VK_LEFT             )  strcpy(text, "Pfeiltaste links");
		if ( num == VK_RIGHT            )  strcpy(text, "Pfeiltaste rechts");
		if ( num == VK_UP               )  strcpy(text, "Pfeil nach oben");
		if ( num == VK_DOWN             )  strcpy(text, "Pfeil nach unten");
		if ( num == VK_CANCEL           )  strcpy(text, "Ctrl-Break");
		if ( num == VK_BACK             )  strcpy(text, "<--");
		if ( num == VK_TAB              )  strcpy(text, "Tab");
		if ( num == VK_CLEAR            )  strcpy(text, "Clear");
		if ( num == VK_RETURN           )  strcpy(text, "Eingabe");
		if ( num == VK_SHIFT            )  strcpy(text, "Shift");
		if ( num == VK_CONTROL          )  strcpy(text, "Ctrl");
		if ( num == VK_MENU             )  strcpy(text, "Alt");
		if ( num == VK_PAUSE            )  strcpy(text, "Pause");
		if ( num == VK_CAPITAL          )  strcpy(text, "Caps Lock");
		if ( num == VK_ESCAPE           )  strcpy(text, "Esc");
		if ( num == VK_SPACE            )  strcpy(text, "Leertaste");
		if ( num == VK_PRIOR            )  strcpy(text, "Page Up");
		if ( num == VK_NEXT             )  strcpy(text, "Page Down");
		if ( num == VK_END              )  strcpy(text, "End");
		if ( num == VK_HOME             )  strcpy(text, "Home");
		if ( num == VK_SELECT           )  strcpy(text, "Select");
		if ( num == VK_EXECUTE          )  strcpy(text, "Execute");
		if ( num == VK_SNAPSHOT         )  strcpy(text, "Print Scrn");
		if ( num == VK_INSERT           )  strcpy(text, "Insert");
		if ( num == VK_DELETE           )  strcpy(text, "Delete");
		if ( num == VK_HELP             )  strcpy(text, "Help");
		if ( num == VK_LWIN             )  strcpy(text, "Left Windows");
		if ( num == VK_RWIN             )  strcpy(text, "Right Windows");
		if ( num == VK_APPS             )  strcpy(text, "Application key");
		if ( num == VK_NUMPAD0          )  strcpy(text, "NumPad 0");
		if ( num == VK_NUMPAD1          )  strcpy(text, "NumPad 1");
		if ( num == VK_NUMPAD2          )  strcpy(text, "NumPad 2");
		if ( num == VK_NUMPAD3          )  strcpy(text, "NumPad 3");
		if ( num == VK_NUMPAD4          )  strcpy(text, "NumPad 4");
		if ( num == VK_NUMPAD5          )  strcpy(text, "NumPad 5");
		if ( num == VK_NUMPAD6          )  strcpy(text, "NumPad 6");
		if ( num == VK_NUMPAD7          )  strcpy(text, "NumPad 7");
		if ( num == VK_NUMPAD8          )  strcpy(text, "NumPad 8");
		if ( num == VK_NUMPAD9          )  strcpy(text, "NumPad 9");
		if ( num == VK_MULTIPLY         )  strcpy(text, "NumPad *");
		if ( num == VK_ADD              )  strcpy(text, "NumPad +");
		if ( num == VK_SEPARATOR        )  strcpy(text, "NumPad sep");
		if ( num == VK_SUBTRACT         )  strcpy(text, "NumPad -");
		if ( num == VK_DECIMAL          )  strcpy(text, "NumPad .");
		if ( num == VK_DIVIDE           )  strcpy(text, "NumPad /");
		if ( num == VK_F1               )  strcpy(text, "F1");
		if ( num == VK_F2               )  strcpy(text, "F2");
		if ( num == VK_F3               )  strcpy(text, "F3");
		if ( num == VK_F4               )  strcpy(text, "F4");
		if ( num == VK_F5               )  strcpy(text, "F5");
		if ( num == VK_F6               )  strcpy(text, "F6");
		if ( num == VK_F7               )  strcpy(text, "F7");
		if ( num == VK_F8               )  strcpy(text, "F8");
		if ( num == VK_F9               )  strcpy(text, "F9");
		if ( num == VK_F10              )  strcpy(text, "F10");
		if ( num == VK_F11              )  strcpy(text, "F11");
		if ( num == VK_F12              )  strcpy(text, "F12");
		if ( num == VK_F13              )  strcpy(text, "F13");
		if ( num == VK_F14              )  strcpy(text, "F14");
		if ( num == VK_F15              )  strcpy(text, "F15");
		if ( num == VK_F16              )  strcpy(text, "F16");
		if ( num == VK_F17              )  strcpy(text, "F17");
		if ( num == VK_F18              )  strcpy(text, "F18");
		if ( num == VK_F19              )  strcpy(text, "F19");
		if ( num == VK_F20              )  strcpy(text, "F20");
		if ( num == VK_NUMLOCK          )  strcpy(text, "Num Lock");
		if ( num == VK_SCROLL           )  strcpy(text, "Scroll");
		if ( num == VK_ATTN             )  strcpy(text, "Attn");
		if ( num == VK_CRSEL            )  strcpy(text, "CrSel");
		if ( num == VK_EXSEL            )  strcpy(text, "ExSel");
		if ( num == VK_EREOF            )  strcpy(text, "Erase EOF");
		if ( num == VK_PLAY             )  strcpy(text, "Play");
		if ( num == VK_ZOOM             )  strcpy(text, "Zoom");
		if ( num == VK_PA1              )  strcpy(text, "PA1");
		if ( num == VK_OEM_CLEAR        )  strcpy(text, "Clear");
		if ( num == VK_BUTTON1          )  strcpy(text, "Knopf 1");
		if ( num == VK_BUTTON2          )  strcpy(text, "Knopf 2");
		if ( num == VK_BUTTON3          )  strcpy(text, "Knopf 3");
		if ( num == VK_BUTTON4          )  strcpy(text, "Knopf 4");
		if ( num == VK_BUTTON5          )  strcpy(text, "Knopf 5");
		if ( num == VK_BUTTON6          )  strcpy(text, "Knopf 6");
		if ( num == VK_BUTTON7          )  strcpy(text, "Knopf 7");
		if ( num == VK_BUTTON8          )  strcpy(text, "Knopf 8");
		if ( num == VK_BUTTON9          )  strcpy(text, "Knopf 9");
		if ( num == VK_BUTTON10         )  strcpy(text, "Knopf 10");
		if ( num == VK_BUTTON11         )  strcpy(text, "Knopf 11");
		if ( num == VK_BUTTON12         )  strcpy(text, "Knopf 12");
		if ( num == VK_BUTTON13         )  strcpy(text, "Knopf 13");
		if ( num == VK_BUTTON14         )  strcpy(text, "Knopf 14");
		if ( num == VK_BUTTON15         )  strcpy(text, "Knopf 15");
		if ( num == VK_BUTTON16         )  strcpy(text, "Knopf 16");
		if ( num == VK_BUTTON17         )  strcpy(text, "Knopf 17");
		if ( num == VK_BUTTON18         )  strcpy(text, "Knopf 18");
		if ( num == VK_BUTTON19         )  strcpy(text, "Knopf 19");
		if ( num == VK_BUTTON20         )  strcpy(text, "Knopf 20");
		if ( num == VK_BUTTON21         )  strcpy(text, "Knopf 21");
		if ( num == VK_BUTTON22         )  strcpy(text, "Knopf 22");
		if ( num == VK_BUTTON23         )  strcpy(text, "Knopf 23");
		if ( num == VK_BUTTON24         )  strcpy(text, "Knopf 24");
		if ( num == VK_BUTTON25         )  strcpy(text, "Knopf 25");
		if ( num == VK_BUTTON26         )  strcpy(text, "Knopf 26");
		if ( num == VK_BUTTON27         )  strcpy(text, "Knopf 27");
		if ( num == VK_BUTTON28         )  strcpy(text, "Knopf 28");
		if ( num == VK_BUTTON29         )  strcpy(text, "Knopf 29");
		if ( num == VK_BUTTON30         )  strcpy(text, "Knopf 30");
		if ( num == VK_BUTTON31         )  strcpy(text, "Knopf 31");
		if ( num == VK_BUTTON32         )  strcpy(text, "Knopf 32");
		if ( num == VK_WHEELUP          )  strcpy(text, "Mausrad nach vorne");
		if ( num == VK_WHEELDOWN        )  strcpy(text, "Mausrad zurück");
	}
#endif

#if _POLISH
	if ( type == RES_TEXT )
	{
		#if _FULL
		if ( num == RT_VERSION_ID          )  strcpy(text, "Wersja 1.18 /pl");
		#endif
		#if _NET
		if ( num == RT_VERSION_ID          )  strcpy(text, "CeeBot-A 1.18");
		#endif
		#if _SCHOOL & _EDU
		#if _TEEN
		if ( num == RT_VERSION_ID          )  strcpy(text, "CeeBot-Teen EDU 1.18");
		#else
		if ( num == RT_VERSION_ID          )  strcpy(text, "CeeBot-A EDU 1.18");
		#endif
		#endif
		#if _SCHOOL & _PERSO
		#if _TEEN
		if ( num == RT_VERSION_ID          )  strcpy(text, "CeeBot-Teen PERSO 1.18");
		#else
		if ( num == RT_VERSION_ID          )  strcpy(text, "CeeBot-A PERSO 1.18");
		#endif
		#endif
		#if _SCHOOL & _CEEBOTDEMO
		#if _TEEN
		if ( num == RT_VERSION_ID          )  strcpy(text, "CeeBot-Teen DEMO 1.18");
		#else
		if ( num == RT_VERSION_ID          )  strcpy(text, "CeeBot-A DEMO 1.18");
		#endif
		#endif
		#if _DEMO
		if ( num == RT_VERSION_ID          )  strcpy(text, "Demo 1.18 /pl");
		#endif
		if ( num == RT_DISINFO_TITLE       )  strcpy(text, "SatCom");
		if ( num == RT_WINDOW_MAXIMIZED    )  strcpy(text, "Powiększ");
		if ( num == RT_WINDOW_MINIMIZED    )  strcpy(text, "Pomniejsz");
		if ( num == RT_WINDOW_STANDARD     )  strcpy(text, "Normalna wielkość");
		if ( num == RT_WINDOW_CLOSE        )  strcpy(text, "Zamknij");

		if ( num == RT_STUDIO_TITLE        )  strcpy(text, "Edytor programu");
		if ( num == RT_SCRIPT_NEW          )  strcpy(text, "Nowy");
		if ( num == RT_NAME_DEFAULT        )  strcpy(text, "Gracz");
		if ( num == RT_IO_NEW              )  strcpy(text, "Nowy ...");
		if ( num == RT_KEY_OR              )  strcpy(text, " lub ");

#if _NEWLOOK
		if ( num == RT_TITLE_BASE          )  strcpy(text, "CeeBot");
		if ( num == RT_TITLE_INIT          )  strcpy(text, "CeeBot");
#else
		if ( num == RT_TITLE_BASE          )  strcpy(text, "COLOBOT");
		if ( num == RT_TITLE_INIT          )  strcpy(text, "COLOBOT");
#endif
		if ( num == RT_TITLE_TRAINER       )  strcpy(text, "Ćwiczenia programistyczne");
		if ( num == RT_TITLE_DEFI          )  strcpy(text, "Wyzwania");
		if ( num == RT_TITLE_MISSION       )  strcpy(text, "Misje");
		if ( num == RT_TITLE_FREE          )  strcpy(text, "Swobodna gra");
		if ( num == RT_TITLE_TEEN          )  strcpy(text, "Swobodna gra");
		if ( num == RT_TITLE_USER          )  strcpy(text, "Poziomy użytkownika");
		if ( num == RT_TITLE_PROTO         )  strcpy(text, "Prototypy");
		if ( num == RT_TITLE_SETUP         )  strcpy(text, "Opcje");
		if ( num == RT_TITLE_NAME          )  strcpy(text, "Imię gracza");
		if ( num == RT_TITLE_PERSO         )  strcpy(text, "Dostosuj wygląd");
		if ( num == RT_TITLE_WRITE         )  strcpy(text, "Zapisz bieżącą misję");
		if ( num == RT_TITLE_READ          )  strcpy(text, "Wczytaj zapisaną misję");

		if ( num == RT_PLAY_CHAPt          )  strcpy(text, " Rozdziały:");
		if ( num == RT_PLAY_CHAPd          )  strcpy(text, " Rozdziały:");
		if ( num == RT_PLAY_CHAPm          )  strcpy(text, " Planety:");
		if ( num == RT_PLAY_CHAPf          )  strcpy(text, " Planety:");
		if ( num == RT_PLAY_CHAPu          )  strcpy(text, " Poziomy użytkownika:");
		if ( num == RT_PLAY_CHAPp          )  strcpy(text, " Planety:");
		if ( num == RT_PLAY_CHAPte         )  strcpy(text, " Planety:");
		if ( num == RT_PLAY_LISTt          )  strcpy(text, " Ćwiczenia w tym rozdziale:");
		if ( num == RT_PLAY_LISTd          )  strcpy(text, " Wyzwania w tym rozdziale:");
		if ( num == RT_PLAY_LISTm          )  strcpy(text, " Misje na tej planecie:");
		if ( num == RT_PLAY_LISTf          )  strcpy(text, " Swobodna gra na tej planecie:");
		if ( num == RT_PLAY_LISTu          )  strcpy(text, " Misje na tym poziomie:");
		if ( num == RT_PLAY_LISTp          )  strcpy(text, " Prototypy na tej planecie:");
		if ( num == RT_PLAY_LISTk          )  strcpy(text, " Prototypy na tej planecie:");
		if ( num == RT_PLAY_RESUME         )  strcpy(text, " Streszczenie:");

		if ( num == RT_SETUP_DEVICE        )  strcpy(text, " Sterowniki:");
		if ( num == RT_SETUP_MODE          )  strcpy(text, " Rozdzielczość:");
		if ( num == RT_SETUP_KEY1          )  strcpy(text, "1) Najpierw kliknij klawisz, który chcesz przedefiniować.");
		if ( num == RT_SETUP_KEY2          )  strcpy(text, "2) Następnie naciśnij klawisz, którego chcesz używać.");

		if ( num == RT_PERSO_FACE          )  strcpy(text, "Rodzaj twarzy:");
		if ( num == RT_PERSO_GLASSES       )  strcpy(text, "Okulary:");
		if ( num == RT_PERSO_HAIR          )  strcpy(text, "Kolor włosów:");
		if ( num == RT_PERSO_COMBI         )  strcpy(text, "Kolor skafandra:");
		if ( num == RT_PERSO_BAND          )  strcpy(text, "Kolor pasków:");

#if _NEWLOOK
		if ( num == RT_DIALOG_TITLE        )  strcpy(text, "CeeBot");
		if ( num == RT_DIALOG_QUIT         )  strcpy(text, "Czy na pewno chcesz opuścić grę CeeBot?");
		if ( num == RT_DIALOG_YESQUIT      )  strcpy(text, "Zakończ\\Kończy grę CeeBot");
#else
		if ( num == RT_DIALOG_TITLE        )  strcpy(text, "COLOBOT");
		if ( num == RT_DIALOG_QUIT         )  strcpy(text, "Czy na pewno chcesz opuścić grę COLOBOT?");
		if ( num == RT_DIALOG_YESQUIT      )  strcpy(text, "Zakończ\\Kończy grę COLOBOT");
#endif
		if ( num == RT_DIALOG_ABORT        )  strcpy(text, "Opuścić misję?");
		if ( num == RT_DIALOG_YES          )  strcpy(text, "Przerwij\\Przerywa bieżącą misję");
		if ( num == RT_DIALOG_NO           )  strcpy(text, "Kontynuuj\\Kontynuuje bieżącą misję");
		if ( num == RT_DIALOG_NOQUIT       )  strcpy(text, "Kontynuuj\\Kontynuuje grę");
		if ( num == RT_DIALOG_DELOBJ       )  strcpy(text, "Czy na pewno chcesz zniszczyć zaznaczony budynek?");
		if ( num == RT_DIALOG_DELGAME      )  strcpy(text, "Czy na pewno chcesz skasować zapisane gry gracza %s? ");
		if ( num == RT_DIALOG_YESDEL       )  strcpy(text, "Usuń");
		if ( num == RT_DIALOG_NODEL        )  strcpy(text, "Anuluj");
		if ( num == RT_DIALOG_LOADING      )  strcpy(text, "WCZYTYWANIE");

		if ( num == RT_STUDIO_LISTTT       )  strcpy(text, "Skróty klawiszowe (\\key cbot;)");
		if ( num == RT_STUDIO_COMPOK       )  strcpy(text, "Program skompilowany (0 błędów)");
		if ( num == RT_STUDIO_PROGSTOP     )  strcpy(text, "Program zakończony");

		if ( num == RT_SATCOM_LIST         )  strcpy(text, "\\b;Lista obiektów\n");
		if ( num == RT_SATCOM_BOT          )  strcpy(text, "\\b;Roboty\n");
		if ( num == RT_SATCOM_BUILDING     )  strcpy(text, "\\b;Budynki\n");
		if ( num == RT_SATCOM_FRET         )  strcpy(text, "\\b;Obiekty ruchome\n");
		if ( num == RT_SATCOM_ALIEN        )  strcpy(text, "\\b;Obcy\n");
		if ( num == RT_SATCOM_NULL         )  strcpy(text, "\\c; (brak)\\n;\n");
		if ( num == RT_SATCOM_ERROR1       )  strcpy(text, "\\b;Błąd\n");
		if ( num == RT_SATCOM_ERROR2       )  strcpy(text, "Lista jest dostępna jedynie gdy działa \\l;stacja radarowa\\u object\\radar;.\n");

		if ( num == RT_IO_OPEN             )  strcpy(text, "Otwórz");
		if ( num == RT_IO_SAVE             )  strcpy(text, "Zapisz");
		if ( num == RT_IO_LIST             )  strcpy(text, "Folder: %s");
		if ( num == RT_IO_NAME             )  strcpy(text, "Nazwa:");
		if ( num == RT_IO_DIR              )  strcpy(text, "Folder:");
		if ( num == RT_IO_PRIVATE          )  strcpy(text, "Prywatny\\Folder prywatny");
		if ( num == RT_IO_PUBLIC           )  strcpy(text, "Publiczny\\Folder ogólnodostępny");

		if ( num == RT_GENERIC_DEV1        )  strcpy(text, "Twórcy:");
		if ( num == RT_GENERIC_DEV2        )  strcpy(text, "www.epsitec.com");
		if ( num == RT_GENERIC_EDIT1       )  strcpy(text, "Wersja polska wydana przez:");
		if ( num == RT_GENERIC_EDIT2       )  strcpy(text, "www.manta.com.pl");
		if ( num == RT_GENERIC_EDIT1       )  strcpy(text, " ");
		if ( num == RT_GENERIC_EDIT2       )  strcpy(text, " ");

		if ( num == RT_INTERFACE_REC       )  strcpy(text, "Recorder");
	}

	if ( type == RES_EVENT )
	{
		if ( num == EVENT_BUTTON_OK        )  strcpy(text, "OK");
		if ( num == EVENT_BUTTON_CANCEL    )  strcpy(text, "Anuluj");
		if ( num == EVENT_BUTTON_NEXT      )  strcpy(text, "Następny");
		if ( num == EVENT_BUTTON_PREV      )  strcpy(text, "Poprzedni");
		if ( num == EVENT_BUTTON_QUIT      )  strcpy(text, "Menu (\\key quit;)");

		if ( num == EVENT_DIALOG_OK        )  strcpy(text, "OK");
		if ( num == EVENT_DIALOG_CANCEL    )  strcpy(text, "Anuluj");

		if ( num == EVENT_INTERFACE_TRAINER)  strcpy(text, "Ćwiczenia\\Ćwiczenia programistyczne");
		if ( num == EVENT_INTERFACE_DEFI   )  strcpy(text, "Wyzwania\\Wyzwania programistyczne");
		if ( num == EVENT_INTERFACE_MISSION)  strcpy(text, "Misje\\Wybierz misję");
		if ( num == EVENT_INTERFACE_FREE   )  strcpy(text, "Swobodna gra\\Swobodna gra bez konkretnych celów");
		if ( num == EVENT_INTERFACE_TEEN   )  strcpy(text, "Swobodna gra\\Swobodna gra bez konkretnych celów");
		if ( num == EVENT_INTERFACE_USER   )  strcpy(text, "Poziomy\\Poziomy użytkownika");
		if ( num == EVENT_INTERFACE_PROTO  )  strcpy(text, "Prototypy\\Prototypy w trakcie rozwijania");
		if ( num == EVENT_INTERFACE_NAME   )  strcpy(text, "Nowy gracz\\Wybierz imię gracza");
		if ( num == EVENT_INTERFACE_SETUP  )  strcpy(text, "Opcje\\Preferencje");
		if ( num == EVENT_INTERFACE_AGAIN  )  strcpy(text, "Uruchom ponownie\\Uruchamia ponownie misję od początku");
		if ( num == EVENT_INTERFACE_WRITE  )  strcpy(text, "Zapisz\\Zapisuje bieżącą misję");
		if ( num == EVENT_INTERFACE_READ   )  strcpy(text, "Wczytaj\\Wczytuje zapisaną misję");
#if _NEWLOOK
		if ( num == EVENT_INTERFACE_ABORT  )  strcpy(text, "\\Powróć do gry CeeBot");
		if ( num == EVENT_INTERFACE_QUIT   )  strcpy(text, "Zakończ\\Kończy grę CeeBot");
#else
		if ( num == EVENT_INTERFACE_ABORT  )  strcpy(text, "\\Powróć do gry COLOBOT");
		if ( num == EVENT_INTERFACE_QUIT   )  strcpy(text, "Zakończ\\Kończy grę COLOBOT");
#endif
		if ( num == EVENT_INTERFACE_BACK   )  strcpy(text, "<<  Wstecz  \\Wraca do poprzedniego ekranu");
		if ( num == EVENT_INTERFACE_PLAY   )  strcpy(text, "Graj\\Rozpoczyna misję!");
		if ( num == EVENT_INTERFACE_SETUPd )  strcpy(text, "Urządzenie\\Ustawienia sterownika i rozdzielczości");
		if ( num == EVENT_INTERFACE_SETUPg )  strcpy(text, "Grafika\\Ustawienia grafiki");
		if ( num == EVENT_INTERFACE_SETUPp )  strcpy(text, "Gra\\Ustawienia gry");
		if ( num == EVENT_INTERFACE_SETUPc )  strcpy(text, "Sterowanie\\Ustawienia klawiatury, joysticka i myszy");
		if ( num == EVENT_INTERFACE_SETUPs )  strcpy(text, "Dźwięk\\Głośność muzyki i dźwięków gry");
		if ( num == EVENT_INTERFACE_DEVICE )  strcpy(text, "Jednostka");
		if ( num == EVENT_INTERFACE_RESOL  )  strcpy(text, "Rozdzielczość");
		if ( num == EVENT_INTERFACE_FULL   )  strcpy(text, "Pełny ekran\\Pełny ekran lub tryb okna");
		if ( num == EVENT_INTERFACE_APPLY  )  strcpy(text, "Zastosuj zmiany\\Aktywuje zmienione ustawienia");

		if ( num == EVENT_INTERFACE_TOTO   )  strcpy(text, "Robbie\\Twój asystent");
		if ( num == EVENT_INTERFACE_SHADOW )  strcpy(text, "Cienie\\Cienie na ziemi");
		if ( num == EVENT_INTERFACE_GROUND )  strcpy(text, "Znaki na ziemi\\Znaki na ziemi");
		if ( num == EVENT_INTERFACE_DIRTY  )  strcpy(text, "Kurz\\Kurz i bród na robotach i budynkach");
		if ( num == EVENT_INTERFACE_FOG    )  strcpy(text, "Mgła\\Mgła");
		if ( num == EVENT_INTERFACE_LENS   )  strcpy(text, "Promienie słoneczne\\Promienie słoneczne na niebie");
		if ( num == EVENT_INTERFACE_SKY    )  strcpy(text, "Niebo\\Chmury i mgławice");
		if ( num == EVENT_INTERFACE_PLANET )  strcpy(text, "Planety i gwiazdy\\Obiekty astronomiczne na niebie");
		if ( num == EVENT_INTERFACE_LIGHT  )  strcpy(text, "Dynamiczne oświetlenie\\Ruchome źródła światła");
		if ( num == EVENT_INTERFACE_PARTI  )  strcpy(text, "Liczba cząstek\\Wybuchy, kurz, odbicia, itp.");
		if ( num == EVENT_INTERFACE_CLIP   )  strcpy(text, "Głębokość pola\\Maksymalna widoczność");
		if ( num == EVENT_INTERFACE_DETAIL )  strcpy(text, "Szczegóły\\Jakość wizualna obiektów 3D");
		if ( num == EVENT_INTERFACE_TEXTURE)  strcpy(text, "Tekstury\\Jakość tekstur ");
		if ( num == EVENT_INTERFACE_GADGET )  strcpy(text, "Ilość elementów dekoracyjnych \\Ilość elementów czysto dekoracyjnych");
		if ( num == EVENT_INTERFACE_RAIN   )  strcpy(text, "Cząstki w interfejsie\\Para i iskry z silników w interfejsie");
		if ( num == EVENT_INTERFACE_GLINT  )  strcpy(text, "Odbicia na przyciskach \\Świecące przyciski");
		if ( num == EVENT_INTERFACE_TOOLTIP)  strcpy(text, "Dymki pomocy\\Wyjaśnia funkcje przycisków");
		if ( num == EVENT_INTERFACE_MOVIES )  strcpy(text, "Sekwencje filmowe\\Filmy przed rozpoczęciem i na zakończenie misji");
		if ( num == EVENT_INTERFACE_NICERST)  strcpy(text, "Końcowy film\\Film na zakończenie ćwiczeń");
		if ( num == EVENT_INTERFACE_HIMSELF)  strcpy(text, "Przyjacielski ogień\\Własne strzały uszkadzają Twoje obiekty");
		if ( num == EVENT_INTERFACE_SCROLL )  strcpy(text, "Przewijanie\\Ekran jest przewijany gdy mysz dotknie prawej lub lewej jego krawędzi");
		if ( num == EVENT_INTERFACE_INVERTX)  strcpy(text, "Odwrócenie myszy X\\Odwrócenie kierunków przewijania w poziomie");
		if ( num == EVENT_INTERFACE_INVERTY)  strcpy(text, "Odwrócenie myszy Y\\Odwrócenie kierunków przewijania w pionie");
		if ( num == EVENT_INTERFACE_EFFECT )  strcpy(text, "Wstrząsy przy wybuchach\\Ekran trzęsie się podczas wybuchów");
		if ( num == EVENT_INTERFACE_MOUSE  )  strcpy(text, "Cień kursora myszy\\Dodaje cień kursorowi myszy");
		if ( num == EVENT_INTERFACE_EDITMODE) strcpy(text, "Automatyczne wcięcia\\Automatyczne wcięcia podczas edycji programu");
		if ( num == EVENT_INTERFACE_EDITVALUE)strcpy(text, "Duże wcięcie\\2 lub 4 spacje wcięcia na każdy poziom zdefiniowany przez klamry");
		if ( num == EVENT_INTERFACE_SOLUCE4)  strcpy(text, "Accčs aux solutions\\Programme \"4: Solution\" dans les exercices");

		if ( num == EVENT_INTERFACE_KDEF   )  strcpy(text, "Standardowa kontrola\\Standardowe klawisze funkcyjne");
		if ( num == EVENT_INTERFACE_KLEFT  )  strcpy(text, "Skręć w lewo\\Obraca robota w lewo");
		if ( num == EVENT_INTERFACE_KRIGHT )  strcpy(text, "Obróć w prawo\\Obraca robota w prawo");
		if ( num == EVENT_INTERFACE_KUP    )  strcpy(text, "Naprzód\\Porusza do przodu");
		if ( num == EVENT_INTERFACE_KDOWN  )  strcpy(text, "Wstecz\\Porusza do tyłu");
		if ( num == EVENT_INTERFACE_KGUP   )  strcpy(text, "W górę\\Zwiększa moc silnika");
		if ( num == EVENT_INTERFACE_KGDOWN )  strcpy(text, "W dół\\Zmniejsza moc silnika");
		if ( num == EVENT_INTERFACE_KCAMERA)  strcpy(text, "Zmień kamerę\\Przełącza pomiędzy kamerą pokładową i śledzącą");
		if ( num == EVENT_INTERFACE_KDESEL )  strcpy(text, "Poprzedni obiekt\\Zaznacz poprzedni obiekt");
		if ( num == EVENT_INTERFACE_KACTION)  strcpy(text, "Standardowa akcja\\Standardowa akcja robota (podnieś/upuść, strzelaj, szukaj, itp.)");
		if ( num == EVENT_INTERFACE_KNEAR  )  strcpy(text, "Kamera bliżej\\Przybliża kamerę");
		if ( num == EVENT_INTERFACE_KAWAY  )  strcpy(text, "Kamera dalej\\Oddala kamerę");
		if ( num == EVENT_INTERFACE_KNEXT  )  strcpy(text, "Następny obiekt\\Zaznacza następny obiekt");
		if ( num == EVENT_INTERFACE_KHUMAN )  strcpy(text, "Zaznacz astronautę\\Zaznacza astronautę");
		if ( num == EVENT_INTERFACE_KQUIT  )  strcpy(text, "Zakończ\\Kończy bieżącą misję lub ćwiczenie");
		if ( num == EVENT_INTERFACE_KHELP  )  strcpy(text, "Rozkazy\\Pokazuje rozkazy dotyczące bieżącej misji");
		if ( num == EVENT_INTERFACE_KPROG  )  strcpy(text, "Podręcznik programowania\\Dostarcza szczegółową pomoc w programowaniu");
		if ( num == EVENT_INTERFACE_KCBOT  )  strcpy(text, "Pomoc dot. słów kluczowych\\Dokładniejsza pomoc na temat słów kluczowych");
		if ( num == EVENT_INTERFACE_KVISIT )  strcpy(text, "Miejsce nadania wiadomości\\Pokazuje skąd została wysłana ostatnia wiadomość");
		if ( num == EVENT_INTERFACE_KSPEED10) strcpy(text, "Prędkość 1,0x\\Prędkość normalna");
		if ( num == EVENT_INTERFACE_KSPEED15) strcpy(text, "Prędkość 1,5x\\1,5 raza szybciej");
		if ( num == EVENT_INTERFACE_KSPEED20) strcpy(text, "Prędkość 2,0x\\Dwa razy szybciej");
		if ( num == EVENT_INTERFACE_KSPEED30) strcpy(text, "Prędkość 3,0x\\Trzy razy szybciej");

		if ( num == EVENT_INTERFACE_VOLSOUND) strcpy(text, "Efekty dźwiękowe:\\Głośność silników, głosów, strzałów, itp.");
		if ( num == EVENT_INTERFACE_VOLMUSIC) strcpy(text, "Muzyka w tle :\\Głośność ścieżek dźwiękowych z płyty CD");
		if ( num == EVENT_INTERFACE_SOUND3D)  strcpy(text, "Dźwięk 3D\\Przestrzenne pozycjonowanie dźwięków");

		if ( num == EVENT_INTERFACE_MIN    )  strcpy(text, "Najniższa\\Minimalna jakość grafiki (najwyższa częstotliwość odświeżania)");
		if ( num == EVENT_INTERFACE_NORM   )  strcpy(text, "Normalna\\Normalna jakość grafiki");
		if ( num == EVENT_INTERFACE_MAX    )  strcpy(text, "Najwyższa\\Maksymalna jakość grafiki (najniższa częstotliwość odświeżania)");

		if ( num == EVENT_INTERFACE_SILENT )  strcpy(text, "Cisza\\Brak dźwięków");
		if ( num == EVENT_INTERFACE_NOISY  )  strcpy(text, "Normalne\\Normalna głośność dźwięków");

		if ( num == EVENT_INTERFACE_JOYSTICK) strcpy(text, "Używaj joysticka\\Joystick lub klawiatura");
		if ( num == EVENT_INTERFACE_SOLUCE )  strcpy(text, "Dostęp do rozwiązania\\Pokazuje rozwiązanie (szczegółowe instrukcje dotyczące misji)");

		if ( num == EVENT_INTERFACE_NEDIT  )  strcpy(text, "\\Nowe imię gracza");
		if ( num == EVENT_INTERFACE_NOK    )  strcpy(text, "OK\\Wybiera zaznaczonego gracza");
		if ( num == EVENT_INTERFACE_NCANCEL)  strcpy(text, "Anuluj\\Zachowuje bieżące imię gracza");
		if ( num == EVENT_INTERFACE_NDELETE)  strcpy(text, "Usuń gracza\\Usuwa gracza z listy");
		if ( num == EVENT_INTERFACE_NLABEL )  strcpy(text, "Imię gracza");

		if ( num == EVENT_INTERFACE_IOWRITE)  strcpy(text, "Zapisz\\Zapisuje bieżącą misję");
		if ( num == EVENT_INTERFACE_IOREAD )  strcpy(text, "Wczytaj\\Wczytuje zaznaczoną misję");
		if ( num == EVENT_INTERFACE_IOLIST )  strcpy(text, "Lista zapisanych misji");
		if ( num == EVENT_INTERFACE_IOLABEL)  strcpy(text, "Nazwa pliku:");
		if ( num == EVENT_INTERFACE_IONAME )  strcpy(text, "Nazwa misji");
		if ( num == EVENT_INTERFACE_IOIMAGE)  strcpy(text, "Fotografia");
		if ( num == EVENT_INTERFACE_IODELETE) strcpy(text, "Usuń\\Usuwa zaznaczony plik");

		if ( num == EVENT_INTERFACE_PERSO  )  strcpy(text, "Wygląd\\Wybierz swoją postać");
		if ( num == EVENT_INTERFACE_POK    )  strcpy(text, "OK");
		if ( num == EVENT_INTERFACE_PCANCEL)  strcpy(text, "Anuluj");
		if ( num == EVENT_INTERFACE_PDEF   )  strcpy(text, "Standardowe\\Standardowe ustawienia wyglądu");
		if ( num == EVENT_INTERFACE_PHEAD  )  strcpy(text, "Głowa\\Twarz i włosy");
		if ( num == EVENT_INTERFACE_PBODY  )  strcpy(text, "Skafander\\Skafander astronauty");
		if ( num == EVENT_INTERFACE_PLROT  )  strcpy(text, "\\Obróć w lewo");
		if ( num == EVENT_INTERFACE_PRROT  )  strcpy(text, "\\Obróć w prawo");
		if ( num == EVENT_INTERFACE_PCRa   )  strcpy(text, "Czerwony");
		if ( num == EVENT_INTERFACE_PCGa   )  strcpy(text, "Zielony");
		if ( num == EVENT_INTERFACE_PCBa   )  strcpy(text, "Niebieski");
		if ( num == EVENT_INTERFACE_PCRb   )  strcpy(text, "Czerwony");
		if ( num == EVENT_INTERFACE_PCGb   )  strcpy(text, "Zielony");
		if ( num == EVENT_INTERFACE_PCBb   )  strcpy(text, "Niebieski");
		if ( num == EVENT_INTERFACE_PFACE1 )  strcpy(text, "\\Twarz 1");
		if ( num == EVENT_INTERFACE_PFACE2 )  strcpy(text, "\\Twarz 4");
		if ( num == EVENT_INTERFACE_PFACE3 )  strcpy(text, "\\Twarz 3");
		if ( num == EVENT_INTERFACE_PFACE4 )  strcpy(text, "\\Twarz 2");
		if ( num == EVENT_INTERFACE_PGLASS0)  strcpy(text, "\\Bez okularów");
		if ( num == EVENT_INTERFACE_PGLASS1)  strcpy(text, "\\Okulary 1");
		if ( num == EVENT_INTERFACE_PGLASS2)  strcpy(text, "\\Okulary 2");
		if ( num == EVENT_INTERFACE_PGLASS3)  strcpy(text, "\\Okulary 3");
		if ( num == EVENT_INTERFACE_PGLASS4)  strcpy(text, "\\Okulary 4");
		if ( num == EVENT_INTERFACE_PGLASS5)  strcpy(text, "\\Okulary 5");

		if ( num == EVENT_OBJECT_DESELECT  )  strcpy(text, "Poprzednie zaznaczenie (\\key desel;)");
		if ( num == EVENT_OBJECT_LEFT      )  strcpy(text, "Skręć w lewo (\\key left;)");
		if ( num == EVENT_OBJECT_RIGHT     )  strcpy(text, "Skręć w prawo (\\key right;)");
		if ( num == EVENT_OBJECT_UP        )  strcpy(text, "Naprzód (\\key up;)");
		if ( num == EVENT_OBJECT_DOWN      )  strcpy(text, "Cofnij (\\key down;)");
		if ( num == EVENT_OBJECT_GASUP     )  strcpy(text, "Góra (\\key gup;)");
		if ( num == EVENT_OBJECT_GASDOWN   )  strcpy(text, "Dół (\\key gdown;)");
		if ( num == EVENT_OBJECT_HTAKE     )  strcpy(text, "Podnieś lub upuść (\\key action;)");
		if ( num == EVENT_OBJECT_MTAKE     )  strcpy(text, "Podnieś lub upuść (\\key action;)");
		if ( num == EVENT_OBJECT_MFRONT    )  strcpy(text, "..przed");
		if ( num == EVENT_OBJECT_MBACK     )  strcpy(text, "..za");
		if ( num == EVENT_OBJECT_MPOWER    )  strcpy(text, "..ogniwo elektryczne");
		if ( num == EVENT_OBJECT_BHELP     )  strcpy(text, "Rozkazy dotyczące misji (\\key help;)");
		if ( num == EVENT_OBJECT_BTAKEOFF  )  strcpy(text, "Odleć, aby zakończyć misję");
		if ( num == EVENT_OBJECT_BDERRICK  )  strcpy(text, "Zbuduj kopalnię");
		if ( num == EVENT_OBJECT_BSTATION  )  strcpy(text, "Zbuduj elektrownię");
		if ( num == EVENT_OBJECT_BFACTORY  )  strcpy(text, "Zbuduj fabrykę robotów");
		if ( num == EVENT_OBJECT_BREPAIR   )  strcpy(text, "Zbuduj warsztat");
		if ( num == EVENT_OBJECT_BCONVERT  )  strcpy(text, "Zbuduj hutę");
		if ( num == EVENT_OBJECT_BTOWER    )  strcpy(text, "Zbuduj wieżę obronną");
		if ( num == EVENT_OBJECT_BRESEARCH )  strcpy(text, "Zbuduj centrum badawcze");
		if ( num == EVENT_OBJECT_BRADAR    )  strcpy(text, "Zbuduj stację radarową");
		if ( num == EVENT_OBJECT_BENERGY   )  strcpy(text, "Zbuduj fabrykę ogniw elektrycznych");
		if ( num == EVENT_OBJECT_BLABO     )  strcpy(text, "Zbuduj laboratorium");
		if ( num == EVENT_OBJECT_BNUCLEAR  )  strcpy(text, "Zbuduj elektrownię atomową");
		if ( num == EVENT_OBJECT_BPARA     )  strcpy(text, "Zbuduj odgromnik");
		if ( num == EVENT_OBJECT_BINFO     )  strcpy(text, "Zbuduj stację przekaźnikową");
		if ( num == EVENT_OBJECT_GFLAT     )  strcpy(text, "Pokaż czy teren jest płaski");
		if ( num == EVENT_OBJECT_FCREATE   )  strcpy(text, "Postaw flagę");
		if ( num == EVENT_OBJECT_FDELETE   )  strcpy(text, "Usuń flagę");
		if ( num == EVENT_OBJECT_FCOLORb   )  strcpy(text, "\\Niebieskie flagi");
		if ( num == EVENT_OBJECT_FCOLORr   )  strcpy(text, "\\Czerwone flagi");
		if ( num == EVENT_OBJECT_FCOLORg   )  strcpy(text, "\\Zielone flagi");
		if ( num == EVENT_OBJECT_FCOLORy   )  strcpy(text, "\\Żółte flagi");
		if ( num == EVENT_OBJECT_FCOLORv   )  strcpy(text, "\\Fioletowe flagi");
		if ( num == EVENT_OBJECT_FACTORYfa )  strcpy(text, "Zbuduj transporter latający");
		if ( num == EVENT_OBJECT_FACTORYta )  strcpy(text, "Zbuduj transporter na gąsienicach");
		if ( num == EVENT_OBJECT_FACTORYwa )  strcpy(text, "Zbuduj transporter na kołach");
		if ( num == EVENT_OBJECT_FACTORYia )  strcpy(text, "Zbuduj transporter na nogach");
		if ( num == EVENT_OBJECT_FACTORYfc )  strcpy(text, "Zbuduj działo latające");
		if ( num == EVENT_OBJECT_FACTORYtc )  strcpy(text, "Zbuduj działo na gąsienicach");
		if ( num == EVENT_OBJECT_FACTORYwc )  strcpy(text, "Zbuduj działo na kołach");
		if ( num == EVENT_OBJECT_FACTORYic )  strcpy(text, "Zbuduj działo na nogach");
		if ( num == EVENT_OBJECT_FACTORYfi )  strcpy(text, "Zbuduj latające działo organiczne");
		if ( num == EVENT_OBJECT_FACTORYti )  strcpy(text, "Zbuduj działo organiczne na gąsienicach");
		if ( num == EVENT_OBJECT_FACTORYwi )  strcpy(text, "Zbuduj działo organiczne na kołach");
		if ( num == EVENT_OBJECT_FACTORYii )  strcpy(text, "Zbuduj działo organiczne na nogach");
		if ( num == EVENT_OBJECT_FACTORYfs )  strcpy(text, "Zbuduj szperacz latający");
		if ( num == EVENT_OBJECT_FACTORYts )  strcpy(text, "Zbuduj szperacz na gąsienicach");
		if ( num == EVENT_OBJECT_FACTORYws )  strcpy(text, "Zbuduj szperacz na kołach");
		if ( num == EVENT_OBJECT_FACTORYis )  strcpy(text, "Zbuduj szperacz na nogach");
		if ( num == EVENT_OBJECT_FACTORYrt )  strcpy(text, "Zbuduj robota uderzacza");
		if ( num == EVENT_OBJECT_FACTORYrc )  strcpy(text, "Zbuduj działo fazowe");
		if ( num == EVENT_OBJECT_FACTORYrr )  strcpy(text, "Zbuduj robota recyklera");
		if ( num == EVENT_OBJECT_FACTORYrs )  strcpy(text, "Zbuduj robota osłaniajacza");
		if ( num == EVENT_OBJECT_FACTORYsa )  strcpy(text, "Zbuduj robota nurka");
		if ( num == EVENT_OBJECT_RTANK     )  strcpy(text, "Rozpocznij prace badawcze nad transporterem na gąsienicach");
		if ( num == EVENT_OBJECT_RFLY      )  strcpy(text, "Rozpocznij prace badawcze nad transporterem latającym");
		if ( num == EVENT_OBJECT_RTHUMP    )  strcpy(text, "Rozpocznij prace badawcze nad robotem uderzaczem");
		if ( num == EVENT_OBJECT_RCANON    )  strcpy(text, "Rozpocznij prace badawcze nad działem");
		if ( num == EVENT_OBJECT_RTOWER    )  strcpy(text, "Rozpocznij prace badawcze nad wieżą obronną");
		if ( num == EVENT_OBJECT_RPHAZER   )  strcpy(text, "Rozpocznij prace badawcze nad działem fazowym");
		if ( num == EVENT_OBJECT_RSHIELD   )  strcpy(text, "Rozpocznij prace badawcze nad robotem osłaniaczem");
		if ( num == EVENT_OBJECT_RATOMIC   )  strcpy(text, "Rozpocznij prace badawcze nad energią atomową");
		if ( num == EVENT_OBJECT_RiPAW     )  strcpy(text, "Rozpocznij prace badawcze nad transporterem na nogach");
		if ( num == EVENT_OBJECT_RiGUN     )  strcpy(text, "Rozpocznij prace badawcze nad działem organicznym");
		if ( num == EVENT_OBJECT_RESET     )  strcpy(text, "Powrót do początku");
		if ( num == EVENT_OBJECT_SEARCH    )  strcpy(text, "Szukaj (\\key action;)");
		if ( num == EVENT_OBJECT_TERRAFORM )  strcpy(text, "Uderz (\\key action;)");
		if ( num == EVENT_OBJECT_FIRE      )  strcpy(text, "Strzelaj (\\key action;)");
		if ( num == EVENT_OBJECT_RECOVER   )  strcpy(text, "Odzyskaj (\\key action;)");
		if ( num == EVENT_OBJECT_BEGSHIELD )  strcpy(text, "Rozszerz osłonę (\\key action;)");
		if ( num == EVENT_OBJECT_ENDSHIELD )  strcpy(text, "Wyłącz osłonę (\\key action;)");
		if ( num == EVENT_OBJECT_DIMSHIELD )  strcpy(text, "Zasięg osłony");
		if ( num == EVENT_OBJECT_PROGRUN   )  strcpy(text, "Wykonaj zaznaczony program");
		if ( num == EVENT_OBJECT_PROGEDIT  )  strcpy(text, "Edytuj zaznaczony program");
		if ( num == EVENT_OBJECT_INFOOK    )  strcpy(text, "\\Przełącz przekaźnik SatCom w stan gotowości");
		if ( num == EVENT_OBJECT_DELETE    )  strcpy(text, "Zniszcz budynek");
		if ( num == EVENT_OBJECT_GENERGY   )  strcpy(text, "Poziom energii");
		if ( num == EVENT_OBJECT_GSHIELD   )  strcpy(text, "Poziom osłony");
		if ( num == EVENT_OBJECT_GRANGE    )  strcpy(text, "Temperatura silnika");
		if ( num == EVENT_OBJECT_GPROGRESS )  strcpy(text, "Wciąż pracuje...");
		if ( num == EVENT_OBJECT_GRADAR    )  strcpy(text, "Liczba wykrytych insektów");
		if ( num == EVENT_OBJECT_GINFO     )  strcpy(text, "Przesłane informacje");
		if ( num == EVENT_OBJECT_COMPASS   )  strcpy(text, "Kompas");
//?		if ( num == EVENT_OBJECT_MAP       )  strcpy(text, "Mapka");
		if ( num == EVENT_OBJECT_MAPZOOM   )  strcpy(text, "Powiększenie mapki");
		if ( num == EVENT_OBJECT_CAMERA    )  strcpy(text, "Kamera (\\key camera;)");
		if ( num == EVENT_OBJECT_CAMERAleft)  strcpy(text, "Camera to left");
		if ( num == EVENT_OBJECT_CAMERAright) strcpy(text, "Camera to right");
		if ( num == EVENT_OBJECT_CAMERAnear)  strcpy(text, "Camera nearest");
		if ( num == EVENT_OBJECT_CAMERAaway)  strcpy(text, "Camera awayest");
		if ( num == EVENT_OBJECT_HELP      )  strcpy(text, "Pomoc na temat zaznaczonego obiektu");
		if ( num == EVENT_OBJECT_SOLUCE    )  strcpy(text, "Pokaż rozwiązanie");
		if ( num == EVENT_OBJECT_SHORTCUT00)  strcpy(text, "Przełącz roboty <-> budynki");
		if ( num == EVENT_OBJECT_LIMIT     )  strcpy(text, "Pokaż zasięg");
		if ( num == EVENT_OBJECT_PEN0      )  strcpy(text, "\\Relčve le crayon");
		if ( num == EVENT_OBJECT_PEN1      )  strcpy(text, "\\Abaisse le crayon noir");
		if ( num == EVENT_OBJECT_PEN2      )  strcpy(text, "\\Abaisse le crayon jaune");
		if ( num == EVENT_OBJECT_PEN3      )  strcpy(text, "\\Abaisse le crayon orange");
		if ( num == EVENT_OBJECT_PEN4      )  strcpy(text, "\\Abaisse le crayon rouge");
		if ( num == EVENT_OBJECT_PEN5      )  strcpy(text, "\\Abaisse le crayon violet");
		if ( num == EVENT_OBJECT_PEN6      )  strcpy(text, "\\Abaisse le crayon bleu");
		if ( num == EVENT_OBJECT_PEN7      )  strcpy(text, "\\Abaisse le crayon vert");
		if ( num == EVENT_OBJECT_PEN8      )  strcpy(text, "\\Abaisse le crayon brun");
		if ( num == EVENT_OBJECT_REC       )  strcpy(text, "\\Démarre l'enregistrement");
		if ( num == EVENT_OBJECT_STOP      )  strcpy(text, "\\Stoppe l'enregistrement");
		if ( num == EVENT_DT_VISIT0        ||
			 num == EVENT_DT_VISIT1        ||
			 num == EVENT_DT_VISIT2        ||
			 num == EVENT_DT_VISIT3        ||
			 num == EVENT_DT_VISIT4        )  strcpy(text, "Pokaż miejsce");
		if ( num == EVENT_DT_END           )  strcpy(text, "Kontynuuj");
		if ( num == EVENT_CMD              )  strcpy(text, "Linia polecenia");
		if ( num == EVENT_SPEED            )  strcpy(text, "Prędkość gry");

		if ( num == EVENT_HYPER_PREV       )  strcpy(text, "Wstecz");
		if ( num == EVENT_HYPER_NEXT       )  strcpy(text, "Naprzód");
		if ( num == EVENT_HYPER_HOME       )  strcpy(text, "Początek");
		if ( num == EVENT_HYPER_COPY       )  strcpy(text, "Kopiuj");
		if ( num == EVENT_HYPER_SIZE1      )  strcpy(text, "Wielkość 1");
		if ( num == EVENT_HYPER_SIZE2      )  strcpy(text, "Wielkość 2");
		if ( num == EVENT_HYPER_SIZE3      )  strcpy(text, "Wielkość 3");
		if ( num == EVENT_HYPER_SIZE4      )  strcpy(text, "Wielkość 4");
		if ( num == EVENT_HYPER_SIZE5      )  strcpy(text, "Wielkość 5");
		if ( num == EVENT_SATCOM_HUSTON    )  strcpy(text, "Rozkazy z Houston");
#if _TEEN
		if ( num == EVENT_SATCOM_SAT       )  strcpy(text, "Raport z satelity");
#else
		if ( num == EVENT_SATCOM_SAT       )  strcpy(text, "Raport z satelity");
#endif
		if ( num == EVENT_SATCOM_LOADING   )  strcpy(text, "Program dostarczony z Houston");
		if ( num == EVENT_SATCOM_OBJECT    )  strcpy(text, "Lista obiektów");
		if ( num == EVENT_SATCOM_PROG      )  strcpy(text, "Podręcznik programowania");
		if ( num == EVENT_SATCOM_SOLUCE    )  strcpy(text, "Rozwiązanie");

		if ( num == EVENT_STUDIO_OK        )  strcpy(text, "OK\\Zamyka edytor programu i powraca do gry");
		if ( num == EVENT_STUDIO_CANCEL    )  strcpy(text, "Anuluj\\Pomija wszystkie zmiany");
		if ( num == EVENT_STUDIO_NEW       )  strcpy(text, "Nowy");
		if ( num == EVENT_STUDIO_OPEN      )  strcpy(text, "Otwórz (Ctrl+O)");
		if ( num == EVENT_STUDIO_SAVE      )  strcpy(text, "Zapisz (Ctrl+S)");
		if ( num == EVENT_STUDIO_UNDO      )  strcpy(text, "Cofnij (Ctrl+Z)");
		if ( num == EVENT_STUDIO_CUT       )  strcpy(text, "Wytnij (Ctrl+X)");
		if ( num == EVENT_STUDIO_COPY      )  strcpy(text, "Kopiuj (Ctrl+C)");
		if ( num == EVENT_STUDIO_PASTE     )  strcpy(text, "Wklej (Ctrl+V)");
		if ( num == EVENT_STUDIO_SIZE      )  strcpy(text, "Wielkość czcionki");
		if ( num == EVENT_STUDIO_TOOL      )  strcpy(text, "Rozkazy (\\key help;)");
		if ( num == EVENT_STUDIO_HELP      )  strcpy(text, "Podręcznik programowania  (\\key prog;)");
		if ( num == EVENT_STUDIO_COMPILE   )  strcpy(text, "Kompiluj");
		if ( num == EVENT_STUDIO_RUN       )  strcpy(text, "Wykonaj/Zatrzymaj");
		if ( num == EVENT_STUDIO_REALTIME  )  strcpy(text, "Pauza/Kontynuuj");
		if ( num == EVENT_STUDIO_STEP      )  strcpy(text, "Jeden krok");
	}

	if ( type == RES_OBJECT )
	{
		if ( num == OBJECT_PORTICO      )  strcpy(text, "Żuraw przesuwalny");
		if ( num == OBJECT_BASE         )  strcpy(text, "Statek kosmiczny");
		if ( num == OBJECT_DERRICK      )  strcpy(text, "Kopalnia");
		if ( num == OBJECT_FACTORY      )  strcpy(text, "Fabryka robotów");
		if ( num == OBJECT_REPAIR       )  strcpy(text, "Warsztat");
		if ( num == OBJECT_DESTROYER    )  strcpy(text, "Destroyer");
		if ( num == OBJECT_STATION      )  strcpy(text, "Stacja energetyczna");
		if ( num == OBJECT_CONVERT      )  strcpy(text, "Przetop rudę na tytan");
		if ( num == OBJECT_TOWER        )  strcpy(text, "Wieża obronna");
		if ( num == OBJECT_NEST         )  strcpy(text, "Gniazdo");
		if ( num == OBJECT_RESEARCH     )  strcpy(text, "Centrum badawcze");
		if ( num == OBJECT_RADAR        )  strcpy(text, "Stacja radarowa");
		if ( num == OBJECT_INFO         )  strcpy(text, "Stacja przekaźnikowa informacji");
#if _TEEN
		if ( num == OBJECT_ENERGY       )  strcpy(text, "Fabryka ogniw elektrycznych");
#else
		if ( num == OBJECT_ENERGY       )  strcpy(text, "Fabryka ogniw elektrycznych");
#endif
		if ( num == OBJECT_LABO         )  strcpy(text, "Laboratorium");
		if ( num == OBJECT_NUCLEAR      )  strcpy(text, "Elektrownia atomowa");
		if ( num == OBJECT_PARA         )  strcpy(text, "Odgromnik");
		if ( num == OBJECT_SAFE         )  strcpy(text, "Skrytka");
		if ( num == OBJECT_HUSTON       )  strcpy(text, "Centrum Kontroli Misji w Houston");
		if ( num == OBJECT_TARGET1      )  strcpy(text, "Cel");
		if ( num == OBJECT_TARGET2      )  strcpy(text, "Cel");
		if ( num == OBJECT_START        )  strcpy(text, "Początek");
		if ( num == OBJECT_END          )  strcpy(text, "Koniec");
		if ( num == OBJECT_STONE        )  strcpy(text, "Ruda tytanu");
		if ( num == OBJECT_URANIUM      )  strcpy(text, "Ruda uranu");
		if ( num == OBJECT_BULLET       )  strcpy(text, "Materia organiczna");
		if ( num == OBJECT_METAL        )  strcpy(text, "Tytan");
		if ( num == OBJECT_POWER        )  strcpy(text, "Ogniwo elektryczne");
		if ( num == OBJECT_ATOMIC       )  strcpy(text, "Atomowe ogniwa elektryczne");
		if ( num == OBJECT_BBOX         )  strcpy(text, "Czarna skrzynka");
		if ( num == OBJECT_KEYa         )  strcpy(text, "Klucz A");
		if ( num == OBJECT_KEYb         )  strcpy(text, "Klucz B");
		if ( num == OBJECT_KEYc         )  strcpy(text, "Klucz C");
		if ( num == OBJECT_KEYd         )  strcpy(text, "Klucz D");
		if ( num == OBJECT_TNT          )  strcpy(text, "Materiały wybuchowe");
		if ( num == OBJECT_BOMB         )  strcpy(text, "Mina");
		if ( num == OBJECT_BAG          )  strcpy(text, "Zestaw przetrwania");
		if ( num == OBJECT_WAYPOINT     )  strcpy(text, "Punkt kontrolny");
		if ( num == OBJECT_FLAGb        )  strcpy(text, "Niebieska flaga");
		if ( num == OBJECT_FLAGr        )  strcpy(text, "Czerwona flaga");
		if ( num == OBJECT_FLAGg        )  strcpy(text, "Zielona flaga");
		if ( num == OBJECT_FLAGy        )  strcpy(text, "Żółta flaga");
		if ( num == OBJECT_FLAGv        )  strcpy(text, "Fioletowa flaga");
		if ( num == OBJECT_MARKPOWER    )  strcpy(text, "Źródło energii (miejsce na elektrownię)");
		if ( num == OBJECT_MARKURANIUM  )  strcpy(text, "Złoże uranu (miejsce na kopalnię)");
		if ( num == OBJECT_MARKKEYa     )  strcpy(text, "Znaleziono klucz A (miejsce na kopalnię)");
		if ( num == OBJECT_MARKKEYb     )  strcpy(text, "Znaleziono klucz B (miejsce na kopalnię)");
		if ( num == OBJECT_MARKKEYc     )  strcpy(text, "Znaleziono klucz C (miejsce na kopalnię)");
		if ( num == OBJECT_MARKKEYd     )  strcpy(text, "Znaleziono klucz D (miejsce na kopalnię)");
		if ( num == OBJECT_MARKSTONE    )  strcpy(text, "Złoże tytanu (miejsce na kopalnię)");
		if ( num == OBJECT_MOBILEft     )  strcpy(text, "Robot treningowy");
		if ( num == OBJECT_MOBILEtt     )  strcpy(text, "Robot treningowy");
		if ( num == OBJECT_MOBILEwt     )  strcpy(text, "Robot treningowy");
		if ( num == OBJECT_MOBILEit     )  strcpy(text, "Robot treningowy");
		if ( num == OBJECT_MOBILEfa     )  strcpy(text, "Transporter latający");
		if ( num == OBJECT_MOBILEta     )  strcpy(text, "Transporter na gąsienicach");
		if ( num == OBJECT_MOBILEwa     )  strcpy(text, "Transporter na kołach");
		if ( num == OBJECT_MOBILEia     )  strcpy(text, "Transporter na nogach");
		if ( num == OBJECT_MOBILEfc     )  strcpy(text, "Działo latające");
		if ( num == OBJECT_MOBILEtc     )  strcpy(text, "Działo na gąsienicach");
		if ( num == OBJECT_MOBILEwc     )  strcpy(text, "Działo na kołach");
		if ( num == OBJECT_MOBILEic     )  strcpy(text, "Działo na nogach");
		if ( num == OBJECT_MOBILEfi     )  strcpy(text, "Latające działo organiczne");
		if ( num == OBJECT_MOBILEti     )  strcpy(text, "Działo organiczne na gąsienicach");
		if ( num == OBJECT_MOBILEwi     )  strcpy(text, "Działo organiczne na kołach");
		if ( num == OBJECT_MOBILEii     )  strcpy(text, "Działo organiczne na nogach");
		if ( num == OBJECT_MOBILEfs     )  strcpy(text, "Szperacz latający");
		if ( num == OBJECT_MOBILEts     )  strcpy(text, "Szperacz na gąsienicach");
		if ( num == OBJECT_MOBILEws     )  strcpy(text, "Szperacz na kołach");
		if ( num == OBJECT_MOBILEis     )  strcpy(text, "Szperacz na nogach");
		if ( num == OBJECT_MOBILErt     )  strcpy(text, "Uderzacz");
		if ( num == OBJECT_MOBILErc     )  strcpy(text, "Działo fazowe");
		if ( num == OBJECT_MOBILErr     )  strcpy(text, "Recykler");
		if ( num == OBJECT_MOBILErs     )  strcpy(text, "Osłaniacz");
		if ( num == OBJECT_MOBILEsa     )  strcpy(text, "Robot nurek");
		if ( num == OBJECT_MOBILEtg     )  strcpy(text, "Robot cel");
		if ( num == OBJECT_MOBILEdr     )  strcpy(text, "Drawer bot");
		if ( num == OBJECT_HUMAN        )  strcpy(text, g_gamerName);
		if ( num == OBJECT_TECH         )  strcpy(text, "Inżynier");
		if ( num == OBJECT_TOTO         )  strcpy(text, "Robbie");
		if ( num == OBJECT_MOTHER       )  strcpy(text, "Królowa Obcych");
		if ( num == OBJECT_ANT          )  strcpy(text, "Mrówka");
		if ( num == OBJECT_SPIDER       )  strcpy(text, "Pająk");
		if ( num == OBJECT_BEE          )  strcpy(text, "Osa");
		if ( num == OBJECT_WORM         )  strcpy(text, "Robal");
		if ( num == OBJECT_EGG          )  strcpy(text, "Jajo");
		if ( num == OBJECT_RUINmobilew1 )  strcpy(text, "Wrak");
		if ( num == OBJECT_RUINmobilew2 )  strcpy(text, "Wrak");
		if ( num == OBJECT_RUINmobilet1 )  strcpy(text, "Wrak");
		if ( num == OBJECT_RUINmobilet2 )  strcpy(text, "Wrak");
		if ( num == OBJECT_RUINmobiler1 )  strcpy(text, "Wrak");
		if ( num == OBJECT_RUINmobiler2 )  strcpy(text, "Wrak");
		if ( num == OBJECT_RUINfactory  )  strcpy(text, "Ruiny");
		if ( num == OBJECT_RUINdoor     )  strcpy(text, "Ruiny");
		if ( num == OBJECT_RUINsupport  )  strcpy(text, "Odpady");
		if ( num == OBJECT_RUINradar    )  strcpy(text, "Ruiny");
		if ( num == OBJECT_RUINconvert  )  strcpy(text, "Ruiny");
		if ( num == OBJECT_RUINbase     )  strcpy(text, "Ruiny statku kosmicznego");
		if ( num == OBJECT_RUINhead     )  strcpy(text, "Ruiny statku kosmicznego");
		if ( num == OBJECT_APOLLO1      ||
			 num == OBJECT_APOLLO3      ||
			 num == OBJECT_APOLLO4      ||
			 num == OBJECT_APOLLO5      )  strcpy(text, "Pozostałości z misji Apollo");
		if ( num == OBJECT_APOLLO2      )  strcpy(text, "Pojazd Księżycowy");
	}

	if ( type == RES_ERR )
	{
		strcpy(text, "Błąd");
		if ( num == ERR_CMD             )  strcpy(text, "Nieznane polecenie");
#if _NEWLOOK
		if ( num == ERR_INSTALL         )  strcpy(text, "Gra CeeBot nie jest zainstalowana.");
		if ( num == ERR_NOCD            )  strcpy(text, "Włóż dysk CD z grą CeeBot\ni uruchom grę jeszcze raz.");
#else
		if ( num == ERR_INSTALL         )  strcpy(text, "Gra COLOBOT nie jest zainstalowana.");
		if ( num == ERR_NOCD            )  strcpy(text, "Włóż dysk CD z grą COLOBOT\ni uruchom grę jeszcze raz.");
#endif
		if ( num == ERR_MANIP_VEH       )  strcpy(text, "Nieodpowiedni robot");
		if ( num == ERR_MANIP_FLY       )  strcpy(text, "Niemożliwe podczas lotu");
		if ( num == ERR_MANIP_BUSY      )  strcpy(text, "Nie można nieść więcej przedmiotów");
		if ( num == ERR_MANIP_NIL       )  strcpy(text, "Nie ma nic do podniesienia");
		if ( num == ERR_MANIP_MOTOR     )  strcpy(text, "Niemożliwe podczas ruchu");
		if ( num == ERR_MANIP_OCC       )  strcpy(text, "Miejsce zajęte");
		if ( num == ERR_MANIP_FRIEND    )  strcpy(text, "Brak innego robota");
		if ( num == ERR_MANIP_RADIO     )  strcpy(text, "Nie możesz przenosić przedmiotów radioaktywnych");
		if ( num == ERR_MANIP_WATER     )  strcpy(text, "Nie możesz przenosić przedmiotów pod wodą");
		if ( num == ERR_MANIP_EMPTY     )  strcpy(text, "Nie ma nic do upuszczenia");
		if ( num == ERR_BUILD_FLY       )  strcpy(text, "Niemożliwe podczas lotu");
		if ( num == ERR_BUILD_WATER     )  strcpy(text, "Niemożliwe pod wodą");
		if ( num == ERR_BUILD_ENERGY    )  strcpy(text, "Za mało energii");
		if ( num == ERR_BUILD_METALAWAY )  strcpy(text, "Tytan za daleko");
		if ( num == ERR_BUILD_METALNEAR )  strcpy(text, "Tytan za blisko");
		if ( num == ERR_BUILD_METALINEX )  strcpy(text, "Brak tytanu w pobliżu");
		if ( num == ERR_BUILD_FLAT      )  strcpy(text, "Powierzchnia nie jest wystarczająco płaska");
		if ( num == ERR_BUILD_FLATLIT   )  strcpy(text, "Za mało płaskiego terenu");
		if ( num == ERR_BUILD_BUSY      )  strcpy(text, "Miejsce zajęte");
		if ( num == ERR_BUILD_BASE      )  strcpy(text, "Za blisko statku kosmicznego");
		if ( num == ERR_BUILD_NARROW    )  strcpy(text, "Za blisko budynku");
		if ( num == ERR_BUILD_MOTOR     )  strcpy(text, "Niemożliwe podczas ruchu");
		if ( num == ERR_SEARCH_FLY      )  strcpy(text, "Niemożliwe podczas lotu");
		if ( num == ERR_SEARCH_VEH      )  strcpy(text, "Nieodpowiedni robot");
		if ( num == ERR_SEARCH_MOTOR    )  strcpy(text, "Niemożliwe podczas ruchu");
		if ( num == ERR_TERRA_VEH       )  strcpy(text, "Nieodpowiedni robot");
		if ( num == ERR_TERRA_ENERGY    )  strcpy(text, "Za mało energii");
		if ( num == ERR_TERRA_FLOOR     )  strcpy(text, "Nieodpowiedni teren");
		if ( num == ERR_TERRA_BUILDING  )  strcpy(text, "Budynek za blisko");
		if ( num == ERR_TERRA_OBJECT    )  strcpy(text, "Obiekt za blisko");
		if ( num == ERR_RECOVER_VEH     )  strcpy(text, "Nieodpowiedni robot");
		if ( num == ERR_RECOVER_ENERGY  )  strcpy(text, "Za mało energii");
		if ( num == ERR_RECOVER_NULL    )  strcpy(text, "Nie ma niczego do odzysku");
		if ( num == ERR_SHIELD_VEH      )  strcpy(text, "Nieodpowiedni robot");
		if ( num == ERR_SHIELD_ENERGY   )  strcpy(text, "Nie ma więcej energii");
		if ( num == ERR_MOVE_IMPOSSIBLE )  strcpy(text, "Błąd w poleceniu ruchu");
		if ( num == ERR_FIND_IMPOSSIBLE )  strcpy(text, "Obiekt nieznany");
		if ( num == ERR_GOTO_IMPOSSIBLE )  strcpy(text, "Goto: miejsce docelowe niedostępne");
		if ( num == ERR_GOTO_ITER       )  strcpy(text, "Goto: miejsce docelowe niedostępne");
		if ( num == ERR_GOTO_BUSY       )  strcpy(text, "Goto: miejsce docelowe zajęte");
		if ( num == ERR_FIRE_VEH        )  strcpy(text, "Nieodpowiedni robot");
		if ( num == ERR_FIRE_ENERGY     )  strcpy(text, "Za mało energii");
		if ( num == ERR_FIRE_FLY        )  strcpy(text, "Niemożliwe podczas lotu");
		if ( num == ERR_CONVERT_EMPTY   )  strcpy(text, "Brak rudy tytanu do przetopienia");
		if ( num == ERR_DERRICK_NULL    )  strcpy(text, "W ziemi nie ma żadnej rudy");
		if ( num == ERR_STATION_NULL    )  strcpy(text, "Brak energii w ziemi");
		if ( num == ERR_TOWER_POWER     )  strcpy(text, "Brak ogniwa elektrycznego");
		if ( num == ERR_TOWER_ENERGY    )  strcpy(text, "Nie ma więcej energii");
		if ( num == ERR_RESEARCH_POWER  )  strcpy(text, "Brak ogniwa elektrycznego");
		if ( num == ERR_RESEARCH_ENERGY )  strcpy(text, "Za mało energii");
		if ( num == ERR_RESEARCH_TYPE   )  strcpy(text, "Nieodpowiedni rodzaj ogniw");
		if ( num == ERR_RESEARCH_ALREADY)  strcpy(text, "Program badawczy został już wykonany");
		if ( num == ERR_ENERGY_NULL     )  strcpy(text, "Brak energii w ziemi");
		if ( num == ERR_ENERGY_LOW      )  strcpy(text, "Wciąż za mało energii");
		if ( num == ERR_ENERGY_EMPTY    )  strcpy(text, "Brak tytanu do przetworzenia");
		if ( num == ERR_ENERGY_BAD      )  strcpy(text, "Przetwarza jedynie tytan");
		if ( num == ERR_BASE_DLOCK      )  strcpy(text, "Drzwi zablokowane przez robota lub inny obiekt ");
		if ( num == ERR_BASE_DHUMAN     )  strcpy(text, "Musisz być na statku kosmicznym aby nim odlecieć");
		if ( num == ERR_LABO_NULL       )  strcpy(text, "Nie ma niczego do zanalizowania");
		if ( num == ERR_LABO_BAD        )  strcpy(text, "Analizuje jedynie materię organiczną");
		if ( num == ERR_LABO_ALREADY    )  strcpy(text, "Analiza została już wykonana");
		if ( num == ERR_NUCLEAR_NULL    )  strcpy(text, "Brak energii w ziemi");
		if ( num == ERR_NUCLEAR_LOW     )  strcpy(text, "Wciąż za mało energii");
		if ( num == ERR_NUCLEAR_EMPTY   )  strcpy(text, "Brak uranu do przetworzenia");
		if ( num == ERR_NUCLEAR_BAD     )  strcpy(text, "Przetwarza jedynie uran");
		if ( num == ERR_FACTORY_NULL    )  strcpy(text, "Brak tytanu");
		if ( num == ERR_FACTORY_NEAR    )  strcpy(text, "Obiekt za blisko");
		if ( num == ERR_RESET_NEAR      )  strcpy(text, "Miejsce zajęte");
		if ( num == ERR_INFO_NULL       )  strcpy(text, "Nie ma żadnej stacji przekaźnikowej w zasięgu");
		if ( num == ERR_VEH_VIRUS       )  strcpy(text, "Program zawirusowany");
		if ( num == ERR_BAT_VIRUS       )  strcpy(text, "Zainfekowane wirusem, chwilowo niesprawne");
		if ( num == ERR_VEH_POWER       )  strcpy(text, "Brak ogniwa elektrycznego");
		if ( num == ERR_VEH_ENERGY      )  strcpy(text, "Nie ma więcej energii");
		if ( num == ERR_FLAG_FLY        )  strcpy(text, "Niemożliwe podczas lotu");
		if ( num == ERR_FLAG_WATER      )  strcpy(text, "Niemożliwe podczas pływania");
		if ( num == ERR_FLAG_MOTOR      )  strcpy(text, "Niemożliwe podczas ruchu");
		if ( num == ERR_FLAG_BUSY       )  strcpy(text, "Niemożliwe podczas przenoszenia przedmiotu");
		if ( num == ERR_FLAG_CREATE     )  strcpy(text, "Za dużo flag w tym kolorze (maksymalnie 5)");
		if ( num == ERR_FLAG_PROXY      )  strcpy(text, "Za blisko istniejącej flagi");
		if ( num == ERR_FLAG_DELETE     )  strcpy(text, "Nie ma flagi w pobliżu");
		if ( num == ERR_MISSION_NOTERM  )  strcpy(text, "Misja nie jest wypełniona (naciśnij \\key help; aby uzyskać szczegóły)");
		if ( num == ERR_DELETEMOBILE    )  strcpy(text, "Robot zniszczony");
		if ( num == ERR_DELETEBUILDING  )  strcpy(text, "Budynek zniszczony");
		if ( num == ERR_TOOMANY         )  strcpy(text, "Nie można tego utworzyć, za dużo obiektów");
		if ( num == ERR_OBLIGATORYTOKEN )  strcpy(text, "It misses \"%s\" in this exercise");
		if ( num == ERR_PROHIBITEDTOKEN )  strcpy(text, "Do not use in this exercise");

		if ( num == INFO_BUILD          )  strcpy(text, "Budowa zakończona");
		if ( num == INFO_CONVERT        )  strcpy(text, "Tytan dostępny");
		if ( num == INFO_RESEARCH       )  strcpy(text, "Program badawczy zakończony");
		if ( num == INFO_RESEARCHTANK   )  strcpy(text, "Dostępne plany tranporterów na gąsienicach");
		if ( num == INFO_RESEARCHFLY    )  strcpy(text, "Możesz latać używając klawiszy (\\key gup;) oraz (\\key gdown;)");
		if ( num == INFO_RESEARCHTHUMP  )  strcpy(text, "Dostępne plany robota uderzacza");
		if ( num == INFO_RESEARCHCANON  )  strcpy(text, "Dostępne plany działa");
		if ( num == INFO_RESEARCHTOWER  )  strcpy(text, "Dostępne plany wieży obronnej");
		if ( num == INFO_RESEARCHPHAZER )  strcpy(text, "Dostępne plany działa fazowego");
		if ( num == INFO_RESEARCHSHIELD )  strcpy(text, "Dostępne plany robota osłaniacza");
		if ( num == INFO_RESEARCHATOMIC )  strcpy(text, "Dostępne plany elektrowni atomowej");
		if ( num == INFO_FACTORY        )  strcpy(text, "Dostępny nowy robot");
		if ( num == INFO_LABO           )  strcpy(text, "Analiza wykonana");
		if ( num == INFO_ENERGY         )  strcpy(text, "Wytworzono ogniwo elektryczne");
		if ( num == INFO_NUCLEAR        )  strcpy(text, "Wytworzono atomowe ogniwo elektryczne");
		if ( num == INFO_FINDING        )  strcpy(text, "Znaleziono użyteczny przedmiot");
		if ( num == INFO_MARKPOWER      )  strcpy(text, "Znaleziono miejsce na elektrownię");
		if ( num == INFO_MARKURANIUM    )  strcpy(text, "Znaleziono miejsce na kopalnię");
		if ( num == INFO_MARKSTONE      )  strcpy(text, "Znaleziono miejsce na kopalnię");
		if ( num == INFO_MARKKEYa       )  strcpy(text, "Znaleziono miejsce na kopalnię");
		if ( num == INFO_MARKKEYb       )  strcpy(text, "Znaleziono miejsce na kopalnię");
		if ( num == INFO_MARKKEYc       )  strcpy(text, "Znaleziono miejsce na kopalnię");
		if ( num == INFO_MARKKEYd       )  strcpy(text, "Znaleziono miejsce na kopalnię");
		if ( num == INFO_WIN            )  strcpy(text, "<<< Dobra robota, misja wypełniona >>>");
		if ( num == INFO_LOST           )  strcpy(text, "<<< Niestety, misja nie powiodła się >>>");
		if ( num == INFO_LOSTq          )  strcpy(text, "<<< Niestety, misja nie powiodła się >>>");
		if ( num == INFO_WRITEOK        )  strcpy(text, "Bieżąca misja zapisana");
		if ( num == INFO_DELETEPATH     )  strcpy(text, "Przekroczono punkt kontrolny");
		if ( num == INFO_DELETEMOTHER   )  strcpy(text, "Królowa Obcych została zabita");
		if ( num == INFO_DELETEANT      )  strcpy(text, "Mrówka śmiertelnie raniona");
		if ( num == INFO_DELETEBEE      )  strcpy(text, "Osa śmiertelnie raniona");
		if ( num == INFO_DELETEWORM     )  strcpy(text, "Robal śmiertelnie raniony");
		if ( num == INFO_DELETESPIDER   )  strcpy(text, "Pająk śmiertelnie raniony");
		if ( num == INFO_BEGINSATCOM    )  strcpy(text, "Naciśnij klawisz \\key help; aby wyświetlić rozkazy na przekaźniku SatCom");
	}

	if ( type == RES_CBOT )
	{
		strcpy(text, "Błąd");
		if ( num == TX_OPENPAR       ) strcpy(text, "Brak nawiasu otwierającego");
		if ( num == TX_CLOSEPAR      ) strcpy(text, "Brak nawiasu zamykającego");
		if ( num == TX_NOTBOOL       ) strcpy(text, "Wyrażenie musi zwrócić wartość logiczną");
		if ( num == TX_UNDEFVAR      ) strcpy(text, "Zmienna nie została zadeklarowana");
		if ( num == TX_BADLEFT       ) strcpy(text, "Przypisanie niemożliwe");
		if ( num == TX_ENDOF         ) strcpy(text, "Brak średnika na końcu wiersza");
		if ( num == TX_OUTCASE       ) strcpy(text, "Polecenie ""case"" na zewnątrz bloku ""switch""");
		if ( num == TX_NOTERM        ) strcpy(text, "Polecenie po końcowej klamrze zamykającej");
		if ( num == TX_CLOSEBLK      ) strcpy(text, "Brak końca bloku");
		if ( num == TX_ELSEWITHOUTIF ) strcpy(text, "Polecenie ""else"" bez wystąpienia ""if"" ");
		if ( num == TX_OPENBLK       ) strcpy(text, "Brak klamry otwierającej");// beginning of a block expected?
		if ( num == TX_BADTYPE       ) strcpy(text, "Zły typ dla przypisania");
		if ( num == TX_REDEFVAR      ) strcpy(text, "Zmienna nie może być zadeklarowana dwukrotnie");
		if ( num == TX_BAD2TYPE      ) strcpy(text, "Niezgodne typy operatorów");
		if ( num == TX_UNDEFCALL     ) strcpy(text, "Funkcja nieznana");
		if ( num == TX_MISDOTS       ) strcpy(text, "Brak znaku "" : ");
		if ( num == TX_WHILE         ) strcpy(text, "Brak kluczowego słowa ""while");
		if ( num == TX_BREAK         ) strcpy(text, "Polecenie ""break"" na zewnątrz pętli");
		if ( num == TX_LABEL         ) strcpy(text, "Po etykiecie musi wystąpić ""for"", ""while"", ""do"" lub ""switch""");
		if ( num == TX_NOLABEL       ) strcpy(text, "Taka etykieta nie istnieje");// This label does not exist
		if ( num == TX_NOCASE        ) strcpy(text, "Brak polecenia ""case");
		if ( num == TX_BADNUM        ) strcpy(text, "Brak liczby");
		if ( num == TX_VOID          ) strcpy(text, "Pusty parametr");
		if ( num == TX_NOTYP         ) strcpy(text, "Brak deklaracji typu");
		if ( num == TX_NOVAR         ) strcpy(text, "Brak nazwy zmiennej");
		if ( num == TX_NOFONC        ) strcpy(text, "Brakująca nazwa funkcji");
		if ( num == TX_OVERPARAM     ) strcpy(text, "Za dużo parametrów");
		if ( num == TX_REDEF         ) strcpy(text, "Funkcja już istnieje");
		if ( num == TX_LOWPARAM      ) strcpy(text, "Brak wymaganego parametru");
		if ( num == TX_BADPARAM      ) strcpy(text, "Funkcja o tej nazwie nie akceptuje parametrów tego typu");
		if ( num == TX_NUMPARAM      ) strcpy(text, "Funkcja o tej nazwie nie akceptuje takiej liczby parametrów");
		if ( num == TX_NOITEM        ) strcpy(text, "To nie jest obiekt tej klasy");
		if ( num == TX_DOT           ) strcpy(text, "Ten obiekt nie jest członkiem klasy");
		if ( num == TX_NOCONST       ) strcpy(text, "Brak odpowiedniego konstruktora");
		if ( num == TX_REDEFCLASS    ) strcpy(text, "Taka klasa już istnieje");
		if ( num == TX_CLBRK         ) strcpy(text, "Brak "" ] """);
		if ( num == TX_RESERVED      ) strcpy(text, "Słowo zarezerwowane języka CBOT");
		if ( num == TX_BADNEW        ) strcpy(text, "Zły argument dla funkcji ""new""");
		if ( num == TX_OPBRK         ) strcpy(text, "Oczekiwane "" [ """);
		if ( num == TX_BADSTRING     ) strcpy(text, "Brak łańcucha");
		if ( num == TX_BADINDEX      ) strcpy(text, "Nieprawidłowy typ indeksu");
		if ( num == TX_PRIVATE       ) strcpy(text, "Element prywatny");
		if ( num == TX_NOPUBLIC      ) strcpy(text, "Wymagany publiczny");
		if ( num == TX_DIVZERO       ) strcpy(text, "Dzielenie przez zero");
		if ( num == TX_NOTINIT       ) strcpy(text, "Zmienna nie została zainicjalizowana");
		if ( num == TX_BADTHROW      ) strcpy(text, "Wartość ujemna odrzucona przez ""throw""");// What is this?
		if ( num == TX_NORETVAL      ) strcpy(text, "Funkcja nie zwróciła żadnej wartości ");
		if ( num == TX_NORUN         ) strcpy(text, "Żadna funkcja nie działa");
		if ( num == TX_NOCALL        ) strcpy(text, "Odwołanie do nieznanej funkcji");
		if ( num == TX_NOCLASS       ) strcpy(text, "Taka klasa nie istnieje");
		if ( num == TX_NULLPT        ) strcpy(text, "Obiekt nieznany");
		if ( num == TX_OPNAN         ) strcpy(text, "Działanie niemożliwe z wartością ""nan""");
		if ( num == TX_OUTARRAY      ) strcpy(text, "Dostęp poza tablicę");
		if ( num == TX_STACKOVER     ) strcpy(text, "Przepełnienie stosu");
		if ( num == TX_DELETEDPT     ) strcpy(text, "Nieprawidłowy obiekt");
		if ( num == TX_FILEOPEN      ) strcpy(text, "Nie można otworzyć pliku");
		if ( num == TX_NOTOPEN       ) strcpy(text, "Plik nie jest otwarty");
		if ( num == TX_ERRREAD       ) strcpy(text, "Błąd odczytu");
		if ( num == TX_ERRWRITE      ) strcpy(text, "Błąd zapisu");
	}

	if ( type == RES_KEY )
	{
		if ( num == 0                   )  strcpy(text, "< brak >");
		if ( num == VK_LEFT             )  strcpy(text, "Strzałka w lewo");
		if ( num == VK_RIGHT            )  strcpy(text, "Strzałka w prawo");
		if ( num == VK_UP               )  strcpy(text, "Strzałka w górę");
		if ( num == VK_DOWN             )  strcpy(text, "Strzałka w dół");
		if ( num == VK_CANCEL           )  strcpy(text, "Ctrl-break");
		if ( num == VK_BACK             )  strcpy(text, "<--");
		if ( num == VK_TAB              )  strcpy(text, "Tab");
		if ( num == VK_CLEAR            )  strcpy(text, "Delete");
		if ( num == VK_RETURN           )  strcpy(text, "Enter");
		if ( num == VK_SHIFT            )  strcpy(text, "Shift");
		if ( num == VK_CONTROL          )  strcpy(text, "Ctrl");
		if ( num == VK_MENU             )  strcpy(text, "Alt");
		if ( num == VK_PAUSE            )  strcpy(text, "Pause");
		if ( num == VK_CAPITAL          )  strcpy(text, "Caps Lock");
		if ( num == VK_ESCAPE           )  strcpy(text, "Esc");
		if ( num == VK_SPACE            )  strcpy(text, "Spacja");
		if ( num == VK_PRIOR            )  strcpy(text, "Page Up");
		if ( num == VK_NEXT             )  strcpy(text, "Page Down");
		if ( num == VK_END              )  strcpy(text, "End");
		if ( num == VK_HOME             )  strcpy(text, "Home");
		if ( num == VK_SELECT           )  strcpy(text, "Zaznacz");
		if ( num == VK_EXECUTE          )  strcpy(text, "Wykonaj");
		if ( num == VK_SNAPSHOT         )  strcpy(text, "Print Scrn");
		if ( num == VK_INSERT           )  strcpy(text, "Insert");
		if ( num == VK_DELETE           )  strcpy(text, "Delete");
		if ( num == VK_HELP             )  strcpy(text, "Pomoc");
		if ( num == VK_LWIN             )  strcpy(text, "Lewy klawisz Windows");
		if ( num == VK_RWIN             )  strcpy(text, "Prawy klawisz Windows");
		if ( num == VK_APPS             )  strcpy(text, "Klawisz menu kontekstowego");
		if ( num == VK_NUMPAD0          )  strcpy(text, "Klaw. Num. 0");
		if ( num == VK_NUMPAD1          )  strcpy(text, "Klaw. Num. 1");
		if ( num == VK_NUMPAD2          )  strcpy(text, "Klaw. Num. 2");
		if ( num == VK_NUMPAD3          )  strcpy(text, "Klaw. Num. 3");
		if ( num == VK_NUMPAD4          )  strcpy(text, "Klaw. Num. 4");
		if ( num == VK_NUMPAD5          )  strcpy(text, "Klaw. Num. 5");
		if ( num == VK_NUMPAD6          )  strcpy(text, "Klaw. Num. 6");
		if ( num == VK_NUMPAD7          )  strcpy(text, "Klaw. Num. 7");
		if ( num == VK_NUMPAD8          )  strcpy(text, "Klaw. Num. 8");
		if ( num == VK_NUMPAD9          )  strcpy(text, "Klaw. Num. 9");
		if ( num == VK_MULTIPLY         )  strcpy(text, "Klaw. Num. *");
		if ( num == VK_ADD              )  strcpy(text, "Klaw. Num. +");
		if ( num == VK_SEPARATOR        )  strcpy(text, "Klaw. Num. separator");
		if ( num == VK_SUBTRACT         )  strcpy(text, "Klaw. Num. -");
		if ( num == VK_DECIMAL          )  strcpy(text, "Klaw. Num. .");
		if ( num == VK_DIVIDE           )  strcpy(text, "Klaw. Num. /");
		if ( num == VK_F1               )  strcpy(text, "F1");
		if ( num == VK_F2               )  strcpy(text, "F2");
		if ( num == VK_F3               )  strcpy(text, "F3");
		if ( num == VK_F4               )  strcpy(text, "F4");
		if ( num == VK_F5               )  strcpy(text, "F5");
		if ( num == VK_F6               )  strcpy(text, "F6");
		if ( num == VK_F7               )  strcpy(text, "F7");
		if ( num == VK_F8               )  strcpy(text, "F8");
		if ( num == VK_F9               )  strcpy(text, "F9");
		if ( num == VK_F10              )  strcpy(text, "F10");
		if ( num == VK_F11              )  strcpy(text, "F11");
		if ( num == VK_F12              )  strcpy(text, "F12");
		if ( num == VK_F13              )  strcpy(text, "F13");
		if ( num == VK_F14              )  strcpy(text, "F14");
		if ( num == VK_F15              )  strcpy(text, "F15");
		if ( num == VK_F16              )  strcpy(text, "F16");
		if ( num == VK_F17              )  strcpy(text, "F17");
		if ( num == VK_F18              )  strcpy(text, "F18");
		if ( num == VK_F19              )  strcpy(text, "F19");
		if ( num == VK_F20              )  strcpy(text, "F20");
		if ( num == VK_NUMLOCK          )  strcpy(text, "Num Lock");
		if ( num == VK_SCROLL           )  strcpy(text, "Scroll Lock");
		if ( num == VK_ATTN             )  strcpy(text, "Attn");
		if ( num == VK_CRSEL            )  strcpy(text, "CrSel");
		if ( num == VK_EXSEL            )  strcpy(text, "ExSel");
		if ( num == VK_EREOF            )  strcpy(text, "Erase EOF");
		if ( num == VK_PLAY             )  strcpy(text, "Graj");
		if ( num == VK_ZOOM             )  strcpy(text, "Powiększenie");
		if ( num == VK_PA1              )  strcpy(text, "PA1");
		if ( num == VK_OEM_CLEAR        )  strcpy(text, "Wyczyść");
		if ( num == VK_BUTTON1          )  strcpy(text, "Przycisk 1");
		if ( num == VK_BUTTON2          )  strcpy(text, "Przycisk 2");
		if ( num == VK_BUTTON3          )  strcpy(text, "Przycisk 3");
		if ( num == VK_BUTTON4          )  strcpy(text, "Przycisk 4");
		if ( num == VK_BUTTON5          )  strcpy(text, "Przycisk 5");
		if ( num == VK_BUTTON6          )  strcpy(text, "Przycisk 6");
		if ( num == VK_BUTTON7          )  strcpy(text, "Przycisk 7");
		if ( num == VK_BUTTON8          )  strcpy(text, "Przycisk 8");
		if ( num == VK_BUTTON9          )  strcpy(text, "Przycisk 9");
		if ( num == VK_BUTTON10         )  strcpy(text, "Przycisk 10");
		if ( num == VK_BUTTON11         )  strcpy(text, "Przycisk 11");
		if ( num == VK_BUTTON12         )  strcpy(text, "Przycisk 12");
		if ( num == VK_BUTTON13         )  strcpy(text, "Przycisk 13");
		if ( num == VK_BUTTON14         )  strcpy(text, "Przycisk 14");
		if ( num == VK_BUTTON15         )  strcpy(text, "Przycisk 15");
		if ( num == VK_BUTTON16         )  strcpy(text, "Przycisk 16");
		if ( num == VK_BUTTON17         )  strcpy(text, "Przycisk 17");
		if ( num == VK_BUTTON18         )  strcpy(text, "Przycisk 18");
		if ( num == VK_BUTTON19         )  strcpy(text, "Przycisk 19");
		if ( num == VK_BUTTON20         )  strcpy(text, "Przycisk 20");
		if ( num == VK_BUTTON21         )  strcpy(text, "Przycisk 21");
		if ( num == VK_BUTTON22         )  strcpy(text, "Przycisk 22");
		if ( num == VK_BUTTON23         )  strcpy(text, "Przycisk 23");
		if ( num == VK_BUTTON24         )  strcpy(text, "Przycisk 24");
		if ( num == VK_BUTTON25         )  strcpy(text, "Przycisk 25");
		if ( num == VK_BUTTON26         )  strcpy(text, "Przycisk 26");
		if ( num == VK_BUTTON27         )  strcpy(text, "Przycisk 27");
		if ( num == VK_BUTTON28         )  strcpy(text, "Przycisk 28");
		if ( num == VK_BUTTON29         )  strcpy(text, "Przycisk 29");
		if ( num == VK_BUTTON30         )  strcpy(text, "Przycisk 30");
		if ( num == VK_BUTTON31         )  strcpy(text, "Przycisk 31");
		if ( num == VK_BUTTON32         )  strcpy(text, "Przycisk 32");
		if ( num == VK_WHEELUP          )  strcpy(text, "Kółko w górę");
		if ( num == VK_WHEELDOWN        )  strcpy(text, "Kółko w dół");
	}
#endif

	return ( text[0] != 0 );
}
