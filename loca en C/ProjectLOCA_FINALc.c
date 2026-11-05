/* Este codigo ha sido generado por el modulo psexport 20230904-w32 de PSeInt.
Es posible que el codigo generado no sea completamente correcto. Si encuentra
errores por favor reportelos en el foro (http://pseint.sourceforge.net). */

#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<stdbool.h>

// Declaraciones adelantadas de las funciones
void mostarmenuopc(bool encendido, bool skalaencendida);
int obtnrbarrasdecontrol();
void mostrarmonitor(int t, int d, int inc, int exc, int def, int dem);
int pasarturnos(int turnoactual);
int pasardia(int turnoactual, int diaactual);
void mostrarpanel(int cantbarrasinsertadas);
int obtenerporcentajeagua(int actual);
float convertiragua(float pctagua);
float reactividad(float aguav, float barrasctl);
int escenarioreactividad(float reac);
void nombreescenario(int reac);
float variaciontermica(float reac, float aguav);
float temperatura(float reac, float aguav, float tempactual);
float temperaturaapagado(float tempactual);
float presionactualizada(float tempactual, float presactual, bool valvulaabierta);
bool hayvapor(bool encendido, float presi);
int velocidadturbina(float presactual, bool hay, bool valvulaabierta);
int potenciaelectrica(float vt);
int creardemanda(float anterior);
void mostrarestado(bool encendido, float pctagua, float aguav, float reac, float tempe, float presi, bool vapr, bool valvula, float velturb, float potenciaagenerar, float potencia, float dem);
void comparargeneraciondemanda(float generacion, float dem);
void mostrarreactividad(bool encendido, float reac);
int checkeodiasfinales(float def, float exc);
void finales(int tipofin);
bool mostraralertasskala(bool encendido, float temp, float presion, float react, float pctagua);
int checkeocatastrofes(float tempactual, float presactual);

typedef struct{
	int agua;
 	int pot;
	int barras;
	float pres;
	float temp;
	float react;
}Reactor;

