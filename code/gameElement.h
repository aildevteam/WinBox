


class gameElement
{
public:
	bool alive;
	float alpha,factor;

	gameElement()
	{
		alpha = 255;
		factor = 0;
		alive = true;
	}

	void setFadeFactor(float fac)
	{
		factor=fac;
	}

	void fade() // Fade in effect
	{
		if(factor!=0)
		{
			if(factor>0)
				alpha = (alpha+factor>255)?alpha:alpha+factor;
			else
				alpha = (alpha+factor<0)?alpha:alpha+factor;
		}
	}

	void update()
	{
		fade();
	}

};