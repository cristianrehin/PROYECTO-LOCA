Proceso proyectLoca01
	Definir reactorP, vapor, valv Como Logico // queFinal es el logico que controla que el juego se repita turno a turno
	Definir opc, demanda, turno, dia, contIncidentes, incidentes, cantEnAct, potAGen Como Entero
	Definir turnosAceptables, turnosExcedidos, turnosDeficit Como Entero
	Definir porcentajeAgua, pot, vTurb, barras, cantDiasExc, cantDiasDef, queFinal Como Entero
	Definir barrasSolicitadas, aguaSolicitada, incidentesSKALA Como Entero
	Definir diasSinAccidentes Como Entero
	Definir skalaActiva, skalaPendiente, incidenteTemperaturaActivo Como Logico
	Definir eventoSeguridadEnDia, huboAlertaSKALA Como Logico
	definir agua, temp, React, xe, pres Como Real
	// Variables:
	// Agua [agua]: volumen equivalente de agua circulado durante UN turno, en millones de litros (real). 100 % = 299,52 (10,4 m3/s x 28.800 s).
	// porcentajeAgua: % de circulacion elegido por el jugador (1 a 100).		[porcentajeAgua]
	// 'agua' NO es lo mismo que 'porcentajeAgua': agua se obtiene solo con ConvertirAgua(porcentajeAgua).
	// Temperatura [temp]: temperatura del reactor en °C. Minimo = ambiente (20 °C).
	// Presion [pres]: presion del sistema en bar. Depende de temp y de la valvula (valv).
	// Valvula [valv]: Verdadero = abierta. Vapor [vapor]: Verdadero si el reactor esta encendido y pres >= 10 bar.
	// Potencia [pot]: MW abstractos, misma unidad que la demanda. Depende solo de vTurb (ver PotenciaElectrica).
	// Velocidad de la Turbina [vTurb]: RPM abstractas, 0 a 3000. Depende de pres, vapor y valv.
	
	// barras = barras de control INSERTADAS (0 = max. reactividad, 11 = min.). El panel dibuja una X por cada barra insertada.
	// Barras de control: valores entre 0 y 11, su porcentaje va a ir hasta 110% pero el jugador verá solo 100%, cada valor equivale 10%
	// Reactividad [React]: indice abstracto, escala aprox. -0.5 a +0.5. 0 = equilibrio (no significa reactor apagado).
	// Zenon: reduce la reactividad del reactor, se acumula cuando se genera poca energia. (Reservado para el Unhappy Path: NO se usa en el Happy Path.)
	
	// Opc es la accion que selecciona el jugador
	// Demanda es la cantidad de energia a generar para satisfacer la demanda
	// Turno es la jornada laboral, mañana, tarde y noche, en la que dividimos el dìa
	// y en la que el jugador podrà tomar accion
	// Dia es el dia en que se da la accion
	
	// Turno a turno la Demanda ya generada, deberìa tener una fluctuacìon menor
	// por ejemplo, un pequeño aumento de la demanda en la tarde y un aumento mayor en la noche
	// pero en el cambio de dia, deberiamos ver reiniciarse la demanda de energia
	// pero el jugador debe afrontar las consecuencias de la jornada anterior
	
	// HAPPY PATH: la demanda se mantiene constante los 3 turnos de un dia y cambia al comenzar el dia siguiente (ver crearDemanda).
	// Las acciones de control (barras, agua, valvula) se aplican UNA vez, al terminar el turno.
	
	queFinal <- 0
	reactorP <- Falso			// el juego comienza con el reactor APAGADO
	turno <- 0
	dia <- 1				// el juego comienza en el primer dia
	incidentes <- 0
	temp <- 20				// temperatura ambiente
	pres <- 1				// presion atmosferica (bar)
	barras <- 11				// arranque seguro: barras totalmente insertadas
	porcentajeAgua <- 50
	agua <- ConvertirAgua(porcentajeAgua)
	contIncidentes <- 0
	React <- 0
	pot <- 0
	vTurb <- 0
	cantDiasExc = 0 //Cantidad de dias seguidos que pasan excediendo el limite de energia
	cantDiasDef = 0 //Cantidad de dias seguidos que pasan con deficit en la generacion de energia
	potAGen = 0 //Potencia que generaria al finalizar el turno, no se utiliza para ninguna aritmetica/logica
	turnosAceptables <- 0
	turnosExcedidos <- 0
	turnosDeficit <- 0
	demanda <- 120				// demanda inicial (MW). Las siguientes salen de crearDemanda(demanda anterior)
	valv <- Falso
	vapor <- Falso
	skalaActiva <- Verdadero
	skalaPendiente <- Falso
	incidenteTemperaturaActivo <- Falso
	incidentesSKALA <- 0
	diasSinAccidentes <- 0
	eventoSeguridadEnDia <- Falso
	huboAlertaSKALA <- Falso
	xe <- 0					// reservado (Unhappy Path), no se usa en el Happy Path
	//Fin de la inicializacion de las variables.
	
	Repetir
		opc <- 0		
		Mientras opc <> 7 Hacer
			// Orden de pantalla: panel de barras, monitor, estado del reactor, notificaciones SKALA y menu.
			MostrarMonitor(turno, dia, diasSinAccidentes, cantDiasExc, cantDiasDef, demanda)
			MostrarPanel(barras)
			// Consulta del estado: solo muestra, NO modifica ninguna variable.
			MostrarEstado(reactorP, porcentajeAgua, agua, React, temp, pres, vapor, valv, vTurb, potAGen, pot, demanda)
			// SKALA informa al comienzo del turno siguiente al calculo del estado. Solo observa y advierte.
			Si skalaPendiente = Verdadero Entonces
				Si skalaActiva = Verdadero Entonces
					huboAlertaSKALA <- MostrarAlertasSKALA(reactorP, temp, pres, react, porcentajeAgua)
					Si huboAlertaSKALA = Verdadero Entonces
						diasSinAccidentes <- 0
						eventoSeguridadEnDia <- Verdadero
					FinSi
					Si temp >= 700 Entonces
						Si incidenteTemperaturaActivo = Falso Entonces
							incidentesSKALA <- incidentesSKALA + 1
							incidenteTemperaturaActivo <- Verdadero
							Escribir " SKALA: INCIDENTE TERMICO REGISTRADO. Total de incidentes termicos: ", incidentesSKALA
							diasSinAccidentes <- 0
							eventoSeguridadEnDia <- Verdadero
						FinSi
					SiNo
						incidenteTemperaturaActivo <- Falso
					FinSi
				SiNo
					// Aunque SKALA este desactivada, un incidente termico sigue contando como accidente.
					Si temp >= 700 Entonces
						Si incidenteTemperaturaActivo = Falso Entonces
							incidentesSKALA <- incidentesSKALA + 1
							incidenteTemperaturaActivo <- Verdadero
							Escribir " INCIDENTE TERMICO REGISTRADO. Total de incidentes termicos: ", incidentesSKALA
							diasSinAccidentes <- 0
							eventoSeguridadEnDia <- Verdadero
						FinSi
					SiNo
						incidenteTemperaturaActivo <- Falso
					FinSi
				FinSi
				skalaPendiente <- Falso
			FinSi
			mostarMenuOpc(reactorP, skalaActiva)
			leer opc
			segun opc Hacer
				1:
					// Iniciar el reactor: solo disponible mientras esta apagado. No calcula nada: el estado
					// cambia al terminar el turno (el reactor no genera electricidad de inmediato).
					Si reactorP = Falso Entonces
						Escribir " Iniciando reactor"
						Escribir " Reactor encendido. Configure agua, barras y valvula, y luego termine el turno."
						Escribir " ...Presione Enter para continuar..."
						Escribir " ";
						reactorP <- Verdadero
						React <- Reactividad(agua, barras)
					SiNo
						Escribir " Reactor ya iniciado. La opcion de encendido no esta disponible."
						Escribir " ...Presione Enter para continuar..."
						Escribir " ";
					Fin Si
					Esperar Tecla
					Limpiar Pantalla
				2:
					Escribir " Ingrese la cantidad de barras de control (0 a 11)."
					barrasSolicitadas <- ObtnrBarrasDeControl()
					// Aviso previo: retirar todas las barras. SKALA no bloquea ni revierte la accion.
					Si skalaActiva = Verdadero y barrasSolicitadas = 0 y barras > 0 Entonces
						Escribir " *** SKALA - ADVERTENCIA PREVENTIVA ***"
						Escribir " Esta a punto de retirar todas las barras de control."
						Escribir " La reactividad puede aumentar bruscamente. Inserte barras para limitarla."
						Escribir " Continue bajo su propio riesgo."
						diasSinAccidentes <- 0
						eventoSeguridadEnDia <- Verdadero
					FinSi
					barras <- barrasSolicitadas
					Si reactorP = Verdadero Entonces
						React <- Reactividad(agua, barras)
					FinSi
					Escribir " ...Presione Enter para continuar..."
					Escribir " ";
					Esperar Tecla
					Limpiar Pantalla
				3:
					Si valv = Falso Entonces
						Escribir " Abriendo Valvulas"
						Escribir " ...Presione Enter para continuar..."
						Escribir " ";
						Escribir " Decrece la cantidad de Vapor"
						Escribir " "
						Esperar Tecla
						Limpiar Pantalla
						valv <- Verdadero
						// La potencia se recalcula al terminar el turno, con el estado actualizado.
					SiNo
						Escribir " Cerrando Valvulas"
						Escribir " ...Presione Enter para continuar..."
						Escribir " ";
						Escribir " Decrece la cantidad de Vapor"
						Escribir " "
						Esperar Tecla
						Limpiar Pantalla
						valv <- Falso
						// La potencia se recalcula al terminar el turno, con el estado actualizado.
					Fin Si
				4:
					aguaSolicitada <- ObtenerPorcentajeAgua(porcentajeAgua)
					// Aviso previo: reducir la circulacion por debajo del 30 %. No impide continuar.
					Si skalaActiva = Verdadero y aguaSolicitada < 30 y aguaSolicitada < porcentajeAgua Entonces
						Escribir " *** SKALA - ADVERTENCIA PREVENTIVA ***"
						Escribir " Esta reduciendo la circulacion de agua por debajo del 30 %."
						Escribir " La refrigeracion puede resultar insuficiente. Mantenga agua por encima del 40 % si la temperatura es elevada."
						Escribir " Continue bajo su propio riesgo."
						diasSinAccidentes <- 0
						eventoSeguridadEnDia <- Verdadero
					FinSi
					porcentajeAgua <- aguaSolicitada
					// Agua y reactividad dependen entre si en el modelo del juego.
					agua <- ConvertirAgua(porcentajeAgua)
					Si reactorP = Verdadero Entonces
						React <- Reactividad(agua, barras)
					FinSi
					Escribir " ...Presione Enter para continuar..."
					Escribir " ";
					Esperar Tecla
					Limpiar Pantalla
				5:
					Escribir " Terminar turno"
					Escribir " ...Presione Enter para continuar..."
					Escribir " ";
					// Orden operativo: (1) actualizar estado y potencia una sola vez, (2) evaluar este turno,
					// (3) avanzar turno/dia, (4) si empieza un dia nuevo, evaluar el dia terminado y crear demanda nueva.
					// (5) Checkea si excede o tiene deficit de energia
					// Actualizar el estado una sola vez por turno. Todas las funciones devuelven
					// un valor; no se pasan variables por referencia.
					agua <- ConvertirAgua(porcentajeAgua)
					Si reactorP = Verdadero Entonces
						temp <- Temperatura(React, agua, temp)
					SiNo
						React <- 0
						temp <- TemperaturaApagado(temp)
					FinSi
					pres <- PresionActualizada(temp, pres, valv)
					vapor <- HayVapor(reactorP, pres)
					vTurb <- VelocidadTurbina(pres, vapor, valv)
					potAGen <- PotenciaElectrica(vTurb)
					// pot representa la potencia ACTUAL del turno, no una suma acumulada.
					pot <- potAGen
					Escribir " Resultado del turno:"
					CompararGeneracionDemanda(pot, demanda)
					// Las alertas se mostraran una sola vez al entrar al turno siguiente.
					skalaPendiente <- Verdadero
					// Evaluacion de este turno con tolerancia de +/- 30 MW.
					Si pot >= demanda - 30 y pot <= demanda + 30 Entonces
						turnosAceptables <- turnosAceptables + 1
					SiNo
						Si pot > demanda + 30 Entonces
							turnosExcedidos <- turnosExcedidos + 1
						SiNo
							turnosDeficit <- turnosDeficit + 1
						FinSi
					FinSi
					turno <- pasarTurnos(turno)
					dia <- pasarDia(turno, dia)
					contIncidentes <- contIncidentes+1
					incidentes <- trunc((contIncidentes)/3)
					Si turno = 0 Entonces
						// Un dia sin accidentes solo cuenta si no hubo incidentes ni alertas de SKALA.
						Si eventoSeguridadEnDia = Falso Entonces
							diasSinAccidentes <- diasSinAccidentes + 1
						FinSi
						eventoSeguridadEnDia <- Falso
						// Un dia cumple si al menos 2 de sus 3 turnos quedaron dentro de +/- 30 MW.
						Si turnosAceptables >= 2 Entonces
							cantDiasExc <- 0
							cantDiasDef <- 0
							Escribir " Balance diario aceptable: ", turnosAceptables, " de 3 turnos dentro de la tolerancia."
						SiNo
							Si turnosExcedidos > turnosDeficit Entonces
								cantDiasExc <- cantDiasExc + 1
								cantDiasDef <- 0
							SiNo
								cantDiasDef <- cantDiasDef + 1
								cantDiasExc <- 0
							FinSi
						FinSi
						demanda <- crearDemanda(demanda)
						Escribir " Comienza el dia ", dia, ". Nueva demanda: ", demanda, " MW."
						// La potencia actual se conserva al cambiar el dia; no se acumula ni se reinicia.
						turnosAceptables <- 0
						turnosExcedidos <- 0
						turnosDeficit <- 0
					FinSi
					Esperar Tecla
					Limpiar Pantalla
					opc <- 7		// sale del bucle interno; el externo continua porque queFinal sigue en Verdadero
					queFinal <- CheckeoCatastrofes(temp,pres)
					Si queFinal = 0 Entonces
						queFinal <- CheckeoDiasFinales(cantDiasDef,cantDiasExc)
					FinSi
				6:
					Si reactorP = Verdadero Entonces
						Escribir " Apagando Reactor"
						reactorP <- Falso
						// Reactor apagado: sin vapor, turbina detenida y generacion cero. La temperatura y la presion
						// bajan en los proximos turnos (ActualizarEstado).
						vapor <- Falso
						vTurb <- 0
						potAGen <- 0
						pot <- 0
					SiNo
						Escribir " El reactor ya esta apagado."
					FinSi
					Escribir " ...Presione Enter para continuar..."
					Escribir " ";
					Esperar Tecla
					Limpiar Pantalla
				7:
					Escribir " Saliendo del juego"
					Escribir " ...Presione Enter para continuar..."
					Escribir " ";
					Esperar Tecla
					Limpiar Pantalla
					queFinal <- 1
					Escribir "Termino el juego"
				8:
					Si skalaActiva = Verdadero Entonces
						skalaActiva <- Falso
						skalaPendiente <- Falso
						Escribir " SKALA DESACTIVADA. No se mostraran advertencias."
					SiNo
						skalaActiva <- Verdadero
						skalaPendiente <- Falso
						Escribir " SKALA ACTIVADA. Evaluara el estado al finalizar el proximo turno."
					FinSi
					Escribir " ...Presione Enter para continuar..."
					Esperar Tecla
					Limpiar Pantalla
				De Otro Modo:
					Limpiar Pantalla
					Escribir "Ingrese una opcion valida"
					Escribir ""
			FinSegun			
		FinMientras		
	Hasta Que queFinal <> 0
	Finales(queFinal)
	Si queFinal >= 4 Entonces
		Escribir ""
		Escribir "Dias completos sobrevividos: ", dia - 1
		Escribir "Presione una tecla para continuar..."
		Esperar Tecla
		Limpiar Pantalla
		Mientras Verdadero Hacer
			Escribir " "
		FinMientras
	FinSi
