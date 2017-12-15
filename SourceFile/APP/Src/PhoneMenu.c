
#include "PhoneMenu.h"
#include "APP.h"
#include "TaskMenu.h"

extern const unsigned char gImage_member[1][808];

const string PhoneMemberTab[4][6]=
{
	{"Contacts","No number","Number","Delete","Cancel"},
	{
    "Phone Numbers",
    "Setting","Delete",
    "  Delete current number",
    "        Delete All",
    "          Cancel"
  },
	{"Call","SMS","CID"},
	{"Please input numbers:"}
};


const string PhoneBmpPath[8]=
{
    "0:/ICON/phone/member.bmp",
    "0:/ICON/phone/member.bmp",
    "0:/ICON/phone/member.bmp",
    "0:/ICON/phone/member.bmp",
    "0:/ICON/phone/member.bmp",
    "0:/ICON/phone/member.bmp",
    "0:/ICON/phone/member.bmp",
    "0:/ICON/phone/member.bmp",

};
const u32 PhoneAddr[8]=
{
	PhoneMember1Addr,	
	PhoneMember2Addr,
	PhoneMember3Addr,
	PhoneMember4Addr,
	PhoneMember5Addr,
	PhoneMember6Addr,
	PhoneMember7Addr,
	PhoneMember8Addr,
};
u32 PhonesetupAddr[2]={9,9};
const u32 PhoneNameAddr[8]=
{
	PhoneName1Addr,
	PhoneName2Addr,
	PhoneName3Addr,
	PhoneName4Addr,
	PhoneName5Addr,
	PhoneName6Addr,
	PhoneName7Addr,
	PhoneName8Addr,
};
const u32 CheckBoxState[]=
{
		Phone1TypeAddr
};
const string SystemString[1]=
{
	"Confirm deletion",
};
const u32 MemberIconAddr[8]=
{
	Phonemember,
	Phonemember,
	Phonemember,
	Phonemember,
	Phonemember,
	Phonemember,
	Phonemember,
	Phonemember,
};


/***************************************************************************************************

***************************************************************************************************/
// void PhoneMember(void)
// {
// 	ListboxList *listx;
// 	ListboxList *listTemp;
// 	u8 number[8][20];
// 	u8 NumberTab[8][20];
// 	u8 i,length;
// 	u8 numberCount=0;
// 	u32 numberAddr[8];
// 	
// 	for(i=0;i<8;i++)
// 	{
// 		System.Device.Storage.Parameter.StorageReadData(PhoneAddr[i],number[i],20);
// 		if((number[i][0]>='0')&&(number[i][0]<='9'))
// 		{
// 			length=strlen((const char *)number[i]);
// 			if(length>3)
// 			{
// 				sprintf((char*)NumberTab[numberCount], "%.*s",length,number[i]);
// 				numberAddr[numberCount]=PhoneAddr[i];
// 				numberCount++;
// 			}
// 		}
// 	}
// 	if(numberCount)
// 	{
// 		i=0;
// 		listx=App.Menu.FocusFormPointer->ListPointer->List;//��ȡ��ǰ�б�
// 		listx=ListSearch(listx,0);//���ҵ�ǰ��Ҫ������
// 		while(listx!=0)
// 		{
// 			listx->Name=(string)NumberTab[i];	//���ĵ�ǰ�б�
// 			listx->FlashAddr=&numberAddr[i];
// 			
// 			i++;				
// 			listx=listx->Nextlist;
// 		}		
// 		listTemp=App.Menu.FocusFormPointer->ListPointer->List;//��ȡ��ǰ�б�
// 		while(i<numberCount)
// 		{	
// 			listx=malloc(sizeof(ListboxList));
// 			listx->Name=(string)NumberTab[i];
// 			listx->IconAdd=&MemberIconAddr[0];
// 			listx->FlashAddr=&numberAddr[i];
// 			listx->Nextlist=NULL;
// 			
// 			while(listTemp->Nextlist!=NULL)listTemp=listTemp->Nextlist;//ƫ�Ƶ����һ��list	 
// 			listx->Prevlist=listTemp;
// 			listTemp->Nextlist=listx;	
// 			i++;			
// 		}
// 		if(numberCount>5)
// 			App.Menu.FocusFormPointer->ListPointer->ItemsPerpage=5;//ҳ�����Ŀ
// 		else
// 			App.Menu.FocusFormPointer->ListPointer->ItemsPerpage=numberCount;//ҳ�����Ŀ
// 		App.Menu.FocusFormPointer->ListPointer->TotalItems=i;
// 	}
// 	else
// 	{
// 		listx=App.Menu.FocusFormPointer->ListPointer->List;//��ȡ��ǰ�б�
// 		listx=ListSearch(listx,0);//���ҵ�ǰ��Ҫ������
// 		while(listx!=0)
// 		{
// 			if(listx!=0)
// 			{	
// 				listx->Name=PhoneMemberTab[0][1];	//���ĵ�ǰ�б�		
// 				App.Menu.FocusFormPointer->ListPointer->Enable=0xFF;				
// 			}
// 			listx=listx->Nextlist;
// 		}	
// 		
// 	}	
// }

