#ifndef __IPLUGGUILIVEEDIT__
#define __IPLUGGUILIVEEDIT__

#include "IControl.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

/*
E - Toggle edit mode
I - Toggle info

By Youlean
*/


#define GUI_EDIT_START 0x00000001
#define GUI_EDIT_FINISH 0x00000002

#define NEW_LAYER 0x00000003
#define END 0x00000004

#define DRAW_RECT(L, T, R, B) IRECT(L, T, R, B)

using namespace std;

class IPlugGUILiveEdit : public IControl
{
public:
	IPlugGUILiveEdit(IPlugBase* pPlug, const char* _pathToSource, int _gridSize, bool editOnStart = true)
		: IControl(pPlug, IRECT(0, 0, 1, 1))
	{
		mTargetRECT = mDrawRECT;
		pathToSource = _pathToSource;
		gridSize = _gridSize;

		if (!editOnStart) editModeActive = false;
	}

	~IPlugGUILiveEdit() {}

	void OnMouseDown(int x, int y, IMouseMod* pMod)
	{
		clickedOnControl = ControlIndexContaining(x, y);

		if (clickedOnControl == -1) return;

		mouseDownDrawRECT = selectedDrawRECT = *mGraphics->GetControl(clickedOnControl)->GetDrawRECT();
		mouseDownTargetRECT = selectedTargetRECT = *mGraphics->GetControl(clickedOnControl)->GetTargetRECT();


		if (IsMouseOverHandle(x, y))
		{
			mouseClickedOnResizeHandle = true;
			// Change mouse cursor to better represent control resizing
			mGraphics->SetMouseCursor(IGraphics::ECursor::SIZENWSE);
		}

		mouseDownX = x;
		mouseDownY = y;
	}

	void OnMouseUp(int x, int y, IMouseMod* pMod)
	{
		mouseIsDragging = false;
		mouseClickedOnResizeHandle = false;
		// Return to normal mouse cursor
		mGraphics->SetMouseCursor(IGraphics::ECursor::ARROW);
	}

	void OnMouseOver(int x, int y, IMouseMod* pMod)
	{
		// Change mouse cursor to better represent control resizing
		if (IsMouseOverHandle(x, y)) 	mGraphics->SetMouseCursor(IGraphics::ECursor::SIZENWSE);
		else if (!mouseClickedOnResizeHandle) 	mGraphics->SetMouseCursor(IGraphics::ECursor::ARROW);
	}

	void OnMouseDrag(int x, int y, int dX, int dY, IMouseMod* pMod)
	{
		if (!foundError)
		{
			mouseIsDragging = true;

			if (mouseClickedOnResizeHandle)
			{
				ResizeSelectedControl(x, y);
			}
			else if (pMod->L && clickedOnControl > 0)
			{
				MoveSelectedControl(x, y); // Move controls, but don't move background
			}

			if (clickedOnControl > -1)
			{
				selectedDrawRECT = *mGraphics->GetControl(clickedOnControl)->GetDrawRECT();
				selectedTargetRECT = *mGraphics->GetControl(clickedOnControl)->GetTargetRECT();
			}
		}
	}

	IRECT RescaleIRECT(IRECT *old_IRECT, double ratio)
	{
		return IRECT(int(old_IRECT->L * ratio), int(old_IRECT->T * ratio), int(old_IRECT->R * ratio), int(old_IRECT->B * ratio));
	}

	void ResizeSelectedControl(int x, int y)
	{

		IRECT *pSelectedDrawRECT = mGraphics->GetControl(clickedOnControl)->GetDrawRECT();
		DRECT *pSelectedNonScaledDrawRECT = mGraphics->GetControl(clickedOnControl)->GetNonScaledDrawRECT();
		IRECT *pSelectedTargetRECT = mGraphics->GetControl(clickedOnControl)->GetTargetRECT();

		pSelectedDrawRECT->R = SnapToGrid(mouseDownDrawRECT.R + (x - mouseDownX));
		pSelectedDrawRECT->B = SnapToGrid(mouseDownDrawRECT.B + (y - mouseDownY));

		*pSelectedTargetRECT = *pSelectedDrawRECT;

		if (GetGUIResize())
		{
			*pSelectedNonScaledDrawRECT = RescaleIRECT(pSelectedDrawRECT, GetGUIResize()->GetGUIScaleRatio());
		}
	}

