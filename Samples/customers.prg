* Set a path to find the files we need.

lcFolder = addbs(justpath(sys(16)))
lcPath   = fullpath('..\', lcFolder)
set path to (lcFolder)
set path to (lcPath) additive

* Open the customer table if necessary.

if used('CUSTOMER')
	select CUSTOMER
else
	select 0
	use CUSTOMER
endif used('CUSTOMER')
set order to COUNTRY

* Create an SFReportFile object and set some properties. We'll use the default
* character units.

loReport = newobject('SFReportFile', 'SFRepObj.vcx')
loReport.cReportFile  = 'CustomerReport.frx'
loReport.lSummaryBand = .T.
loReport.cFontName    = 'Arial'

* Set the height of the page header band.

loPageHeader = loReport.GetReportBand('Page Header')
loPageHeader.nHeight = 6

* Set the height of the detail band.

loDetail = loReport.GetReportBand('Detail')
loDetail.nHeight = 1

* Set the height of the summary band.

loSummary = loReport.GetReportBand('Summary')
loSummary.nHeight = 3

* Insert a group band and put objects in it.

loReport.CreateGroupBand()
loGroup = loReport.GetReportBand('Group Header', 1)
loGroup.cExpression          = 'COUNTRY'
loGroup.nHeight              = 3
loGroup.lPrintOnEachPage     = .T.
loGroup.nNewPageWhenLessThan = 4
loObject = loGroup.Add('Text')
loObject.cExpression = 'Country:'
loObject.nVPosition  = 1
loObject.lFontBold   = .T.
loObject = loGroup.Add('Field')
loObject.cExpression = 'COUNTRY'
loObject.nWidth      = fsize('COUNTRY')
loObject.nVPosition  = 1
loObject.nHPosition  = 8
loObject.lFontBold   = .T.
loGroupFooter = loReport.GetReportBand('Group Footer', 1)
loGroupFooter.nHeight = 2

* Insert column heading and field records into the appropriate band.
	
lnFields = afields(laFields)
for lnI = lnFields to 1 step -1
	if not inlist(laFields[lnI, 1], 'COMPANY', 'CITY', 'MAXORDAMT')
		adel(laFields, lnI)
		dimension laFields[alen(laFields, 1) - 1, alen(laFields, 2)]
	endif not inlist(laFields[lnI, 1], ...
next lnI
lnFields = alen(laFields, 1)
lnWidth  = 0
for lnI = 1 to lnFields

* Get the attributes for the current column.

	lcField  = laFields[lnI, 1]
	lcColumn = proper(strtran(lcField, '_', ' '))
	if laFields[lnI, 2] $ 'NFIBY'
		lcAlign      = 'Right'
		lnFieldWidth = iif(laFields[lnI, 2] $ 'IBY', 12, laFields[lnI, 3])
		lcPicture    = '9,999,999.99'
	else
		lcAlign      = 'Left'
		lnFieldWidth = laFields[lnI, 3]
		lcPicture    = ''
	endif laFields[lnI, 2] $ 'NFIBY'

* Determine the horizontal and vertical positions.

	lnHPos = lnWidth

* Insert the column heading.

	loObject = loPageHeader.Add('Text')
	loObject.cExpression = lcColumn
	loObject.nVPosition  = 5
	loObject.nHPosition  = lnHPos
	loObject.lFontBold   = .T.
	if lcAlign = 'Right'
		loObject.nHPosition = lnHPos + lnFieldWidth - len(lcColumn)
	endif lcAlign = 'Right'
	loObject.cAlignment = lcAlign

* Insert the field.

	loObject = loDetail.Add('Field')
	loObject.cExpression = lcField
	loObject.nWidth      = lnFieldWidth
	loObject.nVPosition  = 0
	loObject.nHPosition  = lnHPos
	loObject.cAlignment  = lcAlign
	loObject.cPicture    = lcPicture

* If this is a numeric field, update the summary band and the appropriate
* group band footer.

	if laFields[lnI, 2] $ 'NFIBY'
		loObject.cDataType = 'N'
		lnVPos    = loSummary.nHeight - 1
		loObject  = loGroupFooter.Add('Field')
		loObject.cExpression   = lcField
		loObject.nWidth        = lnFieldWidth + 2
		loObject.nVPosition    = 1
		loObject.nHPosition    = lnHPos - 2
		loObject.cAlignment    = lcAlign
		loObject.nResetOnGroup = 1
		loObject.cTotalType    = 'Sum'
		loObject.cPicture      = lcPicture
		loObject.lFontBold     = .T.
		loObject.cDataType     = 'N'
		loObject  = loSummary.Add('Field')
		loObject.cExpression = lcField
		loObject.nWidth      = lnFieldWidth + 2
		loObject.nVPosition  = 2
		loObject.nHPosition  = lnHPos - 2
		loObject.cAlignment  = lcAlign
		loObject.cTotalType  = 'Sum'
		loObject.cPicture    = lcPicture
		loObject.lFontBold   = .T.
		loObject.cDataType   = 'N'
	endif laFields[lnI, 2] $ 'NFIBY'

* Increment the current horizontal position.

	lnWidth = lnHPos + lnFieldWidth + 1
next lnI
lnWidth = lnWidth - 1

* Insert the page header.

loObject = loPageHeader.Add('Field')
loObject.cExpression = '"Customer Report"'
loObject.nWidth      = lnWidth
loObject.nHPosition  = 0
loObject.nVPosition  = 0
loObject.cAlignment  = 'Center'
loObject.lFontBold   = .T.

* Insert the date.

loObject = loPageHeader.Add('Field')
loObject.cExpression = 'date()'
loObject.nWidth      = 10
loObject.nHPosition  = 0
loObject.nVPosition  = 2
loObject.lFontBold   = .T.

* Insert the page number.

lnPagenoWidth = 3
loObject = loPageHeader.Add('Text')
loObject.cExpression = 'Page'
loObject.nVPosition  = 2
loObject.nHPosition  = lnWidth - (lnPagenoWidth + 4)
loObject.lFontBold   = .T.
loObject = loPageHeader.Add('Field')
loObject.cExpression = '_pageno'
loObject.nWidth      = lnPagenoWidth
loObject.nVPosition  = 2
loObject.nHPosition  = lnWidth - lnPagenoWidth
loObject.cAlignment  = 'Right'
loObject.lFontBold   = .T.

* Insert a line above the column headings.

loObject = loPageHeader.Add('Line')
loObject.nWidth     = lnWidth
loObject.nVPosition = 4
loObject.nHPosition = 0
loObject.nPenSize   = 6
loObject.nForeColor = rgb(0, 0, 255)

* Insert a line below the column headings.

loObject = loPageHeader.Add('Line')
loObject.nWidth     = lnWidth
loObject.nVPosition = 6
loObject.nHPosition = 0

* Add objects in the summary band.

loObject = loSummary.Add('Line')
loObject.nWidth     = lnWidth
loObject.nVPosition = 1
loObject.nHPosition = 0
loObject = loSummary.Add('Text')
loObject.cExpression = 'Totals:'
loObject.nVPosition  = 2
loObject.nHPosition  = 0
loObject.lFontBold   = .T.
loObject = loSummary.Add('Field')
loObject.cExpression = 'ltrim(str(lnCount)) + " record" + ' + ;
	'iif(lnCount = 1, "", "s") + " printed"'
loObject.nWidth      = 21
loObject.nVPosition  = 2
loObject.nHPosition  = 8
loObject.lFontBold   = .T.

* Create the report variables.

loVariable = loReport.CreateVariable()
loVariable.cName         = 'lnCount'
loVariable.cValue        = 1
loVariable.cInitialValue = 0
loVariable.cTotalType    = 'Sum'

* Create the report, then let's run it.

loReport.Save()
modify report (loReport.cReportFile)
report form (loReport.cReportFile) preview

* Specifically release the report object so proper object cleanup occurs.

loReport.Release()








