/*
 Name:		Testes.ino
 Created:	5/28/2017 10:13:10 PM
 Author:	paulo
*/

//#include <SimpleTimer.h>
#include <LiquidCrystal.h>
#include <Servo.h>

// LCD
LiquidCrystal lcd(12, 8, 5, 4, 3, 2);
int intervaloLcd = 1000;
unsigned long tempoLcd;

// Temporizados
unsigned long agora = 0;

// Iluminação
int lampada = 6;
int sensorIluminacao = A2;
int valorSensorIluminacao;
bool lampadaLigada = false;
int intervaloLampada = 5000;
unsigned long tempoIluminar = 0;

// PH
int sensorPH = A0;
Servo servoPh;
bool phLigado = false;
int valorSensorPH;

// Irrigador
int sensorUmidade = A1;
int irrigador = 9;
bool irrigadorLigado = false;
int valorSensorUmidade;

void setup()
{
	pinMode(lampada, OUTPUT);
	pinMode(irrigador, OUTPUT);

	pinMode(sensorPH, INPUT);
	pinMode(sensorUmidade, INPUT);
	pinMode(sensorIluminacao, INPUT);

	servoPh.attach(7);

	Serial.begin(9600);

	lcd.begin(16, 2);
}

void Iluminar()
{
	if (tempoIluminar + intervaloLampada <= agora)
	{
		tempoIluminar = agora;

		lampadaLigada = !lampadaLigada;

		digitalWrite(lampada, lampadaLigada);
	}
}

void ChecarIluminacao()
{
	valorSensorIluminacao = analogRead(sensorIluminacao);

	if (valorSensorIluminacao <= 5)
	{
		// Disparar aviso que a lâmpada queimou
	}
}

void Irrigar()
{
	valorSensorUmidade = analogRead(sensorUmidade);

	if (valorSensorUmidade >= 300)
	{
		if (!irrigadorLigado)
		{
			irrigadorLigado = true;

			digitalWrite(irrigador, HIGH);
		}
	}
	else
	{
		if (irrigadorLigado)
		{
			irrigadorLigado = false;

			digitalWrite(irrigador, LOW);
		}
	}
}

void PH()
{
	valorSensorPH = analogRead(sensorPH);

	if (valorSensorPH >= 300)
	{
		if (!phLigado)
		{
			phLigado = true;

			servoPh.write(255);
		}
	}
	else
	{
		if (phLigado)
		{
			phLigado = false;

			servoPh.write(0);
		}
	}
}

void ExibirInformacoesLcd()
{
	if (tempoLcd + intervaloLcd < agora)
	{
		tempoLcd = agora;

		lcd.clear();

		String linha1 = "PH: ";
		linha1.concat(valorSensorPH);
		linha1.concat(" Umid: ");
		linha1.concat(valorSensorUmidade);

		String linha2 = "Luz: ";
		linha2.concat(lampadaLigada ? "1" : "0");
		linha2.concat(" Ssr: ");
		linha2.concat(valorSensorIluminacao);

		lcd.setCursor(0, 0);
		lcd.print(linha1);
		lcd.setCursor(0, 1);
		lcd.print(linha2);
	}
}

void loop()
{
	agora = millis();

	Iluminar();
	ChecarIluminacao();

	Irrigar();

	PH();

	ExibirInformacoesLcd();

	delay(100);
}
