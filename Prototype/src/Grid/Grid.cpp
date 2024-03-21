#include "Grid.h"


bool CGrid::GetCell(int32 x, int32 y, SCell& OutCell)
{
	if (IsValidCell(x, y))
	{
		OutCell = Cells[x + y * Width];
		return true;
	}
	return false;
}

bool CGrid::GetCellFromWorldLocation(float x, float y, SCell& OutCell)
{
	return GetCell(std::round((x - OffsetX) / CellSize), std::round((y - OffsetY) / CellSize), OutCell);
}

void CGrid::Generate(uint32 GridWidth, uint32 GridHeight, uint32 CellSize, int32 offfset_x, int32 offfset_y)
{
	OffsetX = offfset_x;
	OffsetY = offfset_y;
	Width = GridWidth;
	Height = GridHeight;
	this->CellSize = CellSize;
	for (int32 i = 0; i < Height; i++)
	{
		for (int32 j = 0; j < Width; j++)
		{
			SCell NewCell;
			NewCell.index = j + i * Width;
			NewCell.x = OffsetX + j * (int32)CellSize;
			NewCell.y = OffsetY + i * (int32)CellSize;
			NewCell.Size = CellSize;
			Cells.push_back(NewCell);
		}
	}
}

bool CGrid::IsValidCell(int32 x, int32 y)
{
	return Cells.size() > (x + y * Width) && x >= 0 && y >= 0;
}
