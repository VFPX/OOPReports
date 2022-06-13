*==============================================================================
* File:				SFRepObj.h
* Purpose:			Constants for SFRepObj.vcx
* Author:			Doug Hennig
* Last Revision:	10/09/2019
*==============================================================================

* Include other constants.

#include SFCTRLS.H

* Totaling types.

#define ccTOTAL_NONE                'N'
#define cnTOTAL_NONE                0
#define ccTOTAL_COUNT               'C'
#define cnTOTAL_COUNT               1
#define ccTOTAL_COUNT_DISTINCT      'B'
#define ccTOTAL_SUM                 'S'
#define cnTOTAL_SUM                 2
#define ccTOTAL_AVERAGE             'A'
#define cnTOTAL_AVERAGE             3
#define ccTOTAL_LOWEST              'L'
#define cnTOTAL_LOWEST              4
#define ccTOTAL_HIGHEST             'H'
#define cnTOTAL_HIGHEST             5
#define ccTOTAL_STDDEV              'D'
#define cnTOTAL_STDDEV              6
#define ccTOTAL_VARIANCE            'V'
#define cnTOTAL_VARIANCE            7

* Object alignment constants.

#define ccALIGN_LEFT                'Left'
#define ccALIGN_CENTER              'Center'
#define ccALIGN_RIGHT               'Right'
#define cnALIGN_LEFT                0
#define cnALIGN_RIGHT               1
#define cnALIGN_CENTER              2

* Modes.

#define cnMODE_OPAQUE               0
#define cnMODE_TRANSPARENT          1

* Report font style constants.

#define ccSTYLE_NORMAL              ''
#define cnSTYLE_NORMAL              0
#define ccSTYLE_BOLD            	'B'
#define cnSTYLE_BOLD                1
#define cnSTYLE_BOLD_BIT            0
#define ccSTYLE_ITALIC          	'I'
#define cnSTYLE_ITALIC              2
#define cnSTYLE_ITALIC_BIT          1
#define ccSTYLE_UNDERLINE       	'U'
#define cnSTYLE_UNDERLINE           4
#define cnSTYLE_UNDERLINE_BIT       2
#define ccSTYLE_STRIKEOUT       	'-'
#define cnSTYLE_STRIKEOUT           128
#define cnSTYLE_STRIKEOUT_BIT       7

* Report measurement units.

#define ccUNITS_CHARACTERS          'C'
#define ccUNITS_INCHES              'I'
#define ccUNITS_CENTIMETERS         'M'

* Report object types.

#define cnREPOBJ_HEADER              1
#define cnREPOBJ_TEXT                5
#define cnREPOBJ_LINE                6
#define cnREPOBJ_BOX                 7
#define cnREPOBJ_FIELD               8
#define cnREPOBJ_BAND                9
#define cnREPOBJ_IMAGE              17
#define cnREPOBJ_VARIABLE           18
#define cnREPOBJ_FONT               23
#define cnREPOBJ_DATA_ENVIRONMENT   25
#define cnREPOBJ_DATA_OBJECT        26

* Report object codes.

#define cnREPOBJ_CODE_TITLE          0
#define cnREPOBJ_CODE_PGHEADER       1
#define cnREPOBJ_CODE_COLHEADER      2
#define cnREPOBJ_CODE_GPHEADER       3
#define cnREPOBJ_CODE_DETAIL         4
#define cnREPOBJ_CODE_GPFOOTER       5
#define cnREPOBJ_CODE_COLFOOTER      6
#define cnREPOBJ_CODE_PGFOOTER       7
#define cnREPOBJ_CODE_SUMMARY        8
#define cnREPOBJ_CODE_DETHEADER      9
#define cnREPOBJ_CODE_DETFOOTER     10
#define cnREPOBJ_CODE_GROUP         10
#define cnREPOBJ_CODE_HEADER        53

* Report band names (in upper case).

#define ccBAND_TITLE                'TITLE'
#define ccBAND_PAGE_HEADER          'PAGE HEADER'
#define ccBAND_COLUMN_HEADER        'COLUMN HEADER'
#define ccBAND_GROUP_HEADER         'GROUP HEADER'
#define ccBAND_DETAIL               'DETAIL'
#define ccBAND_DETAIL_HEADER        'DETAIL HEADER'
#define ccBAND_DETAIL_FOOTER        'DETAIL FOOTER'
#define ccBAND_GROUP_FOOTER         'GROUP FOOTER'
#define ccBAND_COLUMN_FOOTER        'COLUMN FOOTER'
#define ccBAND_PAGE_FOOTER          'PAGE FOOTER'
#define ccBAND_SUMMARY              'SUMMARY'

* Report object types (in upper case).

#define ccOBJECT_FIELD              'FIELD'
#define ccOBJECT_TEXT               'TEXT'
#define ccOBJECT_LINE               'LINE'
#define ccOBJECT_IMAGE              'IMAGE'
#define ccOBJECT_BOX                'BOX'

* Object positions options.

#define cnPOSITION_FLOAT            0
#define cnPOSITION_TOP              1
#define cnPOSITION_BOTTOM           2

* Other constants.

#define cnPOINTS_PER_INCH           72

* cnVERTICAL_FUDGE is a "fudge factor" used so objects are correctly placed.
* This value comes from the fact that VFP sizes objects in the Report Designer
* as FONTMETRIC(1, FontName, FontSize, Style) + 2. For example, FONTMETRIC(1)
* for Arial 20, B is 32 but the Report Designer makes text objects with that
* font 34 * 104.166 = 3541.6 FRUs high. Actually, it not exactly 2; it's
* actually FONTMETRIC(1) + cnVERTICAL_FUDGE/Lines, where Lines in the number
* of lines the field is high.