	void MoveSelectedControl(int x, int y)
	{

		IRECT *pSelectedDrawRECT = mGraphics->GetControl(clickedOnControl)->GetDrawRECT();
		DRECT *pSelectedNonScaledDrawRECT = mGraphics->GetControl(clickedOnControl)->GetNonScaledDrawRECT();
		IRECT *pSelectedTargetRECT = mGraphics->GetControl(clickedOnControl)->GetTargetRECT();

		pSelectedDrawRECT->L = SnapToGrid(mouseDownDrawRECT.L + (x - mouseDownX));
		pSelectedDrawRECT->T = SnapToGrid(mouseDownDrawRECT.T + (y - mouseDownY));
		pSelectedDrawRECT->R = pSelectedDrawRECT->L + mouseDownDrawRECT.W();
		pSelectedDrawRECT->B = pSelectedDrawRECT->T + mouseDownDrawRECT.H();

		*pSelectedTargetRECT = *pSelectedDrawRECT;

		if (GetGUIResize())
		{
			*pSelectedNonScaledDrawRECT = RescaleIRECT(pSelectedDrawRECT, GetGUIResize()->GetGUIScaleRatio());
		}
	}

	int SnapToGrid(int in)
	{
		if (gridSize > 1) return (in / gridSize) * gridSize;
		else return in;
	}

	int ControlIndexContaining(int x, int y)
	{
		for (int i = GetNumberOfControls() - 1; i > 0; i--)
		{
			// Skip hidden controls is needed
			if (!drawHiddenControl && mGraphics->GetControl(i)->IsHidden()) continue;

			// We are including resize handle here
			IRECT controlRECT = *mGraphics->GetControl(i)->GetDrawRECT();

			if (controlRECT.Contains(x, y))
				return i;
		}

		return -1;
	}

	int GetNumberOfControls()
	{
		if (mPlug->GetGUIResize()) return mGraphics->GetNControls() - 4; // Exclude GUI Resize controls and this control
		else return mGraphics->GetNControls() - 1; // Exclude this control
	}

	void ResizeDrawRectToWindowSize()
	{
		mTargetRECT = mDrawRECT = IRECT(0, 0, mGraphics->Width(), mGraphics->Height());
	}

	void DrawGrid()
	{
		if (gridSize > 1)
		{
			// Vertical Lines grid
			for (int i = 0; i < mGraphics->Width(); i += gridSize)
			{
				LICE_Line(mGraphics->GetDrawBitmap(), i, 0, i, mGraphics->Height(),
					LICE_RGBA(DRAW_RECT_COLOR.R, DRAW_RECT_COLOR.G, DRAW_RECT_COLOR.B, DRAW_RECT_COLOR.A), 0.17f);
			}

			// Horizontal Lines grid
			for (int i = 0; i < mGraphics->Height(); i += gridSize)
			{
				LICE_Line(mGraphics->GetDrawBitmap(), 0, i, mGraphics->Width(), i,
					LICE_RGBA(DRAW_RECT_COLOR.R, DRAW_RECT_COLOR.G, DRAW_RECT_COLOR.B, DRAW_RECT_COLOR.A), 0.17f);
			}
		}
		else
		{
			LICE_FillRect(mGraphics->GetDrawBitmap(), 0, 0, mGraphics->Width(), mGraphics->Height(),
				LICE_RGBA(DRAW_RECT_COLOR.R, DRAW_RECT_COLOR.G, DRAW_RECT_COLOR.B, DRAW_RECT_COLOR.A), 0.11f);
		}
	}

