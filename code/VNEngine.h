

#define CHAPTERS 25
#define MAX_TEXT 1000
#define MAX_BGS 5
#define MAX_CHOICE 4
#define MAX_LOOPS 2
#define MAX_CHARACTERS 10

#define CDIR "CHS\\"
#define EDIR "EPS\\"
#define BDIR "BGS\\"
#define WDIR "WAVS\\"
#define IMG_FORMAT ".jpg"

#include "DxManager.h" // The DirectX C++ Manager header
#include "Sprites.h" // The DirectX Sprite C++ Manager header
#include "Font.h" // Handles the font
#include "Choice.h" // Defines what a choice is
#include "Characters.h" // Handles characters


std::string append(LPCSTR a, LPCSTR b)
{
	std::string tmp = a;
	tmp+=b;
	return tmp;
}

class VNEngine 
{
public:
	dxManager* dxMgr; // directX Manager
	LPCSTR tex; // Texture name
	dxBackground* bg[MAX_BGS]; // USUALLY THE LAST 2 ARE PRESERVED FOR WHITE AND THEN BLACK
	Character* characters[MAX_CHARACTERS]; // Characters
	short gameState; // Game state
	int curLevel,curText,i; // Current level, text and index
	std::string gameText[MAX_TEXT]; // Game text
	dxFont *gameFont, *introFont; // Game and title fonts
	int curBg, curCh; // Current background and current choice
	int selCh; // Selected choice
	Choice carr[MAX_CHOICE]; // Choice array
	int numChoices,curChoice; // Number of choices and Current choice
	int menu; // Menu index holder
	std::map<std::string, int> choices;

	bool initDX(){ // Initialize
		dxMgr = new dxManager();
		if(!dxMgr->init())
			return false;

		setSprites();
		//	updateMusic("bg.wav"); // Load menu/basic music
		return true;
	}

	void setUpFonts(std::string language)
	{
		gameFont = new dxFont(language);
		introFont = new dxFont(language);
		D3DXCreateFont( dxMgr->dxDevice, 30, 10, FW_BOLD, 0, true, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, langs[language].gameFont.c_str(), &gameFont->m_font);
		D3DXCreateFont( dxMgr->dxDevice, 80, 20, FW_BOLD, 0, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,langs[language].introFont.c_str(), &introFont->m_font );
	}

	void initLanguages()
	{
		Language heb = {"Arial","Blackadder ITC",DT_LEFT|DT_RTLREADING};
		Language eng = {"Bradley Hand ITC","Blackadder ITC",0};
		langs["heb"]= heb;
		langs["eng"] = eng; 
	}

	VNEngine(std::string language)
	{
		if(!initDX())
			MessageBox(0,"ERROR","Couldn't initialize directx!",0);
			exit();
		initLanguages();
		setUpFonts(language);
		gameState = 0;
		curLevel = 0, curText = 0, i = 0;
		curBg=0, curCh = 1, selCh = 0;
		numChoices = 0, curChoice = 0, menu = 0;
	}

	void setChoice()
	{
		gameState = 1;
		if(carr[curChoice].next > -1)
		{
			curLevel = carr[curChoice].next;
			getGameText();
		}

	}

