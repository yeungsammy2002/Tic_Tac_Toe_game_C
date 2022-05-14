#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>

typedef struct __input {
	int vx;
	int vy;
	int px;
	int py;
	bool filled;
} input;

typedef struct __box {
	int val;
	int pos;
} box;

typedef struct __val {
	box boxes[3][3];
} vals;

typedef struct __board {
	char px[11][17];
} board;

typedef struct __scores {
	int row[3];
	int col[3];
	int slope[2];
} scores;

typedef struct __empty {
	int row[3];
	int col[3];
	int slope[2];
} empty;

void player_init(char* p) {
	srand(time(NULL));
	p[0] = rand() % 2 ? 'O' : 'X';
	p[1] = p[0] == 'O' ? 'X' : 'O';
}

void init(input* inputs, vals* v, scores* s, empty* e) {
	int i = 0;
	for (int y = 0; y < 3; ++y) {
		for (int x = 0; x < 3; ++x) {
			v->boxes[y][x].val = 2;
			v->boxes[y][x].pos = i + 1;
			inputs[i].vx = x;
			inputs[i].vy = y;
			inputs[i].filled = false;
			i++;
		}
		s[0].row[y] = 0;
		s[0].col[y] = 0;
		s[1].row[y] = 0;
		s[1].col[y] = 0;
		e->row[y] = 3;
		e->col[y] = 3;
	}
	s[0].slope[0] = 0;
	s[0].slope[1] = 0;
	s[1].slope[0] = 0;
	s[1].slope[1] = 0;

	e->slope[0] = 3;
	e->slope[1] = 3;
}

void board_init(board* b, input* inputs) {
	for (int y = 0; y < 11; ++y) {
		for (int x = 0; x < 17; ++x) {
			b->px[y][x] = ' ';
			if (y == 3)b->px[y][x] = '+';
			if (y == 7)b->px[y][x] = '+';
			if (x == 5)b->px[y][x] = '+';
			if (x == 11)b->px[y][x] = '+';
		}
	}

	int v = 1;
	int i = 0;
	for (int y = 1; y < 11; y += 4) {
		for (int x = 2; x < 17; x += 6) {
			b->px[y][x] = v + '0';
			inputs[i].px = x;
			inputs[i].py = y;
			i++;
			v++;
		}
	}
}

void print_board(board* b, char* p, int first) {
	system("clear");
	printf("\n\t\t\t\t\t    Welcome to Tic Tac Toe game!\n");
	printf("\n\t\t\t\t\t       You = %c     Com = %c\n", p[0], p[1]);
	if (!first)
		printf("\n\t\t\t\t\t             You first!\n\n");
	else
		printf("\n\t\t\t\t\t             Com first!\n\n");

	for (int y = 0; y < 11; ++y) {
		printf("\t\t\t\t\t\t ");
		for (int x = 0; x < 17; ++x) {
			printf("%c", b->px[y][x]);
		}
		printf("\n");
	}
	printf("\n\n");
}

void print_vals(vals* v) {
	system("clear");
	for (int y = 0; y < 3; ++y) {
		for (int x = 0; x < 3; ++x) {
			printf("%d", v->boxes[y][x].val);
		}
		printf("\n");
	}
}

void input_val(int n, input* inputs, board* b, vals* v, scores* s, empty* e, int player, char* p) {
	int i = n - 1;
	v->boxes[inputs[i].vy][inputs[i].vx].val = player;
	b->px[inputs[i].py][inputs[i].px] = p[player];
	inputs[i].filled = true;
	s[player].row[inputs[i].vy]++;
	s[player].col[inputs[i].vx]++;
	e->row[inputs[i].vy]--;
	e->col[inputs[i].vx]--;

	if (inputs[i].vx == inputs[i].vy) {
		s[player].slope[0]++;
		e->slope[0]--;
		if (inputs[i].vx == 1) {
			s[player].slope[1]++;
			e->slope[1]--;
		}
	}

	if (abs(inputs[i].vx - inputs[i].vy) == 2) {
		s[player].slope[1]++;
		e->slope[1]--;
	}
}

bool is_occupy(int num, input* inputs) {
	int i = num - 1;
	return inputs[i].filled;
}

int get_val(input* inputs) {
	int num = 0;
	printf("\t\t\t\t\t     Please enter the number: ");
	scanf_s("%d", &num);
	while (num < 1 || num > 9 || is_occupy(num, inputs)) {
		printf("\t\t\t\t\t     Invalid move, please enter the number again: ");
		scanf_s("%d", &num);
	}
	return num;
}

