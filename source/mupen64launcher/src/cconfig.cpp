/**
 *  @section LICENSE
 *
 *  PickleLauncher
 *  Copyright (C) 2010-2011 Scott Smith
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  @section LOCATION
 */

#include "cconfig.h"

CConfig::CConfig() : CBase(),
        ResetGUI                (false),
#if defined(DEBUG)
        Fullscreen              (false),
#else
        Fullscreen              (true),
#endif
        ScreenFlip              (false),
        UseZipSupport           (true),
        ShowExts                (true),
        ShowHidden              (false),
#if defined(X86)
        ShowPointer             (true),
#else
        ShowPointer             (false),
#endif
        ShowLabels              (true),
        UnusedKeysLaunch        (false),
        UnusedJoysLaunch        (false),
        ReloadLauncher          (true),
        TextScrollOption        (true),
        AutoLayout              (true),
        FilenameArgNoExt        (false),
        FilenameAbsPath         (true),
        EntryFastMode           (ENTRY_FAST_MODE_FILTER),
        MaxEntries              (MAX_ENTRIES),
        ColorButton             (COLOR_BLUE),
        ColorFontButton         (COLOR_WHITE),
        ColorBackground         (COLOR_WHITE),
        ColorFontFiles          (COLOR_BLACK),
        ColorFontFolders        (COLOR_RED),
        AnalogDeadZone          (DEAD_ZONE),
        ScreenWidth             (SCREEN_WIDTH),
        ScreenHeight            (SCREEN_HEIGHT),
        ScreenDepth             (SCREEN_DEPTH),
        CPUClock                (CPU_CLOCK_DEF),
        ScrollSpeed             (SCROLL_SPEED),
        ScrollPauseSpeed        (SCROLL_PAUSE_SPEED),
        EntryYDelta             (0),
        EntryXOffset            (0),
        EntryYOffset            (0),
        ButtonWidthLeft         (0),
        ButtonHeightLeft        (0),
        ButtonWidthRight        (0),
        ButtonHeightRight       (0),
        PreviewWidth            (0),
        PreviewHeight           (0),
        DisplayListMaxWidth     (0),
        FilePathMaxWidth        (0),
        PosX_Title              (0),
        PosY_Title              (0),
        PosX_ButtonLeft         (0),
        PosY_ButtonLeft         (0),
        PosX_ButtonRight        (0),
        PosY_ButtonRight        (0),
        PosX_ListNames          (0),
        PosY_ListNames          (0),
        ScreenRatioW            ((float)ScreenWidth/(float)BASE_WIDTH),
        ScreenRatioH            ((float)ScreenHeight/(float)BASE_HEIGHT),
        ZipPath                 ("ziptemp"),
        PreviewsPath            ("previews"),
        PathFont                ("DejaVuSansMono-Bold.ttf"),
        PathBackground          ("images/background.png"),
        PathPointer             ("images/pointer.png"),
        PathSelectPointer       ("images/selectpointer.png"),
        Delimiter               (DELIMITER),
        ButtonModesLeftEnable   (),
        ButtonModesRightEnable  (),
        PathButtons             (),
        KeyMaps                 (),
        JoyMaps                 (),
        FontSizes               (),
        Colors                  (),
        ColorNames              ()
{
    SetDefaults();
}

CConfig::~CConfig()
{
}