	void DrawRect()
	{
		for (int i = 1; i < GetNumberOfControls(); i++)
		{
			// Skip hidden controls is needed
			if (!drawHiddenControl && mGraphics->GetControl(i)->IsHidden()) continue;

			// Skip selected control
			if (clickedOnControl == i) continue;

			IRECT *drawRect = mGraphics->GetControl(i)->GetDrawRECT();
			// T
			LICE_DashedLine(mGraphics->GetDrawBitmap(), drawRect->L, drawRect->T, drawRect->R, drawRect->T, 2, 2,
				LICE_RGBA(DRAW_RECT_COLOR.R, DRAW_RECT_COLOR.G, DRAW_RECT_COLOR.B, DRAW_RECT_COLOR.A));
			//B
			LICE_DashedLine(mGraphics->GetDrawBitmap(), drawRect->L, drawRect->B, drawRect->R, drawRect->B, 2, 2,
				LICE_RGBA(DRAW_RECT_COLOR.R, DRAW_RECT_COLOR.G, DRAW_RECT_COLOR.B, DRAW_RECT_COLOR.A));
			//L
			LICE_DashedLine(mGraphics->GetDrawBitmap(), drawRect->L, drawRect->T, drawRect->L, drawRect->B, 2, 2,
				LICE_RGBA(DRAW_RECT_COLOR.R, DRAW_RECT_COLOR.G, DRAW_RECT_COLOR.B, DRAW_RECT_COLOR.A));
			//R
			LICE_DashedLine(mGraphics->GetDrawBitmap(), drawRect->R, drawRect->T, drawRect->R, drawRect->B, 2, 2,
				LICE_RGBA(DRAW_RECT_COLOR.R, DRAW_RECT_COLOR.G, DRAW_RECT_COLOR.B, DRAW_RECT_COLOR.A));
		}
	}

	void OutlineSelectedControl()
	{
		if (clickedOnControl > -1)
		{
			// Skip hidden controls is needed
			if (!drawHiddenControl && mGraphics->GetControl(clickedOnControl)->IsHidden()) return;

			IRECT *drawRect = mGraphics->GetControl(clickedOnControl)->GetDrawRECT();
			mGraphics->DrawRect(&DRAW_RECT_COLOR, drawRect);
		}
	}

	IControl* GetSelectedControl()
	{
		return mGraphics->GetControl(clickedOnControl);
	}

	void GetSelectedClassName(WDL_String *derivedName)
	{
		IControl *selectedControl = GetSelectedControl();
		derivedName->SetFormatted(128, "%s", selectedControl->GetDerivedClassName());
	}

	void DrawSelectedControlInfo()
	{
		if (clickedOnControl > 0)
		{
			// Skip hidden controls is needed
			if (!drawHiddenControl && mGraphics->GetControl(clickedOnControl)->IsHidden()) return;

			WDL_String derivedNameText;
			GetSelectedClassName(&derivedNameText);

			WDL_String layerNumberText;
			layerNumberText.SetFormatted(100, "Layer number = %i", clickedOnControl);

			WDL_String selectedDrawRECTText;
			selectedDrawRECTText.SetFormatted(100, "Draw IRECT(%i,%i,%i,%i)", selectedDrawRECT.L, selectedDrawRECT.T, selectedDrawRECT.R, selectedDrawRECT.B);

			WDL_String selectedTargetRECTText;
			selectedTargetRECTText.SetFormatted(100, "Target IRECT(%i,%i,%i,%i)", selectedTargetRECT.L, selectedTargetRECT.T, selectedTargetRECT.R, selectedTargetRECT.B);


			IRECT textMeasureRect;
			int textWidth = 0;
			int textHeight = 0;
			IText textStyle(TEXT_SIZE, &TEXT_COLOR);
			textStyle.mAlign = IText::kAlignNear;

			// Measure 4 rows of text
			mGraphics->MeasureIText(&textStyle, derivedNameText.Get(), &textMeasureRect);
			textWidth = IPMAX(textWidth, textMeasureRect.W());
			textHeight = IPMAX(textHeight, textMeasureRect.H());

			mGraphics->MeasureIText(&textStyle, layerNumberText.Get(), &textMeasureRect);
			textWidth = IPMAX(textWidth, textMeasureRect.W());
			textHeight = IPMAX(textHeight, textMeasureRect.H());

			mGraphics->MeasureIText(&textStyle, selectedDrawRECTText.Get(), &textMeasureRect);
			textWidth = IPMAX(textWidth, textMeasureRect.W());
			textHeight = IPMAX(textHeight, textMeasureRect.H());

			mGraphics->MeasureIText(&textStyle, selectedTargetRECTText.Get(), &textMeasureRect);
			textWidth = IPMAX(textWidth, textMeasureRect.W());
			textHeight = IPMAX(textHeight, textMeasureRect.H());

			IRECT textDrawRect(selectedDrawRECT.L, selectedDrawRECT.T - textHeight * 4, selectedDrawRECT.L + textWidth, selectedDrawRECT.T);

			// Move text below control
			if (textDrawRect.T < 0)
			{
				textDrawRect.T = selectedDrawRECT.B;
				textDrawRect.B = selectedDrawRECT.B + textHeight * 4;
			}

			// Move text left
			if (textDrawRect.R > mGraphics->Width())
			{
				textDrawRect.L -= textDrawRect.R - mGraphics->Width();
				textDrawRect.R = mGraphics->Width();
			}

			// Move text right
			if (textDrawRect.L < 0)
			{
				textDrawRect.L = 0;
				textDrawRect.R = textWidth;
			}

			// Draw text background
			mGraphics->FillIRect(&TEXT_BACKGROUND, &textDrawRect);

			// Draw Text
			mGraphics->DrawIText(&textStyle, derivedNameText.Get(), &textDrawRect);
			textDrawRect.T += textHeight;
			mGraphics->DrawIText(&textStyle, layerNumberText.Get(), &textDrawRect);
			textDrawRect.T += textHeight;
			mGraphics->DrawIText(&textStyle, selectedDrawRECTText.Get(), &textDrawRect);
			textDrawRect.T += textHeight;
			mGraphics->DrawIText(&textStyle, selectedTargetRECTText.Get(), &textDrawRect);
		}
	}

