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

#ifndef CCONFIG_H
#define CCONFIG_H

#include "cbase.h"
#include "csystem.h"

using namespace std;

#define SCREEN_FLAGS        SDL_SWSURFACE           /**< Basic SDL screen flags. */
#define BASE_WIDTH          320                     /**< Minimum screen width size (pixels). */
#define BASE_HEIGHT         240                     /**< Maximum screen height size (pixels). */
#if defined(PANDORA) || defined(X86)
#define SCREEN_WIDTH        800                     /**< Default screen width for Pandora and PC (pixels). */
#define SCREEN_HEIGHT       480                     /**< Default screen height for Pandora and PC (pixels). */
#else
#define SCREEN_WIDTH        BASE_WIDTH              /**< Default screen width for any other device (pixels). */
#define SCREEN_HEIGHT       BASE_HEIGHT             /**< Default screen height for any other device (pixels). */
#endif
#define SCREEN_DEPTH        16                      /**< Default screen depth for any device (bits per pixel). */
#define REFRESH_DELAY       10                      /**< Default screen depth for any device (millseconds). */
#define MAX_ENTRIES         10                      /**< Default maximum entries in the display list. */
#define SCROLL_SPEED        2                       /**< Default speed for scrolling text. */
#define SCROLL_PAUSE_SPEED  100                     /**< Default speed for pausing scrolling text when left or right ends are reached. */
#define DEAD_ZONE           10000                   /**< Default analog joystick deadzone. */
#define DELIMITER           ";"                     /**< Default profile delimiter. */
#define CFG_LBL_W           30                      /**< Mininum character width for the profile label. */
#define CFG_VAL_W           30                      /**< Mininum character width for the profile value. */

// GUI Options Defaults
#define ENTRY_Y_DELTA       (2*ScreenRatioH)       /** Scaled pixel amount used between entries. */
#define ENTRY_X_OFFSET      (8*ScreenRatioW)       /** Scaled pixel amount used to separate gui elements along the X axis. */
#define ENTRY_Y_OFFSET      (8*ScreenRatioH)       /** Scaled pixel amount used to separate gui elements along the Y axis. */

#define BUTTONS_MAX_LEFT    7                       /** Number of buttons on the left side of the GUI. */
#define BUTTONS_MAX_RIGHT   4                       /** Number of buttons on the right side of the GUI. */

#define BUTTON_W_LEFT       (25*ScreenRatioW)       /** Scaled pixel width for buttons on the left side. */
#define BUTTON_H_LEFT       (15*ScreenRatioH)       /** Scaled pixel hieght for buttons on the left side. */
#define BUTTON_W_RIGHT      (80*ScreenRatioW)       /** Scaled pixel width for buttons on the right side. */
#define BUTTON_H_RIGHT      (30*ScreenRatioH)       /** Scaled pixel hieght for buttons on the right side. */

#define PREVIEW_W           (80*ScreenRatioW)       /** Scaled pixel width for the preview image. */
#define PREVIEW_H           (60*ScreenRatioH)       /** Scaled pixel height for the preview image. */

#define ENTRY_MAX_W         (ScreenWidth-(BUTTON_W_RIGHT)-(ENTRY_X_OFFSET*2))    /** Max scaled pixel width for an entry. */
#define FILEPATH_MAX_W      (ScreenWidth-(ENTRY_X_OFFSET*2))                     /** Max scaled pixel width for an the current path. */

#define HELP_DEFAULT                ""

// Screen options
#define OPT_SCREEN_WIDTH            "screen_width"
#define HELP_SCREEN_WIDTH           "Screen width in pixels."

#define OPT_SCREEN_HEIGHT           "screen_height"
#define HELP_SCREEN_HEIGHT          "Screen height in pixels."

#define OPT_SCREEN_DEPTH            "screen_depth"
#define HELP_SCREEN_DEPTH           "Screen depth in bits per pixel."

#define OPT_REFRESH_DELAY           "refresh_delay"
#define HELP_REFRESH_DELAY          "Delay in milliseconds after a screen flip."

#define OPT_FULLSCREEN              "fullscreen"
#define HELP_FULLSCREEN             "True if screen is set to fullscreen mode, otherwise false."

// Selector options
#define OPT_CPU_CLOCK               "cpu_clock"
#define HELP_CPU_CLOCK              "If supported, will set the CPU to clock value in Mhz."

