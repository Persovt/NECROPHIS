#include "Render.h"

#define D3DFVF_CUSTOM_TEXT ( D3DFVF_XYZ | D3DFVF_DIFFUSE )
#define D3DFVF_CUSTOM_VERTEX ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE )

//namespace Engine
//{
enum EFontFlags
{
	FONTFLAG_NONE,
	FONTFLAG_ITALIC = 0x001,
	FONTFLAG_UNDERLINE = 0x002,
	FONTFLAG_STRIKEOUT = 0x004,
	FONTFLAG_SYMBOL = 0x008,
	FONTFLAG_ANTIALIAS = 0x010,
	FONTFLAG_GAUSSIANBLUR = 0x020,
	FONTFLAG_ROTARY = 0x040,
	FONTFLAG_DROPSHADOW = 0x080,
	FONTFLAG_ADDITIVE = 0x100,
	FONTFLAG_OUTLINE2 = 0x200,
	FONTFLAG_CUSTOM = 0x400,
	FONTFLAG_BITMAP = 0x800,
};

enum FontRenderFlag_t
{
	FONT_LEFT = 0,
	FONT_RIGHT = 1,
	FONT_CENTER = 2
};

enum FontCreateFlag_t
{
	FONT_CREATE_NONE = 0,
	FONT_CREATE_BOLD = 1,
	FONT_CREATE_ITALIC = 2,
	FONT_CREATE_OUTLINE = 4,
	FONT_CREATE_SPRITE = 8,
	FONT_CREATE_ANTIALIASED = 16
};

// Initialises the rendering system, setting up fonts etc
void CRender::SetupFonts()
{
	font.Default = 0x1D; // MainMenu Font from vgui_spew_fonts 
	font.DEF = Interfaces::Surface()->FontCreate();

	Interfaces::Surface()->SetFontGlyphSet(font.DEF, "Arial", 16, 500, 0, 0, FONTFLAG_OUTLINE2);
}

RECT CRender::GetViewport()
{
	RECT Viewport = { 0, 0, 0, 0 };
	int w, h;
	Interfaces::Engine()->GetScreenSize(w, h);
	Viewport.right = w; Viewport.bottom = h;
	return Viewport;
}

void CRender::Clear(int x, int y, int w, int h, Color color)
{
	Interfaces::Surface()->DrawSetColor(color);
	Interfaces::Surface()->DrawFilledRect(x, y, x + w, y + h);
}

void CRender::DrawRect(int x, int y, int w, int h, Color col)
{
	Interfaces::Surface()->DrawSetColor(col);
	Interfaces::Surface()->DrawOutlinedRect(x, y, x + w, y + h);
}

void CRender::DrawOutlineRect(int x, int y, int w, int h, Color color)
{
	DrawRect(x, y, w, h, Color::Black());
	DrawRect(x + 1, y + 1, w - 2, h - 2, color);
	DrawRect(x + 2, y + 2, w - 4, h - 4, Color::Black());
}

void CRender::DrawString(DWORD font, int x, int y, Color color, DWORD alignment, const char* msg, ...)
{
	va_list va_alist;
	char buf[1024];
	va_start(va_alist, msg);
	_vsnprintf(buf, sizeof(buf), msg, va_alist);
	va_end(va_alist);
	wchar_t wbuf[1024];
	MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);



	int width, height;

	Interfaces::Surface()->GetTextSize(font, wbuf, width, height);

	if (alignment & FONT_RIGHT)
		x -= width;
	if (alignment & FONT_CENTER)
		x -= width / 2;

	Interfaces::Surface()->DrawSetTextFont(font);


	Interfaces::Surface()->DrawSetTextPos(x, y - height / 2);
	Interfaces::Surface()->DrawPrintText(wbuf, wcslen(wbuf));
}




void CRender::DrawHeader(int x, int y, int w, int HealthBarWidth)
{
	int i = 0;
	int xCoord = x;
	int yCoord = y;

	for (i = 0; i < 5; i++)
	{
		FillRGBA(x - i, y + i, w, 1, Color::Black());

		FillRGBA(x - i, y + i, HealthBarWidth + 2, 1, Color(0, 250, 0));

		FillRGBA(x - 3, y + 3, HealthBarWidth + 2, 1, Color(0, 170, 0));

		FillRGBA(x - 4, y + 4, HealthBarWidth + 2, 1, Color(0, 170, 0));
	}

	FillRGBA(x, y, w, 1, Color::White());

	FillRGBA((x + 1) - 5, y + 5, w, 1, Color::White());

	for (i = 0; i < 5; i++)
	{
		FillRGBA(x, y, 1, 1, Color::White());
		x--;
		y++;
	}

	x = xCoord;
	y = yCoord;

	for (i = 0; i < 5; i++)
	{
		if (i != 0)
			FillRGBA(x + w, y, 1, 1, Color::White());

		x--;
		y++;
	}




}

