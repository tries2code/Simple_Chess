
#include"Chess_figures.h"
#include <FL/fl_ask.H>

#undef vector

constexpr int sz = 100;									//Kommt vielleicht später in die Fenster Klasse, wäre evt besser ein double um Schwulitäten zu vermeiden wenn sz nicht 100 ist
constexpr int ls = 100;									//Abstand zum linken Fensterrand
constexpr int us = 0;									//Abstand zum oberen Fensterrand


//Helfer Funktionen
Point get_point(int x, int y) {							//Ermittelt Koordinaten für Spielfeld
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
	Chess_figure* black_king_ptr = nullptr;
	Chess_figure* white_king_ptr = nullptr;
	Color c_turn = Color::white;

	Button restart_button;
	Button undo_button;
	Button quit_button;

	Vector_ref<Button>field_buttons;					//Für die Knöppe unter den schwarzen Feldern
	Vector_ref< Graph_lib::Rectangle>tiles;				//Für die Felder
	Vector_ref<Chess_figure>figures;
	vector<Point>ep;									//Mögliche en passant Ziele
	vector<Point>white_castling;
	vector<Point>black_castling;

	//Buffers////////
	bool b_figure_selected;
	bool b_white_l_rook = true;
	bool b_white_king = true;
	bool b_white_r_rook = true;

	bool b_black_l_rook = true;
	bool b_black_king = true;
	bool b_black_r_rook = true;

	Point b_curr_figure;
	Chess_figure* b_black_king_ptr = nullptr;
	Chess_figure* b_white_king_ptr = nullptr;
	Color b_c_turn = Color::white;

	Vector_ref<Chess_figure>b_figures;
	vector<Point>b_ep;
	vector<Point>b_white_castling;
	vector<Point>b_black_castling;

public:
	Chess_window(Point xy, int w, int h, const string& title);
	void wait_for_button();

private:
	bool valid_tile(Point&);
	bool tile_empty(Point&)const;
	bool hostile_present(Point&);
	bool is_ep(Point&);									//Prüft ob es sich bei der übergebenen Koordinate um eine en passanr Ziel handelt
	void check_ep();									//Prüft und aktualisiert die Liste der möglichen en passant Ziele
	Chess_figure* get_figure(Point&);
	void reset_f_color();								//Stellt die ursprüngliche Umrandungsfarbe wieder her
	void refresh_figures();								//Ohne diese Funktion könne Figuren hinter den Felder verschwinden. Keine Ahnung warum.
	bool castling(Point&);
	bool tile_in_check(Point&);
	bool king_in_check();
	bool checkmate();
	void copy_to_buffer();
	void undo();

	void pawn_promotion();
	void pawn_movement(Point&);
	void rook_movement(Point&);
	void knight_movement(Point&);
	void bishop_movement(Point&);
	void queen_movement(Point&);
	void king_movement(Point& p);

	static void cb_restart(Address, Address addr) { static_cast<Chess_window*>(addr)->restart_game(); }
	static void cb_undo(Address, Address addr) { static_cast<Chess_window*>(addr)->undo(); }
	static void cb_quit(Address, Address addr) { reference_to<Chess_window>(addr).quit(); }		//Ist die Stroustrup Variante, macht auch nur 		return *static_cast<W*>(pw);
	static void cb_tile_pressed(Address, Address addr) { reference_to<Chess_window>(addr).tile_pressed(); }

	void restart_game();
	void quit() { hide(); button_pushed = true; }
	void tile_pressed();
};

