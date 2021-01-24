#include"chess_figures.h"

int sz = 100;									//Kommt vielleicht später in die Fenster Klasse, wäre evt besser ein double um Schwulitäten zu vermeiden wenn sz nicht 100 ist
int bd = 1;								//Abstand zum unteren Feld Rand

int ls = 100;									//Abstand zum linken Fensterrand
int us = 0;										//Abstand zum oberen Fensterrand


class Chess_window : public Window {

	bool button_pushed;

	Button quit_button;

	Vector_ref<Button>field_buttons;					//Für die Knöppe unter den schwarzen Feldern
	Vector_ref< Graph_lib::Rectangle>tiles;				//Für die Felder
	Vector_ref<chess_figure>figures;

public:
	Chess_window(Point xy, int w, int h, const string& title);
	void wait_for_button();

private:
	//bool tile_empty(Point p)const;
	//bool hostile_present(Point p) const;


	static void cb_quit(Address, Address addr) { reference_to<Chess_window>(addr).quit(); }		//Ist die Stroustrup Variante, macht auch nur 		return *static_cast<W*>(pw);
	static void cb_tile_pressed(Address, Address addr) { reference_to<Chess_window>(addr).tile_pressed(); }

	void quit() { hide(); button_pushed = true; }
	void tile_pressed() {}
};
Chess_window::Chess_window(Point xy, int w, int h, const string& title) :Window(w, h, title),
button_pushed(false),
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

			if (y == 0 || y == 1)figures[figures.size() - 1].set_color(FL_WHITE);
			if (y == 6 || y == 7)figures[figures.size() - 1].set_fill_color(FL_WHITE);
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


int main() try {
	Chess_window win{ {0,0},1000,800,"Schach" };

	win.wait_for_button();
}
catch (exception& e) {

	cerr << e.what();
	cin.get();
}