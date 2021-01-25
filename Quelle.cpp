#include"Chess_figures.h"
#undef vector

constexpr int sz = 100;									//Kommt vielleicht sp�ter in die Fenster Klasse, w�re evt besser ein double um Schwulit�ten zu vermeiden wenn sz nicht 100 ist
constexpr int ls = 100;									//Abstand zum linken Fensterrand
constexpr int us = 0;									//Abstand zum oberen Fensterrand


//Helfer Funktionen
Point get_point(int x, int y) {							//Ermittelt Koordinaten f�r Spielfeld
	Point res{ x - x % sz,y - y % sz };
	return res;
}
bool operator==(const Graph_lib::Color& a, const Graph_lib::Color& b) {				//Vergleicht Farben
	if (a.as_int() == b.as_int())return true;
	return false;
}
bool operator!=(const Graph_lib::Color& a, const Graph_lib::Color& b) {				//Vergleicht Farben
	if (!operator==(a, b))return true;
	return false;
}


class Chess_window : public Window {

	bool figure_selected;
	bool button_pushed;

	bool white_l_rook = true;
	bool white_king = true;
	bool white_r_rook = true;

	bool black_l_rook = true;
	bool black_king = true;
	bool black_r_rook = true;

	Chess_figure* curr_figure = nullptr;
	Color c_turn = Color::white;

	Button quit_button;

	Vector_ref<Button>field_buttons;					//F�r die Kn�ppe unter den schwarzen Feldern
	Vector_ref< Graph_lib::Rectangle>tiles;				//F�r die Felder
	Vector_ref<Chess_figure>figures;
	vector<Point>ep;									//M�gliche en passant Ziele
	vector<Point>white_castling;
	vector<Point>black_castling;
public:
	Chess_window(Point xy, int w, int h, const string& title);
	void wait_for_button();

private:
	bool tile_empty(Point&)const;
	bool hostile_present(Point&);
	bool is_ep(Point&);									//Pr�ft ob es sich bei der �bergebenen Koordinate um eine en passanr Ziel handelt
	void check_ep();									//Pr�ft und aktualisiert die Liste der m�glichen en passant Ziele
	Chess_figure* get_figure(Point&);
	void reset_f_color();								//Stellt die urspr�ngliche Umrandungsfarbe wieder her
	void refresh_figures();								//Ohne diese Funktion k�nne Figuren hinter den Felder verschwinden. Keine Ahnung warum.
	bool castling(Point&);
	bool tile_in_check(Point&);

	void pawn_movement(Point&);
	void rook_movement(Point&);
	void knight_movement(Point&);
	void bishop_movement(Point&);
	void queen_movement(Point&);
	void king_movement(Point& p);

	static void cb_quit(Address, Address addr) { reference_to<Chess_window>(addr).quit(); }		//Ist die Stroustrup Variante, macht auch nur 		return *static_cast<W*>(pw);
	static void cb_tile_pressed(Address, Address addr) { reference_to<Chess_window>(addr).tile_pressed(); }

	void quit() { hide(); button_pushed = true; }
	void tile_pressed();
};