FinProceso


SubProceso mostarMenuOpc(encendido, skalaEncendida)
	Escribir ""
	Escribir "Ingrese una opcion"
	// La opcion de encendido desaparece cuando el reactor ya esta encendido.
	Si encendido = Falso Entonces
		Escribir "1... Iniciar reactor"
	FinSi
	Escribir "2... Panel de barras de control"
	Escribir "3... Abrir/Cerrar valvula de vapor"
	Escribir "4... Circulacion de agua (porcentaje)"
	Escribir "5... Terminar turno"
	Escribir "6... Apagar reactor"
	Escribir "7... Salir del juego"
	Si skalaEncendida = Verdadero Entonces
		Escribir "8... Desactivar SKALA (actualmente encendida)"
	SiNo
		Escribir "8... Activar SKALA (actualmente apagada)"
	FinSi
FinSubProceso

Funcion d <- ObtnrBarrasDeControl
	Definir d Como Entero
	Repetir
		Leer d
		Si d < 0 o d > 11 Entonces
			Escribir " Valor fuera de rango. Ingrese un valor entre 0 y 11."
		FinSi
	Hasta Que d >= 0 y d <= 11
FinFuncion

SubProceso MostrarMonitor(t,d,inc,exc,def,dem)
	Escribir "| ------------------- | La demanda generada por la ciudad es: " dem " MW."
	Escribir "| Dias Sin Incidentes | El dia es: " d Sin Saltar
	
	Si d < 10 Entonces //Este 'si' esta para saber cuanto espacio hay que tener entre que dia es y cuantos dias seguidos de deficit/exceso hay que tener
		Si exc >= 1 Entonces
			Escribir "                                       Dias seguidos con exceso de energia: ", exc, "/3"
		SiNo
			Escribir "                                       Dias seguidos con deficit de energia: ", def, "/3"
		FinSi
	SiNo
		Si exc >= 1 Entonces
			Escribir "                                      Dias con exceso de energia: ", exc, "/3"
		SiNo
			Escribir "                                      Dias con deficit de energia: ", def, "/3"
		FinSi
	FinSi
	Si inc < 10 Entonces
		Segun t Hacer
			0:
				Escribir "|         ",inc,"           | Turno Mañana."
			1:
				Escribir "|         ",inc,"           | Turno Tarde."
			2:
				Escribir "|         ",inc,"           | Turno Noche."
		Fin Segun
	SiNo
		Segun t Hacer
			0:
				Escribir "|         ",inc,"          | Turno Mañana."
			1:
				Escribir "|         ",inc,"          | Turno Tarde."
			2:
				Escribir "|         ",inc,"          | Turno Noche."
		Fin Segun
	FinSi
	Escribir "| ------------------- | "
	Escribir ""
