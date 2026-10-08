#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<stdbool.h>
#include<time.h>
#include "utiles.h"

#define NO_EXISTE_BARRA 0 /* Valor para que el panel no muestre la barra en dicho lugar*/
#define EXISTE_BARRA 1 /* Valor para que el panel muestre la barra en dicho lugar*/
#define ELEM_SIN_BARRA_IZQ 0 /* barra de control vacia para mostrar en el panel*/
#define ELEM_SIN_BARRA_DER 4 /* barra de control vacia para mostrar en el panel*/
#define CANT_LINEAS_BARRAS 3 /* cantidad de lineas que ocupa una barra de control formada con ASCII*/
#define VALOR_MAX_DEMANDA 250 /* cantidad de demanda maxima para el aleatorio*/
#define VALOR_MIN_DEMANDA 50 /* cantidad de demanda minima para el aleatorio*/
#define CANTIDAD_TURNOS 3 /* cantidad de turnos que hay en un dia */
#define FILA_INICIO_MENU 17 /* fila en donde empieza el menu */
#define OPCIONES_MENU 6 /* cantidad de opciones del menu */
#define OPCION_SALIR 6 /* opción del menú que finaliza el programa */
#define FILA_BARRAS 3 /* filas de las barras de control*/
#define COLUMNAS_BARRAS 5 /* columnas de las barras de control*/


// Declaraciones adelantadas de las funciones
int obtnrBarrasDeControl();
void mostrarMenu(char VecMenu[OPCIONES_MENU][50]);
void mostrarMonitor(int t, int d, int inc);
int crearDemanda();
int pasarTurnos(int turno);
int pasarDia(int turno, int dia);
void mostrarPanel(int cantfuelactivo);
int seleccionaItem(char VecMenu[OPCIONES_MENU][50]);

