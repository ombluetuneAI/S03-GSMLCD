
#include "MenuList.h"


#define SetupTabSize	13

//���˵�
extern void WirelessOpen(void);

MenuListStruct MainMenu=
{
	0,0,1,0,0,
	{&App.Menu.StandbyForm},
    null,
    null,
    null,
    null,
    null,
    WirelessOpen,

};

MenuListStruct MainIconMenu=
{
  0,0,1,0,0,
  {&App.Menu.MainMenuForm},
};
//ͼ��˵�
MenuListStruct IconMenu=
{   
	0,//��һ��
	7,//���һ��
	8,//����Ŀ
	0,//��ǰѡ����
  0,
	{
		&App.Menu.ContactsForm,
    &App.Menu.ContactsForm,
    &App.Menu.ContactsForm,
    &App.Menu.ContactsForm,
    &App.Menu.ContactsForm,
    &App.Menu.ContactsForm,
    &App.Menu.ContactsForm,
    &App.Menu.ContactsForm,
//		&App.Menu.AboutForm,
//		&App.Menu.LogForm,
//		&App.Menu.SecurityForm,
//		&App.Menu.SetupForm,
//		&App.Menu.VoiceForm,
//		&App.Menu.SensorsForm,	
//		&App.Menu.PhoneMenuForm,
	},

};
//�绰
static MenuListStruct PhoneMenu=
{
	0,0,1,0,0,
	{
		&App.Menu.PhoneListForm,
	},

};
//����
static MenuListStruct AboutList=
{
	0,0,1,0,0,
	{
		&App.Menu.AboutInfoForm,
	},
};
static MenuListStruct AboutInfoList=
{
  0,0,1,0,0,
  {
    &App.Menu.AboutInfoForm,
  },
};
//���ò˵�
MenuListStruct SetupMenu=
{
	0,0,1,0,0,
	{
		&App.Menu.SetupListForm,
	},
};
//�绰�б�


static MenuListStruct SensorLists[4]=
{
	{
		0,0,1,0,0,&App.Menu.RemoteForm,
	},
	{
		0,0,1,0,0,&App.Menu.ZoneSetForm,
	},
	{
		0,0,1,0,0,&App.Menu.WirelessSirenForm,
	},
	{
		0,0,1,0,0,&App.Menu.ReadyArmForm,NULL
	},
};
//void ZoneAttributeProcess(void);
extern void ZoneWirelessClose(void);//�رշ���
static MenuListStruct ZoneConfig=
{
	0,0,1,0,0,{&App.Menu.ZoneConfigListForm,},
	NULL,
	NULL,
	NULL,
	null,
	null,
	ZoneWirelessClose,
};
extern void ZoneAttributeSave(void);
extern void ZoneAttributeProcess(Form *formPointer);
static MenuListStruct ZoneAttribute=
{
	0,0,1,0,0,{&App.Menu.ZoneAttributeForm,},
	null,
	ZoneAttributeSave,
	null,
	ZoneAttributeProcess,
};

extern void SirneState(Form *focusFormPointer);
extern void SirenStateSave(void);
static MenuListStruct ZoneSiren=
{
	0,0,1,0,0,{&App.Menu.ZoneSirenForm,},	
	null,
	SirenStateSave,
	NULL,
	SirneState,
};
extern void DelectorDeleteCancel(void);
static MenuListStruct DetectorDelete=
{
	0,0,1,0,0,{&App.Menu.DetectorDelForm,},
	null,
	null,
	null,
	null,
	DelectorDeleteCancel,
};
static MenuListStruct RemoteConfig=
{
	0,0,1,0,0,{&App.Menu.RemoteConfigForm,},
	null,
	NULL,
	null,
	null,
	NULL,
	ZoneWirelessClose,
};
extern void CodeWirelessOpen(void);
static MenuListStruct RemoteCode=
{
	0,0,1,0,0,{&App.Menu.RemoteCodeForm},
	CodeWirelessOpen,
};
static MenuListStruct DetectorCode=
{
	0,0,1,0,0,{&App.Menu.RemoteCodeForm},
	CodeWirelessOpen,
};
extern void RemoteTypeAddr(Form *formPointer);
static MenuListStruct SoundConfig=
{
	0,0,1,0,0,{&App.Menu.RemoteSoundForm,},
	null,
	null,
	NULL,
	RemoteTypeAddr,
	null,
};
extern void DeleteRemoteCancel(void);
static MenuListStruct DeleteRemote=
{
	0,0,1,0,0,{&App.Menu.RemoteDelForm,},
	null,
	null,
	NULL,
	null,
	DeleteRemoteCancel,
};