FinSubProceso

Funcion nuevoTurno <- pasarTurnos(turnoActual)
	Definir nuevoTurno Como Entero
	nuevoTurno <- turnoActual + 1
	Si nuevoTurno >= 3 Entonces
		nuevoTurno <- 0
	FinSi
FinFuncion

Funcion nuevoDia <- pasarDia(turnoActual, diaActual)
	Definir nuevoDia Como Entero
	nuevoDia <- diaActual
	Si turnoActual = 0 Entonces
		nuevoDia <- diaActual + 1
	FinSi
FinFuncion

SubProceso MostrarPanel(cantBarrasInsertadas)
	Escribir "                              Barras de control insertadas (X = insertada, vacia = retirada):"
	Definir cont,i,j,k Como Entero
	cont <- 0
	Para i=1 Hasta 3 Con Paso 1 Hacer
		Para k=1 Hasta 3 Con Paso 1 Hacer
			Escribir "                                    " Sin Saltar // Este 'Escribir' esta para centrar las barras 
			Para j=1 Hasta 5 Con Paso 1 Hacer
				// El panel tiene 11 posiciones activas; las cuatro esquinas quedan vacias.
				Si i = 2 o (j <> 1 y j <> 5) Entonces
					Si cont < cantBarrasInsertadas Entonces
						Segun k Hacer
							1:
								Escribir "  /***\  " Sin Saltar
							2:
								Escribir " (  X  ) " Sin Saltar
								cont <- cont + 1
							3:
								Escribir "  \___/  " Sin Saltar
						FinSegun
					SiNo
					Segun k Hacer
							1:
								Escribir "  /***\  " Sin Saltar
							2:
								Escribir " (     ) " Sin Saltar
								cont <- cont + 1
							3:
								Escribir "  \___/  " Sin Saltar
					FinSegun
				FinSi
				SiNo
					Escribir "         " Sin Saltar
				FinSi
			FinPara
			Escribir ""
		FinPara
	FinPara
	Escribir ""
FinSubProceso

Funcion nuevo <- ObtenerPorcentajeAgua(actual)
	Definir nuevo Como Entero
	Repetir
		Escribir " Porcentaje actual: ", actual, " %. Ingrese el nuevo porcentaje (1 a 100):"
		Leer nuevo
		Si nuevo < 1 o nuevo > 100 Entonces
			Escribir " Valor fuera de rango. Debe estar entre 1 y 100."
		FinSi
	Hasta Que nuevo >= 1 y nuevo <= 100
	Si nuevo > actual Entonces
		Escribir " La circulacion de agua SUBIO de ", actual, " % a ", nuevo, " %."
	SiNo
		Si nuevo < actual Entonces
			Escribir " La circulacion de agua BAJO de ", actual, " % a ", nuevo, " %."
		SiNo
			Escribir " La circulacion de agua no cambio (", nuevo, " %)."
		FinSi
	FinSi
FinFuncion

// ===========================================================================
// MODELO (formulas abstractas de juego; NO representan la fisica real de un reactor)
// ===========================================================================

// Agua. Caudal de referencia al 100 % = 10,4 m3/s; turno = 8 h = 28.800 s; unidad = 1 millon de litros.
// 10,4 m3/s x 28.800 s = 299.520 m3 = 299,52 millones de litros. Relacion proporcional al porcentaje.
Funcion vol <- ConvertirAgua(pctAgua)
	Definir vol Como Real
	vol <- 10.4*28800/1000*pctAgua/100