/*****************************************************************************
*
*����       ���绰��Ա�б�
*�������   ����
*�������   ����
*
*****************************************************************************/
static void ContactsListInit(void)
{
	static FillArea 	RemoteFill;
	//static FillArea		bottomFill;
	static Label 		phoneName;
	static ListBox		phoneList;	

	//items=(string *)RemteName;
	System.Gui.Form.Init(&App.Menu.PhoneListForm);
	App.Menu.PhoneListForm.FillAreaCount=1;
	App.Menu.PhoneListForm.LabelCount=1;
	App.Menu.PhoneListForm.ListCount=1;
	App.Menu.PhoneListForm.FormID=Form14;
	App.Menu.PhoneListForm.Color=GUI_WHITE;
	App.Menu.PhoneListForm.RectWrap.x0=0;
	App.Menu.PhoneListForm.RectWrap.y0=30;
	App.Menu.PhoneListForm.RectWrap.x1=320;
	App.Menu.PhoneListForm.RectWrap.y1=240;

	System.Gui.Form.AddFillArea(&App.Menu.PhoneListForm,&RemoteFill);
	RemoteFill.ID=PromptAreaID5;
	RemoteFill.PaintColor=TopLabelColor;//GUI_ORANGE;
	RemoteFill.RectWrap.x0=0;
	RemoteFill.RectWrap.y0=30;
	RemoteFill.RectWrap.x1=320;
	RemoteFill.RectWrap.y1=80;
	
//	System.Gui.Form.AddFillArea(&App.Menu.PhoneListForm,&bottomFill);
//	bottomFill.ID=PromptAreaID6;
//	bottomFill.PaintColor=GUI_ORANGE;
//	bottomFill.RectWrap.x0=0;
//	bottomFill.RectWrap.y0=30;
//	bottomFill.RectWrap.x1=320;
//	bottomFill.RectWrap.y1=60;
	
	System.Gui.Form.AddLabel(&App.Menu.PhoneListForm,&phoneName);
	phoneName.ID=LabelID1;
	phoneName.Font=ListNameFont;
	phoneName.PaintColor=GUI_BLACK;
	phoneName.TextMode=GUI_TEXTMODE_TRANS;
	phoneName.Count=0;
	phoneName.Type=0x00;
	phoneName.StringBlockPointer=PhoneMemberTab[0];
	phoneName.X=115;
	phoneName.Y=43;	

	System.Gui.Form.AddListBox(&App.Menu.PhoneListForm,&phoneList);
	phoneList.x0=0;
	phoneList.y0=80;
	phoneList.x1=320;
	phoneList.y1=240;
	phoneList.xSize=15;
	phoneList.ySize=15;
	
	phoneList.Select=3;
	phoneList.ItemsHeight=40;
	phoneList.selectedItem=0;
	phoneList.ItemsPerpage=4;//ҳ�����Ŀ
	phoneList.SelectedIndexes=0;//Ĭ��Ϊ0��Ҳ���ǵ�һ��
	phoneList.TopItem=0;//Ĭ��Ϊ0��Ҳ���Ǵӵ�һ�������ſ�ʼ
	phoneList.TotalItems=0;
	
	phoneList.TextMode=GUI_TEXTMODE_TRANS;
	phoneList.BackColor=GUI_WHITE;
	phoneList.PaintColor=GUI_BLACK;
	
	phoneList.SelectedBackColor=MoveBarColor;
	phoneList.SelectedPaintColor=GUI_WHITE;
	
	phoneList.Font=ListFont;
	phoneList.ID=0x01;
	phoneList.Type=IconList;

	System.Gui.Form.AddLists(&phoneList,null,8,null,(u32*)PhoneNameAddr,(u32*)(PhoneAddr),(u8**)gImage_member,1);	
}
/*****************************************************************************
*
*����       ���绰���ԡ�ɾ������� ��
*�������   ����
*�������   ����
*
*****************************************************************************/
static  void ContactsEdit(void)
{
	static Label 		nameBale;
	static FillArea		nameFill;
	static ListBox		editList;
	string *items=(string*)PhoneMemberTab[1];
	
	System.Gui.Form.Init(&App.Menu.PhoneEditForm);
	App.Menu.PhoneEditForm.FillAreaCount=1;
	App.Menu.PhoneEditForm.LabelCount=1;
	App.Menu.PhoneEditForm.ListCount=1;
	App.Menu.PhoneEditForm.FormID=Form5;
	App.Menu.PhoneEditForm.Color=GUI_WHITE;
	App.Menu.PhoneEditForm.RectWrap.x0=0;
	App.Menu.PhoneEditForm.RectWrap.y0=30;
	App.Menu.PhoneEditForm.RectWrap.x1=320;
	App.Menu.PhoneEditForm.RectWrap.y1=240;		
	
	System.Gui.Form.AddFillArea(&App.Menu.PhoneEditForm,&nameFill);
	nameFill.ID=PromptAreaID1;
	nameFill.PaintColor=TopLabelColor;
	nameFill.RectWrap.x0=0;
	nameFill.RectWrap.y0=30;
	nameFill.RectWrap.x1=320;
	nameFill.RectWrap.y1=80;	

	System.Gui.Form.AddLabel(&App.Menu.PhoneEditForm,&nameBale);
	nameBale.ID=LabelID1;
	nameBale.Font=ListNameFont;
	nameBale.PaintColor=GUI_BLACK;
	nameBale.TextMode=GUI_TEXTMODE_TRANS;
//  nameBale.TextAlign=GUI_TA_HCENTER;
	nameBale.Count=0;
	nameBale.Type=0xFC;
	nameBale.StringBlockPointer=NULL;
	nameBale.X=120;
	nameBale.Y=43;	
	
	System.Gui.Form.AddListBox(&App.Menu.PhoneEditForm,&editList);
	editList.x0=0;
	editList.y0=80;
	editList.x1=320;
	editList.y1=240;
	editList.xSize=15;
	editList.ySize=15;
	
	editList.Select=3;
	editList.ItemsHeight=40;
	editList.selectedItem=0;
	editList.ItemsPerpage=3;//ҳ�����Ŀ
	editList.SelectedIndexes=0;//Ĭ��Ϊ0��Ҳ���ǵ�һ��
	editList.TopItem=0;//Ĭ��Ϊ0��Ҳ���Ǵӵ�һ�������ſ�ʼ
	editList.TotalItems=0;
	
	editList.TextMode=GUI_TEXTMODE_TRANS;
	editList.BackColor=GUI_WHITE;
	editList.PaintColor=GUI_BLACK;
	
	
	editList.SelectedBackColor=MoveBarColor;
	editList.SelectedPaintColor=GUI_WHITE;
	
	editList.Font=ListFont;
	editList.ID=0x01;
	editList.Type=TextList;

	System.Gui.Form.AddLists(&editList,items,3,null,null,null,null,3);			
}
/*****************************************************************************
*
*����       ���绰�༭��
*�������   ����
*�������   ����
*
*****************************************************************************/
static void AddContacts(void)
{
	static Label 	nameLabel;
	static Label    noticeLabel;
	static TextBox	numberPhoneText;
	static FillArea nameFill;
	
	System.Gui.Form.Init(&App.Menu.AddPhoneForm);
	App.Menu.AddPhoneForm.FillAreaCount=1;
	App.Menu.AddPhoneForm.LabelCount=2;
	App.Menu.AddPhoneForm.ListCount=0;
	App.Menu.AddPhoneForm.TextBoxCount=1;
	App.Menu.AddPhoneForm.FormID=Form1+30;
	App.Menu.AddPhoneForm.Color=GUI_WHITE;
	App.Menu.AddPhoneForm.RectWrap.x0=0;
	App.Menu.AddPhoneForm.RectWrap.y0=30;
	App.Menu.AddPhoneForm.RectWrap.x1=320;
	App.Menu.AddPhoneForm.RectWrap.y1=210;	
	App.Menu.AddPhoneForm.DefaultFocusTextBoxPointer=&numberPhoneText;
	App.Menu.AddPhoneForm.FocusTextBoxPointer=&numberPhoneText;//��ǰҳ�潹���ArmTextBox��ʼ
	
	System.Gui.Form.AddFillArea(&App.Menu.AddPhoneForm,&nameFill);
	nameFill.ID=PromptAreaID1;
	nameFill.PaintColor=TopLabelColor;
	nameFill.RectWrap.x0=0;
	nameFill.RectWrap.y0=30;
	nameFill.RectWrap.x1=320;
	nameFill.RectWrap.y1=80;

	System.Gui.Form.AddLabel(&App.Menu.AddPhoneForm,&noticeLabel);
	noticeLabel.ID=LabelID1+2;
	noticeLabel.Font=ListNameFont;
	noticeLabel.PaintColor=GUI_BROWN;
	noticeLabel.TextMode=GUI_TEXTMODE_TRANS;
	noticeLabel.Count=0;
	noticeLabel.StringBlockPointer=PhoneMemberTab[3];
	noticeLabel.X=20;
	noticeLabel.Y=100;	

	System.Gui.Form.AddLabel(&App.Menu.AddPhoneForm,&nameLabel);
	nameLabel.ID=LabelID1;
	nameLabel.Font=ListNameFont;
	nameLabel.PaintColor=GUI_BLACK;
	nameLabel.TextMode=GUI_TEXTMODE_TRANS;
	nameLabel.Count=0;
	nameLabel.StringBlockPointer=PhoneMemberTab[1];
	nameLabel.X=115;
	nameLabel.Y=43;	
	
	
	System.Gui.Form.AddTextBox(&App.Menu.AddPhoneForm,&numberPhoneText);
	numberPhoneText.x=21;
	numberPhoneText.y=140;
	numberPhoneText.MaxDataPointer=25;
	numberPhoneText.MinDataPointer=0;
	numberPhoneText.Type=GuiDataTypeString;
	numberPhoneText.RimType=0x01;
	numberPhoneText.BackColor=GUI_WHITE;
	numberPhoneText.PaintColor=GUI_RED;
	numberPhoneText.Font=ListFont;
	numberPhoneText.ID=0x03;
	numberPhoneText.TextMode=GUI_TEXTMODE_NORMAL;	
	numberPhoneText.RectWrap.x0=21;
	numberPhoneText.RectWrap.x1=298;
	numberPhoneText.RectWrap.y0=140;
	numberPhoneText.RectWrap.y1=160;
	numberPhoneText.DataAddr=0;
	numberPhoneText.DataPointer=&App.Data.KeyBuffer2;
	
	
	
}

