# Smart Irrigation System

## Descriere

Proiect realizat folosind ATmega328P pentru monitorizarea umidității solului și controlul automat al unei pompe de apă.

## Funcționalități

* Citire senzor umiditate sol
* Citire senzor nivel apă
* Pornire automată pompă
* Protecție la lipsa apei
* Alarmă cu buzzer
* Afișare pe LCD 16x2
* Comunicare USART
* Interfata grafica cu Python pentru monitorizare
* Mod AUTO și MANUAL

## Componente utilizate

* Arduino R3 / ATmega328P
* Pompă apă + releu
* Senzor umiditate sol
* Senzor nivel apă
* LCD 16x2
* Buzzer
* Butoane control

## Structură proiect

src/
├── main.c
├── pump.c
├── pump.h
├── buzzer.c
├── buzzer.h
├── sensors.c
└── sensors.h

