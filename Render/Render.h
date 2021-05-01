#pragma once

#include "../Engine/Engine.h"
//using namespace Engine;
//namespace Engine
//{
struct DrawData
{
	DrawData(int h, int w, int l, int u, int r, int d)
	{
		Height = h;
		Width = w;
		Left = l;
		Up = u;
		Right = r;
		Down = d;
		rec_type = 1;
	}
	DrawData(int l, int u, int r, int d)
	{
		Left = l;
		Up = u;
		Right = r;
		Down = d;
		rec_type = 2;
	}
	const DrawData Outline()
	{
		if (rec_type == 1)
		{
			return DrawData(Height, Width, Left - 1, Up - 1, Right + 1, Down + 1);
		}
		else if (rec_type == 2)
		{
			return DrawData(Left - 1, Up - 1, Right + 1, Down + 1);
		}
	}
	const DrawData Inline()
	{
		if (rec_type == 1)
		{
			return DrawData(Height, Width, Left + 1, Up + 1, Right - 1, Down - 1);
		}
		else if (rec_type == 2)
		{
			return DrawData(Left + 1, Up + 1, Right - 1, Down - 1);
		}
	}

	int Height, Width, Left, Up, Right, Down, rec_type;
};

	typedef struct D3DTLVERTEX
	{
		float x;
		float y;
		float z;
		float rhw;
		D3DCOLOR dxColor;
	} *PD3DTLVERTEX;
	
	class CRender
	{
	public:

		CRender(IDirect3DDevice9* pDevice);
		~CRender();

		void SetupFonts();


		void Clear(int x, int y, int w, int h, Color color);
		void DrawRect(int x, int y, int w, int h, Color col);
		void DrawOutlineRect(int x, int y, int w, int h, Color color);
		void DrawString(DWORD font, int x, int y, Color Color, DWORD alignment, const char * msg, ...);
		void DrawHeader(int x, int y, int w, int HealthBarWidth);

		void OutlineRainbow(int x, int y, int w, int h, float flSpeed, float &flRainbow);
		void Line(int x, int y, int x2, int y2, Color color);
		void PolyLine(int *x, int *y, int count, Color color);
		void Polygon(int count, Vertex_t* Vertexs, Color color);
		void PolygonOutline(int count, Vertex_t* Vertexs, Color color, Color colorLine);
		void PolyLine(int count, Vertex_t* Vertexs, Color colorLine);
		void DrawWave(Vector loc, float radius, Color color);
		void DrawCircle3D(Vector position, float points, float radius, Color Color);
		void OutlineCircle(int x, int y, int r, int seg, Color color);
		void Pixel(int x, int y, Color col);
		void DrawCoalBox(int x, int y, int w, int h, Color color);
		void BoxDefault(DrawData Data, Color pColor);
		void GradientV(int x, int y, int w, int h, Color c1, Color c2);
		void GradientH(int x, int y, int w, int h, Color c1, Color c2);
		void DrawFilledRect(int x1, int y1, int x2, int y2, Color color);
		void Text(int x, int y, Color color, DWORD font, const char* text, ...);
		void OutlinedBox2(int x, int y, int w, int h, Color color);
		void DrawRectFilled(int x0, int y0, int x1, int y1, Color color);
		void FillRGBA(int x, int y, int w, int h, Color c);
		void DrawCircle(float x, float y, float r, float s, Color color);
		void DrawParticles(int alpha);
		void DrawBackGround();
		void TexturedPolygon(int n, std::vector<SDK::Vertex_t> vertice, Color color);
		void DrawTexturedPoly(int n, Vertex_t * vertice, Color col);
		void Textf(int x, int y, Color color, DWORD font, const char* fmt, ...);
		void DrawFilledCircle(Vector2D center, Color color, float radius, float points);
		void Text(int x, int y, Color color, DWORD font, const wchar_t* text);
		void Text(int x, int y, DWORD font, const wchar_t* text);
		RECT GetTextSize(DWORD font, const char* text);

		// Other rendering functions
		bool WorldToScreen(Vector &in, Vector &out);
		RECT GetViewport();
		class Fonts
		{
		public:
			DWORD Default;
			DWORD DEF;
		} font;


		void OnLostDevice();
		void OnResetDevice();

		void BeginRender();
		void EndRender();

		void DrawLine(int x1, int y1, int x2, int y2, Color color);



		void Line(Vector2D start_pos, Vector2D end_pos, Color col);
		void DrawWave1(Vector loc, float radius, Color color);
		void DrawCircle(Vector2D position, float points, float radius, Color color);

		void GenuineBox(int x, int y, int w, int h, Color color);
		void GenuineOutlineBox(int x, int y, int w, int h, Color color);
		void DrawAlphaBox(int x, int y, int w, int h, Color color);
		void DrawBox(int x, int y, int w, int h, Color color);
		void DrawFillBox(int x, int y, int w, int h, Color color);
		void DrawOutlineBox(int x, int y, int w, int h, Color color);

	
		void DrawOutlineCoalBox(int x, int y, int w, int h, Color color);

		void DrawWindow(int x, int y, int w, int h, Color Border, Color Background);

		void DrawHorBar(int x, int y, int w, int h, int val, Color color1, Color color2);
		void DrawVerBar(int x, int y, int w, int h, int val, Color color1, Color color2);

		void Text(int x, int y, bool center, bool shadow, Color color, const char* format, ...);
		void TextToConsole(int x, int y, bool center, bool shadow, Color color, const char * format, ...);
		void WepIcon(int x, int y, bool center, bool shadow, Color color, const char* format, ...);

		IDirect3DDevice9*		m_pDevice;

	private:

		bool CreateObject();

		int GetTextHeight(const char * text);

		void SetVertexState();

		void CreateVertex(int x, int y, DWORD dxColor, PD3DTLVERTEX pVertext, DWORD index);

		ID3DXFont*				m_pFont;
		ID3DXFont*				m_pIconz;
		ID3DXFont*				m_pLog;
		CHudTexture*			m_pIcon;		// 0x724
		IDirect3DStateBlock9*	m_pStateBlockDraw;
		IDirect3DStateBlock9*	m_pStateBlockText;

		DWORD					dwOld_D3DRS_COLORWRITEENABLE;

	};

	namespace D3D9
	{
		enum TABLE
		{
			QueryInterface ,
			AddRef ,
			Release ,
			TestCooperativeLevel ,
			GetAvailableTextureMem ,
			EvictManagedResources ,
			GetDirect3D ,
			GetDeviceCaps ,
			GetDisplayMode ,
			GetCreationParameters ,
			SetCursorProperties ,
			SetCursorPosition ,
			ShowCursor ,
			CreateAdditionalSwapChain ,
			GetSwapChain ,
			GetNumberOfSwapChains ,
			Reset ,
			Present ,
			GetBackBuffer ,
			GetRasterStatus ,
			SetDialogBoxMode ,
			SetGammaRamp ,
			GetGammaRamp ,
			CreateTexture ,
			CreateVolumeTexture ,
			CreateCubeTexture ,
			CreateVertexBuffer ,
			CreateIndexBuffer ,
			CreateRenderTarget ,
			CreateDepthStencilSurface ,
			UpdateSurface ,
			UpdateTexture ,
			GetRenderTargetData ,
			GetFrontBufferData ,
			StretchRect ,
			ColorFill ,
			CreateOffscreenPlainSurface ,
			SetRenderTarget ,
			GetRenderTarget ,
			SetDepthStencilSurface ,
			GetDepthStencilSurface ,
			BeginScene ,
			EndScene ,
			Clear ,
			SetTransform ,
			GetTransform ,
			MultiplyTransform ,
			SetViewport ,
			GetViewport ,
			SetMaterial ,
			GetMaterial ,
			SetLight ,
			GetLight ,
			LightEnable ,
			GetLightEnable ,
			SetClipPlane ,
			GetClipPlane ,
			SetRenderState ,
			GetRenderState ,
			CreateStateBlock ,
			BeginStateBlock ,
			EndStateBlock ,
			SetClipStatus ,
			GetClipStatus ,
			GetTexture ,
			SetTexture ,
			GetTextureStageState ,
			SetTextureStageState ,
			GetSamplerState ,
			SetSamplerState ,
			ValidateDevice ,
			SetPaletteEntries ,
			GetPaletteEntries ,
			SetCurrentTexturePalette ,
			GetCurrentTexturePalette ,
			SetScissorRect ,
			GetScissorRect ,
			SetSoftwareVertexProcessing ,
			GetSoftwareVertexProcessing ,
			SetNPatchMode ,
			GetNPatchMode ,
			DrawPrimitive ,
			DrawIndexedPrimitive ,
			DrawPrimitiveUP ,
			DrawIndexedPrimitiveUP ,
			ProcessVertices ,
			CreateVertexDeclaration ,
			SetVertexDeclaration ,
			GetVertexDeclaration ,
			SetFVF ,
			GetFVF ,
			CreateVertexShader ,
			SetVertexShader ,
			GetVertexShader ,
			SetVertexShaderConstantF ,
			GetVertexShaderConstantF ,
			SetVertexShaderConstantI ,
			GetVertexShaderConstantI ,
			SetVertexShaderConstantB ,
			GetVertexShaderConstantB ,
			SetStreamSource ,
			GetStreamSource ,
			SetStreamSourceFreq ,
			GetStreamSourceFreq ,
			SetIndices ,
			GetIndices ,
			CreatePixelShader ,
			SetPixelShader ,
			GetPixelShader ,
			SetPixelShaderConstantF ,
			GetPixelShaderConstantF ,
			SetPixelShaderConstantI ,
			GetPixelShaderConstantI ,
			SetPixelShaderConstantB ,
			GetPixelShaderConstantB ,
			DrawRectPatch ,
			DrawTriPatch ,
			DeletePatch ,
			CreateQuery
		};
	}
//}
	extern CRender* Render;