static MenuListStruct DeleteRemoteList[3]=
{
	{0,0,1,0,0,{&App.Menu.RemoteForm,},//ɾ����ǰ����
	},
	{0,0,1,0,0,{&App.Menu.RemoteForm,},//ɾ�����к���
	},
	{0,0,1,0,0,{&App.Menu.RemoteForm,},
	},//������һ��
};
extern void LogAddrGet(Form *pointer);
//��־
static MenuListStruct LogList=
{
	0,0,1,0,0,&App.Menu.LogListForm,
};
static MenuListStruct LogDetailsList=
{
    0,0,1,0,0,{&App.Menu.LogStateListForm,},
    null,
    null,
    null,
    LogAddrGet,
};
//��ȫ
static MenuListStruct SecurityList=
{
	0,
	0,
	1,
	0,
  0,
	{
		&App.Menu.SecurityListForm,
	}

};
//����
static MenuListStruct VoiceList=
{
	0,
	0,
	1,
	0,
  0,
	{
		&App.Menu.VoiceListForm,
	}

};
//����
static MenuListStruct DefenseList=
{
	0,0,1,0,0,&App.Menu.SensorsListForm,

};
extern void RingTimesSetup(void);
extern void KeyToneSetup(void);
static MenuListStruct SetupFuncTab[SetupTabSize]=
{
	{
		0,0,1,0,0,{&App.Menu.DelayForm,},null,null,
	},
	{
		0,0,1,0,0,{&App.Menu.AutoForm,},null,null,
	},
	{
		0,0,1,0,0,{&App.Menu.DateForm,},null,null,
	},	
	{
		0,0,1,0,0,{&App.Menu.VolumeForm,},
		null,
		null,
		null,
	},	
	{
		0,0,1,0,0,{&App.Menu.KeyToneForm,},
		null,
		KeyToneSetup,
	},	
	{
		0,0,1,0,0,{&App.Menu.LanguageForm,},null,null,
	},	
	{
		0,0,1,0,0,{&App.Menu.SMSForm,},null,null,
	},	
// 	{
// 		0,0,1,0,{&App.Menu.PowerAlarmForm,},null,null,
// 	},
	{
		0,0,1,0,0,{&App.Menu.SirenTimeForm,},null,null,
	},	
	{
		0,0,1,0,0,{&App.Menu.RingTimesForm,},
		null,
		RingTimesSetup,
	},
	{
		0,0,1,0,0,{&App.Menu.ScreensaverForm,},null,null,
	},
	{
		0,0,1,0,0,{&App.Menu.ContactIDForm,},null,null,
	},
	{
		0,0,1,0,0,{&App.Menu.PanelPowerOFFForm,},null,null,
	},
//	{
//		0,0,1,0,0,{&App.Menu.AlarmModeForm,},null,null,
//	},
//	{
//		0,0,1,0,0,{&App.Menu.PanelLowForm,},null,null,
//	},
//	{
//		0,0,1,0,0,{&App.Menu.DetectorLowForm,},null,null,
//	}
	{0,0,1,0,0,{&App.Menu.SystemStatesForm,},null,null},
};


//static MenuListStruct Record=
//{
//	0,0,1,0,0,{&App.Menu.RecordForm,},NULL,NULL,
//};
//static MenuListStruct PlayRecording=
//{
//	0,0,1,0,0,{&App.Menu.PlayForm,},NULL,NULL,
//};