int main() {
	int agua = 0, barras = 0, contIncidentes = 0, demanda, dia = 0, incidentes = 0, opc, pot = 0, pres = 0, temp = 0, turno = 0, vTurb = 0;
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
	 pero el jugador debe afrontar las consecuencias de la jornada anterior 
	 
	 Objeto a hacer: Reactor, Caracteristicas ->  agua, pot, pres, temp, react, Xe*/
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
			printf("------------------- | La demanda generada por la ciudad es: %d MW.\n", demanda);
			mostrarMonitor(turno, dia, incidentes);
			printf("------------------- | \n");
			mostrarPanel(barras);
			printf("Ingrese una opcion\n");
			mostrarMenu(VecMenu);
			opc = seleccionaItem(VecMenu);
			switch (opc) {
			case 1:
				/* Iniciar el reactor, en teoria, deberia ser algo que solamente veriamos en el primer turno */
				/* y luego deberiamos cambiarlo por una opcion que nos permita manipular directamente las barras de combustible */
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
				barras = obtnrBarrasDeControl();
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
				scanf(" %d", &agua);
				printf(" ...Presione Enter para continuar...\n");
				printf(" \n");
				getchar();
				system("cls");
				/* Esta opcion hecha funcion, agregar un "si" para mostrar si la cantidad de agua subio o bajo con el nuevo valor ingresado - Tener limite */
				break;
			case 5:
				system("cls");
				printf(" Terminar turno\n");
				printf(" ...Presione Enter para continuar...\n");
				printf(" \n");
				turno = pasarTurnos(turno);
				dia = pasarDia(turno, dia);
				contIncidentes++;
				incidentes = trunc((contIncidentes)/CANTIDAD_TURNOS);
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
	return 0;
}
int obtnrBarrasDeControl() {
	int d;
	scanf(" %d", &d);
	return d;
}
void mostrarMenu(char VecMenu[][50]) {
    int i = 0;
    printf("\033[7m\033[33m %s \033[0m\033[27m\n", VecMenu[i]); //Muestra el primer elemento con el formato de fondo rellenado, con color amarillo
    for (i = 1; i < OPCION_SALIR; i++)
    {
        printf("\033[33m %s \033[0m\n", VecMenu[i]); //muestra el resto de las opciones como letras normales, con color amarillo
    }
}
void mostrarMonitor(int t, int d, int inc) {
	printf("Dias Sin Incidentes | El dia es: %d\n", d);
	switch (t) {
	case 0:
		printf("        %d           | Turno Matutino.\n", inc);
		break;
	case 1:
		printf("        %d           | Turno Vespertino.\n", inc);
		break;
	case 2:
		printf("        %d           | Turno Nocturno.\n", inc);
		break;
	}
}
int crearDemanda() {
	int demanda;
	demanda = rand()%(VALOR_MAX_DEMANDA-(VALOR_MIN_DEMANDA-1))+VALOR_MIN_DEMANDA;
	return demanda;
}
int pasarTurnos(int turno) {
	turno++;
	if (turno >= CANTIDAD_TURNOS) {
		turno = 0;
	}
	return turno;
}
int pasarDia(int turno, int dia) {
	if (turno % CANTIDAD_TURNOS == 0) {
		dia++;
	}
	return dia;
}
void mostrarPanel(int cantfuelactivo) {
	int cont, i, j, k;
	int matriz[FILA_BARRAS][COLUMNAS_BARRAS];
	printf("\n");
	
	cont = 0;
	for (i=0; i<FILA_BARRAS; i++) {
		for (j=0; j<COLUMNAS_BARRAS; j++) {
			if (i==1) {
				matriz[i][j] = EXISTE_BARRA;
			} else {
				if (j==ELEM_SIN_BARRA_IZQ || j==ELEM_SIN_BARRA_IZQ) {
					matriz[i][j] = NO_EXISTE_BARRA;
				} else {
					matriz[i][j] = EXISTE_BARRA;
				}
			}
		}
	}
	for (i=0; i<FILA_BARRAS; i++) {
		for (k=0; k<CANT_LINEAS_BARRAS; k++) {
			for (j=0; j<COLUMNAS_BARRAS; j++) {
				if (matriz[i][j]==NO_EXISTE_BARRA) {
					printf("         ");
				} else {
					if (cont<cantfuelactivo) {
						switch (k) {
						case 0:
							printf("  /***\\  ");
							break;
						case 1:
							printf(" (  X  ) ");
							cont++;
							break;
						case 2:
							printf("  \\___/  ");
							break;
						default:
                        break;
						}
					} else {
						switch (k) {
						case 0:
							printf("  /***\\  ");
							break;
						case 1:
							printf(" (     ) ");
							cont++;
							break;
						case 2:
							printf("  \\___/  ");
							break;
						default:
                        break;
						}
					}
				}
			}
			printf("\n");
		}
	}
	printf("\n");
}
int seleccionaItem(char VecMenu[][50])
{
    int tecla;
    int i = 0;

    do
    {
        tecla = getch();

        if (tecla == 0 || tecla == 224) /* prefijo de tecla especial */
        {
            tecla = getch();

            switch (tecla)
            {
            case 80: /* flecha abajo */
                /* despinta el ítem actual */
                printf("\033[%d;1H\033[K\033[33m %s \033[0m\n", i+FILA_INICIO_MENU, VecMenu[i]);// el "\033[%d;1H\033[K" es para borrar la línea, más para saber en que línea escribir, \033[33m es para el color amarillo, \033[0m resetea formato de colores

                i++;
                if (i > OPCION_SALIR - 1)
                    i = 0;

                /* pinta el nuevo ítem seleccionado */
                printf("\033[%d;1H\033[K\033[7m\033[33m %s \033[0m\033[27m\n", i+FILA_INICIO_MENU, VecMenu[i]); //[7m\033 es para el fondo rellenado, \033[27m resetea el fondo para que sea normal
                break;

            case 72: /* flecha arriba */
                printf("\033[%d;1H\033[K\033[33m %s \033[0m\n", i+FILA_INICIO_MENU, VecMenu[i]); // el "\033[%d;1H\033[K" es para borrar la línea, más para saber en que línea escribir, \033[33m es para el color amarillo, \033[0m resetea formato de colores

                i--;
                if (i < 0)
                    i = OPCION_SALIR - 1;

                printf("\033[%d;1H\033[K\033[7m\033[33m %s \033[0m\033[27m\n", i+FILA_INICIO_MENU, VecMenu[i]); //[7m\033 es para el fondo rellenado, \033[27m resetea el fondo para que sea normal
                break;
            }
        }

    } while (tecla != 13); /* Enter */

    return i+1; // retorna la opción elegida (1 a 6)
}