#ifndef __TASKMENU_H_
#define __TASKMENU_H_


#include "stm32F10x.h"
#include "sys.h"
#include "system.h"


#define  AddBASE     			2048
#define  Phonemember   			AddBASE*0
#define  PhoneAddMember		 	AddBASE*1
#define  PhoneDelete  			AddBASE*2
#define  PhoneSetup				AddBASE*3

#define  AlarmRecord			AddBASE*4
#define  DisarmRecord			AddBASE*5
#define  ArmRecord				AddBASE*6

#define  SetupDelay				AddBASE*8
#define  SetupTime				AddBASE*7
#define  SetupAuto				AddBASE*9
#define  SetupVolume			AddBASE*29
#define  SetupKeyTone			AddBASE*11
#define  SetupLanguage			AddBASE*13
#define  SetupSMS				AddBASE*12
#define	 SetupAlarm  			AlarmRecord
#define  SetupSiren				AddBASE*23
#define  SetupRing				AddBASE*24
#define  SetupScreensaver		AddBASE*39
#define  SMSeply				AddBASE*40

#define  SecurityAdmin			AddBASE*38
#define  SecurityUser			AddBASE*15
#define  SecurityDefault		PhoneSetup

#define  VoiceRecord			AddBASE*17
#define  VoicePlay				AddBASE*18


#define  Remote					AddBASE*19
#define  ZoneAdd				AddBASE*20
#define  ZoneSetup				AddBASE*25
#define  ZoneList				AddBASE*22
#define  ZoneDelete				PhoneDelete

#define	 Sunday					AddBASE*26
#define  Monday					AddBASE*27
#define  Tuesday				AddBASE*28
#define  Wednesday				AddBASE*29
#define  Thursday				AddBASE*30
#define  Friday					AddBASE*31
#define  Saturday				AddBASE*32


#define  PowerOn				AddBASE*34
#define  PowerOff				AddBASE*35
#define  BatteryLow				AddBASE*36
#define  Detector				AddBASE*37

#define Assd					AddBASE*42



#define PromptAreaBASE  		0x00
#define IconBASE				0x10
#define FormBASE				0x20



#define PromptAreaBASE  		0x00
#define IconBASE				0x10
#define FormBASE				0x20
#define LabelBASE				0x30
#define ListBoxBASE				0x40
#define CheckBoxBASE			0x50


#define CheckBoxID1				CheckBoxBASE+1
#define CheckBoxID2				CheckBoxBASE+2
#define CheckBoxID3				CheckBoxBASE+3
#define CheckBoxID4				CheckBoxBASE+4
#define CheckBoxID5				CheckBoxBASE+5
#define CheckBoxID6				CheckBoxBASE+6
#define CheckBoxID7				CheckBoxBASE+7




#define LabelID1				LabelBASE+1


#define ListBoxID1			 	ListBoxBASE+1
#define ListBoxID2			 	ListBoxBASE+2
#define ListBoxID3			 	ListBoxBASE+3
#define ListBoxID4			 	ListBoxBASE+4
#define ListBoxID5			 	ListBoxBASE+5
#define ListBoxID6				ListBoxBASE+6


#define PromptAreaID1    PromptAreaBASE+1
#define PromptAreaID2    PromptAreaBASE+2
#define PromptAreaID3    PromptAreaBASE+3
#define PromptAreaID39    PromptAreaBASE+39
#define PromptAreaID4    PromptAreaBASE+4
#define PromptAreaID5    PromptAreaBASE+5
#define PromptAreaID6    PromptAreaBASE+6
#define PromptAreaID7    PromptAreaBASE+7
#define PromptAreaID8    PromptAreaBASE+8
#define PromptAreaID9    PromptAreaBASE+9
#define PromptAreaID10   PromptAreaBASE+10
#define PromptAreaID11   PromptAreaBASE+11
#define PromptAreaID12   PromptAreaBASE+12
#define PromptAreaID13   PromptAreaBASE+13
#define PromptAreaID14   PromptAreaBASE+14
#define PromptAreaID15   PromptAreaBASE+15
#define	PromptAreaID16   PromptAreaBASE+16
#define PromptAreaID17   PromptAreaBASE+17
#define PromptAreaID18   PromptAreaBASE+18
#define PromptAreaID19   PromptAreaBASE+19
#define PromptAreaID20   PromptAreaBASE+20



