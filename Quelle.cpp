#include"Graph.h"
#include"Simple_window.h"

int sz = 250;									//Kommt später in die Fenster Klasse
int bd = sz / 100;								//Abstand zum unteren Feld Rand

enum class F_kind {								//Art der Spielfigur
	king = 1, queen, bishop, rook, knight, pawn
};
bool operator==(F_kind a, F_kind b) {
	char x = static_cast<char>(a);
	char y = static_cast<char>(b);
	if (x == y)return true;
	return false;
}
bool operator!=(F_kind a, F_kind b) {
	if (!(a == b))  return true;
	return false;
}



struct chess_figure :Shape {
	int ssz = 0;
	F_kind kind;
};

class Pawn :public chess_figure {
	int ssz;
	F_kind kind = F_kind::pawn;
public:
	Pawn(Point p, int x) :ssz(x) { add(p); }
	F_kind what_kind() { return kind; }
private:
	void draw_lines()const;
};
void Pawn::draw_lines()const {
	if (color().visibility()) {

		//Füllung
		fl_color(fill_color().as_int());
		fl_begin_complex_polygon();
		fl_vertex(point(0).x + ssz / 12 * 7, point(0).y + ssz / 12 * 7);
		fl_vertex(point(0).x + ssz / 12 * 5, point(0).y + ssz / 12 * 7);
		fl_vertex(point(0).x + ssz / 5, point(0).y + ssz - bd);
		fl_vertex(point(0).x + ssz / 5 * 4, point(0).y + ssz - bd);
		fl_end_complex_polygon();
		fl_pie(point(0).x + ssz / 3 - bd, point(0).y + ssz / 4, ssz / 3, ssz / 3, 0, 360);

		//Umrandung
		fl_color(color().as_int());
		fl_arc(point(0).x + ssz / 3 - bd, point(0).y + ssz / 4, ssz / 3, ssz / 3, 0, 360);
		fl_line(point(0).x + ssz / 12 * 5, point(0).y + ssz / 12 * 7, point(0).x + ssz / 5, point(0).y + ssz - bd);
		fl_line(point(0).x + ssz / 5, point(0).y + ssz - bd, point(0).x + ssz / 5 * 4, point(0).y + ssz - bd);
		fl_line(point(0).x + ssz / 5 * 4, point(0).y + ssz - bd, point(0).x + ssz / 12 * 7, point(0).y + ssz / 12 * 7);
	}
}

class Knight :public chess_figure {		//Springer/Pferd
	int ssz;
	F_kind kind = F_kind::knight;
public:
	Knight(Point p, int x);

	F_kind what_kind() { return kind; }

private:
	void draw_lines()const;
};

