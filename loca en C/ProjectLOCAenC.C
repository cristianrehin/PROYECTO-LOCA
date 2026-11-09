#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "utiles.h"
#include "funciones.h"

#define CANTIDAD_TURNOS 3 /* cantidad de turnos que hay en un dia */
#define OPCIONES_MENU 9	  /* cantidad de opciones del menu */
#define ULTIMA_OPCION 8	  /* opción del menú que finaliza el programa */
#define ESCAPE 1

// Definir la estructura del reactor
typedef struct
{
	int agua;
	int pot;
	int barras;
	float pres;
	float temp;
	float react;
	float Xe;
} Reactor;

// Declaraciones adelantadas de las funciones
void MostrarMenu(char VecMenu[OPCIONES_MENU][50], bool encendido, bool skalaencendida);
void MostrarMonitor(int t, int d, int inc, int def, int exc, int dem);
void MostrarEstado(bool encendido, float pctagua, float aguav, float reac, float tempe, float presi, bool vapr, bool valvula, float velturb, float potenciaagenerar, float potencia, float dem);
void CompararGeneracionDemanda(float generacion, float dem);
void MostrarReactividad(bool encendido, float reac);
void MostrarPanel(int cantfuelactivo);
void NombreEscenario(int reac);
void Finales(int tipofin);
int CrearDemanda();
int PasarTurnos(int turno);
int PasarDia(int turno, int dia);
int SeleccionaItem(char VecMenu[OPCIONES_MENU][50], bool reactorP, bool skalaActiva);
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
bool MostrarAlertasSkala(bool encendido, float temp, float presion, float react, float pctagua);

