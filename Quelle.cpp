#include"Graph.h"
#include"Simple_window.h"

int sz = 100;			//Kommt sp�ter in die Fenster Klasse
int bd = sz/100;				//Abstand zum unteren Feld Rand

struct chess_figure :Shape {
	int ssz = 0;
	chess_figure(){}
};

class Pawn :public chess_figure {
	int ssz;
public:
	Pawn(Point p,int x);
	int scaling_size() { return ssz; }

private:
	void draw_lines()const;
};
Pawn::Pawn(Point p,int x):ssz(x) {
chess_figure:chess_figure;
	add(p);
}
void Pawn::draw_lines()const {
	if (color().visibility()) {

		//F�llung
		fl_color(fill_color().as_int());						
		fl_begin_complex_polygon();
		fl_vertex(point(0).x + ssz / 12 * 7, point(0).y + ssz / 12*7);
		fl_vertex(point(0).x + ssz / 12 * 5, point(0).y + ssz / 12*7);
		fl_vertex(point(0).x + ssz / 5, point(0).y + ssz-bd );
		fl_vertex(point(0).x + ssz / 5 * 4, point(0).y + ssz-bd );
		fl_end_complex_polygon();
		fl_pie(point(0).x + ssz / 3-bd, point(0).y + ssz / 4, ssz / 3 , ssz / 3 , 0, 360);

		//Umrandung
		fl_color(color().as_int());
		fl_arc(point(0).x + ssz / 3-bd, point(0).y + ssz / 4, ssz / 3, ssz / 3, 0, 360);
		fl_line(point(0).x + ssz / 12 * 5, point(0).y + ssz / 12 * 7, point(0).x + ssz / 5, point(0).y + ssz-bd );
		fl_line(point(0).x + ssz / 5, point(0).y + ssz-bd , point(0).x + ssz / 5 * 4, point(0).y + ssz -bd);
		fl_line(point(0).x + ssz / 5 * 4, point(0).y + ssz-bd , point(0).x + ssz / 12 * 7, point(0).y + ssz / 12 * 7);
	}
}



class Chess_window : public Window {

	

	bool button_pushed;

	Button quit_button;


	Vector_ref<Button>field_buttons;					//F�r die Kn�ppe unter den schwarzen Feldern
	Vector_ref< Graph_lib::Rectangle>checkers;			//F�r die Felder

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


int main() try{

	Chess_window win{ {100,100},1000,800,"Schach" };

	Graph_lib::Rectangle r{ {200,200},sz,sz };				//Test Feld um Proportionen der Figuren in den sp�teren Spielfeldern abscha�tzen zu k�nnen 
	Pawn test({ 200,200 },sz);								// Bauer

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