
#include <map>

float tRate = 1.0/2.0; // Seconds/Letter
long tTime = 0;
int cChar = 0;

struct language
{
	std::string gameFont;
	std::string introFont;
	DWORD flags;
};

typedef language Language;

std::map<std::string, Language> langs;

class dxFont: public gameElement
{
public:
	ID3DXFont* m_font;
	Language* lang;

	dxFont(std::string l)
	{
		lang = &langs[l];
	}

	RECT GetTextRect(const char *szText)
	{
		RECT rcRect = {0,0,0,0};
		if (m_font)
		{
			// calculate required rect
			m_font->DrawText(NULL, szText, strlen(szText), &rcRect, DT_CALCRECT,
						D3DCOLOR_XRGB(0, 0, 0));
		}

		// return width
		return rcRect;
	}

	void showText(LPCSTR msg,int r,int g,int b,float x, float y){ // Show the text!
		x=screen_width-x;
		D3DCOLOR color = D3DCOLOR_ARGB((int)alpha,r,g,b);
		RECT rct = GetTextRect(msg);
		if(lang == &langs["heb"])
		{
			rct.left = 0;
			rct.right = x;
		}
		else
		{
			rct.left = x;
			rct.right += x+2;
		}
		rct.top = y;
		rct.bottom += y+2;
		m_font->DrawText(NULL, msg, -1, &rct, lang->flags, color );
	}

	void typeText(LPCSTR msg,int r,int g, int b,float x, float y) // Type the text!
	{
		long newTime = timeGetTime();
		std::string s = msg;

		if(newTime-tTime>tRate && cChar<s.length())
		{
			cChar++;
			tTime = newTime;
		}

		showText(s.substr(0,cChar).c_str(),r,g,b,x,y);
	}
};