Chess_window::Chess_window(Point xy, int w, int h, const string& title) :Window(w, h, title),
button_pushed(false), figure_selected(false),
quit_button(Point(x_max() - 70, 60), 70, 20, "Quit", cb_quit) {

	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			//Kn�ppe unter den Feldern
			field_buttons.push_back(new Button{ {ls + x * sz,us + y * sz},sz,sz ," ",cb_tile_pressed });
			attach(field_buttons[field_buttons.size() - 1]);

			//Felder
			tiles.push_back(new Graph_lib::Rectangle{ {ls + x * sz,us + y * sz},sz,sz });
			tiles[tiles.size() - 1].set_color(fl_rgb_color(153, 76, 0));
			if ((y + x) % 2 != 0)tiles[tiles.size() - 1].set_fill_color(fl_rgb_color(204, 102, 0));
			if ((y + x) % 2 == 0)tiles[tiles.size() - 1].set_fill_color(fl_rgb_color(255, 255, 204));

			attach(tiles[tiles.size() - 1]);

			//Figuren
			if ((y == 0 || y == 7) && (x == 0 || x == 7))figures.push_back(new Rook{ {ls + x * sz ,us + y * sz },sz });
			if ((y == 0 || y == 7) && (x == 1 || x == 6))figures.push_back(new Knight{ {ls + x * sz ,us + y * sz },sz });
			if ((y == 0 || y == 7) && (x == 2 || x == 5))figures.push_back(new Bishop{ {ls + x * sz ,us + y * sz },sz });
			if ((y == 0 || y == 7) && (x == 3))figures.push_back(new Queen{ {ls + x * sz ,us + y * sz },sz });
			if ((y == 0 || y == 7) && (x == 4))figures.push_back(new King{ {ls + x * sz ,us + y * sz },sz });
			if (y == 1 || y == 6)figures.push_back(new Pawn{ {ls + x * sz ,us + y * sz },sz });
			attach(figures[figures.size() - 1]);

			if (y == 0 || y == 1) { figures[figures.size() - 1].set_color(Color::white); figures[figures.size() - 1].set_fill_color(Color::black); }
			if (y == 6 || y == 7)figures[figures.size() - 1].set_fill_color(Color::white);

			//Rochade Buffer
			if (figures[figures.size() - 1].fill_color() == Color::black && figures[figures.size() - 1].what_kind() == F_kind::rook)black_castling.push_back(figures[figures.size() - 1].point(0));
			if (figures[figures.size() - 1].fill_color() == Color::black && figures[figures.size() - 1].what_kind() == F_kind::king)black_castling.push_back(figures[figures.size() - 1].point(0));
			if (figures[figures.size() - 1].fill_color() == Color::white && figures[figures.size() - 1].what_kind() == F_kind::rook)white_castling.push_back(figures[figures.size() - 1].point(0));
			if (figures[figures.size() - 1].fill_color() == Color::white && figures[figures.size() - 1].what_kind() == F_kind::king)white_castling.push_back(figures[figures.size() - 1].point(0));

		}
	}
	attach(quit_button);
	size_range(w, h, w, h);	//Fixiert Fenster
}
void Chess_window::wait_for_button() {
	while (!button_pushed) Fl::wait();
	button_pushed = false;
	Fl::redraw();
}

