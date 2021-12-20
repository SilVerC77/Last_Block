
#include "CFont.h"


CFont::CFont()
{
}
CFont::~CFont()
{
}

CFont* CFont::create( ID3D11Device* pDevice_, ID3D11DeviceContext* pDeviceContext_, D3D11_VIEWPORT viewPort_[], LPCWSTR shaderName_, LPCWSTR textureName_, XMFLOAT4 position_, XMFLOAT2 size_, std::vector<QuadrangleTexel>	texels, int digits_)
{
	CFont* object;
	object = new CFont();
	if (object != NULL)
	{
		object->init(pDevice_, pDeviceContext_, viewPort_, shaderName_, textureName_, position_, size_, texels, digits_);
	}

	return object;
}

void CFont::init(ID3D11Device* pDevice_, ID3D11DeviceContext* pDeviceContext_, D3D11_VIEWPORT viewPort_[], LPCWSTR shaderName_, LPCWSTR textureName_, XMFLOAT4 position_, XMFLOAT2 size_, std::vector<QuadrangleTexel>	texels, int digits_)
{
	digits = digits_;
	objects = (CPicture**)malloc(sizeof(CPicture*) * digits);
	for (int i0 = 0; i0 < digits; i0++)
	{
		objects[i0] = new CPicture();
		if (objects[i0] != NULL)
		{
			objects[i0]->init(pDevice_, pDeviceContext_, viewPort_, shaderName_, textureName_, position_, size_, texels);
		}
	}
}

void CFont::setPos( float x, float y)
{
	if( objects == NULL)
	{
		return;
	}
	XMFLOAT2 pos = XMFLOAT2( x, y);
	for( int i0 = digits - 1; i0 >= 0; i0--)
	{
		pos.x += objects[i0]->getSize().x;
		objects[ i0]->setPos( pos);
	}
}

void CFont::render(XMFLOAT4X4 matView_, XMFLOAT4X4 matProj_, int num_, size_t texelIndex_, XMFLOAT4 diffuse_)
{

	int value = 0;

	for (int i0 = digits - 1; i0 >= 0; i0--)
	{
		value = num_ / (int)pow(10,i0);
		num_ -= value * (int)pow(10,i0);
		objects[i0]->setAnimation( value);
		objects[i0]->render(matView_, matProj_);
	}
}
void CFont::release()
{
	for (int i0 = 0; i0 < digits; i0++)
	{
		objects[i0]->release();
		objects[i0] = NULL;
	}
	objects = NULL;
}
