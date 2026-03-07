/*  OpenFIRE App: a configuration utility for the OpenFIRE light gun system.
    Common shared assets & constants.

    Copyright (C) 2025  Team OpenFIRE

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef APPCOMMON_H
#define APPCOMMON_H

#include "../boards/OpenFIREshared.h"

#include <QString>
#include <QVector>
#include <QMap>

#define BUTTON_COUNT 14

class App_Common
{
public:
    enum {
        pBoxIRsens = 0,
        pBoxRunMode,
        pBoxLayout,
        pBoxAR
    } profileBoxesTypes_e;

    /// @brief      Types of objects that can be made interactive
    /// @details    These are required for mouse over interactivity.
    ///             Different types distinguish which tab it's made for,
    ///             to activate or modify the appropriate things.
    enum {
        trackPinbox = 0,
        trackSettingsItem,
        trackProfileItem,
        trackButtonMapItem,
        trackTestItem
    } uiTrackableObjects_e;

    enum {
        dataCurrent = 0,
        dataOrig,
        dataTablesCount
    } dataBlocks_e;

    // Single instance of presets and board info
    static inline OF_Const OFPresets;

    typedef struct boardInfo_t {
        int        selectedProfile;
        int        previousProfile;
        QByteArray type;
        QByteArray arch;
        QByteArray version;
    } boardInfo_s;

    typedef struct tinyUSBtable_t {
        uint16_t   tinyUSBid;
        char       tinyUSBname[16];
    } tinyUSBtable_s;

    typedef struct profilesTable_t {
        int32_t    topOffset     = 0;
        int32_t    bottomOffset  = 0;
        int32_t    leftOffset    = 0;
        int32_t    rightOffset   = 0;
        float      TLled         = 0;
        float      TRled         = 0;
        float      AdjX          = 0;
        float      AdjY          = 0;
        uint32_t   irSensitivity = 0;
        uint32_t   runMode       = 0;
        uint32_t   layoutType    = 0;
        uint32_t   aspectRatio   = 0;
        uint32_t   color         = 0;
        char       profName[16]  = "";
    } profilesTable_s;

    // Currently loaded board object
    static inline boardInfo_s board;

    /// @brief      Current array of booleans
    /// @details    Meant for toggle/on-off type settings specifically
    static inline bool boolSettings[dataTablesCount][OF_Const::boolTypesCount] = { false };

    /// @brief      Current array of tunable settings
    static inline uint32_t settingsTable[dataTablesCount][OF_Const::settingsTypesCount] = { 0 };

    // Currently loaded board's TinyUSB identifier info
    static inline tinyUSBtable_s tinyUSBtable;
    // TinyUSB ident, as loaded from the board
    static inline tinyUSBtable_s tinyUSBtable_orig;

    // Current calibration profiles
    static inline QVector<profilesTable_s> profilesTable;
    // Calibration profiles, as loaded from the board
    static inline QVector<profilesTable_s> profilesTable_orig;

    // Map of what inputs are put where,
    // Key = button/output, Value = pin number occupying, if any.
    // Value of -1 means unmapped.
    // Key order based on boardInputs_e, minus 1
    // Map functions used in deduplication
    static inline QMap<uint8_t, int8_t> inputsMap;
    // Inputs map, as loaded from the board
    static inline QMap<uint8_t, int8_t> inputsMap_orig;

    enum {
        inputFuncData = 0,
        inputFuncOrder,
        inputFuncTypes
    } kbInputs_e;

    enum {
        inputMouse = 0,
        inputKB,
        inputGamepad,
        inputTypes
    } inputFuncTypes_e;

    // Map of input funcs
    // array 1 = original and current data array
    // array 2 = buttons count
    // array 3 = data for button ((func type : func num) * 3)
    static inline uint8_t inputFuncTable[dataTablesCount][BUTTON_COUNT][inputTypes*2];

    // Keyboard inputs reference map
    // first int is the value representing the key used by the firmware,
    // second int is the desired order (since no Map allows
    static const inline QMap<std::string, QVector<int>> keyboardInputsMap = {
        {"Player-relative Start Key",   {0xFF,      0 }},
        {"Player-relative Coin Key",    {0xFE,      1 }},
        {"Up Arrow",                    {0xDA,      2 }},
        {"Down Arrow",                  {0xD9,      3 }},
        {"Left Arrow",                  {0xD8,      4 }},
        {"Right Arrow",                 {0xD7,      5 }},
        {"Enter/Return",                {0xB0,      6 }},
        {"Backspace",                   {0xB2,      7 }},
        {"Escape",                      {0xB1,      8 }},
        {"Left Ctrl",                   {0x80,      9 }},
        {"Right Ctrl",                  {0x84,      10}},
        {"Left Alt",                    {0x82,      11}},
        {"Right Alt",                   {0x86,      12}},
        {"Left Shift",                  {0x81,      13}},
        {"Right Shift",                 {0x85,      14}},
        {"Tab",                         {0xB3,      15}},
        {"Space",                       {0x2C,      16}},
        {"A",                           {'a',       17}},
        {"B",                           {'b',       18}},
        {"C",                           {'c',       19}},
        {"D",                           {'d',       20}},
        {"E",                           {'e',       21}},
        {"F",                           {'f',       22}},
        {"G",                           {'g',       23}},
        {"H",                           {'h',       24}},
        {"I",                           {'i',       25}},
        {"J",                           {'j',       26}},
        {"K",                           {'k',       27}},
        {"L",                           {'l',       28}},
        {"M",                           {'m',       29}},
        {"N",                           {'n',       30}},
        {"O",                           {'o',       31}},
        {"P",                           {'p',       32}},
        {"Q",                           {'q',       33}},
        {"R",                           {'r',       34}},
        {"S",                           {'s',       35}},
        {"T",                           {'t',       36}},
        {"U",                           {'u',       37}},
        {"V",                           {'v',       38}},
        {"W",                           {'w',       39}},
        {"X",                           {'x',       40}},
        {"Y",                           {'y',       41}},
        {"Z",                           {'z',       42}},
        {"F1",                          {0xC2,      43}},
        {"F2",                          {0xC3,      44}},
        {"F3",                          {0xC4,      45}},
        {"F4",                          {0xC5,      46}},
        {"F5",                          {0xC6,      47}},
        {"F6",                          {0xC7,      48}},
        {"F7",                          {0xC8,      49}},
        {"F8",                          {0xC9,      50}},
        {"F9",                          {0xCA,      51}},
        {"F10",                         {0xCB,      52}},
        {"F11",                         {0xCC,      53}},
        {"F12",                         {0xCD,      54}}
    };

    // Used for combobox elements so that the order isn't haphazard
    // TODO: is a map the best way of doing this? hrm
    static inline char* kbOrderedStrings[55];

    static const inline QMap<std::string, QVector<int>> mouseMap = {
        {"Left Click",          {0b00000001, 0}},
        {"Right Click",         {0b00000010, 1}},
        {"Middle Click",        {0b00000100, 2}},
        {"Side Button Back",    {0b00001000, 3}},
        {"Side Button Forward", {0b00010000, 4}}
    };

    // Used for combobox elements so that the order isn't haphazard
    static inline char* mouseOrderedStrings[5];

    // Gamepad inputs reference map
    // first int is the value representing the key used by the firmware,
    // second int is the desired order (since no Map allows
    static const inline QMap<std::string, QVector<int>> gamepadMap = {
        {"A Button",            {0,  0 }},
        {"B Button",            {1,  1 }},
        // C Button (N/A)
        {"X Button",            {2,  2 }},
        {"Y Button",            {3,  3 }},
        // Z Button (N/A)
        {"Left Shoulder",       {4,  4 }},
        {"Right Shoulder",      {5,  5 }},
        {"Left Trigger",        {6,  6 }},
        {"Right Trigger",       {7,  7 }},
        {"Select Button",       {8, 8 }},
        {"Start Button",        {9, 9 }},
        // Home Button (N/A)
        {"Left Stick Click",    {10, 10}},
        {"Right Stick Click",   {11, 11}},
        {"D-Pad Up",            {15, 12}},
        {"D-Pad Down",          {16, 13}},
        {"D-Pad Left",          {17, 14}},
        {"D-Pad Right",         {18, 15}}
    };

    // Used for combobox elements so that the order isn't haphazard
    static inline char* gpadOrderedStrings[16];

    static inline const char* inputFuncTypesStrings[3] = {
        "Mouse",
        "Keyboard",
        "Gamepad"
    };

    static const inline QMap<std::string, QVector<int>> *inputFuncMaps[3] = {
        &mouseMap,
        &keyboardInputsMap,
        &gamepadMap
    };

    static inline const char* i2cTypeLabels[2] = {
        "SDA",
        "SCL"
    };

    static inline const char* spiTypeLabels[4] = {
        "RX",
        "TX",
        "SCK",
        "CSn"
    };
};

#endif // CONSTANTS_H