FinFuncion

// Reactividad (indice abstracto, escala aprox. -0.5 a +0.5).
//  - Barras: 0 insertadas = +0.45 ; 11 insertadas = -0.45 antes del ajuste por agua.
//  - Agua como moderador: menos agua = menos moderacion = MAS reactividad (hasta +0.05);
//    mas agua = mas moderacion = MENOS reactividad (hasta -0.05). El termino vale 0 al 50 %.
Funcion React <- Reactividad(aguaV, barrasCtl)
	Definir React Como Real
	Definir fraccionAgua Como Real
	fraccionAgua <- aguaV/299.52
	React <- (0.45 - 0.9*barrasCtl/11) + 0.1*(0.5-fraccionAgua)
FinFuncion

// Escenario de reactividad (unico lugar donde se definen los rangos):
//   2 positiva alta (> 0.25) | 1 positiva (0.05 a 0.25) | 0 equilibrio (-0.05 a +0.05, banda de tolerancia)
//  -1 negativa (-0.25 a -0.05) | -2 negativa alta (< -0.25)
// La banda de +-0.05 existe porque con barras enteras el paso minimo es 0.0909 y el cero exacto casi nunca ocurre.
Funcion e <- EscenarioReactividad(reac)
	Definir e Como Entero
	Si reac > 0.25 Entonces
		e <- 2
	SiNo
		Si reac > 0.05 Entonces
			e <- 1
		SiNo
			Si reac >= -0.05 Entonces
				e <- 0
			SiNo
				Si reac >= -0.25 Entonces
					e <- -1
				SiNo
					e <- -2
				FinSi
			FinSi
		FinSi
	FinSi
FinFuncion

Funcion nombre <- NombreEscenario(reac)
	Definir nombre Como Caracter
	Definir esc Como Entero
	esc <- EscenarioReactividad(reac)
	Si esc = 2 Entonces
		nombre <- "positiva alta"
	SiNo
		Si esc = 1 Entonces
			nombre <- "positiva"
		SiNo
			Si esc = 0 Entonces
				nombre <- "equilibrio"
			SiNo
				Si esc = -1 Entonces
					nombre <- "negativa"
				SiNo
					nombre <- "negativa alta"
				FinSi
			FinSi
		FinSi
	FinSi
FinFuncion

// Cambio de temperatura (°C por turno) segun el escenario. Es CONTINUA en los bordes de cada escenario.
//   Ganancia = 200 °C por unidad de reactividad en exceso de la banda.
//   Calentamiento (reactividad positiva): se multiplica por fCal = 1.25 - 0.75*f  (f = agua/299.52).
//     Mas agua = mas refrigeracion = menos calentamiento neto (f=1 -> x0.5), pero siempre sube.
//   Enfriamiento (reactividad negativa): se multiplica por fRef = 0.5 + f.  Mas agua = enfria mas rapido.
//   Escenarios "alta": pendiente doble sobre 0.25 (la reactividad elevada se nota aunque haya mucha agua).
//   Equilibrio: la temperatura se mantiene.
Funcion dT <- VariacionTermica(reac, aguaV)
	Definir dT, f, fCal, fRef Como Real
	Definir esc Como Entero
	f <- aguaV/299.52
	fCal <- 1.25-0.75*f
	fRef <- 0.5+f
	esc <- EscenarioReactividad(reac)
	Si esc = 2 Entonces
		dT <- 200*(0.2+2*(reac-0.25))*fCal
	SiNo
		Si esc = 1 Entonces
			dT <- 200*(reac-0.05)*fCal
		SiNo
			Si esc = 0 Entonces
				dT <- 0
			SiNo
				Si esc = -1 Entonces
					dT <- 200*(reac+0.05)*fRef
				SiNo
					dT <- 200*(-0.2+2*(reac+0.25))*fRef
				FinSi
			FinSi
		FinSi
	FinSi
FinFuncion

// Temperatura con el reactor ENCENDIDO. Piso = ambiente (20 °C): el agua no puede enfriar por debajo del ambiente.
// Techo 850 °C: permite detectar el final catastrofico termico acordado para LOCA.
Funcion nuevaTemp <- Temperatura(reac, aguaV, tempActual)
	Definir nuevaTemp Como Real
	nuevaTemp <- tempActual + VariacionTermica(reac, aguaV)
	Si nuevaTemp < 20 Entonces
		nuevaTemp <- 20
	FinSi
	Si nuevaTemp > 850 Entonces
		nuevaTemp <- 850
	FinSi
FinFuncion

// Temperatura con el reactor APAGADO: se enfria 25 % por turno de la diferencia con el ambiente (tiende a 20 °C, nunca la cruza).
Funcion nuevaTemp <- TemperaturaApagado(tempActual)
	Definir nuevaTemp Como Real
	nuevaTemp <- tempActual - 0.25*(tempActual-20)
FinFuncion

// Presion (bar). Objetivo segun la temperatura: 1 bar hasta 100 °C y +0.35 bar por cada °C sobre 100.
// Cada turno la presion recorre 50 % de la distancia al objetivo (sube y baja gradualmente, sigue al enfriamiento).
// Valvula abierta: sale vapor y se pierde 20 % de la sobrepresion (pres-1). Nunca baja de 1 bar.
Funcion nuevaPresion <- PresionActualizada(tempActual, presActual, valvulaAbierta)
	Definir nuevaPresion Como Real
	Definir presObj Como Real
	Si tempActual > 100 Entonces
		presObj <- 1+(tempActual-100)*0.35
	SiNo
		presObj <- 1
	FinSi
	nuevaPresion <- presActual+0.5*(presObj-presActual)
	Si valvulaAbierta = Verdadero Entonces
		nuevaPresion <- nuevaPresion-0.2*(nuevaPresion-1)
	FinSi
	Si nuevaPresion < 1 Entonces
		nuevaPresion <- 1
	FinSi
FinFuncion

// Disponibilidad de vapor: derivada. Hay vapor si el reactor esta encendido y pres >= 10 bar.
Funcion hay <- HayVapor(encendido, presi)
	Definir hay Como Logico
	Si encendido = Verdadero y presi >= 10 Entonces
		hay <- Verdadero
	SiNo
		hay <- Falso
	FinSi
FinFuncion

// Velocidad de turbina (RPM abstractas). 0 si valvula cerrada, sin vapor o pres < 10 bar.
// Si no: crece linealmente de 0 (10 bar) a 3000 (40 bar o mas). Sin saltos ante variaciones minimas de presion.
Funcion velocidad <- VelocidadTurbina(presActual, hay, valvulaAbierta)
	Definir velocidad Como Entero
	velocidad <- 0
	Si hay = Verdadero y valvulaAbierta = Verdadero y presActual >= 10 Entonces
		Si presActual >= 40 Entonces
			velocidad <- 3000
		SiNo
			velocidad <- trunc(3000*(presActual-10)/30)
		FinSi
	FinSi
FinFuncion

// Potencia (MW abstractos, misma unidad que la demanda): proporcional a la velocidad, 250 MW a 3000 RPM. Turbina detenida = 0.
Funcion p <- PotenciaElectrica(vt)
	Definir p Como Entero
	p <- trunc(250*vt/3000)
FinFuncion

