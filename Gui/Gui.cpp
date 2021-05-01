#include "Gui.h"

//using namespace CClient;



bool bIsGuiInitalize = false;
bool bIsGuiVisible = false;

WNDPROC WndProc_o = nullptr;

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

LRESULT WINAPI GUI_WndProc( HWND hwnd , UINT uMsg , WPARAM wParam , LPARAM lParam );

CGui::CGui() {}


CGui::~CGui()
{
	ImGui_ImplDX9_Shutdown();
}

void CGui::GUI_Init(IDirect3DDevice9 * pDevice)
{
	HWND hWindow = FindWindowA("Valve001", 0);

	ImGui_ImplDX9_Init(hWindow, pDevice);

	ImGuiIO& io = ImGui::GetIO();

	io.IniFilename = GuiFile.c_str();
	ImFontConfig font_config;
	font_config.OversampleH = 1; 
	font_config.OversampleV = 1;
	font_config.PixelSnapH = 1;

	static const ImWchar ranges[] =
	{
		0x0020, 0x00FF, 
		0x0400, 0x044F, 
		0,
	};
	MainFontBigT = io.Fonts->AddFontFromMemoryCompressedTTF(MainFont_compressed_data, MainFont_compressed_size, 25.f, &font_config, ranges);
	main = io.Fonts->AddFontFromMemoryCompressedTTF(MainFont_compressed_data, MainFont_compressed_size, 18.f, &font_config, ranges);
	title = io.Fonts->AddFontFromMemoryCompressedTTF(TitleFont_compressed_data, TitleFont_compressed_size, 35.f, &font_config, ranges);
	weap = io.Fonts->AddFontFromMemoryCompressedTTF(WeaponFont_compressed_data, WeaponFont_compressed_size, 18.f, &font_config, ranges);


	Default();

	ImGui_ImplDX9_CreateDeviceObjects();

	WndProc_o = (WNDPROC)SetWindowLongA(hWindow, GWL_WNDPROC, (LONG)(LONG_PTR)GUI_WndProc);

	bIsGuiInitalize = true;
}

void CGui::GUI_Begin_Render()
{
	ImGui_ImplDX9_NewFrame();
}

void CGui::GUI_End_Render()
{
	ImGui::Render();
}

LRESULT WINAPI GUI_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static bool bDown = false;
	static bool bClicked = false;
	static bool bPrevMenuState = bIsGuiVisible;
	static ConVar * men = Interfaces::GetConVar()->FindVar("cl_mouseenable");
	if (uMsg == WM_KEYDOWN && wParam == VK_INSERT)
		bIsGuiVisible = !bIsGuiVisible;

	if (bClicked)
	{
		bIsGuiVisible = !bIsGuiVisible;
		men->SetValue(bIsGuiVisible);
	}

	if (bPrevMenuState != bIsGuiVisible)
	{
		men->SetValue(!bIsGuiVisible);
	}
	bPrevMenuState = bIsGuiVisible;

	if (bIsGuiVisible && ImGui_ImplDX9_WndProcHandler(hwnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProcA(WndProc_o, hwnd, uMsg, wParam, lParam);
}


void CGui::GUI_Draw_Elements()
{
	if (!bIsGuiInitalize )//|| Interfaces::Engine()->IsTakingScreenshot() || !Interfaces::Engine()->IsActiveApp())
		return;

	Gui->GUI_Begin_Render();

	bool bOpenTimer = (bIsGuiVisible || (bC4Timer && iC4Timer));

	if (settings.Misc.misc_Spectators) 
		Misc->OnRenderSpectatorList();

	if (Esp && settings.Visual.esp_BombTimer && bOpenTimer )
	{
		ImVec2 OldMinSize = ImGui::GetStyle().WindowMinSize;

		ImGui::GetStyle().WindowMinSize = ImVec2( 0.f , 0.f );

		ImGui::SetNextWindowSize( ImVec2( 125.f , 30.f ) );
		
		ImGui::GetStyle().WindowMinSize = OldMinSize;
	}

	if ( bIsGuiVisible )
	{
		int pX , pY;
		Interfaces::InputSystem()->GetCursorPosition( &pX , &pY );

		ImGui::GetIO().MousePos.x = (float)pX;
		ImGui::GetIO().MousePos.y = (float)pY;

		Client->OnRenderGUI();
	}
	



	Gui->GUI_End_Render();
}


void CGui::TitileFont()
{
	ImGui::PushFont(title);
}

void CCmain()
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.Alpha = 1.f;
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.FramePadding = ImVec2(4, 2);
	style.ItemSpacing = ImVec2(0, 3);
	//style.ItemInnerSpacing = ImVec2(0, 0);
	style.WindowRounding = 0.f;
	style.FrameRounding = 0.0f;
	style.ColumnsMinSpacing = 50.0f;
	style.GrabMinSize = 14.0f;
	style.GrabRounding = 10.f;
	style.ScrollbarSize = 12.0f;
	style.ScrollbarRounding = 10.0f;
	style.AntiAliasedLines = true;
	style.AntiAliasedFill = true;

	style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.81f, 0.81f, 0.81f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.70f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.90f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.27f, 0.27f, 0.27f, 0.65f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.30f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.40f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 0.45f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.16f, 0.16f, 0.16f, 0.83f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.20f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.28f, 0.28f, 0.28f, 0.87f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.16f, 0.16f, 0.16f, 0.80f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.31f, 0.31f, 0.31f, 0.60f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.65f, 0.65f, 0.65f, 0.30f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.70f, 0.70f, 0.70f, 0.40f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.40f);
	
	style.Colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.24f, 0.24f, 0.30f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.67f, 0.67f, 0.67f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.44f, 0.44f, 0.44f, 0.60f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.62f, 0.62f, 0.62f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.29f, 0.29f, 0.29f, 0.45f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.80f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.73f, 0.73f, 0.73f, 0.80f);
	style.Colors[ImGuiCol_Column] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.42f, 0.42f, 0.42f, 1.00f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.57f, 0.57f, 0.57f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.50f, 0.50f, 0.90f, 0.50f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.70f, 0.70f, 0.90f, 0.60f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
};


