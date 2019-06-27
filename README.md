# OOPReports

**Provides object-oriented reports for use in Visual FoxPro 9.0 development**

Project Manager: [Doug Hennig](mailto:dhennig@stonefield.com)

## Introduction

Although VFP is a great object-oriented development language, one glaring area of the product that has never been objectified is reports. Out of the box, reports cannot be easily changed dynamically at runtime nor generated programmatically.

Since FoxPro 2.0, the FRX file has been a table, so it seems like this should be an easy task. However, the fact of the matter is that the FRX structure is ugly as sin. Although it's documented (you can run the 90FRX report in the Tools\FileSpec subdirectory of the VFP home directory to print the FRX file structure), different report object types have subtle uses of the fields in this table structure and that’s not well documented. Also, there are a lot of fields, making for some pretty long INSERT INTO statements. Also, the size and position values are in 10,000th of an inch (known as FoxPro Report Units, or FRUs), and there are a weird bunch of "fudge" factors you have to apply to these numbers such as the height of a band bar in the Report Designer. So I created a set of classes to handle this programmatically.

The report object classes all in SFRepObj.vcx, and all are based on SFReportBase, a subclass of Custom. Each uses SFRepObj.h as its include file; constants like cnBAND_HEIGHT are much easier to understand than hard-coded values like 2083.333.

## SFReportFile

The only class you actually instantiate directly is SFReportFile. This class represents the report file, and has properties and methods that expose the Report Designer interface programmatically. It'll instantiate objects from other classes when necessary (such as when you add a field to the report).

The public properties of SFReportFile are:

| Property | Purpose                        |
|----------|--------------------------------|
| cDevice  | The name of the printer to use |
| cFontName	| The default font for the report (the default value is "Courier New") |
| cMemberData	| The member data for the report header record |
| cReportFile	| The name of the report file to create |
| cUnits	| The unit of measurement: "C" (characters), "I" (inches), or "M" (centimeters) (the default value is “C”; constants are defined for these values in SFRepObj.h) |
| lAdjustObjectWidths  	| .T. (the default) to ensure no objects are wider than the paper width |
| lNoDataEnvironment   	| .T. (the default) to prevent the DataEnvironment from being edited |
| lNoPreview	| .T. to prevent the report from being previewed or printed in the Report Designer |
| lNoQuickReport	| .T. (the default) to prevent access to the Quick Report function |
| lPrintColumns	| .T. to print records in columns across the page, .F. to print top to bottom then in columns |
| lPrivateDataSession	| .T. to use a private datasession with this report |
| lSummaryBand	| .T. if the report has a summary band |
| lTitleBand	| .T. if the report has a title band |
| lWholePage	| .T. to use the whole page, .F. to use the printable page |
| nColumns	| The number of columns in the report (the default value is 1) |
| nColumnSpacing	| The spacing between columns |
| nDefaultSource	| The default paper source for the report (the default value is -1) |
| nDetailBands	| The number of detail bands in the report |
nFontSize	| The default font size for the report (the default value is 10) |
| nFontStyle	| The default font style for the report |
| nGroups	| The number of groups in the report |
| nLeftMargin	| The left margin for the report |
| nMinPaperWidth	| The minimum paper width |
| nOrientation	| The orientation for the report: 0 = auto-set to landscape if the report is too wide for portrait, 1 = use portrait, 2 = use landscape |
| nPaperLength	| The paper length |
| nPaperSize	| The paper size (the default value is PRTINFO(2); see help for that function for a list of values) |
| nPaperWidth	| The paper width |
| nRepWidth	| The calculated report width |
| nRulerScale	| The scale used for the ruler: 1 = inches, 2 = metric, 3 = pixels |
| nWidth	| The report width |

Other than cReportFile and cUnits, the public properties simply represent the same options for the report you see in the Report Designer. Each has an Assign method that prevents values of the wrong data type or range from being stored. cUnits needs a little explanation. Sometimes, it's easier to express report units (such as the horizontal and vertical position of an object) in characters and lines. For example, if you have a 30-character field, it's easier to specify the width for it on a report as 30 characters rather than figuring out how many inches it should be. All position and size properties in all objects we'll look at should be expressed in the units defined in cUnits.

