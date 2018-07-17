#pragma once
#include "Level.h"
#include "Level.h"
#include "defines.h"
#include "ItemMenuArea.h"
#include "AssetList.h"
#include <string>
#include <queue>
#include <vector>
#include "mapstruct.h"
#include "InitOrder.h"
#include "keys.h"
#include "counter.h"
#include "custom_geometry.h"
#include "Room.h"

class Level1 : public Level
{
private:
	//private vars
	bool m_ShowItemMenu = true;
	long m_NumberLayers = 0;
	long m_NumberCustomColors = 0;
	long m_CurrentLayer = 0;
	float m_Size = 128.0f;
	float m_Scale = 1.0f;
	double m_LastInput = 0;
	const float m_StepSize = 0.005f;
	//bool m_Dimetrique = false;
	Grid m_GridType = Grid::m_Square;
	bool m_RecalcLines = true;
	bool m_NextKeepAspect = true;
	bool m_ShowBgColors = true;
	bool m_GridOnTop = false;
	Size m_NextSize = Size::m_Medium;
	COLORREF_F m_BgColor = {};
	FRHOMBUS m_MouseOverArea = {};
	HEX m_HexMouseOverArea = {};
	D2D1_RECT_F m_MouseSelectArea = {};
	D2D1_RECT_F m_DrawArea = {};
	D2D1_POINT_2F m_StartPoint = {};
	D2D1_POINT_2F m_TempWallStartPoint = {};
	D2D1_POINT_2F m_PreviewPoint = {};
	D2D1_POINT_2F m_RulerStartPoint = {};
	D2D1_POINT_2F m_RulerEndPoint = {};
	std::queue<FLINE> m_Lines;
	std::vector<std::string> m_Types;
	ItemMenuArea m_ItemMenuArea;
	float m_ConeAngle = 0;

	//this is for testing geometry
	bool m_Geometry = false;
private:
	HMENU m_Menu = NULL;
	HMENU m_LayerMenu = NULL;
	HMENU m_ShowLayerMenu = NULL;
	HMENU m_RoomMenu = NULL;
	HMENU m_ShowRoomMenu = NULL;
public:
	void Load() override;
	void Unload() override;
	void Render() override;
	void Update(WPARAM wParam, D2D1_POINT_2F p) override;
	void Update(float timeTotal, float timeDelta, D2D1_POINT_2F p) override;
	void ReactInputs(float timeTotal, float timeDelta, D2D1_POINT_2F p);
	void ReactInputsGeometryMode(float timeTotal, float timeDelta, D2D1_POINT_2F p);			//this is for testing geometry modding
public:
	std::queue<FLINE> CalculateGridPointsDimetrique(D2D1_RECT_F area, float xSize, float scale);
	std::queue<FLINE> CalculateGridPoints(D2D1_RECT_F area, float xSize, float scale);
	std::queue<std::string> m_TypeList;
	std::queue<std::string> m_SubmenuList;
	std::queue<std::string> m_ItemList;
	std::vector<Asset> m_AssetList;
	std::vector<std::string> m_SpriteList;
	std::vector<SpriteSheet*> m_Sprites;
	bool BuildAssetList(const wchar_t* initFile, std::vector<Asset>& assetList);
	void BuildAssetList(const char* initFile, std::vector<Asset>& assetList);
	void BuildAssetListFromBuffer(const char* buffer, const size_t bufferLen, std::vector<Asset>& assetList);
	void BuildSpriteList(std::vector<Asset> assetList);
	void LoadSpriteList();
	void BindSpriteToAsset(std::vector<Asset>& assetList);
	bool SpriteExists(std::string path);
	bool EntryExists(std::string path, std::queue<std::string> list);
	void EmptySpriteVector(std::vector<SpriteSheet*>& q);
	void EmptySpriteList(std::vector<std::string>& q);
	void SortAssetList(std::vector<Asset> assetList);
	int GetLine(const char* buffer, char*& target);
	int GetLine(const char* buffer, const int bufferLen, char*& target);
	void TranslateLine(const char* beforeEqual, const char* afterEqual, std::vector<Asset>& assetList);
	void LTrimString(char*& target);
	void RTrimString(char*& target);
	void TrimString(char*& target);
	char* SplitString(char*& target);
	void DrawHexGrid(std::vector<HEX> hex);
	void DrawGridLines(std::queue<FLINE> lines);
	void EmptyLineQueue(std::queue<FLINE>& lines);
	void EmptyAssetList(std::vector<Asset>& asset);
	void EmptyMapData(std::vector<MapData>& data);
	void EmptyShapeData(std::vector<ShapeData>& shape);
	void EmptyWallData(std::vector<WallsData>& wall);
	void DrawHighlightArea(FRHOMBUS area);
	void DrawHighlightArea(FRHOMBUS area, D2D1_COLOR_F color);
	void DrawHighlightArea(D2D1_POINT_2F p, D2D1_COLOR_F color = { 0.0f, 0.0f, 0.0f, 1.0f }, float radius = 5.0f, float thickness = 2.0f);
	bool InTriangle(D2D1_POINT_2F p, FTRIANGLE area);
	bool InRect(D2D1_RECT_F area, D2D1_POINT_2F p);
	bool TouchingLine(FLINE line, D2D1_POINT_2F p);
	bool TouchingLine(D2D1_POINT_2F startPoint, D2D1_POINT_2F endPoint, D2D1_POINT_2F p);
	float Distance(D2D1_POINT_2F p1, D2D1_POINT_2F p2);
	Part SquarePart(D2D1_POINT_2F p, FRECT rect);
	FRHOMBUS CalculateMouseOverArea(float xSize, int numberTilesX, int numberTilesY, D2D1_POINT_2F p);
	void IncreaseSize();
	void DecreaseSize();
	void ChangeMode();
	void CalcConeHighlightArea();
	void CalcLineHighlightArea();
public:
	void BuildMenu();
	void CreateLayerMenu();
	void CreateShowLayerMenu();
	void RebuildShowLayerMenu();
	void EmptyShowLayerMenu();
	void RebuildLayerMenu();
	void EmptyLayerMenu();
	void EmptyRoomMenu();
	void RebuildRoomMenu();
	void EmptyShowRoomMenu();
	void RebuildShowRoomMenu();
	void AddLayerMenu() override;
	void UncheckLayer();
	void CheckLayer();
	void SetHideLayer(unsigned long menuid);
	void SetShowLayer(unsigned long menuid);
	void ToggleShowHideLayer(unsigned long menuid);
	void ToggleShowHideRoom(unsigned long menuid);
	bool MenuItemChecked(HMENU menu, unsigned long MENU_ITEM);
	void CreateRoomMenu();
	void CreateShowRoomMenu();
	void AddRoomMenu();
	void UncheckRoom();
	void CheckRoom();
public:
	bool m_RebuildInitDat = false;
	void BackupDatFile(const char* filepath);
	bool UpdateAssetList(const char* filepath, std::vector<Asset>& assetList);
	void RebuildInitDat(const char* filepath, std::vector<Asset>& assetList);
public:
	std::vector<MapData> m_MapData;
	std::vector<ShapeData> m_Shapes;
	std::vector<WallsData> m_Walls;
	std::vector<CustomGeometry> m_CG;
	void DrawMap(std::vector<MapData> mapData, std::vector<ShapeData> shapes, std::vector<WallsData> walls);
	void UpdateMapValues();
	void UpdateSelectArea();
	MapData* m_SelectedItem = NULL;
	ShapeData* m_SelectedShape = NULL;
	D2D1_RECT_F m_SelectArea = {};
	InitiativeArea m_InitArea;
public:
	void SetDefaultKeys();
	void SetKeys(char* buffer);
	void SetKey(char* beforeEqual, char* afterEqual);
	long CharToLong(char* vk_key);
	KeyClass m_UpKey;
	KeyClass m_DownKey;
	KeyClass m_LeftKey;
	KeyClass m_RightKey;
	KeyClass m_ZoomInKey;
	KeyClass m_ZoomOutKey;
	KeyClass m_NextTurnKey;
	KeyClass m_PrevTurnKey;
	KeyClass m_SelectKey;
	KeyClass m_EraseKey;
	KeyClass m_DrawKey;
	KeyClass m_RulerKey;
	KeyClass m_IncreaseSize;
	KeyClass m_DecreaseSize;
	KeyClass m_InsertKey;

