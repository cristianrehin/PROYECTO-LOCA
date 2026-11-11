#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "utiles.h"
#include "funciones.h"

#define CANTIDAD_TURNOS 3 /* cantidad de turnos que hay en un dia */
#define OPCIONES_MENU 7	  /* cantidad de opciones del menu */
#define ULTIMA_OPCION 7	  /* ultima opcion del menu */
#define TOLERANCIA 30 /* tolerancia de la energia generada y la demandada*/
#define ESCAPE 1 /* final 1: escape*/

// Definir la estructura del reactor
typedef struct
{
	int agua;
	int pot;
	int barras;
	float pres;
	float temp;
	float react;
} Reactor;

// Declaraciones adelantadas de las funciones
void MostrarMenu(char VecMenu[OPCIONES_MENU][50]);
void MostrarMonitor(int t, int d, int inc, int def, int exc, int dem);
void MostrarEstado(bool encendido, float pctagua, float aguav, float reac, float tempe, float presi, bool vapr, bool valvula, float velturb, float potenciaagenerar, float potencia, float dem);
void CompararGeneracionDemanda(float generacion, float dem);
void MostrarReactividad(bool encendido, float reac);
void MostrarPanel(int cantfuelactivo);
void NombreEscenario(float reac);
void Finales(int tipofin);
void MostrarReactividad(bool encendido, float reac);
int CrearDemanda();
int PasarTurnos(int turno);
int PasarDia(int turno, int dia);
int SeleccionaItem(char VecMenu[OPCIONES_MENU][50]);
int ObtenerPorcentajeAgua(int actual);
int CheckeoCatastrofes(float tempactual, float presactual);
int EscenarioReactividad(float reac);
int VelocidadTurbina(float presactual, bool hay, bool valvulaabierta);
int PotenciaElectrica(float vt);
int CheckeoDiasFinales(float def, float exc);
float ConvertirAgua(float pctagua);
float Reactividad(float aguav, float barrasctl);
float VariacionTermica(float reac, float aguav);
float Temperatura(float , float , float );
float TemperaturaApagado(float tempactual);
float PresionActualizada(float tempactual, float presactual, bool valvulaabierta);
bool HayVapor(bool encendido, float presi);