int main()
{
	Reactor reactorSimulado;
	reactorSimulado.pot = 0, reactorSimulado.pres = 1;
	reactorSimulado.react = 0, reactorSimulado.temp = 20, reactorSimulado.barras = 11;

	int demanda = 120, dia = 0, opc, turno = 0, cantDiasExc = 0, cantDiasDef = 0, diasSinAccidentes = 0, aguaSolicitada, barrasSolicitadas, incidentesSkala = 0;
	int porcentajeAgua = 50, potAGen, queFinal = 0, turnosAceptables = 0, turnosDeficit = 0, turnosExcedidos = 0, vTurb= 0;
	bool reactorP = false, vapor = false, eventoSeguridadEnDia, huboAlertaSkala = false, incidenteTemperaturaActivo = false, skalaActiva = false;
	bool skalaPendiente = false, valV = false;
	char VecMenu[OPCIONES_MENU][50] = {"Iniciar reactor", "Panel de barras de control", "Abrir/Cerrar valvula de vapor", "Circulacion de agua (porcentaje)", "Terminar turno", "Apagar reactor", "Salir del juego", "Desactivar SKALA (actualmente encendida)", "Activar SKALA (actualmente apagada)"};
	demanda = CrearDemanda(demanda);

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
		MostrarMonitor(turno, dia, diasSinAccidentes, cantDiasExc, cantDiasDef, demanda);
		MostrarPanel(reactorSimulado.barras);
		if (skalaPendiente == true)
		{
			if (skalaActiva == true)
			{
				huboAlertaSkala = MostrarAlertasSkala(reactorP, reactorSimulado.temp, reactorSimulado.pres, reactorSimulado.react, porcentajeAgua);
				if (huboAlertaSkala == true)
				{
					diasSinAccidentes = 0;
					eventoSeguridadEnDia = true;
				}
				if (reactorSimulado.temp >= 700)
				{
					if (incidenteTemperaturaActivo == false)
					{
						incidentesSkala = incidentesSkala + 1;
						incidenteTemperaturaActivo = true;
						printf(" SKALA: INCIDENTE TERMICO REGISTRADO. Total de incidentes termicos: %d\n", incidentesSkala);
						diasSinAccidentes = 0;
						eventoSeguridadEnDia = true;
					}
				}
				else
				{
					incidenteTemperaturaActivo = false;
				}
			}
			else
			{
				/* Aunque SKALA este desactivada, un incidente termico sigue contando como accidente. */
				if (reactorSimulado.temp >= 700)
				{
					if (incidenteTemperaturaActivo == false)
					{
						incidentesSkala = incidentesSkala + 1;
						incidenteTemperaturaActivo = true;
						printf(" INCIDENTE TERMICO REGISTRADO. Total de incidentes termicos: %d\n", incidentesSkala);
						diasSinAccidentes = 0;
						eventoSeguridadEnDia = true;
					}
				}
				else
				{
					incidenteTemperaturaActivo = false;
				}
			}
			skalaPendiente = false;
		}
		MostrarMenu(VecMenu, reactorP, skalaActiva);
		opc = SeleccionaItem(VecMenu, reactorP, skalaActiva);
		switch (opc)
		{
		case 1:
			/* Iniciar el reactor: solo disponible mientras esta apagado. No calcula nada: el estado */
			/* cambia al terminar el turno (el reactor no genera electricidad de inmediato). */
			system("cls");
			printf(" Iniciando reactor\n");
			printf(" Reactor encendido. Configure agua, barras y valvula, y luego termine el turno.\n");
			printf(" ...Presione Enter para continuar...\n");
			printf(" \n");
			reactorP = true;
			reactorSimulado.react = Reactividad(reactorSimulado.agua, reactorSimulado.barras);
			getchar();
			system("cls");
			break;
		case 2:
			system("cls");
			printf(" Ingrese la cantidad de barras de control (0 a 11).\n");
			barrasSolicitadas = ObtnrBarrasDeControl();
			/* Aviso previo: retirar todas las barras. SKALA no bloquea ni revierte la accion. */
			if (skalaActiva == true && barrasSolicitadas == 0 && reactorSimulado.barras > 0)
			{
				printf(" *** SKALA - ADVERTENCIA PREVENTIVA ***\n");
				printf(" Esta a punto de retirar todas las barras de control.\n");
				printf(" La reactividad puede aumentar bruscamente. Inserte barras para limitarla.\n");
				printf(" Continue bajo su propio riesgo.\n");
				diasSinAccidentes = 0;
				eventoSeguridadEnDia = true;
			}
			reactorSimulado.barras = barrasSolicitadas;
			if (reactorP == true)
			{
				reactorSimulado.react = Reactividad(reactorSimulado.agua, reactorSimulado.barras);
			}
			printf(" ...Presione Enter para continuar...\n");
			printf(" \n");
			getchar();
			system("cls");
			break;
		case 3:
			if (valV == false)
			{
				system("cls");
				printf(" Abriendo Valvulas\n");
				printf(" ...Presione Enter para continuar...\n");
				printf(" \n");
				printf(" Decrece la cantidad de Vapor\n");
				printf(" \n");
				getchar();
				system("cls");
				valV = true;
				/* La potencia se recalcula al terminar el turno, con el estado actualizado. */
			}
			else
			{
				system("cls");
				printf(" Cerrando Valvulas\n");
				printf(" ...Presione Enter para continuar...\n");
				printf(" \n");
				printf(" Decrece la cantidad de Vapor\n");
				printf(" \n");
				getchar();
				system("cls");
				valV = false;
				/* La potencia se recalcula al terminar el turno, con el estado actualizado. */
			}
			break;
		case 4:
			aguaSolicitada = ObtenerPorcentajeAgua(porcentajeAgua);
			/* Aviso previo: reducir la circulacion por debajo del 30 %. No impide continuar. */
			if (skalaActiva == true && aguaSolicitada < 30 && aguaSolicitada < porcentajeAgua)
			{
				printf(" *** SKALA - ADVERTENCIA PREVENTIVA ***\n");
				printf(" Esta reduciendo la circulacion de agua por debajo del 30 %%.\n");
				printf(" La refrigeracion puede resultar insuficiente. Mantenga agua por encima del 40 %% si la temperatura es elevada.\n");
				printf(" Continue bajo su propio riesgo.\n");
				diasSinAccidentes = 0;
				eventoSeguridadEnDia = true;
			}
			porcentajeAgua = aguaSolicitada;
			/* Agua y reactividad dependen entre si en el modelo del juego. */
			reactorSimulado.agua = ConvertirAgua(porcentajeAgua);
			if (reactorP == true)
			{
				reactorSimulado.react = Reactividad(reactorSimulado.agua, reactorSimulado.barras);
			}
			printf(" ...Presione Enter para continuar...\n");
			printf(" \n");
			getchar();
			system("cls");
			break;
		case 5:
			system("cls");
			printf(" Terminar turno\n");
			printf(" ...Presione Enter para continuar...\n");
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
			printf(" Resultado del turno:\n");
			CompararGeneracionDemanda(reactorSimulado.pot, demanda);
			/* Las alertas se mostraran una sola vez al entrar al turno siguiente. */
			skalaPendiente = true;
			/* Evaluacion de este turno con tolerancia de +/- 30 MW. */
			if (reactorSimulado.pot >= demanda - 30 && reactorSimulado.pot <= demanda + 30)
			{
				turnosAceptables = turnosAceptables + 1;
			}
			else
			{
				if (reactorSimulado.pot > demanda + 30)
				{
					turnosExcedidos = turnosExcedidos + 1;
				}
				else
				{
					turnosDeficit = turnosDeficit + 1;
				}
			}
			turno = PasarTurnos(turno);
			dia = PasarDia(turno, dia);
			if (turno == 0)
			{
				/* Un dia sin accidentes solo cuenta si no hubo incidentes ni alertas de SKALA. */
				if (eventoSeguridadEnDia == false)
				{
					diasSinAccidentes = diasSinAccidentes + 1;
				}
				eventoSeguridadEnDia = false;
				/* Un dia cumple si al menos 2 de sus 3 turnos quedaron dentro de +/- 30 MW. */
				if (turnosAceptables >= 2)
				{
					cantDiasExc = 0;
					cantDiasDef = 0;
					printf(" Balance diario aceptable: %d de 3 turnos dentro de la tolerancia.\n", turnosAceptables);
				}
				else
				{
					if (turnosExcedidos > turnosDeficit)
					{
						cantDiasExc = cantDiasExc + 1;
						cantDiasDef = 0;
					}
					else
					{
						cantDiasDef = cantDiasDef + 1;
						cantDiasExc = 0;
					}
				}
				demanda = CrearDemanda(demanda);
				printf(" Comienza el dia %d. Nueva demanda: %d MW.\n", dia, demanda);
				/* La potencia actual se conserva al cambiar el dia; no se acumula ni se reinicia. */
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
				printf(" Apagando Reactor\n");
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
				printf(" El reactor ya esta apagado.\n");
			}
			printf(" ...Presione Enter para continuar...\n");
			printf(" \n");
			getchar();
			system("cls");
			break;
		case 7:
			system("cls");
			queFinal = ESCAPE;
			break;
		case 8:
			if (skalaActiva == true)
			{
				skalaActiva = false;
				skalaPendiente = false;
				printf(" SKALA DESACTIVADA. No se mostraran advertencias.\n");
			}
			else
			{
				skalaActiva = true;
				skalaPendiente = false;
				printf(" SKALA ACTIVADA. Evaluara el estado al finalizar el proximo turno.\n");
			}
			printf(" ...Presione Enter para continuar...\n");
			getchar();
			system("cls");
			break;
		default:
			system("cls");
			printf("Ingrese una opcion valida\n");
			printf("\n");
		}
	} while (queFinal == 0);

	Finales(queFinal);
	if (queFinal >= 4)
	{
		printf("\n");
		printf("Dias completos sobrevividos: %d\n", dia - 1);
		printf("Presione una tecla para continuar...\n");
		getchar();
		system("cls");
		while (true)
		{
			printf(" \n");
		}
	}
	return 0;
}