#define OPT_USEZIPSUPPORT           "use_zip_support"
#define HELP_USEZIPSUPPORT          "True if launcher uses internal zip support."

#define OPT_SHOWEXTS                "show_exts"
#define HELP_SHOWEXTS               "True if the selector should show file extensions in the filenames, otherwise false."

#define OPT_SHOWHIDDEN              "show_hidden"
#define HELP_SHOWHIDDEN             "True if the selector should show hidden files and folders in the display list, otherwise false."

#define OPT_SHOWPOINTER             "show_pointer"
#define HELP_SHOWPOINTER            "True if the selector should show the SDL pointer, otherwise false."

#define OPT_SHOWLABELS              "show_labels"
#define HELP_SHOWLABELS             "True if the selector should show the button text ttf labels, otherwise false."

#define OPT_UNUSED_KEYS_SELECT      "unused_keys_select"
#define HELP_UNUSED_KEYS_SELECT     "True if any unmapped key events should cause the application launch, otherwise false."

#define OPT_UNUSED_JOYS_SELECT      "unused_buttons_select"
#define HELP_UNUSED_JOYS_SELECT     "True if any unmapped button events should cause the application launch, otherwise false."

#define OPT_RELOAD_LAUNCHER         "reload_launcher"
#define HELP_RELOAD_LAUNCHER        "True if the launcher should reload following the shutdown of the target application, otherwise false."

#define OPT_TEXT_SCROLL_OPTION      "text_scroll_option"
#define HELP_TEXT_SCROLL_OPTION     "True if horizontal the text scroll option should enabled, otherwise false."

#define OPT_FILENAMEARGNOEXT        "filename_arg_no_ext"
#define HELP_FILENAMEARGNOEXT       "True if the extension for file to be loaded is removed, else the extension is not removed from the filename."

#define OPT_FILEABSPATH             "file_abs_path"
#define HELP_FILEABSPATH            "True if the absolute path for the file location should be used to calling the filname, else the path is omitted."

#define OPT_ENTRY_FAST_MODE         "entry_fast_mode"
#define HELP_ENTRY_FAST_MODE        "Fast entry navagation mode, where 0 for alphabetic mode 1 for search filter"

#define OPT_MAX_ENTRIES             "max_entries"
#define HELP_MAX_ENTRIES            "Maximum number of entries to be in the display list."

#define OPT_SCROLL_SPEED            "scroll_speed"
#define HELP_SCROLL_SPEED           "The speed of the horizontal the text scroll speed, lower faster, higher slower.."

#define OPT_SCROLL_PAUSE_SPEED      "scroll_pause_speed"
#define HELP_SCROLL_PAUSE_SPEED     "The time delay of the horizontal the text scroll when the left/right ends are reached, lower shorter, higher longer."

#define OPT_PROFILE_DELIMITER       "profile_delimiter"
#define HELP_PROFILE_DELIMITER      "Delimiter character"

// GUI Positions
#define OPT_AUTOLAYOUT              "autolayout"
#define HELP_AUTOLAYOUT             "True if the launcher should auto position gui elements, if false manual positions are used."

#define OPT_POSX_TITLE              "posx_title"
#define OPT_POSY_TITLE              "posy_title"
#define OPT_POSX_BTNLEFT            "posx_btnleft"
#define OPT_POSY_BTNLEFT            "posy_btnleft"
#define OPT_POSX_BTNRIGHT           "posx_btnright"
#define OPT_POSY_BTNRIGHT           "posy_btnright"
#define OPT_POSX_LISTNAMES          "posx_listnames"
#define OPT_POSY_LISTNAMES          "posy_listnames"

#define HELP_POSX_TITLE             "X coordinate in the upper left corner"
#define HELP_POSY_TITLE             "Y coordinate in the upper left corner"
#define HELP_POSX_BTNLEFT           "X coordinate in the upper left corner"
#define HELP_POSY_BTNLEFT           "Y coordinate in the upper left corner"
#define HELP_POSX_BTNRIGHT          "X coordinate in the bottom left corner"
#define HELP_POSY_BTNRIGHT          "Y coordinate in the bottom left corner"
#define HELP_POSX_LISTNAMES         "X coordinate in the upper left corner"
#define HELP_POSY_LISTNAMES         "Y coordinate in the upper left corner"