void CRender::OutlineRainbow(int x, int y, int width, int height, float flSpeed, float &flRainbow)
{
	Color colColor(0, 0, 0);

	flRainbow += flSpeed;
	if (flRainbow > 1.f) flRainbow = 0.f;

	for (int i = 0; i < width; i++)
	{
		float hue = (1.f / (float)width) * i;
		hue -= flRainbow;
		if (hue < 0.f) hue += 1.f;

		Color colRainbow = colColor.FromHSB(hue, 1.f, 1.f);
		DrawOutlineRect(x + i, y, 1, height, colRainbow);
	}
}

void CRender::Pixel(int x, int y, Color col)
{
	Interfaces::Surface()->DrawSetColor(col);
	Interfaces::Surface()->DrawFilledRect(x, y, x + 1, y + 1);
}

void CRender::Line(int x, int y, int x2, int y2, Color color)
{
	Interfaces::Surface()->DrawSetColor(color);
	Interfaces::Surface()->DrawLine(x, y, x2, y2);
}

void CRender::PolyLine(int *x, int *y, int count, Color color)
{
	Interfaces::Surface()->DrawSetColor(color);
	Interfaces::Surface()->DrawPolyLine(x, y, count);
}

bool CRender::WorldToScreen(Vector &in, Vector &out)
{
	const VMatrix& worldToScreen = Interfaces::Engine()->WorldToScreenMatrix(); //Grab the world to screen matrix from CEngineClient::WorldToScreenMatrix

	float w = worldToScreen[3][0] * in[0] + worldToScreen[3][1] * in[1] + worldToScreen[3][2] * in[2] + worldToScreen[3][3]; //Calculate the angle in compareson to the player's camera.
	out.z = 0; //Screen doesn't have a 3rd dimension.

	if (w > 0.001) //If the object is within view.
	{
		RECT ScreenSize = GetViewport();
		float fl1DBw = 1 / w; //Divide 1 by the angle.
		out.x = (ScreenSize.right / 2) + (0.5f * ((worldToScreen[0][0] * in[0] + worldToScreen[0][1] * in[1] + worldToScreen[0][2] * in[2] + worldToScreen[0][3]) * fl1DBw) * ScreenSize.right + 0.5f); //Get the X dimension and push it in to the Vector.
		out.y = (ScreenSize.bottom / 2) - (0.5f * ((worldToScreen[1][0] * in[0] + worldToScreen[1][1] * in[1] + worldToScreen[1][2] * in[2] + worldToScreen[1][3]) * fl1DBw) * ScreenSize.bottom + 0.5f); //Get the Y dimension and push it in to the Vector.
		return true;
	}

	return false;
}

void CRender::Text(int x, int y, Color color, DWORD font, const char* text, ...)
{
	size_t origsize = strlen(text) + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t wcstring[newsize];
	mbstowcs_s(&convertedChars, wcstring, origsize, text, _TRUNCATE);

	Interfaces::Surface()->DrawSetTextFont(font);

	Interfaces::Surface()->DrawSetTextColor(color);
	Interfaces::Surface()->DrawSetTextPos(x, y);
	Interfaces::Surface()->DrawPrintText(wcstring, wcslen(wcstring));
}

void CRender::OutlinedBox2(int x, int y, int w, int h, Color color)
{
	DrawRect(x, y, w, h, Color::Black());
	DrawRect(x + 1, y + 1, w - 2, h - 2, color);
}



void CRender::DrawRectFilled(int x0, int y0, int x1, int y1, Color color)
{
	Interfaces::Surface()->DrawSetColor(color);
	Interfaces::Surface()->DrawFilledRect(x0, y0, x1, y1);
}

void CRender::FillRGBA(int x, int y, int w, int h, Color c)
{
	Interfaces::Surface()->DrawSetColor(c);
	Interfaces::Surface()->DrawFilledRect(x, y, x + w, y + h);
}


void CRender::TexturedPolygon(int n, std::vector<SDK::Vertex_t> vertice, Color color)
{
	static int texture_id = Interfaces::Surface()->CreateNewTextureID(true); // 
	static unsigned char buf[4] = { 255, 255, 255, 255 };
	Interfaces::Surface()->DrawSetTextureRGBA(texture_id, buf, 1, 1); //
	Interfaces::Surface()->DrawSetColor(color); //
	Interfaces::Surface()->DrawSetTexture(texture_id); //
	Interfaces::Surface()->DrawTexturedPolygon(n, vertice.data()); //
}

void CRender::DrawFilledCircle(Vector2D center, Color color, float radius, float points) {
	std::vector<Vertex_t> vertices;
	float step = (float)M_PI * 2.0f / points;

	for (float a = 0; a < (M_PI * 2.0f); a += step)
		vertices.push_back(Vertex_t(Vector2D(radius * cosf(a) + center.x, radius * sinf(a) + center.y)));

	TexturedPolygon((int)points, vertices, color);
}



