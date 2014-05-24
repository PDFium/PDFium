// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
 
// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#define IDD_JS_MSGBOX                   25600
#define IDD_RESPONSE                    25601
#define IDC_JS_MSG_TEXT                 25602
#define ID_JS_MSG_OK                    25603
#define ID_JS_MSG_CANCEL                25604
#define IDC_JS_MSG_ICON                 25605
#define ID_JS_MSG_YES                   25606
#define ID_JS_MSG_NO                    25607
#define IDC_JS_QUESTION                 25608
#define ID_JS_OK                        25609
#define ID_JS_CANCEL                    25610
#define IDC_JS_ANSWER                   25611
#define IDC_JS_EDIT                     25612
#define IDS_STRING_JSALERT              25613
#define IDS_STRING_JSPARAMERROR         25614
#define IDS_STRING_JSAFNUMBER_KEYSTROKE 25615
#define IDS_STRING_JSINPUTERROR         25616
#define IDS_STRING_JSPARAM_TOOLONG      25617
#define IDS_STRING_JSPARSEDATE          25618
#define IDS_STRING_JSRANGE1             25619
#define IDS_STRING_JSRANGE2             25620
#define IDS_STRING_JSRANGE3             25621
#define IDS_STRING_JSRANGE4             25622
#define IDS_STRING_FILEOPENFAIL         25623
#define IDS_STRING_JSATTENTION          25624
#define IDS_STRING_JSSUBMITS            25625
#define IDS_STRING_JSSUBMITF            25626
#define IDS_STRING_NOTSUPPORT           25627
#define IDS_STRING_JSBUSY               25628
#define IDS_STRING_JSEVENT              25629
#define IDS_STRING_RUN                  25630
#define IDS_STRING_UNHANDLED            25631
#define IDS_STRING_JSPRINT1             25632
#define IDS_STRING_JSPRINT2             25633
#define IDS_STRING_LAUNCHURL            25634
#define IDS_JSPARAM_INCORRECT           25635
#define IDD_JS_CONSOLE                  25636
#define IDS_STRING_SAFEMODEL            25636
#define IDC_EDTSCRIPT                   25637
#define IDC_BTNCLEAR                    25638
#define IDC_EDTOUTPUT                   25639
#define IDC_CHECK_TIPS                  25640
#define IDC_BTNRUN                      25641



static CFX_WideString JSGetStringFromID(CJS_Context* pContext, FX_UINT ID)
{
	switch(ID)
	{                  
	case IDS_STRING_JSALERT:
		return L"Alert";
	case IDS_STRING_JSPARAMERROR:
        return L"The amount of parameters is not correct !";	
	case IDS_STRING_JSAFNUMBER_KEYSTROKE:
		return L"The input value is invalid.";
	case	IDS_STRING_JSINPUTERROR:
        return L"Input error !";
	case	IDS_STRING_JSPARAM_TOOLONG:
		return L"The value you are going to input is too long.";
	case	IDS_STRING_JSPARSEDATE:
		return L"The input string can't be parsed to a valid date time (%s).";
	case	IDS_STRING_JSRANGE1:
		return L"Invalid value: must be greater or equal to %s and less than or equal to %s.";	
	case	IDS_STRING_JSRANGE2:
		return L"Invalid value: must be greater or equal to %s.";
	case	IDS_STRING_JSRANGE3:
		return L"Invalid value: must be less than or equal to %s.";
	case	IDS_STRING_JSRANGE4:
		return L"Range Error";	
	case	IDS_STRING_FILEOPENFAIL:
        return L"Opening file failed.";
	case	IDS_STRING_JSATTENTION:
		return L"Attention";	
	case	IDS_STRING_JSSUBMITS:
		return L"Submit form successfully!";
	case	IDS_STRING_JSSUBMITF:
		return L"Submit form failed!";	
	case	IDS_STRING_NOTSUPPORT:
		return L"Not supported.";
	case	IDS_STRING_JSBUSY:
		return L"System is busy!";	
	case	IDS_STRING_JSEVENT:
		return L"The event of the formfield exists!";	
	case	IDS_STRING_RUN:
		return L"It runs successfully.";
	case	IDS_STRING_UNHANDLED:
		return L"An unhandled error!";
	case	IDS_STRING_JSPRINT1:
		return L"The second parameter can't convert to Date!";
	case	IDS_STRING_JSPRINT2:
		return L"The second parameter isn't a valid Date!";
	case	IDS_STRING_LAUNCHURL:
		return L"The Document is trying to connect to \r\n%s\r\nIf you trust the site, choose OK. If you don't trust the site, choose Cancel.";	
	case	IDS_JSPARAM_INCORRECT:
		return L"The parameter you inputted is incorrect!";
	case	IDS_STRING_SAFEMODEL:
		return L"Secure reading mode";
	default:
		return L"";

	}
}