int main() {
	Reactor reactorSimulado;
	reactorSimulado.pot = 0, reactorSimulado.pres = 1;
	reactorSimulado.react = 0, reactorSimulado.temp = 20, reactorSimulado.barras = 11;

	int demanda, dia = 0, opc, turno = 0, cantDiasExc = 0, cantDiasDef = 0, diasSinAccidentes = 0, aguasolicitada, barrassolicitadas, incidentesskala;
	int porcentajeagua, potagen, quefinal, turnosaceptables, turnosdeficit, turnosexcedidos, vturb;
	bool reactorp, vapor, eventoseguridadendia, huboalertaskala, incidentetemperaturaactivo, skalaactiva;
	bool skalapendiente, valv;
	
	/* queFinal es el logico que controla que el juego se repita turno a turno */
	/* Variables: */
	/* Agua [agua]: volumen equivalente de agua circulado durante UN turno, en millones de litros (real). 100 % = 299,52 (10,4 m3/s x 28.800 s). */
	/* porcentajeAgua: % de circulacion elegido por el jugador (1 a 100).		[porcentajeAgua] */
	/* 'agua' NO es lo mismo que 'porcentajeAgua': agua se obtiene solo con ConvertirAgua(porcentajeAgua). */
	/* Temperatura [temp]: temperatura del reactor en �C. Minimo = ambiente (20 �C). */
	/* Presion [pres]: presion del sistema en bar. Depende de temp y de la valvula (valv). */
	/* Valvula [valv]: Verdadero = abierta. Vapor [vapor]: Verdadero si el reactor esta encendido y pres >= 10 bar. */
	/* Potencia [pot]: MW abstractos, misma unidad que la demanda. Depende solo de vTurb (ver PotenciaElectrica). */
	/* Velocidad de la Turbina [vTurb]: RPM abstractas, 0 a 3000. Depende de pres, vapor y valv. */
	/* barras = barras de control INSERTADAS (0 = max. reactividad, 11 = min.). El panel dibuja una X por cada barra insertada. */
	/* Barras de control: valores entre 0 y 11, su porcentaje va a ir hasta 110% pero el jugador ver� solo 100%, cada valor equivale 10% */
	/* Reactividad [React]: indice abstracto, escala aprox. -0.5 a +0.5. 0 = equilibrio (no significa reactor apagado). */
	/* Zenon: reduce la reactividad del reactor, se acumula cuando se genera poca energia. (Reservado para el Unhappy Path: NO se usa en el Happy Path.) */
	/* Opc es la accion que selecciona el jugador */
	/* Demanda es la cantidad de energia a generar para satisfacer la demanda */
	/* Turno es la jornada laboral, ma�ana, tarde y noche, en la que dividimos el d�a */
	/* y en la que el jugador podr� tomar accion */
	/* Dia es el dia en que se da la accion */
	/* Turno a turno la Demanda ya generada, deber�a tener una fluctuac�on menor */
	/* por ejemplo, un peque�o aumento de la demanda en la tarde y un aumento mayor en la noche */
	/* pero en el cambio de dia, deberiamos ver reiniciarse la demanda de energia */
	/* pero el jugador debe afrontar las consecuencias de la jornada anterior */
	/* HAPPY PATH: la demanda se mantiene constante los 3 turnos de un dia y cambia al comenzar el dia siguiente (ver crearDemanda). */
	/* Las acciones de control (barras, agua, valvula) se aplican UNA vez, al terminar el turno. */
	quefinal = 0;
	reactorp = false;
	porcentajeagua = 50;
	reactorSimulado.agua = convertiragua(porcentajeagua);
	/* el juego comienza con el reactor APAGADO */
	turno = 0;
	dia = 1;
	/* el juego comienza en el primer dia */	
	vturb = 0;
	potagen = 0;
	/* Potencia que generaria al finalizar el turno, no se utiliza para ninguna aritmetica/logica */
	turnosaceptables = 0;
	turnosexcedidos = 0;
	turnosdeficit = 0;
	demanda = 120;
	/* demanda inicial (MW). Las siguientes salen de crearDemanda(demanda anterior) */
	valv = false;
	vapor = false;
	skalaactiva = true;
	skalapendiente = false;
	incidentetemperaturaactivo = false;
	incidentesskala = 0;
	diasSinAccidentes = 0;
	eventoseguridadendia = false;
	huboalertaskala = false;
	
	/* Fin de la inicializacion de las variables. */
	do {
		opc = 0;
		while (opc!=7) {
			/* Orden de pantalla: panel de barras, monitor, estado del reactor, notificaciones SKALA y menu. */
			mostrarmonitor(turno, dia, diasSinAccidentes, cantDiasExc, cantDiasDef, demanda);
			mostrarpanel(reactorSimulado.barras);
			/* Consulta del estado: solo muestra, NO modifica ninguna variable. */
			mostrarestado(reactorp, porcentajeagua, reactorSimulado.agua, reactorSimulado.react, reactorSimulado.temp, reactorSimulado.pres, vapor, valv, vturb, potagen, reactorSimulado.pot, demanda);
			/* SKALA informa al comienzo del turno siguiente al calculo del estado. Solo observa y advierte. */
			if (skalapendiente==true) {
				if (skalaactiva==true) {
					huboalertaskala = mostraralertasskala(reactorp, reactorSimulado.temp, reactorSimulado.pres, reactorSimulado.react, porcentajeagua);
					if (huboalertaskala==true) {
						diasSinAccidentes = 0;
						eventoseguridadendia = true;
					}
					if (reactorSimulado.temp>=700) {
						if (incidentetemperaturaactivo==false) {
							incidentesskala = incidentesskala+1;
							incidentetemperaturaactivo = true;
							printf(" SKALA: INCIDENTE TERMICO REGISTRADO. Total de incidentes termicos: %d\n", incidentesskala);
							diasSinAccidentes = 0;
							eventoseguridadendia = true;
						}
					} else {
						incidentetemperaturaactivo = false;
					}
				} else {
					/* Aunque SKALA este desactivada, un incidente termico sigue contando como accidente. */
					if (reactorSimulado.temp>=700) {
						if (incidentetemperaturaactivo==false) {
							incidentesskala = incidentesskala+1;
							incidentetemperaturaactivo = true;
							printf(" INCIDENTE TERMICO REGISTRADO. Total de incidentes termicos: %d\n", incidentesskala);
							diasSinAccidentes = 0;
							eventoseguridadendia = true;
						}
					} else {
						incidentetemperaturaactivo = false;
					}
				}
				skalapendiente = false;
			}
			mostarmenuopc(reactorp, skalaactiva);
			scanf("%d", &opc);
			switch (opc) {
			case 1:
				/* Iniciar el reactor: solo disponible mientras esta apagado. No calcula nada: el estado */
				/* cambia al terminar el turno (el reactor no genera electricidad de inmediato). */
				if (reactorp==false) {
					printf(" Iniciando reactor\n");
					printf(" Reactor encendido. Configure agua, barras y valvula, y luego termine el turno.\n");
					printf(" ...Presione Enter para continuar...\n");
					printf(" \n");
					reactorp = true;
					reactorSimulado.react = reactividad(reactorSimulado.agua, reactorSimulado.barras);
				} else {
					printf(" Reactor ya iniciado. La opcion de encendido no esta disponible.\n");
					printf(" ...Presione Enter para continuar...\n");
					printf(" \n");
				}
				getchar(); /* a diferencia del pseudoc�digo, espera un Enter, no cualquier tecla */
				system("cls");
				break;
			case 2:
				printf(" Ingrese la cantidad de barras de control (0 a 11).\n");
				barrassolicitadas = obtnrbarrasdecontrol();
				/* Aviso previo: retirar todas las barras. SKALA no bloquea ni revierte la accion. */
				if (skalaactiva==true && barrassolicitadas==0 && reactorSimulado.barras>0) {
					printf(" *** SKALA - ADVERTENCIA PREVENTIVA ***\n");
					printf(" Esta a punto de retirar todas las barras de control.\n");
					printf(" La reactividad puede aumentar bruscamente. Inserte barras para limitarla.\n");
					printf(" Continue bajo su propio riesgo.\n");
					diasSinAccidentes = 0;
					eventoseguridadendia = true;
				}
				reactorSimulado.barras = barrassolicitadas;
				if (reactorp==true) {
					reactorSimulado.react = reactividad(reactorSimulado.agua, reactorSimulado.barras);
				}
				printf(" ...Presione Enter para continuar...\n");
				printf(" \n");
				getchar(); /* a diferencia del pseudoc�digo, espera un Enter, no cualquier tecla */
				system("cls");
				break;
			case 3:
				if (valv==false) {
					printf(" Abriendo Valvulas\n");
					printf(" ...Presione Enter para continuar...\n");
					printf(" \n");
					printf(" Decrece la cantidad de Vapor\n");
					printf(" \n");
					getchar(); /* a diferencia del pseudoc�digo, espera un Enter, no cualquier tecla */
					system("cls");
					valv = true;
					/* La potencia se recalcula al terminar el turno, con el estado actualizado. */
				} else {
					printf(" Cerrando Valvulas\n");
					printf(" ...Presione Enter para continuar...\n");
					printf(" \n");
					printf(" Decrece la cantidad de Vapor\n");
					printf(" \n");
					getchar(); /* a diferencia del pseudoc�digo, espera un Enter, no cualquier tecla */
					system("cls");
					valv = false;
					/* La potencia se recalcula al terminar el turno, con el estado actualizado. */
				}
				break;
			case 4:
				aguasolicitada = obtenerporcentajeagua(porcentajeagua);
				/* Aviso previo: reducir la circulacion por debajo del 30 %. No impide continuar. */
				if (skalaactiva==true && aguasolicitada<30 && aguasolicitada<porcentajeagua) {
					printf(" *** SKALA - ADVERTENCIA PREVENTIVA ***\n");
					printf(" Esta reduciendo la circulacion de agua por debajo del 30 %%.\n");
					printf(" La refrigeracion puede resultar insuficiente. Mantenga agua por encima del 40 %% si la temperatura es elevada.\n");
					printf(" Continue bajo su propio riesgo.\n");
					diasSinAccidentes = 0;
					eventoseguridadendia = true;
				}
				porcentajeagua = aguasolicitada;
				/* Agua y reactividad dependen entre si en el modelo del juego. */
				reactorSimulado.agua = convertiragua(porcentajeagua);
				if (reactorp==true) {
					reactorSimulado.react = reactividad(reactorSimulado.agua, reactorSimulado.barras);
				}
				printf(" ...Presione Enter para continuar...\n");
				printf(" \n");
				getchar(); /* a diferencia del pseudoc�digo, espera un Enter, no cualquier tecla */
				system("cls");
				break;
			case 5:
				printf(" Terminar turno\n");
				printf(" ...Presione Enter para continuar...\n");
				printf(" \n");
				/* Orden operativo: (1) actualizar estado y potencia una sola vez, (2) evaluar este turno, */
				/* (3) avanzar turno/dia, (4) si empieza un dia nuevo, evaluar el dia terminado y crear demanda nueva. */
				/* (5) Checkea si excede o tiene deficit de energia */
				/* Actualizar el estado una sola vez por turno. Todas las funciones devuelven */
				/* un valor; no se pasan variables por referencia. */
				reactorSimulado.agua = convertiragua(porcentajeagua);
				if (reactorp==true) {
					reactorSimulado.temp = temperatura(reactorSimulado.react, reactorSimulado.agua, reactorSimulado.temp);
				} else {
					reactorSimulado.react = 0;
					reactorSimulado.temp = temperaturaapagado(reactorSimulado.temp);
				}
				reactorSimulado.pres = presionactualizada(reactorSimulado.temp, reactorSimulado.pres, valv);
				vapor = hayvapor(reactorp, reactorSimulado.pres);
				vturb = velocidadturbina(reactorSimulado.pres, vapor, valv);
				potagen = potenciaelectrica(vturb);
				/* pot representa la potencia ACTUAL del turno, no una suma acumulada. */
				reactorSimulado.pot = potagen;
				printf(" Resultado del turno:\n");
				comparargeneraciondemanda(reactorSimulado.pot, demanda);
				/* Las alertas se mostraran una sola vez al entrar al turno siguiente. */
				skalapendiente = true;
				/* Evaluacion de este turno con tolerancia de +/- 30 MW. */
				if (reactorSimulado.pot>=demanda-30 && reactorSimulado.pot<=demanda+30) {
					turnosaceptables = turnosaceptables+1;
				} else {
					if (reactorSimulado.pot>demanda+30) {
						turnosexcedidos = turnosexcedidos+1;
					} else {
						turnosdeficit = turnosdeficit+1;
					}
				}
				turno = pasarturnos(turno);
				dia = pasardia(turno, dia);
					if (turno==0) {
					/* Un dia sin accidentes solo cuenta si no hubo incidentes ni alertas de SKALA. */
					if (eventoseguridadendia==false) {
						diasSinAccidentes = diasSinAccidentes+1;
					}
					eventoseguridadendia = false;
					/* Un dia cumple si al menos 2 de sus 3 turnos quedaron dentro de +/- 30 MW. */
					if (turnosaceptables>=2) {
						cantDiasExc = 0;
						cantDiasDef = 0;
						printf(" Balance diario aceptable: %d de 3 turnos dentro de la tolerancia.\n", turnosaceptables);
					} else {
						if (turnosexcedidos>turnosdeficit) {
							cantDiasExc = cantDiasExc+1;
							cantDiasDef = 0;
						} else {
							cantDiasDef = cantDiasDef+1;
							cantDiasExc = 0;
						}
					}
					demanda = creardemanda(demanda);
					printf(" Comienza el dia %d. Nueva demanda: %d MW.\n", dia, demanda);
					/* La potencia actual se conserva al cambiar el dia; no se acumula ni se reinicia. */
					turnosaceptables = 0;
					turnosexcedidos = 0;
					turnosdeficit = 0;
				}
				getchar(); /* a diferencia del pseudoc�digo, espera un Enter, no cualquier tecla */
				system("cls");
				opc = 7;
				/* sale del bucle interno; el externo continua porque queFinal sigue en Verdadero */
				quefinal = checkeocatastrofes(reactorSimulado.temp, reactorSimulado.pres);
				if (quefinal==0) {
					quefinal = checkeodiasfinales(cantDiasDef, cantDiasExc);
				}
				break;
			case 6:
				if (reactorp==true) {
					printf(" Apagando Reactor\n");
					reactorp = false;
					/* Reactor apagado: sin vapor, turbina detenida y generacion cero. La temperatura y la presion */
					/* bajan en los proximos turnos (ActualizarEstado). */
					vapor = false;
					vturb = 0;
					potagen = 0;
					reactorSimulado.pot = 0;
				} else {
					printf(" El reactor ya esta apagado.\n");
				}
				printf(" ...Presione Enter para continuar...\n");
				printf(" \n");
				getchar(); /* a diferencia del pseudoc�digo, espera un Enter, no cualquier tecla */
				system("cls");
				break;
			case 7:
				printf(" Saliendo del juego\n");
				printf(" ...Presione Enter para continuar...\n");
				printf(" \n");
				getchar(); /* a diferencia del pseudoc�digo, espera un Enter, no cualquier tecla */
				system("cls");
				quefinal = 1;
				printf("Termino el juego\n");
				break;
			case 8:
				if (skalaactiva==true) {
					skalaactiva = false;
					skalapendiente = false;
					printf(" SKALA DESACTIVADA. No se mostraran advertencias.\n");
				} else {
					skalaactiva = true;
					skalapendiente = false;
					printf(" SKALA ACTIVADA. Evaluara el estado al finalizar el proximo turno.\n");
				}
				printf(" ...Presione Enter para continuar...\n");
				getchar(); /* a diferencia del pseudoc�digo, espera un Enter, no cualquier tecla */
				system("cls");
				break;
			default:
				system("cls"); /* no hay forma directa de borrar la pantalla con C estandar */
				printf("Ingrese una opcion valida\n");
				printf("\n");
			}
		}
	} while (quefinal==0);
	finales(quefinal);
	if (quefinal>=4) {
		printf("\n");
		printf("Dias completos sobrevividos: %d\n", dia-1);
		printf("Presione una tecla para continuar...\n");
		getchar(); /* a diferencia del pseudoc�digo, espera un Enter, no cualquier tecla */
		system("cls");
	/* no hay forma directa de borrar la pantalla con C estandar */
		while (true) {
			printf(" \n");
		}
	}
	return 0;
}

