#include "main.h"
#include "rc.h"
#include <filesystem>

void main_window::on_paint(HDC hdc) 
{
	RECT r;
	GetClientRect(*this, &r);
	using namespace Gdiplus;
	{
		Graphics g(hdc);

		if (!name.empty()) {

			g.DrawImage(image.get(), 0, 0, r.right, r.bottom);

			std::filesystem::path f_path(name);
			std::wstring f_name = f_path.filename();

			Font font(L"Arial", 30);
			SolidBrush b(Color::Black);
			SolidBrush w(Color::White);
			RectF s_b(r.left, r.top, r.right, r.bottom);
			RectF s_w(r.left - 3, r.top, r.right, r.bottom - 3);
			StringFormat format;
			format.SetAlignment(StringAlignmentCenter);
			format.SetLineAlignment(StringAlignmentFar);
			g.DrawString(f_name.c_str(), -1, &font, s_b, &format, &b);
			g.DrawString(f_name.c_str(), -1, &font, s_w, &format, &w);
		}
	}

}

void main_window::on_command(int id) 
{
	switch (id) 
	{
		case ID_OPEN:
		{
			wchar_t path[MAX_PATH];
			*path = 0;
			wchar_t filter[] = L"JPEG\0*.jpeg\0PNG\0*.png\0BMP\0*.bmp\0GIF\0*.gif\0TIFF\0*.tiff\0EMF\0*.emf\0";

			OPENFILENAMEW ofn;
			ZeroMemory(&ofn, sizeof ofn);

			ofn.lStructSize = sizeof ofn;
			ofn.lpstrFile = path;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrFilter = filter;

			if (GetOpenFileName(&ofn))
			{
				name = std::filesystem::path(path).filename();
				image = std::make_unique<Gdiplus::Image>(path);
				::InvalidateRect(*this, nullptr, TRUE);
			}
		}
			break;
		case ID_EXIT:
			DestroyWindow(*this);
			break;
	}
}

void main_window::on_destroy() 
{
	::PostQuitMessage(0);
}

int WINAPI _tWinMain(HINSTANCE instance, HINSTANCE, LPTSTR, int)
{
	gdiplus_application app;
	main_window wnd;
	wnd.create(0, WS_OVERLAPPEDWINDOW | WS_VISIBLE, _T("NWP 7"), (int64_t)LoadMenu(instance, MAKEINTRESOURCE(IDM_MAIN)));
	return app.run();
}