class Rook :public chess_figure {		//Turm
	int ssz;
	F_kind kind = F_kind::rook;
public:
	Rook(Point p, int x) :ssz(x) { add(p); }
	F_kind what_kind() { return kind; }
private:
	void draw_lines()const;
};
void Rook::draw_lines()const {
	if (color().visibility()) {

		//Füllung
		fl_color(fill_color().as_int());
		fl_begin_complex_polygon();
		fl_vertex(point(0).x + ssz / 16 * 3, point(0).y + ssz - bd);
		fl_vertex(point(0).x + ssz / 16 * 3, point(0).y + ssz / 16 * 14);
		fl_vertex(point(0).x + ssz / 16 * 5, point(0).y + ssz / 16 * 14);
		fl_vertex(point(0).x + ssz / 16 * 6, point(0).y + ssz / 16 * 6);
		fl_vertex(point(0).x + ssz / 16 * 3, point(0).y + ssz / 16 * 6);
		fl_vertex(point(0).x + ssz / 16 * 3, point(0).y + ssz / 16 * 2);
		fl_vertex(point(0).x + ssz / 16 * 5, point(0).y + ssz / 16 * 2);
		fl_vertex(point(0).x + ssz / 16 * 5, point(0).y + ssz / 16 * 4);
		fl_vertex(point(0).x + ssz / 16 * 7, point(0).y + ssz / 16 * 4);
		fl_vertex(point(0).x + ssz / 16 * 7, point(0).y + ssz / 16 * 2);
		fl_vertex(point(0).x + ssz / 16 * 9, point(0).y + ssz / 16 * 2);
		fl_vertex(point(0).x + ssz / 16 * 9, point(0).y + ssz / 16 * 4);
		fl_vertex(point(0).x + ssz / 16 * 11, point(0).y + ssz / 16 * 4);
		fl_vertex(point(0).x + ssz / 16 * 11, point(0).y + ssz / 16 * 2);
		fl_vertex(point(0).x + ssz / 16 * 13, point(0).y + ssz / 16 * 2);
		fl_vertex(point(0).x + ssz / 16 * 13, point(0).y + ssz / 16 * 6);
		fl_vertex(point(0).x + ssz / 16 * 10, point(0).y + ssz / 16 * 6);
		fl_vertex(point(0).x + ssz / 16 * 11, point(0).y + ssz / 16 * 14);
		fl_vertex(point(0).x + ssz / 16 * 13, point(0).y + ssz / 16 * 14);
		fl_vertex(point(0).x + ssz / 16 * 13, point(0).y + ssz - bd);
		fl_end_complex_polygon();

		//Umrandung
		fl_color(color().as_int());
		fl_line(point(0).x + ssz / 16 * 3, point(0).y + ssz - bd, point(0).x + ssz / 16 * 3, point(0).y + ssz / 16 * 14);
		fl_line(point(0).x + ssz / 16 * 3, point(0).y + ssz / 16 * 14, point(0).x + ssz / 16 * 5, point(0).y + ssz / 16 * 14);
		fl_line(point(0).x + ssz / 16 * 5, point(0).y + ssz / 16 * 14, point(0).x + ssz / 16 * 6, point(0).y + ssz / 16 * 6);
		fl_line(point(0).x + ssz / 16 * 6, point(0).y + ssz / 16 * 6, point(0).x + ssz / 16 * 3, point(0).y + ssz / 16 * 6);
		fl_line(point(0).x + ssz / 16 * 3, point(0).y + ssz / 16 * 6, point(0).x + ssz / 16 * 3, point(0).y + ssz / 16 * 2);
		fl_line(point(0).x + ssz / 16 * 3, point(0).y + ssz / 16 * 2, point(0).x + ssz / 16 * 5, point(0).y + ssz / 16 * 2);
		fl_line(point(0).x + ssz / 16 * 5, point(0).y + ssz / 16 * 2, point(0).x + ssz / 16 * 5, point(0).y + ssz / 16 * 4);
		fl_line(point(0).x + ssz / 16 * 5, point(0).y + ssz / 16 * 4, point(0).x + ssz / 16 * 7, point(0).y + ssz / 16 * 4);
		fl_line(point(0).x + ssz / 16 * 7, point(0).y + ssz / 16 * 4, point(0).x + ssz / 16 * 7, point(0).y + ssz / 16 * 2);
		fl_line(point(0).x + ssz / 16 * 7, point(0).y + ssz / 16 * 2, point(0).x + ssz / 16 * 9, point(0).y + ssz / 16 * 2);
		fl_line(point(0).x + ssz / 16 * 9, point(0).y + ssz / 16 * 2, point(0).x + ssz / 16 * 9, point(0).y + ssz / 16 * 4);
		fl_line(point(0).x + ssz / 16 * 9, point(0).y + ssz / 16 * 4, point(0).x + ssz / 16 * 11, point(0).y + ssz / 16 * 4);
		fl_line(point(0).x + ssz / 16 * 11, point(0).y + ssz / 16 * 4, point(0).x + ssz / 16 * 11, point(0).y + ssz / 16 * 2);
		fl_line(point(0).x + ssz / 16 * 11, point(0).y + ssz / 16 * 2, point(0).x + ssz / 16 * 11, point(0).y + ssz / 16 * 2);
		fl_line(point(0).x + ssz / 16 * 11, point(0).y + ssz / 16 * 2, point(0).x + ssz / 16 * 13, point(0).y + ssz / 16 * 2);
		fl_line(point(0).x + ssz / 16 * 13, point(0).y + ssz / 16 * 2, point(0).x + ssz / 16 * 13, point(0).y + ssz / 16 * 6);
		fl_line(point(0).x + ssz / 16 * 13, point(0).y + ssz / 16 * 6, point(0).x + ssz / 16 * 10, point(0).y + ssz / 16 * 6);
		fl_line(point(0).x + ssz / 16 * 10, point(0).y + ssz / 16 * 6, point(0).x + ssz / 16 * 11, point(0).y + ssz / 16 * 14);
		fl_line(point(0).x + ssz / 16 * 11, point(0).y + ssz / 16 * 14, point(0).x + ssz / 16 * 13, point(0).y + ssz / 16 * 14);
		fl_line(point(0).x + ssz / 16 * 13, point(0).y + ssz / 16 * 14, point(0).x + ssz / 16 * 13, point(0).y + ssz - bd);
		fl_line(point(0).x + ssz / 16 * 13, point(0).y + ssz - bd, point(0).x + ssz / 16 * 3, point(0).y + ssz - bd);
	}
}