void CConfig::SetDefaults( void )
{
    ButtonModesLeftEnable.resize(  BUTTONS_MAX_LEFT,  true );
    ButtonModesRightEnable.resize( BUTTONS_MAX_RIGHT, true );

    KeyMaps.resize(EVENT_TOTAL);
    KeyMaps.at(EVENT_ONE_UP)     = SDLK_UP;
    KeyMaps.at(EVENT_ONE_DOWN)   = SDLK_DOWN;
    KeyMaps.at(EVENT_PAGE_UP)    = SDLK_LEFT;
    KeyMaps.at(EVENT_PAGE_DOWN)  = SDLK_RIGHT;
    KeyMaps.at(EVENT_DIR_UP)     = SDLK_LCTRL;
    KeyMaps.at(EVENT_DIR_DOWN)   = SDLK_RSHIFT;
    KeyMaps.at(EVENT_CFG_APP)    = SDLK_1;
    KeyMaps.at(EVENT_CFG_ITEM)   = SDLK_2;
    KeyMaps.at(EVENT_SET_ONE)    = SDLK_3;
	KeyMaps.at(EVENT_SET_ALL)    = SDLK_4;
    KeyMaps.at(EVENT_SELECT)     = SDLK_RETURN;
    KeyMaps.at(EVENT_BACK)       = SDLK_LCTRL;
    KeyMaps.at(EVENT_QUIT)       = SDLK_ESCAPE;

    JoyMaps.resize(EVENT_TOTAL);
    JoyMaps.at(EVENT_ONE_UP)     = 0;
    JoyMaps.at(EVENT_ONE_DOWN)   = 1;
    JoyMaps.at(EVENT_PAGE_UP)    = 2;
    JoyMaps.at(EVENT_PAGE_DOWN)  = 3;
    JoyMaps.at(EVENT_DIR_UP)     = 4;
    JoyMaps.at(EVENT_DIR_DOWN)   = 5;
    JoyMaps.at(EVENT_CFG_APP)    = 6;
    JoyMaps.at(EVENT_CFG_ITEM)   = 7;
    JoyMaps.at(EVENT_SET_ONE)    = 8;
	JoyMaps.at(EVENT_SET_ALL)    = 9;
    JoyMaps.at(EVENT_SELECT)     = 10;
    JoyMaps.at(EVENT_BACK)       = 11;
    JoyMaps.at(EVENT_QUIT)       = 12;

    PathButtons.resize(EVENT_TOTAL);
    PathButtons.at(EVENT_ONE_UP)        = "images/button_oneup.png";
    PathButtons.at(EVENT_ONE_DOWN)      = "images/button_onedn.png";
    PathButtons.at(EVENT_PAGE_UP)       = "images/button_pageup.png";
    PathButtons.at(EVENT_PAGE_DOWN)     = "images/button_pagedn.png";
    PathButtons.at(EVENT_DIR_UP)        = "images/button_dirup.png";
    PathButtons.at(EVENT_DIR_DOWN)      = "images/button_dirdn.png";
    PathButtons.at(EVENT_ZIP_MODE)      = "images/button_zipmode.png";
    PathButtons.at(EVENT_CFG_APP)       = "images/button_cfg_app.png";
    PathButtons.at(EVENT_CFG_ITEM)      = "images/button_cfg_item.png";
    PathButtons.at(EVENT_SET_ONE)       = "images/button_set_one.png";
    PathButtons.at(EVENT_SET_ALL)       = "images/button_set_all.png";
    PathButtons.at(EVENT_BACK)          = "images/button_back.png";
    PathButtons.at(EVENT_SELECT)        = "images/button_select.png";
    PathButtons.at(EVENT_QUIT)          = "images/button_quit.png";

    Colors.resize(COLOR_TOTAL);
    ColorNames.resize(COLOR_TOTAL);
    ColorNames.at(COLOR_WHITE)      = "white";      Colors.at(COLOR_WHITE).r    = 255;  Colors.at(COLOR_WHITE).g    = 255;  Colors.at(COLOR_WHITE).b    = 255;
    ColorNames.at(COLOR_YELLOW)     = "yellow";     Colors.at(COLOR_YELLOW).r   = 255;  Colors.at(COLOR_YELLOW).g   = 255;  Colors.at(COLOR_YELLOW).b   = 0;
    ColorNames.at(COLOR_FUCHSIA)    = "fushsia";    Colors.at(COLOR_FUCHSIA).r  = 255;  Colors.at(COLOR_FUCHSIA).g  = 0;    Colors.at(COLOR_FUCHSIA).b  = 255;
    ColorNames.at(COLOR_RED)        = "red";        Colors.at(COLOR_RED).r      = 255;  Colors.at(COLOR_RED).g      = 0;    Colors.at(COLOR_RED).b      = 0;
    ColorNames.at(COLOR_SILVER)     = "silver";     Colors.at(COLOR_SILVER).r   = 192;  Colors.at(COLOR_SILVER).g   = 192;  Colors.at(COLOR_SILVER).b   = 192;
    ColorNames.at(COLOR_GRAY)       = "gray";       Colors.at(COLOR_GRAY).r     = 128;  Colors.at(COLOR_GRAY).g     = 128;  Colors.at(COLOR_GRAY).b     = 128;
    ColorNames.at(COLOR_OLIVE)      = "olive";      Colors.at(COLOR_OLIVE).r    = 128;  Colors.at(COLOR_OLIVE).g    = 128;  Colors.at(COLOR_OLIVE).b    = 0;
    ColorNames.at(COLOR_PURPLE)     = "purple";     Colors.at(COLOR_PURPLE).r   = 128;  Colors.at(COLOR_PURPLE).g   = 0;    Colors.at(COLOR_PURPLE).b   = 128;
    ColorNames.at(COLOR_MAROON)     = "maroon";     Colors.at(COLOR_MAROON).r   = 128;  Colors.at(COLOR_MAROON).g   = 0;    Colors.at(COLOR_MAROON).b   = 0;
    ColorNames.at(COLOR_AQUA)       = "aqua";       Colors.at(COLOR_AQUA).r     = 0;    Colors.at(COLOR_AQUA).g     = 255;  Colors.at(COLOR_AQUA).b     = 255;
    ColorNames.at(COLOR_LIME)       = "lime";       Colors.at(COLOR_LIME).r     = 0;    Colors.at(COLOR_LIME).g     = 255;  Colors.at(COLOR_LIME).b     = 0;
    ColorNames.at(COLOR_TEAL)       = "teal";       Colors.at(COLOR_TEAL).r     = 0;    Colors.at(COLOR_TEAL).g     = 128;  Colors.at(COLOR_TEAL).b     = 128;
    ColorNames.at(COLOR_GREEN)      = "green";      Colors.at(COLOR_GREEN).r    = 0;    Colors.at(COLOR_GREEN).g    = 128;  Colors.at(COLOR_GREEN).b    = 0;
    ColorNames.at(COLOR_BLUE)       = "blue";       Colors.at(COLOR_BLUE).r     = 0;    Colors.at(COLOR_BLUE).g     = 0;    Colors.at(COLOR_BLUE).b     = 255;
    ColorNames.at(COLOR_NAVY)       = "navy";       Colors.at(COLOR_NAVY).r     = 0;    Colors.at(COLOR_NAVY).g     = 0;    Colors.at(COLOR_NAVY).b     = 128;
    ColorNames.at(COLOR_BLACK)      = "black";      Colors.at(COLOR_BLACK).r    = 0;    Colors.at(COLOR_BLACK).g    = 0;    Colors.at(COLOR_BLACK).b    = 0;

    FontSizes.resize(FONT_SIZE_TOTAL, 0);
    FontSizes.at(FONT_SIZE_SMALL)   = (int16_t)( 8*ScreenRatioH);
    FontSizes.at(FONT_SIZE_MEDIUM)  = (int16_t)(12*ScreenRatioH);
    FontSizes.at(FONT_SIZE_LARGE)   = (int16_t)(14*ScreenRatioH);

    EntryYDelta         = ENTRY_Y_DELTA;
    EntryXOffset        = ENTRY_X_OFFSET;
    EntryYOffset        = ENTRY_Y_OFFSET;
    ButtonWidthLeft     = BUTTON_W_LEFT;
    ButtonHeightLeft    = BUTTON_H_LEFT;
    ButtonWidthRight    = BUTTON_W_RIGHT;
    ButtonHeightRight   = BUTTON_H_RIGHT;
    PreviewWidth        = PREVIEW_W;
    PreviewHeight       = PREVIEW_H;
    DisplayListMaxWidth = ENTRY_MAX_W;
    FilePathMaxWidth    = FILEPATH_MAX_W;
}

