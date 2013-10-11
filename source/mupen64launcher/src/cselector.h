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

#ifndef CSELECTOR_H
#define CSELECTOR_H

#include "version.h"
#include "cbase.h"
#include "cconfig.h"
#include "cprofile.h"
#include "csystem.h"

using namespace std;

#define ARG_RESETGUI            "--resetgui"                    /** Flag to reset GUI settings based on the current resolution. */
#define ARG_CONFIG              "--config"                      /** Flag to override the config file. */
#define DEF_CONFIG              "config.txt"                    /** Default config filename. */
#define ARG_PROFILE             "--profile"                     /** Flag to override the profile file. */
#define DEF_PROFILE             "profile.txt"                   /** Default profile filename. */
#define ARG_ZIPLIST             "--ziplist"                     /** Flag to override the ziplist file. */
#define DEF_ZIPLIST             "ziplist.txt"                   /** Default ziplist filename. */

#define ENTRY_ARROW             "-> "                           /** Ascii fallback for the entry arrow selector. */
#define BUTTON_LABEL_ONE_UP     "<"                             /** Ascii text fallback for the one up button label. */
#define BUTTON_LABEL_ONE_DOWN   ">"                             /** Ascii text fallback for the one down button label. */
#define BUTTON_LABEL_PAGE_UP    "<<"                            /** Ascii text fallback for the page up button label. */
#define BUTTON_LABEL_PAGE_DOWN  ">>"                            /** Ascii text fallback for the page down button label. */
#define BUTTON_LABEL_DIR_UP     "U"                             /** Ascii text fallback for the directory up button label. */
#define BUTTON_LABEL_DIR_DOWN   "D"                             /** Ascii text fallback for the directory down button label. */
#define BUTTON_LABEL_ZIP_MODE   "Z"                             /** Ascii text fallback for the zip mode label. */
#define BUTTON_LABEL_CONFIG     "Def. Plugins"                  /** Ascii text fallback for the options button label. */
#define BUTTON_LABEL_EDIT       "ROM Plugins"                   /** Ascii text fallback for the edit item button label. */
#define BUTTON_LABEL_SET_ONE    "Set"                           /** Ascii text fallback for the set button label. */
#define BUTTON_LABEL_SET_ALL    "Default"                       /** Ascii text fallback for the default button label. */
#define BUTTON_LABEL_BACK       "Back"                          /** Ascii text fallback for the back button label. */
#define BUTTON_LABEL_SELECT     "Select"                        /** Ascii text fallback for the select button label. */
#define BUTTON_LABEL_QUIT       "Quit"                          /** Ascii text fallback for the quit button label. */

#define EMPTY_DIR_LABEL         "<no files detected in dir>"    /** Ascii text for display if no files or dirs are detected. */
#define EMPTY_ZIP_LABEL         "<no files detected in zip>"    /** Ascii text for display if no files in a zip are detected. */

#define EVENT_LOOPS_ON          0                               /** Value set to loops to indentify an active event. */
#define EVENT_LOOPS_OFF         0x7F                            /** Value set to loops to indentify an inactive event. */
#define EVENT_LOOPS             10                              /** Number of loops an event must be active to be detected to be on. */
#define IsEventOn(x)    ((EventPressCount.at(x) <= EVENT_LOOPS_ON)  ? true : false) /** Determines if an event is active. */
#define IsEventOff(x)   ((EventPressCount.at(x) == EVENT_LOOPS_OFF) ? true : false) /** Determines if an event is inactive. */

#define MS_PER_SEC              1000                            /** Milliseconds in 1 Second. */
#define FRAMES_PER_SEC          60                              /** Frames in 1 Second. */
#define FRAME_SKIP_RATIO        4                               /** Maximum frames skip ratio, draw 1 frame for every X number of skipped frames. */

#define FREE_IMAGE(X)   if (X != NULL) { SDL_FreeSurface(X); X = NULL; } /** Macro for checking and releasing pointers to pixel data. */

/** @brief Modes of the launcher.
 */