Chess_window::Chess_window(Point xy, int w, int h, const string& title) :Window(xy, w, h, title),
button_pushed(false), figure_selected(false),
restart_button(Point(x_max() - 70, 0), 70, 20, "Restart", cb_restart),
undo_button(Point(x_max() - 70, 30), 70, 20, "Undo", cb_undo),
quit_button(Point(x_max() - 70, 60), 70, 20, "Quit", cb_quit) {

	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			//Knöppe unter den Feldern
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

			//King Pointer
			if (figures[figures.size() - 1].fill_color() == Color::black && figures[figures.size() - 1].what_kind() == F_kind::king)black_king_ptr = &figures[figures.size() - 1];
			if (figures[figures.size() - 1].fill_color() == Color::white && figures[figures.size() - 1].what_kind() == F_kind::king)white_king_ptr = &figures[figures.size() - 1];
		}
	}
	attach(restart_button);
	attach(undo_button);
	attach(quit_button);
	size_range(w, h, w, h);	//Fixiert Fenster
}
void Chess_window::wait_for_button() {
	while (!button_pushed) Fl::wait();
	button_pushed = false;
	Fl::redraw();
}
bool Chess_window::valid_tile(Point& p) {
	if (p.x < ls || p.x > ls + 7 * sz || p.y < us || p.y > us + 7 * sz)return false;
	return true;
}
bool Chess_window::tile_empty(Point& p)const {
	Point curr{ p.x ,p.y };
	if (p.x < ls || p.x > ls + 7 * sz || p.y < us || p.y > us + 7 * sz)return false;
	for (int i = 0; i < figures.size(); i++) {
		if (figures[i].point(0) == curr)return false;
	}
	return true;
}
bool Chess_window::hostile_present(Point& p) {
	Chess_figure* res;
	if (p.x < ls || p.x > ls + 7 * sz || p.y < us || p.y > us + 7 * sz)return false;
	if (!tile_empty(p))res = get_figure(p);
	else return false;
	if (c_turn == res->fill_color())return false;
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
	//Prüfung Pferd
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
	//Prüfung Bauer oder König
	temp = p;
	temp.x += sz;
	temp.y += sz;
	if (hostile_present(temp)) {
		hostile = get_figure(temp);
		if (c_turn == Color::black && hostile->what_kind() == F_kind::pawn)return true;
		if (hostile->what_kind() == F_kind::king)return true;

	}
	temp = p;
	temp.x -= sz;
	temp.y += sz;
	if (hostile_present(temp)) {
		hostile = get_figure(temp);
		if (c_turn == Color::black && hostile->what_kind() == F_kind::pawn)return true;
		if (hostile->what_kind() == F_kind::king)return true;
	}
	temp = p;
	temp.x -= sz;
	temp.y -= sz;
	if (hostile_present(temp)) {
		hostile = get_figure(temp);
		if (c_turn == Color::white && hostile->what_kind() == F_kind::pawn)return true;
		if (hostile->what_kind() == F_kind::king)return true;
	}
	temp = p;
	temp.x += sz;
	temp.y -= sz;
	if (hostile_present(temp)) {
		hostile = get_figure(temp);
		if (c_turn == Color::white && hostile->what_kind() == F_kind::pawn)return true;
		if (hostile->what_kind() == F_kind::king)return true;
	}
	temp = p;
	temp.x += sz;
	if (hostile_present(temp)) {
		hostile = get_figure(temp);
		if (hostile->what_kind() == F_kind::king)return true;
	}
	temp = p;
	temp.x -= sz;
	if (hostile_present(temp)) {
		hostile = get_figure(temp);
		if (hostile->what_kind() == F_kind::king)return true;
	}
	temp = p;
	temp.y -= sz;
	if (hostile_present(temp)) {
		hostile = get_figure(temp);
		if (hostile->what_kind() == F_kind::king)return true;
	}
	temp = p;
	temp.y += sz;
	if (hostile_present(temp)) {
		hostile = get_figure(temp);
		if (hostile->what_kind() == F_kind::king)return true;
	}
	//Prüfung Dame und Turm
	temp = p;
	while (temp.x <= ls + 7 * sz) {
		temp.x += sz;
		if (hostile_present(temp)) {
			hostile = get_figure(temp);
			if (hostile->what_kind() == F_kind::rook)return true;
			if (hostile->what_kind() == F_kind::queen)return true;
			break;
		}
		if (!tile_empty(temp) && black_king_ptr->point(0) != temp && white_king_ptr->point(0) != temp)break;
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
		if (!tile_empty(temp) && black_king_ptr->point(0) != temp && white_king_ptr->point(0) != temp)break;
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
		if (!tile_empty(temp) && black_king_ptr->point(0) != temp && white_king_ptr->point(0) != temp)break;
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
		if (!tile_empty(temp) && black_king_ptr->point(0) != temp && white_king_ptr->point(0) != temp)break;
	}
	//Prüfung Läufer oder Dame
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
		if (!tile_empty(temp) && black_king_ptr->point(0) != temp && white_king_ptr->point(0) != temp)break;
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
		if (!tile_empty(temp) && black_king_ptr->point(0) != temp && white_king_ptr->point(0) != temp)break;
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
		if (!tile_empty(temp) && black_king_ptr->point(0) != temp && white_king_ptr->point(0) != temp)break;
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
		if (!tile_empty(temp) && black_king_ptr->point(0) != temp && white_king_ptr->point(0) != temp)break;
	}
	return false;
}
bool Chess_window::king_in_check() {
	Chess_figure* temp_ptr = nullptr;
	c_turn == Color::black ? temp_ptr = black_king_ptr : temp_ptr = white_king_ptr;
	Point temp{ temp_ptr->point(0) };
	if (tile_in_check(temp))return true;
	return false;
}
bool Chess_window::checkmate() {

	if (!king_in_check())return false;

	Point p;
	c_turn == Color::black ? p = black_king_ptr->point(0) : p = white_king_ptr->point(0);
	Point pp = p;

	pp.x += sz;												//rechts
	if (valid_tile(pp) && !tile_in_check(pp) && (tile_empty(pp) || hostile_present(pp)))return false;
	pp = p;
	pp.x -= sz;												//links
	if (valid_tile(pp) && !tile_in_check(pp) && (tile_empty(pp) || hostile_present(pp)))return false;
	pp = p;
	pp.y -= sz;												//oben
	if (valid_tile(pp) && !tile_in_check(pp) && (tile_empty(pp) || hostile_present(pp)))return false;
	pp = p;
	pp.y += sz;												//unten
	if (valid_tile(pp) && !tile_in_check(pp) && (tile_empty(pp) || hostile_present(pp)))return false;
	pp = p;
	pp.x += sz;												//unten rechts
	pp.y += sz;
	if (valid_tile(pp) && !tile_in_check(pp) && (tile_empty(pp) || hostile_present(pp)))return false;
	pp = p;
	pp.x -= sz;												//unten links
	pp.y += sz;
	if (valid_tile(pp) && !tile_in_check(pp) && (tile_empty(pp) || hostile_present(pp)))return false;
	pp = p;
	pp.x += sz;												//oben rechts
	pp.y -= sz;
	if (valid_tile(pp) && !tile_in_check(pp) && (tile_empty(pp) || hostile_present(pp)))return false;
	pp = p;
	pp.x -= sz;												//oben links
	pp.y -= sz;
	if (valid_tile(pp) && !tile_in_check(pp) && (tile_empty(pp) || hostile_present(pp)))return false;

	Point p_table;								//rettet curr_figure's Koordinateb falls checkmate hier weiter macht. curr_figure wird hinterher wieder hergestellt
	bool p_t = false;
	if (curr_figure) {
		p_table = curr_figure->point(0);
		p_t = true;
	}

	for (int i = 0; i < tiles.size(); i++) {

		Point p{ tiles[i].point(0) };

		if ((tile_empty(p) || hostile_present(p))) {

			for (int j = 0; j < figures.size(); j++) {

				if (figures[j].fill_color() == c_turn) {

					curr_figure = &figures[j];
					F_kind c_kind = curr_figure->what_kind();
					copy_to_buffer();

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
					pawn_promotion();

					if (!king_in_check()) {
						undo();
						if (p_t) {												//Wiederherstellung curr_figure
							for (int i = 0; i < figures.size(); i++) {
								if (figures[i].point(0) == p_table)curr_figure = &figures[i];
							}
						}
						else curr_figure = nullptr;
						return false;
					}
					else {
						undo();
						curr_figure = nullptr;
					}
				}
			}
		}
	}
	return true;
}
void Chess_window::copy_to_buffer() {

	//curr_figers Position zwischenspeichern
	b_curr_figure = curr_figure->point(0);				//Muss anhand von point(0) neu ermittelt werden, da curr_figure ein Pointer ist

	//Buffer Vektoren leeren

	for (int i = 0; i < b_figures.size(); i++) {
		//if (&b_figures[i] != curr_figure) {
		Chess_figure* temp{ &b_figures[i] };
		delete temp;
		//}
	}
	b_figures.clear();
	b_ep.clear();
	b_white_castling.clear();
	b_black_castling.clear();

	//Bools buffern
	b_figure_selected = figure_selected;
	b_white_l_rook = white_l_rook;
	b_white_king = white_king;
	b_white_r_rook = white_r_rook;
	b_black_l_rook = black_l_rook;
	b_black_king = black_king;
	b_black_r_rook = black_r_rook;

	//Damit der andere Spieler wieder dran ist
	b_c_turn = c_turn;

	//Buffern der Figuren, man muss im Buffer neue Figuren erstellen, damit sich die Figuren im Buffer nicht mitbewegen
	for (int i = 0; i < figures.size(); i++) {

		Point p{ figures[i].point(0) };
		Color lc{ figures[i].color() };
		Color fc{ figures[i].fill_color() };

		if (figures[i].what_kind() == F_kind::pawn)
			b_figures.push_back(new Pawn{ {p.x,p.y},sz });

		if (figures[i].what_kind() == F_kind::rook)
			b_figures.push_back(new Rook{ {p.x,p.y},sz });

		if (figures[i].what_kind() == F_kind::bishop)
			b_figures.push_back(new Bishop{ {p.x,p.y},sz });

		if (figures[i].what_kind() == F_kind::knight)
			b_figures.push_back(new Knight{ {p.x,p.y},sz });

		if (figures[i].what_kind() == F_kind::queen)
			b_figures.push_back(new Queen{ {p.x,p.y},sz });

		if (figures[i].what_kind() == F_kind::king)
			b_figures.push_back(new King{ {p.x,p.y},sz });

		b_figures[b_figures.size() - 1].set_color(lc);
		b_figures[b_figures.size() - 1].set_fill_color(fc);
	}

	//Vektoren buffern
	b_ep = ep;
	b_white_castling = white_castling;
	b_black_castling = black_castling;
}
void Chess_window::undo() {

	for (int j = 0; j < figures.size(); j++) {//Alte Figurenvom Bildschirm löschen
		detach(figures[j]);
		Chess_figure* temp{ &figures[j] };
		delete temp;
	}
	//Vektoren leeren
	figures.clear();
	ep.clear();
	white_castling.clear();
	black_castling.clear();

	//Pointer zurücksetzen
	black_king_ptr = nullptr;
	white_king_ptr = nullptr;
	curr_figure = nullptr;

	//Vektoren aus dem Buffer laden
	for (int i = 0; i < b_figures.size(); i++) {

		Point p{ b_figures[i].point(0) };
		Color lc{ b_figures[i].color() };
		Color fc{ b_figures[i].fill_color() };

		if (b_figures[i].what_kind() == F_kind::pawn)
			figures.push_back(new Pawn{ {p.x,p.y},sz });

		if (b_figures[i].what_kind() == F_kind::rook)
			figures.push_back(new Rook{ {p.x,p.y},sz });

		if (b_figures[i].what_kind() == F_kind::bishop)
			figures.push_back(new Bishop{ {p.x,p.y},sz });

		if (b_figures[i].what_kind() == F_kind::knight)
			figures.push_back(new Knight{ {p.x,p.y},sz });

		if (b_figures[i].what_kind() == F_kind::queen)
			figures.push_back(new Queen{ {p.x,p.y},sz });

		if (b_figures[i].what_kind() == F_kind::king)
			figures.push_back(new King{ {p.x,p.y},sz });

		figures[figures.size() - 1].set_color(lc);
		figures[figures.size() - 1].set_fill_color(fc);
	}
	ep = b_ep;
	white_castling = b_white_castling;
	black_castling = b_white_castling;

	//Auf Fenster Platzieren, Adressen für Pointer ermitteln
	for (int i = 0; i < figures.size(); i++) {
		attach(figures[i]);
		if (figures[i].what_kind() == F_kind::king && figures[i].fill_color() == Color::black)black_king_ptr = &figures[i];
		if (figures[i].what_kind() == F_kind::king && figures[i].fill_color() == Color::white)white_king_ptr = &figures[i];
		if (figures[i].point(0) == b_curr_figure) curr_figure = &figures[i];
	}

	//Bools und c_turn aus Buffer wiederhestellen
	figure_selected = b_figure_selected;
	white_l_rook = b_white_l_rook;
	white_king = b_white_king;
	white_r_rook = b_white_r_rook;
	black_l_rook = b_black_l_rook;
	black_king = b_black_king;
	black_r_rook = b_black_r_rook;
	c_turn = b_c_turn;

	Fl::redraw();
}

