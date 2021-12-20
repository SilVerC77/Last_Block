#include "stdafx.h"
#include "COriginalGameApp.h"
#include "GameResource.h"
#include "UV.h"

/**
 * �֐��� : createSprite()
 * �������e : �w�肳�ꂽ�p�X�ɂ���摜���A�w�肳�ꂽ�T�C�Y�ō쐬����
 * ���� :	path_		�摜�̂���p�X
 *			width_		�쐬����摜�̕�
 *			height_		�쐬����摜�̍���
 * �߂�l : �쐬�����摜���Ǘ�����I�u�W�F�N�g
 */
CPicture* COriginalGameApp::createSprite (LPCWSTR path_, float width_, float height_)
{
	vector <QuadrangleTexel>  singleTexels = 
	{
		QuadrangleTexel{ { 0.0, 1.0 },{ 0.0, 0.0 },{ 1.0, 1.0 },{ 1.0, 0.0 } }	//!< �w�肷��UV���W�̔z�񂪈�̔z��
	};
	XMFLOAT2 spriteSize = { width_, height_};
	CPicture* object = NULL;

	// ��{�I�ȍ쐬
	object = CPicture::create (m_pDevice, m_pDeviceContext, m_viewPort, HLSL_PlaneHLSL, path_,
		{ 0.0f, 0.0f, 0.0f, 0.0f },	//!< �X�v���C�g�̈ʒu
		spriteSize,					//!< �X�v���C�g�̃T�C�Y
		singleTexels);				//!< �w�肷��UV���W�̔z��
	return object;
}

CFont* COriginalGameApp::createFont(LPCWSTR path_, int digits_, float width_, float height_)
{
	XMFLOAT2 spriteSize = { width_, height_ };
	CFont* objects = NULL;

	return objects;
}
