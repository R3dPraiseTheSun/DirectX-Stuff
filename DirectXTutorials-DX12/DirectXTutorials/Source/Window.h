#pragma once

#include "pch.h"
#include "Graphics.h"
#include "SubWindow.h"

class Window : public SubWindow /*public Graphics*/
{
public:
	Window(uint32_t width, uint32_t height, LPCWSTR className, LPCWSTR title);
	~Window();

	void ParseCommandLineArguments();
	void SetFullscreen(bool fullscreen);

	virtual void Initialize() override;

	virtual	LRESULT	MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override;

private:
	HWND m_Handle;
	Graphics* graphics;
	// Setters
public:
	void SetHandle(HWND handle) { m_Handle = handle; }

	// Getters
public:
	HWND GetHandle() { return m_Handle; }

};