Here's some sample code that instantiates an SFReportFile object, specifies the name of report to create, indicates that the report has a summary band, and sets the default font for the report.

```foxpro
loReport = newobject('SFReportFile', 'SFRepObj.vcx')
loReport.cReportFile  = 'CustomerReport.frx'
loReport.lSummaryBand = .T.
loReport.cFontName    = 'Arial'
```

SFReportFile's public methods are:

| Method | Description                    |
|--------|--------------------------------|
| CreateDetailBand	| Creates a detail band |
| CreateGroupBand	| Creates a new group band |
| CreateVariable	| Creates a report variable |
| GetHFactor	| Calculates the horizontal factor for the specified font |
| GetPaperWidth	| Gets the max. paper width |
| GetRelativeVPosition	| Get the vertical position of the specified object relative to the start of its band |
| GetReportBand	| Returns an object reference to the specified band |
| GetVariable	| Returns a variable object |
| GetVFactor	| Calculates the vertical factor for the specified font |
| Load	| Loads the specified FRX into report objects |
| LoadFromCursor	| Loads the FRX in the current work area into report objects |
| Save	| Creates the report file |

We'll look at some of these methods as we explore other objects.

## Report bands
SFReportFile creates an object for each band in a report by instantiating an SFReportBand object into a protected property. For example, the Init method uses the following code to create page header, detail, and page footer band objects automatically, because every report has at least these three bands:

```foxpro
with This
    .oPageHeaderBand = newobject('SFReportBand', .ClassLibrary, '', ;
        'Page Header', This)
    .oDetailBand     = newobject('SFReportBand', .ClassLibrary, '', ;
        'Detail', This)
    .oPageFooterBand = newobject('SFReportBand', .ClassLibrary, '', ;
        'Page Footer', This)
endwith
```

The Init method of SFReportBand accepts two parameters: the band type (which it puts into the cBandType property) and an object reference to the SFReportFile object (which it puts into oReport) so it can call back to some SFReportFile methods if necessary.

In addition to the three default bands, you can create additional bands in one of three ways. To specify that the report has a title or summary band, set the lTitleBand or lSummaryBand property to .T. To create group header and footer bands, call the CreateGroupBand method. Groups are automatically numbered in the order they’re defined; a possible enhancement would allow groups to be reordered. To create additional detail bands, call the CreateDetailBand method, passing .T. if you want detail header and footer bands. Like groups, detail bands are automatically numbered in the order they’re defined.

The GetReportBand method returns an object reference to the specified band. In the case of a group header or footer band, you also specify which group number you want the band for. Here's some sample code that sets the height of the page header and detail bands.

```foxpro
loPageHeader = loReport.GetReportBand('Page Header')
loPageHeader.nHeight = 8
loDetail = loReport.GetReportBand('Detail')
loDetail.nHeight = 1
```

By the way, you don't have to hard-code band names as I've done in this example. All band names are defined as constants in SFRepObj.h, the include file for all classes in SFRepObj.vcx. For example, ccBAND_PAGE_HEADER is defined as "PAGE HEADER", so you could use this code:

```foxpro
loPageHeader = loReport.GetReportBand(ccBAND_PAGE_HEADER)
```

The public properties of SFReportBand are:

| Property | Purpose                        |
|----------|--------------------------------|
| cOnEntry	| The "on entry" expression for the band |
| cOnExit	| The "on exit" expression for the band |
| cTargetAlias	| The target alias for detail bands |
| lAdjustBandHeight	| .T. (the default) if the height of the band should be adjusted to fit the objects in it |
| lConstantHeight	| .T. if the band should be a constant height |
| lDeleteObjectsOutsideBand	| .T. (the default) to delete objects outside the band height; this is only used if lAdjustBandHeight is .F. |
| lPageFooter	| .T. if this is a summary band and a page footer should be printed |
| lPageHeader	| .T. if this is a summary band and a page header should be printed |
| lStartOnNewPage		| .T. if this band should start on a new page |
| nCount	| The number of items in the band |
| nHeight	| The height of the band |
| nNewPageWhenLessThan	| Starts a group on a new page when there is less than this much space left on the current page |