class Bishop :public chess_figure {		//Läufer
	int ssz;
	F_kind kind = F_kind::bishop;
public:
	Bishop(Point p, int x) :ssz(x) { add(p); }
	F_kind what_kind() { return kind; }
private:
	void draw_lines()const;
};
void Bishop::draw_lines()const {
	if (color().visibility()) {

		//Füllung
		fl_color(fill_color().as_int());
		fl_begin_complex_polygon();
		fl_vertex(point(0).x + ssz / 12 * 7, point(0).y + ssz / 48 * 18);
		fl_vertex(point(0).x + ssz / 12 * 5, point(0).y + ssz / 48 * 18);
		fl_vertex(point(0).x + ssz / 4, point(0).y + ssz - bd);
		fl_vertex(point(0).x + ssz / 4 * 3, point(0).y + ssz - bd);
		fl_end_complex_polygon();
		fl_pie(point(0).x + ssz / 50 * 20, point(0).y + ssz / 12, ssz / 6, ssz / 6, 0, 360);
		fl_pie(point(0).x + ssz / 4 - bd, point(0).y + ssz / 5, ssz / 2, ssz / 6, 0, 360);

		//Umrandung
		fl_color(color().as_int());
		fl_arc(point(0).x + ssz / 50 * 20, point(0).y + ssz / 12, ssz / 6, ssz / 6, 330, 217);
		fl_arc(point(0).x + ssz / 4 - bd, point(0).y + ssz / 5, ssz / 2, ssz / 6, 0, 360);
		fl_line(point(0).x + ssz / 12 * 5, point(0).y + ssz / 48 * 18, point(0).x + ssz / 4, point(0).y + ssz - bd);
		fl_line(point(0).x + ssz / 4, point(0).y + ssz - bd, point(0).x + ssz / 4 * 3, point(0).y + ssz - bd);					//untere Linie
		fl_line(point(0).x + ssz / 4 * 3, point(0).y + ssz - bd, point(0).x + ssz / 12 * 7, point(0).y + ssz / 48 * 18);
	}
}

class King :public chess_figure {
	int ssz;
	F_kind kind = F_kind::king;
public:
	King(Point p, int x) :ssz(x) { add(p); }