enum MODES_T {
    MODE_SELECT_ENTRY=0,    /** Main mode for selecting an file to run. */
    MODE_SELECT_ARGUMENT,   /** Select an argument or command belonging to an entry. */
    MODE_SELECT_VALUE,      /** Select an value belonging to an argument. */
    MODE_SELECT_OPTION,     /** Select a Def. Plugin type */
    MODE_SELECT_PLUGIN,     /** Selct one Def. Plugin */
    MODE_ROM_OPTION,
    MODE_ROM_PLUGIN,
    MODE_TOTAL              /** Number of modes. */
};

/** @brief List positions for the current item and first and last items.
 */
struct item_pos_t {
    item_pos_t() : first(0), last(0), absolute(0), relative(0), total(0) {};
    int16_t first;          /** Index of the first item on the display list. */
    int16_t last;           /** Index of the last item on the display list. */
    int16_t absolute;       /** Absolute index of the currently selected item in the display list. */
    int16_t relative;       /** Relative index of the currently selected item in the display list. */
    int16_t total;          /** Total number of items. */
};

/** @brief The font, color, text for a item in the display list
 */
struct listtext_t {
    listtext_t() : font(0), color(0), text("") {};
    int8_t font;            /** Index of the selected font size for the text. */
    int8_t color;           /** Index of the color for the text. */
    string text;            /** Text for an item for the display list. */
};

/** @brief This class controls resources, logic for gui, interaction with the user.
 */
class CSelector : public CBase
{
    public:
        /** Constructor. */
        CSelector();
        /** Destructor. */
        virtual ~CSelector();

        /** @brief Run the main loop of the application.
         * @param argc : number of arguments.
         * @param argv : arguments passed to the application.
         * @return 0 if passed 1 if failed.
         */
        int8_t Run( int argc, char** argv );

    private:
        /** @brief Parse and handle arguments sent to the application.
         * @param argc : number of arguments.
         * @param argv : arguments passed to the application.
         */
        void    ProcessArguments    ( int32_t argc, char** argv );

        /** @brief Open system interfaces and load configuration information.
         * @return 0 if passed 1 if failed.
         */
        int8_t  OpenResources       ( void );

        /** @brief Close system interfaces and save configuration information.
         * @param result : result of the application, do not save config files if an error occurs.
         * @return 0 if passed 1 if failed.
         */
        void    CloseResources      ( int8_t result );

        /** @brief Main loop of the application, polls input, runs current mode, and freshes the screen.
         * @return -1  for no selection otherwise the entry selection number.
         */
        int16_t DisplayScreen       ( void );

        /** @brief Check which screen rects a rect overlaps. The marked screen rects will only be updated for the screen.
         */
        void    UpdateRect          ( int16_t x, int16_t y, int16_t w, int16_t h );

        /** @brief Handles logic for measuring and counting frame drawing to the screen.
         */
        void    UpdateScreen        ( void );

        /** @brief Determines the mode to run based on user input events.
         */
        void    SelectMode          ( void );

        /** @brief Mode to display list of input files.
         * @return 0 if passed 1 if failed.
         */
        int8_t  DisplaySelector     ( void );

        /** @brief Loads the preview image if any exists.
         */
        void    LoadPreview         ( const string& name );

        /** @brief Moves the current path one directory level up.
         */
        void    DirectoryUp         ( void );

        /** @brief Moves the current path one directory level down.
         */
        void    DirectoryDown       ( void );

        /** @brief Moves the current path out of a zip file.
         */
        void    ZipUp               ( void );

        /** @brief Moves the current path into a zip file.
         */
        void    ZipDown             ( void );

        /** @brief Load the display list with text labels and font info.
         */
        void    PopulateList        ( void );

        /** @brief Load the display with items with entries.
         */
        void    PopModeEntry        ( void );

        /** @brief Load the display with items with entry arguments.
         */
        void    PopModeArgument     ( void );

        /** @brief Load the display list with items with argument values.
         */
        void    PopModeValue        ( void );

        /** @brief Load the display list with items with type of plugin.
         */
        void    PopModeOption       ( void );

        void    PopModeRomOption    ( void );

        /** @brief Load the display list with items with Plugin detail for 1 type.
         */
        void    PopModePlugin       ( void );

        void    PopModeRomPlugin    ( void );