bool Chess_window::tile_empty(Point& p)const {
	Point curr{ p.x ,p.y };
	if (p.x < ls || p.x >= ls + 8 * sz || p.y < us || p.y >= us + 8 * sz)return false;
	for (int i = 0; i < figures.size(); i++) {
		if (figures[i].point(0) == curr)return false;
	}
	return true;
}
bool Chess_window::hostile_present(Point& p) {
	Chess_figure* res;
	if (p.x < ls || p.x >= ls + 8 * sz || p.y < us || p.y >= us + 8 * sz)return false;
	if (!tile_empty(p))res = get_figure(p);
	else return false;
	if (figure_selected && curr_figure->fill_color() == res->fill_color())return false;
	return true;
}
bool Chess_window::is_ep(Point& p) {
	for (int i = 0; i < ep.size(); i++)if (p == ep[i])return true;
	return false;
}
void Chess_window::check_ep() {
	for (int i = 0; i < ep.size(); i++) {
		bool check = false;
		for (int j = 0; j < figures.size(); j++) if (ep[i] == figures[j].point(0) && figures[j].what_kind() == F_kind::pawn)check = true;

		if (check == false)ep.erase(ep.begin() + i);
	}
}
Chess_figure* Chess_window::get_figure(Point& p) {
	Chess_figure* res;
	Point curr;
	for (int i = 0; i < figures.size(); i++) {
		curr = figures[i].point(0);
		if (curr == Point{ p.x ,p.y }) {
			res = &figures[i];
			return res;
		}
	}
	return nullptr;
}
void Chess_window::reset_f_color() {
	if (curr_figure->fill_color() == Color::white) curr_figure->set_color(Color::black);
	if (curr_figure->fill_color() == Color::black) curr_figure->set_color(Color::white);
}
void Chess_window::refresh_figures() {
	for (int i = 0; i < figures.size(); i++) detach(figures[i]);
	for (int i = 0; i < figures.size(); i++) attach(figures[i]);
}
bool Chess_window::castling(Point& p) {
	if (c_turn == Color::black && !black_king)return false;
	if (c_turn == Color::white && !white_king)return false;
	if (!curr_figure)return false;
	if (!(curr_figure->what_kind() == F_kind::king))return false;
	if (tile_empty(p) || hostile_present(p))return false;
	Chess_figure* R = get_figure(p);
	if (R->what_kind() != F_kind::rook)return false;

	Point Ki{ curr_figure->point(0) };
	Point Ro{ R->point(0) };
	if (Ki.x < Ro.x) {
		if (curr_figure->fill_color() == Color::black && !black_r_rook)return false;
		if (curr_figure->fill_color() == Color::white && !white_r_rook)return false;

		Point temp{ Ki };
		temp.x += sz;
		if (!(tile_empty(temp)))return false;
		temp.x += sz;
		if (!(tile_empty(temp)))return false;
	}
	if (Ki.x > Ro.x) {
		if (curr_figure->fill_color() == Color::black && !black_l_rook)return false;
		if (curr_figure->fill_color() == Color::white && !white_l_rook)return false;


		Point temp{ Ki };
		temp.x -= sz;
		if (!(tile_empty(temp)))return false;
		temp.x -= sz;
		if (!(tile_empty(temp)))return false;
		temp.x -= sz;
		if (!(tile_empty(temp)))return false;
	}
	return true;
}
bool Chess_window::tile_in_check(Point& p) {
	Point temp{ p };
	Chess_figure* hostile = nullptr;
	//Pr�fung Pferd
	temp.x -= sz;
	temp.y -= (sz * 2);
	if (hostile_present(temp)) {
		hostile = get_figure(temp);
		if (hostile->what_kind() == F_kind::knight)return true;
	}
	temp = p;
	temp.x += sz;
	temp.y -= (sz * 2);
	if (hostile_present(temp)) {
		hostile = get_figure(temp);
		if (hostile->what_kind() == F_kind::knight)return true;
	}
	temp = p;
	temp.x += (sz * 2);
	temp.y -= sz;
	if (hostile_present(temp)) {
		hostile = get_figure(temp);
		if (hostile->what_kind() == F_kind::knight)return true;
	}
	temp = p;
	temp.x += (sz * 2);
	temp.y += sz;
	if (hostile_present(temp)) {
		hostile = get_figure(temp);
		if (hostile->what_kind() == F_kind::knight)return true;
	}
	temp = p;
	temp.x += sz;
	temp.y += (sz * 2);
	if (hostile_present(temp)) {
		hostile = get_figure(temp);
		if (hostile->what_kind() == F_kind::knight)return true;
	}
	temp = p;
	temp.x -= sz;
	temp.y += (sz * 2);
	if (hostile_present(temp)) {
		hostile = get_figure(temp);
		if (hostile->what_kind() == F_kind::knight)return true;
	}
	temp = p;
	temp.x -= (sz * 2);
	temp.y += sz;
	if (hostile_present(temp)) {
		hostile = get_figure(temp);
		if (hostile->what_kind() == F_kind::knight)return true;
	}
	temp = p;
	temp.x -= (sz * 2);
	temp.y -= sz;
	if (hostile_present(temp)) {
		hostile = get_figure(temp);
		if (hostile->what_kind() == F_kind::knight)return true;
	}
	//Pr�fung Bauer oder K�nig
	temp = p;
	temp.x += sz;
	temp.y += sz;
	if (hostile_present(temp)) {
		hostile = get_figure(temp);
		if (curr_figure->fill_color() == Color::black && hostile->what_kind() == F_kind::pawn)return true;
		if (hostile->what_kind() == F_kind::king)return true;

	}
	temp = p;
	temp.x -= sz;
	temp.y += sz;
	if (hostile_present(temp)) {
		hostile = get_figure(temp);
		if (curr_figure->fill_color() == Color::black && hostile->what_kind() == F_kind::pawn)return true;
		if (hostile->what_kind() == F_kind::king)return true;
	}
	temp = p;
	temp.x -= sz;
	temp.y -= sz;
	if (hostile_present(temp)) {
		hostile = get_figure(temp);
		if (curr_figure->fill_color() == Color::white && hostile->what_kind() == F_kind::pawn)return true;
		if (hostile->what_kind() == F_kind::king)return true;
	}
	temp = p;
	temp.x += sz;
	temp.y -= sz;
	if (hostile_present(temp)) {
		hostile = get_figure(temp);
		if (curr_figure->fill_color() == Color::white && hostile->what_kind() == F_kind::pawn)return true;
		if (hostile->what_kind() == F_kind::king)return true;
	}
	//Pr�fung Dame und Turm
	temp = p;
	while (temp.x <= ls + 7 * sz) {
		temp.x += sz;
		if (hostile_present(temp)) {
			hostile = get_figure(temp);
			if (hostile->what_kind() == F_kind::rook)return true;
			if (hostile->what_kind() == F_kind::queen)return true;
			break;
		}
		if (!tile_empty(temp))break;
	}
	temp = p;
	while (temp.x >= ls) {
		temp.x -= sz;
		if (hostile_present(temp)) {
			hostile = get_figure(temp);
			if (hostile->what_kind() == F_kind::rook)return true;
			if (hostile->what_kind() == F_kind::queen)return true;
			break;
		}
		if (!tile_empty(temp))break;
	}
	temp = p;
	while (temp.y <= us + 7 * sz) {
		temp.y += sz;
		if (hostile_present(temp)) {
			hostile = get_figure(temp);
			if (hostile->what_kind() == F_kind::rook)return true;
			if (hostile->what_kind() == F_kind::queen)return true;
			break;
		}
		if (!tile_empty(temp))break;
	}
	temp = p;
	while (temp.y >= us) {
		temp.y -= sz;
		if (hostile_present(temp)) {
			hostile = get_figure(temp);
			if (hostile->what_kind() == F_kind::rook)return true;
			if (hostile->what_kind() == F_kind::queen)return true;
			break;
		}
		if (!tile_empty(temp))break;
	}
	//Pr�fung L�ufer oder Dame
	temp = p;
	while (temp.x <= ls + 7 * sz && temp.y <= us + 7 * sz) {
		temp.x += sz;
		temp.y += sz;
		if (hostile_present(temp)) {
			hostile = get_figure(temp);
			if (hostile->what_kind() == F_kind::bishop)return true;
			if (hostile->what_kind() == F_kind::queen)return true;
			break;
		}
		if (!tile_empty(temp))break;
	}
	temp = p;
	while (temp.x <= ls + 7 * sz && temp.y >= us) {
		temp.x += sz;
		temp.y -= sz;
		if (hostile_present(temp)) {
			hostile = get_figure(temp);
			if (hostile->what_kind() == F_kind::bishop)return true;
			if (hostile->what_kind() == F_kind::queen)return true;
			break;
		}
		if (!tile_empty(temp))break;
	}
	temp = p;
	while (temp.x >= ls && temp.y <= us + 7 * sz) {
		temp.x -= sz;
		temp.y += sz;
		if (hostile_present(temp)) {
			hostile = get_figure(temp);
			if (hostile->what_kind() == F_kind::bishop)return true;
			if (hostile->what_kind() == F_kind::queen)return true;
			break;
		}
		if (!tile_empty(temp))break;
	}
	temp = p;
	while (temp.x >= ls && temp.y >= us) {
		temp.x -= sz;
		temp.y -= sz;
		if (hostile_present(temp)) {
			hostile = get_figure(temp);
			if (hostile->what_kind() == F_kind::bishop)return true;
			if (hostile->what_kind() == F_kind::queen)return true;
			break;
		}
		if (!tile_empty(temp))break;
	}
	return false;
}

