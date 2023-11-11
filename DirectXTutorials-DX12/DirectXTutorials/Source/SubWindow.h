#pragma once

#include "pch.h"

class Window;

class SubWindow {
public:
	SubWindow(uint32_t width, uint32_t height, LPCWSTR classname, LPCWSTR title);
	~SubWindow();

	virtual void RegisterNewClass();
	virtual void Initialize() = 0;

protected:
	HINSTANCE m_hInstance = GET_HINSTANCE();

	uint32_t m_Width;
	uint32_t m_Height;

	LPCWSTR m_Title;
	LPCWSTR m_ClassName;

	HWND m_Handle;

public:
	HWND GetHandle() { return m_Handle; }

public:
	void Handle(HWND hwnd) { m_Handle = hwnd; }

protected:
	static			LRESULT CALLBACK	SetupMessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static			LRESULT				AssignMessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	virtual			LRESULT				MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

};