#define cnVERTICAL_FUDGE            2

* cnWIDTH_FUDGE is a "fudge factor" used so field objects are correctly sized.

#define cnWIDTH_FUDGE               0.23

* cnTXTWIDTH_FUDGE is a "fudge factor" applied to values returned by TXTWIDTH().

#define cnTXTWIDTH_FUDGE            1.044

* cnPEN_RED_FUDGE is a "fudge factor" for the PEN_RED value in an FRX.

#define cnPEN_RED_FUDGE             4

* cnREPORT_UNITS is the number of report units (a value used for all
* measurements inside the FRX) per inch

#define cnREPORT_UNITS              10000

* cnFACTOR is the number of report units per pixel. It's calculated as report
* units per inch (10,000) divided by pixels per inch (96). 10000/96 = 104.166.

#define cnFACTOR                    104.166

* cnSCREEN_DPI is the pixels per inch (96) used by the report engine.

#define cnSCREEN_DPI                96

* cnBAND_HEIGHT is the height of the band separator, which is 20 pixels or
* 2083.333 report units.

#define cnBAND_HEIGHT               2083.333

* cnGROUP_OFFSET is what to add to a group number for group breaks.

#define cnGROUP_OFFSET              5

* cnDETAIL_OFFSET is what to add to a detail band number for group breaks.

#define cnDETAIL_OFFSET             79

* ccAVG_CHAR is the "average" character in a font used to determine object
* sizing.

#define ccAVG_CHAR                  'N'

* cnPORTRAIT_WIDTH, cnLANDSCAPE_WIDTH, and cnLANDLEGAL_WIDTH are the widths of
* portrait, landscape, and landscape on legal paper reports, respectively, in
* report units (80000 = 8", 105000 = 10.5", 140000 = 14", which is for 8 1/2"
* x 11" paper, less laser printer margins). Change these to use a different
* paper size. cnPRINTER_MARGIN is the printer margin (0.25" per side = 0.5").

#define cnDEFAULT_REPORT_LENGTH       11
#define cnDEFAULT_REPORT_WIDTH        8.5
#define cnDEFAULT_PRTINFO_POINTONEMM  254
#define cnDEFAULT_PRTINFO_MM          25.4
#define cnPORTRAIT_WIDTH               80000
#define cnLANDSCAPE_WIDTH             105000
#define cnLANDLEGAL_WIDTH             140000
#define cnPRINTER_MARGIN              0.5

* Orientation.

#define cnORIENTATION_AUTO            0
#define cnORIENTATION_PORTRAIT        1
#define cnORIENTATION_LANDSCAPE       2

* Ruler scale.

#define cnRULER_INCHES                1
#define cnRULER_METRIC                2
#define cnRULER_PIXELS                3

* Pen patterns.

#define cnPEN_PATTERN_NONE            0
#define cnPEN_PATTERN_DOTTED          1
#define cnPEN_PATTERN_DASDHED         2
#define cnPEN_PATTERN_DASH_DOT        3
#define cnPEN_PATTERN_DASH_DOT_DOT    4
#define cnPEN_PATTERN_NORMAL          8

* Fill patterns.

#define cnFILL_PATTERN_NONE           0
#define cnFILL_PATTERN_SOLID          1
#define cnFILL_PATTERN_HLINES         2
#define cnFILL_PATTERN_VLINES         3
#define cnFILL_PATTERN_DLINES_LEFT    4
#define cnFILL_PATTERN_DLINES_RIGHT   5
#define cnFILL_PATTERN_GRID           6
#define cnFILL_PATTERN_HATCH          7

* Report protection bit positions.

#define FRX_PROTECT_REPORT_NO_PREVIEW       7
#define FRX_PROTECT_REPORT_NO_OPTBAND       8
#define FRX_PROTECT_REPORT_NO_GROUP         9
#define FRX_PROTECT_REPORT_NO_VARIABLES    10
#define FRX_PROTECT_REPORT_NO_PAGESETUP    11
#define FRX_PROTECT_REPORT_NO_MULTISELECT  12
#define FRX_PROTECT_REPORT_NO_DATAENV      13
#define FRX_PROTECT_REPORT_NO_PRINT        15
#define FRX_PROTECT_REPORT_NO_QUICKREPORT  16

* FRX GENERAL column values for RECTANGLE/SHAPEs

#define FRX_PICTUREMODE_CLIP                0
#define FRX_PICTUREMODE_SCALE_KEEP_SHAPE    1
#define FRX_PICTUREMODE_SCALE_STRETCH       2

* FRX Picture object source type (OFFSET column)

#define FRX_PICTURE_SOURCE_FILENAME         0  && stored in PICTURE column
#define FRX_PICTURE_SOURCE_GENERAL          1  && stored in NAME    column
#define FRX_PICTURE_SOURCE_EXPRESSION       2  && stored in NAME    column

* Printer constants.

#define LOGPIXELSX       88
	&& Logical pixels/inch in X
#define LOGPIXELSY       90
	&& Logical pixels/inch in Y
#define PHYSICALWIDTH   110
	&& Physical Width in device units
#define PHYSICALHEIGHT  111
	&& Physical Height in device units
#define PHYSICALOFFSETX 112
	&& Physical Printable Area x margin
#define PHYSICALOFFSETY 113
	&& Physical Printable Area y margin
#define HORZRES           8
	&& the width, in pixels, of the printable area of the page
#define VERTRES          10
	&& the height, in pixels, of the printable area of the page
