#include <miniwin.h>
#include <vector>
using namespace miniwin;
using namespace std;
#include "mapaDeBit.h"
#include "objeto.h"
#include <time.h>

const int nMarcianos = 12;
const int retado = 150;

bool frame(int initfp);
void pintaVidas(int vidas, int x, int y, int alto, int ancho, char* map);
bool gameOver(int vidas);
void creaMarcianos(vector<marciano>& marci);

int main() {
	srand(time(NULL));
	int initx = 180, inity = 280;
	player p(initx, inity, 7, 14, *nav);
	vector<marciano> marci;
	vector<disparo> dispM;
	vector<escudo> e;
/*	nave n(0, 35, 7, 17, *navalien);*/
	int frameMar, frameDisp;
	float frameMarDisp;
	int dx, dy;
	int azar, vidas, puntos, level, esp, vel, apareceNav;
	char puntuacion[30], nivel[30];
	
	vredimensiona(400, 300);
	
	creaMarcianos(marci);
	
	for(int i=0; i<6; i++){
		int x = 25+((18*3)*i);
		e.push_back(escudo(x, 250, 13));
	}
	
	
	dx = 3;
	dy = 0;
	frameMar = 0;
	frameDisp = 0;
	frameMarDisp = 0;
	vidas = 3;
	puntos = 0;
	level = 1;
	esp = 20;
	vel = 1;
	
	sprintf(nivel, "Nivel: %d", level);
	
	while(!gameOver(vidas)){
//////////////////borrado del fuber/////////////////
		borra();
/////////////////comprobaciones/////////////////////
		for(auto itm = marci.begin(); itm != marci.end(); itm++){
			if(!(*itm).dentro(cuadrado(35, 0, vancho()-35, valto())) && frame(frameMar)){
				dx = -dx;
				dy = 3;
				break;
			}else dy = 0;
		}
		
		switch(tecla()){
		case DERECHA: p.mueve(3, 0); break;
		case IZQUIERDA: p.mueve(-3, 0); break;
		case ESPACIO:
			if(!(p.getCanDisp() >=3) && frame(frameDisp)){
				p.dispara(3, 2, -3, *dispPlay);
				frameDisp = 0;
			}
		}
///////////////logica///////////////////////////////
		azar = rand()%marci.size();
		/*if(!n.getFlag()) apareceNav = rand()%retado;*/
		sprintf(puntuacion, "Puntos: %d", puntos);
		
		p.limites();
		
		for(int i=0; i<(int)marci.size(); i++){
			if(p.colDisp(marci[i])){
				objeto explocion(marci[i].getpixel().x, marci[i].getpixel().y, 8, 14, *explo);
				marci.erase(marci.begin()+i);
				puntos += marci[i].getPuntos();
				explocion.pinta();
			}
		}
		
		if(frame(frameMar)){
			for(auto itm = marci.begin(); itm != marci.end(); itm++){
				(*itm).mueve(dx, dy);
				(*itm).anima();
			}
			frameMar = 0;
		}
		
		if(frame(frameMarDisp)){
			for(int i=azar; i<(int)marci.size(); i++){
				if(i == (int)marci.size()-1){
					dispM.push_back(disparo(marci[i].getMitadW(), marci[i].getMitadH(), 5, 4, 5, *dispMar));
				}else if(marci[i].getpixel().y > marci[i+1].getpixel().y){
					dispM.push_back(disparo(marci[i].getMitadW(), marci[i].getMitadH(), 5, 4, 5, *dispMar));
					break;
				}else if(marci[i].getpixel().y == marci[i+1].getpixel().y){
					dispM.push_back(disparo(marci[i].getMitadW(), marci[i].getMitadH(), 5, 4, 5, *dispMar));
					break;
				}
			}
			frameMarDisp = 0;
		}
		
		p.moverDisparo();
		for(int i=0; i<(int)dispM.size(); i++){
			dispM[i].mueve();
		}
		
		for(int i = 0; i<(int)dispM.size(); i++){
			if(!dispM[i].dentro(cuadrado(0, 0, vancho(), valto()))){
				dispM.erase(dispM.begin()+i);
			}
			
			if(dispM[i].colPlayer(p)){
				dispM.erase(dispM.begin()+i);
				vidas--;
				p.setPos(initx, inity);
				espera (100);
			}
			
			for(int j=0; j<(int)e.size(); j++){
				if(e[j].colObj(dispM[i])){
					dispM.erase(dispM.begin()+i);
				}
			}
		}
		
		if(marci.empty()){
			marci.clear();
			dispM.clear();
			p.clearDisp();
			
			creaMarcianos(marci);
			p.setPos(initx, inity);
			dx = 3;
			vel += 1;
			
			sprintf(nivel, "Nivel: %d", ++level);
			vidas++;
			if(esp >= 5) esp -= 2;
		}
		
		for(int i=0; i<(int)e.size(); i++){
			p.colDisp(e[i]);
		}
		
		if(frame(apareceNav)){
			/*n.activa(rand()%2);*/
			apareceNav = 0;
		}
		
		/*n.mueve();*/
		
////////////////pintado/////////////////////////////
		linea(10, 30, vancho()-10, 30);
		linea(10, 30, 10, valto());
		linea(vancho()-10, 30, vancho()-10, valto());
		texto(250, 10, "Vidas:");
		pintaVidas(vidas, 300, 10, 7, 14, *nav);
		texto(5, 10, puntuacion);
		texto(150, 10, nivel);
		
		color(VERDE);
		p.pinta();
		p.pintaDisparo();
		for(int i=0; i<(int)e.size(); i++){
			e[i].pinta();
		}
		color(ROJO);
		for(auto itd = dispM.begin(); itd != dispM.end(); itd++){
			(*itd).pinta();
		}
		/*n.pinta();*/
		
		color(BLANCO);
		for(auto itm = marci.begin(); itm != marci.end(); itm++){
			(*itm).pinta();
		}
		
///////////////frames//////////////////////////////
		refresca();
		espera(esp);
		frameMar += vel;
		frameDisp += 2;
		frameMarDisp+= .2;
		apareceNav++;
	}
	
	borra();
	texto(160, 145, "Game Over");
	refresca();
	
	return 0;
}