// GUI Options
#define OPT_ENTRY_Y_DELTA           "entry_y_delta"
#define OPT_ENTRY_X_OFFSET          "entry_x_offset"
#define OPT_ENTRY_Y_OFFSET          "entry_y_offset"
#define OPT_BUTTON_W_LEFT           "button_w_left"
#define OPT_BUTTON_H_LEFT           "button_h_left"
#define OPT_BUTTON_W_RIGHT          "button_w_right"
#define OPT_BUTTON_H_RIGHT          "button_h_right"
#define OPT_PREVIEW_W               "preview_w"
#define OPT_PREVIEW_H               "preview_h"
#define OPT_ENTRY_MAX_W             "entry_max_w"
#define OPT_FILEPATH_MAX_W          "filepath_max_w"

#define OPT_BUTTONLEFT_ENABLED      "buttonleft_enable"
#define OPT_BUTTONRIGHT_ENABLED     "buttonright_enable"

#define HELP_ENTRY_Y_DELTA          "todo"
#define HELP_ENTRY_X_OFFSET         "todo"
#define HELP_ENTRY_Y_OFFSET         "todo"
#define HELP_BUTTON_W_LEFT          "todo"
#define HELP_BUTTON_H_LEFT          "todo"
#define HELP_BUTTON_W_RIGHT         "todo"
#define HELP_BUTTON_H_RIGHT         "todo"
#define HELP_PREVIEW_W              "todo"
#define HELP_PREVIEW_H              "todo"
#define HELP_ENTRY_MAX_W            "todo"
#define HELP_FILEPATH_MAX_W         "todo"

// Paths
#define OPT_PATH_ZIPTEMP            "zip_temp_path"
#define HELP_PATH_ZIPTEMP           "Path to store extracted files from zip."

#define OPT_PATH_PREVIEWS           "previews_path"
#define HELP_PATH_PREVIEWS          "Path to read preview graphics."

#define OPT_PATH_FONT               "font_path"
#define HELP_PATH_FONT              "Path to ttf font file."

#define OPT_FONT_SIZE_SMALL         "font_size_small"
#define HELP_FONT_SIZE_SMALL        "Font size identified as small."

#define OPT_FONT_SIZE_MEDIUM        "font_size_medium"
#define HELP_FONT_SIZE_MEDIUM       "Font size identified as medium."

#define OPT_FONT_SIZE_LARGE         "font_size_large"
#define HELP_FONT_SIZE_LARGE        "Font size identified as large."

#define OPT_PATH_BACKGND            "image_background"
#define HELP_PATH_BACKGND           "Path to background graphic."

#define OPT_PATH_POINTER            "image_pointer"
#define HELP_PATH_POINTER           "Path to pointer graphic"

#define OPT_PATH_SELECTPOINTER      "image_selectpointer"
#define HELP_PATH_SELECTPOINTER     "Path to selector pointer graphic"

#define OPT_PATH_ONEUP              "image_upone"
#define HELP_PATH_ONEUP             "Path to button graphic for one up."

#define OPT_PATH_ONEDN              "image_downone"
#define HELP_PATH_ONEDN             "Path to button graphic for one down."

#define OPT_PATH_PGUP               "image_pageup"
#define HELP_PATH_PGUP              "Path to button graphic for page up."

#define OPT_PATH_PGDN               "image_pagedown"
#define HELP_PATH_PGDN              "Path to button graphic for page down."

#define OPT_PATH_DIRUP              "image_dirup"
#define HELP_PATH_DIRUP             "Path to button graphic for dir up."

#define OPT_PATH_DIRDN              "image_dirdown"
#define HELP_PATH_DIRDN             "Path to button graphic for dir down."

#define OPT_PATH_CFG_APP            "image_cfgapp"
#define HELP_PATH_CFG_APP           "Path to button graphic for cfg app."

#define OPT_PATH_CFG_ITEM           "image_cfgitem"
#define HELP_PATH_CFG_ITEM          "Path to button graphic for cfg item."

#define OPT_PATH_SELECT             "image_select"
#define HELP_PATH_SELECT            "Path to button graphic for cfg select."

#define OPT_PATH_QUIT               "image_quit"
#define HELP_PATH_QUIT              "Path to button graphic for quit."

// Color GUI options
#define OPT_COLOR_BUTTON            "color_buttons"
#define HELP_COLOR_BUTTON           "Color index used for the coloring the fallback button."

