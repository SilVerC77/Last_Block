/**
* @file CQuadPolygon.h
* @brief ポリゴン・クラス
* @author 織戸　喜隆
* @date 日付（2018.07.27）
*/
#pragma once

#include "stdafx.h"
#include "Common3DType.h"

/*! @class CQuadPolygon
@brief  ポリゴン・クラス
*/

class CQuadPolygon
{
protected:
	// Direct3D
	ID3D11DeviceContext * m_pDeviceContext = NULL;	//!< デバイス・コンテキスト
	D3D11_VIEWPORT			m_viewPort[1];			//!< ビューポート
		ID3DBlob *					m_pBlobVS = NULL;	//!< ブロブとはシェーダーの塊みたいなもの
	ID3D11VertexShader*	m_pVertexShader = NULL;		//!< effectではなく、個別に頂点シェーダ
	ID3D11InputLayout*		m_pInputLayout = NULL;	//!< 入力レイアウト
	ID3D11PixelShader*	m_pPixelShader = NULL;		//!< effectではなく、個別にピクセルシェーダー
	std::vector<ID3D11Buffer*>	m_pVertexBuffer;				//!< 頂点バッファ
	ID3D11Buffer*		m_pConstantBuffer = NULL;	//!< アプリ←→シェーダー架け橋 ディフューズを渡すための定数バッファ

	// Direct3D以外
	XMFLOAT2					m_VBLeftTop		= { 0.0f, 0.0f };	//!< 頂点バッファ　矩形　左上
	XMFLOAT2					m_VBRightBottom	= { 0.0f, 0.0f };	//!< 頂点バッファ　矩形　右下
	XMFLOAT2					m_VBSize		= { 0.0f, 0.0f };	//!< 頂点バッファ　サイズ
	XMFLOAT2					m_originalSize	= { 0.0f, 0.0f };	//!< 元のサイズ

	Position<XMFLOAT4>	m_position;						//!< 姿勢<位置 向き スケール >
	FLOAT				m_forwardDirectionOfBullet;		//!< 弾丸の発射時の進行方向
	XMFLOAT4X4		m_mFinalWorld;		//!< 最終的なワールド行列（この姿勢でレンダリングする）
	XMFLOAT4X4		m_matTran;			//!< 平行移動行列
	XMFLOAT4X4		m_matPitch;			//!< 回転行列　ピッチ
	XMFLOAT4X4		m_matYaw;			//!< 回転行列　ヨー
	XMFLOAT4X4		m_matRoll;			//!< 回転行列　ロール
	XMFLOAT4X4		m_matScale;			//!< スケール行列
	size_t			m_animIndex = 0;	//!< アニメーションのテクセルの配列（0～3）
	int				m_animBeginIndex = 0;
	int				m_animEndIndex = 0;

	ID3D11BlendState* m_pDefaultBlendState = NULL;	//!< ブレンド・ステート
	ID3D11BlendState* m_pAlphaBlendState = NULL;	//!< ブレンド・ステート



	BOOL			m_active = TRUE;	//!< 活動中
	eObjectState	m_state = eWaiting;	//!< 移動 / 待機
	FLOAT			m_moveQuantity;		//!< 移動量
	BOOL			m_damage = FALSE;
	std::vector<QuadrangleTexel> m_texels;