bool check_winner(scores* s, int player) {
	for (int i = 0; i < 3; ++i) {
		if (s[player].row[i] == 3) return true;
		if (s[player].col[i] == 3) return true;
	}
	if (s[player].slope[0] == 3) return true;
	if (s[player].slope[1] == 3) return true;
	return false;
}

int ai_decision(int move, scores* s, empty* e, vals* v) {
	for (int i = 0; i < 3; ++i) {
		if (s[1].row[i] == 2 && e->row[i] == 1) {
			for (int j = 0; j < 3; ++j) {
				if (v->boxes[i][j].val == 2) return v->boxes[i][j].pos;
			}
		}
		if (s[1].col[i] == 2 && e->col[i] == 1) {
			for (int j = 0; j < 3; ++j) {
				if (v->boxes[j][i].val == 2) return v->boxes[j][i].pos;
			}
		}
	}

	if (s[1].slope[0] == 2 && e->slope[0] == 1) {
		for (int i = 0; i < 3; ++i) {
			if (v->boxes[i][i].val == 2) return v->boxes[i][i].pos;
		}
	}

	if (s[1].slope[1] == 2 && e->slope[1] == 1) {
		for (int i = 0, j = 2; i < 3; ++i, --j) {
			if (v->boxes[i][i].val == 2) return v->boxes[i][i].pos;
		}
	}

	for (int i = 0; i < 3; ++i) {
		if (s[0].row[i] == 2 && e->row[i] == 1) {
			for (int j = 0; j < 3; ++j) {
				if (v->boxes[i][j].val == 2) return v->boxes[i][j].pos;
			}
		}
		if (s[0].col[i] == 2 && e->col[i] == 1) {
			for (int j = 0; j < 3; ++j) {
				if (v->boxes[j][i].val == 2) return v->boxes[j][i].pos;
			}
		}
	}

	if (s[0].slope[0] == 2 && e->slope[0] == 1) {
		for (int i = 0; i < 3; ++i) {
			if (v->boxes[i][i].val == 2) return v->boxes[i][i].pos;
		}
	}

	if (s[0].slope[1] == 2 && e->slope[1] == 1) {
		for (int i = 0, j = 2; i < 3; ++i, --j) {
			if (v->boxes[i][j].val == 2) return v->boxes[i][j].pos;
		}
	}

	for (int i = 0; i < 3; ++i) {
		if (s[1].row[i] == 1 && e->row[i] == 2) {
			for (int j = 0; j < 3; ++j) {
				if (v->boxes[i][j].val == 2) {
					return v->boxes[i][j].pos;
				}
			}
		}
		if (s[1].col[i] == 1 && e->col[i] == 2) {
			for (int j = 0; j < 3; ++j) {
				if (v->boxes[j][i].val == 2) return v->boxes[j][i].pos;
			}
		}
	}

	if (s[1].slope[0] == 1 && e->slope[0] == 2) {
		for (int i = 0; i < 3; ++i) {
			if (v->boxes[i][i].val == 2) return v->boxes[i][i].pos;
		}
	}

	if (s[1].slope[1] == 1 && e->slope[1] == 2) {
		for (int i = 0, j = 2; i < 3; ++i, --j) {
			if (v->boxes[i][j].val == 2) return v->boxes[i][j].pos;
		}
	}

	return move;
}

int ai_move(input* inputs, scores* s, empty* e, vals* v) {
	srand(time(NULL));
	int move = rand() % 9 + 1;
	while (is_occupy(move, inputs)) {
		move = rand() % 9 + 1;
	}
	move = ai_decision(move, s, e, v);
	if (is_occupy(move, inputs)) {
		printf("error occur!");
		Sleep(600000);
	}

	return move;
}