	F_kind what_kind() { return kind; }

private:
	void draw_lines()const;
};
void King::draw_lines()const {
	if (color().visibility()) {
		//Füllung
		fl_color(fill_color().as_int());
		fl_begin_complex_polygon();
		fl_vertex(point(0).x + ssz / 16 * 2, point(0).y + ssz - bd);
		fl_vertex(point(0).x + ssz / 16 * 2, point(0).y + ssz / 16 * 15);
		fl_vertex(point(0).x + ssz / 16 * 3, point(0).y + ssz / 16 * 14);
		fl_vertex(point(0).x + ssz / 16 * 7, point(0).y + ssz / 16 * 6);
		fl_vertex(point(0).x + ssz / 32 * 16, point(0).y + ssz / 16 * 3);
		fl_vertex(point(0).x + ssz / 32 * 16, point(0).y + ssz / 16 * 2);
		fl_vertex(point(0).x + ssz / 32 * 14, point(0).y + ssz / 16 * 2);
		fl_vertex(point(0).x + ssz / 32 * 14, point(0).y + ssz / 16 * 1);
		fl_vertex(point(0).x + ssz / 32 * 16, point(0).y + ssz / 16 * 1);
		fl_vertex(point(0).x + ssz / 32 * 16, point(0).y + bd);
		fl_vertex(point(0).x + ssz / 32 * 18, point(0).y + bd);
		fl_vertex(point(0).x + ssz / 32 * 18, point(0).y + ssz / 16 * 1);
		fl_vertex(point(0).x + ssz / 32 * 20, point(0).y + ssz / 16 * 1);
		fl_vertex(point(0).x + ssz / 32 * 20, point(0).y + ssz / 16 * 2);
		fl_vertex(point(0).x + ssz / 32 * 18, point(0).y + ssz / 16 * 2);
		fl_vertex(point(0).x + ssz / 32 * 18, point(0).y + ssz / 16 * 3);
		fl_vertex(point(0).x + ssz / 16 * 9, point(0).y + ssz / 16 * 6);
		fl_vertex(point(0).x + ssz / 16 * 13, point(0).y + ssz / 16 * 14);
		fl_vertex(point(0).x + ssz / 16 * 14, point(0).y + ssz / 16 * 15);
		fl_vertex(point(0).x + ssz / 16 * 14, point(0).y + ssz - bd);
		fl_end_complex_polygon();
		fl_pie(point(0).x + ssz / 16 * 4 - bd, point(0).y + ssz / 16 * 3, ssz / 2, ssz / 6 + bd * 2, 0, 360);

		//Umrandung
		fl_color(color().as_int());
		fl_line(point(0).x + ssz / 16 * 2, point(0).y + ssz - bd, point(0).x + ssz / 16 * 2, point(0).y + ssz / 16 * 15);
		fl_line(point(0).x + ssz / 16 * 2, point(0).y + ssz / 16 * 15, point(0).x + ssz / 16 * 3, point(0).y + ssz / 16 * 14);
		fl_line(point(0).x + ssz / 16 * 3, point(0).y + ssz / 16 * 14, point(0).x + ssz / 16 * 7, point(0).y + ssz / 16 * 6);
		//fl_line(point(0).x + ssz / 16 * 7, point(0).y + ssz / 16 * 6, (point(0).x + ssz / 32 * 16, point(0).y + ssz / 16 * 3);	//Ausgelassen, wegen der Elipse
		fl_line(point(0).x + ssz / 32 * 16, point(0).y + ssz / 16 * 3, point(0).x + ssz / 32 * 16, point(0).y + ssz / 16 * 2);
		fl_line(point(0).x + ssz / 32 * 16, point(0).y + ssz / 16 * 2, point(0).x + ssz / 32 * 14, point(0).y + ssz / 16 * 2);
		fl_line(point(0).x + ssz / 32 * 14, point(0).y + ssz / 16 * 2, point(0).x + ssz / 32 * 14, point(0).y + ssz / 16 * 1);
		fl_line(point(0).x + ssz / 32 * 14, point(0).y + ssz / 16 * 1, point(0).x + ssz / 32 * 16, point(0).y + ssz / 16 * 1);
		fl_line(point(0).x + ssz / 32 * 16, point(0).y + ssz / 16 * 1, point(0).x + ssz / 32 * 16, point(0).y + bd);
		fl_line(point(0).x + ssz / 32 * 16, point(0).y + bd, point(0).x + ssz / 32 * 18, point(0).y + bd);
		fl_line(point(0).x + ssz / 32 * 18, point(0).y + bd, point(0).x + ssz / 32 * 18, point(0).y + ssz / 16 * 1);
		fl_line(point(0).x + ssz / 32 * 18, point(0).y + ssz / 16 * 1, point(0).x + ssz / 32 * 20, point(0).y + ssz / 16 * 1);
		fl_line(point(0).x + ssz / 32 * 20, point(0).y + ssz / 16 * 1, point(0).x + ssz / 32 * 20, point(0).y + ssz / 16 * 2);
		fl_line(point(0).x + ssz / 32 * 20, point(0).y + ssz / 16 * 2, point(0).x + ssz / 32 * 18, point(0).y + ssz / 16 * 2);
		fl_line(point(0).x + ssz / 32 * 18, point(0).y + ssz / 16 * 2, point(0).x + ssz / 32 * 18, point(0).y + ssz / 16 * 3);
		//fl_line(point(0).x + ssz / 32 * 18, point(0).y + ssz / 16 * 3, point(0).x + ssz / 16 * 9, point(0).y + ssz / 16 * 6);		//Ausgelassen, wegen der Elipse
		fl_line(point(0).x + ssz / 16 * 9, point(0).y + ssz / 16 * 6, point(0).x + ssz / 16 * 13, point(0).y + ssz / 16 * 14);
		fl_line(point(0).x + ssz / 16 * 13, point(0).y + ssz / 16 * 14, point(0).x + ssz / 16 * 14, point(0).y + ssz / 16 * 15);
		fl_line(point(0).x + ssz / 16 * 14, point(0).y + ssz / 16 * 15, point(0).x + ssz / 16 * 14, point(0).y + ssz - bd);
		fl_line(point(0).x + ssz / 16 * 14, point(0).y + ssz - bd, point(0).x + ssz / 16 * 2, point(0).y + ssz - bd);
		fl_arc(point(0).x + ssz / 16 * 4 - bd, point(0).y + ssz / 16 * 3, ssz / 2, ssz / 6 + bd * 2, 0, 360);
	}
}

