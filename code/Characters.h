
#define STD_SIZE 2048

enum Mood {upset=0, sad=1, happy=2, angry=3, blushing=4};

class Character:public dxMoveSprite
{
public:
	Mood m;
	LPCSTR name;

	void setMood(Mood M)
	{
		m=M;
		anim = m;
	}

	Character(LPCSTR N,Mood M,LPDIRECT3DDEVICE9 pDevice):dxMoveSprite(pDevice)
	{
		setMood(M);
		name = N;
		moveTo(screen_width/2-STD_SIZE/10,screen_height/2-STD_SIZE/10);
		setRect(0,0,STD_SIZE/5,STD_SIZE/5);
	}


};