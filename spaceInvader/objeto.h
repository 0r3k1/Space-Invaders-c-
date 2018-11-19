#ifndef OBJETO_H
#define OBJETO_H
#include <vector>
#include <miniwin.h>

const int tam = 2;

struct pixel{
	int x;
	int y;
	int punto;
	
	pixel(int _x = 0, int _y = 0, int _p = 1): x(_x), y(_y), punto(_p){};
};

struct cuadrado{
	int x, y;
	int w, h;
	
	cuadrado(int _x = 0, int _y = 0, int _w = 0, int _h = 0): x(_x), y(_y), w(_w), h(_h){};
};

class objeto;
class disparo;
class player;
class marciano;
class escudo;
class nave;

///////////////////////////////clase objeto/////////////////////////////////////
class objeto {
protected:
	std::vector<pixel> pix;
	int x, y;
	int alto, ancho;
public:
	objeto();
	objeto(int _x, int _y, int _alto, int _ancho, char* map);
	objeto(int _x, int _y, int _alto, int _ancho);
	objeto(const objeto& arg);
	~objeto();
	void pinta();
	void mueve(int dx, int dy);
	cuadrado getCuadrado();
	bool dentro(cuadrado c);
	bool colObj(objeto obj);
};

//////////////////////////////clase disparo/////////////////////////////////////
class disparo: public objeto{
	int vel;
public:
	disparo(int _x, int _y, int _alto, int _ancho, int _vel, char* map);
	~disparo();
	void mueve();
	bool colPlayer(player p);
};

///////////////////////////////clase player/////////////////////////////////////
class player : public objeto{
	std::vector<disparo> disp;
	bool colCuadrada(cuadrado c1, cuadrado c2);
public:
	player(int _x, int _y, int _alto, int _ancho, char* map);
	void dispara(int _alto, int _ancho, int _vel, char* map);
	void moverDisparo();
	void pintaDisparo();
	bool colDisp(objeto obj);
	bool colDisp(escudo& e);
	int getCanDisp(){return disp.size();};
	void setPos(int _x, int _y){x = _x; y = _y;}
	bool dentro(cuadrado c);
	void clearDisp(){disp.clear();}
	void limites();
};

////////////////////////////////clase marciano//////////////////////////////////
class marciano: public objeto{
	int animacion;
	bool flag;
	int puntos;
public:
	marciano(int _x, int _y, int _alto, int _ancho, int _puntos, char* map);
	~marciano();
	void pinta();
	void anima();
	void dispara(int _ancho, int _alto, int vel, char* dip);
	void pintaDisparo();
	void moverDisparo();
	pixel getpixel()const;
	int getAncho()const{ return ancho*tam;}
	int getMitadW()const{return x+(alto/2);}
	int getMitadH()const{return y+(ancho/2);}
	int getPuntos()const{return puntos;}
};

class escudo{
	std::vector<cuadrado> parte;
public:
	escudo(int _x, int _y, int lado);
	~escudo();
	void pinta();
	bool colObj(objeto obj);
};

class nave: public objeto{
	int puntaje;
	int resta;
	int inicio;
	int fin;
	int dx;
	bool flag;
public:
	nave(int _x, int _y, int _alto, int _ancho, char* _map);
	void setFag(bool _flag){flag = _flag;}
	bool getFlag()const{return flag;}
	void activa(int azar);
	void fuera();
	void mueve();
};

#endif