As with SFReportFile, they simply expose band options available in the Report Designer as properties. nHeight is expressed in the units defined in the cUnits property of the SFReportFile object; for example, if cUnits is "C," setting nHeight to 8 defines an 8-line high band. One nice feature: if the objects in a band extend below the defined height of the band, saving the report  automatically adjusts the band height if the lAdjustBandHeight property is .T. (the nHeight property isn’t changed, but the band’s record in the FRX is).

SFReportBand just has three public methods that you'll use: Add, GetReportObjects, and Remove (it has a few other public methods but they're called from methods in SFReportFile). Add is probably the method you'll use the most in any of the classes; it adds an object to a band, so you'll call it once for every object in the report. Pass Add the object type you want to add: "field,"" "text," "image," "line," or "box" (these values are defined in SFRepObj.h, so you can use constants rather than hard-coded values), and it returns a reference to the newly created object. GetReportObjects populates the specified array with object references to the objects added to the band. You can optionally pass it a filter condition to only get certain items; typically, the filter would check for a property of the objects being a certain value. You may not use this method yourself, but SFReportFile uses it to output a band and all of its objects to the FRX file. Remove removes the specified object from the band; it isn’t used often since you likely wouldn’t add an object only to remove it later.

SFReportGroup is a subclass of SFReportBand that's specific for group header and footer bands. Its public properties are:

| Property | Purpose                        |
|----------|--------------------------------|
| cExpression	| The group expression |
| lPrintOnEachPage| 	.T. to print the group header on each page |
| lResetPage	| .T. to reset the page number for each page to 1 |
| lStartInNewColumn	| .T. if each group should start in a new column |
| nNewPageWhenLessThan	| Starts a group on a new page when there is less than this much space left on the current page |

The most important one is obviously cExpression, since this determines what constitutes a group.

Here's some code that creates a group, gets a reference to the group header band, and sets the group expression, height, and printing properties.

```foxpro
loReport.CreateGroupBand()
loGroup = loReport.GetReportBand('Group Header', 1)
loGroup.cExpression          = 'CUSTOMER.COUNTRY'
loGroup.nHeight              = 3
loGroup.lPrintOnEachPage     = .T.
loGroup.nNewPageWhenLessThan = 4
```

## Fields and text
The most common thing you'll add to a report band is a field (actually, an expression which could be a field name but can be any valid FoxPro expression), since the whole purpose of a report is to output data. SFReportField is the class used for fields. However, before we talk about SFReportField, let's look at its parent classes.

SFReportRecord is an ancestor class for every class in SFRepObj.vcx except SFReportFile and SFReportBase. It has a few properties that all objects share:

* Recno: the record number of the object in the report.
* cComment: the comment for the report record.
* cMemberData: the member data for the report record.
* cUniqueID: the unique ID for the report record (normally left blank and auto-assigned).
* cUser: the user-defined property.
* nObjectType: contains the OBJTYPE value for the FRX record of the object (for example, fields have an OBJTYPE of 8).

It also has two methods: CreateRecord and ReadFromFRX. CreateRecord is called from the SFReportFile object when it creates a report. SFReportFile creates an object from a record in the FRX using SCATTER NAME loRecord BLANK MEMO to create an object with one property for each field in the record and then passes that object to the CreateRecord method of the SFReportRecord object, which fills in properties of the report record object from values in its own properties. SFReportRecord is an abstract class; it isn't used directly, but is the parent class for other classes. Its CreateRecord method simply ensures a valid report record object was passed and sets the ObjType property of this object (which is written to the OBJTYPE field in the FRX) to its nObjectType property. ReadFromFRX sort of does the opposite: it assign its properties the values from the current record in an open FRX table.

SFReportObject is a subclass of SFReportRecord that's used for report objects (here, I mean "object" in the "thingy" sense, such as a field, rather than "what you get when you instantiate a class" sense). It has these public properties, which represent the minimum set of options for a report object.