	// init の詳細
	void createVertexShader(ID3D11Device* pDevice_, LPCWSTR shaderName_);
	void createVertexInputLayout(ID3D11Device* pDevice_);
	void createPixelShader(ID3D11Device* pDevice_, LPCWSTR shaderName_);
	void createVertexBuffer(ID3D11Device* pDevice_, XMFLOAT2 size_, std::vector<QuadrangleTexel>	texels);
	void createConstBuffer(ID3D11Device* pDevice_);

public:
	static std::unique_ptr<CQuadPolygon> create(
		ID3D11Device*			pDevice_,		//!< デバイス
		ID3D11DeviceContext*	pDeviceContext_,//!< デバイスコンテキスト
		D3D11_VIEWPORT			viewPort_[],	//!< ビューポート
		LPCWSTR					shaderName_,	//!< シェーダープログラム
		XMFLOAT4				position_,		//!< ウィンドウの位置
		XMFLOAT2				size_,			//!< スプライトの大きさ
		std::vector<QuadrangleTexel>	texels);		//!< テクセルの配列
	CQuadPolygon();
	~CQuadPolygon();
	virtual void init(
		ID3D11Device*			pDevice_,		//!< デバイス
		ID3D11DeviceContext*	pDeviceContext_,//!< デバイスコンテキスト
		D3D11_VIEWPORT			viewPort_[],	//!< ビューポート
		LPCWSTR					shaderName_,	//!< シェーダープログラム
		XMFLOAT4				position_,		//!< ウィンドウの位置
		XMFLOAT2				size_,			//!< スプライトの大きさ
		std::vector<QuadrangleTexel>	texels);		//!< テクセルの配列
	/**
	* アクション
	*/
	virtual void render(
		XMFLOAT4X4 matView_,
		XMFLOAT4X4 matProj_,
		std::function<void(ID3D11DeviceContext* pDeviceContext_)> callbackTextureProc_,
		size_t texelIndex_ = 0,
		XMFLOAT4 diffuse_ = { 0, 0, 0, 0 });
	virtual BOOL collision(XMFLOAT4 rivalPos_, FLOAT range_);
	virtual BOOL collision(XMFLOAT2 pos_, FLOAT range_);
	virtual BOOL collision(XMFLOAT2 pos_);
	virtual BOOL collision(XMFLOAT4 pos1_, XMFLOAT4 pos2_, FLOAT range_);
	virtual BOOL collision(CQuadPolygon* target_);
	virtual BOOL collisionBottom(CQuadPolygon* target_);
	virtual BOOL collisionTop(CQuadPolygon* target_);
	virtual BOOL collisionLeft(CQuadPolygon* target_);
	virtual BOOL collisionRight(CQuadPolygon* target_);
	virtual BOOL collisionOffset(CQuadPolygon* target_, XMFLOAT4 offset_ = { 0.0f, 0.0f, 0.0f, 0.0f }, XMFLOAT2 scale_ = {0.5f, 0.5f});
	void computeMatrixFromPos(void);
	void release();
	void reset(XMFLOAT4 pos_) {
		setActive(TRUE);
		setState(eWaiting);
		setPos(pos_);
	}
	/**
	* 状態
	*/
	BOOL isInvalid()
	{
		if (outOfWindow() || !m_active)
		{
			return TRUE;
		}
		return FALSE;
	}
	BOOL outOfWindow(XMFLOAT3 cameraPos_ = {0.0f, 0.0f, 0.0f}) {
		FLOAT left		= m_position.pos.x - m_VBSize.x / 2;
		FLOAT right		= m_position.pos.x + m_VBSize.x / 2;
		FLOAT top		= m_position.pos.y + m_VBSize.y / 2;
		FLOAT bottom	= m_position.pos.y - m_VBSize.y / 2;

		if ((left < WINDOW_LEFT + cameraPos_.x) || (right > WINDOW_RIGHT + cameraPos_.x) || (top > WINDOW_TOP + cameraPos_.y) || (bottom < WINDOW_BOTTOM + cameraPos_.y))
		{
			return TRUE;
		}
		return FALSE;
	}
	/**
	* プロパティ　アクセス
	*/
	BOOL getActive(void)				{ return m_active; }
	void setActive(BOOL cond_)			{ m_active = cond_; }
	eObjectState getState()				{ return m_state; }
	void setState(eObjectState state_)	{ m_state = state_; }
	void setDamage(BOOL damage_)		{ m_damage = damage_; }
	void setScale(XMFLOAT3 scale_) {
		m_position.scale = scale_;
		m_VBSize.x = m_originalSize.x * scale_.x;
		m_VBSize.y = m_originalSize.y * scale_.y;
	};
	void setFlipX (BOOL isFlip)
	{
		if (isFlip == false)
		{
			m_VBSize.x = m_originalSize.x * m_position.scale.x;
			m_VBSize.y = m_originalSize.y * m_position.scale.y;
		}
		else
		{
			m_VBSize.x = m_originalSize.x * m_position.scale.x * -1.0f;
			m_VBSize.y = m_originalSize.y * m_position.scale.y;
		}
	}
	void setScale(float x, float y)
	{
		m_position.scale.x = x;
		m_position.scale.y = y;
	}
	XMFLOAT2 getSize()					{ return m_VBSize; }
	XMFLOAT4 getPos()			{ return m_position.pos; }
	void setPos(XMFLOAT4 pos_)	{ m_position.pos = pos_; }
	void setPos(float x, float y, float z){ m_position.pos.x = x; m_position.pos.y = y; m_position.pos.z;}
	void setPos (float x, float y) { m_position.pos.x = x; m_position.pos.y = y; }
	void setPos(XMFLOAT2 pos) { m_position.pos.x = pos.x; m_position.pos.y = pos.y; }
	void offsetPos(XMFLOAT4 pos_)	{ m_position.offset(pos_); }
	void offsetPosInNormalRange(XMFLOAT4 pos_) {
		offsetPos(pos_);
		if (outOfWindow()) {
			offsetPos({ -pos_.x, -pos_.y, -pos_.z, pos_.w });
		}
	};

	void setAngle(XMFLOAT3 degrees_)	{ m_position.setAngle(degrees_); }//!<  【向き】
	FLOAT getAngleZ()					{ return m_position.roll; }
	void setAngleZ(FLOAT degree_)		{ m_position.roll = degree_; }
	void addAngle(XMFLOAT3 alpha_)		{
		m_position.setAngle({
			m_position.pitch += alpha_.x,
			m_position.yaw += alpha_.y,
			m_position.roll += alpha_.z
			});
	}
	void addAngleZ(FLOAT alpha_) {
		m_position.roll += alpha_;
		m_position.normalize();
	};
	FLOAT getForwardDirection()				{ return m_forwardDirectionOfBullet; }
	void setForwardDirection(FLOAT value_)	{ m_forwardDirectionOfBullet = value_; }

	size_t getNumOfVBs()				{ return m_pVertexBuffer.size(); }
	void setMoveQuantity(FLOAT alpha_)	{ m_moveQuantity = alpha_; }
	FLOAT getMoveQuantity(void)			{ return m_moveQuantity; }
};
