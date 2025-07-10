#pragma once


class RenderManager {
public:
	RenderManager();
	~RenderManager();

	void Render(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList);


};

