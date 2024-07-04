#include <graphics.h>
#include <iostream>

static const int WINDOW_LENGTH = 1280;
static const int WINDOW_WIDTH = 720;
static const int GRID_NUM = 3;
static const int GRID_WIDTH = WINDOW_WIDTH / GRID_NUM;
enum class GridStatus {
	Empty,
	x,
	o,
};

enum class GameStatus {
	X_winning,
	O_winning,
	Draw,
	Running,
};

void draw_grid()
{
	line(0, WINDOW_WIDTH / 3, WINDOW_WIDTH, WINDOW_WIDTH / 3);
	line(0, WINDOW_WIDTH * 2 / 3, WINDOW_WIDTH, WINDOW_WIDTH * 2 / 3);
	line(WINDOW_WIDTH / 3, 0, WINDOW_WIDTH / 3, WINDOW_WIDTH);
	line(WINDOW_WIDTH * 2 / 3, 0, WINDOW_WIDTH * 2 / 3, WINDOW_WIDTH);
}
void draw_stroke(GridStatus grid[GRID_NUM][GRID_NUM])
{
	for (size_t i = 0; i < 3; ++i)
	{
		for (size_t j = 0; j < 3; ++j)
		{
			switch (grid[i][j])
			{
			case GridStatus::x:

				line(GRID_WIDTH * j, GRID_WIDTH * i, GRID_WIDTH * (j + 1), GRID_WIDTH * (i + 1));
				line(GRID_WIDTH * j, GRID_WIDTH * (i + 1), GRID_WIDTH * (j + 1), GRID_WIDTH * i);
				break;
			case GridStatus::o:
				circle(GRID_WIDTH / 2 + j * GRID_WIDTH, GRID_WIDTH / 2 + i * GRID_WIDTH, GRID_WIDTH / 2);

				break;
			default:
				break;
			}
		}
	}
}

GridStatus change_grid(int x,int y,GridStatus& current_stroke,GridStatus TicTacGrid[GRID_NUM][GRID_NUM])
{

		int col = x / GRID_WIDTH;
		int row = y / GRID_WIDTH;

		if (col >= 0 && col < GRID_NUM && row >= 0 && row < GRID_NUM)
		{

			if (TicTacGrid[row][col] == GridStatus::Empty)
			{
				TicTacGrid[row][col] = current_stroke;
				return (current_stroke == GridStatus::o) ? GridStatus::x : GridStatus::o;
			}
		}
		return current_stroke;
}

 void draw_tip_text(GridStatus current_stroke)
{
	 static TCHAR str[64];
	 _stprintf_s(str, _T("当前棋子类型:%c"), (current_stroke==GridStatus::x)?'x':'o');
	 settextcolor(RGB(225, 175, 45));
	 outtextxy(1000, 200, str);
}

 GameStatus check_win(GridStatus tictacgrid[GRID_NUM][GRID_NUM])
{
	for (int i = 0; i < GRID_NUM; ++i)
	{
		if (tictacgrid[i][0] == tictacgrid[i][1] && tictacgrid[i][1] == tictacgrid[i][2])
		{
			if (tictacgrid[i][0] == GridStatus::x)
			{
				return GameStatus::X_winning;
			}
			else if (tictacgrid[i][0] == GridStatus::o)
			{
				return GameStatus::O_winning;
			}
		}
		else if (tictacgrid[0][i] == tictacgrid[1][i] && tictacgrid[1][i] == tictacgrid[2][i])
		{
			if (tictacgrid[0][i] == GridStatus::x)
			{
				return GameStatus::X_winning;
			}
			else if (tictacgrid[0][i] == GridStatus::o)
			{
				return GameStatus::O_winning;
			}
		}
	}
	if (tictacgrid[0][0] == tictacgrid[1][1] && tictacgrid[1][1] == tictacgrid[2][2])
	{
		if (tictacgrid[0][0] == GridStatus::x) {
			return GameStatus::X_winning;
		}
		else if (tictacgrid[0][0] == GridStatus::o)
		{
			return GameStatus::O_winning;
		}
	}
	else if (tictacgrid[2][0] == tictacgrid[1][1] && tictacgrid[1][1] == tictacgrid[0][2])
	{
		if (tictacgrid[2][0] == GridStatus::x)
		{
			return GameStatus::X_winning;
		}
		else if (tictacgrid[2][0] == GridStatus::o)
		{
			return GameStatus::O_winning;
		}
	}
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			if (tictacgrid[i][j] == GridStatus::Empty)
			{
				return GameStatus::Running;
			}
		}
	}
	return GameStatus::Draw;
}



int main()
{
	GridStatus TicTacGrid[GRID_NUM][GRID_NUM];
	for (int i = 0; i < GRID_NUM; ++i)
	{
		for (int j = 0; j < GRID_NUM; ++j)
		{
			TicTacGrid[i][j] = GridStatus::Empty;
		}
	}

	initgraph(WINDOW_LENGTH, WINDOW_WIDTH);
	GameStatus gamestatus = GameStatus::Running;
	ExMessage msg;
	BeginBatchDraw();
	GridStatus current_stroke = GridStatus::x;

	while (gamestatus == GameStatus::Running)
	{
		DWORD start_time = GetTickCount();
		while (peekmessage(&msg))
		{
			if (msg.message == WM_LBUTTONDOWN)
			{
				int x = msg.x;
				int y = msg.y;
				current_stroke = change_grid(x, y, current_stroke, TicTacGrid);
			}
		}
	
		gamestatus = check_win(TicTacGrid);
		 
		cleardevice();
		draw_grid();
		draw_stroke(TicTacGrid);
		draw_tip_text(current_stroke);
		FlushBatchDraw();
		DWORD end_time = GetTickCount();
		DWORD delta_time = end_time - start_time;
		if (delta_time < 1000 / 60)
		{
			Sleep(1000 / 60 - delta_time);
		}
	}
	if (gamestatus == GameStatus::O_winning)
	{
		MessageBox(GetHWnd(), _T("O玩家获胜"), _T("游戏结束"), MB_OK);
	}
	else if (gamestatus == GameStatus::X_winning)
	{
			MessageBox(GetHWnd(), _T("X玩家获胜"), _T("游戏结束"), MB_OK);
	}
	else if (gamestatus == GameStatus::Draw)
	{
		MessageBox(GetHWnd(), _T("平局"), _T("游戏结束"), MB_OK);
	}
	EndBatchDraw();
	closegraph();
	return 0;
	
}