// Demanda: constante todo el dia. Cada dia nuevo varia entre -15 y +15 MW respecto del dia anterior, limitada a 50-200 MW.
funcion demanda <- crearDemanda(anterior)
	Definir demanda, chance Como Entero
	chance <- Aleatorio(0,10)
	demanda <- anterior + aleatorio(0, 30) - 15
	Si demanda < 50 Entonces
		demanda <- 50
	FinSi
	Si demanda > 200 Entonces
		demanda <- 200
	FinSi
	
	Si chance > 8 Entonces
		Segun chance Hacer
			9:
				demanda <- trunc(demanda*1.2) // Agregar lore de por que aumenta un 20% la demanda
			10:
				demanda <- trunc(demanda*0.8) // Agregar lore de por que se reduce un 20% la demanda
		Fin Segun
	FinSi
	
	
FinFuncion

// Solo muestra. No modifica nada.
SubProceso MostrarEstado(encendido, pctAgua, aguaV, reac, tempe, presi, vapr, valvula, velTurb, potenciaAGenerar, potencia, dem)
	Si encendido = Verdadero Entonces
		Escribir " Reactor: ENCENDIDO                                                       " Sin Saltar
		MostrarReactividad(encendido,reac)
	SiNo
		Escribir " Reactor: APAGADO                                                         " Sin Saltar
		MostrarReactividad(encendido,reac)
	FinSi
	
	Si (trunc(tempe) / 10) < 10 Entonces //Verifica la cantidad de unidades que tiene para saber cuantos espacios hay que tener entre la temperatura y la presion
		Escribir " Temperatura: ", redon(redon(tempe*10)/10), " grados C                                                 " Sin Saltar
	SiNo
		Escribir " Temperatura: ", redon(redon(tempe*10)/10), " grados C                                                " Sin Saltar
	FinSi
	Escribir " Presion: ", redon(presi*10)/10, " bar"
	
	Si valvula = Verdadero Entonces
		Escribir " Valvula de vapor: ABIERTA                                                " Sin Saltar
		Si vapr = Verdadero Entonces
			Escribir " Vapor disponible: SI"
		SiNo
			Escribir " Vapor disponible: NO"
		FinSi
	SiNo
		Escribir " Valvula de vapor: CERRADA                                                " Sin Saltar
		Si vapr = Verdadero Entonces
			Escribir " Vapor disponible: SI"
		SiNo
			Escribir " Vapor disponible: NO"
		FinSi
	FinSi
	
	Si velTurb / 100 >= 10 Entonces //Verifica la cantidad de unidades que tiene para saber cuantos espacios hay que tener entre la velocidad de la turbina y la potencia generada
		Escribir " Velocidad de la turbina: ", velTurb, " RPM                                        " Sin Saltar
	SiNo
		Si velTurb = 0 Entonces //Verifica que la velocidad de la turbina sea 0
			Escribir " Velocidad de la turbina: ", velTurb, " RPM                                           " Sin Saltar
		SiNo
			Escribir " Velocidad de la turbina: ", velTurb, " RPM                                         " Sin Saltar			
		FinSi
	FinSi
	
	Escribir " Potencia generada al final del turno: ", potenciaAGenerar, " MW"
	
	Escribir " Circulacion de agua: ", pctAgua, " % (", redon(aguaV*100)/100, " millones de litros por turno)"
	CompararGeneracionDemanda(potencia, dem)
FinSubProceso

// Compara generacion y demanda: deficit / balance / excedente.
SubProceso CompararGeneracionDemanda(generacion, dem)
	Si generacion < dem - 30 Entonces
		Escribir " DEFICIT: generacion ", generacion, " MW; esta ", dem - generacion, " MW por debajo de la demanda."
		Escribir " "
	SiNo
		Si generacion > dem + 30 Entonces
			Escribir " EXCEDENTE: generacion ", generacion, " MW; esta ", generacion - dem, " MW por encima de la demanda."
			Escribir " "
		SiNo
			Escribir " BALANCE ACEPTABLE: generacion ", generacion, " MW para una demanda de ", dem, " MW (tolerancia +/- 30 MW)."
			Escribir " "
		FinSi
	FinSi
FinSubProceso

SubProceso MostrarReactividad(encendido,reac)
	Si encendido = Verdadero Entonces
		Escribir " Reactividad: ", redon(reac*1000)/1000, " (", NombreEscenario(reac), ")"
	SiNo
		Escribir " Reactividad: no aplica (reactor apagado)"
	FinSi
FinSubProceso

Funcion terminarJuego <- CheckeoDiasFinales(Def,Exc)
	Definir terminarJuego Como Entero
	terminarJuego  = 0
	Si Def >= 3 Entonces
		terminarJuego <- 2
	FinSi
	Si Exc >= 3 Entonces
		terminarJuego <- 3
	FinSi
FinFuncion

