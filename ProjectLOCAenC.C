#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<stdbool.h>
#include<time.h>
#include "utiles.h"
#include "funciones.h"

#define CANTIDAD_TURNOS 3 /* cantidad de turnos que hay en un dia */
#define OPCIONES_MENU 6 /* cantidad de opciones del menu */
#define OPCION_SALIR 6 /* opción del menú que finaliza el programa */

// Definir la estructura del reactor
typedef struct{
	int agua;
 	int pot;
	int barras;
	float pres;
	float temp;
	float react;
	float Xe;
}Reactor;

// Declaraciones adelantadas de las funciones
int obtnrBarrasDeControl();
void mostrarMenu(char VecMenu[OPCIONES_MENU][50]);
void mostrarMonitor(int t, int d, int inc, int def, int exc, int dem);
int crearDemanda();
int pasarTurnos(int turno);
int pasarDia(int turno, int dia);
void mostrarPanel(int cantfuelactivo);
int seleccionaItem(char VecMenu[OPCIONES_MENU][50]);

int main() {
	Reactor reactorSimulado;
	reactorSimulado.agua = 0, reactorSimulado.pot = 0, reactorSimulado.pres = 0;
	reactorSimulado.react = 0, reactorSimulado.temp = 0, reactorSimulado.Xe = 0;
	reactorSimulado.barras = 0;
	int demanda, dia = 0, opc, turno = 0, vTurb = 0, aguaAnterior = 0, cantDiasExc = 0, cantDiasDef = 0, diasSinAccidentes = 0;
	bool estasvivo, reactorp, vapor;
	/* 'estasVivo' es el logico que controla que el juego se repita turno a turno
	 Variables:
	 Agua: cant. de agua en el reactor en %.								[agua] Independiente
	 Valvula de Vapor: vapor que sale del reactor.							[vapor] Independiente
	 Temperatura: temperatura del agua en °C.								[temp] Depende de 'agua' y 'react'
	 Presion: presion dentro del reactor.									[pres] Depende de 'agua' y 'temp'
	 Potencia: energia generada en el dia de la misma demanda.				[pot] Depende de la 'vTurb'
	 Velocidad de la Turbina: para calcular energia generada.				[vTurb] Depende de la 'pres' y de 'vapor'

	 Barras de control: valores entre 0 y 11, su porcentaje va a ir hasta 110% pero el jugador verá solo 100%, cada valor equivale 10%  [barrasDeContr] Independiente
	 Reactividad: cantidad de energia que genera los combustibles individualmente.                                                      [react] Depende de 'agua', 'barrasDeContr' y 'Xe'
	 Zenon: reduce la reactividad del reactor, se acumula cuando se genera poca energia.                                                [Xe] Depende de 'react' y 'temp' (tiene 1 turno de half-life)

	 Opc es la accion que selecciona el jugador 
	 Demanda es la cantidad de energia a generar para satisfacer la demanda 
	 Turno es la jornada laboral, mañana, tarde y noche, en la que dividimos el dìa
	 y en la que el jugador podrà tomar accion 
	 Dia es el dia en que se da la accion 
	 Turno a turno la Demanda ya generada, deberìa tener una fluctuacìon menor 
	 por ejemplo, un pequeño aumento de la demanda en la tarde y un aumento mayor en la noche 
	 pero en el cambio de dia, deberiamos ver reiniciarse la demanda de energia 
	 pero el jugador debe afrontar las consecuencias de la jornada anterior */
	char VecMenu[OPCIONES_MENU][50] = {"Iniciar reactor", "Panel de barras de control", "Abrir/Cerrar valvula de vapor", "Nivel de agua", "Terminar turno", "Apagar reactor"};
    srand(time(NULL));
	estasvivo = true;
	reactorp = false;
	vapor = false;
	demanda = crearDemanda();
	/* Fin de la inicializacion de las variables. */
	system("cls");
	do {
		opc = 0;
		while (opc!=OPCION_SALIR) {
			mostrarMonitor(turno, dia, diasSinAccidentes, cantDiasExc, cantDiasDef, demanda);
			mostrarPanel(reactorSimulado.barras);
			printf("Ingrese una opcion\n");
			mostrarMenu(VecMenu);
			opc = seleccionaItem(VecMenu);
			switch (opc) {
			case 1:
				/* Iniciar el reactor, en teoria, deberia ser algo que solamente veriamos en el primer turno
				y luego deberiamos cambiarlo por una opcion que nos permita manipular directamente las barras de combustible */
				if (reactorp==false) {
				    system("cls");
					printf(" Iniciando reactor\n");
					printf(" ...Presione Enter para continuar...\n");
                    getchar();
					reactorp = true;
				} else {
				    system("cls");
					printf(" Reactor ya iniciado.\n");
					printf(" ...Presione Enter para continuar...\n");
                    getchar();
				}
				system("cls");
				break;
			case 2:
				system("cls");
				printf(" Ingrese la cantidad de barras de control.\n");
				reactorSimulado.barras = obtnrBarrasDeControl();
				getchar();
				system("cls");
				break;
			case 3:
				if (vapor==false) {
					system("cls");
					printf(" Abriendo Valvulas\n");
					printf(" ...Presione Enter para continuar...\n");
                    getchar();
					system("cls");
					vapor = true;
				} else {
					system("cls");
					printf(" Cerrando Valvulas\n");
					printf(" ...Presione Enter para continuar...\n");
                    getchar();
					system("cls");
					vapor = false;
				}
				break;
			case 4:
				system("cls");
				printf(" Ingrese el nivel de agua en el reactor\n");
				scanf(" %d", &reactorSimulado.agua);
				if(aguaAnterior < reactorSimulado.agua) printf("Se aumento el nivel del agua, el valor anterior del agua fue %d\n", aguaAnterior);
				else printf("Se bajo el nivel del agua, el valor anterior del agua fue %d\n", aguaAnterior);
				aguaAnterior = reactorSimulado.agua;
				printf(" ...Presione Enter para continuar...\n");
				printf(" \n");
				getchar();
				getchar();
				system("cls");
				break;
			case 5:
				system("cls");
				printf(" Terminar turno\n");
				printf(" ...Presione Enter para continuar...\n");
				printf(" \n");
				turno = pasarTurnos(turno);
				dia = pasarDia(turno, dia);
				getchar();
				system("cls");
				opc = OPCION_SALIR;
				break;
			case OPCION_SALIR:
				printf(" Apagando Reactor\n");
				printf(" ...Presione Enter para continuar...\n");
				getchar();
				system("cls");
				reactorp = false;
				estasvivo = false;
				printf("Termino el juego (placeholder end)\n");
				break;
			default:
				system("cls");
				printf("Ingrese una opcion valida\n");
				printf("\n");
			}
		}
	} while (estasvivo!=false);
	mostrarExplosion();
	return 0;
}