	void DrawErrorMessageText()
	{
		if (clickedOnControl > 0)
		{
			IText textStyle(18, &COLOR_RED);
			textStyle.mAlign = IText::kAlignCenter;

			IRECT errorRECT = mDrawRECT;
			errorRECT.T = errorRECT.T + errorRECT.W() / 2;
			// Draw text background
			mGraphics->FillIRect(&COLOR_BLACK, &mDrawRECT);

			// Draw Text
			mGraphics->DrawIText(&textStyle, errorMessage.Get(), &errorRECT);
		}
	}

	void DrawResizeHandles()
	{
		for (int i = GetNumberOfControls() - 1; i > -1; i--)
		{
			// Skip hidden controls is needed
			if (!drawHiddenControl && mGraphics->GetControl(i)->IsHidden()) continue;

			IRECT *drawRect = mGraphics->GetControl(i)->GetDrawRECT();

			IRECT handle = IRECT(drawRect->R - RESIZE_HANDLE_SIZE, drawRect->B - RESIZE_HANDLE_SIZE, drawRect->R, drawRect->B);
			mGraphics->FillTriangle(&DRAW_RECT_COLOR, handle.L, handle.B, handle.R, handle.B, handle.R, handle.T, 0);
		}
	}

	bool IsMouseOverHandle(int x, int y)
	{
		for (int i = GetNumberOfControls() - 1; i > -1; i--)
		{
			// Skip hidden controls is needed
			if (!drawHiddenControl && mGraphics->GetControl(i)->IsHidden()) continue;

			IRECT *drawRect = mGraphics->GetControl(i)->GetDrawRECT();

			IRECT handle = IRECT(drawRect->R - RESIZE_HANDLE_SIZE, drawRect->B - RESIZE_HANDLE_SIZE, drawRect->R, drawRect->B);

			if (drawRect->Contains(x, y) && handle.Contains(x, y)) return true;
			else if (drawRect->Contains(x, y) && !handle.Contains(x, y)) return false;
		}

		return false;
	}

	void ReadSourceFile(string* outString, const char* filePath)
	{
		outString->resize(0);

		string line;
		ifstream myfile(filePath);
		if (myfile.is_open())
		{
			while (getline(myfile, line))
			{
				outString->append(line);
				outString->append("\n");
			}
			myfile.close();
		}
	}

	void WriteToSourceFile(const char* data, const char* filePath)
	{
		ofstream myfile;
		myfile.open(filePath);
		if (myfile.is_open())
		{
			myfile << data;
			myfile.close();
		}
	}

	void ErrorHandling(char* _errorMessage)
	{
		errorMessage.Set(_errorMessage);
		foundError = true;
	}

	bool FindIfLineIsCommentedOut(string *code, size_t startPosition, size_t endPosition, size_t layerPosition)
	{
		// TODO: Add ability to check multi line comments

		size_t newLinePosition = layerPosition;

		while (newLinePosition > startPosition)
		{
			char c = (*code)[newLinePosition];

			if (c == '\n' || c == '\r') break;

			newLinePosition--;
		}

		string before_NEW_LAYER = code->substr(newLinePosition, layerPosition - newLinePosition);

		size_t commentPostion = before_NEW_LAYER.find("//");

		if (commentPostion != string::npos)
			return true;
		else 
			return false;
	}

