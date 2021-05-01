#pragma once

#include "SDK.h"

#ifdef PlaySound
#undef PlaySound
#endif

namespace SDK
{
	namespace TABLE
	{
		namespace ISurface
		{
    enum
            {
                DrawSetColor1 = 14,
                DrawSetColor2 = 15,
                DrawFilledRect = 16,
                DrawOutlinedRect = 18,
                DrawLine = 19,
                DrawSetTextFont = 23,
                DrawSetTextColor = 24,
                DrawSetTextPos = 26,
                DrawPrintText = 28,
                UnlockCursor = 66,
                LockCursor = 67,
                FontCreate = 71,
                SetFontGlyphSet = 72,
                GetTextSize = 79,
                PlaySound = 82
            };
		}
	}

	enum FontDrawType_t
	{
		FONT_DRAW_DEFAULT = 0 ,
		FONT_DRAW_NONADDITIVE ,
		FONT_DRAW_ADDITIVE ,
		FONT_DRAW_TYPE_COUNT = 2 ,
	};

	enum EFontFlags
	{
		FONTFLAG_NONE ,
		FONTFLAG_ITALIC = 0x001 ,
		FONTFLAG_UNDERLINE = 0x002 ,
		FONTFLAG_STRIKEOUT = 0x004 ,
		FONTFLAG_SYMBOL = 0x008 ,
		FONTFLAG_ANTIALIAS = 0x010 ,
		FONTFLAG_GAUSSIANBLUR = 0x020 ,
		FONTFLAG_ROTARY = 0x040 ,
		FONTFLAG_DROPSHADOW = 0x080 ,
		FONTFLAG_ADDITIVE = 0x100 ,
		FONTFLAG_OUTLINE = 0x200 ,
		FONTFLAG_CUSTOM = 0x400 ,
		FONTFLAG_BITMAP = 0x800 ,
	};
	struct Vertex_t
	{
		Vector2D m_Position;
		Vector2D m_TexCoord;

		Vertex_t()
		{
		}

		Vertex_t(const Vector2D& pos, const Vector2D& coord = Vector2D(0, 0))
		{
			m_Position = pos;
			m_TexCoord = coord;
		}

		void Init(const Vector2D& pos, const Vector2D& coord = Vector2D(0, 0))
		{
			m_Position = pos;
			m_TexCoord = coord;
		}
	};
	typedef Vertex_t FontVertex_t;
	class ISurface
	{
	public:
		void DrawSetColor( Color col )
		{
			VirtualFn( void )( PVOID , Color );
			GetMethod< OriginalFn >( this , TABLE::ISurface::DrawSetColor1 )( this , col );
		}

		void DrawSetColor( int r , int g , int b , int a )
		{
			VirtualFn( void )( PVOID , int , int , int , int );
			GetMethod< OriginalFn >( this , TABLE::ISurface::DrawSetColor2 )( this , r , g , b , a );
		}

		void DrawFilledRect( int x0 , int y0 , int x1 , int y1 )
		{
			VirtualFn( void )( PVOID , int , int , int , int );
			GetMethod< OriginalFn >( this , TABLE::ISurface::DrawFilledRect )( this , x0 , y0 , x1 , y1 );
		}

		void DrawOutlinedRect( int x0 , int y0 , int x1 , int y1 )
		{
			VirtualFn( void )( PVOID , int , int , int , int );
			GetMethod< OriginalFn >( this , TABLE::ISurface::DrawOutlinedRect )( this , x0 , y0 , x1 , y1 );
		}

		void DrawOutlinedCircle(int x, int y, int r, int seg)
		{
			typedef void(__thiscall* oDrawOutlinedCircle)(PVOID, int, int, int, int);
			return GetMethod<oDrawOutlinedCircle>(this, 103)(this, x, y, r, seg);
		}

