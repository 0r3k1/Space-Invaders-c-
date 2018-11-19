#include "objeto.h"
using namespace std;
using namespace miniwin;

objeto::objeto() {
	x = 0;
	y = 0;
	alto = 0;
	ancho = 0;
	pix.clear();
}

objeto::objeto(int _x, int _y, int _alto, int _ancho, char* map){
	x = _x;
	y = _y;
	alto = _alto;
	ancho = _ancho;
	
	for(int i=0; i<alto; i++){
		int punto = 0;
		for(int j=0; j<ancho; j++){
			if(map[i*ancho+j] == '*') punto = 1;
			else punto = 0;
			pix.push_back(pixel(x+(tam*j), y+(tam*i), punto));
		}
	}
}

objeto::objeto(int _x, int _y, int _alto, int _ancho){
	x = _x;
	y = _y;
	alto = _alto;
	ancho = _ancho;
}

objeto::objeto(const objeto& arg){
	x = arg.x;
	y = arg.y;
	alto = arg.alto;
	ancho = arg.ancho;
	pix = arg.pix;
}

objeto::~objeto(){
	pix.clear();
}


void objeto::pinta(){
	for(int i=0; i<alto; i++){
		for(int j=0; j<ancho; j++){
			if(pix[i*ancho+j].punto == 1){
				int _x = x+(tam*j);
				int _y = y+(tam*i);
				rectangulo_lleno(_x, _y, _x+tam, _y+tam);
			}
		}
	}
}

void objeto::mueve(int dx, int dy){
	x += dx;
	y += dy;
	for(int i=0; i<alto; i++){
		for(int j=0; j<ancho; j++){
			int cor = i*ancho+j;
			pix[cor].x +=dx;
			pix[cor].y +=dy;
		}
	}
}

cuadrado objeto::getCuadrado(){
	if(alto > 8){
		return cuadrado(x, y, x+(ancho*tam), y+((alto/2)*tam));
	}else{
		return cuadrado(x, y, x+(ancho*tam), y+(alto*tam));
	}
}

bool objeto::dentro(cuadrado c){
	if((x >= c.x && x <= c.w) && (y >= c.y && y <= c.h)) return true;
	if((x+(ancho*tam) >= c.x && x+(ancho*tam) <= c.w) && (y >= c.y && y <= c.h)) return true;
	if((x+(ancho*tam) >= c.x && x+(ancho*tam) <= c.w) && (y+(alto*tam) >= c.y && y+(alto*tam) <= c.h)) return true;
	if((x >= c.x && x <= c.w) && (y+(alto*tam) >= c.y && y+(alto*tam) <= c.h)) return true;
	return false;
}

bool objeto::colObj(objeto obj){
	return dentro(obj.getCuadrado());
}

///////////////////clase dispara////////////////////////////
disparo::disparo(int _x, int _y, int _alto, int _ancho, int _vel, char* map)
	:objeto(_x, _y, _alto, _ancho, map){
	vel = _vel;
}

disparo::~disparo(){
	pix.clear();
}

void disparo::mueve(){
	y += vel;
	for(int i=0; i<alto; i++){
		for(int j=0; j<ancho; j++){
			pix[i*ancho+j].y += vel;
		}
	}
}

bool disparo::colPlayer(player p){
	return p.dentro(getCuadrado());
}

////////////////////////clase player////////////////////////////////////////////
player::player(int _x, int _y, int _alto, int _ancho, char* map)
	:objeto(_x, _y, _alto, _ancho, map){
	disp.clear();
};

void player::dispara(int _alto, int _ancho, int _vel, char* map){
	disp.push_back(disparo(x+(ancho/2)+5, y-alto, _alto, _ancho, _vel, map));
}

void player::moverDisparo(){
	for(int i=0; i<(int)disp.size(); i++){
		disp[i].mueve();
		if(!disp[i].dentro(cuadrado(0, 40, vancho(), valto()))){
			disp.erase(disp.begin()+i);
		}
	}
}

void player::pintaDisparo(){
	for(auto itd = disp.begin(); itd != disp.end(); itd++){
		(*itd).pinta();
	}
}

bool player::colDisp(objeto obj){
	for(auto itd = disp.begin(); itd != disp.end(); itd++){
		if((*itd).colObj(obj)){
			itd = disp.erase(itd);
			return true;
		}
	}
	
	return false;
}

