#include"Chess_figures.h"
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

	Chess_figure* curr_figure = nullptr;
	Color c_turn = Color::white;

	Button quit_button;

	Vector_ref<Button>field_buttons;					//Für die Knöppe unter den schwarzen Feldern
	Vector_ref< Graph_lib::Rectangle>tiles;				//Für die Felder
	Vector_ref<Chess_figure>figures;
	vector<Point>ep;									//Mögliche en passant Ziele
public:
	Chess_window(Point xy, int w, int h, const string& title);
	void wait_for_button();

private:
	bool tile_empty(Point&)const;
	bool hostile_present(Point&);
	bool is_ep(Point&);									//Prüft ob es sich bei der übergebenen Koordinate um eine en passanr Ziel handelt
	void check_ep();									//Prüft und aktualisiert die Liste der möglichen en passant Ziele
	Chess_figure* get_figure(Point&);
	void reset_f_color();								//Stellt die ursprüngliche Umrandungsfarbe wieder her
	void refresh_figures();								//Ohne diese Funktion könne Figuren hinter den Felder verschwinden. Keine Ahnung warum.

	void pawn_movement(Point&);


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
			//Knöppe unter den Feldern
			field_buttons.push_back(new Button{ {ls + x * sz,us + y * sz},sz,sz ," ",cb_tile_pressed });
			attach(field_buttons[field_buttons.size() - 1]);

			//Felder
			tiles.push_back(new Graph_lib::Rectangle{ {ls + x * sz,us + y * sz},sz,sz });
			tiles[tiles.size() - 1].set_color(fl_rgb_color(153, 76, 0));
			if ((y + x) % 2 == 0)tiles[tiles.size() - 1].set_fill_color(fl_rgb_color(204, 102, 0));
			if ((y + x) % 2 != 0)tiles[tiles.size() - 1].set_fill_color(fl_rgb_color(255, 255, 204));

			attach(tiles[tiles.size() - 1]);

			//Figuren
			if ((y == 0 || y == 7) && (x == 0 || x == 7))figures.push_back(new Rook{ {ls + x * sz ,us + y * sz },sz });
			if ((y == 0 || y == 7) && (x == 1 || x == 6))figures.push_back(new Knight{ {ls + x * sz ,us + y * sz },sz });
			if ((y == 0 || y == 7) && (x == 2 || x == 5))figures.push_back(new Bishop{ {ls + x * sz ,us + y * sz },sz });
			if ((y == 0 || y == 7) && (x == 3))figures.push_back(new King{ {ls + x * sz ,us + y * sz },sz });
			if ((y == 0 || y == 7) && (x == 4))figures.push_back(new Queen{ {ls + x * sz ,us + y * sz },sz });
			if (y == 1 || y == 6)figures.push_back(new Pawn{ {ls + x * sz ,us + y * sz },sz });
			attach(figures[figures.size() - 1]);

			if (y == 0 || y == 1) { figures[figures.size() - 1].set_color(Color::white); figures[figures.size() - 1].set_fill_color(Color::black); }
			if (y == 6 || y == 7)figures[figures.size() - 1].set_fill_color(Color::white);
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
	if (!tile_empty(p))res = get_figure(p);
	else return false;
	if (curr_figure->fill_color() == res->fill_color())return false;
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
	}
}

void Chess_window::tile_pressed() {

	Point p = get_point(Fl::event_x(), Fl::event_y());									//p=Koordinaten des aktuell gedrücketen Feldes

	if (!tile_empty(p) && !figure_selected) {											//Spielstein auswählen
		curr_figure = get_figure(p);
		if (curr_figure->fill_color() == c_turn) {
			curr_figure->set_color(Color::cyan);
			figure_selected = true;
		}
	}
	else {
		if (!tile_empty(p) && figure_selected && !hostile_present(p)) {					//Auswahl ändern
			reset_f_color();
			curr_figure = get_figure(p);
			if (curr_figure->fill_color() == c_turn) curr_figure->set_color(Color::cyan);
		}
	}
	if (tile_empty(p) || hostile_present(p) && figure_selected) {
		F_kind c_kind = curr_figure->what_kind();

		switch (c_kind) {
		case F_kind::pawn:
			pawn_movement(p);
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