void CGui::Default()
{
  CCmain();
  settings.Misc.Blackout = true;
}
/*
void CGui::Default1()
{
	black();
	settings.Misc.Blackout = true;
}
*/
void CGui::MainFont1()
{
	ImGui::PushFont(main);
}
void CGui::MainFontBig()
{
	ImGui::PushFont(MainFontBigT);
}
void CGui::weap1()
{
	ImGui::PushFont(weap);
}
void CGui::MainFont()
{
	
	ImGuiStyle& style = ImGui::GetStyle();

	static int hue = 140;
	ImVec4 col_text = ImColor::HSV(hue / 255.f, 20.f / 255.f, 235.f / 255.f);
	ImVec4 col_theme = ImColor(17, 17, 17);
	ImVec4 col_main = ImColor(9, 82, 128);
	ImVec4 col_back = ImColor(35, 35, 35);
	ImVec4 col_area = ImColor(4, 32, 41);

	
}

namespace ImGui
{
	static auto vector_getter = []( void* vec , int idx , const char** out_text )
	{
		auto& vector = *static_cast<std::vector<std::string>*>( vec );
		if ( idx < 0 || idx >= static_cast<int>( vector.size() ) ) { return false; }
		*out_text = vector.at( idx ).c_str();
		return true;
	};

	IMGUI_API bool ListBoxConfigArray(const char* label, int* currIndex, std::vector<std::string>& values)
	{
		return ListBox(label, currIndex, vector_getter,
			static_cast<void*>(&values), values.size(), 10);
	}

	IMGUI_API bool ComboBoxArray( const char* label , int* currIndex , std::vector<std::string>& values )
	{
		if ( values.empty() ) { return false; }
		return Combo( label , currIndex , vector_getter ,
					  static_cast<void*>( &values ) , values.size() );
	}