| Property | Purpose                        |
|----------|--------------------------------|
| cAlignment	| The alignment for the object: "left", "center", or "right" (constants are defined for these values in SFRepObj.h) |
| cName	| A name for the object (used by SFReportBand.Item to locate an item by name) |
| cPrintWhen	| The Print When expression |
| lAutoCenter	| .T. (the default) to automatically center this object vertically in a row when using character units for the report |
| lPrintInFirstWholeBand	| .T. (the default) to print in the first whole band of a new page |
| lPrintOnNewPage	| .T. to print when the detail band overflows to a new page |
| lPrintRepeats	| .T. (the default) to print repeated values |
| lRemoveLineIfBlank	| .T. to remove a line if there are no objects on it |
| lStretch	| .T. if the object can stretch |
| lTransparent	| .T. (the default) if the object is transparent, .F. for opaque |
| nBackColor	| The object's background color; use an RGB() value (-1 = default) |
| nFloat	| 0 if the object should float in its band, 1 (the default) if it should be positioned relative to the top of the band, or 2 if it should be relative to the bottom of the band (constants are defined for these values in SFRepObj.h) |
| nForeColor	| The object's foreground color; use an RGB() value (-1 = default) |
| nGroup	| Non-zero if this object is grouped with other objects |
| nHeight	| The height of the object |
| nHPosition	| The horizontal position for the object
| nPrintOnGroupChange	| The group number if this object should print on a group change |
| nVPosition	| The vertical position for the object relative to the top of the band |
| nWidth	| The width of the object |

As with other classes, these properties simply expose options available in the Report Designer as properties.

The CreateRecord method first uses DODEFAULT() to execute the behavior of SFReportRecord, then it has some data conversion to do. For example, object colors are stored in the PENRED, PENGREEN, and PENBLUE fields in the FRX record, but we want to have a single nForeColor property that we set (for example, to red using RGB(255, 0, 0)) like we do with VFP controls. Other properties are similar; for example, the value in nFloat updates the FLOAT, TOP, and BOTTOM fields in the FRX.

Finally, we're back to SFReportField, the subclass of SFReportObject that holds information about fields in a report. This class adds the following properties to those of SFReportObject.