void display_scores(scores* s, empty* e) {
	printf("\nPlayer-0\n\n");
	for (int i = 0; i < 3; ++i) {
		printf("row-%d: %d\n", i, s[0].row[i]);
	}
	for (int i = 0; i < 3; ++i) {
		printf("col-%d: %d\n", i, s[0].col[i]);
	}
	printf("slope-0: %d\n", s[0].slope[0]);
	printf("slope-1: %d\n\n", s[0].slope[1]);

	printf("Player-1\n\n");
	for (int i = 0; i < 3; ++i) {
		printf("row-%d: %d\n", i, s[1].row[i]);
	}
	for (int i = 0; i < 3; ++i) {
		printf("col-%d: %d\n", i, s[1].col[i]);
	}
	printf("slope-0: %d\n", s[1].slope[0]);
	printf("slope-1: %d\n\n", s[1].slope[1]);

	printf("\nEmpty:\n\n");
	for (int i = 0; i < 3; ++i) {
		printf("row-%d: %d\n", i, e->row[i]);
	}
	for (int i = 0; i < 3; ++i) {
		printf("col-%d: %d\n", i, e->col[i]);
	}
	printf("slope-0: %d\n", e->slope[0]);
	printf("slope-1: %d\n\n", e->slope[1]);
}

bool run(input* inputs, board* b, vals* v, scores* s, empty* e, int player, char* p, int* winner, int* turn, int first) {
	int move = 0;

	if (player == 0)
		move = get_val(inputs);
	else if (player == 1)
		move = ai_move(inputs, s, e, v);

	input_val(move, inputs, b, v, s, e, player, p);

	print_board(b, p, first);

	//print_vals(v);
	//display_scores(s, e);

	if (check_winner(s, player)) {
		*winner = player;
		if (*winner == 0)
			printf("\t\t\t\t\t     Congratulations! You win!");
		if (*winner == 1)
			printf("\t\t\t\t\t     You lose!");
		return true;
	}

	(*turn)++;
	if (*turn > 9) {
		printf("\t\t\t\t\t     Draw game!");
		return true;
	}
	return false;
}

int main() {
	char again = 'y';

	while (again == 'y') {
		char* p = malloc(sizeof(char) * 2);
		input* inputs = malloc(sizeof(input) * 9);
		vals* v = malloc(sizeof(vals));
		board* b = malloc(sizeof(board));
		scores* s = malloc(sizeof(scores) * 2);
		empty* e = malloc(sizeof(empty));
		if (!v || !b || !s || !e || !p || !inputs) exit(-1);

		int turn = 1;
		int* turn_p = &turn;
		srand(time(NULL));
		int first = rand() % 2;
		int p1 = first;
		int p2 = first ? 0 : 1;

		player_init(p);
		init(inputs, v, s, e);
		board_init(b, inputs);

		print_board(b, p, first);

		//print_vals(v);

		int winner = 2;
		int* w = &winner;
		while (1) {
			bool end = run(inputs, b, v, s, e, p1, p, w, turn_p, first);
			if (end) break;
			end = run(inputs, b, v, s, e, p2, p, w, turn_p, first);
			if (end) break;
		}

		Sleep(3000);

		char game_winner[10];

		if (winner == 0)
			strcpy_s(game_winner, 10, "You");
		if (winner == 1)
			strcpy_s(game_winner, 10, "Com");
		if (winner == 2)
			strcpy_s(game_winner, 10, "Draw");

		FILE* fp = NULL;
		fopen_s(&fp, "result.txt", "a+");
		if (!fp) exit(-1);

		// for blank txt
		fseek(fp, 0, SEEK_END);
		int len = ftell(fp);
		if (!len) {
			fprintf(fp, "\n\t\t\tTic Tac Toe game result");

			char* first_line = "\n\n\t\t\tRound\tWinner\tDate\n";
			fputs(first_line, fp);
		}

		int round = 1;
		if (len) {
			fseek(fp, 0, SEEK_SET);
			int nl = 0;
			int ch = 0;
			while ((ch = fgetc(fp)) != EOF) {
				if (ch == '\n') nl++;
			}
			round = ((nl - 6) / 2) + 2;
		}

		time_t t = time(NULL);
		struct tm ts;
		char buf[80];
		localtime_s(&ts, &t);
		strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S", &ts);
		fprintf(fp, "\n\t\t\t%d\t%s\t%s\n", round, game_winner, buf);
		fclose(fp);

		system("clear");

		// print result.txt to the console
		FILE* nfp = NULL;
		fopen_s(&nfp, "result.txt", "r");
		if (!nfp) exit(-1);

		char result_str[300];



		while (fgets(result_str, 299, nfp) != NULL) {
			printf("%s", result_str);
		}
		fclose(nfp);

		printf("\n\t\t\tWould you like to play again? (Yes(y)/No(n): ");
		scanf_s(" %c", &again, 1);

		free(p);
		free(inputs);
		free(v);
		free(b);
		free(s);
		free(e);
	}

	return 0;
}