static MenuListStruct Securinty[2]=
{
	{
		0,0,1,0,0,&App.Menu.PasswordForm,
	},
	{
		0,0,1,0,0,&App.Menu.RestoreForm,
	}
};
//��λѯ�� ȷ��ҳ��
extern void WatiFunc(void);
static MenuListStruct SureMessage=
{
	0,0,1,0,0,{&App.Menu.SureRestForm,},
	null,
	null,
	null,
	null,
	WatiFunc,
};
extern void DeleteCancel(void);
extern void PhoneTypeAddr(Form *formPointer);
static MenuListStruct PhoneSetupAdd=
{
	0,0,1,0,0,{&App.Menu.PhoneEditForm,},//�����б�Ĺ���ѡ�����б�
	null,
	null,
	null,
	PhoneTypeAddr,
	null,
  null,
};
extern void DefaultsSetup(void);
static MenuListStruct Defaults=
{
	0,0,1,0,0,
	{&App.Menu.WaitForm,},//ȷ���Ƿ�ɾ��
	DefaultsSetup,
	null,
	null,
	null,
	null,
  null,
  null,
};

static MenuListStruct NumberEdit[3]=
{
	{0,0,1,0,0,{&App.Menu.AddPhoneForm,}},//��Ӻ���
	{0,0,1,0,0,{&App.Menu.PhoneSetForm,},//���ú���
		null,
		null,
		null,
		PhoneTypeAddr,
		null,
	},
	{0,0,1,0,0,{&App.Menu.PhoneDelForm,},//ɾ������
		null,
		null,
		null,
		null,
		DeleteCancel,//����ϴεĲ�����¼
	},
};
extern void DeleteAllNumber(void);
static MenuListStruct DeleteNumber[3]=
{
	{0,0,1,0,0,{&App.Menu.PhoneListForm,},//ɾ����ǰ����
	},
	{0,0,1,0,0,{&App.Menu.PhoneListForm,},//ɾ�����к���
	},
	{0,0,1,0,0,{&App.Menu.PhoneListForm,},
	},//������һ��
};
extern void AutoAddrGet(Form *pointer);
static MenuListStruct AutoArmDisarm=
{
	0,0,1,0,0,{&App.Menu.AutoSetupForm,},//ɾ������
	null,
	null,
	null,
	AutoAddrGet,
  null,
  null,
};
extern void DialBufferClean(Form *pointer);
/**********************************************************************
*
*�绰��������
*
**********************************************************************/
static MenuListStruct CallList=
{
	0,0,1,0,0,{&App.Menu.CallForm,},//
	null,
	null,
	null,
	DialBufferClean,
  null,
  null,	
};
void KeyPadCallProcess(Form *focusFormPointer);
static MenuListStruct DialList=
{
	0,0,1,0,0,{&App.Menu.DialForm,},
	null,
	null,
	null,
	KeyPadCallProcess,
  null,
  null,		
};
static void MenuCall(void)
{
	CallList.SubMenus=malloc(sizeof(&CallList));
	CallList.SubMenus[0]=&DialList;
	CallList.Parent=&IconMenu;
	
	DialList.SubMenus=malloc(sizeof(&DialList));
	DialList.SubMenus[0]=NULL;
	DialList.Parent=&CallList;
	
	//CurrentCall =&CallList;
	
}
//************************************************************************************************