void CRender::Textf(int x, int y, Color color, DWORD font, const char* fmt, ...)
{
	if (!fmt) return; //if the passed string is null return
	if (strlen(fmt) < 2) return;

	//Set up va_list and buffer to hold the params 
	va_list va_alist;
	char logBuf[256] = { 0 };

	//Do sprintf with the parameters
	va_start(va_alist, fmt);
	_vsnprintf_s(logBuf + strlen(logBuf), 256 - strlen(logBuf), sizeof(logBuf) - strlen(logBuf), fmt, va_alist);
	va_end(va_alist);

	Text(x, y, color, font, logBuf);
}

void CRender::BoxDefault(DrawData Data, Color pColor)
{
	Interfaces::Surface()->DrawSetColor(pColor);
	Interfaces::Surface()->DrawOutlinedRect(Data.Left, Data.Up, Data.Right, Data.Down);
}

void CRender::GradientV(int x, int y, int w, int h, Color c1, Color c2)
{
	Clear(x, y, w, h, c1);
	BYTE first = c2.r();
	BYTE second = c2.g();
	BYTE third = c2.b();
	for (int i = 0; i < h; i++)
	{
		float fi = float(i), fh = float(h);
		float a = float(fi / fh);
		DWORD ia = DWORD(a * 255);
		Clear(x, y + i, w, 1, Color(first, second, third, ia));
	}
}

void CRender::GradientH(int x, int y, int w, int h, Color c1, Color c2)
{
	Clear(x, y, w, h, c1);
	BYTE first = c2.r();
	BYTE second = c2.g();
	BYTE third = c2.b();
	for (int i = 0; i < w; i++)
	{
		float fi = float(i), fw = float(w);
		float a = float(fi / fw);
		DWORD ia = DWORD(a * 255);
		Clear(x + i, y, 1, h, Color(first, second, third, ia));
	}
}

void CRender::Polygon(int count, Vertex_t* Vertexs, Color color)
{
	static int Texture = Interfaces::Surface()->CreateNewTextureID(true); //need to make a texture with procedural true
	unsigned char buffer[4] = { 255, 255, 255, 255 };//{ color.r(), color.g(), color.b(), color.a() };

	Interfaces::Surface()->DrawSetTextureRGBA(Texture, buffer, 1, 1); //Texture, char array of texture, width, height
	Interfaces::Surface()->DrawSetColor(color); // keep this full color and opacity use the RGBA @top to set values.
	Interfaces::Surface()->DrawSetTexture(Texture); // bind texture

	Interfaces::Surface()->DrawTexturedPolygon(count, Vertexs);
}

void CRender::PolygonOutline(int count, Vertex_t* Vertexs, Color color, Color colorLine)
{
	static int x[128];
	static int y[128];

	Polygon(count, Vertexs, color);

	for (int i = 0; i < count; i++)
	{
		x[i] = int(Vertexs[i].m_Position.x);
		y[i] = int(Vertexs[i].m_Position.y);
	}

	PolyLine(x, y, count, colorLine);
}

void CRender::PolyLine(int count, Vertex_t* Vertexs, Color colorLine)
{
	static int x[128];
	static int y[128];

	for (int i = 0; i < count; i++)
	{
		x[i] = int(Vertexs[i].m_Position.x);
		y[i] = int(Vertexs[i].m_Position.y);
	}

	PolyLine(x, y, count, colorLine);
}

void CRender::DrawWave(Vector loc, float radius, Color color)
{
	static float Step = M_PI * 3.0f / 40;
	Vector prev;
	for (float lat = 0; lat <= M_PI * 3.0f; lat += Step)
	{
		float sin1 = sin(lat);
		float cos1 = cos(lat);
		float sin3 = sin(0.0);
		float cos3 = cos(0.0);

		Vector point1;
		point1 = Vector(sin1 * cos3, cos1, sin1 * sin3) * radius;
		Vector point3 = loc;
		Vector Out;
		point3 += point1;

		if (WorldToScreen(point3, Out))
		{
			if (lat > 0.000) {

				Line(prev.x, prev.y, Out.x, Out.y, color);
			}
		}
		prev = Out;
	}
}

void CRender::DrawCircle3D(Vector position, float points, float radius, Color Color)
{
	float Step = (float)M_PI * 2.0f / points;

	std::vector<Vector> points3d;

	for (float a = 0; a < (M_PI * 2.0f); a += Step)
	{
		Vector Start(radius * cosf(a) + position.x, radius * sinf(a) + position.y, position.z);
		Vector End(radius * cosf(a + Step) + position.x, radius * sinf(a + Step) + position.y, position.z);

		Vector start2d, end2d;

		if (!WorldToScreen(Start, start2d) || !WorldToScreen(End, end2d))
			return;

		Line(start2d.x, start2d.y, end2d.x, end2d.y, Color);
	}
}