	KeyClass m_TagKey;
	KeyClass m_Range;
	KeyClass m_BuffKey;
	KeyClass m_DebuffKey;
	KeyClass m_UnconsciousKey;
	KeyClass m_OnFireKey;

	KeyClass m_ResetPositionKey;
	KeyClass m_RotationKey;

	KeyClass m_SetGeometry;
public:
	void Open(std::wstring filePath);
	void Open(const wchar_t* filePath) override;
	void Open(std::string filePath);
	void Open(const char* filePath);
	void Save(std::wstring filePath);
	void Save(const wchar_t* filePath);
	void Save(std::string filePath);
	void Save(const char* filePath);
	size_t CalcSaveBufferSize();
	size_t CalcCustomColorSaveSize();
	char* FillBuffer(int& bufferSize);
	void LoadFromBuffer(char* buffer, const size_t bufferLen);
	void LoadOldVersion(FILE* file);
	std::string GetSaveFilePath();
	std::wstring WGetSaveFilePath();
	std::string GetLoadFilePath();
	std::wstring WGetLoadFilePath();
public:
	void EmptyHexVector(std::vector<HEX>& v);
	void CalcHexGrid();
	std::vector<HEX> m_Hexes;
	HEX CalculateMouseOverArea(D2D1_POINT_2F p);
	bool InHex(HEX hex, D2D1_POINT_2F p);
	float CalcRulerDistance();
	std::wstring m_RulerDistance;
	SpriteSheet* m_OnFire = NULL;
	counter m_Counter;
	std::vector<D2D1_POINT_2F> m_CurrentGeometry;
	std::vector<ID2D1PathGeometry*> m_PathGeometry;
	std::vector<ID2D1GeometrySink*> m_SinkGeometry;
	CustomGeometry cg;
	std::vector<Room> m_Rooms;
	bool m_UseOld = false;
	unsigned long m_CurrentRoom = 0;
};