	IMGUI_API bool TabLabels( const char **tabLabels , int tabSize , int &tabIndex , int *tabOrder )
	{
		ImGuiStyle& style = ImGui::GetStyle();

		const ImVec2 itemSpacing = style.ItemSpacing;
		const ImVec4 color = style.Colors[ImGuiCol_Button];
		const ImVec4 colorActive = style.Colors[ImGuiCol_ButtonActive];
		const ImVec4 colorHover = style.Colors[ImGuiCol_ButtonHovered];
		const ImVec4 colorText = style.Colors[ImGuiCol_Text];
		style.ItemSpacing.x = 1;
		style.ItemSpacing.y = 1;
		const ImVec4 colorSelectedTab = ImVec4( color.x , color.y , color.z , color.w*0.5f );
		const ImVec4 colorSelectedTabHovered = ImVec4( colorHover.x , colorHover.y , colorHover.z , colorHover.w*0.5f );
		const ImVec4 colorSelectedTabText = ImVec4( colorText.x*0.8f , colorText.y*0.8f , colorText.z*0.8f , colorText.w*0.8f );

		if ( tabSize>0 && ( tabIndex<0 || tabIndex >= tabSize ) )
		{
			if ( !tabOrder )
			{
				tabIndex = 0;
			}
			else
			{
				tabIndex = -1;
			}
		}

		float windowWidth = 0.f , sumX = 0.f;
		windowWidth = ImGui::GetWindowWidth() - style.WindowPadding.x - ( ImGui::GetScrollMaxY()>0 ? style.ScrollbarSize : 0.f );

		static int draggingTabIndex = -1; int draggingTabTargetIndex = -1;
		static ImVec2 draggingtabSize( 0 , 0 );
		static ImVec2 draggingTabOffset( 0 , 0 );

		const bool isMMBreleased = ImGui::IsMouseReleased( 2 );
		const bool isMouseDragging = ImGui::IsMouseDragging( 0 , 2.f );
		int justClosedTabIndex = -1 , newtabIndex = tabIndex;

		bool selection_changed = false; bool noButtonDrawn = true;

		for ( int j = 0 , i; j < tabSize; j++ )
		{
			i = tabOrder ? tabOrder[j] : j;
			if ( i == -1 ) continue;

			if ( sumX > 0.f )
			{
				sumX += style.ItemSpacing.x;
				sumX += ImGui::CalcTextSize( tabLabels[i] ).x + 2.f*style.FramePadding.x;
				
				if ( sumX>windowWidth )
				{
					sumX = 0.f;
				}
				else
				{
					ImGui::SameLine();
				}
			}

			if ( i != tabIndex )
			{
				// Push the style
				style.Colors[ImGuiCol_Button] = colorSelectedTab;
				style.Colors[ImGuiCol_ButtonActive] = colorSelectedTab;
				style.Colors[ImGuiCol_ButtonHovered] = colorSelectedTabHovered;
				style.Colors[ImGuiCol_Text] = colorSelectedTabText;
			}
			// Draw the button
			ImGui::PushID( i );   // otherwise two tabs with the same name would clash.
			
				if (ImGui::Button(tabLabels[i],  ImVec2(135.f, 50.f))) { selection_changed = (tabIndex != i); newtabIndex = i; } //216, 50 //165,30 //tab change
			
			ImGui::PopID();
			if ( i != tabIndex )
			{
				// Reset the style
				style.Colors[ImGuiCol_Button] = color;
				style.Colors[ImGuiCol_ButtonActive] = colorActive;
				style.Colors[ImGuiCol_ButtonHovered] = colorHover;
				style.Colors[ImGuiCol_Text] = colorText;
			}
			noButtonDrawn = false;

			if ( sumX == 0.f ) sumX = style.WindowPadding.x + ImGui::GetItemRectSize().x; // First element of a line

			if ( ImGui::IsItemHoveredRect() )
			{
				if ( tabOrder )
				{
					// tab reordering
					if ( isMouseDragging )
					{
						if ( draggingTabIndex == -1 )
						{
							draggingTabIndex = j;
							draggingtabSize = ImGui::GetItemRectSize();
							const ImVec2& mp = ImGui::GetIO().MousePos;
							const ImVec2 draggingTabCursorPos = ImGui::GetCursorPos();
							draggingTabOffset = ImVec2(
								mp.x + draggingtabSize.x*0.5f - sumX + ImGui::GetScrollX() ,
								mp.y + draggingtabSize.y*0.5f - draggingTabCursorPos.y + ImGui::GetScrollY()
							);

						}
					}
					else if ( draggingTabIndex >= 0 && draggingTabIndex<tabSize && draggingTabIndex != j )
					{
						draggingTabTargetIndex = j; // For some odd reasons this seems to get called only when draggingTabIndex < i ! (Probably during mouse dragging ImGui owns the mouse someway and sometimes ImGui::IsItemHovered() is not getting called)
					}
				}
			}

		}

		tabIndex = newtabIndex;
		
		// Reset draggingTabIndex if necessary
		if ( !isMouseDragging ) draggingTabIndex = -1;

		// Change selected tab when user closes the selected tab
		if ( tabIndex == justClosedTabIndex && tabIndex >= 0 )
		{
			tabIndex = -1;
			for ( int j = 0 , i; j < tabSize; j++ )
			{
				i = tabOrder ? tabOrder[j] : j;
				if ( i == -1 )
				{
					continue;
				}
				tabIndex = i;
				break;
			}
		}

		// Restore the style
		style.Colors[ImGuiCol_Button] = color;
		style.Colors[ImGuiCol_ButtonActive] = colorActive;
		style.Colors[ImGuiCol_ButtonHovered] = colorHover;
		style.Colors[ImGuiCol_Text] = colorText;
		style.ItemSpacing = itemSpacing;

		return selection_changed;
	}
}

CGui* Gui = new CGui;