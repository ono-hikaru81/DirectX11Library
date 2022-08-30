
#include "Camera.h"

#include "../Window/Window.h"

namespace Engine
{
	// 初期化
	void Camera::Initialize(DirectGraphics* p_DirectGraphics_)
	{
		HWND windowHandle = FindWindow(Window::p_ClassName.c_str(), nullptr);
		RECT rect {};
		GetClientRect(windowHandle, &rect);

		// プロジェクションマトリクス設定
		constexpr float fov = DirectX::XMConvertToRadians(45.0f);
		float aspect = static_cast<float>((rect.right - rect.left) / (rect.bottom - rect.top));
		float nearZ = 0.1f;
		float farZ = 100.0f;
		DirectX::XMMATRIX projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fov, aspect, nearZ, farZ);
		
		Utility::ObjFile::ConstantBuffer constantBufferData = p_DirectGraphics_->GetConstantBufferData3D();

		// コンスタントバッファの設定
		DirectX::XMStoreFloat4x4(&constantBufferData.projection, DirectX::XMMatrixTranspose(projectionMatrix));

		p_DirectGraphics_->SetConstantBufferData3D(constantBufferData);
	}

	// 更新
	void Camera::Update(DirectGraphics* p_DirectGraphics_, Utility::Vector targetPos_, Utility::Vector targetDegree_)
	{
		targetPos = targetPos_;

		HWND windowHandle = FindWindow(Window::p_ClassName.c_str(), nullptr);
		RECT rect{};
		GetClientRect(windowHandle, &rect);

		float distance = sqrtf(powf(pos.x - targetPos_.GetX(), 2.0f) + powf(pos.z - targetPos_.GetZ(), 2.0f));

		pos.x = distance * -sinf(DirectX::XMConvertToRadians(targetDegree_.GetY()));
		pos.z = distance * -cosf(DirectX::XMConvertToRadians(targetDegree_.GetY()));

		// Viewマトリクス設定
		DirectX::XMVECTOR eye = DirectX::XMVectorSet(pos.x, pos.y, pos.z, 0.0f);
		DirectX::XMVECTOR focus = DirectX::XMVectorSet(targetPos.x, targetPos.y, targetPos.z, 0.0f);
		DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(eye, focus, up);

		Utility::ObjFile::ConstantBuffer constantBufferData = p_DirectGraphics_->GetConstantBufferData3D();

		// コンスタントバッファの設定
		DirectX::XMStoreFloat4x4(&constantBufferData.view, DirectX::XMMatrixTranspose(viewMatrix));

		p_DirectGraphics_->SetConstantBufferData3D(constantBufferData);
	}
}
