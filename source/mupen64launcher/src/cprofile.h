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

#ifndef CPROFILE_H
#define CPROFILE_H

#include "cbase.h"
#include "czip.h"

using namespace std;

#define ZIP_EXT ".zip"                              /** The zip extension. */

#define PROFILE_TARGETAPP       "targetapp="        /** Prefix for the profile file to identify the path the target application. */
#define PROFILE_FILEPATH        "filepath="         /** Prefix for the profile file to identify the initial path for files. */
#define PROFILE_BLACKLIST       "blacklist="        /** Prefix for the profile file to identify items not to be displayed. */
#define PROFILE_CMDPATH         "cmdpath="          /** Prefix for the profile file to identify a command path and binary. */
#define PROFILE_CMDARG          "cmdarg="           /** Prefix for the profile file to identify a command argument. */
#define PROFILE_EXEPATH         "exepath="          /** Prefix for the profile file to identify the executable path. */
#define PROFILE_EXEFORCE        "exeforce="         /** Prefix for the profile file to identify the forced executable path for certain files. */
#define PROFILE_EXTARG          "extarg="           /** Prefix for the profile file to identify the executable path. */
#define PROFILE_ARGFORCE        "argforce="         /** Prefix for the profile file to identify the executable argument force. */
#define PROFILE_ENTRY_ARGS      "entryargs="        /** Prefix for the profile file to identify an entry custom argument values. */
#define PROFILE_ENTRY_CMDS      "entrycmds="        /** Prefix for the profile file to identify an entry custom command values. */

#define EXT_DIRS                "dirs"              /** Special extension identifier for directories. */
#define VALUE_FILENAME          "%filename%"        /** Profile keyword that is replaced by the path to selected file. */
#define VALUE_NOVALUE           "%na%"              /** Profile keyword that identifies a empty option. */
#define VALUE_FLAGONLY          "%flagonly%"        /** Profile keyword that identifies a empty option. */

#define MAX_PATH                1024                /** Maximum path length. */
#define ARG_MIN_COUNT           4                   /** Minumum options for an argument. */
#define ARGFORCE_COUNT          3                   /** Minumum options for an argument force. */
#define EXEFORCE_COUNT          2                   /** Minumum options for an exe force. */
#define TOTAL_LETTERS           27                  /** 26 alpha chars plus 1 for anything else */

//SEB
#define PROFILE_PLUGIN		"plugin"	    /** Plugin name in profile */
#define PROFILE_PLUGINTXT	"plugintxt"	    /** Plugin .txt file */
#define PROFILE_PLUGINNAME	"pluginname"	    /** Plugin clear name */

/** @brief Type of items that can be displayed in selection mode
 */
enum ITEMTYPES_T {
    TYPE_DIR=0,                     /** @brief Directory */
    TYPE_FILE,                      /** @brief File */
    TYPE_ZIP                        /** @brief Zipfile */
};

/** @brief Data structure for of items that can be displayed in selection mode
 */
struct listitem_t {
    listitem_t() : Type(0), Entry(-1), Name("") {};
    uint8_t Type;                   /** @brief The type of item, file/dir/etc. */
    int16_t Entry;                  /** @brief The entry index assoicated with this item. */
    string  Name;                   /** @brief The name of the list item. */
};

/** @brief Data structure for of options that can be displayed in selection mode
 */
struct listoption_t {
    listoption_t() : Extension(-1), Command(-1), Argument(-1), Name("") {};
    int16_t Extension;              /** @brief Index to the extension for the option */
    int16_t Command;                /** @brief Index to the command for the option (-1 if not a command) */
    int16_t Argument;               /** @brief Index to the argument for the option */
    string  Name;                   /** @brief Display name for the option */
};

/** @brief Data structure for an argument
 */
struct argument_t {
    argument_t() : Default(0), Flag(""), Names(), Values() {};
    int8_t          Default;        /** @brief Index to the default value for the argument */
    string          Flag;           /** @brief Flag that is used in the command line string which is followed by the value */
    vector<string>  Names;          /** @brief Names for the possible values */
    vector<string>  Values;         /** @brief Values for the argument */
};

/** @brief Data structure for overriding argument values that reside in the path
 */