	void PrepareSourceCode()
	{
		size_t sourceCodeSize = sourceCode.size();
		size_t positionOfGUIEditStart = sourceCode.find("GUI_EDIT_START");
		size_t positionOfGUIEditEnd = sourceCode.find("GUI_EDIT_FINISH");

		if (positionOfGUIEditStart > sourceCodeSize) ErrorHandling("GUI_EDIT_START not found!!!");
		if (positionOfGUIEditEnd > sourceCodeSize) ErrorHandling("GUI_EDIT_FINISH not found!!!");

		// Find number of occurrence
		string findLayerStart = "NEW_LAYER";
		string findLayerEnd = "END";
		size_t startIndex = positionOfGUIEditStart;
		int layerNumber = numberOfLayersInCode = 0;
		layerProperties.resize(0);

		while (true)
		{
			size_t layerStartPosition = sourceCode.find(findLayerStart, startIndex);

			// Find if there is a matching end after layer start
			size_t layerEndPosition = sourceCode.find(findLayerEnd, layerStartPosition);
			size_t layerStartPositionNext = sourceCode.find(findLayerStart, layerEndPosition + findLayerEnd.size());
			
			if (layerStartPosition <= positionOfGUIEditEnd)
			{
				layerNumber++;
				numberOfLayersInCode = layerNumber;

				layer tpmLayerProperties;
				tpmLayerProperties.layerStartPosition = layerStartPosition;
				tpmLayerProperties.layerEndPosition = layerEndPosition;

				layerProperties.push_back(tpmLayerProperties);
				
				if ((layerEndPosition > layerStartPositionNext) || (layerEndPosition > positionOfGUIEditEnd))
				{
					WDL_String error;
					error.SetFormatted(100, "Layer %i don't have matching END!!!", layerNumber);
					ErrorHandling(error.Get());
					return;
				}
			}
			else break;

			startIndex = layerStartPosition + findLayerStart.size();
		}

		for (size_t i = 0; i < layerProperties.size();)
		{
			if (FindIfLineIsCommentedOut(&sourceCode, positionOfGUIEditStart, positionOfGUIEditEnd, layerProperties[i].layerStartPosition))
			{
				layerProperties.erase(layerProperties.begin() + i);
				numberOfLayersInCode--;
			}
			else i++;
		}
	}

	string GetLayerAt(int layerIndex)
	{
		int controlIndex = layerIndex - 1; // -1 because we have not included a background
		size_t layerSize = layerProperties[controlIndex].layerEndPosition - layerProperties[controlIndex].layerStartPosition;
		return sourceCode.substr(layerProperties[controlIndex].layerStartPosition, layerSize);
	}

	void SetLayerAt(int layerIndex, const char* layerText)
	{
		int controlIndex = layerIndex - 1; // -1 because we have not included a background
		size_t layerSize = layerProperties[controlIndex].layerEndPosition - layerProperties[controlIndex].layerStartPosition;
		sourceCode.erase(layerProperties[controlIndex].layerStartPosition, layerSize);

		sourceCode.insert(layerProperties[controlIndex].layerStartPosition, layerText);
	}

	void UpdateDrawRECT(int layerIndex)
	{
		string findDrawRectText = "DRAW_RECT(";
		string layerText = GetLayerAt(layerIndex);

		size_t drawRectStartPosition = layerText.find(findDrawRectText);

		// Allow DRAW_RECT not to be declared everywhere. Just skip this fiction if DRAW_RECT is not in the layer
		if (drawRectStartPosition > layerText.size()) return;

		// Get opening bracket position
		drawRectStartPosition += findDrawRectText.size();

		// Find draw rect end position
		size_t drawRectEndPosition = drawRectStartPosition;
		int numberOfClosingBrackets = 1; // Handle if there are brackets inside DRAW_RECT()

		while (numberOfClosingBrackets > 0)
		{
			drawRectEndPosition++;

			if (drawRectEndPosition > layerText.size())
			{
				ErrorHandling("Closing bracket after DRAW_RECT not found!!!");
				return;
			}

			char layerTextAt = layerText[drawRectEndPosition];

			if (layerTextAt == ')') numberOfClosingBrackets--;
			if (layerTextAt == '(') numberOfClosingBrackets++;
		}
		
		layerText.erase(drawRectStartPosition, drawRectEndPosition - drawRectStartPosition);

		WDL_String drawRectValues;
		drawRectValues.SetFormatted(128, "%i, %i, %i, %i", selectedDrawRECT.L, selectedDrawRECT.T, selectedDrawRECT.R, selectedDrawRECT.B);

		layerText.insert(drawRectStartPosition, drawRectValues.Get());

		SetLayerAt(layerIndex, layerText.c_str());
	}