/*********************************************************************************
*
*����		���绰���빦���趨��ѡ����Ż����ǵ绰���������߶��߱���
*�������   ����
*�������   ����
*
*********************************************************************************/
static void ContactsSetupList(void)
{
	static Label 	nameLabel;
	static FillArea nameFill;
	static ListBox  typeList;
	static string   *items;
	items=(string*)PhoneMemberTab[2];
	
	System.Gui.Form.Init(&App.Menu.PhoneSetForm);
	App.Menu.PhoneSetForm.FillAreaCount=1;
	App.Menu.PhoneSetForm.LabelCount=1;
	App.Menu.PhoneSetForm.ListCount=1;
	App.Menu.PhoneSetForm.FormID=Form3;
	App.Menu.PhoneSetForm.Color=GUI_WHITE;
	App.Menu.PhoneSetForm.RectWrap.x0=0;
	App.Menu.PhoneSetForm.RectWrap.y0=30;
	App.Menu.PhoneSetForm.RectWrap.x1=320;
	App.Menu.PhoneSetForm.RectWrap.y1=210;	
	
	System.Gui.Form.AddFillArea(&App.Menu.PhoneSetForm,&nameFill);
	nameFill.ID=PromptAreaID1;
	nameFill.PaintColor=TopLabelColor;
	nameFill.RectWrap.x0=0;
	nameFill.RectWrap.y0=30;
	nameFill.RectWrap.x1=320;
	nameFill.RectWrap.y1=80;

	System.Gui.Form.AddLabel(&App.Menu.PhoneSetForm,&nameLabel);
	nameLabel.ID=LabelID1;
	nameLabel.Font=ListNameFont;
	nameLabel.PaintColor=GUI_BLACK;
	nameLabel.TextMode=GUI_TEXTMODE_TRANS;
	nameLabel.Count=1;
	nameLabel.StringBlockPointer=PhoneMemberTab[1];
	nameLabel.X=120;
	nameLabel.Y=43;	
	
	
	System.Gui.Form.AddListBox(&App.Menu.PhoneSetForm,&typeList);
	typeList.x0=0;
	typeList.y0=80;
	typeList.x1=320;
	typeList.y1=210;
	typeList.xSize=15;
	typeList.ySize=15;
	
	typeList.selectedItem=0;
	typeList.ItemsHeight=40;
	typeList.ItemsPerpage=3;//ҳ�����Ŀ
	typeList.SelectedIndexes=0;//Ĭ��Ϊ0��Ҳ���ǵ�һ��
	typeList.TopItem=0;//Ĭ��Ϊ0��Ҳ���Ǵӵ�һ�������ſ�ʼ
	typeList.TotalItems=0;
	
	typeList.TextMode=GUI_TEXTMODE_TRANS;
	typeList.BackColor=GUI_WHITE;
	typeList.PaintColor=GUI_BLACK;
	
	typeList.SelectedBackColor=MoveBarColor;
	typeList.SelectedPaintColor=GUI_WHITE;
	
	typeList.Font=ListFont;
	typeList.ID=0x56;
	typeList.Type=CheckList;

	System.Gui.Form.AddLists(&typeList,items,3,(u32*)PhonesetupAddr,null,null,null,3);
}