int main()
{
	Reactor reactorSimulado;
	reactorSimulado.pot = 0, reactorSimulado.pres = 1;
	reactorSimulado.react = 0, reactorSimulado.temp = 20, reactorSimulado.barras = 11;

	int demanda = 120, dia = 0, opc, turno = 0, cantDiasExc = 0, cantDiasDef = 0, diasSinAccidentes = 0, aguaSolicitada, barrasSolicitadas;
	int porcentajeAgua = 50, potAGen, queFinal = 0, turnosAceptables = 0, turnosDeficit = 0, turnosExcedidos = 0, vTurb= 0;
	bool reactorP = false, vapor = false, valV = false;
	char VecMenu[OPCIONES_MENU][50] = {"Iniciar reactor", "Panel de barras de control", "Abrir/Cerrar valvula de vapor", "Circulacion de agua (porcentaje)", "Terminar turno", "Apagar reactor", "Salir del juego"};
	demanda = CrearDemanda(demanda);
	reactorSimulado.agua = ConvertirAgua(porcentajeAgua);
	/*
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
	/* Fin de la inicializacion de las variables. */
	srand(time(NULL));
	system("cls");
	do
	{
		MostrarMonitor(turno, dia, diasSinAccidentes, cantDiasDef, cantDiasExc, demanda);
		MostrarPanel(reactorSimulado.barras);
		MostrarEstado(reactorP, porcentajeAgua, reactorSimulado.agua, reactorSimulado.react, reactorSimulado.temp, reactorSimulado.pres, vapor, valV, vTurb, potAGen, reactorSimulado.pot, demanda);
		MostrarMenu(VecMenu);
		opc = SeleccionaItem(VecMenu);
		switch (opc)
		{
		case 1:
		/* Iniciar el reactor: solo disponible mientras esta apagado. No calcula nada: el estado */
		/* cambia al terminar el turno (el reactor no genera electricidad de inmediato). */
		if (reactorP == false)
		{
			system("cls");
			printf("\033[33m Iniciando reactor\033[0m\n");
			printf("\033[33m Reactor encendido. Configure agua, barras y valvula, y luego termine el turno.\033[0m\n");
			printf("\033[33m ...Presione Enter para continuar...\033[0m\n");
			printf(" \n");
			reactorP = true;
			reactorSimulado.react = Reactividad(reactorSimulado.agua, reactorSimulado.barras);
			getchar();
			system("cls");
		}else 
		{
			system("cls");
			printf("\033[33m El reactor ya esta prendido. \033[0m\n"); 
			printf("\033[33m ...Presione Enter para continuar...\033[0m\n");
			getchar();
			system("cls");
		}
		break;
		case 2:
			system("cls");
			printf("\033[33m Ingrese la cantidad de barras de control (0 a 11).\033[0m\n");
			barrasSolicitadas = ObtnrBarrasDeControl();
			/* Aviso previo: retirar todas las barras. SKALA no bloquea ni revierte la accion. */
			reactorSimulado.barras = barrasSolicitadas;
			if (reactorP == true)
			{
				reactorSimulado.react = Reactividad(reactorSimulado.agua, reactorSimulado.barras);
			}
			printf("\033[33m ...Presione Enter para continuar...\033[0m\n");
			printf(" \n");
			getchar();
			getchar();
			system("cls");
			break;
		case 3:
			if (valV == false)
			{
				system("cls");
				printf("\033[33m Abriendo Valvulas\033[0m\n");
				printf("\033[33m ...Presione Enter para continuar...\033[0m\n");
				printf(" \n");
				printf("\033[33m Decrece la cantidad de Vapor\033[0m\n");
				printf(" \n");
				getchar();
				system("cls");
				valV = true;
				/* La potencia se recalcula al terminar el turno, con el estado actualizado. */
			}
			else
			{
				system("cls");
				printf("\033[33m Cerrando Valvulas\033[0m\n");
				printf("\033[33m ...Presione Enter para ontinuar...\033[0m\n");
				printf(" \n");
				printf("\033[33m Decrece la cantidad de Vapor\033[0m\n");
				printf(" \n");
				getchar();
				system("cls");
				valV = false;
				/* La potencia se recalcula al terminar el turno, con el estado actualizado. */
			}
			break;
		case 4:
			system("cls");
			aguaSolicitada = ObtenerPorcentajeAgua(porcentajeAgua);
			porcentajeAgua = aguaSolicitada;
			/* Agua y reactividad dependen entre si en el modelo del juego. */
			reactorSimulado.agua = ConvertirAgua(porcentajeAgua);
			if (reactorP == true)
			{
				reactorSimulado.react = Reactividad(reactorSimulado.agua, reactorSimulado.barras);
			}
			printf("\033[33m ...Presione Enter para continuar...\033[0m\n");
			printf(" \n");
			getchar();
			getchar();
			system("cls");
			break;
		case 5:
			system("cls");
			printf("\033[33m Terminar turno\033[0m\n");
			printf("\033[33m ...Presione Enter para continuar...\033[0m\n");
			printf(" \n");
			/* Orden operativo: (1) actualizar estado y potencia una sola vez, (2) evaluar este turno, */
			/* (3) avanzar turno/dia, (4) si empieza un dia nuevo, evaluar el dia terminado y crear demanda nueva. */
			/* (5) Checkea si excede o tiene deficit de energia */
			/* Actualizar el estado una sola vez por turno. Todas las funciones devuelven */
			/* un valor; no se pasan variables por referencia. */
			reactorSimulado.agua = ConvertirAgua(porcentajeAgua);
			if (reactorP == true)
			{
				reactorSimulado.temp = Temperatura(reactorSimulado.react, reactorSimulado.agua, reactorSimulado.temp);
			}
			else
			{
				reactorSimulado.react = 0;
				reactorSimulado.temp = TemperaturaApagado(reactorSimulado.temp);
			}
			reactorSimulado.pres = PresionActualizada(reactorSimulado.temp, reactorSimulado.pres, valV);
			vapor = HayVapor(reactorP, reactorSimulado.pres);
			vTurb = VelocidadTurbina(reactorSimulado.pres, vapor, valV);
			potAGen = PotenciaElectrica(vTurb);
			/* pot representa la potencia ACTUAL del turno, no una suma acumulada. */
			reactorSimulado.pot = potAGen;
			printf("\033[33m Resultado del turno:\033[0m\n");
			CompararGeneracionDemanda(reactorSimulado.pot, demanda);
			/* Evaluacion de este turno con tolerancia de +/- 30 MW. */
			if (reactorSimulado.pot >= demanda - TOLERANCIA && reactorSimulado.pot <= demanda + TOLERANCIA)
			{
				turnosAceptables = turnosAceptables + 1;
			}
			else
			{
				if (reactorSimulado.pot > demanda + TOLERANCIA)
				{
					turnosExcedidos++;
				}
				else
				{
					turnosDeficit++;
				}
			}
			turno = PasarTurnos(turno);
			dia = PasarDia(turno, dia);
			if (turno == 0)
			{
				/* Un dia cumple si al menos 2 de sus 3 turnos quedaron dentro de +/- 30 MW. */
				if (turnosAceptables >= 2)
				{
					cantDiasExc = 0;
					cantDiasDef = 0;
					printf("\033[33m Balance diario aceptable: %d de 3 turnos dentro de la tolerancia.\033[0m\n", turnosAceptables);
				}
				else
				{
					if (turnosExcedidos > turnosDeficit)
					{
						cantDiasExc++;
						cantDiasDef = 0;
					}
					else
					{
						cantDiasDef++;
						cantDiasExc = 0;
					}
				}
				demanda = CrearDemanda(demanda);
				printf("\033[33m Comienza el dia %d. Nueva demanda: %d MW.\033[0m\n", dia, demanda);
				/* La potencia actual se conserva al cambiar el dia; no se acumula ni se reinicia. */
				diasSinAccidentes++;
				turnosAceptables = 0;
				turnosExcedidos = 0;
				turnosDeficit = 0;

			}
			getchar();
			system("cls");
			opc = 7;
			/* sale del bucle interno; el externo continua porque queFinal sigue en Verdadero */
			queFinal = CheckeoCatastrofes(reactorSimulado.temp, reactorSimulado.pres);
			if (queFinal == 0)
			{
				queFinal = CheckeoDiasFinales(cantDiasDef, cantDiasExc);
			}
			break;
		case 6:
			system("cls");
			if (reactorP == true)
			{
				printf("\033[33m Apagando Reactor\033[0m\n");
				reactorP = false;
				/* Reactor apagado: sin vapor, turbina detenida y generacion cero. La temperatura y la presion */
				/* bajan en los proximos turnos (ActualizarEstado). */
				vapor = false;
				vTurb = 0;
				potAGen = 0;
				reactorSimulado.pot = 0;
			}
			else
			{
				printf("\033[33m El reactor ya esta apagado.\033[0m\n");
			}
			printf("\033[33m ...Presione Enter para continuar...\033[0m\n");
			printf(" \n");
			getchar();
			system("cls");
			break;
		case 7:
			system("cls");
			queFinal = ESCAPE;
			break;
		default:
			system("cls");
			printf("\033[33mIngrese una opcion valida\033[0m\n");
			printf("\n");
		}
	} while (queFinal == 0);

	Finales(queFinal);
	if (queFinal >= 4)
	{
		printf("\n");
		printf("\033[33mDias completos sobrevividos: %d\033[0m\n", dia - 1);
		printf("\033[33mPresione una tecla para continuar...\033[0m\n");
		getchar();
		system("cls");
		while (true)
		{
			printf(" \n");
		}
	}
	return 0;
}