void mostarmenuopc(bool encendido, bool skalaencendida) {
	printf("\n");
	printf("Ingrese una opcion\n");
	/* La opcion de encendido desaparece cuando el reactor ya esta encendido. */
	if (encendido==false) {
		printf("1... Iniciar reactor\n");
	}
	printf("2... Panel de barras de control\n");
	printf("3... Abrir/Cerrar valvula de vapor\n");
	printf("4... Circulacion de agua (porcentaje)\n");
	printf("5... Terminar turno\n");
	printf("6... Apagar reactor\n");
	printf("7... Salir del juego\n");
	if (skalaencendida==true) {
		printf("8... Desactivar SKALA (actualmente encendida)\n");
	} else {
		printf("8... Activar SKALA (actualmente apagada)\n");
	}
}

int obtnrbarrasdecontrol() {
	int d;
	do {
		scanf("%d", &d);
		if (d<0 || d>11) {
			printf(" Valor fuera de rango. Ingrese un valor entre 0 y 11.\n");
		}
	} while (!(d>=0 && d<=11));
	return d;
}

void mostrarmonitor(int t, int d, int inc, int exc, int def, int dem) {
	printf("+---------------------+ La demanda generada por la ciudad es: %d MW.\n", dem);
	printf("| Dias Sin Incidentes | El dia es: %d", d);
	if (d<10) {
		/* Este 'si' esta para saber cuanto espacio hay que tener entre que dia es y cuantos dias seguidos de deficit/exceso hay que tener */
		if (exc>=1) {
			printf("                                       Dias seguidos con exceso de energia: %d/3\n", exc);
		} else {
			printf("                                       Dias seguidos con deficit de energia: %d/3\n", def);
		}
	} else {
		if (exc>=1) {
			printf("                                      Dias con exceso de energia: %d/3\n", exc);
		} else {
			printf("                                      Dias con deficit de energia: %d/3\n", def);
		}
	}
	if (inc<10) {
		switch (t) {
		case 0:
			printf("|         %d           | Turno Matutino.\n", inc);
			break;
		case 1:
			printf("|         %d           | Turno Vespertino.\n", inc);
			break;
		case 2:
			printf("|         %d           | Turno Nocturno.\n", inc);
			break;
		}
	} else {
		switch (t) {
		case 0:
			printf("|         %d          | Turno Matutino.\n", inc);
			break;
		case 1:
			printf("|         %d          | Turno Vespertino.\n", inc);
			break;
		case 2:
			printf("|         %d          | Turno Nocturno.\n", inc);
			break;
		}
	}
	printf("+---------------------+ \n");
	printf("\n");
}

int pasarturnos(int turnoactual) {
	int nuevoturno;
	nuevoturno = turnoactual+1;
	if (nuevoturno>=3) {
		nuevoturno = 0;
	}
	return nuevoturno;
}

int pasardia(int turnoactual, int diaactual) {
	int nuevodia;
	nuevodia = diaactual;
	if (turnoactual==0) {
		nuevodia = diaactual+1;
	}
	return nuevodia;
}

void mostrarpanel(int cantbarrasinsertadas) {
	int cont;
	int i;
	int j;
	int k;
	printf("                              Barras de control insertadas (X = insertada, vacia = retirada):\n");
	cont = 0;
	for (i=1; i<=3; ++i) {
		for (k=1; k<=3; ++k) {
			printf("                                    ");
			/* Este 'Escribir' esta para centrar las barras */
			for (j=1; j<=5; ++j) {
				/* El panel tiene 11 posiciones activas; las cuatro esquinas quedan vacias. */
				if (i==2 || (j!=1 && j!=5)) {
					if (cont<cantbarrasinsertadas) {
						switch (k) {
						case 1:
							printf("  /***\\  ");
							break;
						case 2:
							printf(" (  X  ) ");
							cont = cont+1;
							break;
						case 3:
							printf("  \\___/  ");
							break;
						}
					} else {
						switch (k) {
						case 1:
							printf("  /***\\  ");
							break;
						case 2:
							printf(" (     ) ");
							cont = cont+1;
							break;
						case 3:
							printf("  \\___/  ");
							break;
						}
					}
				} else {
					printf("         ");
				}
			}
			printf("\n");
		}
	}
	printf("\n");
}

