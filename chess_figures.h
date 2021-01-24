#pragma once
#include"Graph.h"
#include"Simple_window.h"

//Die Figurem sind nicht besonderes Hübsch, Malen war noch nie meine Stärke...


//Unterscheidung der Figuren
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


//Basis Klasse für den Vector_Ref
struct Chess_figure :Shape {
	int ssz = 0;
	F_kind kind;
	virtual F_kind what_kind() { return kind; }

};

//Bauer
class Pawn :public Chess_figure {
	int ssz;
	F_kind kind = F_kind::pawn;
public:
	Pawn(Point p, int x) :ssz(x) { add(p); }
	F_kind what_kind() { return kind; }
private:
	void draw_lines()const;
};
void Pawn::draw_lines()const {
	int bd = ssz / 100;
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
//Pferd
class Knight :public Chess_figure {		//Springer/Pferd
	int ssz;
	F_kind kind = F_kind::knight;
public:
	Knight(Point p, int x) :ssz(x) { add(p); }
	F_kind what_kind() { return kind; }
private:
	void draw_lines()const;
};
void Knight::draw_lines()const {
	int bd = ssz / 100;
	if (color().visibility()) {
		//Füllung
		fl_color(fill_color().as_int());
		fl_begin_complex_polygon();
		fl_vertex(point(0).x + ssz / 16 * 5, point(0).y + ssz - bd);
		fl_vertex(point(0).x + ssz / 16 * 5, point(0).y + ssz / 16 * 13);
		fl_vertex(point(0).x + ssz / 16 * 8, point(0).y + ssz / 16 * 7);
		fl_vertex(point(0).x + ssz / 32 * 9, point(0).y + ssz / 32 * 17);
		fl_vertex(point(0).x + ssz / 16 * 4, point(0).y + ssz / 16 * 4);
		fl_vertex(point(0).x + ssz / 16 * 10, point(0).y + ssz / 16 * 3);
		fl_vertex(point(0).x + ssz / 16 * 11, point(0).y + ssz / 16 * 1);
		fl_vertex(point(0).x + ssz / 16 * 12, point(0).y + ssz / 16 * 3);
		fl_vertex(point(0).x + ssz / 16 * 14, point(0).y + ssz / 16 * 5);
		fl_vertex(point(0).x + ssz / 16 * 14, point(0).y + ssz / 16 * 7);
		fl_vertex(point(0).x + ssz / 16 * 11, point(0).y + ssz / 16 * 13);
		fl_vertex(point(0).x + ssz / 16 * 11, point(0).y + ssz - bd);
		fl_end_complex_polygon();
		fl_pie(point(0).x + ssz / 16 * 2 - bd, point(0).y + ssz / 16 * 4, ssz / 16 * 4, ssz / 16 * 4, 0, 360);
		//Umrandung
		fl_color(color().as_int());
		fl_line(point(0).x + ssz / 16 * 5, point(0).y + ssz - bd, point(0).x + ssz / 16 * 5, point(0).y + ssz / 16 * 13);
		fl_line(point(0).x + ssz / 16 * 5, point(0).y + ssz / 16 * 13, point(0).x + ssz / 16 * 8, point(0).y + ssz / 16 * 7);
		fl_line(point(0).x + ssz / 16 * 8, point(0).y + ssz / 16 * 7, point(0).x + ssz / 32 * 9, point(0).y + ssz / 32 * 17);
		//fl_line(point(0).x + ssz / 16 * 4, point(0).y + ssz / 16 * 8, point(0).x + ssz / 16 * 4, point(0).y + ssz / 16 * 4);
		fl_line(point(0).x + ssz / 16 * 4, point(0).y + ssz / 16 * 4, point(0).x + ssz / 16 * 10, point(0).y + ssz / 16 * 3);
		fl_line(point(0).x + ssz / 16 * 10, point(0).y + ssz / 16 * 3, point(0).x + ssz / 16 * 11, point(0).y + ssz / 16 * 1);
		fl_line(point(0).x + ssz / 16 * 11, point(0).y + ssz / 16 * 1, point(0).x + ssz / 16 * 12, point(0).y + ssz / 16 * 3);
		fl_line(point(0).x + ssz / 16 * 12, point(0).y + ssz / 16 * 3, point(0).x + ssz / 16 * 14, point(0).y + ssz / 16 * 5);
		fl_line(point(0).x + ssz / 16 * 14, point(0).y + ssz / 16 * 5, point(0).x + ssz / 16 * 14, point(0).y + ssz / 16 * 7);
		fl_line(point(0).x + ssz / 16 * 14, point(0).y + ssz / 16 * 7, point(0).x + ssz / 16 * 11, point(0).y + ssz / 16 * 13);
		fl_line(point(0).x + ssz / 16 * 11, point(0).y + ssz / 16 * 13, point(0).x + ssz / 16 * 11, point(0).y + ssz - bd);
		fl_line(point(0).x + ssz / 16 * 11, point(0).y + ssz - bd, point(0).x + ssz / 16 * 5, point(0).y + ssz - bd);
		fl_arc(point(0).x + ssz / 16 * 2 - bd, point(0).y + ssz / 16 * 4, ssz / 16 * 4, ssz / 16 * 4, 85, 280);
	}
}
//Turm
class Rook :public Chess_figure {		//Turm
	int ssz;
	F_kind kind = F_kind::rook;
public:
	Rook(Point p, int x) :ssz(x) { add(p); }
	F_kind what_kind() { return kind; }
private:
	void draw_lines()const;
};
void Rook::draw_lines()const {
	int bd = ssz / 100;
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
//Läufer
class Bishop :public Chess_figure {		//Läufer
	int ssz;
	F_kind kind = F_kind::bishop;
public:
	Bishop(Point p, int x) :ssz(x) { add(p); }
	F_kind what_kind() { return kind; }
private:
	void draw_lines()const;
};
void Bishop::draw_lines()const {
	int bd = ssz / 100;
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

class King :public Chess_figure {
	int ssz;
	F_kind kind = F_kind::king;
public:
	King(Point p, int x) :ssz(x) { add(p); }
	F_kind what_kind() { return kind; }
private:
	void draw_lines()const;
};
void King::draw_lines()const {
	int bd = ssz / 100;
	if (color().visibility()) {
		//Füllung
		fl_color(fill_color().as_int());
		fl_begin_complex_polygon();
		fl_vertex(point(0).x + ssz / 16 * 2, point(0).y + ssz - bd);
		fl_vertex(point(0).x + ssz / 16 * 2, point(0).y + ssz / 16 * 15);
		fl_vertex(point(0).x + ssz / 16 * 3, point(0).y + ssz / 16 * 14);
		fl_vertex(point(0).x + ssz / 16 * 7, point(0).y + ssz / 16 * 6);

		/*
		//Wenn sz= 50 sind diese Einstellungen besser, glaub ich...

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
		*/
		fl_vertex(point(0).x + ssz / 32 * 15, point(0).y + ssz / 16 * 3);
		fl_vertex(point(0).x + ssz / 32 * 15, point(0).y + ssz / 16 * 2);
		fl_vertex(point(0).x + ssz / 32 * 13, point(0).y + ssz / 16 * 2);
		fl_vertex(point(0).x + ssz / 32 * 13, point(0).y + ssz / 16 * 1);
		fl_vertex(point(0).x + ssz / 32 * 15, point(0).y + ssz / 16 * 1);
		fl_vertex(point(0).x + ssz / 32 * 15, point(0).y + bd);
		fl_vertex(point(0).x + ssz / 32 * 17, point(0).y + bd);
		fl_vertex(point(0).x + ssz / 32 * 17, point(0).y + ssz / 16 * 1);
		fl_vertex(point(0).x + ssz / 32 * 19, point(0).y + ssz / 16 * 1);
		fl_vertex(point(0).x + ssz / 32 * 19, point(0).y + ssz / 16 * 2);
		fl_vertex(point(0).x + ssz / 32 * 17, point(0).y + ssz / 16 * 2);
		fl_vertex(point(0).x + ssz / 32 * 17, point(0).y + ssz / 16 * 3);

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
		fl_line(point(0).x + ssz / 32 * 15, point(0).y + ssz / 16 * 3, point(0).x + ssz / 32 * 15, point(0).y + ssz / 16 * 2);
		fl_line(point(0).x + ssz / 32 * 15, point(0).y + ssz / 16 * 2, point(0).x + ssz / 32 * 13, point(0).y + ssz / 16 * 2);
		fl_line(point(0).x + ssz / 32 * 13, point(0).y + ssz / 16 * 2, point(0).x + ssz / 32 * 13, point(0).y + ssz / 16 * 1);
		fl_line(point(0).x + ssz / 32 * 13, point(0).y + ssz / 16 * 1, point(0).x + ssz / 32 * 15, point(0).y + ssz / 16 * 1);
		fl_line(point(0).x + ssz / 32 * 15, point(0).y + ssz / 16 * 1, point(0).x + ssz / 32 * 15, point(0).y + bd);
		fl_line(point(0).x + ssz / 32 * 15, point(0).y + bd, point(0).x + ssz / 32 * 17, point(0).y + bd);
		fl_line(point(0).x + ssz / 32 * 17, point(0).y + bd, point(0).x + ssz / 32 * 17, point(0).y + ssz / 16 * 1);
		fl_line(point(0).x + ssz / 32 * 17, point(0).y + ssz / 16 * 1, point(0).x + ssz / 32 * 19, point(0).y + ssz / 16 * 1);
		fl_line(point(0).x + ssz / 32 * 19, point(0).y + ssz / 16 * 1, point(0).x + ssz / 32 * 19, point(0).y + ssz / 16 * 2);
		fl_line(point(0).x + ssz / 32 * 19, point(0).y + ssz / 16 * 2, point(0).x + ssz / 32 * 17, point(0).y + ssz / 16 * 2);
		fl_line(point(0).x + ssz / 32 * 17, point(0).y + ssz / 16 * 2, point(0).x + ssz / 32 * 17, point(0).y + ssz / 16 * 3);
		/*
		//Wenn sz= 50 sind diese Einstellungen besser, glaub ich...
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
		*/
		//fl_line(point(0).x + ssz / 32 * 18, point(0).y + ssz / 16 * 3, point(0).x + ssz / 16 * 9, point(0).y + ssz / 16 * 6);		//Ausgelassen, wegen der Elipse
		fl_line(point(0).x + ssz / 16 * 9, point(0).y + ssz / 16 * 6, point(0).x + ssz / 16 * 13, point(0).y + ssz / 16 * 14);
		fl_line(point(0).x + ssz / 16 * 13, point(0).y + ssz / 16 * 14, point(0).x + ssz / 16 * 14, point(0).y + ssz / 16 * 15);
		fl_line(point(0).x + ssz / 16 * 14, point(0).y + ssz / 16 * 15, point(0).x + ssz / 16 * 14, point(0).y + ssz - bd);
		fl_line(point(0).x + ssz / 16 * 14, point(0).y + ssz - bd, point(0).x + ssz / 16 * 2, point(0).y + ssz - bd);
		fl_arc(point(0).x + ssz / 16 * 4 - bd, point(0).y + ssz / 16 * 3, ssz / 2, ssz / 6 + bd * 2, 0, 360);
	}
}

class Queen :public Chess_figure {
	int ssz;
	F_kind kind = F_kind::queen;
public:
	Queen(Point p, int x) :ssz(x) { add(p); }
	F_kind what_kind() { return kind; }
private:
	void draw_lines()const;
};
void Queen::draw_lines()const {
	int bd = ssz / 100;
	if (color().visibility()) {
		//Füllung
		fl_color(fill_color().as_int());
		fl_begin_complex_polygon();
		fl_vertex(point(0).x + ssz / 16 * 3, point(0).y + ssz - bd);
		fl_vertex(point(0).x + ssz / 16 * 6, point(0).y + ssz / 16 * 7);
		fl_vertex(point(0).x + ssz / 16 * 4, point(0).y + ssz / 16 * 1);
		fl_vertex(point(0).x + ssz / 16 * 6, point(0).y + ssz / 16 * 4);
		fl_vertex(point(0).x + ssz / 16 * 8, point(0).y + bd);
		fl_vertex(point(0).x + ssz / 16 * 10, point(0).y + ssz / 16 * 4);
		fl_vertex(point(0).x + ssz / 16 * 12, point(0).y + ssz / 16 * 1);
		fl_vertex(point(0).x + ssz / 16 * 10, point(0).y + ssz / 16 * 7);
		fl_vertex(point(0).x + ssz / 16 * 13, point(0).y + ssz - bd);
		fl_end_complex_polygon();
		//Umrandung
		fl_color(color().as_int());
		fl_line(point(0).x + ssz / 16 * 3, point(0).y + ssz - bd, point(0).x + ssz / 16 * 6, point(0).y + ssz / 16 * 7);
		fl_line(point(0).x + ssz / 16 * 6, point(0).y + ssz / 16 * 7, point(0).x + ssz / 16 * 4, point(0).y + ssz / 16 * 1);
		fl_line(point(0).x + ssz / 16 * 4, point(0).y + ssz / 16 * 1, point(0).x + ssz / 16 * 6, point(0).y + ssz / 16 * 4);
		fl_line(point(0).x + ssz / 16 * 6, point(0).y + ssz / 16 * 4, point(0).x + ssz / 16 * 8, point(0).y + bd);
		fl_line(point(0).x + ssz / 16 * 8, point(0).y + bd, point(0).x + ssz / 16 * 10, point(0).y + ssz / 16 * 4);
		fl_line(point(0).x + ssz / 16 * 10, point(0).y + ssz / 16 * 4, point(0).x + ssz / 16 * 12, point(0).y + ssz / 16 * 1);
		fl_line(point(0).x + ssz / 16 * 12, point(0).y + ssz / 16 * 1, point(0).x + ssz / 16 * 10, point(0).y + ssz / 16 * 7);
		fl_line(point(0).x + ssz / 16 * 10, point(0).y + ssz / 16 * 7, point(0).x + ssz / 16 * 13, point(0).y + ssz - bd);
		fl_line(point(0).x + ssz / 16 * 13, point(0).y + ssz - bd, point(0).x + ssz / 16 * 3, point(0).y + ssz - bd);
	}
}