class Queen :public chess_figure {
	int ssz;
	F_kind kind = F_kind::queen;
public:
	Queen(Point p, int x);

	F_kind what_kind() { return kind; }

private:
	void draw_lines()const;
};




class Chess_window : public Window {



	bool button_pushed;

	Button quit_button;


	Vector_ref<Button>field_buttons;					//Für die Knöppe unter den schwarzen Feldern
	Vector_ref< Graph_lib::Rectangle>checkers;			//Für die Felder

public:
	Chess_window(Point xy, int w, int h, const string& title);
	void wait_for_button();

private:
	//bool tile_empty(Point p)const;
	//bool hostile_present(Point p) const;


	static void cb_quit(Address, Address addr) { reference_to<Chess_window>(addr).quit(); }		//Ist die Stroustrup Variante, macht auch nur 		return *static_cast<W*>(pw);


	void quit() { hide(); button_pushed = true; }

};
Chess_window::Chess_window(Point xy, int w, int h, const string& title) :Window(w, h, title),
button_pushed(false),
quit_button(Point(x_max() - 70, 60), 70, 20, "Quit", cb_quit) {


	attach(quit_button);

}
void Chess_window::wait_for_button() {
	while (!button_pushed) Fl::wait();
	button_pushed = false;
	Fl::redraw();
}


int main() try {

	Chess_window win{ {100,100},1000,800,"Schach" };

	Graph_lib::Rectangle r{ {200,200},sz,sz };				//Test Feld um Proportionen der Figuren in den späteren Spielfeldern abschaätzen zu können 
	//Pawn test({ 200,200 },sz);								// Bauer
	//Bishop test({ 200,200 }, sz);								//Läufer
	//Rook test({ 200,200 }, sz);								//Turm
	King test({ 200,200 }, sz);


	test.set_color(FL_RED);
	test.set_fill_color(FL_GREEN);

	win.attach(r);
	win.attach(test);

	win.wait_for_button();

}
catch (exception& e) {

	cerr << e.what();
	cin.get();
}