/***************************************************************************************************
*����		��	���ò˵�������
*�������	��	��
*�������	��	��
***************************************************************************************************/
static void MenuSetup(void)
{
	u8 i;
	SetupMenu.SubMenus=malloc(sizeof(SetupMenu)*SetupTabSize);
	for(i=0;i<SetupTabSize;i++)
	{
		SetupFuncTab[i].SubMenus=malloc(sizeof(SetupFuncTab[i])*1);
		SetupFuncTab[i].Parent=&SetupMenu;
		SetupMenu.SubMenus[i]=&SetupFuncTab[i];//��һ���ڵ�
	}
	
	SetupFuncTab[1].SubMenus=malloc(sizeof(SetupFuncTab[1])*8);//
	for(i=0;i<8;i++)
  {
		SetupFuncTab[1].SubMenus[i]=&AutoArmDisarm;
  }

	AutoArmDisarm.SubMenus=malloc(sizeof(&AutoArmDisarm)*1);
	AutoArmDisarm.SubMenus[0]=NULL;
	AutoArmDisarm.Parent=&SetupFuncTab[1];

	SetupFuncTab[4].SubMenus=malloc(sizeof(SetupFuncTab[4])*1);
	SetupFuncTab[4].SubMenus[0]=NULL;	
	
	SetupMenu.Parent=&IconMenu;
}
/**********************************************************************************

*����		���绰��������

**********************************************************************************/
static void MenuPhone(void)
{
	u8 i;
	//�绰�˵�
	PhoneMenu.SubMenus = malloc(sizeof(&PhoneMenu)*8);
	for(i=0;i<9;i++)
  {
		PhoneMenu.SubMenus[i]=&PhoneSetupAdd;	
  }
 	PhoneMenu.Parent=&IconMenu;
	
	PhoneSetupAdd.SubMenus=malloc(sizeof(&PhoneSetupAdd)*3);
	PhoneSetupAdd.SubMenus[0]=&NumberEdit[0];//��Ӻ���
	PhoneSetupAdd.SubMenus[1]=&NumberEdit[1];//����
	PhoneSetupAdd.SubMenus[2]=&NumberEdit[2];//ɾ��ѡ����
	PhoneSetupAdd.Parent=&PhoneMenu;	
	
	NumberEdit[0].SubMenus=malloc(sizeof(&NumberEdit[0]));
	NumberEdit[0].SubMenus[0]=null;
	NumberEdit[0].Parent=&PhoneSetupAdd;

	NumberEdit[1].SubMenus=malloc(sizeof(&NumberEdit[1])*2);
	NumberEdit[1].SubMenus[0]=null;
	NumberEdit[1].SubMenus[1]=null;
	NumberEdit[1].Parent=&PhoneSetupAdd;	
	
	NumberEdit[2].SubMenus=malloc(sizeof(&NumberEdit[2])*3);
	NumberEdit[2].SubMenus[0]=&DeleteNumber[1];
	NumberEdit[2].SubMenus[1]=&DeleteNumber[1];
	NumberEdit[2].SubMenus[2]=&PhoneSetupAdd;
	NumberEdit[2].Parent=&PhoneSetupAdd;
	
	DeleteNumber[1].SubMenus=malloc(sizeof(DeleteNumber[1])*8);
	for(i=0;i<8;i++)
  {
		DeleteNumber[1].SubMenus[i]=&PhoneSetupAdd;	
  }
	DeleteNumber[1].Parent=&IconMenu;
	
}
/***********************************************************************

***********************************************************************/
void MenuZone(void)
{
	u8 i;
	
	i=4;
	DefenseList.SubMenus=malloc(sizeof(&DefenseList)*4);
	while(i--)
	{
		DefenseList.SubMenus[i]=&SensorLists[i];
	}
	DefenseList.Parent=&IconMenu;	

	SensorLists[3].SubMenus=malloc(sizeof(&SensorLists[3])*1);
	SensorLists[3].SubMenus[0]=NULL;
	SensorLists[3].Parent=&DefenseList;	
	
	SensorLists[2].SubMenus=malloc(sizeof(&SensorLists[2])*1);
	SensorLists[2].SubMenus[0]=NULL;
//	SensorLists[2].SubMenus[1]=NULL;
	SensorLists[2].Parent=&DefenseList;	
	
	SensorLists[1].SubMenus=malloc(sizeof(&SensorLists[1])*30);
	for(i=0;i<30;i++)
	{
		SensorLists[1].SubMenus[i]=&ZoneConfig;
		SensorLists[1].Parent=&DefenseList;	
	}
	ZoneConfig.SubMenus=malloc(sizeof(ZoneConfig)*4);
	ZoneConfig.SubMenus[0]=&DetectorCode;
	ZoneConfig.SubMenus[1]=&ZoneAttribute;
	ZoneConfig.SubMenus[2]=&ZoneSiren;
	ZoneConfig.SubMenus[3]=&DetectorDelete;
	ZoneConfig.Parent=&SensorLists[1];
	
	ZoneSiren.SubMenus=malloc(sizeof(ZoneSiren));
	ZoneSiren.SubMenus[0]=NULL;
	ZoneSiren.Parent=&ZoneConfig;

	DetectorDelete.SubMenus=malloc(sizeof(&DetectorDelete)*3);
	DetectorDelete.SubMenus[0]=&SensorLists[1];
	DetectorDelete.SubMenus[1]=&SensorLists[1];
	DetectorDelete.SubMenus[2]=&ZoneConfig;
	DetectorDelete.Parent=&ZoneConfig;    
	
	ZoneAttribute.SubMenus=malloc(sizeof(ZoneAttribute)*6);
	ZoneAttribute.SubMenus[0]=NULL;
	ZoneAttribute.SubMenus[1]=NULL;
	ZoneAttribute.SubMenus[2]=NULL;
	ZoneAttribute.SubMenus[3]=NULL;
	ZoneAttribute.SubMenus[4]=NULL;
	ZoneAttribute.SubMenus[5]=NULL;
	ZoneAttribute.Parent=&ZoneConfig;
	
	
	DetectorCode.SubMenus=malloc(sizeof(DetectorCode));
	DetectorCode.SubMenus[0]=null;
	DetectorCode.Parent=&ZoneConfig;
	
	SensorLists[0].SubMenus=malloc(sizeof(&SensorLists[0])*8);
	for(i=0;i<8;i++)
	{
		SensorLists[0].SubMenus[i]=&RemoteConfig;
		SensorLists[0].Parent=&DefenseList;	
	}
	RemoteConfig.SubMenus=malloc(sizeof(&RemoteConfig)*3);
	RemoteConfig.SubMenus[0]=&RemoteCode;
	RemoteConfig.SubMenus[1]=&SoundConfig;
	RemoteConfig.SubMenus[2]=&DeleteRemote;
	RemoteConfig.Parent=&SensorLists[0];
	
	RemoteCode.SubMenus=malloc(sizeof(&RemoteCode));
	RemoteCode.SubMenus[0]=null;
	RemoteCode.Parent=&RemoteConfig;
	
	SoundConfig.SubMenus=malloc(sizeof(&SoundConfig));
	SoundConfig.SubMenus[0]=null;
	SoundConfig.Parent=&RemoteConfig;
	
	DeleteRemote.SubMenus=malloc(sizeof(&DeleteRemote)*3);
	DeleteRemote.SubMenus[0]=&DeleteRemoteList[0];
	DeleteRemote.SubMenus[1]=&DeleteRemoteList[0];
	DeleteRemote.SubMenus[2]=&RemoteConfig;
	DeleteRemote.Parent=&RemoteConfig;
	
	DeleteRemoteList[0].SubMenus=malloc(sizeof(DeleteRemoteList[0])*8);
	for(i=0;i<8;i++)
  {
		DeleteRemoteList[0].SubMenus[i]=&RemoteConfig;
  }    
	DeleteRemoteList[0].Parent=&DefenseList;
}
/***********************************************************************

***********************************************************************/
static void MenuVoice(void)
{
	VoiceList.SubMenus=malloc(sizeof(&VoiceList));
	VoiceList.SubMenus[0]=&VoiceList;
	VoiceList.Parent=&IconMenu;  
//	VoiceList.SubMenus=malloc(sizeof(&VoiceList)*2);
//	VoiceList.SubMenus[0]=&Record;
//	Record.SubMenus=malloc(sizeof(&Record));
//	Record.SubMenus[0]=NULL;
//	Record.Parent=&VoiceList;
//	VoiceList.SubMenus[1]=&PlayRecording;
//  PlayRecording.SubMenus=malloc(sizeof(&PlayRecording));
//  PlayRecording.SubMenus[0]=NULL;
//  PlayRecording.Parent=&VoiceList;
//	VoiceList.Parent=&IconMenu;
}
/***********************************************************************

***********************************************************************/
static void MenuSecurity(void)
{
	SecurityList.SubMenus= malloc(sizeof(&SecurityList)*3);
	SecurityList.SubMenus[0]=&Securinty[0];
	SecurityList.SubMenus[1]=&Securinty[1];
	//SecurityList.SubMenus[2]=&Securinty[2];
	
	Securinty[0].SubMenus=malloc(sizeof(Securinty[0]));
	Securinty[0].SubMenus[0]=null;
	Securinty[0].Parent=&SecurityList;
	
	Securinty[1].SubMenus=malloc(sizeof(Securinty[1])*4);
	Securinty[1].SubMenus[0]=&SureMessage;
	Securinty[1].SubMenus[1]=&SureMessage;
	Securinty[1].SubMenus[2]=&SureMessage;
	Securinty[1].SubMenus[3]=&SureMessage;
	Securinty[1].Parent=&SecurityList;

	SureMessage.SubMenus=malloc(sizeof(SureMessage)*2);
	SureMessage.SubMenus[0]=&Defaults;
	SureMessage.SubMenus[1]=&Securinty[1];
	SureMessage.Parent=&Securinty[1];
	
 	Defaults.SubMenus=malloc(sizeof(Defaults));
 	Defaults.SubMenus[0]=null;
	
	SecurityList.Parent=&IconMenu;	
}
/***********************************************************************

***********************************************************************/
static void MenuLog(void)
{
  u8 i;
	LogList.SubMenus = malloc(sizeof(&LogList)*3);
	LogList.SubMenus[0]=&LogDetailsList;
	LogList.SubMenus[1]=&LogDetailsList;
	LogList.SubMenus[2]=&LogDetailsList;
	LogList.Parent=&IconMenu;	
    
  LogDetailsList.SubMenus=malloc(sizeof(&LogDetailsList)*30);
  for(i=0;i<30;i++)
  {
    LogDetailsList.SubMenus[i]=null;
  }
  LogDetailsList.Parent=&LogList;
}
/***********************************************************************

***********************************************************************/
static void MenuAbout(void)
{
  AboutList.SubMenus=malloc(sizeof(&AboutList));
	AboutList.SubMenus[0]=&AboutList;
	AboutList.Parent=&IconMenu;
}
/****************************************************************************************************
����   :    �˵������ʼ��

*****************************************************************************************************/
void MenuListInit(void)
{
  MainMenu.SubMenus = malloc(sizeof(&MainMenu)*1);

	MainMenu.SubMenus[0]=&IconMenu;//��һ���ڵ�
	MainMenu.Parent=NULL;//������
	IconMenu.SubMenus = malloc(sizeof(&IconMenu)*8);
  IconMenu.SubMenus[0] =&PhoneMenu;
	IconMenu.SubMenus[1] =&AboutList;
	IconMenu.SubMenus[2] =&LogList;
	IconMenu.SubMenus[3] =&SecurityList;
	IconMenu.SubMenus[4] =&SetupMenu;
	IconMenu.SubMenus[5] =&VoiceList;
	IconMenu.SubMenus[6] =&DefenseList;
	IconMenu.SubMenus[7] =&CallList;
  
//  IconMenu.SubMenus[0] =&PhoneMenu;
//	IconMenu.SubMenus[1] =&CallList;
//  IconMenu.SubMenus[2] =&DefenseList;
//  IconMenu.SubMenus[3] =&VoiceList;
//  IconMenu.SubMenus[4] =&SetupMenu;
//  IconMenu.SubMenus[5] =&SecurityList;
//  IconMenu.SubMenus[6] =&LogList;
//  IconMenu.SubMenus[7] =&AboutList;
	IconMenu.Parent=&MainMenu;
	
	MenuSetup();
	
	MenuPhone();	

	MenuSecurity();
	
	MenuZone();
	
	MenuLog();
	
	MenuVoice();
	
	MenuCall();
  
  MenuAbout();
	
	CurrentMenu =& MainMenu;
//	MainMenu.SubMenus = malloc(sizeof(&MainMenu)*1);

//	MainMenu.SubMenus[0]=&IconMenu;//��һ���ڵ�
//	MainMenu.Parent=NULL;//������
//	
//	IconMenu.SubMenus = malloc(sizeof(&IconMenu)*8);
//	IconMenu.SubMenus[0] =&PhoneMenu;
//	IconMenu.SubMenus[1] =&AboutList;
//	IconMenu.SubMenus[2] =&LogList;
//	IconMenu.SubMenus[3] =&SecurityList;
//	IconMenu.SubMenus[4] =&SetupMenu;
//	IconMenu.SubMenus[5] =&VoiceList;
//	IconMenu.SubMenus[6] =&DefenseList;
//	IconMenu.SubMenus[7] =&CallList;
//	IconMenu.Parent=&MainMenu;
//	
//	MenuSetup();
//	
//	MenuPhone();	

//	MenuSecurity();
//	
//	MenuZone();
//	
//	MenuLog();
//	
//	MenuVoice();
//	
//	MenuCall();
//  
//  MenuAbout();
//	
//	CurrentMenu =& MainMenu;
}