struct argforce_t {
    argforce_t() : Argument(0), Path(""), Value("") {};
    uint8_t Argument;               /** @brief The index of the agrument to override */
    string  Path;                   /** @brief The location of target files to override */
    string  Value;                  /** @brief Value to overide the argument with */
};

/** @brief Data structure for overriding the exe path for listed files.
 */
struct exeforce_t {
    exeforce_t() : exeName(""), exePath(""), Files() {};
    string              exeName;    /** @brief The executable name to pass the detected files to. */
    string              exePath;    /** @brief The path to the executable. */
    vector<string>      Files;      /** @brief Value to overide the argument with */
};

/** @brief Data structure for an command that is run before the application
 */
struct command_t {
    command_t() : Name(""), Command(""), Path(""), Arguments() {};
    string              Name;       /** @brief Display name for the command. */
    string              Command;    /** @brief Executable name for the command. */
    string              Path;       /** @brief Path to the executable. */
    vector<argument_t>  Arguments;  /** @brief Arguments to apply to the command. */
};

/** @brief Data structure for an file extension type
 */
struct extension_t {
    extension_t() : exeName(""), exePath(""), extName(), Blacklist(), Arguments(), ArgForces(), ExeForces() {};
    string              exeName;    /** @brief The executable name to pass the detected files to. */
    string              exePath;    /** @brief The path to the executable. */
    vector<string>      extName;    /** @brief The extenion (usually 3 letters). */
    vector<string>      Blacklist;  /** @brief List of filenames that will be filtered from the selection output. */
    vector<argument_t>  Arguments;  /** @brief Arguments to apply to the appliction. */
    vector<argforce_t>  ArgForces;  /** @brief Overide arguments based on path. */
    vector<exeforce_t>  ExeForces;  /** @brief Overide the exe path based on file names. */
};

/** @brief Data structure for a detected entry that has been set with custom arg/cmd values
 */
struct entry_t {
    entry_t() : Custom(false), Name(""), Path(""), Alias(""), CmdValues(), ArgValues() {};
    bool                Custom;     /** @brief Does the entry have values that are different than the defaults */
    string              Name;       /** @brief Name of the entry */
    string              Path;       /** @brief Path to the file */
    string              Alias;      /** @brief Overides the actual filename for display */
    vector<uint16_t>    CmdValues;  /** @brief The current selected values for the commands */
    vector<uint16_t>    ArgValues;  /** @brief The current selected values for the arguments */
};

//SEB
/** @brief Data structure for a plugin that has been set in profile
 */
struct oneplugin_t {
    oneplugin_t() : So(""), Name(""), Desc("") {};
    string              So;         /** @brief So Name of the entry */
    string              Name;       /** @brief Clear Name to the file */
    string              Desc;       /** @brief Description of plugin */
};

/** @brief Data structure for a plugin that has been set in profile
 */
struct plugin_t {
    plugin_t() : Name(""), TextFile(""), Alias(""), Which(0), Rom(-1), Plugins() {};
    string                 Name;       /** @brief Name of the entry */
    string                 TextFile;   /** @brief Path to the file */
    string                 Alias;      /** @brief Overides the actual filename for display */
    uint16_t		   Which;      /** @brief Current selected plugin */
    int16_t                Rom;        /** @brief Current plugin for selected Rom */
    vector<oneplugin_t>    Plugins;    /** @brief The current plugins */
};

/** @brief This class controls loading, processing, and saving of profile data
 */
class CProfile : public CBase
{
    public:
        /** Constructor. */
        CProfile();
        /** Destructor. */
        virtual ~CProfile();

        /** @brief Load the profile data from file and store in memory.
         * @param location : path to the profile file.
         * @param delimiter : the delimiter used between options.
         * @return 0 if passed 1 if failed.
         */
        int8_t  Load            ( const string& location, const string&  delimiter );

        /** @brief Save the profile data to file from memory.
         * @param location : path to the profile file.
         * @param delimiter : the delimiter used between options.
         * @return 0 if passed 1 if failed.
         */
        int8_t  Save            ( const string& location, const string&  delimiter );

        /** @brief Load a command from a line from the profile.
         * @param fin : file input.
         * @param line : current read line.
         * @param delimiter : the delimiter used between options.
         * @return 0 if passed 1 if failed.
         */
        int8_t  LoadCmd         ( ifstream& fin, string& line, const string& delimiter );

