
#include "Texture.h"

#include "../../Utility/Graphics.h"

namespace Library
{
    // テクスチャを張り付けるポリゴンを作成
    bool Texture::CreatePorigonModel(ID3D11Device* p_Device_, Shader::Vertex* p_VertexShader_)
    {
        D3D11_INPUT_ELEMENT_DESC vertexDesc[]
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };

        // 頂点データ作成
        Utility::TextureVertex vertexList[]
        {
            { { -0.2f,  0.2f, 0.2f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
            { {  0.2f,  0.2f, 0.2f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
            { { -0.2f, -0.2f, 0.2f }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } },

            { {  0.2f,  0.2f, 0.2f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
            { {  0.2f, -0.2f, 0.2f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
            { { -0.2f, -0.2f, 0.2f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },
        };

        // 頂点バッファ作成
        D3D11_BUFFER_DESC bufferDesc
        {
            .ByteWidth = sizeof(Utility::TextureVertex) * 6,
            .Usage = D3D11_USAGE_DEFAULT,
            .BindFlags = D3D11_BIND_VERTEX_BUFFER,
            .CPUAccessFlags = 0,
            .MiscFlags = 0,
            .StructureByteStride = sizeof(Utility::TextureVertex)
        };

        // 頂点バッファの初期データ
        D3D11_SUBRESOURCE_DATA subResourceData
        {
            .pSysMem = vertexList,
            .SysMemPitch = 0,
            .SysMemSlicePitch = 0
        };

        // バッファ作成
        if (FAILED(p_Device_->CreateBuffer(&bufferDesc, &subResourceData, &p_VertexBuffer))) return false;

        // 頂点レイアウト作成
        if (FAILED(p_Device_->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), p_VertexShader_->GetData(), p_VertexShader_->GetSize(), &p_InputLayout))) return false;

        return true;
    }
}
