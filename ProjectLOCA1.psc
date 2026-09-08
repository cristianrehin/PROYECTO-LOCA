Proceso proyectLoca01
	Definir estasVivo, reactorP Como Logico // estasVivo es el logico que controla que el juego se repita turno a turno
	Definir opc, demanda, turno, dia, incidentes, cantEnAct Como Entero
	Definir temperatura, presion, vapor Como Entero
	
	// Variables:
	// Temperatura: temperatura del reactor.
	// Agua: cant. de agua en el reactor.
	// Presion: presion dentro del reactor.
	// Vapor: vapor que sale del reactor.
	// Potencia: energia generada en el dia de la misma demanda.
	// Barras de control: valores entre 0 y 11, su porcentaje va a ir hasta 110% pero el jugador verá solo 100%, cada valor equivale 10%
	// Reactividad: cantidad de energia que genera los combustibles individualmente.
	// Zenon: reduce la reactividad del reactor, se acumula cuando se genera poca energia.
	//  
	// Opc es la accion que selecciona el jugador
	// Demanda es la cantidad de energia a generar para satisfacer la demanda
	// Turno es la jornada laboral, mañana, tarde y noche, en la que dividimos el dìa
	// y en la que el jugador podrà tomar accion
	// Dia es el dia en que se da la accion
	
	// Turno a turno la Demanda ya generada, deberìa tener una fluctuacìon menor
	// por ejemplo, un pequeño aumento de la demanda en la tarde y un aumento mayor en la noche
	// pero en el cambio de dia, deberiamos ver reiniciarse la demanda de energia
	// pero el jugador debe afrontar las consecuencias de la jornada anterior
	estasVivo <- Verdadero
	reactorP <- Falso
	turno <- 0
	dia <- 0
	incidentes <- 0
	temperatura <- 0
	presion <- 0
	vapor <- 0
	cantFuelActivo = 0
	demanda <- crearDemanda
	
	Repetir
		opc <- 0
		
		Mientras opc <> 6 Hacer
			Escribir "------------------- | La demanda generada por la ciudad es: " demanda " MW."
			MostrarMonitor(turno,dia,incidentes)
			Escribir "------------------- | "
			MostrarPanel(cantFuelActivo)
			Escribir "Ingrese una opcion"
			
			mostrarMenu
			leer opc
			
			segun opc Hacer
				1:
					
					// Iniciar el reactor, en teoria, deberia ser algo que solamente veriamos en el primer turno
					// y luego deberiamos cambiarlo por una opcion que nos permita manipular directamente las barras de combustible
					Si reactorP = Falso Entonces
						Escribir " Iniciando reactor"
						Escribir " ...Presione Enter para continuar..."
						Escribir " ";
						reactorP = Verdadero
					SiNo
						Escribir " Reactor ya iniciado."
						Escribir " ...Presione Enter para continuar..."
						Escribir " ";
					Fin Si
					Esperar Tecla
					Limpiar Pantalla
				2:
					
					Escribir " Ingrese la cantidad de barras de control."
					cantFuelActivo = ObtnrBarrasDeControl
					Escribir " ...Presione Enter para continuar..."
					Escribir " ";
					Esperar Tecla
					Limpiar Pantalla
				3:
					
					Escribir " Abriendo Valvulas"
					Escribir " ...Presione Enter para continuar..."
					Escribir " ";
					Escribir " Decrece la cantidad de Vapor"
					Escribir " "
					Esperar Tecla
					Limpiar Pantalla
				4:
					
					Escribir " Subiendo niveles de agua"
					Escribir " ...Presione Enter para continuar..."
					Escribir " ";
					Escribir " Aumenta la cantidad de refrigerante."
					Esperar Tecla
					Limpiar Pantalla
				5:
					
					Escribir " Terminar turno"
					Escribir " ...Presione Enter para continuar..."
					Escribir " ";
					turno <- pasarTurnos(turno)
					dia <- pasarDia(turno, dia)
					incidentes <- dia
					Esperar Tecla
					Limpiar Pantalla
					opc <- 6
				6:
					
					Escribir " Apagando Reactor"
					Escribir " ...Presione Enter para continuar..."
					Escribir " ";
					Esperar Tecla
					Limpiar Pantalla
					reactorP <- Falso
					estasVivo <- Falso
					Escribir "Termino el juego"
				De Otro Modo:
					Limpiar Pantalla
					Escribir "Ingrese una opcion valida"
					Escribir ""
			FinSegun			
		FinMientras		
Hasta Que estasVivo = Falso
FinProceso

Funcion d <- ObtnrBarrasDeControl
	Definir d Como Entero
	Leer d
FinFuncion

SubProceso mostrarMenu
	Escribir "1... Iniciar reactor"
	Escribir "2... Panel de barras de control"
	Escribir "3... Abrir valvulas"
	Escribir "4... Subir niveles de agua"
	Escribir "5... Terminar turno"
	Escribir "6... Apagar reactor"
FinSubProceso

SubProceso MostrarMonitor(t,d,inc)
	Escribir "Dias Sin Incidentes | El dia es: " d
	Segun t Hacer
		0:
			Escribir "        ",inc,"           | Turno Mañana."
		1:
			Escribir "        ",inc,"           | Turno Tarde."
		2:
			Escribir "        ",inc,"           | Turno Noche."
	Fin Segun
FinFuncion

funcion demanda <- crearDemanda
	Definir demanda Como Entero
	demanda <- aleatorio(50, 200)
FinFuncion

funcion turno <- pasarTurnos(turno)
	turno <- turno + 1
	Si turno >= 3 Entonces
		turno <- 0
	FinSi
FinFuncion

Funcion dia <- pasarDia(turno, dia)	
	si turno % 3 = 0 Entonces
		dia <- dia + 1
	FinSi
FinFuncion

SubProceso MostrarPanel(cantFuelActivo)
	Escribir ""
	Definir cont,matriz,FILAS,COLUMNAS,i,j,k Como Entero
	FILAS = 3
	COLUMNAS = 5
	Dimension matriz[FILAS,COLUMNAS]
	cont = 0
	Para i=0 Hasta FILAS-1 Con Paso 1 Hacer
		Para j=0 Hasta COLUMNAS-1 Con Paso 1 Hacer
			Si i = 1 Entonces
				matriz[i,j] = 1
			SiNo
				Si j = 0 o j = 4 Entonces
					matriz[i,j] = 0
				SiNo
					matriz[i,j] = 1
				Fin Si
			Fin Si
		Fin Para
	Fin Para
	
	Para i=0 Hasta FILAS-1 Con Paso 1 Hacer
		Para k=0 Hasta 3-1 Con Paso 1 Hacer
			Para j=0 Hasta COLUMNAS-1 Con Paso 1 Hacer
				Si matriz[i,j] = 0 Entonces
					Escribir "         " Sin Saltar
				SiNo
					Si cont < cantFuelActivo Entonces
						Segun k Hacer
							0:
								Escribir "  /***\  " Sin Saltar
							1:
								Escribir " (  X  ) " Sin Saltar
								cont <- cont + 1
							2:
								Escribir "  \___/  " Sin Saltar
							De Otro Modo:
						Fin Segun
					SiNo
						Segun k Hacer
							0:
								Escribir "  /***\  " Sin Saltar
							1:
								Escribir " (     ) " Sin Saltar
								cont = cont + 1
							2:
								Escribir "  \___/  " Sin Saltar
							De Otro Modo:
						Fin Segun
					Fin Si
				Fin Si
			Fin Para
			Escribir ""
		Fin Para
	Fin Para
	Escribir ""
FinSubProceso
	