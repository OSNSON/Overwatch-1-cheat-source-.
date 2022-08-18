#pragma once

#include "imgui.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "renderer.h"
#include <string>

class Renderer
{
public:
	void Initialize();

	void BeginScene();
	void DrawScene();
	void EndScene();
	void DrawCircle(const ImVec2& position, float radius, uint32_t color, float thickness);

	static Renderer* GetInstance();

private:
	Renderer();
	~Renderer();

	void DrawLine(const ImVec2& from, const ImVec2& to, uint32_t color, float thickness = 1.0f);
	void DrawCircleFilled(const ImVec2& position, float radius, uint32_t color);
	void DrawRect(const ImVec2& from, const ImVec2& to, uint32_t color, float thickness);
	void DrawRect(const ImVec2& from, const ImVec2& to, uint32_t color, float thickness, float rounding);
	float DrawText(const std::string& text, const ImVec2& pos, float size, uint32_t color, bool center);
	float DrawText(ImFont* pFont, const std::string& text, const ImVec2& pos, float size, uint32_t color, bool center);


	ImFont* m_pFont;
	static Renderer* m_pInstance;
};
