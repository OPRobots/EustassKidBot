#include "Servo.h"

/////////////////
// PINES MOTOR //
/////////////////
#define MOTOR_ENABLE 2
#define MOTOR_ADELANTE 4
#define MOTOR_ATRAS 5
#define MOTOR_PWM 11
#define MOTOR_CONSUMO A7

//////////////////
// CONFIG SERVO //
//////////////////
Servo servoBrazo;
#define SERVO_MANO 6
#define SERVO_DEFAULT 160 // Ángulo inicial para armado de mano.
#define SERVO_LANZAR 23 // Ángulo objetivo cuando se da orden de iniciar asalto.

#define TIMEOUT_CONSUMO 250 // Millis entre cada impresión de consumo
#define TIMEOUT_SERVO 4850 // Millis desde que se recibe orden de iniciar y el momento en el que se mueve el servo para liberar la mano
#define TIMEOUT_MOTOR 5000 // Millis desde que se recibe orden de iniciar y el motor empieza a andar a 255

bool iniciado = false;
long tiempoIniciado = 0;
long tiempoConsumo = 0;

void setup() {
  Serial.begin(9600);

  servoBrazo.attach(SERVO_MANO);
pinMode(13, OUTPUT);
  pinMode(MOTOR_ENABLE, OUTPUT);
  digitalWrite(MOTOR_ENABLE, LOW);
  pinMode(MOTOR_ADELANTE, OUTPUT);
  digitalWrite(MOTOR_ADELANTE, HIGH);
  pinMode(MOTOR_ATRAS, OUTPUT);
  digitalWrite(MOTOR_ADELANTE, LOW);
  pinMode(MOTOR_PWM, OUTPUT);
  analogWrite(MOTOR_PWM, 0);
}

void loop() {
  if (Serial.available() > 0) {
    char letra = (char)Serial.read();
    if (letra == 's') {
      iniciado = !iniciado;
      if (iniciado) {
          digitalWrite(13, HIGH);
        tiempoIniciado = millis();
      } else {
        tiempoIniciado = 0;
          digitalWrite(13, LOW);
      }
    }
  }

  if (iniciado) {
    if ((millis() - tiempoIniciado) >= TIMEOUT_SERVO) {
      servoBrazo.write(SERVO_LANZAR);
    }
    if ((millis() - tiempoIniciado) >= TIMEOUT_MOTOR) {
      digitalWrite(MOTOR_ENABLE, HIGH);
      digitalWrite(MOTOR_ADELANTE, HIGH);
      analogWrite(MOTOR_PWM, 255);
      digitalWrite(13, LOW);
    }
    if ((millis()-tiempoConsumo) >= TIMEOUT_CONSUMO) {
      Serial.println(analogRead(MOTOR_CONSUMO));
      tiempoConsumo = millis();
    }
  } else {
    digitalWrite(MOTOR_ENABLE, LOW);
    analogWrite(MOTOR_PWM, 0);
    servoBrazo.write(SERVO_DEFAULT);
  }
}