| Property | Purpose                        |
|----------|--------------------------------|
| cCaption	| The design-time caption for the field |
| cDataType	| The data type of the expression: "N" for numeric, "D" for date, and "C" for everything else (only required if you'll edit the report in the Report Designer later) |
| cExpression	| The expression to display |
| cFontName	| The font to use (if blank, which it is by default, SFReportFile.cFontName is used) |
| cPicture	| The picture (format and inputmask) for the field |
| cTotalType	| The total type: "N" for none, "C" for count, "S" for sum, "A" for average, "L" for lowest, "H" for highest, "D" for standard deviation, and "V" for variance (constants are defined for these values in SFRepObj.h) |
| lFontBold	| .T. if the object should be bolded |
| lFontItalic	| .T. if the object should be in italics |
| lFontUnderline	| .T. if the object should be underlined |
| lResetOnPage	| .T. to reset the variable at the end of each page; .F. to reset at the end of the report |
| nDataTrimming	| Specifies how the Trim Mode for Character Expressions is set |
| nFontCharSet	| The font charset to use |
| nFontSize	| The font size to use (if 0, which it is by default, SFReportFile.nFontSize is used) |
| nResetOnDetail	| The detail band number to reset the value on |
| nResetOnGroup	| The group number to reset the value on |

As you can see, you have the same control over the properties of an object in a report as you do in the Report Designer.

As with SFReportObject, SFReportField's CreateRecord method uses DODEFAULT() to get the behavior of SFReportRecord and SFReportObject, then it does some data conversion similar to what SFReportObject does (for example, lFontBold, lFontItalic, and lFontUnderline are combined into a single FONTSTYLE value).
SFReportText is a subclass of SFReportField, since it has the same properties but only slightly different behavior. It automatically adds quotes around the expression since text objects always contain literal strings rather than expressions. It also sets the PICTURE field in the FRX to match the alignment of the data (because that's how alignment is handled for text objects), and sizes the object appropriately for the size of the text (in other words, it acts like setting the AutoSize property of a Label control to .T.).

Here's some code that adds text and field objects to the detail band and sets their properties. This code uses characters as the units, so values are in characters or lines.

```foxpro
loObject = loDetail.Add('Text')
loObject.cExpression = 'Country:'
loObject.nVPosition  = 1
loObject.lFontBold   = .T.
loObject = loDetail.Add('Field')
loObject.cExpression = 'CUSTOMER.COUNTRY'
loObject.nWidth      = fsize('COUNTRY', 'CUSTOMER')
loObject.nVPosition  = 1
loObject.nHPosition  = 10
loObject.lFontBold   = .T.
```

## Lines, boxes, and images
SFReportShape is a subclass of SFReportObject that defines the properties for lines and boxes (it isn;t used directly but is subclassed). nPenPattern is the pen pattern for the object: 0 = none, 1 = dotted, 2 = dashed, 3 = dash-dot, 4 = dash-dot-dot, and 8 = normal. nPenSize is the pen size for the line: 0, 1, 2, 4, or 6.

SFReportLine is a subclass of SFReportShape that's used for line objects. It adds one property, lVertical, that you should set to .T. to create a vertical line or .F. (the default) for a horizontal one. Its CreateRecord method sets the height for a horizontal line or the width for a vertical one to the appropriate value based on the pen size.

The following code adds a heavy blue line on line 4 of the page header band:

```foxpro
loObject = loPageHeader.Add('Line')
loObject.nWidth     = lnWidth
loObject.nVPosition = 4
loObject.nHPosition = 0
loObject.nPenSize   = 6
loObject.nForeColor = rgb(0, 0, 255)
```

Boxes use SFReportBox, which is also a subclass of SFReportShape. It adds nCurvature (the curvature of the box corners; the default is 0, meaning no curvature), lStretchToTallest (.T. to stretch the object relative to the tallest object in the band), and nFillPattern (the fill pattern for the object: 0 = none, 1 = solid, 2 = horizontal lines, 3 = vertical lines, 4 = diagonal lines, leaning left, 5 = diagonal lines, leaning right, 6 = grid, 7 = hatch) properties.

SFReportImage, a subclass of SFReportObject, is used for images. Set the cImageSource property to the name of the image file or General field that's the source of the image and nImageSource to 0 if the image comes from a file, 1 if it comes from a General field, or 2 if it's an expression. nStretch defines how to scale the image: 0 = clip, 1 = isometric, and 2 = stretch (the same values used by the Stretch property of an Image control). Its CreateRecord method automatically puts quotes around the image source if a file is used, and sets the appropriate field in the FRX record if the cAlignment property is set to "Center" (only applicable for General fields).

## Report variables
Report variables are defined using the CreateVariable method of the SFReportFile object. This method returns an object reference to the SFReportVariable object it created so you can set properties of the variable. The public properties for variables are:

| Property | Purpose                        |
|----------|--------------------------------|
| cInitialValue	| The initial value	|
| cName	| The variable name	|
| cTotalType	| The total type: "N" for none, "C" for count, "S" for sum, "A" for average, "L" for lowest, "H" for highest, "D" for standard deviation, and "V" for variance (constants are defined for these values in SFRepObj.h)	|
| cValue	| The value to store	|
| lReleaseAtEnd	| .T. to release the variable at the end of the report	|
| lResetOnPage	| .T. to reset the variable at the end of each page; .F. to reset at the end of the report	|
| nResetOnGroup	| The group number to reset the variable on	|

The following code creates a report variable called lnCount and specifies that it should start at 0 and increment by 1 for each record printed in the report. This variable is then printed in the summary band of the report, showing the number of records printed.

```foxpro
loVariable = loReport.CreateVariable()
loVariable.cName         = 'lnCount'
loVariable.cValue        = 1
loVariable.cInitialValue = 0
loVariable.cTotalType    = 'Sum'
loSummary = loReport.GetReportBand('Summary')
loObject  = loSummary.Add('Field')
loObject.cExpression = 'ltrim(str(lnCount)) + ' + ;
  '" record" + iif(lnCount = 1, "", "s") + " printed"'
loObject.nWidth      = 21
loObject.nVPosition  = 2
loObject.nHPosition  = 0
loObject.lFontBold   = .T.
```

## Examples
Customers.prg and Employees.prg are sample programs that create reports for the Customer and Employee tables in the VFP Testdata database. Customers.prg creates (and previews) CustomerReport.frx, which shows customers grouped by country, with the maximum order amount subtotaled by country and totaled at the end of the report. Employees.prg creates EmployeeReport.frx, which shows the name and photo of each employee. These reports aren't intended to be realistic; they just show off various features of the report classes described in this article, including printing images and lines, setting font sizes and object colors, positioning objects in different bands, use of report variables and group bands, etc.

Craig Boyd's [GridExtras](https://tinyurl.com/ycmqo8fg): sorting, incremental searching, and filtering on each column, column selection, and output to Excel and Print Preview. The Print Preview feature uses SFReportFile to dynamically create a report based on the current grid layout.

Altering existing reports is also much easier to do using these classes. Suppose you have a report that contains some sensitive information. Some staff shouldn't see that information, so you use the Print When expression for those fields to not output them unless the staff have the correct permissions. For example, the sample Employees report shows a full employee listing, but Birth Date and Home Phone should only be visible to Human Resources (HR) staff.
 
The Print When expression for those two fields and their column headers is "plHR." The variable plHR is .T. for HR staff and .F. otherwise. When the report is run by non-HR staf, BirthDate and HomePhone don't appear but leave obvious holes in the report layout. What would be nice is if the other columns could move left to take up the empty space.
 
HackReport.prg shows how to do this. The code isn't complicated: instantiate SFReportFile, load the FRX, remove objects in the Page Header and Detail bands that don't appear because of their Print When expressions, and move the rest of the fields the appropriate distance to the left.

```foxpro
loReport = newobject('SFReportFile', 'SFRepObj.vcx')
loReport.Load('Employees.frx')

* Process objects in the page header and detail bands.

loBand = loReport.GetReportBand('Page Header')
ProcessObjects(loBand)
loBand = loReport.GetReportBand('Detail')
ProcessObjects(loBand)

* Save the updated report and run it.

loReport.cReportFile = addbs(sys(2023)) + 'EmployeeReport.frx'
loReport.Save()
report form (loReport.cReportFile) preview

* Specifically release the report object so proper object cleanup occurs.

loReport.Release()

* Remove any objects that fail their PrintWhen expression and move objects to
* the right of those objects on the same line to the left.

function ProcessObjects(toBand)
local laObjects[1], ;
	lnObjects, ;
	lnAdjust, ;
	lnI, ;
	loObject, ;
	lcExpr, ;
	lnVPos, ;
	llVisible
lnObjects = toBand.GetReportObjects(@laObjects)
lnAdjust  = 0
for lnI = 1 to lnObjects
	loObject = laObjects[lnI]
	lcExpr   = loObject.cPrintWhen
	if lnAdjust <> 0 and loObject.nVPosition = lnVPos
		loObject.nHPosition = loObject.nHPosition - lnAdjust
	endif lnAdjust <> 0 ...
	llVisible = .T.
	if not empty(lcExpr)
		try
			llVisible = evaluate(lcExpr)
		catch
		endtry
	endif not empty(lcExpr)
	if not llVisible
		if lnI < lnObjects
			lnAdjust = laObjects[lnI + 1].nHPosition - ;
				loObject.nHPosition
		endif lnI < lnObjects
		toBand.Remove(loObject.cUniqueID)
		lnVPos = loObject.nVPosition
	endif not llVisible
next lnI
```

## Conclusion
Although you might write a fair bit of code to create an FRX using the report object classes, the code is simple: create some objects and set their properties. It sure beats writing 50 INSERT INTO statement with 75 fields to fill for each. Please report (pun intended) to me any suggestions you have for improvements.