bool player::colDisp(escudo& e){
	for(auto itd = disp.begin(); itd != disp.end(); itd++){
		if(e.colObj((*itd))){
			itd = disp.erase(itd);
			return true;
		}
	}
	
	return false;
}

bool player::dentro(cuadrado c){
	cuadrado c1, c2;
	
	c1.x = x +(1*tam);
	c1.y = y +(3*tam);
	c1.w = x +(ancho*tam)-(1*tam);
	c1.h = alto*tam;
	
	c2.x = x +(5*tam);
	c2.y = c1.y;
	c2.w = x +(ancho*tam)-(5*tam);
	c2.h = c1.h;
	if(colCuadrada(c1, c) || colCuadrada(c2, c)) return true;
	return false;
}

bool player::colCuadrada(cuadrado c1, cuadrado c2){
	if((c1.x >= c2.x && c1.x <= c2.w) && (c1.y >= c2.y && c1.y <= c2.h)) return true;
	if((c1.w >= c2.x && c1.w <= c2.w) && (c1.y >= c2.y && c1.y <= c2.h)) return true;
	if((c1.w >= c2.x && c1.w <= c2.w) && (c1.h >= c2.y && c1.h <= c2.h)) return true;
	if((c1.x >= c2.x && c1.x <= c2.w) && (c1.h >= c2.y && c1.h <= c2.h)) return true;
	return false;
}

void player::limites(){
	if(x <= 10) x = 11;
	if(x+(ancho*tam) >= vancho()-10) x = (vancho()-9)-ancho*tam;
}


////////////////////////clase marciano//////////////////////////////////////////
marciano::marciano(int _x, int _y, int _alto, int _ancho, int _puntos, char* map)
	: objeto(_x, _y, _alto, _ancho){
	animacion = 0;
	flag = false;
	puntos = _puntos;
	
	for(int i=0; i<alto; i++){
		int punto = 0;
		animacion = 0;
		for(int j=0; j<ancho; j++){
			if(map[i*ancho+j] == '*') punto = 1;
			else punto = 0;
			if(i>=8) animacion = alto/2;
			pix.push_back(pixel(x+(tam*j), y+(tam*(i-animacion)), punto));
		}
	}
	
	animacion = 0;
}

marciano::~marciano() {
	pix.clear();
}

void marciano::pinta(){
	for(int i=0+animacion; i<(alto/2)+animacion; i++){
		for(int j=0; j<ancho; j++){
			int cor = i*ancho+j;
			if(pix[cor].punto == 1){
				rectangulo_lleno(pix[cor].x, pix[cor].y, pix[cor].x+tam, pix[cor].y+tam);
			}
		}
	}
}

void marciano::anima(){
	if(flag) animacion = 0;
	else animacion = alto/2;
	flag = !flag;
}

pixel marciano::getpixel()const{
	return pix[0];
}

///////////////////////////////clase escudo/////////////////////////////////////
escudo::escudo(int _x, int _y, int lado){
	
	for(int i=0; i<2; i++){
		for(int j=0; j<3; j++){
			int x = _x+(lado*j);
			int y = _y+(lado*i);
			if(!(j == 1 && i == 1)){
				cuadrado c(x, y, x+lado, y+lado);
				parte.push_back(c);
			}
		}
	}
}

escudo::~escudo(){
	parte.clear();
}

void escudo::pinta(){
	for(int i=0; i<(int)parte.size(); i++){
		rectangulo_lleno(parte[i].x, parte[i].y, parte[i].w, parte[i].h);
	}
}

bool escudo::colObj(objeto obj){
	for(int i=0; i<(int)parte.size(); i++){
		if(obj.dentro(parte[i])){
			parte.erase(parte.begin()+i);
			return true;
		}
	}
	
	return false;
}

///////////////////////////////clase nave///////////////////////////////////////

nave::nave(int _x, int _y, int _alto, int _ancho, char* map): objeto(_x, _y, _alto, _ancho, map){
	puntaje = 20;
	resta = 0;
	flag = false;
}

void nave::activa(int azar){
	flag = true;
	if(azar == 0){
		inicio = 0-(ancho*tam);;
		fin = vancho();
		dx = 3;
	}else{
		inicio = vancho();
		fin = 0-(ancho*tam);
		dx = -3;
	}
}

void nave::fuera(){
	if(flag && inicio == fin){
		flag = false;
	}
}

void nave::mueve(){
	for(int i=0; i<alto; i++){
		for(int j=0; j<ancho; j++){
			pix[i*ancho+j].x += dx;
		}
	}
}