int obtenerporcentajeagua(int actual) {
	int nuevo;
	do {
		printf(" Porcentaje actual: %d %%. Ingrese el nuevo porcentaje (1 a 100):\n", actual);
		scanf("%d", &nuevo);
		if (nuevo<1 || nuevo>100) {
			printf(" Valor fuera de rango. Debe estar entre 1 y 100.\n");
		}
	} while (!(nuevo>=1 && nuevo<=100));
	if (nuevo>actual) {
		printf(" La circulacion de agua SUBIO de %d %% a %d %%.\n", actual, nuevo);
	} else {
		if (nuevo<actual) {
			printf(" La circulacion de agua BAJO de %d %% a %d %%.\n", actual, nuevo);
		} else {
			printf(" La circulacion de agua no cambio (%d %%).\n", nuevo);
		}
	}
	return nuevo;
}

/* =========================================================================== */
/* MODELO (formulas abstractas de juego; NO representan la fisica real de un reactor) */
/* =========================================================================== */
/* Agua. Caudal de referencia al 100 % = 10,4 m3/s; turno = 8 h = 28.800 s; unidad = 1 millon de litros. */
/* 10,4 m3/s x 28.800 s = 299.520 m3 = 299,52 millones de litros. Relacion proporcional al porcentaje. */
float convertiragua(float pctagua) {
	float vol;
	vol = 10.4*28800/1000*pctagua/100;
	return vol;
}

/* Reactividad (indice abstracto, escala aprox. -0.5 a +0.5). */
/* - Barras: 0 insertadas = +0.45 ; 11 insertadas = -0.45 antes del ajuste por agua. */
/* - Agua como moderador: menos agua = menos moderacion = MAS reactividad (hasta +0.05); */
/* mas agua = mas moderacion = MENOS reactividad (hasta -0.05). El termino vale 0 al 50 %. */
float reactividad(float aguav, float barrasctl) {
	float fraccionagua;
	float react;
	fraccionagua = aguav/299.52;
	react = (0.45-0.9*barrasctl/11)+0.1*(0.5-fraccionagua);
	return react;
}

/* Escenario de reactividad (unico lugar donde se definen los rangos): */
/* 2 positiva alta (> 0.25) | 1 positiva (0.05 a 0.25) | 0 equilibrio (-0.05 a +0.05, banda de tolerancia) */
/* -1 negativa (-0.25 a -0.05) | -2 negativa alta (< -0.25) */
/* La banda de +-0.05 existe porque con barras enteras el paso minimo es 0.0909 y el cero exacto casi nunca ocurre. */
int escenarioreactividad(float reac) {
	int e;
	if (reac>0.25) {
		e = 2;
	} else {
		if (reac>0.05) {
			e = 1;
		} else {
			if (reac>=-0.05) {
				e = 0;
			} else {
				if (reac>=-0.25) {
					e = -1;
				} else {
					e = -2;
				}
			}
		}
	}
	return e;
}

void nombreescenario(int reac) {
	int esc;
	esc = escenarioreactividad(reac);
	if (esc==2) {
	printf("positiva alta\n");
	} else {
		if (esc==1) {
			printf("positiva\n");
		} else {
			if (esc==0) {
				printf("equilibrio\n");
			} else {
				if (esc==-1) {
					printf("negativa\n");
				} else {
					printf("negativa alta\n");
				}
			}
		}
	}
}

/* Cambio de temperatura (�C por turno) segun el escenario. Es CONTINUA en los bordes de cada escenario. */
/* Ganancia = 200 �C por unidad de reactividad en exceso de la banda. */
/* Calentamiento (reactividad positiva): se multiplica por fCal = 1.25 - 0.75*f  (f = agua/299.52). */
/* Mas agua = mas refrigeracion = menos calentamiento neto (f=1 -> x0.5), pero siempre sube. */
/* Enfriamiento (reactividad negativa): se multiplica por fRef = 0.5 + f.  Mas agua = enfria mas rapido. */
/* Escenarios "alta": pendiente doble sobre 0.25 (la reactividad elevada se nota aunque haya mucha agua). */
/* Equilibrio: la temperatura se mantiene. */
float variaciontermica(float reac, float aguav) {
	float dt;
	int esc;
	float f;
	float fcal;
	float fref;
	f = aguav/299.52;
	fcal = 1.25-0.75*f;
	fref = 0.5+f;
	esc = escenarioreactividad(reac);
	if (esc==2) {
		dt = 200*(0.2+2*(reac-0.25))*fcal;
	} else {
		if (esc==1) {
			dt = 200*(reac-0.05)*fcal;
		} else {
			if (esc==0) {
				dt = 0;
			} else {
				if (esc==-1) {
					dt = 200*(reac+0.05)*fref;
				} else {
					dt = 200*(-0.2+2*(reac+0.25))*fref;
				}
			}
		}
	}
	return dt;
}

/* Temperatura con el reactor ENCENDIDO. Piso = ambiente (20 �C): el agua no puede enfriar por debajo del ambiente. */
/* Techo 850 �C: permite detectar el final catastrofico termico acordado para LOCA. */
float temperatura(float reac, float aguav, float tempactual) {
	float nuevatemp;
	nuevatemp = tempactual+variaciontermica(reac, aguav);
	if (nuevatemp<20) {
		nuevatemp = 20;
	}
	if (nuevatemp>850) {
		nuevatemp = 850;
	}
	return nuevatemp;
}

/* Temperatura con el reactor APAGADO: se enfria 25 % por turno de la diferencia con el ambiente (tiende a 20 �C, nunca la cruza). */
float temperaturaapagado(float tempactual) {
	float nuevatemp;
	nuevatemp = tempactual-0.25*(tempactual-20);
	return nuevatemp;
}

/* Presion (bar). Objetivo segun la temperatura: 1 bar hasta 100 �C y +0.35 bar por cada �C sobre 100. */
/* Cada turno la presion recorre 50 % de la distancia al objetivo (sube y baja gradualmente, sigue al enfriamiento). */
/* Valvula abierta: sale vapor y se pierde 20 % de la sobrepresion (pres-1). Nunca baja de 1 bar. */
float presionactualizada(float tempactual, float presactual, bool valvulaabierta) {
	float nuevapresion;
	float presobj;
	if (tempactual>100) {
		presobj = 1+(tempactual-100)*0.35;
	} else {
		presobj = 1;
	}
	nuevapresion = presactual+0.5*(presobj-presactual);
	if (valvulaabierta==true) {
		nuevapresion = nuevapresion-0.2*(nuevapresion-1);
	}
	if (nuevapresion<1) {
		nuevapresion = 1;
	}
	return nuevapresion;
}

/* Disponibilidad de vapor: derivada. Hay vapor si el reactor esta encendido y pres >= 10 bar. */
bool hayvapor(bool encendido, float presi) {
	bool hay;
	if (encendido==true && presi>=10) {
		hay = true;
	} else {
		hay = false;
	}
	return hay;
}

/* Velocidad de turbina (RPM abstractas). 0 si valvula cerrada, sin vapor o pres < 10 bar. */
/* Si no: crece linealmente de 0 (10 bar) a 3000 (40 bar o mas). Sin saltos ante variaciones minimas de presion. */
int velocidadturbina(float presactual, bool hay, bool valvulaabierta) {
	int velocidad;
	velocidad = 0;
	if (hay==true && valvulaabierta==true && presactual>=10) {
		if (presactual>=40) {
			velocidad = 3000;
		} else {
			velocidad = trunc(3000*(presactual-10)/30);
		}
	}
	return velocidad;
}