void CRender::OutlineCircle(int x, int y, int r, int seg, Color color)
{
	Interfaces::Surface()->DrawSetColor(0, 0, 0, 255);
	Interfaces::Surface()->DrawOutlinedCircle(x, y, r - 1, seg);
	Interfaces::Surface()->DrawOutlinedCircle(x, y, r + 1, seg);
	Interfaces::Surface()->DrawSetColor(color);
	Interfaces::Surface()->DrawOutlinedCircle(x, y, r, seg);
}

	CRender::CRender(IDirect3DDevice9* pDevice)
	{
		m_pFont = nullptr;
		m_pIconz = nullptr;
		m_pLog = nullptr;
		m_pDevice = pDevice;

		m_pStateBlockDraw = nullptr;
		m_pStateBlockText = nullptr;

		if (!CreateObject())
		{
			MessageBoxA(0, "Fail to create d3d9 objects", "Error", MB_OK | MB_ICONERROR);
			TerminateProcess(GetCurrentProcess(), 0);
		}
	}

	CRender::~CRender()
	{
		if (m_pFont)
			m_pFont = nullptr;

		if (m_pIconz)
			m_pIconz = nullptr;

		if (m_pLog)
			m_pLog = nullptr;

		if (m_pStateBlockDraw)
			m_pStateBlockDraw = nullptr;

		if (m_pStateBlockText)
			m_pStateBlockText = nullptr;

		m_pDevice = nullptr;
	}

	bool CRender::CreateObject()
	{

		if (!m_pFont)
		{
			HRESULT hFontRes = D3DXCreateFontA(m_pDevice, 17, 0, FW_HEAVY, 0, FALSE, RUSSIAN_CHARSET, OUT_OUTLINE_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FW_DONTCARE, "Comfortaa", &m_pFont);

			if (hFontRes != D3D_OK)
				return false;
		}

		if (!m_pIconz)
		{
			HRESULT hFontRes = D3DXCreateFontA(m_pDevice, 17, 0, FW_HEAVY, 0, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FW_DONTCARE, "AstriumWep", &m_pIconz);

			if (hFontRes != D3D_OK)
				return false;
		}

		if (!m_pLog)
		{
			HRESULT lFont = D3DXCreateFontA(m_pDevice, 12, 0, 666, 0, FALSE, RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FW_DONTCARE, "BRLNSB", &m_pLog); //Agencyb
			if (lFont != D3D_OK)
				return false;
		}


		return true;
	}

	void CRender::OnLostDevice()
	{
		if (m_pFont)
			m_pFont->OnLostDevice();

		if (m_pIconz)
			m_pIconz->OnLostDevice();

		if (m_pLog)
			m_pLog->OnLostDevice();

		if (m_pStateBlockDraw)
			m_pStateBlockDraw = nullptr;

		if (m_pStateBlockText)
			m_pStateBlockText = nullptr;
	}

	void CRender::OnResetDevice()
	{
		if (m_pFont)
			m_pFont->OnResetDevice();

		if (m_pIconz)
			m_pIconz->OnResetDevice();

		if (m_pLog)
			m_pLog->OnResetDevice();

		if (!CreateObject())
		{
			MessageBoxA(0, "Fail to reset d3d9 objects", "Error", MB_OK | MB_ICONERROR);
			TerminateProcess(GetCurrentProcess(), 0);
		}
	}
	void CRender::DrawAlphaBox(int x, int y, int w, int h, Color color)
	{
		DWORD dxFillBoxColor = D3DCOLOR_RGBA(color.r(), color.g(), color.b(), 100);

		D3DTLVERTEX D3DTLV[4] = { 0 };

		CreateVertex(x, y, dxFillBoxColor, PD3DTLVERTEX(&D3DTLV), 0);
		CreateVertex(x + w, y, dxFillBoxColor, PD3DTLVERTEX(&D3DTLV), 1);
		CreateVertex(x, y + h, dxFillBoxColor, PD3DTLVERTEX(&D3DTLV), 2);
		CreateVertex(x + w, y + h, dxFillBoxColor, PD3DTLVERTEX(&D3DTLV), 3);

		SetVertexState();

		m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &D3DTLV, sizeof(D3DTLVERTEX));
	}
	void CRender::BeginRender()
	{
		m_pDevice->CreateStateBlock(D3DSBT_ALL, &m_pStateBlockDraw);
		m_pDevice->CreateStateBlock(D3DSBT_ALL, &m_pStateBlockText);

		if (m_pStateBlockDraw)
			m_pStateBlockDraw->Capture();

		m_pDevice->GetRenderState(D3DRS_COLORWRITEENABLE, &dwOld_D3DRS_COLORWRITEENABLE);
		m_pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
	}

	void CRender::EndRender()
	{
		m_pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, dwOld_D3DRS_COLORWRITEENABLE);

		if (m_pStateBlockDraw)
		{
			m_pStateBlockDraw->Apply();
			m_pStateBlockDraw->Release();
		}

		if (m_pStateBlockText)
			m_pStateBlockText->Release();
	}


	void CRender::DrawLine(int x1, int y1, int x2, int y2, Color color)
	{
		DWORD dxLineColor = D3DCOLOR_RGBA(color.r(), color.g(), color.b(), color.a());

		D3DTLVERTEX D3DTLV[2] = { 0 };

		CreateVertex(x1, y1, dxLineColor, PD3DTLVERTEX(&D3DTLV), 0);
		CreateVertex(x2, y2, dxLineColor, PD3DTLVERTEX(&D3DTLV), 1);

		SetVertexState();

		m_pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, &D3DTLV, sizeof(D3DTLVERTEX));
	}

	

	void CRender::Line(Vector2D start_pos, Vector2D end_pos, Color col)
	{
		DrawLine(start_pos.x, start_pos.y, end_pos.x, end_pos.y, col);
	}

	void CRender::DrawCircle(Vector2D position, float points, float radius, Color color)
	{
		float step = (float)M_PI * 2.0f / points;

		for (float a = 0; a < (M_PI * 2.0f); a += step)
		{
			Vector2D start(radius * cosf(a) + position.x, radius * sinf(a) + position.y);
			Vector2D end(radius * cosf(a + step) + position.x, radius * sinf(a + step) + position.y);
			Line(start, end, color);
		}
	}

	void CRender::DrawWave1(Vector loc, float radius, Color color)
	{
		static float Step = M_PI * 3.0f / 40;
		Vector prev;
		for (float lat = 0; lat <= M_PI * 3.0f; lat += Step)
		{
			float sin1 = sin(lat);
			float cos1 = cos(lat);
			float sin3 = sin(0.0);
			float cos3 = cos(0.0);

			Vector point1;
			point1 = Vector(sin1 * cos3, cos1, sin1 * sin3) * radius;
			Vector point3 = loc;
			Vector Out;
			point3 += point1;

			if (WorldToScreen(point3, Out))
			{
				if (lat > 0.000)
					DrawLine(prev.x, prev.y, Out.x, Out.y, color);
			}
			prev = Out;
		}
	}
	

	
	void CRender::GenuineBox(int x, int y, int w, int h, Color color)
	{
		int iw = w / 4;
		int ih = h / 1;
		// top
		DrawLine(x, y, x + iw, y, color);                    // left
		DrawLine(x + w - iw, y, x + w, y, color);            // right
		DrawLine(x, y, x, y + ih, color);                    // top left
		DrawLine(x + w - 1, y, x + w - 1, y + ih, color);    // top right
															 // bottom
		DrawLine(x, y + h, x + iw, y + h, color);            // left
		DrawLine(x + w - iw, y + h, x + w, y + h, color);    // right
		DrawLine(x, y + h - ih, x, y + h, color);            // bottom left
		DrawLine(x + w - 1, y + h - ih, x + w - 1, y + h, color);    // bottom right
	}

	void CRender::GenuineOutlineBox(int x, int y, int w, int h, Color color)
	{
		int iw = w / 4;
		int ih = h / 1.001;

		GenuineBox(x, y, w, h, Color::Black());

		// top left
		DrawLine(x + 1, y + 1, x + iw, y + 1, color);
		DrawLine(x + iw, y, x + iw, y + 2, Color::Black());
		DrawLine(x + 1, y + 1, x + 1, y + ih, color);
		DrawLine(x, y + ih, x + 2, y + ih, Color::Black());

		// top right
		DrawLine(x + w - iw, y + 1, x + w - 1, y + 1, color);
		DrawLine(x + w - iw - 1, y, x + w - iw, y + 2, Color::Black());
		DrawLine(x + w - 2, y + 1, x + w - 2, y + ih, color);
		DrawLine(x + w - 2, y + ih, x + w, y + ih, Color::Black());

		// bottom left
		DrawLine(x + 1, y + h - ih, x + 1, y + h, color);
		DrawLine(x, y + h - ih - 1, x + 2, y + h - ih - 1, Color::Black());
		DrawLine(x + 1, y + h - 1, x + iw, y + h - 1, color);
		DrawLine(x + iw, y + h - 1, x + iw, y + h + 1, Color::Black());

		// bottom right
		DrawLine(x + w - iw, y + h - 1, x + w - 1, y + h - 1, color);
		DrawLine(x + w - iw - 1, y + h - 1, x + w - iw, y + h + 1, Color::Black());
		DrawLine(x + w - 2, y + h - ih, x + w - 2, y + h, color);
		DrawLine(x + w - 2, y + h - ih - 1, x + w, y + h - ih - 1, Color::Black());

		GenuineBox(x + 2, y + 2, w - 4, h - 4, Color::Black());
	}

	void CRender::DrawBox(int x, int y, int w, int h, Color color)
	{
		DWORD dxBoxColor = D3DCOLOR_RGBA(color.r(), color.g(), color.b(), color.a());

		D3DTLVERTEX D3DTLV[8] = { 0 };

		// top
		CreateVertex(x, y, dxBoxColor, PD3DTLVERTEX(&D3DTLV), 0);
		CreateVertex(x + w, y, dxBoxColor, PD3DTLVERTEX(&D3DTLV), 1);

		// left
		CreateVertex(x, y, dxBoxColor, PD3DTLVERTEX(&D3DTLV), 2);
		CreateVertex(x, y + h, dxBoxColor, PD3DTLVERTEX(&D3DTLV), 3);

		// right
		CreateVertex(x + w - 1, y, dxBoxColor, PD3DTLVERTEX(&D3DTLV), 4);
		CreateVertex(x + w - 1, y + h, dxBoxColor, PD3DTLVERTEX(&D3DTLV), 5);

		// bottom
		CreateVertex(x, y + h - 1, dxBoxColor, PD3DTLVERTEX(&D3DTLV), 6);
		CreateVertex(x + w, y + h - 1, dxBoxColor, PD3DTLVERTEX(&D3DTLV), 7);

		SetVertexState();

		m_pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 4, &D3DTLV, sizeof(D3DTLVERTEX));
	}

	void CRender::DrawFillBox(int x, int y, int w, int h, Color color)
	{
		DWORD dxFillBoxColor = D3DCOLOR_RGBA(color.r(), color.g(), color.b(), color.a());

		D3DTLVERTEX D3DTLV[4] = { 0 };

		CreateVertex(x, y, dxFillBoxColor, PD3DTLVERTEX(&D3DTLV), 0);
		CreateVertex(x + w, y, dxFillBoxColor, PD3DTLVERTEX(&D3DTLV), 1);
		CreateVertex(x, y + h, dxFillBoxColor, PD3DTLVERTEX(&D3DTLV), 2);
		CreateVertex(x + w, y + h, dxFillBoxColor, PD3DTLVERTEX(&D3DTLV), 3);

		SetVertexState();

		m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &D3DTLV, sizeof(D3DTLVERTEX));
	}

	void CRender::DrawOutlineBox(int x, int y, int w, int h, Color color)
	{
		DrawBox(x, y, w, h, Color::Black());
		DrawBox(x + 1, y + 1, w - 2, h - 2, color);
		DrawBox(x + 2, y + 2, w - 4, h - 4, Color::Black());
	}

	void CRender::DrawCoalBox(int x, int y, int w, int h, Color color)
	{
		int iw = w / 4;
		int ih = h / 4;
		// top
		DrawLine(x, y, x + iw, y, color);					// left
		DrawLine(x + w - iw, y, x + w, y, color);			// right
		DrawLine(x, y, x, y + ih, color);					// top left
		DrawLine(x + w - 1, y, x + w - 1, y + ih, color);	// top right
															// bottom
		DrawLine(x, y + h, x + iw, y + h, color);			// left
		DrawLine(x + w - iw, y + h, x + w, y + h, color);	// right
		DrawLine(x, y + h - ih, x, y + h, color);			// bottom left
		DrawLine(x + w - 1, y + h - ih, x + w - 1, y + h, color);	// bottom right
	}

	void CRender::DrawOutlineCoalBox(int x, int y, int w, int h, Color color)
	{
		int iw = w / 4;
		int ih = h / 4;

		DrawCoalBox(x, y, w, h, Color::Black());

		// top left
		DrawLine(x + 1, y + 1, x + iw, y + 1, color);
		DrawLine(x + iw, y, x + iw, y + 2, Color::Black());
		DrawLine(x + 1, y + 1, x + 1, y + ih, color);
		DrawLine(x, y + ih, x + 2, y + ih, Color::Black());

		// top right
		DrawLine(x + w - iw, y + 1, x + w - 1, y + 1, color);
		DrawLine(x + w - iw - 1, y, x + w - iw, y + 2, Color::Black());
		DrawLine(x + w - 2, y + 1, x + w - 2, y + ih, color);
		DrawLine(x + w - 2, y + ih, x + w, y + ih, Color::Black());

		// bottom left
		DrawLine(x + 1, y + h - ih, x + 1, y + h, color);
		DrawLine(x, y + h - ih - 1, x + 2, y + h - ih - 1, Color::Black());
		DrawLine(x + 1, y + h - 1, x + iw, y + h - 1, color);
		DrawLine(x + iw, y + h - 1, x + iw, y + h + 1, Color::Black());

		// bottom right
		DrawLine(x + w - iw, y + h - 1, x + w - 1, y + h - 1, color);
		DrawLine(x + w - iw - 1, y + h - 1, x + w - iw, y + h + 1, Color::Black());
		DrawLine(x + w - 2, y + h - ih, x + w - 2, y + h, color);
		DrawLine(x + w - 2, y + h - ih - 1, x + w, y + h - ih - 1, Color::Black());

		DrawCoalBox(x + 2, y + 2, w - 4, h - 4, Color::Black());
	}

	void CRender::DrawWindow(int x, int y, int w, int h, Color Border, Color Background)
	{
		DrawBox(x, y, w, h, Border);
		DrawFillBox(x + 1, y + 1, w - 2, h - 2, Background);
	}

	void CRender::DrawHorBar(int x, int y, int w, int h, int val, Color color1, Color color2)
	{
		if (val > 100) val = 100;
		int size = (w * val) / 100;

		DrawBox(x, y, w, h, Color::Black());
		DrawFillBox(x + 1, y + 1, w - 2, h - 2, color2);

		if (size == w)
			DrawFillBox(x + 1, y + 1, size - 2, h - 2, color1);
		else
			DrawFillBox(x + 1, y + 1, size, h - 2, color1);
	}

	void CRender::DrawVerBar(int x, int y, int w, int h, int val, Color color1, Color color2)
	{
		if (val > 100) val = 100;
		int size = (h * val) / 100;

		DrawBox(x, y, w, h, Color(0, 0, 0));
		DrawFillBox(x + 1, y + 1, w - 2, h - 2, color2);

		if (size == h)
			DrawFillBox(x + 1, y + 1 + h - size, w - 2, size - 2, color1);
		else
			DrawFillBox(x + 1, y + 1 + h - size, w - 2, size, color1);
	}

	void CRender::Text(int x, int y, bool center, bool shadow, Color color, const char* format, ...)
	{
		if (m_pStateBlockText)
			m_pStateBlockText->Capture();

		m_pDevice->SetFVF(D3DFVF_CUSTOM_TEXT);

		m_pDevice->SetRenderState(D3DRS_LIGHTING, false);
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
		m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		char Buffer[128] = { '\0' };
		va_list va_alist;
		va_start(va_alist, format);
		vsprintf_s(Buffer, format, va_alist);
		va_end(va_alist);

		BSTR text = CSX::Utils::ConvertStringToBSTR_UTF8(Buffer);

		DWORD dxTextColor = D3DCOLOR_XRGB(color.r(), color.g(), color.b());

		auto drawShadow = [&](RECT rect)
		{
			rect.left++;
			m_pFont->DrawTextW(NULL, text, -1, &rect, DT_TOP | DT_LEFT | DT_NOCLIP, 0xFF000000);
			rect.top++;
			m_pFont->DrawTextW(NULL, text, -1, &rect, DT_TOP | DT_LEFT | DT_NOCLIP, 0xFF000000);
		};

		if (center)
		{
			RECT rec = { 0,0,0,0 };

			m_pFont->DrawTextW(NULL, text, -1, &rec, DT_CALCRECT | DT_NOCLIP, dxTextColor);

			rec =
			{
				static_cast<LONG>(x) - rec.right / 2,
				static_cast<LONG>(y),
				0,
				0
			};

			if (shadow)
				drawShadow(rec);

			m_pFont->DrawTextW(NULL, text, -1, &rec, DT_TOP | DT_LEFT | DT_NOCLIP, dxTextColor);
		}
		else
		{
			RECT rec =
			{
				static_cast<LONG>(x),
				static_cast<LONG>(y),
				0,
				0
			};

			if (shadow)
				drawShadow(rec);

			m_pFont->DrawTextW(NULL, text, -1, &rec, DT_TOP | DT_LEFT | DT_NOCLIP, dxTextColor);
		}

		if (m_pStateBlockText)
			m_pStateBlockText->Apply();

		SysFreeString(text);
	}

	void CRender::WepIcon(int x, int y, bool center, bool shadow, Color color, const char* format, ...)
	{
		if (m_pStateBlockText)
			m_pStateBlockText->Capture();

		m_pDevice->SetFVF(D3DFVF_CUSTOM_TEXT);

		m_pDevice->SetRenderState(D3DRS_LIGHTING, false);
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
		m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		char Buffer[128] = { '\0' };
		va_list va_alist;
		va_start(va_alist, format);
		vsprintf_s(Buffer, format, va_alist);
		va_end(va_alist);

		BSTR text = CSX::Utils::ConvertStringToBSTR_UTF8(Buffer);

		DWORD dxTextColor = D3DCOLOR_XRGB(color.r(), color.g(), color.b());

		auto drawShadow = [&](RECT rect)
		{
			rect.left++;
			m_pIconz->DrawTextW(NULL, text, -1, &rect, DT_TOP | DT_LEFT | DT_NOCLIP, 0xFF000000);
			rect.top++;
			m_pIconz->DrawTextW(NULL, text, -1, &rect, DT_TOP | DT_LEFT | DT_NOCLIP, 0xFF000000);
		};

		if (center)
		{
			RECT rec = { 0,0,0,0 };

			m_pIconz->DrawTextW(NULL, text, -1, &rec, DT_CALCRECT | DT_NOCLIP, dxTextColor);

			rec =
			{
				static_cast<LONG>(x) - rec.right / 2,
				static_cast<LONG>(y),
				0,
				0
			};

			if (shadow)
				drawShadow(rec);

			m_pIconz->DrawTextW(NULL, text, -1, &rec, DT_TOP | DT_LEFT | DT_NOCLIP, dxTextColor);
		}
		else
		{
			RECT rec =
			{
				static_cast<LONG>(x),
				static_cast<LONG>(y),
				0,
				0
			};

			if (shadow)
				drawShadow(rec);

			m_pIconz->DrawTextW(NULL, text, -1, &rec, DT_TOP | DT_LEFT | DT_NOCLIP, dxTextColor);
		}

		if (m_pStateBlockText)
			m_pStateBlockText->Apply();

		SysFreeString(text);
	}

	int CRender::GetTextHeight(const char* text)
	{
		RECT rcRect = { 0,0,0,0 };
		if (m_pLog)
			m_pLog->DrawTextA(NULL, text, strlen(text), &rcRect, DT_CALCRECT, D3DCOLOR_XRGB(0, 0, 0));
		return rcRect.bottom - rcRect.top;
	}

	void CRender::TextToConsole(int x, int y, bool center, bool shadow, Color color, const char* format, ...)
	{
		if (m_pStateBlockText)
			m_pStateBlockText->Capture();

		m_pDevice->SetFVF(D3DFVF_CUSTOM_TEXT);

		m_pDevice->SetRenderState(D3DRS_LIGHTING, false);
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
		m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		char Buffer[128] = { '\0' };
		va_list va_alist;
		va_start(va_alist, format);
		vsprintf_s(Buffer, format, va_alist);
		va_end(va_alist);

		BSTR text = CSX::Utils::ConvertStringToBSTR_UTF8(Buffer);

		DWORD
			dxTextColor = D3DCOLOR_ARGB(color.a(), color.r(), color.g(), color.b()),
			shadowColor = D3DCOLOR_ARGB(color.a(), 0, 0, 0);

		auto drawShadow = [&](RECT rect)
		{
			rect.left++;
			m_pLog->DrawTextW(NULL, text, -1, &rect, DT_TOP | DT_LEFT | DT_NOCLIP, shadowColor);
			rect.top++;
			m_pLog->DrawTextW(NULL, text, -1, &rect, DT_TOP | DT_LEFT | DT_NOCLIP, shadowColor);
		};

		if (center)
		{
			RECT rec = { 0,0,0,0 };

			m_pLog->DrawTextW(NULL, text, -1, &rec, DT_CALCRECT | DT_NOCLIP, dxTextColor);

			rec =
			{
				static_cast<LONG>(x) - rec.right / 2,
				static_cast<LONG>(y),
				0,
				0
			};

			if (shadow)
				drawShadow(rec);

			m_pLog->DrawTextW(NULL, text, -1, &rec, DT_TOP | DT_LEFT | DT_NOCLIP, dxTextColor);
		}
		else
		{
			RECT rec =
			{
				static_cast<LONG>(x),
				static_cast<LONG>(y),
				0,
				0
			};

			if (shadow)
				drawShadow(rec);

			m_pLog->DrawTextW(NULL, text, -1, &rec, DT_TOP | DT_LEFT | DT_NOCLIP, dxTextColor);
		}

		if (m_pStateBlockText)
			m_pStateBlockText->Apply();

		SysFreeString(text);
	}

	void CRender::SetVertexState()
	{
		m_pDevice->SetTexture(0, 0);
		m_pDevice->SetPixelShader(0);

		m_pDevice->SetFVF(D3DFVF_CUSTOM_VERTEX);

		m_pDevice->SetRenderState(D3DRS_LIGHTING, false);
		m_pDevice->SetRenderState(D3DRS_FOGENABLE, false);

		m_pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

		m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
		m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		m_pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, true);
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);

		m_pDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, false);
		m_pDevice->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, false);

		m_pDevice->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, true);

		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		m_pDevice->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_INVDESTALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ONE);
	}

	void CRender::CreateVertex(int x, int y, DWORD dxColor, PD3DTLVERTEX pVertext, DWORD index)
	{
		pVertext[index].x = (float)x;
		pVertext[index].y = (float)y;
		pVertext[index].z = 0.f;
		pVertext[index].rhw = 0.f;
		pVertext[index].dxColor = dxColor;
	}
//}

	
	