void Chess_window::pawn_movement(Point& p) {
	Point start{ curr_figure->point(0) };
	Point temp;
	int x = 0;
	int y = 0;
	if (curr_figure->fill_color() == Color::white) {

		if (p.x == curr_figure->point(0).x - sz && p.y == curr_figure->point(0).y - sz && hostile_present(p)) {
			x = -sz;
			y = -sz;
		}
		temp = { start.x - sz, start.y };
		if (p.x == curr_figure->point(0).x - sz && p.y == curr_figure->point(0).y - sz && is_ep(temp)) {
			x = -sz;
			y = -sz;
			for (int i = 0; i < figures.size(); i++) {
				if (figures[i].point(0) == temp) {
					detach(figures[i]);
					figures.erase(i);
				}
			}
		}
		if (p.x == curr_figure->point(0).x + sz && p.y == curr_figure->point(0).y - sz && hostile_present(p)) {
			x = sz;
			y = -sz;
		}
		temp = { start.x + sz, start.y };
		if (p.x == curr_figure->point(0).x + sz && p.y == curr_figure->point(0).y - sz && is_ep(temp)) {
			x = sz;
			y = -sz;
			for (int i = 0; i < figures.size(); i++) {
				if (figures[i].point(0) == temp) {
					detach(figures[i]);
					figures.erase(i);
				}
			}
		}
		if (p.x == curr_figure->point(0).x && p.y == curr_figure->point(0).y - sz && !hostile_present(p)) {
			x = 0;
			y = -sz;
		}
		if (curr_figure->point(0).y == sz * 6 && p.y == curr_figure->point(0).y - sz * 2 && p.x == curr_figure->point(0).x && !hostile_present(p)) {
			x = 0;
			y = -(sz * 2);
			ep.push_back({ p.x ,p.y });
		}
	}
	if (curr_figure->fill_color() == Color::black) {
		if (p.x == curr_figure->point(0).x - sz && p.y == curr_figure->point(0).y + sz && hostile_present(p)) {
			x = -sz;
			y = sz;
		}
		temp = { start.x - sz, start.y };
		if (p.x == curr_figure->point(0).x - sz && p.y == curr_figure->point(0).y + sz && is_ep(temp)) {
			x = -sz;
			y = sz;
			for (int i = 0; i < figures.size(); i++) {
				if (figures[i].point(0) == temp) {
					detach(figures[i]);
					figures.erase(i);
				}
			}
		}
		if (p.x == curr_figure->point(0).x + sz && p.y == curr_figure->point(0).y + sz && hostile_present(p)) {
			x = sz;
			y = sz;
		}
		temp = { start.x + sz, start.y };
		if (p.x == curr_figure->point(0).x + sz && p.y == curr_figure->point(0).y + sz && is_ep(temp)) {
			x = sz;
			y = sz;
			for (int i = 0; i < figures.size(); i++) {
				if (figures[i].point(0) == temp) {
					detach(figures[i]);
					figures.erase(i);
				}
			}
		}
		if (p.x == curr_figure->point(0).x && p.y == curr_figure->point(0).y + sz && !hostile_present(p)) {
			x = 0;
			y = sz;
		}
		if (curr_figure->point(0).y == sz && p.y == curr_figure->point(0).y + sz * 2 && p.x == curr_figure->point(0).x && !hostile_present(p)) {
			x = 0;
			y = sz * 2;
			ep.push_back({ p.x ,p.y });

		}
	}
	Point target{ curr_figure->point(0).x + x,curr_figure->point(0).y + y };
	if (hostile_present(target)) {
		for (int i = 0; i < figures.size(); i++) {
			if (figures[i].point(0) == target) {
				detach(figures[i]);
				figures.erase(i);
			}
		}
	}
	curr_figure->move(x, y);
	if (start != curr_figure->point(0)) {
		reset_f_color();
		if (c_turn == Color::white)c_turn = Color::black;
		else c_turn = Color::white;
		figure_selected = false;
		curr_figure = nullptr;
	}
}
void Chess_window::rook_movement(Point& p) {
	Point upper_left{ ls,0 };
	Point upper_right{ ls + 7 * sz,0 };
	Point lower_left{ ls,us + 7 * sz };
	Point lower_right{ ls + 7 * sz,us + 7 * sz };
	Point start{ curr_figure->point(0) };
	Point temp{ curr_figure->point(0) };
	int x = 0;
	int y = 0;
	if (p.x == start.x && p.y < start.y)y = -sz;		//nach oben
	if (p.x == start.x && p.y > start.y) y = sz;		//nach unten
	if (p.x > start.x && p.y == start.y)x = sz;			//nach rechts
	if (p.x < start.x && p.y == start.y)x = -sz;		//nach links
	while (temp != p) {
		temp.x += x;
		temp.y += y;
		if (!tile_empty(temp) && temp != p) {
			temp = start;
			x = 0;
			y = 0;
			break;
		}
	}
	if (hostile_present(temp)) {
		for (int i = 0; i < figures.size(); i++) {
			if (figures[i].point(0) == temp) {
				detach(figures[i]);
				figures.erase(i);
			}
		}
	}
	curr_figure->move(temp.x - start.x, temp.y - start.y);
	if (start != curr_figure->point(0)) {
		if (start == upper_left)black_l_rook = false;
		if (start == upper_right)black_r_rook = false;
		if (start == lower_left)white_l_rook = false;
		if (start == lower_right)white_r_rook = false;
		reset_f_color();
		if (c_turn == Color::white)c_turn = Color::black;
		else c_turn = Color::white;
		figure_selected = false;
		curr_figure = nullptr;
	}
}
void Chess_window::knight_movement(Point& p) {
	Point start{ curr_figure->point(0) };
	Point temp{ curr_figure->point(0) };
	if (p.x == start.x - sz && p.y == start.y - (sz * 2)) {								//2 hoch, 1 links
		temp.x -= sz;
		temp.y -= (sz * 2);
	}
	if (p.x == start.x + sz && p.y == start.y - (sz * 2)) {								//2 hoch, 1 rechts
		temp.x += sz;
		temp.y -= (sz * 2);
	}
	if (p.x == start.x + sz * 2 && p.y == start.y - sz) {								//2 rechts, 1 hoch
		temp.x += (sz * 2);
		temp.y -= sz;
	}
	if (p.x == start.x + sz * 2 && p.y == start.y + sz) {								//2 rechts, 1 runter
		temp.x += (sz * 2);
		temp.y += sz;
	}
	if (p.x == start.x + sz && p.y == start.y + sz * 2) {								//2 runter, 1 rechts
		temp.x += sz;
		temp.y += (sz * 2);
	}
	if (p.x == start.x - sz && p.y == start.y + sz * 2) {								//2 runter, 1 links
		temp.x -= sz;
		temp.y += (sz * 2);
	}
	if (p.x == start.x - (sz * 2) && p.y == start.y + sz) {								//2 links, 1 runter
		temp.x -= (sz * 2);
		temp.y += sz;
	}
	if (p.x == start.x - (sz * 2) && p.y == start.y - sz) {								//2 links, 1 hoch
		temp.x -= (sz * 2);
		temp.y -= sz;
	}
	if (hostile_present(temp)) {
		for (int i = 0; i < figures.size(); i++) {
			if (figures[i].point(0) == temp) {
				detach(figures[i]);
				figures.erase(i);
			}
		}
	}
	curr_figure->move(temp.x - start.x, temp.y - start.y);
	if (start != curr_figure->point(0)) {
		reset_f_color();
		if (c_turn == Color::white)c_turn = Color::black;
		else c_turn = Color::white;
		figure_selected = false;
		curr_figure = nullptr;
	}

}
void Chess_window::bishop_movement(Point& p) {
	Point start{ curr_figure->point(0) };
	Point temp{ curr_figure->point(0) };
	int x = 0;
	int y = 0;
	if (p.x > start.x && p.y < start.y) {			//nach oben rechts
		x = sz;
		y = -sz;
	}
	if (p.x < start.x && p.y > start.y) {			//nach unten links
		x = -sz;
		y = sz;
	}
	if (p.x > start.x && p.y > start.y) {			//nach unten rechts
		x = sz;
		y = sz;
	}
	if (p.x < start.x && p.y < start.y) {		//nach oben links
		x = -sz;
		y = -sz;
	}
	while (temp != p) {
		temp.x += x;
		temp.y += y;
		if (!tile_empty(temp) && temp != p) {
			temp = start;
			x = 0;
			y = 0;
			break;
		}
	}
	if (hostile_present(temp)) {
		for (int i = 0; i < figures.size(); i++) {
			if (figures[i].point(0) == temp) {
				detach(figures[i]);
				figures.erase(i);
			}
		}
	}
	curr_figure->move(temp.x - start.x, temp.y - start.y);
	if (start != curr_figure->point(0)) {
		reset_f_color();
		if (c_turn == Color::white)c_turn = Color::black;
		else c_turn = Color::white;
		figure_selected = false;
		curr_figure = nullptr;
	}
}
void Chess_window::queen_movement(Point& p) {
	Point start{ curr_figure->point(0) };
	Point temp{ curr_figure->point(0) };
	int x = 0;
	int y = 0;
	if (p.x == start.x && p.y < start.y)y = -sz;		//nach oben
	if (p.x == start.x && p.y > start.y) y = sz;		//nach unten
	if (p.x > start.x && p.y == start.y)x = sz;			//nach rechts
	if (p.x < start.x && p.y == start.y)x = -sz;		//nach links
	if (p.x > start.x && p.y < start.y) {			//nach oben rechts
		x = sz;
		y = -sz;
	}
	if (p.x < start.x && p.y > start.y) {			//nach unten links
		x = -sz;
		y = sz;
	}
	if (p.x > start.x && p.y > start.y) {			//nach unten rechts
		x = sz;
		y = sz;
	}
	if (p.x < start.x && p.y < start.y) {		//nach oben links
		x = -sz;
		y = -sz;
	}
	while (temp != p) {
		temp.x += x;
		temp.y += y;
		if (!tile_empty(temp) && temp != p) {
			temp = start;
			x = 0;
			y = 0;
			break;
		}
	}
	if (hostile_present(temp)) {
		for (int i = 0; i < figures.size(); i++) {
			if (figures[i].point(0) == temp) {
				detach(figures[i]);
				figures.erase(i);
			}
		}
	}
	curr_figure->move(temp.x - start.x, temp.y - start.y);
	if (start != curr_figure->point(0)) {
		reset_f_color();
		if (c_turn == Color::white)c_turn = Color::black;
		else c_turn = Color::white;
		figure_selected = false;
		curr_figure = nullptr;
	}
}
void Chess_window::king_movement(Point& p) {
	Point start{ curr_figure->point(0) };
	Point temp{ curr_figure->point(0) };
	Chess_figure* f_temp = nullptr;
	if (!hostile_present(p) && !tile_empty(p))f_temp = get_figure(p);
	if (p.x == start.x + sz && p.y == start.y + sz && (tile_empty(p) || hostile_present(p)) && !tile_in_check(p)) {	//unten rechts
		temp.x += sz;
		temp.y += sz;
	}
	if (p.x == start.x && p.y == start.y + sz && (tile_empty(p) || hostile_present(p)) && !tile_in_check(p)) {		//unten
		temp.y += sz;
	}
	if (p.x == start.x + sz && p.y == start.y && (tile_empty(p) || hostile_present(p)) && !tile_in_check(p)) {		//rechts
		temp.x += sz;
	}
	if (p.x == start.x - sz && p.y == start.y - sz && (tile_empty(p) || hostile_present(p)) && !tile_in_check(p)) {	//oben links
		temp.x -= sz;
		temp.y -= sz;
	}
	if (p.x == start.x && p.y == start.y - sz && (tile_empty(p) || hostile_present(p)) && !tile_in_check(p)) {		//oben
		temp.y -= sz;
	}
	if (p.x == start.x - sz && p.y == start.y && (tile_empty(p) || hostile_present(p)) && !tile_in_check(p)) {		//links
		temp.x -= sz;
	}
	if (p.x == start.x - sz && p.y == start.y + sz && (tile_empty(p) || hostile_present(p)) && !tile_in_check(p)) {	//unten links
		temp.x -= sz;
		temp.y += sz;
	}
	if (p.x == start.x + sz && p.y == start.y - sz && (tile_empty(p) || hostile_present(p)) && !tile_in_check(p)) {	//oben rechts
		temp.x += sz;
		temp.y -= sz;
	}
	if (p.x < start.x && f_temp && f_temp->what_kind() == F_kind::rook) {
		if (castling(p)) {
			temp.x -= (2 * sz);
			Chess_figure* R = get_figure(p);
			R->move((sz * 3), 0);
		}
	}
	if (p.x > start.x && f_temp && f_temp->what_kind() == F_kind::rook) {
		if (castling(p)) {

			temp.x += (2 * sz);
			Chess_figure* R = get_figure(p);
			R->move(-(sz * 2), 0);
		}
	}
	if (hostile_present(temp)) {
		for (int i = 0; i < figures.size(); i++) {
			if (figures[i].point(0) == temp) {
				detach(figures[i]);
				figures.erase(i);
			}
		}
	}
	curr_figure->move(temp.x - start.x, temp.y - start.y);
	if (start != curr_figure->point(0)) {
		reset_f_color();
		if (c_turn == Color::white) {
			white_king = false;
			c_turn = Color::black;
		}
		else {
			black_king = false;
			c_turn = Color::white;
		}
		figure_selected = false;
		curr_figure = nullptr;
	}
}