Funcion Finales(tipoFin)
	Segun tipoFin Hacer
		1: // Final si elegis la opcion 7
			Escribir ""
			Escribir "Usted decide abandonar su puesto, dejando todo atras, escabulliendose en el bosque, por miedo a lo que haran sus supervisores al enterarse."
			Escribir ""
			Escribir ""
			Escribir "                                                                                   %:                                                                           "
			Escribir "                                                                                  -@+                                                                           "
			Escribir "                                                                                 -%@@*                                                                          "
			Escribir "                                                                                  +@@*                                                                          "
			Escribir "                                                                                *%@@@*-                                                                         "
			Escribir "                                                                                :-@@@%:                                                                         "
			Escribir "                                                                               =#@@@@@##+                                                                       "
			Escribir "                                                                               :-%@@@@%=                                                                        "
			Escribir "                                                            :@:                 =@@@@@@@@*                      *+                   =@:                        "
			Escribir "                                        +#                  *@#              :#@@@@@@@#-                        %@:                  #@%                        "
			Escribir "                                       :@@=               :*@@@%=            =@%#@@@@@@*=                      =@@#                =%@@@@:                      "
			Escribir "                                     :@@@@@%-              -@@@+             -*=+@@@@@@@@+                    -@@@@#                 @@@%#:                     "
			Escribir "                                       #@@%                :@@@@*         -@@@@@@@@@@@@*                    :-=#@@@-              :+#@@@%                       "
			Escribir "                                   +###@@@@+::           =*%@@@%             :=@@@@@@@@@@@@=                -%@@@@@@*:             -*@@@@@#                     "
			Escribir "                   :#               :-+@@@@@@#:          :+@@@@@@@%         :*@@@@@@@@@@*-         :*      =+-+@@@@*             ***#@@@@+                      "
			Escribir "                   +@#           :+=: *@@@@:             :+@@@@@%         :#@@@@@@@@@@@@@:         %@+     -#@@@@@@@@@@:          -+@@@@@@@@#                   "
			Escribir "                   @@@:           +@@@@@@@@@@@@@:       *@@@@@@@@#=:      +@@@@%@@@@@@@@@@@%:     =@@%     -+@@@@@@%              -#@@@@@@-                     "
			Escribir "                 -*@@@#+=          :=@@@@@@@*+            -@@@@@@%=        :: *@@@@@@@@@@@@%:   =*@@@@#++ *@@@@@@@@#           :@@@@@@@@@@#+++-                 "
			Escribir "                 +%@@@@%:       +@@@@@@@@@@@%+==:       +@@@@@@@@:       -==+@@@@@@@@@@@#--++    %@@@@@%-  :--%@@@@@@@@@*          :@@@@@@@@@*      *           "
			Escribir "                  *@@@@=            -@@@@@@@@@@@@:     :#@@@@@@@@@@@*   +@@@@@@@@@@@@@@@@@@#-  =#@@@@@@+    -@@@@@@@@@@+       =+*@@@@@@@@@#:      +@#          "
			Escribir "              -+#@@@@@@@@@@:     :%@@@@@@@@@@%=:     #@@@@@@@@@@@@        -*#@@@@@@@@@@@@@@@@%::=@@@@@@*+-  =@@@@@@@#=         :+***@@@@%*%*:    :+%@@+:        "
			Escribir "              :#@%@@@@@@#=     -@@@@@@@@@@@@@@+*@@%:   :  #@@@@@@@@:     *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%=:-@@@@@@@@@@@@@@@=     :=#@@@@@@@@+::    =@@@=         "
			Escribir "            :--==#@@@@@@@%#-     +%@@@@@@@@@@@@@@@- :+@@@@@@@@@@@@@@@@* =****@@@@@@@@@@@@@#++#@@@@@@@@@+:%@@@@@@@@@@@@@@#-    *@@@@@@@@@@@@@@@@+:+@@@@@%        "
			Escribir "            =@@@@@@@@@@@@*-   :--=*@@@@@@@@@@@@@%   :=###*%@@@@@@@@@@#:  -+@@@@@@@@@@@@@@@%:  -%@@@@@@@@##@@@@@@@@@@@@@=       :--+@@@@@@@@@@#:  :@@@@%         "
			Escribir "       :      -#@@@@@@@@@+--  -@@@@@@@@@@@@@@@*+:     +@=%@@@@@@@@@=:   %@@@@@@@@@@@@@@@@@@@#-%@@@@@@@@@@@@@@@@@@@@@@@@%*%#: %@@@@@@@@@@@@@@@@#-#@@@@@@*        "
			Escribir "      -@+    :--=@@@@@@@@@@-     :@@@@@@@@@@@@@*-:  =@@@@@@@@@@@@@@@@@@%%###@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@:  +@@@@@@@@@@@@@@@@@@@@@@@@@@:      "
			Escribir "      #@@-  *@@@@@@@@@@@@:    #@@%@@@@@@@@@@@@@@@%@=  =@@@@@@@@@@@@@@@=  -%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@-  %@#%@@@@@@@@@@@@@%:%@@@@@@@@@+      "
			Escribir "    =@@@@@@:  #@@@@@@@@@@@#*  -@@@@@@@@@@@@@@@@@@@@%:#@@@@@@@@@@@@@@% :@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%@@@@@@@@@@@@@@@@@@%%@@@@@@@@@+      "
			Escribir "     -@@@@+: +@@@@@@@@@@@@@*    =@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@     "
			Escribir "    %@@@@@#:@@@@@@@@@@@@@@@@%#@@@@@@@@@@@@@@@@@@@%*=%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*:=@@@@@@@@@@@@@@@@@@@@@@@@@@@%      "
			Escribir "    +@@@@@@=+@@@@@@@@@@@@@@@#%@@@@@@@@@@@@@@@@@@@@##@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%     "
			Escribir " :@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@+   "
			Escribir "    =@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@:  "
			Escribir " :@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#    "
			Escribir "-*%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
			Escribir ":@%%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@* "
			Escribir ":#@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@+ "
		2: // Final si llevas 3 dias seguidos por terminando el dia por debajo de la demanda
			Escribir ""
			Escribir "Debido a su inoperancia ha sido arrestado por el Comité de Recursos y Energia del Estado."
			Escribir ""
			Escribir ""
			Escribir ""
			Escribir "    ::      :::::::::                                                                :::::::::----: "
			Escribir "               :::::-:                                                              ::::::::::----  "
			Escribir "               ::::::::                                                            :::::::::::---:  "
			Escribir "                 :::::::                                                           :  ::::::::::::  "
			Escribir "                    ::::::                                                         ::::::::::----:  "
			Escribir "                       ::::                                              :-=#+--%#*++**##%%%%*---:  "
			Escribir "                       :::::                                       :-::--==+======++++++++=---*#%*: "
			Escribir "                         :::::                     ::::::    :-::---==--==++--::::::::::---=+**=--+:"
			Escribir "              ::          ::::::       :::---- : :--:  --::=-++-=+========-::::::::::::::---==+=---:"
			Escribir "                 :       : ::-=:::-=++-=--=+=+*#=-==-:-=-:         -===--::::::::: :::::-----==-:-::"
			Escribir "                ::::::  :+#*=-==-:-+#*:::::::::                        :::::::::::::::::-------     "
			Escribir "                ::-=+++-:::=+++=-:::-=-:::-:                          :::::::::::::::::-------:     "
			Escribir "                =%%+-:-=-:::::::::::::--:                            :::::::::::::::::--------      "
			Escribir "               ==-::-::        :::::::::--:                          :::::::::::::::::-------:      "
			Escribir "               :   ::::::       ::::::::::--                        ::::::::::::::::::-------       "
			Escribir "                      : :      : ::::::::::--:                     ::::::::::::::::::-------        "
			Escribir "                        ::::   ::::::::: ::::-:                   :: ::::::::-::::::----=--:        "
			Escribir "                       ::::::::::::::::::::::---                  ::::::::--+-::::::----=--:        "
			Escribir "                       :::::: ::::::::::::::::---                :::::----=*=::::::----==--         "
			Escribir "                       :::::::::::::::::::::::::--               :::::---++-:::::::----==-          "
			Escribir "                        ::::::::::::::--::::::::-=-             :::::--=+:::::::::---==--:          "
			Escribir "                         :::::::::::::-+-:::::---==:           :::::--==:  :-:::::--====-           "
			Escribir "                           ::::::::::--**-:::-----::          ::::---=+:   ::::::::-====-           "
			Escribir "                           ::::::::::-=++++=-----:::-:       ::-----++-    ::::::::-====-           "
			Escribir "                          ::::::::::--=++    -:::---==-     :::---==+=   :::::::---=====-           "
			Escribir "                          ::::::::::-=+=      :::----==-   ::-:----++::-=*-:::::--=+===-            "
			Escribir "                          :::::::::::--=       :-:----=+=--:::::--=+=--=*#=::::---=++=:             "
			Escribir "                           ::::::::::--=-       :----==+***-:::---++====+=:::::--=+*+-              "
			Escribir "                           ::::::::::---=-      :---===+*#*-:::-==--+**#+:::::--=+**=:              "
			Escribir "                            :::::::::::-:-=+*+===+=---=+=*##+=+++=-==*##-::::--==**+:               "
			Escribir "                              ::::::::::-=+#***#%%#=--=--=+##%%#*=--====-:::--==+*+:                "
			Escribir "                                ::::::::---==#%%%%#+===---=#%##*=-::---------=+**+-                 "
			Escribir "                                  :::::::::--=****+======+**###+-:----:----==++**-                  "
			Escribir "                                    :::::::---+======+======-------------====**+=                   "
			Escribir "                                      :::::---=-=-----=-==--:::-------==+*+=--:                     "
			Escribir "                                         ::----=======-==--------==++++=::                          "
			Escribir "                                          :---------::        :::::                                 "
		3: // Final si llevas 3 dias seguidos por terminando el dia por encima de la demanda
			Escribir ""
			Escribir "Debido a su ineficiencia con los recursos previstos ha sido arrestado por el Comité de Recursos y Energia del Estado."
			Escribir ""
			Escribir ""
			Escribir ""
			Escribir "    ::      :::::::::                                                                :::::::::----: "
			Escribir "               :::::-:                                                              ::::::::::----  "
			Escribir "               ::::::::                                                            :::::::::::---:  "
			Escribir "                 :::::::                                                           :  ::::::::::::  "
			Escribir "                    ::::::                                                         ::::::::::----:  "
			Escribir "                       ::::                                              :-=#+--%#*++**##%%%%*---:  "
			Escribir "                       :::::                                       :-::--==+======++++++++=---*#%*: "
			Escribir "                         :::::                     ::::::    :-::---==--==++--::::::::::---=+**=--+:"
			Escribir "              ::          ::::::       :::---- : :--:  --::=-++-=+========-::::::::::::::---==+=---:"
			Escribir "                 :       : ::-=:::-=++-=--=+=+*#=-==-:-=-:         -===--::::::::: :::::-----==-:-::"
			Escribir "                ::::::  :+#*=-==-:-+#*:::::::::                        :::::::::::::::::-------     "
			Escribir "                ::-=+++-:::=+++=-:::-=-:::-:                          :::::::::::::::::-------:     "
			Escribir "                =%%+-:-=-:::::::::::::--:                            :::::::::::::::::--------      "
			Escribir "               ==-::-::        :::::::::--:                          :::::::::::::::::-------:      "
			Escribir "               :   ::::::       ::::::::::--                        ::::::::::::::::::-------       "
			Escribir "                      : :      : ::::::::::--:                     ::::::::::::::::::-------        "
			Escribir "                        ::::   ::::::::: ::::-:                   :: ::::::::-::::::----=--:        "
			Escribir "                       ::::::::::::::::::::::---                  ::::::::--+-::::::----=--:        "
			Escribir "                       :::::: ::::::::::::::::---                :::::----=*=::::::----==--         "
			Escribir "                       :::::::::::::::::::::::::--               :::::---++-:::::::----==-          "
			Escribir "                        ::::::::::::::--::::::::-=-             :::::--=+:::::::::---==--:          "
			Escribir "                         :::::::::::::-+-:::::---==:           :::::--==:  :-:::::--====-           "
			Escribir "                           ::::::::::--**-:::-----::          ::::---=+:   ::::::::-====-           "
			Escribir "                           ::::::::::-=++++=-----:::-:       ::-----++-    ::::::::-====-           "
			Escribir "                          ::::::::::--=++    -:::---==-     :::---==+=   :::::::---=====-           "
			Escribir "                          ::::::::::-=+=      :::----==-   ::-:----++::-=*-:::::--=+===-            "
			Escribir "                          :::::::::::--=       :-:----=+=--:::::--=+=--=*#=::::---=++=:             "
			Escribir "                           ::::::::::--=-       :----==+***-:::---++====+=:::::--=+*+-              "
			Escribir "                           ::::::::::---=-      :---===+*#*-:::-==--+**#+:::::--=+**=:              "
			Escribir "                            :::::::::::-:-=+*+===+=---=+=*##+=+++=-==*##-::::--==**+:               "
			Escribir "                              ::::::::::-=+#***#%%#=--=--=+##%%#*=--====-:::--==+*+:                "
			Escribir "                                ::::::::---==#%%%%#+===---=#%##*=-::---------=+**+-                 "
			Escribir "                                  :::::::::--=****+======+**###+-:----:----==++**-                  "
			Escribir "                                    :::::::---+======+======-------------====**+=                   "
			Escribir "                                      :::::---=-=-----=-==--:::-------==+*+=--:                     "
			Escribir "                                         ::----=======-==--------==++++=::                          "
			Escribir "                                          :---------::        :::::                                 "
			Escribir ""
		4: // Final termico: flash boil y destruccion total de la central.
			Escribir ""
			Escribir "==============================================="
			Escribir " Flash Boil"
			Escribir "==============================================="
			Escribir ""
			Escribir "La temperatura alcanzo el punto critico."
			Escribir "El hervor fue tan repentino que, el agua del sistema se evaporo de forma instantanea."
			Escribir "La expansion destruyo la central y provoco una explosion" Sin Saltar
			Escribir "devastadora que arraso el emplazamiento y la ciudad."
			Escribir "No se detectan sistemas operativos ni comunicaciones."
			Escribir ""
			Escribir "                                   ..:=*+--=====-:.                                         "
			Escribir "                               .+@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*                          "
			Escribir "                            :#@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@-                      "
			Escribir "                          :@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@-                    "
			Escribir "                     -@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%                   "
			Escribir "                 .%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*#@@-+@@@@-:*-               "
			Escribir "               -@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*+*+:#@@@@=:++:            "
			Escribir "              =@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@+-:------:+@=:  +@:        "
			Escribir "           .#@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%%@@@@@@@@@@@@@@@@*+=-----:+--.==%-      "
			Escribir "         .@@@@@@@%%%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%+**@@%@@@@@@@@@@@@-===:::+@#+   #%     "
			Escribir "       . -@@@@%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%@@@@@@@@@#*+*#@@@@@@@@@@@@# ------:.    -@+    "
			Escribir "     ....=@%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*=++===+*+=+*+**++=-*@@#:--    ..=#.    "
			Escribir "   ....:@@@@@@@@@@@@@@@@@@@%*##%@@%%%@@@@@@@@@@@@@@@@#=++=-%@@@@@@+:::-+#=:%@@#-.      .+@- "
			Escribir " ...: #@@@@@@@@@@@@@@@@@@#@@%@#%@%=:=#+-#@@@@@@@@@@@@@#=-+%@=-++--+%@#:=*=     +@+      .=%:"
			Escribir " ....:@@@@@@@@@@@@@@@@@@@%@*--:-:... :*#*#**#+%@@@@*     .:..:=+++-=#+@+.:=+#+  ...=*:. :@# "
			Escribir "  ... %@@@%@@@@@@@@@@@*#%#*++-=#@@@#+%+*+-++-.. :+=       :-:::-+**-*. -* ....#:    .. -%*  "
			Escribir "  ...::+@@@@@*+****#@#@@@@@%#%@%###**++==+***-   ++           ....:...+:.:.. .  :+-::-:+*+  "
			Escribir "  ....::::@@++*#%#+*+#@@%@@#%@*+#*=-:-::::::.   --                  ..:.--        ...  ==   "
			Escribir "    ...:::-@@@@@++++*%%@@#=-+%+-+#-:    ...   .     .+%+*+--#+-.:::       .        ::::.    "
			Escribir "      ....::::::-=-:+=--:.:++:      . .  ..:%*++*@@@@@@%%+-                                 "
			Escribir "         ...................            .  ::%@@@@%#@@@=.                                   "
			Escribir "           ......:::::::::::::::::::::::::-==*@#@@@@@@@*:::::::::::..........               "
			Escribir "       ...::::----===#*++@@@@@@@@@@@@@%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#-=+%*----:::::.... "
			Escribir "      ....::-+=-+++***%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#@@@##***-...."
			Escribir "          ..........:::::::-------===++=====+**#@@@@@@%========-+*=-----:::::::........     "
			Escribir "                               :::.  ....:...--=@@@@@@#:. ..       -=.                      "
			Escribir "                            =+++:           ::-*@@@@@@%.             .#.                    "
			Escribir "                        -=*.                :.+%@@@@@@+                =+                   "
			Escribir "                    .*=:*                   ..-@@@@@@@#                 =*                  "
			Escribir "     ....::::--:  +*  ==                    ..%%*@@@@@%.                -*     ..           "
			Escribir " ...::-==*%%#***@%-==#*=+*#%*-. .:.......::. -@=#@@@@@#.              :+%@@#*+=+=--::.      "
			Escribir "..:=+=+#@%###*%@+===*@%**+=-*@%%+==-....==-:.:@@@@@@@@@:.:...:........-*@@@@@%%%%%#**+-..=#*"
			Escribir ":::::=*#*%@@#@@%%#*+##*#@@@@@**+=:-%@#=.....%#*@@@@@@@@@@#=---+%*-=*+--+@@@@@@@%#**++=-:-=:-"
			Escribir "  -#**##%#*+@%+*+###@@@@@@%@@@@@@@@@=@@@-..%@@@@@@@@@@@@@@@@@@@@@#*@%#**#%#**#%@@@@%%#@*    "
			Escribir "        %%-@%:--=-=#%@@@@@@@##*%@@@@@@@@*=@@@@@@@@@@@@@@@@@@@@@*%%*****+-:+##%#*@+          "
			Escribir "        %@-@-:::..::-++*#%%%#*@*:..*@@@@@@@@@@@@@@@@@@@@@@@@@%@@%*=:.-=+==+++++-%+          "
			Escribir "        #*....:.........:::::#=:-=: .: :@@@@@@-+= .-+#@@@@@@@@%*=+===--:........*=          "
			Escribir "        +*...............   ++ .   :::..-===+#%.+@%++**##%%%%*++-=:  ... .:.....*-          "
			Escribir "        +=......     .     +%           :---:-=*=:.  ..:::--:::.     ....       =-          "
			Escribir "        =-                 +#                :::.          .      .             -:          "
			Escribir "        +-          :      =*  .                       ..            -          -:          "
			Escribir "        .-=-.       : ..         :         .   ..   .             .. =        :-=.          "
			Escribir ""
		5: // Final por sobrepresion: ruptura del reactor.
			Escribir ""
			Escribir "==============================================="
			Escribir " RUPTURA DE LA TAPA DEL REACTOR"
			Escribir "==============================================="
			Escribir ""
			Escribir "La presion supero el punto de no retorno."
			Escribir "El agua que circulaba dentro del reactor se transformo violentamente en vapor." 
			Escribir "La explosion desprendio la tapa " Sin Saltar
			Escribir "del reactor y dejo el nucleo expuesto a la atmosfera."
			Escribir "Material radiactivo y restos del nucleo fueron liberados " Sin Saltar
			Escribir "a la atmosfera y llevados por el viento a las ciudades y paises vecinos."
		6: // Final combinado: ruptura del reactor y del sistema de refrigeracion.
			Escribir ""
			Escribir "==============================================="
			Escribir " COLAPSO COMBINADO"
			Escribir "==============================================="
			Escribir ""
			Escribir "Tanto la temperatura como la presion entraron simultaneamente en zona critica."
			Escribir "La estructura del reactor y el sistema de refrigeracion sufrieron rupturas graves."
			Escribir "El agua circulante quedo contaminada."
			Escribir "La evaporacion libero contaminantes radiactivos al ambiente; "Sin Saltar
			Escribir "nubes toxicas se desplazaron sobre la ciudad y sus alrededores, mientras que los "Sin Saltar
			Escribir "daños estructurales dispersaron restos y material contaminado."
			Escribir "La central quedo inutilizada y la region gravemente contaminada."
			Escribir ""
			Escribir ""
		De Otro Modo:
			Escribir "Final fuera de rango"
	FinSegun