        /** @brief Load plugins from the profile.
         * @return 0 if passed 1 if failed.
         */
        int8_t  LoadPlugins         ( );

        /** @brief Load a extension from a line from the profile.
         * @param fin : file input.
         * @param line : current read line.
         * @param delimiter : the delimiter used between options.
         * @return 0 if passed 1 if failed.
         */
        int8_t  LoadExt         ( ifstream& fin, string& line, const string& delimiter );

        /** @brief Load a entry from a line from the profile.
         * @param fin : file input.
         * @param line : current read line.
         * @param delimiter : the delimiter used between options.
         * @return 0 if passed 1 if failed.
         */
        int8_t  LoadEntry       ( ifstream& fin, string& line, const string& delimiter );

        /** @brief Scan an entry for command and arguments and load them into a list.
         * @param item : the selected item from the list.
         * @param items : list to load into.
         * @return 0 if passed 1 if failed.
         */
        int8_t  ScanEntry       ( listitem_t& item, vector<listoption_t>& items );

        /** @brief Add an entry that will contain custom values.
         * @param argument : argument with the custom value.
         * @param name : name for the new entry.
         * @return -1 if failed, else the new size of the entry list.
         */
        int16_t AddEntry        ( listoption_t& argument, const string& name );

        /** @brief Scan an entry for values and load them into a list.
         * @param item : argument with the custom value.
         * @param values : values for the argument.
         */
        void    ScanArgument    ( listoption_t& item, vector<string>& values );

        /** @brief Scan an the current path for dirs and runable files.
         * @param location : path the scan.
         * @param showhidden : if true include hidden items in output list, else ignore.
         * @param showzip : if true include zip items in output list, else put through filters.
         * @param items : entries det.
         * @return 0 if passed 1 if failed.
         */
        int8_t  ScanDir         ( string location, bool showhidden, bool showzip, vector<listitem_t>& items );

        /** @brief Find the extension a file belongs to.
         * @param ext : extension to search for.
         * @return -1 if failed, else the index of the ext structure.
         */
        int16_t FindExtension   ( const string& ext );

        /** @brief Scan def. plugins and load them into a list.
         * @param values : values for the argument.
         */
        void    ScanDefPlugins    ( vector<listoption_t>& items );

        /** @brief Scan rom plugins and load them into a list.
         * @param values : values for the argument.
         */
        void    ScanRomPlugins     ( const string& name, vector<listoption_t>& items );

        /** @brief Scan def. one plugin type and load them into a list.
         * @param which : which of the default plugins
         * @param values : values for the argument.
         */
        void    ScanDef1Plugins    ( int8_t which, vector<listitem_t>& items );

        void    ScanRom1Plugins    ( int8_t which, vector<listitem_t>& items );

        /** @brief Save 1 type of default plugin
         */
        void    SaveDef1Plugin     ( int8_t which, int8_t index );

        /** @brief Save plugin for curent Rom
         */
        void    SaveRom1Plugin     ( int8_t which, int8_t index );

        /** @brief Get CRC of Rom file
         */
        string  RomCRC             (const string& name);

        int8_t FindPlugin(const string& name);
        int8_t Find1Plugin(int8_t which, const string& name);

        bool                LaunchableDirs;     /**< If true directories are considered as launchable, if false browsing is on. */
        string              LauncherPath;       /**< Path where the launcher was executed from. */
        string              LauncherName;       /**< Name of the launcher when executed. */
        string              FilePath;           /**< Current path for searching for runable files. */
        string              TargetApp;          /**< Label for the target appliction the launcher is executing. */
        string              ZipFile;            /**< If not empty then the currently loaded zip file. */
        string              EntryFilter;
        vector<command_t>   Commands;           /**< Commands to be run before executing the target application. */
        vector<extension_t> Extensions;         /**< File extensions runable by the target application. */
        vector<entry_t>     Entries;            /**< Entries with custom values. */
        vector<int16_t>     AlphabeticIndices;  /**< Set to cause the current directory to be rescaned. */
        string              Rom;                /**< CRC of the current rom */
	//SEB
	vector<plugin_t>    Plugins;		/**< All plugins */
        CZip                Minizip;            /**< Handles examing and extracting zip files. */
};

bool CompareItems( listitem_t a, listitem_t b );    /**< Compare two listitems, which sort by type and then by name. */

#endif // CPROFILE_H
