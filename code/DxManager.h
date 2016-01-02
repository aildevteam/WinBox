// Loading required stuff
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )
#include<d3d9.h>
#include<d3dx9.h>

// The DirectX C++ Manager
class dxManager
{
public:
	int SceneState;
	LPDIRECT3D9             dxObject;			// the Direct3D Object
	LPDIRECT3DDEVICE9       dxDevice;			// the Direct3D Device

	dxManager()
	{
		SceneState = 1;
		dxObject = NULL;
		dxDevice = NULL;
	}

	void createLights() // Creating lights
	{
		dxDevice->SetRenderState(D3DRS_LIGHTING,false);
	}

/********************************************************************
* init
* Initializes DirectX
********************************************************************/
	bool init()
	{

	if( (dxObject = Direct3DCreate9( D3D_SDK_VERSION )) == NULL) // Setting the DirectX Object
	{
		MessageBox(NULL, "Failed  to create the DirectX Object!", "Error", MB_OK);
		return false;
	}

	D3DPRESENT_PARAMETERS d3dpp; // Holds the Direct3D Present Parameters
	// Setting the Present Parameters
    ZeroMemory( &d3dpp, sizeof(d3dpp) );  
    d3dpp.Windowed = true;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount  = 1;
	d3dpp.BackBufferHeight = screen_height;
	d3dpp.BackBufferWidth  = screen_width;
	d3dpp.hDeviceWindow    = wndHandle;

    if( FAILED( dxObject->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, wndHandle,
                                      D3DCREATE_HARDWARE_VERTEXPROCESSING,
                                      &d3dpp, &dxDevice ) ) ) // Setting the DirectX Device
    {
		MessageBox(NULL, "Failed  to create the DirectX Device!", "Error", MB_OK);
        return false;
    }

	dxDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW); // Setting the Culling
	
	dxDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID); // Setting the Filling Mode to solid
	//	dxDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME); - optional WireFrame Filling Mode
	dxDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);  // Normalize

	dxDevice->SetRenderState( D3DRS_ZENABLE, true ); // Enabling the Z-Buffer

	// Create and turn on the lights for the scene
	createLights(); // Creating lights

	return true;
	}

/********************************************************************
* beginRender
* Prepares DirectX for rendering each frame
********************************************************************/
void beginRender(D3DCOLOR bgColor)
{
	if( NULL == dxDevice )
        return;

    // Clear the backbuffer to a black color
    dxDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, bgColor, 1.0f, 0 );

	dxDevice->BeginScene(); // Beginning the scene
}

/********************************************************************
* endRender
* Called after rendering has completed. Presents the result to the
* screen.
********************************************************************/
void endRender(void)
{	
	dxDevice->EndScene(); // Ending the scene
	// Present the backbuffer contents to the display
    dxDevice->Present( NULL, NULL, NULL, NULL );
}


/********************************************************************
* shutdown
* Releases the DirectX interfaces
********************************************************************/
~dxManager()
{
    if( dxDevice != NULL) 
	{
        dxDevice->Release();
		dxDevice = NULL;
	}
    if( dxObject != NULL)
	{
        dxObject->Release();
		dxObject = NULL;
	}
	SceneState = NULL;
}

};

class dxCamera
{
public:
	D3DXVECTOR3 camera_position; // Holding the camera position
	D3DXVECTOR3 camera_lootat; // Holding the camera looking direction
	D3DXVECTOR3 camera_upvector; // Holding the up-vector
	D3DXMATRIX camera_Projection; // The projection matrix
	D3DXMATRIXA16 camera_view; // The view matrix

// Init our camera vars
 dxCamera()
{
	 camera_position = D3DXVECTOR3( 0.0f, 0.0f,0.0f ); // Setting deafult position
	 camera_lootat = D3DXVECTOR3( 0.0f, 0.0f, 0.0f ); // Setting deafult looking direction
	 camera_upvector = D3DXVECTOR3( 0.0f, 1.0f, 0.0f ); // Setting deafult up-vector
}

/********************************************************************
* createCamera
* Utility function used to define a camera
********************************************************************/
void createCamera(float nearClip, float farClip, LPDIRECT3DDEVICE9 lpDevice)
{
	// Setting the camera position, looking direction and up-vector..
	D3DXMatrixLookAtLH(&camera_view, &camera_position,		//Camera Position
                                 &camera_lootat,		//Look At Position
                                 &camera_upvector);		//Up Direction
	//Here we specify the field of view, aspect ration and near and far clipping planes.
    D3DXMatrixPerspectiveFovLH(&camera_Projection, D3DX_PI/4, screen_width/screen_height, nearClip, farClip);
	lpDevice->SetTransform(D3DTS_VIEW, &camera_view);
	lpDevice->SetTransform(D3DTS_PROJECTION, &camera_Projection);
}

};