	void updateMusic(LPCSTR play){ // Play (bg) music
			PlaySound(NULL, NULL, 0); 
			PlaySound(append(WDIR,play).c_str(),NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	}

	void play(LPCSTR play) // Play a sound (effect)
	{
		PlaySound(append(WDIR,play).c_str(),NULL,SND_FILENAME | SND_ASYNC);
	}

	void resetBgFade()
	{
		bg[MAX_BGS-2]->alpha=0;
		bg[MAX_BGS-1]->alpha=0;
		bg[MAX_BGS-2]->setFadeFactor(0);
		bg[MAX_BGS-1]->setFadeFactor(0);
	}

	void fade(float* a,float factor) // Fade in effect
	{
		if(factor>0)
			*a = (*a+factor>255)?*a:*a+factor;
		else
			*a = (*a+factor<0)?*a:*a+factor;
	}

	int getCharacterIndex(LPCSTR Name) // get character index
	{
		if(!strcmp(Name, "0"))
		{
			return 0;
		}

		for(int i=1; i<curCh; i++)
		{
			if(characters[i]->name == getName(Name))
			{
				return i;
			}
		}

		return -1; // not found
	}

	void updateSprites(){ // Update
		//FONTS
		gameFont->update();
		introFont->update();
		//BLAC & WHITE BGS
		bg[MAX_BGS-1]->update();
		bg[MAX_BGS-2]->update();

		//ALL BGS
		int g = 0;
		for(g=0; g<curBg; g++)
		{
			bg[g]->update();
		}
		// For characters, animation and all those changes in visuals
		for(g=1; g<curCh; g++)
		{
			characters[g]->Update();
		}
	}

	void setUpBg(dxBackground** bg,LPCSTR gameBg) // Background setter
	{
		(*bg) = new dxBackground(dxMgr->dxDevice,append(BDIR,gameBg).c_str());
		curBg++;
	}

	std::string getName(std::string gametext) // Get name of character
	{
		return gametext.substr(0,gametext.length()-1).c_str();
	}

	void setUpCharacter(Character** c,LPCSTR N, Mood M) // 
	{
		(*c) = new Character(getName(N).c_str(),M,dxMgr->dxDevice);
		(*c)->m_sTexture->loadTex(append(CDIR,append(getName(N).c_str(),IMG_FORMAT).c_str()).c_str(),dxMgr->dxDevice);
		curCh++;
	}

	void setSprites(){ // Main basic sprites (background, gui, etc... )
		setUpBg(&bg[MAX_BGS-1], "black.png"); // add bg
		setUpBg(&bg[MAX_BGS-2],"white.png"); // add bg
		bg[MAX_BGS-2]->alpha=0; // RESET
		bg[MAX_BGS-1]->alpha=0; // RESET
		curBg=0; // RESET
		setUpBg(&bg[curBg],"bg.bmp");
		// Load sprites
	}

	void renderSprites(){ // Render sprites
		int g = 0;
		//BGS
		for(g=0; g<=curBg; g++)
		{
			bg[g]->Render(D3DXSPRITE_ALPHABLEND);
		}
		//CHARACTERS
		if(selCh!=0)
		{
			characters[selCh]->Render(D3DXSPRITE_ALPHABLEND);
		}
		//BLACK AND WHITE BGS
		bg[MAX_BGS-1]->Render(D3DXSPRITE_ALPHABLEND); // BLACK
		bg[MAX_BGS-2]->Render(D3DXSPRITE_ALPHABLEND); // WHITE
	}

	
	void sgText(){ // Render relevent text

		if(curText>i-1){
			curLevel++;
			getGameText();
		}

		if(strcmp(gameText[curText].c_str(),"!")!=0)
		{
			if(gameState == 1)
				gameFont->typeText(gameText[curText].c_str(),0,0,0,50,screen_height-150);
			else
				gameFont->showText(gameText[curText].c_str(),0,0,0,50,screen_height-150);
		}
	}

	void getGameText(){ // Get chunk of text
		std::ofstream save("save.vns");
		if(save.is_open())
			save<<curLevel;
		save.close();
		curText = 0;
		i=0;
		char fname[20];
		sprintf(fname,append(EDIR,"%d.vnp").c_str(),curLevel); 
		std::ifstream file(fname);
		if(file.is_open()){
			i = 0;
			while(file.good()){
				getline(file,gameText[i]);
				i++;
				if(i>25)
					break;
			}
		}
		else{
			introFont->alpha = 255;
			gameFont->alpha = 180;
			gameState = 2; // Credits
			//updateMusic("WAVS\\bg.wav");
		}

		file.close();
	}

	void setNew() // RESET
	{
		gameState = 1;
		getGameText();
	}

	void saveProfile() // SAVE
	{

	}

	void loadProfile() // LOAD
	{
		std::string s;
		std::ifstream file("save.vns");
		if(file.is_open()){
			getline(file,s);
		}
		file.close();

		curLevel = atoi(s.c_str());
		gameState = 1;
		getGameText();
	}

	void choiceCursor(int* holder,int count,int operation) // operation: -1 for down, 1 for up; holder: the index holder; count - # of choices;
	{
		*holder = (abs(*holder+operation))%count;
	}

	void readData(int id) // Read data from novel text
	{
		int j=0, max=10;
		numChoices = 0;
		curChoice=0;

		while(strcmp(gameText[curText].c_str(),"!")!=0)
		{
			curText++;
			j++;
			if(j<max)
			{
				if(id==0)
				{
					resetBgFade();
					max = 1;
				}
				if(id==1) // background
				{
					setUpBg(&bg[curBg], gameText[curText].c_str());
					resetBgFade();
					max = MAX_BGS-2;
				}
				else if(id==2) // music
				{
					updateMusic(gameText[curText].c_str());
					max = MAX_LOOPS;
				}
				else if(id==3) // sound
				{
					play(gameText[curText].c_str());
					max = 10;
				}
				else if(id==4) // Use fade (only one!)
				{
					if(atoi(gameText[curText].c_str())==1) // ON TEXT
						gameFont->setFadeFactor(atof(gameText[curText+1].c_str()));
					else // ON BG - 1 FOR BLACK - 2 FOR WHITE
						bg[MAX_BGS-atoi(gameText[curText+1].c_str())]->setFadeFactor(atof(gameText[curText+2].c_str())); // 2nd parameter - the factor
					max = 1;
				}
				else if(id==5) // Change plot
				{
					max = MAX_CHOICE/2+1;
					carr[j-1].str = gameText[curText].c_str();
					curText++;
					carr[j-1].next = atoi(gameText[curText].c_str());
					numChoices++;
					if(strcmp(gameText[curText+1].c_str(),"!")==0)
						max=0;
				}
			}
		}
		next();
	}

	void next()
	{
		cChar=0;
		curText++;
		if(strcmp(gameText[curText].c_str(),"!")==0){ // If it's a special command
			curText++;
			if(atoi(gameText[curText].c_str())==5)
				gameState=3; // Choice making
			readData(atoi(gameText[curText].c_str()));
			// More cases here
		}
		else // else:
		{
			selCh = getCharacterIndex(gameText[curText].c_str());
			if(selCh!=0) // Set Mood and Character
			{
				if(selCh==-1)
				{
					//MessageBox(0,0,0,0);
					selCh = curCh;
					curText++;
					setUpCharacter(&characters[selCh],gameText[curText-1].c_str(),(Mood)0);
					characters[selCh]->setMood((Mood)atoi(gameText[curText].c_str()));
				}
					curText++;
			}
			else
			{
				curText++;
			}
		}

	}

	void exit() // EXIT
	{
		gameState = 0;
	}
};