#define IconID1				IconBASE+1
#define IconID2				IconBASE+2
#define IconID3				IconBASE+3
#define IconID4				IconBASE+4
#define IconID5				IconBASE+5
#define IconID6				IconBASE+6
#define IconID7				IconBASE+7
#define IconID8				IconBASE+8
#define IconID9				IconBASE+9
#define IconID10			IconBASE+10
#define IconID11			IconBASE+11
#define IconID12			IconBASE+12


#define Form0         FormBASE
#define Form1				  FormBASE+1
#define Form2   			FormBASE+2
#define Form3				  FormBASE+3
#define Form4   			FormBASE+4
#define Form5				  FormBASE+5
#define Form6   			FormBASE+6
#define Form7				  FormBASE+7
#define Form8   			FormBASE+8
#define Form9				  FormBASE+9
#define Form10				FormBASE+10
#define Form11  			FormBASE+11
#define Form12  			FormBASE+12
#define	Form13				FormBASE+13
#define	Form14				FormBASE+14
#define Form15				FormBASE+15
#define Form16				FormBASE+16
#define Form79        FormBASE+79


#define IconMenuForm  Form79

#define MainIconSize       56
#define MainMenuIconWidth  100
#define MainMenuIconHeight 100

typedef struct
{
	string Name;
	u32    BitmapAdd;	
}FuncListStruct;

typedef struct Menu
{
	Form 	  *FocusFormPointer;
	
//	Form   	LogoForm;
	Form   	StandbyForm;
  
  Form    MainMenuForm;
  
	Form   	ContactsForm;
	Form	  PhoneMenuForm;
  Form   	SensorsForm;
  Form   	VoiceForm;
	Form   	SetupForm;
	Form   	SecurityForm;
	Form   	LogForm;
	Form	  AboutForm;
  
	Form	  SetupListForm;
	Form	  PhoneListForm;
	Form	  LogListForm;
	Form   	SecurityListForm;
	Form   	VoiceListForm;
	Form	  SensorsListForm;
		
	Form	  DelayForm;
	Form	  AutoForm;
	Form 	  AutoSetupForm;
	Form	  DateForm;
	Form	  VolumeForm;
	
	Form	  KeyToneForm;
	Form	  LanguageForm;
	Form	  SMSForm;
	Form	  SirenTimeForm;
	Form	  RingTimesForm;
	Form	  ScreensaverForm;
	Form	  PanelPowerOFFForm;
  Form    AlarmModeForm;
//	Form	  PanelPowerONForm;
//	Form	  PanelLowForm;
//	Form	  DetectorLowForm;
	Form		SystemStatesForm;
	
	Form	  PhoneEditForm;
	Form	  PhoneDialogBoxForm;
	Form	  AddPhoneForm;
	Form	  PhoneSetForm;
	Form	  PhoneDelForm;
	
	Form	  RemoteForm;
	Form	  ZoneSetForm;
	Form 	  WirelessSirenForm;
	Form	  ReadyArmForm;
	
	
//	Form	  RecordForm;
//	Form	  PlayForm;
	
	Form	  PasswordForm;
	Form	  RestoreForm;
	
	Form 	  SureRestForm;
	Form	  WaitForm;
	
	Form	  RemoteConfigForm;
	Form	  RemoteCodeForm;
	Form	  RemoteSoundForm;
	Form	  RemoteDelForm;
  Form    DetectorDelForm;
  
  Form    AlarmForm;
	
	Form	  ZoneConfigListForm;
	Form	  ZoneAttributeForm;
	Form	  ZoneSirenForm;
    
  Form    LogStateListForm;
	Form	  CallForm;
	Form	  DialForm;
	Form	  ContactIDForm;
	Form	  AboutInfoForm;
	
}MenuStruct;
  

void MenuTask (void);
#endif