void Chess_window::tile_pressed() {
	Chess_figure* temp_figure = nullptr;
	Point p = get_point(Fl::event_x(), Fl::event_y());									//p=Koordinaten des aktuell gedr�cketen Feldes
	if (!tile_empty(p)) temp_figure = get_figure(p);


	if (!tile_empty(p) && !figure_selected) {	//Spielstein ausw�hlen
		Chess_figure* temp = get_figure(p);
		if (temp->fill_color() == c_turn) {
			curr_figure = get_figure(p);
			if (curr_figure->fill_color() == c_turn) {
				curr_figure->set_color(Color::cyan);
				figure_selected = true;
			}
		}
	}
	else {
		if (!tile_empty(p) && figure_selected && !hostile_present(p) && !(curr_figure->what_kind() == F_kind::king && temp_figure->what_kind() == F_kind::rook)) {					//Auswahl �ndern
			reset_f_color();
			curr_figure = get_figure(p);
			if (curr_figure->fill_color() == c_turn) curr_figure->set_color(Color::cyan);
		}
	}

	if (curr_figure && (tile_empty(p) || hostile_present(p) || temp_figure->what_kind() == F_kind::rook)) {
		F_kind c_kind = curr_figure->what_kind();

		switch (c_kind) {
		case F_kind::pawn:
			pawn_movement(p);
			break;
		case F_kind::rook:
			rook_movement(p);
			break;
		case F_kind::knight:
			knight_movement(p);
			break;
		case F_kind::bishop:
			bishop_movement(p);
			break;
		case F_kind::queen:
			queen_movement(p);
			break;
		case F_kind::king:
			king_movement(p);
			break;
		}

	}

	check_ep();											//aktualisiert ep
	refresh_figures();									//detached und attached alle Figuren nochmal, damit die nicht unter den Feldern verschwinden
	Fl::redraw();
	cout << "p: " << p.x << " " << p.y << endl;			//Nur zum debuggen
}


int main() try {
	Chess_window win{ {0,0},1000,800,"Schach" };

	win.wait_for_button();
}
catch (exception& e) {

	cerr << e.what();
	cin.get();
}