#define OPT_COLOR_FONTBUTTON        "color_fontbuttons"
#define HELP_COLOR_FONTBUTTON       "Color index used for the coloring the font used in the fallback button."

#define OPT_COLOR_BACKGND           "color_background"
#define HELP_COLOR_BACKGND          "Color index used for the coloring the screen background. "

#define OPT_COLOR_FONTFILES         "color_fontfiles"
#define HELP_COLOR_FONTFILES        "Color index used for the coloring the entries that are files."

#define OPT_COLOR_FONTFOLDERS       "color_fontfolders"
#define HELP_COLOR_FONTFOLDERS      "Color index used for the coloring the entries that are folders."

// Keyboard
#define OPT_KEYUP                   "key_up"
#define OPT_KEYDOWN                 "key_down"
#define OPT_KEYLEFT                 "key_left"
#define OPT_KEYRIGHT                "key_right"
#define OPT_KEYDIRUP                "key_dirup"
#define OPT_KEYDIRDOWN              "key_dirdown"
#define OPT_KEYCFGENTRY             "key_cfgentry"
#define OPT_KEYCFGAPP               "key_cfgapp"
#define OPT_KEYSETONE               "key_setone"
#define OPT_KEYSETALL               "key_setall"
#define OPT_KEYSELECT               "key_select"
#define OPT_KEYBACK                 "key_back"
#define OPT_KEYQUIT                 "key_quit"

// Joystick
#define OPT_JOYUP                   "button_up"
#define OPT_JOYDOWN                 "button_down"
#define OPT_JOYLEFT                 "button_left"
#define OPT_JOYRIGHT                "button_right"
#define OPT_JOYDIRUP                "button_dirup"
#define OPT_JOYDIRDOWN              "button_dirdown"
#define OPT_JOYCFGENTRY             "button_cfgentry"
#define OPT_JOYCFGAPP               "button_cfgapp"
#define OPT_JOYSETONE               "button_setone"
#define OPT_JOYSETALL               "button_setall"
#define OPT_JOYSELECT               "button_select"
#define OPT_JOYBACK                 "button_back"
#define OPT_JOYQUIT                 "button_quit"

#define OPT_DEADZONE                "deadzone"
#define HELP_DEADZONE               "Deadzone for analog joysticks."

/** @brief Internal user events
 */
enum EVENT_T {
    EVENT_ONE_UP=0,     /**<  0 Move selection one item up. */
    EVENT_ONE_DOWN,     /**<  1 Move selection one item down. */
    EVENT_PAGE_UP,      /**<  2 Move selection one page up. */
    EVENT_PAGE_DOWN,    /**<  3 Move selection one page down. */
    EVENT_DIR_UP,       /**<  4 Move current path one directory up. */
    EVENT_DIR_DOWN,     /**<  5 Move current path one directory down. */
    EVENT_ZIP_MODE,     /**<  6 Mode to use when extracing files from a zip. */
    EVENT_CFG_APP,      /**<  7 Enter mode to configure the launcher. */
    EVENT_CFG_ITEM,     /**<  8 Enter mode to configure an entry. */
    EVENT_SET_ONE,      /**<  9 For an argument set selected value for the current entry. */
    EVENT_SET_ALL,      /**< 10 For an argument set selected value for the all entry. */
    EVENT_SELECT,       /**< 11 Select the current item. */
    EVENT_BACK,         /**< 12 Go back from the current mode and return to the previous mode. */
    EVENT_QUIT,         /**< 13 Quit the launcher. */
    EVENT_TOTAL,        /**< 14 Total number of events. */
    EVENT_NONE          /**< 15 No event. */
};

/** @brief Mode for navagating to an entry quickly.
 */
enum ENTRY_FAST_MODE_T {
    ENTRY_FAST_MODE_ALPHA=0,
    ENTRY_FAST_MODE_FILTER
};

/** @brief Basic colors
 */
enum COLORS_T {
    COLOR_WHITE=0,
    COLOR_YELLOW,
    COLOR_FUCHSIA,
    COLOR_RED,
    COLOR_SILVER,
    COLOR_GRAY,
    COLOR_OLIVE,
    COLOR_PURPLE,
    COLOR_MAROON,
    COLOR_AQUA,
    COLOR_LIME,
    COLOR_TEAL,
    COLOR_GREEN,
    COLOR_BLUE,
    COLOR_NAVY,
    COLOR_BLACK,
    COLOR_TOTAL
};