        /** @brief Will redetect items for the current mode.
         */
        void    RescanItems         ( void );

        /** @brief Draws the names for the items in the display list for the current mode.
         * @return 0 if passed 1 if failed.
         */
        int8_t  DrawNames           ( SDL_Rect& location );

        /** @brief Calculates the indices of the entries that should be displayed based on the current selection
         * @param pos : reference to the position selections in the current list of entries
         */
        void    SelectionLimits     ( item_pos_t& pos );

        /** @brief Will either fill the screen with a color or blit a specified image to the screen
         */
        void    DrawBackground      ( void );

        /** @brief Configures the buttons to be displayed on the screen for the current mode
         * @return 0 if passed 1 if failed
         */
        int8_t  ConfigureButtons    ( void );

        /** @brief Draws the buttons to the screen, in either fill color mode or bitmap's
         * @param location : rect contains the starting coordinates for the buttons
         */
        int8_t  DrawButtons         ( SDL_Rect& location );

        /** @brief Draw a button to the screen
         * @param button : the button to draw (index is defined in EVENT_T)
         * @param font : the font used for render
         * @param location : the coordinates and dimensions of the button
         * @return 0 if passed 1 if failed
         */
        int8_t  DrawButton          ( uint8_t button, TTF_Font* font, SDL_Rect& location );

        /** @brief Draws text labels to the screen, like title and author info
         * @param location : contains the starting coordinates for the text
         * @return 0 if passed 1 if failed
         */
        int8_t  DrawText            ( SDL_Rect& location );

        /** @brief Creates a command script to run the target application
         * @param selection : the entry selection to use for input to the target application
         * @return 0 if passed 1 if failed
         */
        int8_t  RunExec             ( uint16_t selection );

        /** @brief Collect all input events from the user
         * @return 0 if passed 1 if failed
         */
        int8_t  PollInputs          ( void );

        CSelector(const CSelector &);
        CSelector & operator=(const CSelector&);

        bool                    Redraw;
        bool                    SkipFrame;
        bool                    Rescan;             /**< Set to cause the current directory to be rescaned. */
        bool                    RefreshList;        /**< Set to cause the current display list to be populated. */
        bool                    SetOneEntryValue;   /**< Set value for the current entry to the selected value. */
        bool                    SetAllEntryValue;   /**< Set default for all entries to the selected value. */
        bool                    TextScrollDir;      /**< Determines the direction of the horizontal scroll, left or right. */
        bool                    ExtractAllFiles;    /**< True if all files should be extracted from a zip, if false only the selected file is. */

        bool                    DrawState_Title;
        bool                    DrawState_About;
        bool                    DrawState_Filter;
        bool                    DrawState_FilePath;
        bool                    DrawState_Index;
        bool                    DrawState_ZipMode;
        bool                    DrawState_Preview;
        bool                    DrawState_ButtonL;
        bool                    DrawState_ButtonR;