		void DrawSetTexture(int textureID)
		{
			typedef void(__thiscall* oDrawSetTextColor)(PVOID, int);
			return GetMethod<oDrawSetTextColor>(this, 38)(this, textureID);
		}
		int CreateNewTextureID(bool procedural)
		{
			typedef int(__thiscall* oDrawSetTextColor)(PVOID, bool);
			return GetMethod<oDrawSetTextColor>(this, 43)(this, procedural);
		}
		void DrawTexturedPolygon(int vtxCount, FontVertex_t* vtx, bool bClipVertices = true)
		{
			typedef void(__thiscall* oDrawSetTextColor)(PVOID, int, FontVertex_t*, bool);
			return GetMethod<oDrawSetTextColor>(this, 106)(this, vtxCount, vtx, bClipVertices);
		}

		void DrawPolyLine(int* x, int* y, int count)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, int*, int*, int);
			GetMethod<OriginalFn>(this, 19 + 1)(this, x, y, count);
		}

		void DrawLine( int x0 , int y0 , int x1 , int y1 )
		{
			VirtualFn( void )( PVOID , int , int , int , int );
			GetMethod< OriginalFn >( this , TABLE::ISurface::DrawLine )( this , x0 , y0 , x1 , y1 );
		}

		void DrawSetTextFont( unsigned long hFont )
		{
			VirtualFn( void )( PVOID , unsigned long );
			GetMethod< OriginalFn >( this , TABLE::ISurface::DrawSetTextFont )( this , hFont );
		}

		void DrawSetTextColor( Color col )
		{
			VirtualFn( void )( PVOID , Color );
			GetMethod< OriginalFn >( this , TABLE::ISurface::DrawSetTextColor )( this , col );
		}

		void DrawSetTextureRGBA(int textureID, unsigned char const* colors, int w, int h)
		{
			typedef void(__thiscall* oDrawSetTextColor)(PVOID, int, unsigned char const*, int, int);
			return GetMethod<oDrawSetTextColor>(this, 37)(this, textureID, colors, w, h);
		}

		void DrawSetTextPos( int x , int y )
		{
			VirtualFn( void )( PVOID , int , int );
			GetMethod< OriginalFn >( this , TABLE::ISurface::DrawSetTextPos )( this , x , y );
		}

		void DrawPrintText( wchar_t *text , int textLen , FontDrawType_t drawType = FONT_DRAW_DEFAULT )
		{
			VirtualFn( void )( PVOID , wchar_t* , int , FontDrawType_t );
			GetMethod< OriginalFn >( this , TABLE::ISurface::DrawPrintText )( this , text , textLen , drawType );
		}

		DWORD FontCreate()
		{
			VirtualFn( DWORD )( PVOID );
			return GetMethod< OriginalFn >( this , TABLE::ISurface::FontCreate )( this );
		}

		void SetFontGlyphSet( unsigned long font , const char *windowsFontName , int tall , int weight , int blur , int scanlines , int flags , int nRangeMin = 0 , int nRangeMax = 0 )
		{
			VirtualFn( void )( PVOID , unsigned long , const char * , int , int , int , int , int , int , int );
			GetMethod< OriginalFn >( this , TABLE::ISurface::SetFontGlyphSet )( this , font , windowsFontName , tall , weight , blur , scanlines , flags , nRangeMin , nRangeMax );
		}

		void GetTextSize( unsigned long font , const wchar_t *text , int &wide , int &tall )
		{
			VirtualFn( void )( PVOID , unsigned long , const wchar_t* , int& , int& );
			GetMethod< OriginalFn >( this , TABLE::ISurface::GetTextSize )( this , font , text , wide , tall );
		}

		void PlaySound( const char* pszSoundName )
		{
			VirtualFn( void )( PVOID , const char* );
			GetMethod< OriginalFn >( this , TABLE::ISurface::PlaySound )( this , pszSoundName );
		}
		void UnlockCursor()
		{
			typedef void(__thiscall* OriginalFn)(void*);
			GetMethod< OriginalFn >(this, TABLE::ISurface::UnlockCursor)(this);
		}

		void LockCursor()
		{
			typedef void(__thiscall* OriginalFn)(void*);
			GetMethod< OriginalFn >(this, TABLE::ISurface::LockCursor)(this);
		}
	};
}