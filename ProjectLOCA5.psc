Proceso proyectLoca01
	Definir estasVivo, reactorP, vapor Como Logico // estasVivo es el logico que controla que el juego se repita turno a turno
	Definir opc, demanda, turno, dia, contIncidentes, incidentes, cantEnAct Como Entero
	Definir agua, pot, vTurb, barras,Egenerada Como Entero
	definir temp, React, xe, pres Como Real
	// Variables:
	// Agua: cant. de agua en el reactor.										[agua]
	// Temperatura: temperatura del agua en °C.									[temp]
	// Presion: presion dentro del reactor.										[pres] Depende de agua y temp
	// Valvula de Vapor: vapor que sale del reactor.							[vapor]
	// Potencia: energia generada en el dia de la misma demanda.				[pot] Depende de la 'vTurb', de la 'pres' y de 'vapor'
	// Velocidad de la Turbina: para calcular energia generada.					[vTurb]
	
	// Barras de control: valores entre 0 y 11, su porcentaje va a ir hasta 110% pero el jugador verá solo 100%, cada valor equivale 10%
	// Reactividad: cantidad de energia que genera los combustibles individualmente.
	// Zenon: reduce la reactividad del reactor, se acumula cuando se genera poca energia.
	
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
	temp <- 0
	pres <- 1
	barras <- 0
	agua <- 1
	contIncidentes <- 0
	pot <- 0
	vTurb <- 0
	demanda <- crearDemanda
	vapor <- Falso
	xe<-Aleatorio(1,100)/100
	//Fin de la inicializacion de las variables.
	
	Repetir
		opc <- 0		
		Mientras opc <> 6 Hacer
			Escribir "------------------- | La demanda generada por la ciudad es: " demanda " MW."
			MostrarMonitor(turno,dia,incidentes) 
			Escribir "------------------- | "
			MostrarPanel(barras)
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
						reactorP <- Verdadero
						React<-Reactividad(agua,barras,xe)
						Mostrar " la reactividad es de " React
						temp<-Temperatura(react,agua,temp)
						mostrar "la temperatura es de " temp " grados"
						mostrar "el nivel de agua es " agua
						pres<-presion(temp,agua,pres,vapor)
						mostrar "la presion es de " pres " bar"
						vturb<-velocidadturbina(pres,vapor)
						Mostrar vturb
						Egenerada<-EnergiaGenerada(vturb)
						Mostrar "la energia generada es de " Egenerada  " MWH"
					SiNo
						Escribir " Reactor ya iniciado."
						Escribir " ...Presione Enter para continuar..."
						Escribir " ";
						React<-Reactividad(agua,barras,xe)
						Mostrar " la reactividad es de " React
						temp<-Temperatura(react,agua,temp)
						mostrar "la temperatura es de " temp " grados"
						mostrar "el nivel de agua es " agua
						pres<-presion(temp,agua,pres,vapor)
						mostrar "la presion es de " pres " bar"
						vturb<-velocidadturbina(pres,vapor)
						Mostrar vturb
						Egenerada<-EnergiaGenerada(vturb)
						Mostrar "la energia generada es de " Egenerada  " MWH"
					Fin Si
					Esperar Tecla
					Limpiar Pantalla
				2:
					
					Escribir " Ingrese la cantidad de barras de control."
					barras <- ObtnrBarrasDeControl
					Escribir " ...Presione Enter para continuar..."
					Escribir " ";
					Esperar Tecla
					Limpiar Pantalla
				3:
					
					Si vapor = Falso Entonces
						Escribir " Abriendo Valvulas"
						Escribir " ...Presione Enter para continuar..."
						Escribir " ";
						Escribir " Decrece la cantidad de Vapor"
						Escribir " "
						Esperar Tecla
						Limpiar Pantalla
						vapor <- Verdadero
					SiNo
						Escribir " Cerrando Valvulas"
						Escribir " ...Presione Enter para continuar..."
						Escribir " ";
						Escribir " Decrece la cantidad de Vapor"
						Escribir " "
						Esperar Tecla
						Limpiar Pantalla
						vapor <- Falso
					Fin Si
				4:
					
					Escribir " Ingrese el nivel de agua en el reactor"
					Leer agua
					Escribir " ...Presione Enter para continuar..."
					Escribir " ";
					Esperar Tecla
					Limpiar Pantalla
					// Esta opcion hecha funcion, agregar un "si" para mostrar si la cantidad de agua subio o bajo con el nuevo valor ingresado - Tener limite
				5:
					
					Escribir " Terminar turno"
					Escribir " ...Presione Enter para continuar..."
					Escribir " ";
					turno <- pasarTurnos(turno)
					dia <- pasarDia(turno, dia)
					contIncidentes <- contIncidentes+1
					incidentes <- trunc((contIncidentes)/3)
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
	Escribir "3... Abrir/Cerrar valvula de vapor"
	Escribir "4... Nivel de agua"
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

Funcion React <- Reactividad ( agua, barras, xe)
	React<-agua/100*(0.5-barras/11)*(1-xe)
FinFuncion
Funcion Temp<-Temperatura(react,agua,temp)
	Temp<- temp+(100*react)-(1-agua/100)
FinFuncion
funcion pres<-presion(temp,agua,pres,vapor)
	si vapor==Verdadero
		definir vp como entero
		vp<--1
	sino
		vp<-1
	FinSi
	pres<-pres+((temp/agua)*vp)
FinFuncion
funcion vturb<-velocidadturbina(pres,vapor)
	si vapor==Verdadero
		si pres>10 y pres<60
			si pres>15
				si pres>27
					si pres>38
						si pres>50
							vturb<-3000
						sino 
							vturb<-2250	
						FinSi
						
					sino 
						vturb<-1500	
					FinSi
				sino 
					vturb<-750
				FinSi
			sino 
				vturb<-450
			FinSi
		FinSi
	FinSi
FinFuncion
Funcion Egenerada<-EnergiaGenerada(vturb)
	Segun vturb Hacer
		450:
			Egenerada=86
		750:
			Egenerada=140
		1500:
			Egenerada=206
			
		2250:
			Egenerada=263
		3000:
			Egenerada=326
		de Otro Modo:
			Egenerada=0
	Fin Segun
FinFuncion
	