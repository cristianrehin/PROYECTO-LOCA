#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "funciones.h"

/* Funcion de monitoreo definida en SKALA.C. */
void EvaluarSkala(const Reactor *reactor, int porcentajeAgua);

int main()
{
	srand(time(NULL));
	Reactor reactorSimulado;
	FILE *fp;
	int demanda, dia, opc, turno, cantDiasExc, cantDiasDef, diasSinAccidentes, aguaSolicitada, barrasSolicitadas;
	int porcentajeAgua, potAGen, queFinal, turnosAceptables, turnosDeficit, turnosExcedidos, vTurb;
	int reactorP, vapor, valV, existePartidaAnterior;
	fp = fopen("persistencia.txt", "r");
	if (fp != NULL)
		fscanf(fp,"%d",&existePartidaAnterior);
		else
			existePartidaAnterior = NO_EXISTE_VALORES_ANTERIORES;
	
	if (existePartidaAnterior == VALORES_ANTERIORES)
	{	
		if (fp != NULL){
			fscanf(fp, "%d\n", &demanda);
			fscanf(fp, "%d\n", &dia);
			fscanf(fp, "%d\n", &turno);
			fscanf(fp, "%d\n", &cantDiasExc);
			fscanf(fp, "%d\n", &cantDiasDef);
			fscanf(fp, "%d\n", &diasSinAccidentes);
			fscanf(fp, "%d\n", &porcentajeAgua);
			fscanf(fp, "%d\n", &potAGen);
			fscanf(fp, "%d\n", &queFinal);
			fscanf(fp, "%d\n", &turnosAceptables);
			fscanf(fp, "%d\n", &turnosDeficit);
			fscanf(fp, "%d\n", &turnosExcedidos);
			fscanf(fp, "%d\n", &vTurb);
			fscanf(fp, "%d\n", &reactorP);
			fscanf(fp, "%d\n", &vapor);
			fscanf(fp, "%d\n", &valV);
			fscanf(fp, "%d\n", &reactorSimulado.pot);
			fscanf(fp, "%f\n", &reactorSimulado.pres);
			fscanf(fp, "%f\n", &reactorSimulado.react);
			fscanf(fp, "%f\n", &reactorSimulado.temp);
			fscanf(fp, "%d\n", &reactorSimulado.barras);
			fclose(fp);	
		}
	}else{
		SubirDatosNuevos();
		fp = fopen("persistencia.txt", "r");
		if (fp != NULL){
			int valorCheckeo;
			fscanf(fp, "%d\n", &valorCheckeo);
			fscanf(fp, "%d\n", &demanda);
			fscanf(fp, "%d\n", &dia);
			fscanf(fp, "%d\n", &turno);
			fscanf(fp, "%d\n", &cantDiasExc);
			fscanf(fp, "%d\n", &cantDiasDef);
			fscanf(fp, "%d\n", &diasSinAccidentes);
			fscanf(fp, "%d\n", &porcentajeAgua);
			fscanf(fp, "%d\n", &potAGen);
			fscanf(fp, "%d\n", &queFinal);
			fscanf(fp, "%d\n", &turnosAceptables);
			fscanf(fp, "%d\n", &turnosDeficit);
			fscanf(fp, "%d\n", &turnosExcedidos);
			fscanf(fp, "%d\n", &vTurb);
			fscanf(fp, "%d\n", &reactorP);
			fscanf(fp, "%d\n", &vapor);
			fscanf(fp, "%d\n", &valV);
			fscanf(fp, "%d\n", &reactorSimulado.pot);
			fscanf(fp, "%f\n", &reactorSimulado.pres);
			fscanf(fp, "%f\n", &reactorSimulado.react);
			fscanf(fp, "%f\n", &reactorSimulado.temp);
			fscanf(fp, "%d\n", &reactorSimulado.barras);
			fclose(fp);	
		}
	}
	demanda = CrearDemanda(demanda);
	reactorSimulado.agua = ConvertirAgua(porcentajeAgua);
	char VecMenu[OPCIONES_MENU][50] = {"Iniciar reactor", "Panel de barras de control", "Abrir/Cerrar valvula de vapor", "Circulacion de agua (porcentaje)", "Terminar turno", "Apagar reactor", "Salir del juego"};

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
	system("cls");
	do
	{
		MostrarMonitor(turno, dia, diasSinAccidentes, cantDiasDef, cantDiasExc, demanda);
		MostrarPanel(&reactorSimulado);
		MostrarEstado(reactorP, porcentajeAgua, &reactorSimulado, vapor, valV, vTurb, potAGen, demanda);
		MostrarMenu(VecMenu);
		opc = SeleccionaItem(VecMenu);
		switch (opc)
		{
		case ENCEN_REACTOR:
		/* Iniciar el reactor: solo disponible mientras esta apagado. No calcula nada: el estado */
		/* cambia al terminar el turno (el reactor no genera electricidad de inmediato). */
		if (reactorP == false)
		{
			system("cls");
			printf("%s Iniciando reactor\n", AMARILLO);
			printf(" Reactor encendido. Configure agua, barras y valvula, y luego termine el turno.\n");
			printf(" ...Presione Enter para continuar...%s\n", RESET_FORMATO_TEXTO);
			printf(" \n");
			reactorP = true;
			reactorSimulado.react = Reactividad(&reactorSimulado);
			getchar();
			system("cls");
		}else 
		{
			system("cls");
			printf("%s El reactor ya esta prendido.\n", AMARILLO); 
			printf(" ...Presione Enter para continuar...%s\n", RESET_FORMATO_TEXTO);
			getchar();
			system("cls");
		}
		break;
		case INGRESAR_BARRAS:
			system("cls");
			printf("%s Ingrese la cantidad de barras de control (0 a 11).%s\n", AMARILLO, RESET_FORMATO_TEXTO);
			barrasSolicitadas = ObtnrBarrasDeControl();
			/* Aviso previo: retirar todas las barras. SKALA no bloquea ni revierte la accion. */
			reactorSimulado.barras = barrasSolicitadas;
			if (reactorP == true)
			{
				reactorSimulado.react = Reactividad(&reactorSimulado);
			}
			printf("%s ...Presione Enter para continuar...%s\n", AMARILLO, RESET_FORMATO_TEXTO);
			printf(" \n");
			getchar();
			system("cls");
			break;
		case ABR_VALVULA:
			if (valV == false)
			{
				system("cls");
				printf("%s Abriendo Valvulas\n", AMARILLO);
				printf(" ...Presione Enter para continuar...%s\n", RESET_FORMATO_TEXTO);
				printf(" \n");
				getchar();
				system("cls");
				valV = true;
				/* La potencia se recalcula al terminar el turno, con el estado actualizado. */
			}
			else
			{
				system("cls");
				printf("%s Cerrando Valvulas\n", AMARILLO);
				printf(" ...Presione Enter para ontinuar...%s\n", RESET_FORMATO_TEXTO);
				printf(" \n");
				getchar();
				system("cls");
				valV = false;
				/* La potencia se recalcula al terminar el turno, con el estado actualizado. */
			}
			break;
		case CANT_AGUA:
			system("cls");
			aguaSolicitada = ObtenerPorcentajeAgua(porcentajeAgua);
			porcentajeAgua = aguaSolicitada;
			/* Agua y reactividad dependen entre si en el modelo del juego. */
			reactorSimulado.agua = ConvertirAgua(porcentajeAgua);
			if (reactorP == true)
			{
				reactorSimulado.react = Reactividad(&reactorSimulado);
			}
			printf("%s ...Presione Enter para continuar...%s\n", AMARILLO, RESET_FORMATO_TEXTO);
			printf(" \n");
			getchar();
			system("cls");
			break;
		case FIN_TURNO:
			system("cls");
			printf("%s Terminar turno\n", AMARILLO);
			printf(" ...Presione Enter para continuar...%s\n", RESET_FORMATO_TEXTO);
			printf(" \n");
			/* Orden operativo: (1) actualizar estado y potencia una sola vez, (2) evaluar este turno, */
			/* (3) avanzar turno/dia, (4) si empieza un dia nuevo, evaluar el dia terminado y crear demanda nueva. */
			/* (5) Checkea si excede o tiene deficit de energia */
			/* Actualizar el estado una sola vez por turno. Todas las funciones devuelven */
			/* un valor; no se pasan variables por referencia. */
			reactorSimulado.agua = ConvertirAgua(porcentajeAgua);
			if (reactorP == true)
			{
				reactorSimulado.temp = Temperatura(&reactorSimulado);
			}
			else
			{
				reactorSimulado.react = 0;
				reactorSimulado.temp = TemperaturaApagado(&reactorSimulado);
			}
			reactorSimulado.pres = PresionActualizada(&reactorSimulado, valV);
			vapor = HayVapor(reactorP, &reactorSimulado);
			vTurb = VelocidadTurbina(&reactorSimulado, vapor, valV);
			potAGen = PotenciaElectrica(vTurb);
			/* SKALA evalua las condiciones del reactor al cierre de cada turno. */
			EvaluarSkala(&reactorSimulado, porcentajeAgua);
			/* pot representa la potencia ACTUAL del turno, no una suma acumulada. */
			reactorSimulado.pot = potAGen;
			printf("%s Resultado del turno:%s\n", AMARILLO, RESET_FORMATO_TEXTO);
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
			PasarTurnosYDias(&turno, &dia);
			if (turno == 0)
			{
				/* Un dia cumple si al menos 2 de sus 3 turnos quedaron dentro de +/- 30 MW. */
				if (turnosAceptables >= 2)
				{
					cantDiasExc = 0;
					cantDiasDef = 0;
					printf("%s Balance diario aceptable: %d de 3 turnos dentro de la tolerancia.%s\n", AMARILLO, turnosAceptables, RESET_FORMATO_TEXTO);
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
				printf("%s Comienza el dia %d. Nueva demanda: %d MW.%s\n", AMARILLO, dia, demanda, RESET_FORMATO_TEXTO);
				/* La potencia actual se conserva al cambiar el dia; no se acumula ni se reinicia. */
				diasSinAccidentes++;
				turnosAceptables = 0;
				turnosExcedidos = 0;
				turnosDeficit = 0;
			}
			SubirDatosActualizados(&reactorSimulado,&demanda,&dia,&turno,&cantDiasExc,&cantDiasDef,&diasSinAccidentes,&porcentajeAgua,&potAGen,&queFinal,&turnosAceptables,&turnosDeficit,&turnosExcedidos,&vTurb, &reactorP, &vapor, &valV);
			getchar();
			system("cls");
			opc = SALIR;
			/* sale del bucle interno; el externo continua porque queFinal sigue en Verdadero */
			queFinal = CheckeoCatastrofes(&reactorSimulado);
			if (queFinal == 0)
			{
				queFinal = CheckeoDiasFinales(cantDiasDef, cantDiasExc);
			}
			break;
		case APAG_REACTOR:
			system("cls");
			if (reactorP == true)
			{
				printf("%s Apagando Reactor\n", AMARILLO);
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
				printf("%s El reactor ya esta apagado.\n", AMARILLO);
			}
			printf(" ...Presione Enter para continuar...%s\n", RESET_FORMATO_TEXTO);
			printf(" \n");
			getchar();
			system("cls");
			break;
		case ULTIMA_OPCION:
			system("cls");
			queFinal = FINAL_ESCAPE;
			Finales(queFinal);
			printf(" \n");
			printf(" ...Presione Enter para continuar...%s\n", RESET_FORMATO_TEXTO);
			getchar();
			break;
		default:
			system("cls");
			printf("%sIngrese una opcion valida%s\n", AMARILLO, RESET_FORMATO_TEXTO);
			printf("\n");
		}
	} while (queFinal == SIGUE_VIVO);

	Finales(queFinal);
	if (queFinal >= 4)
	{
		printf("\n");
		printf("%sDias completos sobrevividos: %d\n", AMARILLO, dia - 1);
		printf("Presione la tecla enter para continuar...%s\n", RESET_FORMATO_TEXTO);
		getchar();
		system("cls");
	}
	return 0;
}