FinFuncion	

// SKALA: solo observa variables y emite recomendaciones. Nunca modifica el estado del reactor.
// Presion: umbrales de juego provisionales, basados en el rango operativo previsto de 40-120 bar.
// El final termico se activa a 850 grados C y se evalua al terminar cada turno.
Funcion huboAlerta <- MostrarAlertasSKALA(encendido, temp, presion, React, pctAgua)
	Definir huboAlerta Como Logico
	huboAlerta <- Falso
	// Temperatura y presion se supervisan aunque el reactor este apagado: pueden seguir elevadas.
	Si temp >= 800 Entonces
		Escribir " *** SKALA - ALERTA CRITICA DE TEMPERATURA ***"
		Escribir " Temperatura: ", redon(temp), " grados C. Zona critica alcanzada."
		Escribir " Inserte barras de control y aumente la circulacion de agua."
		Escribir " El apagado del reactor es el ultimo recurso."
		huboAlerta <- Verdadero
	SiNo
		Si temp >= 700 Entonces
			Escribir " *** SKALA - ALERTA DE INCIDENTE TERMICO ***"
			Escribir " Temperatura: ", redon(temp), " grados C. Se registra un incidente termico."
			Escribir " Inserte barras de control y aumente la circulacion de agua."
			Escribir " El apagado del reactor es el ultimo recurso."
			huboAlerta <- Verdadero
		SiNo
			Si temp >= 600 Entonces
				Escribir " *** SKALA - ADVERTENCIA PREVENTIVA DE TEMPERATURA ***"
				Escribir " Temperatura: ", redon(temp), " grados C (rango preventivo: 600-699)."
				Escribir " Revise la reactividad, inserte barras si es elevada y aumente el agua."
				huboAlerta <- Verdadero
			FinSi
		FinSi
	FinSi
	Si presion >= 120 Entonces
		Escribir " *** SKALA - ALERTA CRITICA DE PRESION ***"
		Escribir " Presion: ", redon(presion*10)/10, " bar. Umbral critico del simulador alcanzado."
		Escribir " Abra la valvula para aliviar presion si corresponde; el apagado es el ultimo recurso."
		huboAlerta <- Verdadero
	SiNo
		Si presion > 100 Entonces
			Escribir " *** SKALA - ADVERTENCIA PREVENTIVA DE PRESION ***"
			Escribir " Presion: ", redon(presion*10)/10, " bar. Revise la valvula de vapor."
			huboAlerta <- Verdadero
		FinSi
	FinSi
	Si encendido = Verdadero y React > 0.33 Entonces
		Escribir " *** SKALA - ADVERTENCIA DE REACTIVIDAD ELEVADA ***"
		Escribir " Reactividad: ", redon(react*1000)/1000, " (umbral: > 0,25). Inserte barras de control."
		huboAlerta <- Verdadero
	FinSi
	Si pctAgua < 40 y temp > 600 Entonces
		Escribir " *** SKALA - ALERTA DE REFRIGERACION INSUFICIENTE ***"
		Escribir " Agua: ", pctAgua, " % con temperatura superior a 600 grados C."
		Escribir " Aumente la circulacion de agua inmediatamente."
		Escribir " El apagado del reactor es el ultimo recurso."
		huboAlerta <- Verdadero
	FinSi
	Si huboAlerta = Falso Entonces
		Escribir " SKALA: sin alertas en las variables supervisadas."
	FinSi
FinFuncion

// Detecta el final catastrofico segun temperatura y presion. No muestra mensajes ni modifica el reactor.
// Codigos: 0 = sin catastrofe; 4 = termica; 5 = sobrepresion; 6 = combinada.
Funcion catastrofe <- CheckeoCatastrofes(tempActual, presActual)
	Definir catastrofe Como Entero
	catastrofe <- 0
	Si tempActual >= 850 Entonces
		catastrofe <- 4
	SiNo
		Si presActual > 125 Entonces
			catastrofe <- 5
		SiNo
			Si tempActual >= 650 y tempActual <= 799 y presActual >= 100 y presActual <= 119 Entonces
				catastrofe <- 6
			FinSi
		FinSi
	FinSi
FinFuncion
