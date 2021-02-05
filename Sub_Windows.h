#pragma once
#include"Graph.h"
#include"GUI.h"
#include <FL/Fl_Multiline_Output.H>
#include"chess_figures.h"

using namespace Graph_lib;
extern string text;
extern F_kind promotion_choice;

struct Multi_Out_box : Widget {				//Multiline Out_box

	Multi_Out_box(Point xy, int w, int h, const string& s) :Widget(xy, w, h, s, 0) {}

	void put(const string&);

	void attach(Graph_lib::Window& win);
};
void Multi_Out_box::put(const string& s) {
	reference_to<Fl_Multiline_Output>(pw).value(s.c_str());
}

void Multi_Out_box::attach(Graph_lib::Window& win) {
	pw = new Fl_Multiline_Output(loc.x, loc.y, width, height, label.c_str());
	own = &win;
}

struct Rules_window :public Window {

	bool button_pushed;
	Button ok_button;
	Multi_Out_box txt;

public:
	Rules_window(Point xy, int w, int h, const string& title) :Window(w, h, title),
		button_pushed(false), ok_button(Point(x_max() - 70, 30), 70, 20, "OK", cb_OK),
		txt({ 0,0 }, 1200, 800, "") {
		attach(txt);
		attach(ok_button);
		txt.put(text);
	}

	void wait_for_button() {
		while (!button_pushed) Fl::wait();
		button_pushed = false;
		Fl::redraw();
	}

private:
	static void cb_OK(Address, Address addr) { reference_to<Rules_window>(addr).quit(); }
	void quit() { hide(); button_pushed = true; }
};

struct Promotion_window :public Window {
	bool button_pushed = false;
	Button q_button;
	Button b_button;
	Button k_button;
	Button r_button;

	Queen qu;
	Bishop bi;
	Knight kn;
	Rook ro;

	Text txt;

public:
	Promotion_window(Point xy, int w, int h, const string& title) :Window(xy,w, h, title),
		 q_button(Point(50, 100), 100, 100, "", cb_queen),
		b_button(Point(200, 100), 100, 100, "", cb_bishop) ,
		k_button(Point(350, 100), 100, 100, "", cb_knight) ,
		r_button(Point(500, 100), 100, 100, "", cb_rook),
		qu({ 50,99 }, 100), bi({ 200,99 }, 100), kn({ 350,99 }, 100), ro({ 500,99 },100),
		txt({ 50,45 },"Select a Figure to promote your pawn.") {
		clear_border();
		attach(q_button);
		attach(b_button);
		attach(k_button);
		attach(r_button);

		attach(qu);
		attach(bi);
		attach(kn);
		attach(ro);

		attach(txt);

		qu.set_fill_color(Color::green);
		bi.set_fill_color(Color::green);
		kn.set_fill_color(Color::green);
		ro.set_fill_color(Color::green);
		
	}
	void wait_for_button() {
		while (!button_pushed) Fl::wait();
		button_pushed = false;
		Fl::redraw();
	}
private:
	static void cb_queen(Address, Address addr) { reference_to<Promotion_window>(addr).queen(); }
	static void cb_bishop(Address, Address addr) { reference_to<Promotion_window>(addr).bishop(); }
	static void cb_knight(Address, Address addr) { reference_to<Promotion_window>(addr).knight(); }
	static void cb_rook(Address, Address addr) { reference_to<Promotion_window>(addr).rook(); }

	void queen() { promotion_choice = F_kind::queen; hide(); }
	void bishop() { promotion_choice = F_kind::bishop; hide(); }
	void knight() { promotion_choice = F_kind::knight; hide(); }
	void rook() { promotion_choice = F_kind::rook; hide(); }
};