/* Potencia (MW abstractos, misma unidad que la demanda): proporcional a la velocidad, 250 MW a 3000 RPM. Turbina detenida = 0. */
int potenciaelectrica(float vt) {
	int p;
	p = trunc(250*vt/3000);
	return p;
}

/* Demanda: constante todo el dia. Cada dia nuevo varia entre -15 y +15 MW respecto del dia anterior, limitada a 50-200 MW. */
int creardemanda(float anterior) {
	int chance;
	int demanda;
	chance = (0+rand()%10-0+1);
	demanda = anterior+(0+rand()%30-0+1)-15;
	if (demanda<50) {
		demanda = 50;
	}
	if (demanda>200) {
		demanda = 200;
	}
	if (chance>8) {
		switch (chance) {
		case 9:
			demanda = trunc(demanda*1.2);
			/* Agregar lore de por que aumenta un 20% la demanda */
			break;
		case 10:
			demanda = trunc(demanda*0.8);
			/* Agregar lore de por que se reduce un 20% la demanda */
			break;
		}
	}
	return demanda;
}

/* Solo muestra. No modifica nada. */
void mostrarestado(bool encendido, float pctagua, float aguav, float reac, float tempe, float presi, bool vapr, bool valvula, float velturb, float potenciaagenerar, float potencia, float dem) {
	if (encendido==true) {
		printf(" Reactor: ENCENDIDO                                                       ");
		mostrarreactividad(encendido, reac);
	} else {
		printf(" Reactor: APAGADO                                                         ");
		mostrarreactividad(encendido, reac);
	}
	if ((trunc(tempe)/10)<10) {
		/* Verifica la cantidad de unidades que tiene para saber cuantos espacios hay que tener entre la temperatura y la presion */
		printf(" Temperatura: %.0f grados C                                                 ", round(round((tempe*10))/10));
	} else {
		printf(" Temperatura: %.0f grados C                                                ", round(round((tempe*10))/10));
	}
	printf(" Presion: %.2f bar\n", round((presi*10))/10);
	if (valvula==true) {
		printf(" Valvula de vapor: ABIERTA                                                ");
		if (vapr==true) {
			printf(" Vapor disponible: SI\n");
		} else {
			printf(" Vapor disponible: NO\n");
		}
	} else {
		printf(" Valvula de vapor: CERRADA                                                ");
		if (vapr==true) {
			printf(" Vapor disponible: SI\n");
		} else {
			printf(" Vapor disponible: NO\n");
		}
	}
	if (velturb/100>=10) {
		/* Verifica la cantidad de unidades que tiene para saber cuantos espacios hay que tener entre la velocidad de la turbina y la potencia generada */
		printf(" Velocidad de la turbina: %.0f RPM                                        ", velturb);
	} else {
		if (velturb==0) {
			/* Verifica que la velocidad de la turbina sea 0 */
			printf(" Velocidad de la turbina: %.0f RPM                                           ", velturb);
		} else {
			printf(" Velocidad de la turbina: %.0f RPM                                         ", velturb);
		}
	}
	printf(" Potencia generada al final del turno: %.2f MW\n", potenciaagenerar);
	printf(" Circulacion de agua: %.2f %% (%.2f millones de litros por turno)\n", pctagua, round((aguav*100))/100);
	comparargeneraciondemanda(potencia, dem);
}

/* Compara generacion y demanda: deficit / balance / excedente. */
void comparargeneraciondemanda(float generacion, float dem) {
	if (generacion<dem-30) {
		printf(" DEFICIT: generacion %.2f MW; esta %.2f MW por debajo de la demanda.\n", generacion, dem-generacion);
		printf(" \n");
	} else {
		if (generacion>dem+30) {
			printf(" EXCEDENTE: generacion %.2f MW; esta %.2f MW por encima de la demanda.\n", generacion, generacion-dem);
			printf(" \n");
		} else {
			printf(" BALANCE ACEPTABLE: generacion %.2f MW para una demanda de %.2f MW (tolerancia +/- 30 MW).\n", generacion, dem);
			printf(" \n");
		}
	}
}

void mostrarreactividad(bool encendido, float reac) {
	if (encendido==true) {
		printf(" Reactividad: %.2f ", round((reac*1000))/1000);
		nombreescenario(reac);
	} else {
		printf(" Reactividad: no aplica (reactor apagado)\n");
	}
}

int checkeodiasfinales(float def, float exc) {
	int terminarjuego;
	terminarjuego = 0;
	if (def>=3) {
		terminarjuego = 2;
	}
	if (exc>=3) {
		terminarjuego = 3;
	}
	return terminarjuego;
}

