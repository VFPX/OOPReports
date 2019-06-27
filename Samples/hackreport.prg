* Set a path to find the files we need.

lcFolder = addbs(justpath(sys(16)))
lcPath   = fullpath('..\', lcFolder)
set path to (lcFolder)
set path to (lcPath) additive

* Open the employee table if necessary.

if used('EMPLOYEE')
	select EMPLOYEE
else
	select 0
	use EMPLOYEE
endif used('EMPLOYEE')

* Run the report with plHR .T. so we can see all fields.

messagebox("Here's the report with all fields")
plHR = .T.
report form Employees preview

* Now run it with plHR .F. so some fields are missing.

messagebox("Here's the report with some fields missing")
plHR = .F.
report form Employees preview

* Create an SFReportFile object and load the Employees report.

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
messagebox("Here's the report with fields removed")
modify report (loReport.cReportFile)
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
 