/** @brief Font sizes
 */
enum FONT_SIZE_T {
    FONT_SIZE_SMALL=0,
    FONT_SIZE_MEDIUM,
    FONT_SIZE_LARGE,
    FONT_SIZE_TOTAL
};

/** @brief Class that handles loading and saving of all configurable options
 */
class CConfig : public CBase
{
    public:
        /** Constructor. */
        CConfig();
        /** Destructor. */
        virtual ~CConfig();

        /** @brief Set configuration options to calculated defaults
         */
        void    SetDefaults ( void );

        /** @brief Load the configuration option from file
         * @param location : path and filename to config file
         * @return 0 no errors or 1 if an error is detected
         */
        int8_t  Load        ( const string& location );

        /** @brief Save the configuration option from file
         * @param location : path and filename to config file
         * @return 0 no errors or 1 if an error is detected
         */
        int8_t  Save        ( const string& location );

        bool                ResetGUI;               /**< NOT CONFIGURABLE Set via argument and uses scaled defaults and not config file */
        bool                Fullscreen;             /**< CONFIGURABLE Refer to HELP_FULLSCREEN */
        bool                ScreenFlip;             /**< CONFIGURABLE Refer to HELP_SCREENFLIP */
        bool                UseZipSupport;          /**< CONFIGURABLE Refer to HELP_USEZIPSUPPORT */
        bool                ShowExts;               /**< CONFIGURABLE Refer to HELP_SHOWEXTS */
        bool                ShowHidden;             /**< CONFIGURABLE Refer to HELP_SHOWHIDDEN */
        bool                ShowPointer;            /**< CONFIGURABLE Refer to HELP_SHOWPOINTER */
        bool                ShowLabels;             /**< CONFIGURABLE Refer to HELP_SHOWLABELS */
        bool                UnusedKeysLaunch;       /**< CONFIGURABLE Refer to HELP_UNUSED_KEYS_SELECT */
        bool                UnusedJoysLaunch;       /**< CONFIGURABLE Refer to HELP_UNUSED_JOYS_SELECT */
        bool                ReloadLauncher;         /**< CONFIGURABLE Refer to HELP_RELOAD_LAUNCHER */
        bool                TextScrollOption;       /**< CONFIGURABLE Refer to HELP_TEXT_SCROLL_OPTION */
        bool                AutoLayout;             /**< CONFIGURABLE Refer to HELP_AUTOLAYOUT */
        bool                FilenameArgNoExt;       /**< CONFIGURABLE Refer to HELP_FILENAMEARGNOEXT */
        bool                FilenameAbsPath;        /**< CONFIGURABLE Refer to HELP_FILEABSPATH */
        uint8_t             EntryFastMode;          /**< CONFIGURABLE Refer to HELP_ENTRY_FAST_MODE */
        uint8_t             MaxEntries;             /**< CONFIGURABLE Refer to HELP_MAX_ENTRIES */
        uint8_t             ColorButton;            /**< CONFIGURABLE Refer to HELP_COLOR_BUTTON */
        uint8_t             ColorFontButton;        /**< CONFIGURABLE Refer to HELP_COLOR_FONTBUTTON */
        uint8_t             ColorBackground;        /**< CONFIGURABLE Refer to HELP_COLOR_BACKGND */
        uint8_t             ColorFontFiles;         /**< CONFIGURABLE Refer to HELP_COLOR_FONTFILES */
        uint8_t             ColorFontFolders;       /**< CONFIGURABLE Refer to HELP_COLOR_FONTFOLDERS */
        int16_t             AnalogDeadZone;         /**< CONFIGURABLE Refer to HELP_DEADZONE */
        int16_t             ScreenWidth;            /**< CONFIGURABLE Refer to HELP_SCREEN_WIDTH */
        int16_t             ScreenHeight;           /**< CONFIGURABLE Refer to HELP_SCREEN_HEIGHT */
        int16_t             ScreenDepth;            /**< CONFIGURABLE Refer to HELP_SCREEN_DEPTH */
        uint16_t            CPUClock;               /**< CONFIGURABLE Refer to HELP_CPU_CLOCK */
        uint16_t            ScrollSpeed;            /**< CONFIGURABLE Refer to HELP_SCROLL_PAUSE_SPEED */
        uint16_t            ScrollPauseSpeed;       /**< CONFIGURABLE Refer to HELP_SCROLL_PAUSE_SPEED */
        uint16_t            EntryYDelta;            /**< CONFIGURABLE Refer to HELP_SCROLL_PAUSE_SPEED */
        uint16_t            EntryXOffset;           /**< CONFIGURABLE Refer to HELP_SCROLL_PAUSE_SPEED */
        uint16_t            EntryYOffset;           /**< CONFIGURABLE Refer to HELP_SCROLL_PAUSE_SPEED */
        uint16_t            ButtonWidthLeft;        /**< CONFIGURABLE Refer to HELP_SCROLL_PAUSE_SPEED */
        uint16_t            ButtonHeightLeft;       /**< CONFIGURABLE Refer to HELP_SCROLL_PAUSE_SPEED */
        uint16_t            ButtonWidthRight;       /**< CONFIGURABLE Refer to HELP_SCROLL_PAUSE_SPEED */
        uint16_t            ButtonHeightRight;      /**< CONFIGURABLE Refer to HELP_SCROLL_PAUSE_SPEED */
        uint16_t            PreviewWidth;           /**< CONFIGURABLE Refer to HELP_SCROLL_PAUSE_SPEED */
        uint16_t            PreviewHeight;          /**< CONFIGURABLE Refer to HELP_SCROLL_PAUSE_SPEED */
        uint16_t            DisplayListMaxWidth;    /**< CONFIGURABLE Refer to HELP_DISPLAY_LIST_MAX_WIDTH */
        uint16_t            FilePathMaxWidth;       /**< CONFIGURABLE Refer to HELP_SCROLL_PAUSE_SPEED */
        uint16_t            PosX_Title;             /**< CONFIGURABLE */
        uint16_t            PosY_Title;             /**< CONFIGURABLE */
        uint16_t            PosX_ButtonLeft;        /**< CONFIGURABLE */
        uint16_t            PosY_ButtonLeft;        /**< CONFIGURABLE */
        uint16_t            PosX_ButtonRight;       /**< CONFIGURABLE */
        uint16_t            PosY_ButtonRight;       /**< CONFIGURABLE */
        uint16_t            PosX_ListNames;         /**< CONFIGURABLE */
        uint16_t            PosY_ListNames;         /**< CONFIGURABLE */
        float               ScreenRatioW;           /**< NOT CONFIGURABLE The ratio of the current resolution to the base resolution, used for scaling. */
        float               ScreenRatioH;           /**< NOT CONFIGURABLE The ratio of the current resolution to the base resolution, used for scaling. */
        string              ZipPath;                /**< CONFIGURABLE Refer to HELP_PATH_ZIPTEMP */
        string              PreviewsPath;           /**< CONFIGURABLE Refer to HELP_PATH_PREVIEWS */
        string              PathFont;               /**< CONFIGURABLE Refer to HELP_PATH_FONT */
        string              PathBackground;         /**< CONFIGURABLE Refer to HELP_PATH_BACKGND */
        string              PathPointer;            /**< CONFIGURABLE Refer to HELP_PATH_POINTER */
        string              PathSelectPointer;      /**< CONFIGURABLE Refer to HELP_PATH_SELECTPOINTER */
        string              Delimiter;              /**< CONFIGURABLE Refer to HELP_PROFILE_DELIMITER */
        vector<bool>        ButtonModesLeftEnable;  /**< CONFIGURABLE force buttons to be disabled */
        vector<bool>        ButtonModesRightEnable; /**< CONFIGURABLE force buttons to be disabled */
        vector<string>      PathButtons;            /**< CONFIGURABLE Paths for the button graphics */
        vector<SDLKey>      KeyMaps;                /**< CONFIGURABLE Key mappings to user actions */
        vector<uint8_t>     JoyMaps;                /**< CONFIGURABLE Button mappings to user actions */
        vector<uint8_t>     FontSizes;              /**< CONFIGURABLE Point size of the font sizes */
        vector<SDL_Color>   Colors;                 /**< NOT CONFIGURABLE Basic color types */
        vector<string>      ColorNames;             /**< NOT CONFIGURABLE Basic color names */
};

#endif // CCONFIG_H