void finales(int tipofin) {
	switch (tipofin) {
	case 1:
		/* Final si elegis la opcion 7 */
		printf("\n");
		printf("Usted decide abandonar su puesto, dejando todo atras, escabulliendose en el bosque, por miedo a lo que haran sus supervisores al enterarse.\n");
		printf("\n");
		printf("\n");
		printf("                                                                                   %%:                                                                           \n");
		printf("                                                                                  -@+                                                                           \n");
		printf("                                                                                 -%%@@*                                                                          \n");
		printf("                                                                                  +@@*                                                                          \n");
		printf("                                                                                *%%@@@*-                                                                         \n");
		printf("                                                                                :-@@@%%:                                                                         \n");
		printf("                                                                               =#@@@@@##+                                                                       \n");
		printf("                                                                               :-%%@@@@%%=                                                                        \n");
		printf("                                                            :@:                 =@@@@@@@@*                      *+                   =@:                        \n");
		printf("                                        +#                  *@#              :#@@@@@@@#-                        %%@:                  #@%%                        \n");
		printf("                                       :@@=               :*@@@%%=            =@%%#@@@@@@*=                      =@@#                =%%@@@@:                      \n");
		printf("                                     :@@@@@%%-              -@@@+             -*=+@@@@@@@@+                    -@@@@#                 @@@%%#:                     \n");
		printf("                                       #@@%%                :@@@@*         -@@@@@@@@@@@@*                    :-=#@@@-              :+#@@@%%                       \n");
		printf("                                   +###@@@@+::           =*%%@@@%%             :=@@@@@@@@@@@@=                -%%@@@@@@*:             -*@@@@@#                     \n");
		printf("                   :#               :-+@@@@@@#:          :+@@@@@@@%%         :*@@@@@@@@@@*-         :*      =+-+@@@@*             ***#@@@@+                      \n");
		printf("                   +@#           :+=: *@@@@:             :+@@@@@%%         :#@@@@@@@@@@@@@:         %%@+     -#@@@@@@@@@@:          -+@@@@@@@@#                   \n");
		printf("                   @@@:           +@@@@@@@@@@@@@:       *@@@@@@@@#=:      +@@@@%%@@@@@@@@@@@%%:     =@@%%     -+@@@@@@%%              -#@@@@@@-                     \n");
		printf("                 -*@@@#+=          :=@@@@@@@*+            -@@@@@@%%=        :: *@@@@@@@@@@@@%%:   =*@@@@#++ *@@@@@@@@#           :@@@@@@@@@@#+++-                 \n");
		printf("                 +%%@@@@%%:       +@@@@@@@@@@@%%+==:       +@@@@@@@@:       -==+@@@@@@@@@@@#--++    %%@@@@@%%-  :--%%@@@@@@@@@*          :@@@@@@@@@*      *           \n");
		printf("                  *@@@@=            -@@@@@@@@@@@@:     :#@@@@@@@@@@@*   +@@@@@@@@@@@@@@@@@@#-  =#@@@@@@+    -@@@@@@@@@@+       =+*@@@@@@@@@#:      +@#          \n");
		printf("              -+#@@@@@@@@@@:     :%%@@@@@@@@@@%%=:     #@@@@@@@@@@@@        -*#@@@@@@@@@@@@@@@@%%::=@@@@@@*+-  =@@@@@@@#=         :+***@@@@%%*%%*:    :+%%@@+:        \n");
		printf("              :#@%%@@@@@@#=     -@@@@@@@@@@@@@@+*@@%%:   :  #@@@@@@@@:     *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%=:-@@@@@@@@@@@@@@@=     :=#@@@@@@@@+::    =@@@=         \n");
		printf("            :--==#@@@@@@@%%#-     +%%@@@@@@@@@@@@@@@- :+@@@@@@@@@@@@@@@@* =****@@@@@@@@@@@@@#++#@@@@@@@@@+:%%@@@@@@@@@@@@@@#-    *@@@@@@@@@@@@@@@@+:+@@@@@%%        \n");
		printf("            =@@@@@@@@@@@@*-   :--=*@@@@@@@@@@@@@%%   :=###*%%@@@@@@@@@@#:  -+@@@@@@@@@@@@@@@%%:  -%%@@@@@@@@##@@@@@@@@@@@@@=       :--+@@@@@@@@@@#:  :@@@@%%         \n");
		printf("       :      -#@@@@@@@@@+--  -@@@@@@@@@@@@@@@*+:     +@=%%@@@@@@@@@=:   %%@@@@@@@@@@@@@@@@@@@#-%%@@@@@@@@@@@@@@@@@@@@@@@@%%*%%#: %%@@@@@@@@@@@@@@@@#-#@@@@@@*        \n");
		printf("      -@+    :--=@@@@@@@@@@-     :@@@@@@@@@@@@@*-:  =@@@@@@@@@@@@@@@@@@%%%%###@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@:  +@@@@@@@@@@@@@@@@@@@@@@@@@@:      \n");
		printf("      #@@-  *@@@@@@@@@@@@:    #@@%%@@@@@@@@@@@@@@@%%@=  =@@@@@@@@@@@@@@@=  -%%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@-  %%@#%%@@@@@@@@@@@@@%%:%%@@@@@@@@@+      \n");
		printf("    =@@@@@@:  #@@@@@@@@@@@#*  -@@@@@@@@@@@@@@@@@@@@%%:#@@@@@@@@@@@@@@%% :@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%@@@@@@@@@@@@@@@@@@%%@@@@@@@@@+      \n");
		printf("     -@@@@+: +@@@@@@@@@@@@@*    =@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@     \n");
		printf("    %%@@@@@#:@@@@@@@@@@@@@@@@%%#@@@@@@@@@@@@@@@@@@@%%*=%%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*:=@@@@@@@@@@@@@@@@@@@@@@@@@@@%%      \n");
		printf("    +@@@@@@=+@@@@@@@@@@@@@@@#%%@@@@@@@@@@@@@@@@@@@@##@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%     \n");
		printf(" :@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@+   \n");
		printf("    =@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@:  \n");
		printf(" :@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#    \n");
		printf("-*%%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
		printf(":@%%%%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@* \n");
		printf(":#@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@+ \n");
		break;
	case 2:
		/* Final si llevas 3 dias seguidos por terminando el dia por debajo de la demanda */
		printf("\n");
		printf("Debido a su inoperancia ha sido arrestado por el Comit� de Recursos y Energia del Estado.\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("    ::      :::::::::                                                                :::::::::----: \n");
		printf("               :::::-:                                                              ::::::::::----  \n");
		printf("               ::::::::                                                            :::::::::::---:  \n");
		printf("                 :::::::                                                           :  ::::::::::::  \n");
		printf("                    ::::::                                                         ::::::::::----:  \n");
		printf("                       ::::                                              :-=#+--%%#*++**##%%%%%%%%*---:  \n");
		printf("                       :::::                                       :-::--==+======++++++++=---*#%%*: \n");
		printf("                         :::::                     ::::::    :-::---==--==++--::::::::::---=+**=--+:\n");
		printf("              ::          ::::::       :::---- : :--:  --::=-++-=+========-::::::::::::::---==+=---:\n");
		printf("                 :       : ::-=:::-=++-=--=+=+*#=-==-:-=-:         -===--::::::::: :::::-----==-:-::\n");
		printf("                ::::::  :+#*=-==-:-+#*:::::::::                        :::::::::::::::::-------     \n");
		printf("                ::-=+++-:::=+++=-:::-=-:::-:                          :::::::::::::::::-------:     \n");
		printf("                =%%%%+-:-=-:::::::::::::--:                            :::::::::::::::::--------      \n");
		printf("               ==-::-::        :::::::::--:                          :::::::::::::::::-------:      \n");
		printf("               :   ::::::       ::::::::::--                        ::::::::::::::::::-------       \n");
		printf("                      : :      : ::::::::::--:                     ::::::::::::::::::-------        \n");
		printf("                        ::::   ::::::::: ::::-:                   :: ::::::::-::::::----=--:        \n");
		printf("                       ::::::::::::::::::::::---                  ::::::::--+-::::::----=--:        \n");
		printf("                       :::::: ::::::::::::::::---                :::::----=*=::::::----==--         \n");
		printf("                       :::::::::::::::::::::::::--               :::::---++-:::::::----==-          \n");
		printf("                        ::::::::::::::--::::::::-=-             :::::--=+:::::::::---==--:          \n");
		printf("                         :::::::::::::-+-:::::---==:           :::::--==:  :-:::::--====-           \n");
		printf("                           ::::::::::--**-:::-----::          ::::---=+:   ::::::::-====-           \n");
		printf("                           ::::::::::-=++++=-----:::-:       ::-----++-    ::::::::-====-           \n");
		printf("                          ::::::::::--=++    -:::---==-     :::---==+=   :::::::---=====-           \n");
		printf("                          ::::::::::-=+=      :::----==-   ::-:----++::-=*-:::::--=+===-            \n");
		printf("                          :::::::::::--=       :-:----=+=--:::::--=+=--=*#=::::---=++=:             \n");
		printf("                           ::::::::::--=-       :----==+***-:::---++====+=:::::--=+*+-              \n");
		printf("                           ::::::::::---=-      :---===+*#*-:::-==--+**#+:::::--=+**=:              \n");
		printf("                            :::::::::::-:-=+*+===+=---=+=*##+=+++=-==*##-::::--==**+:               \n");
		printf("                              ::::::::::-=+#***#%%%%#=--=--=+##%%%%#*=--====-:::--==+*+:                \n");
		printf("                                ::::::::---==#%%%%%%%%#+===---=#%%##*=-::---------=+**+-                 \n");
		printf("                                  :::::::::--=****+======+**###+-:----:----==++**-                  \n");
		printf("                                    :::::::---+======+======-------------====**+=                   \n");
		printf("                                      :::::---=-=-----=-==--:::-------==+*+=--:                     \n");
		printf("                                         ::----=======-==--------==++++=::                          \n");
		printf("                                          :---------::        :::::                                 \n");
		break;
	case 3:
		/* Final si llevas 3 dias seguidos por terminando el dia por encima de la demanda */
		printf("\n");
		printf("Debido a su ineficiencia con los recursos previstos ha sido arrestado por el Comit� de Recursos y Energia del Estado.\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("    ::      :::::::::                                                                :::::::::----: \n");
		printf("               :::::-:                                                              ::::::::::----  \n");
		printf("               ::::::::                                                            :::::::::::---:  \n");
		printf("                 :::::::                                                           :  ::::::::::::  \n");
		printf("                    ::::::                                                         ::::::::::----:  \n");
		printf("                       ::::                                              :-=#+--%%#*++**##%%%%%%%%*---:  \n");
		printf("                       :::::                                       :-::--==+======++++++++=---*#%%*: \n");
		printf("                         :::::                     ::::::    :-::---==--==++--::::::::::---=+**=--+:\n");
		printf("              ::          ::::::       :::---- : :--:  --::=-++-=+========-::::::::::::::---==+=---:\n");
		printf("                 :       : ::-=:::-=++-=--=+=+*#=-==-:-=-:         -===--::::::::: :::::-----==-:-::\n");
		printf("                ::::::  :+#*=-==-:-+#*:::::::::                        :::::::::::::::::-------     \n");
		printf("                ::-=+++-:::=+++=-:::-=-:::-:                          :::::::::::::::::-------:     \n");
		printf("                =%%%%+-:-=-:::::::::::::--:                            :::::::::::::::::--------      \n");
		printf("               ==-::-::        :::::::::--:                          :::::::::::::::::-------:      \n");
		printf("               :   ::::::       ::::::::::--                        ::::::::::::::::::-------       \n");
		printf("                      : :      : ::::::::::--:                     ::::::::::::::::::-------        \n");
		printf("                        ::::   ::::::::: ::::-:                   :: ::::::::-::::::----=--:        \n");
		printf("                       ::::::::::::::::::::::---                  ::::::::--+-::::::----=--:        \n");
		printf("                       :::::: ::::::::::::::::---                :::::----=*=::::::----==--         \n");
		printf("                       :::::::::::::::::::::::::--               :::::---++-:::::::----==-          \n");
		printf("                        ::::::::::::::--::::::::-=-             :::::--=+:::::::::---==--:          \n");
		printf("                         :::::::::::::-+-:::::---==:           :::::--==:  :-:::::--====-           \n");
		printf("                           ::::::::::--**-:::-----::          ::::---=+:   ::::::::-====-           \n");
		printf("                           ::::::::::-=++++=-----:::-:       ::-----++-    ::::::::-====-           \n");
		printf("                          ::::::::::--=++    -:::---==-     :::---==+=   :::::::---=====-           \n");
		printf("                          ::::::::::-=+=      :::----==-   ::-:----++::-=*-:::::--=+===-            \n");
		printf("                          :::::::::::--=       :-:----=+=--:::::--=+=--=*#=::::---=++=:             \n");
		printf("                           ::::::::::--=-       :----==+***-:::---++====+=:::::--=+*+-              \n");
		printf("                           ::::::::::---=-      :---===+*#*-:::-==--+**#+:::::--=+**=:              \n");
		printf("                            :::::::::::-:-=+*+===+=---=+=*##+=+++=-==*##-::::--==**+:               \n");
		printf("                              ::::::::::-=+#***#%%%%#=--=--=+##%%%%#*=--====-:::--==+*+:                \n");
		printf("                                ::::::::---==#%%%%%%%%#+===---=#%%##*=-::---------=+**+-                 \n");
		printf("                                  :::::::::--=****+======+**###+-:----:----==++**-                  \n");
		printf("                                    :::::::---+======+======-------------====**+=                   \n");
		printf("                                      :::::---=-=-----=-==--:::-------==+*+=--:                     \n");
		printf("                                         ::----=======-==--------==++++=::                          \n");
		printf("                                          :---------::        :::::                                 \n");
		printf("\n");
		break;
	case 4:
		/* Final termico: flash boil y destruccion total de la central. */
		printf("\n");
		printf("===============================================\n");
		printf(" Flash Boil\n");
		printf("===============================================\n");
		printf("\n");
		printf("La temperatura alcanzo el punto critico.\n");
		printf("El hervor fue tan repentino que, el agua del sistema se evaporo de forma instantanea.\n");
		printf("La expansion destruyo la central y provoco una explosion");
		printf("devastadora que arraso el emplazamiento y la ciudad.\n");
		printf("No se detectan sistemas operativos ni comunicaciones.\n");
		printf("\n");
				printf("                                   ..:=*+--=====-:.                                         \n");
		printf("                               .+@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*                          \n");
		printf("                            :#@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@-                      \n");
		printf("                          :@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@-                    \n");
		printf("                     -@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%                   \n");
		printf("                 .%%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*#@@-+@@@@-:*-               \n");
		printf("               -@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*+*+:#@@@@=:++:            \n");
		printf("              =@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@+-:------:+@=:  +@:        \n");
		printf("           .#@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%%%%%@@@@@@@@@@@@@@@@*+=-----:+--.==%%-      \n");
		printf("         .@@@@@@@%%%%%%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%+**@@%%@@@@@@@@@@@@-===:::+@#+   #%%     \n");
		printf("       . -@@@@%%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%@@@@@@@@@#*+*#@@@@@@@@@@@@# ------:.    -@+    \n");
		printf("     ....=@%%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*=++===+*+=+*+**++=-*@@#:--    ..=#.    \n");
		printf("   ....:@@@@@@@@@@@@@@@@@@@%%*##%%@@%%%%%%@@@@@@@@@@@@@@@@#=++=-%%@@@@@@+:::-+#=:%%@@#-.      .+@- \n");
		printf(" ...: #@@@@@@@@@@@@@@@@@@#@@%%@#%%@%%=:=#+-#@@@@@@@@@@@@@#=-+%%@=-++--+%%@#:=*=     +@+      .=%%:\n");
		printf(" ....:@@@@@@@@@@@@@@@@@@@%%@*--:-:... :*#*#**#+%%@@@@*     .:..:=+++-=#+@+.:=+#+  ...=*:. :@# \n");
		printf("  ... %%@@@%%@@@@@@@@@@@*#%%#*++-=#@@@#+%%+*+-++-.. :+=       :-:::-+**-*. -* ....#:    .. -%%*  \n");
		printf("  ...::+@@@@@*+****#@#@@@@@%%#%%@%%###**++==+***-   ++           ....:...+:.:.. .  :+-::-:+*+  \n");
		printf("  ....::::@@++*#%%#+*+#@@%%@@#%%@*+#*=-:-::::::.   --                  ..:.--        ...  ==   \n");
		printf("    ...:::-@@@@@++++*%%%%@@#=-+%%+-+#-:    ...   .     .+%%+*+--#+-.:::       .        ::::.    \n");
		printf("      ....::::::-=-:+=--:.:++:      . .  ..:%%*++*@@@@@@%%%%+-                                 \n");
		printf("         ...................            .  ::%%@@@@%%#@@@=.                                   \n");
		printf("           ......:::::::::::::::::::::::::-==*@#@@@@@@@*:::::::::::..........               \n");
		printf("       ...::::----===#*++@@@@@@@@@@@@@%%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#-=+%%*----:::::.... \n");
		printf("      ....::-+=-+++***%%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#@@@##***-....\n");
		printf("          ..........:::::::-------===++=====+**#@@@@@@%%========-+*=-----:::::::........     \n");
		printf("                               :::.  ....:...--=@@@@@@#:. ..       -=.                      \n");
		printf("                            =+++:           ::-*@@@@@@%%.             .#.                    \n");
		printf("                        -=*.                :.+%%@@@@@@+                =+                   \n");
		printf("                    .*=:*                   ..-@@@@@@@#                 =*                  \n");
		printf("     ....::::--:  +*  ==                    ..%%%%*@@@@@%%.                -*     ..           \n");
		printf(" ...::-==*%%%%#***@%%-==#*=+*#%%*-. .:.......::. -@=#@@@@@#.              :+%%@@#*+=+=--::.      \n");
		printf("..:=+=+#@%%###*%%@+===*@%%**+=-*@%%%%+==-....==-:.:@@@@@@@@@:.:...:........-*@@@@@%%%%%%%%%%#**+-..=#*\n");
		printf(":::::=*#*%%@@#@@%%%%#*+##*#@@@@@**+=:-%%@#=.....%%#*@@@@@@@@@@#=---+%%*-=*+--+@@@@@@@%%#**++=-:-=:-\n");
		printf("  -#**##%%#*+@%%+*+###@@@@@@%%@@@@@@@@@=@@@-..%%@@@@@@@@@@@@@@@@@@@@@#*@%%#**#%%#**#%%@@@@%%%%#@*    \n");
		printf("        %%%%-@%%:--=-=#%%@@@@@@@##*%%@@@@@@@@*=@@@@@@@@@@@@@@@@@@@@@*%%%%*****+-:+##%%#*@+          \n");
		printf("        %%@-@-:::..::-++*#%%%%%%#*@*:..*@@@@@@@@@@@@@@@@@@@@@@@@@%%@@%%*=:.-=+==+++++-%%+          \n");
		printf("        #*....:.........:::::#=:-=: .: :@@@@@@-+= .-+#@@@@@@@@%%*=+===--:........*=          \n");
		printf("        +*...............   ++ .   :::..-===+#%%.+@%%++**##%%%%%%%%*++-=:  ... .:.....*-          \n");
		printf("        +=......     .     +%%           :---:-=*=:.  ..:::--:::.     ....       =-          \n");
		printf("        =-                 +#                :::.          .      .             -:          \n");
		printf("        +-          :      =*  .                       ..            -          -:          \n");
		printf("        .-=-.       : ..         :         .   ..   .             .. =        :-=.          \n");
		printf("\n");

		printf("\n");
		break;
	case 5:
		/* Final por sobrepresion: ruptura del reactor. */
		printf("\n");
		printf("===============================================\n");
		printf(" RUPTURA DE LA TAPA DEL REACTOR\n");
		printf("===============================================\n");
		printf("\n");
		printf("La presion supero el punto de no retorno.\n");
		printf("El agua que circulaba dentro del reactor se transformo violentamente en vapor.\n");
		printf("La explosion desprendio la tapa ");
		printf("del reactor y dejo el nucleo expuesto a la atmosfera.\n");
		printf("Material radiactivo y restos del nucleo fueron liberados ");
		printf("a la atmosfera y llevados por el viento a las ciudades y paises vecinos.\n");
		break;
	case 6:
		/* Final combinado: ruptura del reactor y del sistema de refrigeracion. */
		printf("\n");
		printf("===============================================\n");
		printf(" COLAPSO COMBINADO\n");
		printf("===============================================\n");
		printf("\n");
		printf("Tanto la temperatura como la presion entraron simultaneamente en zona critica.\n");
		printf("La estructura del reactor y el sistema de refrigeracion sufrieron rupturas graves.\n");
		printf("El agua circulante quedo contaminada.\n");
		printf("La evaporacion libero contaminantes radiactivos al ambiente; ");
		printf("nubes toxicas se desplazaron sobre la ciudad y sus alrededores, mientras que los ");
		printf("da�os estructurales dispersaron restos y material contaminado.\n");
		printf("La central quedo inutilizada y la region gravemente contaminada.\n");
		printf("\n");
		printf("\n");
		break;
	default:
		printf("Final fuera de rango\n");
	}
}

/* SKALA: solo observa variables y emite recomendaciones. Nunca modifica el estado del reactor. */
/* Presion: umbrales de juego provisionales, basados en el rango operativo previsto de 40-120 bar. */
/* El final termico se activa a 850 grados C y se evalua al terminar cada turno. */
bool mostraralertasskala(bool encendido, float temp, float presion, float react, float pctagua) {
	bool huboalerta;
	huboalerta = false;
	/* Temperatura y presion se supervisan aunque el reactor este apagado: pueden seguir elevadas. */
	if (temp>=800) {
		printf(" *** SKALA - ALERTA CRITICA DE TEMPERATURA ***\n");
		printf(" Temperatura: %.0f grados C. Zona critica alcanzada.\n", round(temp));
		printf(" Inserte barras de control y aumente la circulacion de agua.\n");
		printf(" El apagado del reactor es el ultimo recurso.\n");
		huboalerta = true;
	} else {
		if (temp>=700) {
			printf(" *** SKALA - ALERTA DE INCIDENTE TERMICO ***\n");
			printf(" Temperatura: %.0f grados C. Se registra un incidente termico.\n", round(temp));
			printf(" Inserte barras de control y aumente la circulacion de agua.\n");
			printf(" El apagado del reactor es el ultimo recurso.\n");
			huboalerta = true;
		} else {
			if (temp>=600) {
				printf(" *** SKALA - ADVERTENCIA PREVENTIVA DE TEMPERATURA ***\n");
				printf(" Temperatura: %.0f grados C (rango preventivo: 600-699).\n", round(temp));
				printf(" Revise la reactividad, inserte barras si es elevada y aumente el agua.\n");
				huboalerta = true;
			}
		}
	}
	if (presion>=120) {
		printf(" *** SKALA - ALERTA CRITICA DE PRESION ***\n");
		printf(" Presion: %.2f bar. Umbral critico del simulador alcanzado.\n", round((presion*10))/10);
		printf(" Abra la valvula para aliviar presion si corresponde; el apagado es el ultimo recurso.\n");
		huboalerta = true;
	} else {
		if (presion>100) {
			printf(" *** SKALA - ADVERTENCIA PREVENTIVA DE PRESION ***\n");
			printf(" Presion: %.2f bar. Revise la valvula de vapor.\n", round((presion*10))/10);
			huboalerta = true;
		}
	}
	if (encendido==true && react>0.33) {
		printf(" *** SKALA - ADVERTENCIA DE REACTIVIDAD ELEVADA ***\n");
		printf(" Reactividad: %.2f (umbral: > 0,25). Inserte barras de control.\n", round((react*1000))/1000);
		huboalerta = true;
	}
	if (pctagua<40 && temp>600) {
		printf(" *** SKALA - ALERTA DE REFRIGERACION INSUFICIENTE ***\n");
		printf(" Agua: %.2f %% con temperatura superior a 600 grados C.\n", pctagua);
		printf(" Aumente la circulacion de agua inmediatamente.\n");
		printf(" El apagado del reactor es el ultimo recurso.\n");
		huboalerta = true;
	}
	if (huboalerta==false) {
		printf(" SKALA: sin alertas en las variables supervisadas.\n");
	}
	return huboalerta;
}

/* Detecta el final catastrofico segun temperatura y presion. No muestra mensajes ni modifica el reactor. */
/* Codigos: 0 = sin catastrofe; 4 = termica; 5 = sobrepresion; 6 = combinada. */
int checkeocatastrofes(float tempactual, float presactual) {
	int catastrofe;
	catastrofe = 0;
	if (tempactual>=850) {
		catastrofe = 4;
	} else {
		if (presactual>125) {
			catastrofe = 5;
		} else {
			if (tempactual>=650 && tempactual<=799 && presactual>=100 && presactual<=119) {
				catastrofe = 6;
			}
		}
	}
	return catastrofe;
}