	void UpdateSourceCode()
	{
		ReadSourceFile(&sourceCode, pathToSource);
		PrepareSourceCode();

		int numberOfControls = GetNumberOfControls() - 1;

		if (numberOfLayersInCode != numberOfControls)
		{
			ErrorHandling("Some layer declaration is missing!!!");
			return;
		}

		if (clickedOnControl > 0 && mouseIsDragging) UpdateDrawRECT(clickedOnControl);
		
		WriteToSourceFile(sourceCode.c_str(), pathToSource);
	}

	void UpdateGUIResize()
	{
		// Update current view mode
		if (clickedOnControl > 0 && mouseIsDragging)
		{
			IControl *pControl = GetSelectedControl();
			mPlug->GetGUIResize()->LiveEditSetLayout(mPlug->GetGUIResize()->GetViewMode(), clickedOnControl, clickedOnControl, selectedDrawRECT, selectedTargetRECT, pControl->IsHidden());
		}
	}

	bool OnGlobalKeyDown(int x, int y, int key) 
	{ 
		if (key == 19 && editModeActive == false) editModeActive = true;
		else if (key == 19) editModeActive = false;

		if (key == 23 && showInfo == false) showInfo = true;
		else if (key == 23) showInfo = false;

		return true; 
	}

	bool Draw(IGraphics* pGraphics)
	{
		mGraphics = pGraphics;
		mGraphics->HandleMouseOver(true);

		IText textStyle(14, &COLOR_WHITE);

		IRECT warningRECT = IRECT();
		warningRECT.B = 20;
		warningRECT.R = 100;

		// Draw text background
		//mGraphics->FillIRect(&COLOR_BLACK, &warningRECT);

		// Draw Text
		mGraphics->DrawIText(&textStyle, "live resizing", &warningRECT);


		ResizeDrawRectToWindowSize();

		// If edit mode is disabled.
		if (editModeActive == false)
		{
			mTargetRECT = IRECT(0, 0, 0, 0);
			return true;
		}

		DrawRect();
		DrawResizeHandles();
		OutlineSelectedControl();

		if (showInfo) DrawSelectedControlInfo();

		DrawGrid();

		if (!foundError)
		{
			if (clickedOnControl > 0 || mouseClickedOnResizeHandle) UpdateSourceCode();

			if (mPlug->GetGUIResize()) UpdateGUIResize();

			mGraphics->SetAllControlsDirty();
		}
		else
		{
			DrawErrorMessageText();
		}
        
		return true;
	}

	bool IsDirty() { return true; }

private:
	bool editModeActive = true;
	bool showInfo = false;


	IGraphics* mGraphics;
	const char* pathToSource;

	WDL_String errorMessage;
	bool foundError = false;

	IColor DRAW_RECT_COLOR = IColor(255, 255, 255, 255);
	IColor TEXT_COLOR = IColor(255, 255, 255, 255);
	IColor TEXT_BACKGROUND = IColor(150, 0, 0, 0);
	int TEXT_SIZE = 15;
	int RESIZE_HANDLE_SIZE = 10;

	IRECT selectedDrawRECT = IRECT(0, 0, 0, 0);
	IRECT selectedTargetRECT = IRECT(0, 0, 0, 0);
	IRECT mouseDownDrawRECT = IRECT(0, 0, 0, 0);
	IRECT mouseDownTargetRECT = IRECT(0, 0, 0, 0);

	int mouseDownX = 0;
	int mouseDownY = 0;

	bool liveEditingEnabled = false;
	bool mouseClickedOnResizeHandle = false;
	bool mouseIsDragging = false;
	bool drawHiddenControl = false;
	int gridSize = 10;
	int clickedOnControl = -1;

	int numberOfLayersInCode = 0;

	string sourceCode;

	vector <string> perLayerCode;

	struct layer
	{
		size_t layerStartPosition;
		size_t layerEndPosition;
	};

	vector <layer> layerProperties;
};
#endif