#define LOAD_INT(X,Y)  if (UnprefixString( line, line, X ) == true) { Y = a_to_i(line); }
#define LOAD_KEY(X,Y)  if (UnprefixString( line, line, X ) == true) { Y = (SDLKey)a_to_i(line); }
#define LOAD_STR(X,Y)  if (UnprefixString( Y, line, X ) == true) {}

int8_t CConfig::Load( const string& location )
{
    uint16_t        i;
    string          line;
    string          label;
    ifstream        fin;

    fin.open(location.c_str(), ios_base::in);

    if (!fin)
    {
        Log( "Failed to open config\n" );
        return 0;   // Dont stop the app if it cant be opened, default values will be used and then save to file.
    }

    // Read in the profile
    if (fin.is_open())
    {
        while (!fin.eof())
        {
            getline(fin,line);

#if defined(DEBUG)
            Log( "%s\n", line.c_str() );
#endif

            if (line.length() > 0)
            {
                LOAD_INT( OPT_SCREEN_WIDTH,         ScreenWidth );
                LOAD_INT( OPT_SCREEN_HEIGHT,        ScreenHeight );
                LOAD_INT( OPT_SCREEN_DEPTH,         ScreenDepth );
                LOAD_INT( OPT_FULLSCREEN,           Fullscreen );
                LOAD_INT( OPT_CPU_CLOCK,            CPUClock );
                LOAD_INT( OPT_USEZIPSUPPORT,        UseZipSupport );
                LOAD_INT( OPT_SHOWEXTS,             ShowExts );
                LOAD_INT( OPT_SHOWHIDDEN,           ShowHidden );
                LOAD_INT( OPT_SHOWPOINTER,          ShowPointer );
                LOAD_INT( OPT_SHOWLABELS,           ShowLabels );
                LOAD_INT( OPT_UNUSED_KEYS_SELECT,   UnusedKeysLaunch );
                LOAD_INT( OPT_UNUSED_JOYS_SELECT,   UnusedJoysLaunch );
                LOAD_INT( OPT_RELOAD_LAUNCHER,      ReloadLauncher );
                LOAD_INT( OPT_TEXT_SCROLL_OPTION,   TextScrollOption );
                LOAD_INT( OPT_FILENAMEARGNOEXT,     FilenameArgNoExt );
                LOAD_INT( OPT_FILEABSPATH,          FilenameAbsPath );
                LOAD_INT( OPT_FONT_SIZE_SMALL,      FontSizes.at(FONT_SIZE_SMALL) );
                LOAD_INT( OPT_FONT_SIZE_MEDIUM,     FontSizes.at(FONT_SIZE_MEDIUM) );
                LOAD_INT( OPT_FONT_SIZE_LARGE,      FontSizes.at(FONT_SIZE_LARGE) );
                LOAD_INT( OPT_ENTRY_FAST_MODE,      EntryFastMode );
                LOAD_INT( OPT_MAX_ENTRIES,          MaxEntries );
                LOAD_INT( OPT_SCROLL_SPEED,         ScrollSpeed );
                LOAD_INT( OPT_SCROLL_PAUSE_SPEED,   ScrollPauseSpeed );
                LOAD_STR( OPT_PROFILE_DELIMITER,    Delimiter );
                // GUI Positions
                LOAD_INT( OPT_AUTOLAYOUT,           AutoLayout );
                LOAD_INT( OPT_POSX_TITLE,           PosX_Title );
                LOAD_INT( OPT_POSY_TITLE,           PosY_Title );
                LOAD_INT( OPT_POSX_BTNLEFT,         PosX_ButtonLeft );
                LOAD_INT( OPT_POSY_BTNLEFT,         PosY_ButtonLeft );
                LOAD_INT( OPT_POSX_BTNRIGHT,        PosX_ButtonRight );
                LOAD_INT( OPT_POSY_BTNRIGHT,        PosY_ButtonRight );
                LOAD_INT( OPT_POSX_LISTNAMES,       PosX_ListNames );
                LOAD_INT( OPT_POSY_LISTNAMES,       PosY_ListNames );
                // GUI Options
                LOAD_INT( OPT_ENTRY_Y_DELTA,        EntryYDelta );
                LOAD_INT( OPT_ENTRY_X_OFFSET,       EntryXOffset );
                LOAD_INT( OPT_ENTRY_Y_OFFSET,       EntryYOffset );
                LOAD_INT( OPT_BUTTON_W_LEFT,        ButtonWidthLeft );
                LOAD_INT( OPT_BUTTON_H_LEFT,        ButtonHeightLeft );
                LOAD_INT( OPT_BUTTON_W_RIGHT,       ButtonWidthRight );
                LOAD_INT( OPT_BUTTON_H_RIGHT,       ButtonHeightRight );
                LOAD_INT( OPT_PREVIEW_W,            PreviewWidth );
                LOAD_INT( OPT_PREVIEW_H,            PreviewHeight );
                LOAD_INT( OPT_ENTRY_MAX_W,          DisplayListMaxWidth );
                LOAD_INT( OPT_FILEPATH_MAX_W,       FilePathMaxWidth );
                // GUI Buttons
                for (i=0; i<ButtonModesLeftEnable.size(); i++)
                {
                    label = string(OPT_BUTTONLEFT_ENABLED) + "_"+ i_to_a(i);
                    LOAD_INT( label,                ButtonModesLeftEnable.at(i) );
                }
                for (i=0; i<ButtonModesRightEnable.size(); i++)
                {
                    label = string(OPT_BUTTONRIGHT_ENABLED) + "_"+ i_to_a(i);
                    LOAD_INT( label,                ButtonModesRightEnable.at(i) );
                }
                // Paths
                LOAD_STR( OPT_PATH_ZIPTEMP,         ZipPath );
                LOAD_STR( OPT_PATH_PREVIEWS,        PreviewsPath );
                LOAD_STR( OPT_PATH_FONT,            PathFont );
                LOAD_STR( OPT_PATH_BACKGND,         PathBackground );
                LOAD_STR( OPT_PATH_POINTER,         PathPointer );
                LOAD_STR( OPT_PATH_POINTER,         PathPointer );
                LOAD_STR( OPT_PATH_ONEUP,           PathButtons.at(EVENT_ONE_UP) );
                LOAD_STR( OPT_PATH_ONEDN,           PathButtons.at(EVENT_ONE_DOWN) );
                LOAD_STR( OPT_PATH_PGUP,            PathButtons.at(EVENT_PAGE_UP) );
                LOAD_STR( OPT_PATH_PGDN,            PathButtons.at(EVENT_PAGE_DOWN) );
                LOAD_STR( OPT_PATH_DIRUP,           PathButtons.at(EVENT_DIR_UP) );
                LOAD_STR( OPT_PATH_DIRDN,           PathButtons.at(EVENT_DIR_DOWN) );
                LOAD_STR( OPT_PATH_CFG_APP,         PathButtons.at(EVENT_CFG_APP) );
                LOAD_STR( OPT_PATH_CFG_ITEM,        PathButtons.at(EVENT_CFG_ITEM) );
                LOAD_STR( OPT_PATH_SELECT,          PathButtons.at(EVENT_SELECT) );
                LOAD_STR( OPT_PATH_QUIT,            PathButtons.at(EVENT_QUIT) );
                // Color mappings
                LOAD_INT( OPT_COLOR_BUTTON,         ColorButton );
                LOAD_INT( OPT_COLOR_FONTBUTTON,     ColorFontButton );
                LOAD_INT( OPT_COLOR_BACKGND,        ColorBackground );
                LOAD_INT( OPT_COLOR_FONTFILES,      ColorFontFiles );
                LOAD_INT( OPT_COLOR_FONTFOLDERS,    ColorFontFolders );
                // Keyboard mappings
                LOAD_KEY( OPT_KEYUP,                KeyMaps.at(EVENT_ONE_UP) );
                LOAD_KEY( OPT_KEYDOWN,              KeyMaps.at(EVENT_ONE_DOWN) );
                LOAD_KEY( OPT_KEYLEFT ,             KeyMaps.at(EVENT_PAGE_UP) );
                LOAD_KEY( OPT_KEYRIGHT,             KeyMaps.at(EVENT_PAGE_DOWN) );
                LOAD_KEY( OPT_KEYDIRUP,             KeyMaps.at(EVENT_DIR_UP) );
                LOAD_KEY( OPT_KEYDIRDOWN,           KeyMaps.at(EVENT_DIR_DOWN) );
                LOAD_KEY( OPT_KEYCFGAPP,            KeyMaps.at(EVENT_CFG_APP) );
                LOAD_KEY( OPT_KEYCFGENTRY,          KeyMaps.at(EVENT_CFG_ITEM) );
				LOAD_KEY( OPT_KEYSETONE,            KeyMaps.at(EVENT_SET_ONE) );
				LOAD_KEY( OPT_KEYSETALL,            KeyMaps.at(EVENT_SET_ALL) );
                LOAD_KEY( OPT_KEYSELECT,            KeyMaps.at(EVENT_SELECT) );
				LOAD_KEY( OPT_KEYBACK,              KeyMaps.at(EVENT_BACK) );
                LOAD_KEY( OPT_KEYQUIT,              KeyMaps.at(EVENT_QUIT) );
                // Joystick mappings
                LOAD_INT( OPT_JOYUP,                JoyMaps.at(EVENT_ONE_UP) );
                LOAD_INT( OPT_JOYDOWN,              JoyMaps.at(EVENT_ONE_DOWN) );
                LOAD_INT( OPT_JOYLEFT,              JoyMaps.at(EVENT_PAGE_UP) );
                LOAD_INT( OPT_JOYRIGHT,             JoyMaps.at(EVENT_PAGE_DOWN) );
                LOAD_INT( OPT_JOYDIRUP,             JoyMaps.at(EVENT_DIR_UP) );
                LOAD_INT( OPT_JOYDIRDOWN,           JoyMaps.at(EVENT_DIR_DOWN) );
                LOAD_INT( OPT_JOYCFGAPP,            JoyMaps.at(EVENT_CFG_APP) );
                LOAD_INT( OPT_JOYCFGENTRY,          JoyMaps.at(EVENT_CFG_ITEM) );
				LOAD_INT( OPT_JOYSETONE,            JoyMaps.at(EVENT_SET_ONE) );
				LOAD_INT( OPT_JOYSETALL,            JoyMaps.at(EVENT_SET_ALL) );
                LOAD_INT( OPT_JOYSELECT,            JoyMaps.at(EVENT_SELECT) );
                LOAD_INT( OPT_JOYBACK,              JoyMaps.at(EVENT_BACK) );
                LOAD_INT( OPT_JOYQUIT,              JoyMaps.at(EVENT_QUIT) );
                // Analog joystick mappings
                LOAD_INT( OPT_DEADZONE,             AnalogDeadZone );
            }
        }
        fin.close();
    }
    else
    {
        Log( "Failed to open config\n" );
        return 1;
    }

    ScreenRatioW = (float)ScreenWidth/(float)BASE_WIDTH;
    ScreenRatioH = (float)ScreenHeight/(float)BASE_HEIGHT;

    if (ResetGUI == true)
    {
        SetDefaults();
    }

    return 0;
}