/**********************************************************************************************************************
*
*����       ����ȡ�绰���빦�ܣ����Ż��ߵ绰��
*�������   ��formPointer,��ǰ�Ľ���ҳ��ָ��
*�������   ����
*
**********************************************************************************************************************/
void PhoneTypeAddr(Form *formPointer)
{
	MenuListStruct 	*currentMenu;
	Form 			*focusFormPointer;
	ListboxList		*listx,*modifyList;	
	Label			*labelPointer;
	u32 			addr;
	
	if(formPointer==&App.Menu.PhoneEditForm)
	{
		currentMenu=CurrentMenu->Parent;
		focusFormPointer=currentMenu->MenuItems[CurrentMenu->curSelected];
		listx=ListSearch(focusFormPointer->ListPointer->List,focusFormPointer->ListPointer->SelectedIndexes);
		addr=*listx->FlashAddr;
		labelPointer=formPointer->LabelPointer;
		while(null!=labelPointer)
		{
			if(labelPointer->Type==0xFC)
				labelPointer->Addr=addr;
			labelPointer=labelPointer->NextLabelPointer;		
		}	
	}
	else
	{
		currentMenu=CurrentMenu->Parent;
		currentMenu=currentMenu->Parent;
		focusFormPointer=currentMenu->MenuItems[CurrentMenu->curSelected];
		listx=ListSearch(focusFormPointer->ListPointer->List,focusFormPointer->ListPointer->SelectedIndexes);
		modifyList=ListSearch(formPointer->ListPointer->List,0);
		*modifyList->IconAdd=*listx->FlashAddr+14;
		modifyList=modifyList->Nextlist;
		*modifyList->IconAdd=*listx->FlashAddr+16;
		modifyList=modifyList->Nextlist;
		*modifyList->IconAdd=*listx->FlashAddr+18;
	}
}
/********************************************************************************
*����		��ɾ���绰����ѯ�ʽ���
********************************************************************************/
static void ContactsDelFomInit(void)
{
	//static FillArea	sureFill;
	//static Label	sureName;
	static ListBox	sureList;
	
	string *items;
	items=(string *)PhoneMemberTab[1];
	
	System.Gui.Form.Init(&App.Menu.PhoneDelForm);
	App.Menu.PhoneDelForm.FillAreaCount=0;
	App.Menu.PhoneDelForm.LabelCount=0;
	App.Menu.PhoneDelForm.ListCount=1;
	App.Menu.PhoneDelForm.FormID=Form9+16;
	App.Menu.PhoneDelForm.Color=GUI_ORANGE;
	App.Menu.PhoneDelForm.RectWrap.x0=70;
	App.Menu.PhoneDelForm.RectWrap.y0=80;
	App.Menu.PhoneDelForm.RectWrap.x1=250;
	App.Menu.PhoneDelForm.RectWrap.y1=200;	

//	System.Gui.Form.AddFillArea(&App.Menu.PhoneDelForm,&sureFill);
//	sureFill.ID=PromptAreaID5;
//	sureFill.PaintColor=GREEN;
//	sureFill.RectWrap.x0=70;
//	sureFill.RectWrap.y0=75;
//	sureFill.RectWrap.x1=250;
//	sureFill.RectWrap.y1=110;	

//	System.Gui.Form.AddLabel(&App.Menu.PhoneDelForm,&sureName);
//	sureName.ID=LabelID1+10;
//	sureName.Font=GUI_FONT_20_1;
//	sureName.PaintColor=GUI_BLUE;
//	sureName.TextMode=GUI_TEXTMODE_TRANS;
//	sureName.Count=2;
//	sureName.StringBlockPointer=PhoneMemberTab[1];
//	sureName.X=100;
//	sureName.Y=85;	
//	
	System.Gui.Form.AddListBox(&App.Menu.PhoneDelForm,&sureList);
	sureList.x0=50;
	sureList.y0=110;
	sureList.x1=270;
	sureList.y1=200;
	sureList.xSize=10;//���ı��б�ʱ������������ʾƫ����
	sureList.ySize=15;
	
	sureList.selectedItem=0;
	sureList.ItemsPerpage=3;//ҳ�����Ŀ
	sureList.SelectedIndexes=0;//Ĭ��Ϊ0��Ҳ���ǵ�һ��
	sureList.TopItem=0;//Ĭ��Ϊ0��Ҳ���Ǵӵ�һ�������ſ�ʼ
	sureList.TotalItems=0;
	
	
	sureList.TextMode=GUI_TEXTMODE_TRANS;
	sureList.BackColor=GUI_WHITE;
	sureList.PaintColor=GUI_BLACK;
	
	sureList.SelectedBackColor=MoveBarColor;
	sureList.SelectedPaintColor=GUI_WHITE;
	sureList.RimColor=BLUE;
	
	sureList.Font=ListFont;;
	sureList.ID=0x36;
	sureList.Type=TextList;
	
	System.Gui.Form.AddLists(&sureList,&items[3],3,(u32*)NULL,null,null,null,3);
}
/**********************************************************************************
*
*����       ��ɾ������ҳ�洦��������ѡ���� 1 ɾ����ǰ���� 2 ɾ��ȫ������ 3 ����
*�������   ����
*�������   ����
*
**********************************************************************************/
void DeleteCancel(void)
{
	MenuListStruct 	*currentMenu;
	Form 			*formPointer;
	u8 				indexes;
	u32				nameAddr,IDAddr;
	ListboxList		*listx;
	u8 				buffer[30];
	u8 				name[20];
	
	indexes=App.Menu.FocusFormPointer->ListPointer->SelectedIndexes;
	if(&App.Menu.PhoneDelForm==App.Menu.FocusFormPointer)
	{
		if(1==indexes)//�ڶ���ѡ����Ϊɾ�����к���
		{
			System.SystemApp.DeleteAllNumber();	
			App.Menu.FocusFormPointer->ListPointer->SelectedIndexes=0;
			App.Menu.FocusFormPointer->ListPointer->selectedItem=0;
			App.Menu.FocusFormPointer->ListPointer->TopIndexes=0;
			App.Menu.FocusFormPointer->ListPointer->TopItem=0;	
			currentMenu=CurrentMenu->Parent;
			formPointer=currentMenu->MenuItems[CurrentMenu->curSelected];
			formPointer->ListPointer->SelectedIndexes=0;
			formPointer->ListPointer->selectedItem=0;
			formPointer->ListPointer->TopIndexes=0;
			formPointer->ListPointer->TopItem=0;				
		}
		else if(0==indexes)
		{
			App.Menu.FocusFormPointer->ListPointer->SelectedIndexes=0;
			App.Menu.FocusFormPointer->ListPointer->selectedItem=0;
			App.Menu.FocusFormPointer->ListPointer->TopIndexes=0;
			App.Menu.FocusFormPointer->ListPointer->TopItem=0;	
			currentMenu=CurrentMenu->Parent;
			formPointer=currentMenu->MenuItems[CurrentMenu->curSelected];
			formPointer->ListPointer->SelectedIndexes=0;
			formPointer->ListPointer->selectedItem=0;
			formPointer->ListPointer->TopIndexes=0;
			formPointer->ListPointer->TopItem=0;

			currentMenu=CurrentMenu->Parent;
			formPointer=currentMenu->MenuItems[CurrentMenu->curSelected];
			formPointer->ListPointer->SelectedIndexes=0;
			formPointer->ListPointer->selectedItem=0;
			formPointer->ListPointer->TopIndexes=0;
			formPointer->ListPointer->TopItem=0;

			currentMenu=currentMenu->Parent;
			formPointer=currentMenu->MenuItems[CurrentMenu->curSelected];			
			listx=ListSearch(formPointer->ListPointer->List,formPointer->ListPointer->SelectedIndexes);
			nameAddr=*listx->FlashAddr;
			IDAddr=*listx->NullPointer;
			memset(buffer,0x00,30);
			sprintf((char*)buffer,"NO.%1d (Empty)",formPointer->ListPointer->SelectedIndexes+1);
			buffer[28]=0x01;
			System.Device.Storage.Parameter.StorageWriteData(IDAddr,(u16*)buffer,strlen((char*)buffer));
			System.Device.Storage.Parameter.StorageWriteData(IDAddr+28,(u16*)&buffer[28],1);
			memset(name,0x00,20);
			System.Device.Storage.Parameter.StorageReadData(nameAddr+20,name,20);
			memset(name,0x00,20);
			sprintf((char*)name,"Number%.1d",formPointer->ListPointer->SelectedIndexes+1);
			System.Device.Storage.Parameter.StorageWriteData(nameAddr,(u16*)name,10);
			name[14]=1;
			name[12]=1;
			System.Device.Storage.Parameter.StorageWriteData(nameAddr+12,(u16*)&name[12],1);	
			System.Device.Storage.Parameter.StorageWriteData(nameAddr+14,(u16*)&name[14],1);
		}
		else
		{
			App.Menu.FocusFormPointer->ListPointer->SelectedIndexes=0;
			App.Menu.FocusFormPointer->ListPointer->selectedItem=0;
			App.Menu.FocusFormPointer->ListPointer->TopIndexes=0;
			App.Menu.FocusFormPointer->ListPointer->TopItem=0;	
		}
	}		
}
/**********************************************************************************
*
*����       ��ɾ��ȫ������
*�������   ����
*�������   ����
*
**********************************************************************************/
void DeleteAllNumber(void)
{
	System.SystemApp.DeleteAllNumber();	
}

/**********************************************************************************
*
*����       : ���е绰����ҳ���ʼ��
*�������   ����
*�������   ����
*
**********************************************************************************/
void ContactsTab(void)
{
	ContactsListInit();
	ContactsEdit();
	AddContacts();
	ContactsSetupList();
	ContactsDelFomInit();
}



















