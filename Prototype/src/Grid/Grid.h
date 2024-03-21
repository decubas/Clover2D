#pragma once

#include <CloverEngine.h>


class CGrid
{
public:
	struct SCell
	{
		float x = 0;
		float y = 0;
		uint32 index = 0;
		uint32 Size = 0;
	};

	void Generate(uint32 GridWidth, uint32 GridHeight, uint32 CellSize, int32 offfset_x, int32 offfset_y);
	
	bool IsValidCell(int32 x, int32 y);
	bool GetCell(int32 x, int32 y, SCell& OutCell);
	bool GetCellFromWorldLocation(float x, float y, SCell& OutCell);
	
	std::vector<SCell>& GetCells() { return Cells; };
	const std::vector<SCell>& GetCells() const { return Cells; };

	const uint32 GetWidth() const { return Width; }
	const uint32 GetHeight() const { return Height; }
	const uint32 GetCellSize() const { return CellSize; }
	const int32 GetOffsetX() const { return OffsetX; }
	const int32 GetOffsetY() const { return OffsetY; }

	static SCell InvalidCell;
private:

	int32 OffsetX = 0;
	int32 OffsetY = 0;
	uint32 CellSize = 0;
	uint32 Width = 0;
	uint32 Height = 0;

	std::vector<SCell> Cells;
};