bool frame(int initfp){
	return initfp*10 >= retado;
}

void pintaVidas(int vidas, int x, int y, int alto, int ancho, char* map){
	if(vidas > 3){
		char text[20];
		sprintf(text, " %dx", vidas);
		texto(x-13, y, string(text));
		x += 15;
		for(int i = 0; i<1; i++){
			for(int a = 0; a<alto; a++){
				for(int b = 0; b<ancho; b++){
					int _x = x+(tam*b);
					int _y = y+(tam*a);
					if(map[a*ancho+b] == '*'){
						rectangulo_lleno(_x, _y, _x+tam, _y+tam);
					}
				}
			}
			
			x+= (ancho*tam)+5;
		}
	}else{
		for(int i = 0; i<vidas; i++){
			for(int a = 0; a<alto; a++){
				for(int b = 0; b<ancho; b++){
					int _x = x+(tam*b);
					int _y = y+(tam*a);
					if(map[a*ancho+b] == '*'){
						rectangulo_lleno(_x, _y, _x+tam, _y+tam);
					}
				}
			}
			
			x+= (ancho*tam)+5;
		}
	}
}

bool gameOver(int vidas){
	return vidas <= 0;
}

void creaMarcianos(vector<marciano>& marci){
	for(int i=0; i<nMarcianos; i++){
		marci.push_back(marciano(20+(i*(tam*tamM1)), 60, altoM, tamM1, 3, *marciano1));
		marci.push_back(marciano(24+(i*(tam*tamM2+8)), 61+((altoM/2)*tam), altoM, tamM2, 2, *marciano2));
		marci.push_back(marciano(20+(i*(tam*tamM3+2)), 63+((altoM/2)*tam)*2, altoM, tamM3, 3, *marciano3));
	}
}