void Chess_window::pawn_promotion() {

	for (int i = 0; i < 8; i++) {
		Point top{ ls + i * sz,us };
		Point bottom{ ls + i * sz,us + 7 * sz };
		for (int j = 0; j < figures.size(); j++) {
			if (figures[j].point(0) == top && figures[j].what_kind() == F_kind::pawn) {
				detach(figures[j]);
				figures.erase(j);
				figures.push_back(new Queen{ top,sz });
				figures[figures.size() - 1].set_fill_color(Color::white);
			}
			if (figures[j].point(0) == bottom && figures[j].what_kind() == F_kind::pawn) {
				detach(figures[j]);
				figures.erase(j);
				figures.push_back(new Queen{ bottom,sz });
				figures[figures.size() - 1].set_fill_color(Color::black);
				figures[figures.size() - 1].set_color(Color::white);
			}
		}
	}

}
void Chess_window::pawn_movement(Point& p) {
	Point start{ curr_figure->point(0) };
	Point temp;
	Chess_figure* temp_target = nullptr;
	int x = 0;
	int y = 0;
	if (curr_figure->fill_color() == Color::white) {

		if (p.x == curr_figure->point(0).x - sz && p.y == curr_figure->point(0).y - sz && hostile_present(p)) {
			x = -sz;
			y = -sz;
		}
		temp = { start.x - sz, us + sz * 3 };
		if (p.x == curr_figure->point(0).x - sz && p.y == curr_figure->point(0).y - sz && is_ep(temp)) {
			x = -sz;
			y = -sz;
			for (int i = 0; i < figures.size(); i++) {
				if (figures[i].point(0) == temp && figures[i].fill_color() == Color::black) {
					detach(figures[i]);
					temp_target = &figures[i];
					delete temp_target;
					figures.erase(i);
				}
			}
		}
		if (p.x == curr_figure->point(0).x + sz && p.y == curr_figure->point(0).y - sz && hostile_present(p)) {
			x = sz;
			y = -sz;
		}
		temp = { start.x + sz, us + sz * 3 };
		if (p.x == curr_figure->point(0).x + sz && p.y == curr_figure->point(0).y - sz && is_ep(temp)) {
			x = sz;
			y = -sz;
			for (int i = 0; i < figures.size(); i++) {
				if (figures[i].point(0) == temp && figures[i].fill_color() == Color::black) {
					detach(figures[i]);
					temp_target = &figures[i];
					delete temp_target;
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
		temp = { start.x - sz, us + sz * 4 };
		if (p.x == curr_figure->point(0).x - sz && p.y == curr_figure->point(0).y + sz && is_ep(temp)) {
			x = -sz;
			y = sz;
			for (int i = 0; i < figures.size(); i++) {
				if (figures[i].point(0) == temp && figures[i].fill_color() == Color::white) {
					detach(figures[i]);
					temp_target = &figures[i];
					delete temp_target;
					figures.erase(i);
				}
			}
		}
		if (p.x == curr_figure->point(0).x + sz && p.y == curr_figure->point(0).y + sz && hostile_present(p)) {
			x = sz;
			y = sz;
		}
		temp = { start.x + sz, us + sz * 4 };
		if (p.x == curr_figure->point(0).x + sz && p.y == curr_figure->point(0).y + sz && is_ep(temp)) {
			x = sz;
			y = sz;
			for (int i = 0; i < figures.size(); i++) {
				if (figures[i].point(0) == temp && figures[i].fill_color() == Color::white) {
					detach(figures[i]);
					temp_target = &figures[i];
					delete temp_target;
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
	if (hostile_present(target))temp_target = get_figure(target);
	curr_figure->move(x, y);
	if (king_in_check())curr_figure->move(-(x), -(y));
	if (start != curr_figure->point(0)) {
		if (temp_target) {
			for (int i = 0; i < figures.size(); i++) {
				if (&figures[i] == temp_target) {
					detach(figures[i]);
					delete temp_target;
					figures.erase(i);
				}
			}
		}
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
	Chess_figure* temp_target = nullptr;
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
	if (hostile_present(temp))temp_target = get_figure(temp);
	curr_figure->move(temp.x - start.x, temp.y - start.y);
	if (king_in_check())curr_figure->move(start.x - temp.x, start.y - temp.y);
	if (start != curr_figure->point(0)) {
		if (start == upper_left)black_l_rook = false;
		if (start == upper_right)black_r_rook = false;
		if (start == lower_left)white_l_rook = false;
		if (start == lower_right)white_r_rook = false;
		if (temp_target) {
			for (int i = 0; i < figures.size(); i++) {
				if (&figures[i] == temp_target) {
					detach(figures[i]);
					delete temp_target;
					figures.erase(i);
				}
			}
		}
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
	Chess_figure* temp_target = nullptr;
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
	if (hostile_present(temp)) temp_target = get_figure(temp);
	curr_figure->move(temp.x - start.x, temp.y - start.y);
	if (king_in_check())curr_figure->move(start.x - temp.x, start.y - temp.y);
	if (start != curr_figure->point(0)) {
		if (temp_target) {
			for (int i = 0; i < figures.size(); i++) {
				if (&figures[i] == temp_target) {
					detach(figures[i]);
					delete temp_target;
					figures.erase(i);
				}
			}
		}
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
	Chess_figure* temp_target = nullptr;
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
	if (hostile_present(temp))temp_target = get_figure(temp);

	curr_figure->move(temp.x - start.x, temp.y - start.y);
	if (king_in_check())curr_figure->move(start.x - temp.x, start.y - temp.y);
	if (start != curr_figure->point(0)) {
		if (temp_target) {
			for (int i = 0; i < figures.size(); i++) {
				if (&figures[i] == temp_target) {
					detach(figures[i]);
					delete temp_target;
					figures.erase(i);
				}
			}
		}
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
	Chess_figure* temp_target = nullptr;
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
	if (hostile_present(temp)) temp_target = get_figure(temp);
	curr_figure->move(temp.x - start.x, temp.y - start.y);
	if (king_in_check())curr_figure->move(start.x - temp.x, start.y - temp.y);
	if (start != curr_figure->point(0)) {
		if (temp_target) {
			for (int i = 0; i < figures.size(); i++) {
				if (&figures[i] == temp_target) {
					detach(figures[i]);
					delete temp_target;
					figures.erase(i);
				}
			}
		}
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
	if ((tile_empty(p) || hostile_present(p)) && !tile_in_check(p)) {
		if (p.x == start.x + sz && p.y == start.y + sz) {	//unten rechts
			temp.x += sz;
			temp.y += sz;
		}
		if (p.x == start.x && p.y == start.y + sz) {		//unten
			temp.y += sz;
		}
		if (p.x == start.x + sz && p.y == start.y) {		//rechts
			temp.x += sz;
		}
		if (p.x == start.x - sz && p.y == start.y - sz) {	//oben links
			temp.x -= sz;
			temp.y -= sz;
		}
		if (p.x == start.x && p.y == start.y - sz) {		//oben
			temp.y -= sz;
		}
		if (p.x == start.x - sz && p.y == start.y) {		//links
			temp.x -= sz;
		}
		if (p.x == start.x - sz && p.y == start.y + sz) {	//unten links
			temp.x -= sz;
			temp.y += sz;
		}
		if (p.x == start.x + sz && p.y == start.y - sz) {	//oben rechts
			temp.x += sz;
			temp.y -= sz;
		}
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
				Chess_figure* temp_ptr = &figures[i];
				delete temp_ptr;
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

void Chess_window::restart_game() {
	figure_selected = false;

	white_l_rook = true;
	white_king = true;
	white_r_rook = true;

	black_l_rook = true;
	black_king = true;
	black_r_rook = true;

	curr_figure = nullptr;
	black_king_ptr = nullptr;
	white_king_ptr = nullptr;
	c_turn = Color::white;

	for (int i = 0; i < figures.size(); i++) detach(figures[i]);
	figures.clear();
	ep.clear();
	white_castling.clear();
	black_castling.clear();
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {

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

			//King Pointer
			if (figures[figures.size() - 1].fill_color() == Color::black && figures[figures.size() - 1].what_kind() == F_kind::king)black_king_ptr = &figures[figures.size() - 1];
			if (figures[figures.size() - 1].fill_color() == Color::white && figures[figures.size() - 1].what_kind() == F_kind::king)white_king_ptr = &figures[figures.size() - 1];

		}
	}
Fl:redraw();
}
void Chess_window::tile_pressed() {

	Chess_figure* temp_figure = nullptr;
	Point p = get_point(Fl::event_x(), Fl::event_y());									//p=Koordinaten des aktuell gedrücketen Feldes
	if (!tile_empty(p)) temp_figure = get_figure(p);

	if (!tile_empty(p) && !curr_figure) {	//Spielstein auswählen
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
		if (!tile_empty(p) && figure_selected && !hostile_present(p) && !(curr_figure->what_kind() == F_kind::king && temp_figure->what_kind() == F_kind::rook)) {					//Auswahl ändern
			reset_f_color();
			curr_figure = get_figure(p);
			if (curr_figure->fill_color() == c_turn) curr_figure->set_color(Color::cyan);
		}
	}
	if (curr_figure && (tile_empty(p) || hostile_present(p) || temp_figure->what_kind() == F_kind::rook)) {
		F_kind c_kind = curr_figure->what_kind();

		copy_to_buffer();		//Wird hier nur gebraucht falls man einen Undo Knopf einbauen möchte

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

	pawn_promotion();
	check_ep();											//aktualisiert ep
	refresh_figures();									//detached und attached alle Figuren nochmal, damit die nicht unter den Feldern verschwinden
	Fl::redraw();

	if (checkmate()) {									// Spielende
		switch (fl_choice("Checkmate! New game?", "Yes", "No", 0)) {
		case 0:
			restart_game();								// Spielbrett neu aufbauen
			break;
		case 1:
			quit();
			break;
		}
	}
}


int main() try {
	Chess_window win{ {100,100},1000,y_max(),"Schach" };


	win.wait_for_button();
}
catch (exception& e) {

	cerr << e.what();
	cin.get();
}