#define SAVE_LBL(X)         fout << endl << X << endl;
#define SAVE_INT(X,H,V)     fout << setw(CFG_LBL_W) << X << "= " << setw(CFG_VAL_W) << i_to_a(V) << "# " << H << endl
#define SAVE_STR(X,H,V)     fout << setw(CFG_LBL_W) << X << "= " << setw(CFG_VAL_W) << V << "# " << H << endl
#define GET_KEY_NAME(X)     SDL_GetKeyName(KeyMaps.at(X))

int8_t CConfig::Save( const string& location )
{
    uint8_t     i;
    ofstream    fout;
    string      label;

    fout.open(location.c_str(), ios_base::trunc);

    if (!fout)
    {
        Log( "Failed to open profile\n" );
        return 1;
    }

    // Write out the profile
    if (fout.is_open())
    {
        fout.setf( ios::left, ios::adjustfield );
        SAVE_LBL( "# General Settings" );
        SAVE_INT( OPT_SCREEN_WIDTH,         HELP_SCREEN_WIDTH,          ScreenWidth );
        SAVE_INT( OPT_SCREEN_HEIGHT,        HELP_SCREEN_HEIGHT,         ScreenHeight );
        SAVE_INT( OPT_SCREEN_DEPTH,         HELP_SCREEN_DEPTH,          ScreenDepth );
        SAVE_INT( OPT_FULLSCREEN,           HELP_FULLSCREEN,            Fullscreen );
        SAVE_INT( OPT_CPU_CLOCK,            HELP_CPU_CLOCK,             CPUClock );
        SAVE_INT( OPT_USEZIPSUPPORT,        HELP_USEZIPSUPPORT,         UseZipSupport );
        SAVE_INT( OPT_SHOWEXTS,             HELP_SHOWEXTS,              ShowExts );
        SAVE_INT( OPT_SHOWHIDDEN,           HELP_SHOWHIDDEN,            ShowHidden );
        SAVE_INT( OPT_SHOWPOINTER,          HELP_SHOWPOINTER,           ShowPointer );
        SAVE_INT( OPT_SHOWLABELS,           HELP_SHOWLABELS,            ShowLabels );
        SAVE_INT( OPT_UNUSED_KEYS_SELECT,   HELP_UNUSED_KEYS_SELECT,    UnusedKeysLaunch );
        SAVE_INT( OPT_UNUSED_JOYS_SELECT,   HELP_UNUSED_JOYS_SELECT,    UnusedJoysLaunch );
        SAVE_INT( OPT_RELOAD_LAUNCHER,      HELP_RELOAD_LAUNCHER,       ReloadLauncher );
        SAVE_INT( OPT_TEXT_SCROLL_OPTION,   HELP_TEXT_SCROLL_OPTION,    TextScrollOption );
        SAVE_INT( OPT_FILENAMEARGNOEXT,     HELP_FILENAMEARGNOEXT,      FilenameArgNoExt );
        SAVE_INT( OPT_FILEABSPATH,          HELP_FILEABSPATH,           FilenameAbsPath );
        SAVE_INT( OPT_FONT_SIZE_SMALL,      HELP_FONT_SIZE_SMALL,       FontSizes.at(FONT_SIZE_SMALL) );
        SAVE_INT( OPT_FONT_SIZE_MEDIUM,     HELP_FONT_SIZE_MEDIUM,      FontSizes.at(FONT_SIZE_MEDIUM) );
        SAVE_INT( OPT_FONT_SIZE_LARGE,      HELP_FONT_SIZE_LARGE,       FontSizes.at(FONT_SIZE_LARGE) );
        SAVE_INT( OPT_ENTRY_FAST_MODE,      HELP_ENTRY_FAST_MODE,       EntryFastMode );
        SAVE_INT( OPT_MAX_ENTRIES,          HELP_MAX_ENTRIES,           MaxEntries );
        SAVE_INT( OPT_SCROLL_SPEED,         HELP_SCROLL_SPEED,          ScrollSpeed );
        SAVE_INT( OPT_SCROLL_PAUSE_SPEED,   HELP_SCROLL_PAUSE_SPEED,    ScrollPauseSpeed );
        SAVE_STR( OPT_PROFILE_DELIMITER,    HELP_PROFILE_DELIMITER,     Delimiter );
        SAVE_LBL( "# GUI Positions" );
        SAVE_INT( OPT_AUTOLAYOUT,           HELP_AUTOLAYOUT,            AutoLayout );
        SAVE_INT( OPT_POSX_TITLE,           HELP_POSX_TITLE,            PosX_Title );
        SAVE_INT( OPT_POSY_TITLE,           HELP_POSY_TITLE,            PosY_Title );
        SAVE_INT( OPT_POSX_BTNLEFT,         HELP_POSX_BTNLEFT,          PosX_ButtonLeft );
        SAVE_INT( OPT_POSY_BTNLEFT,         HELP_POSY_BTNLEFT,          PosY_ButtonLeft );
        SAVE_INT( OPT_POSX_BTNRIGHT,        HELP_POSX_BTNRIGHT,         PosX_ButtonRight );
        SAVE_INT( OPT_POSY_BTNRIGHT,        HELP_POSY_BTNRIGHT,         PosY_ButtonRight );
        SAVE_INT( OPT_POSX_LISTNAMES,       HELP_POSX_LISTNAMES,        PosX_ListNames );
        SAVE_INT( OPT_POSY_LISTNAMES,       HELP_POSY_LISTNAMES,        PosY_ListNames );
        SAVE_LBL( "# GUI Options" );
        SAVE_INT( OPT_ENTRY_Y_DELTA,        HELP_ENTRY_Y_DELTA,         EntryYDelta );
        SAVE_INT( OPT_ENTRY_X_OFFSET,       HELP_ENTRY_X_OFFSET,        EntryXOffset );
        SAVE_INT( OPT_ENTRY_Y_OFFSET,       HELP_ENTRY_Y_OFFSET,        EntryYOffset );
        SAVE_INT( OPT_BUTTON_W_LEFT,        HELP_BUTTON_W_LEFT,         ButtonWidthLeft );
        SAVE_INT( OPT_BUTTON_H_LEFT,        HELP_BUTTON_H_LEFT,         ButtonHeightLeft );
        SAVE_INT( OPT_BUTTON_W_RIGHT,       HELP_BUTTON_W_RIGHT,        ButtonWidthRight );
        SAVE_INT( OPT_BUTTON_H_RIGHT,       HELP_BUTTON_H_RIGHT,        ButtonHeightRight );
        SAVE_INT( OPT_PREVIEW_W,            HELP_PREVIEW_W,             PreviewWidth );
        SAVE_INT( OPT_PREVIEW_H,            HELP_PREVIEW_H,             PreviewHeight );
        SAVE_INT( OPT_ENTRY_MAX_W,          HELP_ENTRY_MAX_W,           DisplayListMaxWidth );
        SAVE_INT( OPT_FILEPATH_MAX_W,       HELP_FILEPATH_MAX_W,        FilePathMaxWidth );
        SAVE_LBL( "# Button Enable Options" );
        for (i=0; i<ButtonModesLeftEnable.size(); i++)
        {
            label = string(OPT_BUTTONLEFT_ENABLED) + "_"+ i_to_a(i);
            SAVE_INT( label,                HELP_DEFAULT,               ButtonModesLeftEnable.at(i) );
        }
        for (i=0; i<ButtonModesRightEnable.size(); i++)
        {
            label = string(OPT_BUTTONRIGHT_ENABLED) + "_"+ i_to_a(i);
            SAVE_INT( label,                HELP_DEFAULT,               ButtonModesRightEnable.at(i) );
        }
        SAVE_LBL( "# Paths" );
        SAVE_STR( OPT_PATH_ZIPTEMP,         HELP_PATH_ZIPTEMP,          ZipPath );
        SAVE_STR( OPT_PATH_PREVIEWS,        HELP_PATH_PREVIEWS,         PreviewsPath );
        SAVE_STR( OPT_PATH_FONT,            HELP_PATH_FONT,             PathFont );
        SAVE_STR( OPT_PATH_BACKGND,         HELP_PATH_BACKGND,          PathBackground );
        SAVE_STR( OPT_PATH_POINTER,         HELP_PATH_POINTER,          PathPointer );
        SAVE_STR( OPT_PATH_SELECTPOINTER,   HELP_PATH_SELECTPOINTER,    PathSelectPointer );
        SAVE_STR( OPT_PATH_ONEUP,           HELP_PATH_ONEUP,            PathButtons.at(EVENT_ONE_UP) );
        SAVE_STR( OPT_PATH_ONEDN,           HELP_PATH_ONEDN,            PathButtons.at(EVENT_ONE_DOWN) );
        SAVE_STR( OPT_PATH_PGUP,            HELP_PATH_PGUP,             PathButtons.at(EVENT_PAGE_UP) );
        SAVE_STR( OPT_PATH_PGDN,            HELP_PATH_PGDN,             PathButtons.at(EVENT_PAGE_DOWN) );
        SAVE_STR( OPT_PATH_DIRUP,           HELP_PATH_DIRUP,            PathButtons.at(EVENT_DIR_UP) );
        SAVE_STR( OPT_PATH_DIRDN,           HELP_PATH_DIRDN,            PathButtons.at(EVENT_DIR_DOWN) );
        SAVE_STR( OPT_PATH_CFG_APP,         HELP_PATH_CFG_APP,          PathButtons.at(EVENT_CFG_APP) );
        SAVE_STR( OPT_PATH_CFG_ITEM,        HELP_PATH_CFG_ITEM,         PathButtons.at(EVENT_CFG_ITEM) );
        SAVE_STR( OPT_PATH_SELECT,          HELP_PATH_SELECT,           PathButtons.at(EVENT_SELECT) );
        SAVE_STR( OPT_PATH_QUIT,            HELP_PATH_QUIT,             PathButtons.at(EVENT_QUIT) );
        SAVE_LBL( "# Colors" );
        fout << "#   ";
        for (i=0; i<ColorNames.size(); i++)
        {
            fout << setw(4) << i_to_a(i) << setw(20) << ColorNames.at(i);
            if ((i+1) % 3 == 0)
            {
                fout << endl << "#   ";
            }
        }
        fout << endl;
        SAVE_INT( OPT_COLOR_BUTTON,         HELP_COLOR_BUTTON,          ColorButton );
        SAVE_INT( OPT_COLOR_FONTBUTTON,     HELP_COLOR_FONTBUTTON,      ColorFontButton );
        SAVE_INT( OPT_COLOR_BACKGND,        HELP_COLOR_BACKGND,         ColorBackground );
        SAVE_INT( OPT_COLOR_FONTFILES,      HELP_COLOR_FONTFILES,       ColorFontFiles );
        SAVE_INT( OPT_COLOR_FONTFOLDERS,    HELP_COLOR_FONTFOLDERS,     ColorFontFolders );
        SAVE_LBL( "# Controls" );
        SAVE_LBL( "#   Keyboard" );
        SAVE_STR( OPT_KEYUP,                GET_KEY_NAME(EVENT_ONE_UP),     KeyMaps.at(EVENT_ONE_UP) );
        SAVE_STR( OPT_KEYDOWN,              GET_KEY_NAME(EVENT_ONE_DOWN),   KeyMaps.at(EVENT_ONE_DOWN) );
        SAVE_STR( OPT_KEYLEFT,              GET_KEY_NAME(EVENT_PAGE_UP),    KeyMaps.at(EVENT_PAGE_UP) );
        SAVE_STR( OPT_KEYRIGHT,             GET_KEY_NAME(EVENT_PAGE_DOWN),  KeyMaps.at(EVENT_PAGE_DOWN) );
        SAVE_STR( OPT_KEYDIRUP,             GET_KEY_NAME(EVENT_DIR_UP),     KeyMaps.at(EVENT_DIR_UP) );
        SAVE_STR( OPT_KEYDIRDOWN,           GET_KEY_NAME(EVENT_DIR_DOWN),   KeyMaps.at(EVENT_DIR_DOWN) );
        SAVE_STR( OPT_KEYCFGAPP,            GET_KEY_NAME(EVENT_CFG_APP),    KeyMaps.at(EVENT_CFG_APP) );
        SAVE_STR( OPT_KEYCFGENTRY,          GET_KEY_NAME(EVENT_CFG_ITEM),   KeyMaps.at(EVENT_CFG_ITEM) );
		SAVE_STR( OPT_KEYSETONE,            GET_KEY_NAME(EVENT_SET_ONE),    KeyMaps.at(EVENT_SET_ONE) );
		SAVE_STR( OPT_KEYSETALL,            GET_KEY_NAME(EVENT_SET_ALL),    KeyMaps.at(EVENT_SET_ALL) );
        SAVE_STR( OPT_KEYSELECT,            GET_KEY_NAME(EVENT_SELECT),     KeyMaps.at(EVENT_SELECT) );
        SAVE_STR( OPT_KEYBACK,              GET_KEY_NAME(EVENT_BACK),       KeyMaps.at(EVENT_BACK) );
        SAVE_STR( OPT_KEYQUIT,              GET_KEY_NAME(EVENT_QUIT),       KeyMaps.at(EVENT_QUIT) );
        SAVE_LBL( "#   Joystick" );
        SAVE_INT( OPT_JOYUP,                HELP_DEFAULT,                   JoyMaps.at(EVENT_ONE_UP) );
        SAVE_INT( OPT_JOYDOWN,              HELP_DEFAULT,                   JoyMaps.at(EVENT_ONE_DOWN) );
        SAVE_INT( OPT_JOYLEFT,              HELP_DEFAULT,                   JoyMaps.at(EVENT_PAGE_UP) );
        SAVE_INT( OPT_JOYRIGHT,             HELP_DEFAULT,                   JoyMaps.at(EVENT_PAGE_DOWN) );
        SAVE_INT( OPT_JOYDIRUP,             HELP_DEFAULT,                   JoyMaps.at(EVENT_DIR_UP) );
        SAVE_INT( OPT_JOYDIRDOWN,           HELP_DEFAULT,                   JoyMaps.at(EVENT_DIR_DOWN) );
        SAVE_INT( OPT_JOYCFGAPP,            HELP_DEFAULT,                   JoyMaps.at(EVENT_CFG_APP) );
        SAVE_INT( OPT_JOYCFGENTRY,          HELP_DEFAULT,                   JoyMaps.at(EVENT_CFG_ITEM) );
		SAVE_INT( OPT_JOYSETONE,            HELP_DEFAULT,                   JoyMaps.at(EVENT_SET_ONE) );
		SAVE_INT( OPT_JOYSETALL,            HELP_DEFAULT,                   JoyMaps.at(EVENT_SET_ALL) );
        SAVE_INT( OPT_JOYSELECT,            HELP_DEFAULT,                   JoyMaps.at(EVENT_SELECT) );
        SAVE_INT( OPT_JOYBACK,              HELP_DEFAULT,                   JoyMaps.at(EVENT_BACK) );
        SAVE_INT( OPT_JOYQUIT,              HELP_DEFAULT,                   JoyMaps.at(EVENT_QUIT) );
        SAVE_INT( OPT_DEADZONE,             HELP_DEADZONE,                  AnalogDeadZone );
        fout.close();
    }
    else
    {
        Log( "Failed to open profile\n" );
        return 1;
    }
    return 0;
}