        uint8_t                 Mode;               /**< The current mode of the application. */
        uint8_t                 LastSelectedEntry;  /**< Stores the index of the last seclected entry so scrolling can be restarted. */
        uint16_t                TextScrollOffset;   /**< Number of pixels to offset the entry text surface when it will blit to the screen. */
        uint16_t                CurScrollSpeed;     /**< Current number of loops used to decide when to offset the entry text scroll effect. */
        uint16_t                CurScrollPause;     /**< Current number of loops used to decide when to pause the entry text scroll effect. */
        uint16_t                ListNameHeight;
        int16_t                 FramesDrawn;        /**< Counter for the number frames drawn to the screen. */
        int16_t                 FramesSkipped;      /**< Counter for the number frames not drawn to the screen. */
        int16_t                 FramesSleep;
#if defined(DEBUG)
        int16_t                 FPSDrawn;           /**< Number frames drawn to the screen per second. */
        int16_t                 FPSSkip;            /**< Number frames not drawn to the screen per second. */
        int16_t                 FPSSleep;
        int32_t                 FrameCountTime;     /**< Tick count from measureing FPS. */
        int16_t                 LoopTimeAverage;    /**< Average loop time. */
#endif
        int32_t                 FrameEndTime;       /**< Tick count at the end of the frame. */
        int32_t                 FrameStartTime;     /**< Tick count at the start of the frame. */
        int16_t                 FrameDelay;         /**< Tick duration of the frame. */
        SDL_Rect                Mouse;              /**< Stores the absolute position of the mouse pointer. */
        SDL_Joystick*           Joystick;           /**< SDL surface reference to the first joystick device. */
        SDL_Surface*            Screen;             /**< SDL surface reference to the screen. */
        SDL_Surface*            ImageBackground;    /**< SDL surface reference to the background pixel data (optional). */
        SDL_Surface*            ImagePointer;       /**< SDL surface reference to the pointer pixel data (optional). */
        SDL_Surface*            ImageSelectPointer; /**< SDL surface reference to the list select pointer pixel data (optional). */
        SDL_Surface*            ImagePreview;       /**< SDL surface reference to the preview pixel data. */
        SDL_Surface*            ImageTitle;         /**< SDL surface reference to the title text pixel data. */
        SDL_Surface*            ImageAbout;         /**< SDL surface reference to the about text pixel data. */
        SDL_Surface*            ImageFilePath;      /**< SDL surface reference to the about text pixel data. */
        SDL_Surface*            ImageFilter;        /**< SDL surface reference to the about text pixel data. */
        SDL_Surface*            ImageIndex;         /**< SDL surface reference to the about text pixel data. */
        SDL_Surface*            ImageZipMode;       /**< SDL surface reference to the about text pixel data. */
#if defined(DEBUG)
        SDL_Surface*            ImageDebug;         /**< SDL surface reference to the about text pixel data. */
#endif
        vector<SDL_Surface*>    ImageButtons;       /**< SDL surface references to the button's pixel data (optional). */
        vector<TTF_Font*>       Fonts;              /**< SDL-TTF references to the rendered font in different size. */
        CConfig                 Config;             /**< The configuration data. */
        CProfile                Profile;            /**< The extension and entries data. */
        CSystem                 System;             /**< System specific controls and methods. */
        string                  ConfigPath;         /**< Contains the file path to the config.txt. */
        string                  ProfilePath;        /**< Contains the file path to the profile.txt. */
        string                  ZipListPath;        /**< Contains the path and name of the files that have been unzipped. */
        vector<bool>            EventReleased;      /**< Collection of the states if a release event was detected. */
        vector<int8_t>          EventPressCount;    /**< Collection of the loop counts for when an event can act again. */
        vector<uint8_t>         ButtonModesLeft;    /**< Collection of the state of the buttons on the left side. */
        vector<uint8_t>         ButtonModesRight;   /**< Collection of the state of the buttons on the right side. */
        vector<item_pos_t>      DisplayList;        /**< Collection of the positions and limits of list selection for each mode. */
        vector<string>          LabelButtons;       /**< Collection of text labels for the buttons. */
        vector<listtext_t>      ListNames;          /**< Collection of text and font information for the entry currently displayed. */
        vector<listitem_t>      ItemsEntry;         /**< Collection of directories and filenames detected in the current path. */
        vector<listoption_t>    ItemsArgument;      /**< Collection of options for an entry or config. */
        vector<listitem_t>      ItemsDefPlugin;     /**< Collection of Plugin for selected plugin type. */
        uint8_t                 WhichPlugin;        /**< Wich Plugin is selected */
        vector<listoption_t>    ItemsRomOption;     /**< Collection of options for an entry or config. */
        vector<listitem_t>      ItemsRomPlugin;     /**< Collection of Plugin for selected plugin type. */
        uint8_t                 WhichRomPlugin;     /**< Wich Plugin is selected */
        vector<string>          ItemsValue;         /**< Collection of values for an option. */
        vector<SDL_Rect>        RectEntries;        /**< Collection of position rects for the displayed entries. */
        vector<SDL_Rect>        RectButtonsLeft;    /**< Collection of position rects for the displayed buttons on left. */
        vector<SDL_Rect>        RectButtonsRight;   /**< Collection of position rects for the displayed buttons on right. */
        vector<SDL_Rect>        ScreenRectsDirty;   /**< Collection of rects for the areas of the screen that will be updated. */
};

#endif // CSELECTOR_H
