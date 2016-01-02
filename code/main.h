
VNEngine* vne;
LPCSTR Menu[] = {"New","Continue"};

bool initEngine()
{
	vne = new VNEngine("eng");
	return true;
}

void handleMenu()
{
	switch(vne->menu) // CONTINUE
	{
		case 0:
			vne->setNew();
			break;
		case 1:
			vne->loadProfile();
			break;
		case 3:
			vne->exit();
			break;
	}
}

void Scene3() // Make a choice
{
	int k;
	vne->updateSprites(); // Update game visuals
	vne->dxMgr->beginRender(D3DCOLOR_XRGB(255,255,255));
	vne->renderSprites(); // Render game visuals
	// Render choices:
	for(k=0; k<vne->numChoices; k++)
	{
		int c = (k==vne->curChoice)?0:100;
		vne->gameFont->showText(vne->carr[k].str.c_str(),c,c,c,50,screen_height-150+k*30);
	}
	vne->dxMgr->endRender();
}

void Scene2() // Credits
{
	vne->updateSprites();
	vne->introFont->setFadeFactor(-0.7);
	vne->dxMgr->beginRender(D3DCOLOR_XRGB(0,0,0));
	vne->renderSprites();
	vne->introFont->showText("Credits\n",0,0,0,150,200); // Render title
	vne->introFont->setFadeFactor(-1);
	vne->gameFont->showText("Made using fresh/g_o BOX VN Engine.",0,0,0,200,400);
	vne->gameFont->setFadeFactor(-0.3);
	vne->dxMgr->endRender();
}

void Scene1(){ // In-game
	vne->updateSprites(); // Update game visuals
	vne->dxMgr->beginRender(D3DCOLOR_XRGB(255,255,255));
	vne->renderSprites(); // Render game visuals
	vne->sgText(); // Render text
	vne->dxMgr->endRender();
}

void Scene0(){	 // Menu/Home
	vne->updateSprites(); // Update game visuals
	vne->introFont->setFadeFactor(2); // Show vn name
	vne->dxMgr->beginRender(D3DCOLOR_XRGB(255,255,255));
	vne->renderSprites(); // Render game visuals
	vne->introFont->showText(Menu[vne->menu],255,255,220,200,200); // Render title
	vne->dxMgr->endRender();
}

void mScenes(){ // Handle scenes
	switch(vne->gameState){
		case 0:
			Scene0();
			break;
		case 1:
			Scene1();
			break;
		case 2:
			Scene2();
			break;
		case 3:
			Scene3();
			break;
	}

}

void Shutdown(){ // Shutdown function
	vne->exit();
	delete vne;
}